from trezorcrypto import mina

def get_public_key(priv_key: bytes) -> str:
    pass

def address_from_public_key(pubkey: bytes) -> str:
    return mina.get_address(pubkey)
