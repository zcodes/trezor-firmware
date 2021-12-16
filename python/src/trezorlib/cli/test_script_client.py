import subprocess
import os


def get_address():
    args = """
        trezorctl --script get-address -n "m/49'/0'/0'/0/0"
    """.strip()
    p = subprocess.Popen(
        args,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        shell=True,
    )

    new_line = ""
    while "Please enter" not in new_line:
        new_line = p.stdout.readline()
        print(new_line, end="")

    pin = input()

    p.stdin.write(pin)

    stdout, stderr = p.communicate()
    print("result", stdout.strip())


def clear_session_to_enable_pin():
    args = """
        trezorctl clear-session
    """.strip()
    os.system(args)


if __name__ == "__main__":
    get_address()
    clear_session_to_enable_pin()
