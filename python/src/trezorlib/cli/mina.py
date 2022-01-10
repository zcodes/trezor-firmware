from typing import TYPE_CHECKING

import click

from .. import mina, tools
from . import with_client

if TYPE_CHECKING:
    from ..client import TrezorClient

PATH_HELP = "BIP-32 path, e.g. m/44'/134'/0'/0'"

@click.group(name="mina")
def cli() -> None:
    """Mina Commands"""

@cli.command()
@click.option("-n", "--address", required=True, help=PATH_HELP)
@click.option("-N", "--network", type=int, default=0x01)
@click.option("-d", "--show-display", is_flag=True)
@with_client
def get_address(
    client: "TrezorClient", address: str, network: int, show_display: bool
) -> str:
    """Get NEM address for specified path."""
    address_n = tools.parse_path(address)
    return mina.get_address(client, address_n, network, show_display)
