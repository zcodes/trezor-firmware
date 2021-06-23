from micropython import const

from trezor import loop, res, ui, utils

from ..common.confirm import CANCELLED, CONFIRMED, INFO, ConfirmBase, HoldToConfirmBase
from .button import Button, ButtonAbort, ButtonCancel, ButtonConfirm, ButtonDefault
from .loader import Loader, LoaderDefault

if False:
    from typing import Any
    from .button import ButtonContent, ButtonStyleType
    from .loader import LoaderStyleType


class Confirm(ConfirmBase):
    DEFAULT_CONFIRM = res.load(ui.ICON_CONFIRM)
    DEFAULT_CONFIRM_STYLE = ButtonConfirm
    DEFAULT_CANCEL = res.load(ui.ICON_CANCEL)
    DEFAULT_CANCEL_STYLE = ButtonCancel

    def __init__(
        self,
        content: ui.Component,
        confirm: ButtonContent | None = DEFAULT_CONFIRM,
        confirm_style: ButtonStyleType = DEFAULT_CONFIRM_STYLE,
        cancel: ButtonContent | None = DEFAULT_CANCEL,
        cancel_style: ButtonStyleType = DEFAULT_CANCEL_STYLE,
        major_confirm: bool = False,
    ) -> None:
        self.content = content
        button_confirm: Button | None = None
        button_cancel: Button | None = None

        if confirm is not None:
            if cancel is None:
                area = ui.grid(4, n_x=1)
            elif major_confirm:
                area = ui.grid(13, cells_x=2)
            else:
                area = ui.grid(9, n_x=2)
            button_confirm = Button(area, confirm, confirm_style)
            button_confirm.on_click = self.on_confirm  # type: ignore

        if cancel is not None:
            if confirm is None:
                area = ui.grid(4, n_x=1)
            elif major_confirm:
                area = ui.grid(12, cells_x=1)
            else:
                area = ui.grid(8, n_x=2)
            button_cancel = Button(area, cancel, cancel_style)
            button_cancel.on_click = self.on_cancel  # type: ignore

        super().__init__(content, button_confirm, button_cancel)


class Pageable:
    def __init__(self) -> None:
        self._page = 0

    def page(self) -> int:
        return self._page

    def page_count(self) -> int:
        raise NotImplementedError

    def is_first(self) -> bool:
        return self._page == 0

    def is_last(self) -> bool:
        return self._page == self.page_count() - 1

    def next(self) -> None:
        self._page = min(self._page + 1, self.page_count() - 1)

    def prev(self) -> None:
        self._page = max(self._page - 1, 0)


class ConfirmPageable(Confirm):
    def __init__(self, pageable: Pageable, *args: Any, **kwargs: Any):
        super().__init__(*args, **kwargs)
        self.pageable = pageable

    async def handle_paging(self) -> None:
        from .swipe import SWIPE_HORIZONTAL, SWIPE_LEFT, SWIPE_RIGHT, Swipe

        if self.pageable.is_first():
            directions = SWIPE_LEFT
        elif self.pageable.is_last():
            directions = SWIPE_RIGHT
        else:
            directions = SWIPE_HORIZONTAL

        if __debug__:
            from apps.debug import swipe_signal

            swipe = await loop.race(Swipe(directions), swipe_signal())
        else:
            swipe = await Swipe(directions)

        if swipe == SWIPE_LEFT:
            self.pageable.next()
        else:
            self.pageable.prev()

        self.content.repaint = True
        if self.confirm is not None:
            self.confirm.repaint = True
        if self.cancel is not None:
            self.cancel.repaint = True

    def create_tasks(self) -> tuple[loop.Task, ...]:
        tasks = super().create_tasks()
        if self.pageable.page_count() > 1:
            return tasks + (self.handle_paging(),)
        else:
            return tasks

    def on_render(self) -> None:
        PULSE_PERIOD = const(1_200_000)

        super().on_render()

        if not self.pageable.is_first():
            t = ui.pulse(PULSE_PERIOD)
            c = ui.blend(ui.GREY, ui.DARK_GREY, t)
            icon = res.load(ui.ICON_SWIPE_RIGHT)
            if utils.DISABLE_ANIMATION:
                ui.display.icon(18, 68, icon, ui.GREY, ui.BG)
            else:
                ui.display.icon(18, 68, icon, c, ui.BG)

        if not self.pageable.is_last():
            t = ui.pulse(PULSE_PERIOD, PULSE_PERIOD // 2)
            c = ui.blend(ui.GREY, ui.DARK_GREY, t)
            icon = res.load(ui.ICON_SWIPE_LEFT)
            if utils.DISABLE_ANIMATION:
                ui.display.icon(205, 68, icon, ui.GREY, ui.BG)
            else:
                ui.display.icon(205, 68, icon, c, ui.BG)


class InfoConfirm(ui.Layout):
    DEFAULT_CONFIRM = res.load(ui.ICON_CONFIRM)
    DEFAULT_CONFIRM_STYLE = ButtonConfirm
    DEFAULT_CANCEL = res.load(ui.ICON_CANCEL)
    DEFAULT_CANCEL_STYLE = ButtonCancel
    DEFAULT_INFO = res.load(ui.ICON_CLICK)  # TODO: this should be (i) icon, not click
    DEFAULT_INFO_STYLE = ButtonDefault

    def __init__(
        self,
        content: ui.Component,
        confirm: ButtonContent = DEFAULT_CONFIRM,
        confirm_style: ButtonStyleType = DEFAULT_CONFIRM_STYLE,
        cancel: ButtonContent = DEFAULT_CANCEL,
        cancel_style: ButtonStyleType = DEFAULT_CANCEL_STYLE,
        info: ButtonContent = DEFAULT_INFO,
        info_style: ButtonStyleType = DEFAULT_INFO_STYLE,
    ) -> None:
        super().__init__()
        self.content = content

        self.confirm = Button(ui.grid(14), confirm, confirm_style)
        self.confirm.on_click = self.on_confirm  # type: ignore

        self.info = Button(ui.grid(13), info, info_style)
        self.info.on_click = self.on_info  # type: ignore

        self.cancel = Button(ui.grid(12), cancel, cancel_style)
        self.cancel.on_click = self.on_cancel  # type: ignore

    def dispatch(self, event: int, x: int, y: int) -> None:
        self.content.dispatch(event, x, y)
        if self.confirm is not None:
            self.confirm.dispatch(event, x, y)
        if self.cancel is not None:
            self.cancel.dispatch(event, x, y)
        if self.info is not None:
            self.info.dispatch(event, x, y)

    def on_confirm(self) -> None:
        raise ui.Result(CONFIRMED)

    def on_cancel(self) -> None:
        raise ui.Result(CANCELLED)

    def on_info(self) -> None:
        raise ui.Result(INFO)

    if __debug__:

        def read_content(self) -> list[str]:
            return self.content.read_content()

        def create_tasks(self) -> tuple[loop.Task, ...]:
            from apps.debug import confirm_signal

            return super().create_tasks() + (confirm_signal(),)


class HoldToConfirm(HoldToConfirmBase):
    DEFAULT_CONFIRM = "Hold to confirm"
    DEFAULT_CONFIRM_STYLE = ButtonConfirm
    DEFAULT_CANCEL = res.load(ui.ICON_CANCEL)
    DEFAULT_CANCEL_STYLE = ButtonAbort
    DEFAULT_LOADER_STYLE = LoaderDefault

    BOTTOM_MARGIN = 58

    def __init__(
        self,
        content: ui.Component,
        confirm: ButtonContent = DEFAULT_CONFIRM,
        confirm_style: ButtonStyleType = DEFAULT_CONFIRM_STYLE,
        loader_style: LoaderStyleType = DEFAULT_LOADER_STYLE,
        cancel: bool = True,
    ):
        loader = Loader(loader_style)

        if cancel:
            button_confirm = Button(
                ui.grid(17, n_x=4, cells_x=3), confirm, confirm_style
            )
        else:
            button_confirm = Button(ui.grid(4, n_x=1), confirm, confirm_style)

        button_cancel = None
        if cancel:
            button_cancel = Button(
                ui.grid(16, n_x=4), self.DEFAULT_CANCEL, self.DEFAULT_CANCEL_STYLE
            )

        super().__init__(content, loader, button_confirm, button_cancel)
