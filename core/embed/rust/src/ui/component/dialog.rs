use crate::micropython::buffer::Buffer;
use crate::ui::display;
use crate::ui::geometry::{Grid, Point, Rect};
use crate::ui::theme;

use super::{
    base::{Child, Component, Event, EventCtx},
    button::{Button, ButtonMsg::Clicked},
};

pub enum DialogMsg<T: Component> {
    Content(T::Msg),
    LeftClicked,
    RightClicked,
}

pub struct Dialog<T> {
    header: Option<Buffer>,
    content: Child<T>,
    left_btn: Option<Child<Button>>,
    right_btn: Option<Child<Button>>,
}

impl<T: Component> Dialog<T> {
    #[cfg(feature = "model_t")]
    pub fn new(
        area: Rect,
        content: impl FnOnce(Rect) -> T,
        left: Option<impl FnOnce(Rect) -> Button>,
        right: Option<impl FnOnce(Rect) -> Button>,
        header: Option<Buffer>,
    ) -> Self {
        let grid = Grid::new(area, 5, 2);
        let content = Child::new(content(Rect::new(
            grid.row_col(0, 0).top_left(),
            grid.row_col(4, 1).bottom_right(),
        )));
        let left_btn = left.map(|f| Child::new(f(grid.row_col(4, 0))));
        let right_btn = right.map(|f| Child::new(f(grid.row_col(4, 1))));
        Self {
            header,
            content,
            left_btn: left_btn,
            right_btn: right_btn,
        }
    }

    #[cfg(feature = "model_t")]
    fn paint_header(&self) {
        if let Some(h) = &self.header {
            todo!();
        }
    }

    #[cfg(feature = "model_1")]
    pub fn new(
        area: Rect,
        content: impl FnOnce(Rect) -> T,
        left: Option<impl FnOnce(Rect) -> Button>,
        right: Option<impl FnOnce(Rect) -> Button>,
        header: Option<Buffer>,
    ) -> Self {
        let button_h = 11;
        let header_h = 13;

        let (content_area, buttons) = area.hsplit(-button_h);
        let content_area = if header.is_none() {
            content_area
        } else {
            content_area.hsplit(header_h).1
        };
        let (left_rect, right_rect) = buttons.vsplit(buttons.width() / 2);

        let content = Child::new(content(content_area));
        let left_btn = left.map(|f| Child::new(f(left_rect)));
        let right_btn = right.map(|f| Child::new(f(right_rect)));
        Self {
            header: header,
            content: content,
            left_btn: left_btn,
            right_btn: right_btn,
        }
    }

    #[cfg(feature = "model_1")]
    fn paint_header(&self) {
        if let Some(h) = &self.header {
            let line_height = theme::FONT_BOLD.line_height();
            display::text(
                Point::new(0, line_height - 2),
                h,
                theme::FONT_BOLD,
                theme::FG,
                theme::BG,
            );
            display::dotted_line(Point::new(0, line_height), theme::FG)
        }
    }
}

impl<T: Component> Component for Dialog<T> {
    type Msg = DialogMsg<T>;

    fn event(&mut self, ctx: &mut EventCtx, event: Event) -> Option<Self::Msg> {
        if let Some(msg) = self.content.event(ctx, event) {
            Some(DialogMsg::Content(msg))
        } else if let Some(Clicked) = self.left_btn.as_mut().and_then(|b| b.event(ctx, event)) {
            Some(DialogMsg::LeftClicked)
        } else if let Some(Clicked) = self.right_btn.as_mut().and_then(|b| b.event(ctx, event)) {
            Some(DialogMsg::RightClicked)
        } else {
            None
        }
    }

    fn paint(&mut self) {
        self.paint_header();
        self.content.paint();
        if let Some(b) = self.left_btn.as_mut() {
            b.paint();
        }
        if let Some(b) = self.right_btn.as_mut() {
            b.paint();
        }
    }
}

#[cfg(feature = "ui_debug")]
impl<T> crate::trace::Trace for Dialog<T>
where
    T: crate::trace::Trace,
{
    fn trace(&self, t: &mut dyn crate::trace::Tracer) {
        t.open("Dialog");
        t.field("content", &self.content);
        t.field("left", &self.left_btn);
        t.field("right", &self.right_btn);
        t.close();
    }
}
