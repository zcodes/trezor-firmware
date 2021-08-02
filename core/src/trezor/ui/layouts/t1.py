from trezorui import layout_new_confirm_action

from trezor import log, ui, utils, wire
from trezor.enums import ButtonRequestType
from trezor.ui.qr import Qr
from trezor.utils import chunks, chunks_intersperse

from ..components.common import break_path_to_lines
from ..components.common.confirm import is_confirmed, raise_if_cancelled
from ..components.common.homescreen import HomescreenBase
from ..components.common.text import Span
from ..components.t1.confirm import Confirm, HoldToConfirm
from ..components.t1.homescreen import Homescreen
from ..components.t1.loader import loader
from ..components.t1.text import Text
from ..constants.t1 import (
    MONO_ADDR_PER_LINE,
    MONO_CHARS_PER_LINE,
    MONO_HEX_PER_LINE,
    QR_SIZE_THRESHOLD,
    QR_X,
    QR_Y,
    TEXT_MAX_LINES,
)
from .common import interact

if False:
    from typing import Awaitable, Iterator, Sequence, NoReturn, Type, Union

    ExceptionType = Union[BaseException, Type[BaseException]]


async def confirm_action(
    ctx: wire.GenericContext,
    br_type: str,
    title: str,
    action: str | None = None,
    description: str | None = None,
    description_param: str | None = None,
    description_param_font: int = ui.BOLD,
    verb: str | bytes | None = Confirm.DEFAULT_CONFIRM,
    verb_cancel: str | bytes | None = Confirm.DEFAULT_CANCEL,
    hold: bool = False,
    hold_danger: bool = False,
    icon: str | None = None,  # TODO cleanup @ redesign
    icon_color: int | None = None,  # TODO cleanup @ redesign
    reverse: bool = False,  # TODO cleanup @ redesign
    larger_vspace: bool = False,  # TODO cleanup @ redesign
    exc: ExceptionType = wire.ActionCancelled,
    br_code: ButtonRequestType = ButtonRequestType.Other,
) -> None:
    if isinstance(verb, bytes) or isinstance(verb_cancel, bytes):
        raise NotImplementedError

    if description_param is not None:
        if description_param_font != ui.BOLD:
            log.error(__name__, "confirm_action description_param_font not implemented")
        description = description.format(description_param)

    if hold:
        log.error(__name__, "confirm_action hold not implemented")

    result = await interact(
        ctx,
        ui.RustLayout(
            layout_new_confirm_action(
                title.upper(),
                action,
                description,
                verb,
                verb_cancel,
                hold,
                reverse,
            )
        ),
        br_type,
        br_code,
    )
    if result == 1:
        raise exc


async def confirm_reset_device(ctx: wire.GenericContext, prompt: str) -> None:
    text = Text(None, new_lines=False)
    if prompt:
        text.bold(prompt.replace("\n", " "))  # FIXME
        text.br()
    text.br_half()
    text.normal("By continuing you agree")
    text.br()
    text.normal("to ")
    text.bold("trezor.io/tos")
    await raise_if_cancelled(
        interact(
            ctx,
            Confirm(text, confirm="CREATE"),
            "setup_device",
            ButtonRequestType.ResetDevice,
        )
    )


async def confirm_backup(ctx: wire.GenericContext) -> bool:
    text1 = Text(None)
    text1.bold("New wallet created", "successfully!")
    text1.br_half()
    text1.normal("You should back up your", "new wallet right now.")

    text2 = Text("Skip the backup?")
    text2.normal("You can back up ", "your Trezor once, ", "at any time.")

    if is_confirmed(
        await interact(
            ctx,
            Confirm(text1, cancel="NO", confirm="BACKUP"),
            "backup_device",
            ButtonRequestType.ResetDevice,
        )
    ):
        return True

    confirmed = is_confirmed(
        await interact(
            ctx,
            Confirm(text2, cancel="NO", confirm="BACKUP"),
            "backup_device",
            ButtonRequestType.ResetDevice,
        )
    )
    return confirmed


async def confirm_path_warning(
    ctx: wire.GenericContext, path: str, path_type: str = "ADDRESS PATH"
) -> None:
    text = Text("WRONG {}".format(path_type.upper()))
    text.mono(*break_path_to_lines(path, MONO_CHARS_PER_LINE))
    text.br_half()
    text.normal("Are you sure?")
    await raise_if_cancelled(
        interact(
            ctx,
            Confirm(text),
            "path_warning",
            ButtonRequestType.UnknownDerivationPath,
        )
    )


def _show_qr(
    address: str,
) -> Confirm:
    QR_COEF = 2 if len(address) < QR_SIZE_THRESHOLD else 1
    qr = Qr(address, QR_X, QR_Y, QR_COEF)

    return Confirm(qr, confirm="CONTINUE", cancel="")


def _split_address(address: str) -> Iterator[str]:
    return chunks(address, MONO_CHARS_PER_LINE)


def _truncate_hex(
    hex_data: str,
    lines: int = TEXT_MAX_LINES,
    width: int = MONO_HEX_PER_LINE,
    middle: bool = False,
    ellipsis: str = "...",  # TODO: cleanup @ redesign
) -> Iterator[str]:
    ell_len = len(ellipsis)
    if len(hex_data) > width * lines:
        if middle:
            hex_data = (
                hex_data[: lines * width // 2 - (ell_len // 2)]
                + ellipsis
                + hex_data[-lines * width // 2 + (ell_len - ell_len // 2) :]
            )
        else:
            hex_data = hex_data[: (width * lines - ell_len)] + ellipsis
    return chunks_intersperse(hex_data, width)


# TODO: pagination
def _show_address(
    address: str,
    title: str,
    network: str | None = None,
    extra: str | None = None,
) -> Confirm:
    text = Text(title)
    if network is not None:
        text.normal("%s network" % network)
    if extra is not None:
        text.bold(extra)
    text.mono(*_split_address(address))

    return Confirm(text, confirm="CONTINUE", cancel="QR CODE")


def _show_xpub(xpub: str, title: str, cancel: str) -> Confirm:
    return Confirm(Text("NOT IMPLEMENTED"), cancel=cancel.upper())


async def show_xpub(
    ctx: wire.GenericContext, xpub: str, title: str, cancel: str
) -> None:
    await raise_if_cancelled(
        interact(
            ctx,
            _show_xpub(xpub, title, cancel),
            "show_xpub",
            ButtonRequestType.PublicKey,
        )
    )


async def show_address(
    ctx: wire.GenericContext,
    address: str,
    address_qr: str | None = None,
    title: str = "Confirm address",
    network: str | None = None,
    multisig_index: int | None = None,
    xpubs: Sequence[str] = [],
    address_extra: str | None = None,
    title_qr: str | None = None,  # ignored, no room for title
) -> None:
    is_multisig = len(xpubs) > 0
    while True:
        if is_confirmed(
            await interact(
                ctx,
                _show_address(address, title, network, extra=address_extra),
                "show_address",
                ButtonRequestType.Address,
            )
        ):
            break
        if is_confirmed(
            await interact(
                ctx,
                _show_qr(
                    address if address_qr is None else address_qr,
                ),
                "show_qr",
                ButtonRequestType.Address,
            )
        ):
            break

        if is_multisig:
            for i, xpub in enumerate(xpubs):
                cancel = "NEXT" if i < len(xpubs) - 1 else "ADDRESS"
                title_xpub = "XPUB #%d" % (i + 1)
                title_xpub += " (yours)" if i == multisig_index else " (cosigner)"
                if is_confirmed(
                    await interact(
                        ctx,
                        _show_xpub(xpub, title=title_xpub, cancel=cancel),
                        "show_xpub",
                        ButtonRequestType.PublicKey,
                    )
                ):
                    return


async def _show_modal(
    ctx: wire.GenericContext,
    br_type: str,
    br_code: ButtonRequestType,
    header: str | None,
    subheader: str | None,
    content: str,
    button_confirm: str | None,
    button_cancel: str | None,
    exc: ExceptionType = wire.ActionCancelled,
) -> None:
    if header is not None:
        header = header.upper()
    text = Text(header, new_lines=False)
    if subheader:
        text.bold(subheader)
        text.br()
        text.br_half()
    text.normal(content)
    await raise_if_cancelled(
        interact(
            ctx,
            Confirm(text, confirm=button_confirm, cancel=button_cancel),
            br_type,
            br_code,
        ),
        exc,
    )


async def show_error_and_raise(
    ctx: wire.GenericContext,
    br_type: str,
    content: str,
    header: str = "FAIL!",
    subheader: str | None = None,
    button: str = "CLOSE",
    red: bool = False,
    exc: ExceptionType = wire.ActionCancelled,
) -> NoReturn:
    await _show_modal(
        ctx,
        br_type=br_type,
        br_code=ButtonRequestType.Other,
        header=header,
        subheader=subheader,
        content=content,
        button_confirm=None,
        button_cancel=button,
        exc=exc,
    )
    raise exc


def show_warning(
    ctx: wire.GenericContext,
    br_type: str,
    content: str,
    header: str = "WARNING!",
    subheader: str | None = None,
    button: str = "TRY AGAIN",
    br_code: ButtonRequestType = ButtonRequestType.Warning,
) -> Awaitable[None]:
    return _show_modal(
        ctx,
        br_type=br_type,
        br_code=br_code,
        header=header,
        subheader=subheader,
        content=content,
        button_confirm=button,
        button_cancel=None,
    )


def show_success(
    ctx: wire.GenericContext,
    br_type: str,
    content: str,
    subheader: str | None = None,
    button: str = "CLOSE",
) -> Awaitable[None]:
    return _show_modal(
        ctx,
        br_type=br_type,
        br_code=ButtonRequestType.Success,
        header="SUCCESS!",
        subheader=subheader,
        content=content,
        button_confirm=button,
        button_cancel=None,
    )


async def confirm_output(
    ctx: wire.GenericContext,
    address: str,
    amount: str,
    font_amount: int = ui.NORMAL,  # TODO cleanup @ redesign
    title: str = "TRANSACTION",
    subtitle: str | None = None,  # TODO cleanup @ redesign
    color_to: int = ui.FG,  # TODO cleanup @ redesign
    to_str: str = " to\n",  # TODO cleanup @ redesign
    to_paginated: bool = False,  # TODO cleanup @ redesign
    width: int = MONO_ADDR_PER_LINE,
    width_paginated: int = MONO_ADDR_PER_LINE - 1,
    br_code: ButtonRequestType = ButtonRequestType.ConfirmOutput,
) -> None:
    text = Text("TRANSACTION", new_lines=False)
    if subtitle is not None:
        text.normal(subtitle, "\n")
    text.normal("Send " + amount + " to\n")
    text.mono(*_split_address(address))

    await raise_if_cancelled(interact(ctx, Confirm(text), "confirm_output", br_code))


# TODO: pagination
async def confirm_hex(
    ctx: wire.GenericContext,
    br_type: str,
    title: str,
    data: str,
    subtitle: str | None = None,
    description: str | None = None,
    br_code: ButtonRequestType = ButtonRequestType.Other,
    icon: str = ui.ICON_SEND,  # TODO cleanup @ redesign
    icon_color: int = ui.GREEN,  # TODO cleanup @ redesign
    color_description: int = ui.FG,  # TODO cleanup @ redesign
    width: int | None = MONO_HEX_PER_LINE,
    width_paginated: int = MONO_HEX_PER_LINE - 2,
    truncate: bool = False,
    truncate_middle: bool = False,
    truncate_ellipsis: str = "...",
) -> None:
    text = Text(title, new_lines=False)
    description_lines = 0
    if subtitle is not None:
        description_lines += Span(subtitle, 0, ui.BOLD).count_lines()
        text.bold(subtitle)
        text.br()
    if description is not None:
        description_lines += Span(description, 0, ui.NORMAL).count_lines()
        text.content.extend((ui.NORMAL, color_description, description, ui.FG))
        text.br()
    if width is not None:
        text.mono(
            *_truncate_hex(
                data,
                lines=TEXT_MAX_LINES - description_lines,
                width=width,
                middle=truncate_middle,
                ellipsis=truncate_ellipsis,
            )
        )
    else:
        text.mono(data)
    content: ui.Layout = Confirm(text)
    await raise_if_cancelled(interact(ctx, content, br_type, br_code))


async def confirm_total(
    ctx: wire.GenericContext, total_amount: str, fee_amount: str
) -> None:
    text = Text("TRANSACTION")
    text.bold("Total amount:")
    text.mono(total_amount)
    text.bold("Fee included:")
    text.mono(fee_amount)
    await raise_if_cancelled(
        interact(
            ctx,
            HoldToConfirm(text, confirm="HOLD TO CONFIRM", cancel="X"),
            "confirm_total",
            ButtonRequestType.SignTx,
        )
    )


async def confirm_joint_total(
    ctx: wire.GenericContext, spending_amount: str, total_amount: str
) -> None:
    text = Text("JOINT TRANSACTION")
    text.bold("You are contributing:")
    text.mono(spending_amount)
    text.bold("to the total amount:")
    text.mono(total_amount)
    await raise_if_cancelled(
        interact(
            ctx,
            HoldToConfirm(text, confirm="HOLD TO CONFIRM", cancel="X"),
            "confirm_joint_total",
            ButtonRequestType.SignTx,
        )
    )


async def confirm_metadata(
    ctx: wire.GenericContext,
    br_type: str,
    title: str,
    content: str,
    param: str | None = None,
    br_code: ButtonRequestType = ButtonRequestType.SignTx,
    hide_continue: bool = False,
    hold: bool = False,
    param_font: int = ui.BOLD,
    icon: str = ui.ICON_SEND,  # TODO cleanup @ redesign
    icon_color: int = ui.GREEN,  # TODO cleanup @ redesign
    larger_vspace: bool = False,  # TODO cleanup @ redesign
) -> None:
    text = Text(title.upper(), new_lines=False)
    text.format_parametrized(
        content, param if param is not None else "", param_font=param_font
    )

    if not hide_continue:
        text.br()
        if larger_vspace:
            text.br_half()
        text.normal("Continue?")

    cls = HoldToConfirm if hold else Confirm

    await raise_if_cancelled(interact(ctx, cls(text), br_type, br_code))


def draw_progress_init(sign: bool = True) -> None:
    if sign:
        ui.display.text_center(
            ui.WIDTH // 2, 19, "Signing transaction", ui.BOLD, ui.FG, ui.BG
        )
    else:
        ui.display.text_center(ui.WIDTH // 2, 19, "Starting up", ui.BOLD, ui.FG, ui.BG)
    ui.display.text_center(ui.WIDTH // 2, 32, "Please wait", ui.NORMAL, ui.FG, ui.BG)


def draw_progress_update(progress: int, total: int) -> None:
    p = 1000 * progress // total
    loader(p)


def homescreen() -> HomescreenBase:
    return Homescreen()
