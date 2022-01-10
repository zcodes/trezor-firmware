from apps.common.paths import PATTERN_SEP5, PATTERN_BIP44

CURVE = "secp256k1"
SLIP44_ID = 12586

PATTERNS = (
    PATTERN_SEP5,
    PATTERN_BIP44,
    "m/44'/coin_type'/account'/0'/0'",  # NanoWallet compatibility
)
