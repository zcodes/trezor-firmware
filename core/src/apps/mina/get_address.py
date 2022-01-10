from apps.common.keychain import with_slip44_keychain
from apps.common import seed
from trezor.messages import MinaAddress

from . import CURVE, PATTERNS, SLIP44_ID
from .helpers import address_from_public_key

@with_slip44_keychain(*PATTERNS, slip44_id=SLIP44_ID, curve=CURVE)
async def get_address(ctx, msg, keychain):
    node = keychain.derive(msg.address_n)
    priv_key = node.private_key()
    print(priv_key)
    pubkey = seed.remove_ed25519_prefix(node.public_key())
    print(pubkey)
    address = address_from_public_key(pubkey)
    print(address)
    # print(pubkey)
    # address = "abcdef"
    return MinaAddress(address=address)
