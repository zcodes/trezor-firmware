from typing import TYPE_CHECKING

from . import messages
from .tools import expect

if TYPE_CHECKING:
    from .client import TrezorClient
    from .tools import Address
    from .protobuf import MessageType

@expect(messages.MinaAddress, field="address", ret_type=str)
def get_address(
    client: "TrezorClient", n: "Address", network: int, show_display: bool = False
) -> "MessageType":
    return client.call(
        messages.MinaGetAddress(address_n=n, network=network, show_display=show_display)
    )
