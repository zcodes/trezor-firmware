from trezor import ui

from ..common.confirm import ConfirmBase, HoldToConfirmBase
from .button import Button, ButtonBlack, ButtonWhite
from .loader import Loader

if False:
    from .button import ButtonContent, ButtonStyleType


class Confirm(ConfirmBase):
    DEFAULT_CONFIRM = "CONFIRM"
    DEFAULT_CONFIRM_STYLE = ButtonWhite
    DEFAULT_CANCEL = "CANCEL"
    DEFAULT_CANCEL_STYLE = ButtonBlack

    def __init__(
        self,
        content: ui.Component,
        confirm: ButtonContent | None = DEFAULT_CONFIRM,
        confirm_style: ButtonStyleType = DEFAULT_CONFIRM_STYLE,
        cancel: ButtonContent | None = DEFAULT_CANCEL,
        cancel_style: ButtonStyleType = DEFAULT_CANCEL_STYLE,
    ) -> None:
        button_confirm: Button | None = None
        button_cancel: Button | None = None

        if confirm is not None:
            button_confirm = Button(True, confirm, confirm_style)
            button_confirm.on_click = self.on_confirm  # type: ignore

        if cancel is not None:
            button_cancel = Button(False, cancel, cancel_style)
            button_cancel.on_click = self.on_cancel  # type: ignore

        super().__init__(content, button_confirm, button_cancel)


class HoldToConfirm(HoldToConfirmBase):
    DEFAULT_CONFIRM = "HOLD TO CONFIRM"
    DEFAULT_CONFIRM_STYLE = ButtonWhite
    DEFAULT_CANCEL = "X"

    TOP_MARGIN = 11
    BOTTOM_MARGIN = 12

    def __init__(
        self,
        content: ui.Component,
        confirm: ButtonContent = DEFAULT_CONFIRM,
        confirm_style: ButtonStyleType = DEFAULT_CONFIRM_STYLE,
        cancel: ButtonContent = DEFAULT_CANCEL,
    ):
        loader = Loader()
        button_confirm = Button(True, confirm, confirm_style)

        button_cancel = None
        if cancel:
            button_cancel = Button(False, cancel, ButtonBlack)

        super().__init__(content, loader, button_confirm, button_cancel)
