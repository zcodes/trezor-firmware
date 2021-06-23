from trezor import loop, ui, utils, wire

from .loader import LoaderBase

if False:
    from typing import Any, Awaitable

CONFIRMED = object()
CANCELLED = object()
INFO = object()


def is_confirmed(x: Any) -> bool:
    return x is CONFIRMED


async def raise_if_cancelled(a: Awaitable, exc: Any = wire.ActionCancelled) -> None:
    result = await a
    if result is CANCELLED:
        raise exc


class ConfirmBase(ui.Layout):
    def __init__(
        self,
        content: ui.Component,
        confirm: ui.Component | None = None,
        cancel: ui.Component | None = None,
    ) -> None:
        self.content = content
        self.confirm = confirm
        self.cancel = cancel

    def dispatch(self, event: int, x: int, y: int) -> None:
        super().dispatch(event, x, y)
        self.content.dispatch(event, x, y)
        if self.confirm is not None:
            self.confirm.dispatch(event, x, y)
        if self.cancel is not None:
            self.cancel.dispatch(event, x, y)

    def on_confirm(self) -> None:
        raise ui.Result(CONFIRMED)

    def on_cancel(self) -> None:
        raise ui.Result(CANCELLED)

    if __debug__:

        def read_content(self) -> list[str]:
            return self.content.read_content()

        def create_tasks(self) -> tuple[loop.Task, ...]:
            from apps.debug import confirm_signal

            return super().create_tasks() + (confirm_signal(),)


class HoldToConfirmBase(ui.Layout):
    TOP_MARGIN = 0
    BOTTOM_MARGIN = 0

    def __init__(
        self,
        content: ui.Component,
        loader: LoaderBase,
        confirm: ui.Component,
        cancel: ui.Component | None = None,
    ):
        super().__init__()
        self.content = content
        self.loader = loader
        self.confirm = confirm
        self.cancel = cancel

        self.loader.on_start = self._on_loader_start  # type: ignore
        self.confirm.on_press_start = self._on_press_start  # type: ignore
        self.confirm.on_press_end = self._on_press_end  # type: ignore
        self.confirm.on_click = self._on_click  # type: ignore

        if cancel is not None:
            self.cancel.on_click = self.on_cancel  # type: ignore

    def _on_press_start(self) -> None:
        self.loader.start()

    def _on_press_end(self) -> None:
        self.loader.stop()

    def _on_loader_start(self) -> None:
        # Loader has either started growing, or returned to the 0-position.
        # In the first case we need to clear the content leftovers, in the latter
        # we need to render the content again.
        ui.display.bar(
            0,
            0 + self.TOP_MARGIN,
            ui.WIDTH,
            ui.HEIGHT - self.TOP_MARGIN - self.BOTTOM_MARGIN,
            ui.BG,
        )
        self.content.dispatch(ui.REPAINT, 0, 0)

    def _on_click(self) -> None:
        if self.loader.elapsed_ms() >= self.loader.target_ms:
            self.on_confirm()

    def dispatch(self, event: int, x: int, y: int) -> None:
        super().dispatch(event, x, y)
        if self.loader.start_ms is not None:
            if utils.DISABLE_ANIMATION:
                self.on_confirm()
            self.loader.dispatch(event, x, y)
        else:
            self.content.dispatch(event, x, y)
        self.confirm.dispatch(event, x, y)
        if self.cancel:
            self.cancel.dispatch(event, x, y)

    def on_confirm(self) -> None:
        raise ui.Result(CONFIRMED)

    def on_cancel(self) -> None:
        raise ui.Result(CANCELLED)

    if __debug__:

        def read_content(self) -> list[str]:
            return self.content.read_content()

        def create_tasks(self) -> tuple[loop.Task, ...]:
            from apps.debug import confirm_signal

            return super().create_tasks() + (confirm_signal(),)
