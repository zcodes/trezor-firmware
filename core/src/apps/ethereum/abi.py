
def abi_encode_single(type_name, arg) -> bytes:
    if type_name is "address":
        return abi_encode_single("uint160", parse_number(arg))
    elif type_name is "bool":
        return abi_encode_single("uint256", 1 if arg is True else 0)
    elif type_name is "string":
        return abi_encode_single("bytes", bytes(arg, encoding="utf8"))
    elif is_array(type_name):
        # this part handles fixed-length ([2]) and variable length ([]) arrays
        if not isinstance(arg, list):
            raise ValueError("not an array")

        size = parse_array_n(type_name)
        if not (size is "dynamic") and not (size is 0) and len(arg) > size:
            raise ValueError("elements exceed array size: %d" % size)

        ret = []
        type_name = type_name[:type_name.rindex('[')]
        for item in arg:
            ret.append(abi_encode_single(type_name, item))

        if size is "dynamic":
            ret = [abi_encode_single("uint256", len(arg))] + ret

        return b"".join(ret)
    elif type_name is "bytes":
        ret = bytearray(0)
        ret.extend(abi_encode_single("uint256", len(arg)))
        ret.extend(arg)

        if not (len(arg) % 32 is 0):
            zeros_padding = bytearray(32 - (len(arg) % 32))
            ret = b"".join([ret, zeros_padding])

        return ret
    elif type_name.startswith("bytes"):
        size = parse_type_n(type_name)
        if size < 1 or size > 32:
            raise ValueError("invalid bytes<N> width: %d" % size)
        if not (isinstance(arg, bytes) or isinstance(arg, bytearray)):
            raise ValueError("arg for bytes is not bytes")

        return bytearray(set_length_right(arg, 32))
    elif type_name.startswith("uint"):
        size = parse_type_n(type_name)

        if (not size % 8 is 0) or (size < 8) or (size > 256):
            raise ValueError("invalid uint<N> width: %d" % size)

        num = parse_number(arg)
        if num < 0:
            raise ValueError("supplied uint is negative")

        return num.to_bytes(length=32, byteorder="big")
    elif type_name.startswith("int"):
        size = parse_type_n(type_name)

        if (not size % 8 is 0) or (size < 8) or (size > 256):
            raise ValueError("invalid int<N> width: %d" % size)

        num = parse_number(arg)
        return num.to_bytes(length=32, byteorder="big", signed=True)

    raise ValueError("unsupported or invalid type: %s" % type_name)


def abi_encode(types: list, values: list) -> bytearray:
    output = []
    data = []
    head_len = 0

    for type_name in types:
        if is_array(type_name):
            size = parse_array_n(type_name)
            if not (size is "dynamic"):
                head_len += 32 * size
            else:
                head_len += 32
        else:
            head_len += 32

    for i in range(0, len(types)):
        type_name = types[i]
        value = values[i]
        buf = abi_encode_single(type_name, value)

        if isinstance(buf, bytes):
            raise ValueError("encoded {} with {} as bytes! HALT".format(type_name, value))

        # use the head/tail method for storing dynamic data
        if is_dynamic(type_name):
            output.append(abi_encode_single("uint256", head_len))
            data.append(buf)
            head_len += len(buf)
        else:
            output.append(buf)

    res = bytearray(0)
    for x in output + data:
        res.extend(x)

    return res



def set_length_right(msg: bytes, length) -> bytes:
    """
    Pads a `msg` with zeros till it has `length` bytes.
    Truncates the end of input if its length exceeds `length`.
    """
    if len(msg) < length:
        buf = bytearray(length)
        buf[:len(msg)] = msg
        return buf

    return msg[:length]


def parse_type_n(type_name):
    """Parse N from type<N>"""
    accum = []
    for c in type_name:
        if c.isdigit():
            accum.append(c)
        else:
            accum = []

    # join collected digits into a number
    return int("".join(accum))


def parse_number(arg):
    if isinstance(arg, str):
        return int(arg, 16)
    elif isinstance(arg, int):
        return arg

    raise ValueError("arg is not a number")


def is_array(type_name: str) -> bool:
    if type_name:
        return type_name[len(type_name) - 1] == ']'

    return False


def typeof_array(type_name) -> str:
    return type_name[:type_name.rindex('[')]


def parse_array_n(type_name: str):
    """Parse N in type[<N>] where "type" can itself be an array type."""
    if type_name.endswith("[]"):
        return "dynamic"

    start_idx = type_name.rindex('[')+1
    end_idx = len(type_name) - 1

    return int(type_name[start_idx:end_idx])


def is_dynamic(type_name: str) -> bool:
    if type_name is "string":
        return True
    elif type_name is "bytes":
        return True
    elif is_array(type_name) and parse_array_n(type_name) is "dynamic":
        return True

    return False


def signed_int(val, bits):
    if type(val) is bytes:
        val = int.from_bytes(val, "big")
    elif type(val) is str:
        val = int(val, 16)
    if (val & (1 << (bits - 1))) != 0:
        val = val - (1 << bits)
    return val

