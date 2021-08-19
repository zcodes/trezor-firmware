from trezor import wire, workflow
from trezor.enums import ButtonRequestType
from trezor.messages import ButtonAck, ButtonRequest

if False:
    from typing import Any, Awaitable, Optional, Tuple, Type, Union

    LayoutType = Awaitable[Any]
    PropertyType = Tuple[Optional[str], Union[str, bytes, None]]
    ExceptionType = Union[BaseException, Type[BaseException]]


if __debug__:
    from ..components.tt.scroll import Paginated


async def button_request(
    ctx: wire.GenericContext,
    name: str,
    code: ButtonRequestType = ButtonRequestType.Other,
    index: int | None = None,
    pages: int | None = None,
) -> None:
    workflow.close_others()
    await ctx.call(
        ButtonRequest(code=code, name=name, index=index, pages=pages), ButtonAck
    )


async def interact(
    ctx: wire.GenericContext,
    layout: LayoutType,
    name: str,
    *,
    br_code: ButtonRequestType = ButtonRequestType.Other,
    index: int | None = None,
) -> Any:
    if layout.__class__.__name__ == "Paginated":
        assert isinstance(layout, Paginated)
        return await layout.interact(ctx, name, code=br_code, index=index)
    else:
        await button_request(ctx, name, br_code, index=index)
        return await ctx.wait(layout)
