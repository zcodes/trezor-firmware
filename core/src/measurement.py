from trezor import log
from trezor.crypto import bip32, bip39, slip39
import utime


def run():
    measure(cardano_128)
    measure(cardano_256)
    measure(bip39_128)
    measure(bip39_256)
    measure(slip39_128)
    measure(slip39_256)


def measure(function, iterations=10):
    start = utime.ticks_ms()
    for _ in range(iterations):
        function()
    end = utime.ticks_ms()
    time = (end - start) / iterations
    log.info(__name__, "{:20} {:>5} ms".format(function.__name__, time))


mnemonic_bip39_128_bits = (
    "legal winner thank year wave sausage worth useful legal winner thank yellow"
)
mnemonic_bip39_256_bits = "legal winner thank year wave sausage worth useful legal winner thank year wave sausage worth useful legal winner thank year wave sausage worth title"
mnemonic_slip39_128_bits = "duckling enlarge academic academic agency result length solution fridge kidney coal piece deal husband erode duke ajar critical decision keyboard"
mnemonic_slip39_256_bits = "theory painting academic academic armed sweater year military elder discuss acne wildlife boring employer fused large satoshi bundle carbon diagnose anatomy hamster leaves tracks paces beyond phantom capital marvel lips brave detect luck"
passphrase = ""


def cardano_128():
    bip32.from_mnemonic_cardano(mnemonic_bip39_128_bits, passphrase)


def cardano_256():
    bip32.from_mnemonic_cardano(mnemonic_bip39_256_bits, passphrase)


def bip39_128():
    bip39.seed(mnemonic_bip39_128_bits, passphrase)


def bip39_256():
    bip39.seed(mnemonic_bip39_256_bits, passphrase)


def slip39_128():
    identifier, iteration_exponent, encrypted_master_secret = slip39.recover_ems(
        [mnemonic_slip39_128_bits]
    )
    slip39.decrypt(encrypted_master_secret, passphrase, iteration_exponent, identifier)


def slip39_256():
    identifier, iteration_exponent, encrypted_master_secret = slip39.recover_ems(
        [mnemonic_slip39_256_bits]
    )
    slip39.decrypt(encrypted_master_secret, passphrase, iteration_exponent, identifier)
