from trezor import ui, utils, workflow

_progress = 0
_steps = 0


def init(inputs: int, outputs: int) -> None:
    global _progress, _steps
    _progress = 0
    _steps = inputs + inputs + outputs + inputs
    report_init()
    report()


def advance(i: int = 1) -> None:
    global _progress
    _progress += i
    report()


def report_init() -> None:
    from trezor.ui.layouts import draw_progress_init

    workflow.close_others()
    ui.display.clear()
    draw_progress_init()


def report() -> None:
    from trezor.ui.layouts import draw_progress_update

    if utils.DISABLE_ANIMATION:
        return
    draw_progress_update(_progress, _steps)
