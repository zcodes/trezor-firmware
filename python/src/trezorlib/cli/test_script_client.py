import subprocess
import os


def get_address() -> str:
    args = """
        trezorctl --script get-address -n "m/49'/0'/0'/0/0"
    """.strip()
    p = subprocess.Popen(
        args,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        text=True,
        shell=True,
        bufsize=0
    )

    result_address = ""
    while True:
        line = p.stdout.readline()
        if not line:
            break
        print(line, end="")
        if "Please enter" in line:
            user_input = input()
            p.stdin.write(user_input + "\n")

        # Address will appear as the very last line
        result_address = line

    print("Address:", result_address)
    return result_address


def clear_session_to_enable_pin():
    args = """
        trezorctl clear-session
    """.strip()
    os.system(args)


if __name__ == "__main__":
    get_address()
    clear_session_to_enable_pin()
