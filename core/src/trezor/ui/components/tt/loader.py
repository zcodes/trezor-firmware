from micropython import const

from trezor import res, ui, utils
from trezor.ui import display

from ..common.loader import LoaderBase


class LoaderDefault:
    class normal:
        bg_color = ui.BG
        fg_color = ui.GREEN
        icon: str | None = None
        icon_fg_color: int | None = None

    class active(normal):
        bg_color = ui.BG
        fg_color = ui.GREEN
        icon: str | None = ui.ICON_CHECK
        icon_fg_color: int | None = ui.WHITE


class LoaderDanger(LoaderDefault):
    class normal(LoaderDefault.normal):
        fg_color = ui.RED

    class active(LoaderDefault.active):
        fg_color = ui.RED


class LoaderNeutral(LoaderDefault):
    class normal(LoaderDefault.normal):
        fg_color = ui.FG

    class active(LoaderDefault.active):
        fg_color = ui.FG


if False:
    LoaderStyleType = type[LoaderDefault]


_OFFSET_Y = const(-24)


class Loader(LoaderBase):
    def __init__(
        self,
        style: LoaderStyleType = LoaderDefault,
        target_ms: int | None = None,
        offset_y: int = _OFFSET_Y,
        reverse_speedup: int | None = None,
    ) -> None:
        super().__init__(target_ms, reverse_speedup)
        self.normal_style = style.normal
        self.active_style = style.active
        self.offset_y = offset_y

    def render_loader(self, elapsed: int, target: int) -> None:
        if elapsed != target:
            s = self.normal_style
        else:
            s = self.active_style

        progress = elapsed * 1000 // target  # scale to 0-1000
        if s.icon is None:
            display.loader(progress, False, self.offset_y, s.fg_color, s.bg_color)
        else:
            display.loader(
                progress,
                False,
                self.offset_y,
                s.fg_color,
                s.bg_color,
                res.load(s.icon),
                s.icon_fg_color,
            )


class LoadingAnimation(ui.Layout):
    def __init__(self, style: LoaderStyleType = LoaderDefault) -> None:
        super().__init__()
        self.loader = Loader(style)
        self.loader.on_finish = self.on_finish  # type: ignore
        self.loader.start()

    def dispatch(self, event: int, x: int, y: int) -> None:
        if not self.loader.elapsed_ms():
            self.loader.start()
        self.loader.dispatch(event, x, y)

        if utils.DISABLE_ANIMATION:
            self.on_finish()

    def on_finish(self) -> None:
        raise ui.Result(None)
