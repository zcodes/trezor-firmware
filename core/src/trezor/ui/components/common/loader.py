import utime
from micropython import const

from trezor import ui

_TARGET_MS = const(1000)
_REVERSE_SPEEDUP = const(2)


class LoaderBase(ui.Component):
    def __init__(
        self,
        target_ms: int | None = None,
        reverse_speedup: int | None = None,
    ) -> None:
        super().__init__()
        self.target_ms = target_ms or _TARGET_MS
        self.start_ms: int | None = None
        self.stop_ms: int | None = None
        self.reverse_speedup = reverse_speedup or _REVERSE_SPEEDUP

    def start(self) -> None:
        if self.start_ms is not None and self.stop_ms is not None:
            self.start_ms = utime.ticks_ms() - self.elapsed_ms()
        else:
            self.start_ms = utime.ticks_ms()
        self.stop_ms = None
        self.on_start()

    def stop(self) -> None:
        self.stop_ms = utime.ticks_ms()

    def elapsed_ms(self) -> int:
        start = self.start_ms
        stop = self.stop_ms
        now = utime.ticks_ms()
        if start is None:
            return 0
        elif stop is not None:
            return max(stop - start + (stop - now) * self.reverse_speedup, 0)
        else:
            return min(now - start, self.target_ms)

    def on_render(self) -> None:
        if self.start_ms is None:
            return
        target = self.target_ms
        r = self.elapsed_ms()

        self.render_loader(r, target)

        if (r == 0) and (self.stop_ms is not None):
            self.start_ms = None
            self.stop_ms = None
            self.on_start()
        if r == target:
            self.on_finish()

    def render_loader(self, elapsed: int, target: int) -> None:
        raise NotImplementedError

    def on_start(self) -> None:
        pass

    def on_finish(self) -> None:
        pass
