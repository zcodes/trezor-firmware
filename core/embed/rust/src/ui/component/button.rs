use crate::micropython::buffer::Buffer;
use crate::ui::{
    display::{self, Color, Font},
    geometry::{Offset, Point, Rect},
    theme,
};

use super::base::{Component, Event, EventCtx};

pub enum ButtonMsg {
    Clicked,
}

pub struct Button {
    area: Rect,
    content: ButtonContent,
    styles: ButtonStyleSheet,
    state: State,
}

impl Button {
    pub fn new(area: Rect, content: ButtonContent, styles: ButtonStyleSheet) -> Self {
        Self {
            area,
            content,
            styles,
            state: State::Initial,
        }
    }

    pub fn with_text(area: Rect, text: Buffer, styles: ButtonStyleSheet) -> Self {
        Self::new(area, ButtonContent::Text(text), styles)
    }

    pub fn with_image(area: Rect, image: &'static [u8], styles: ButtonStyleSheet) -> Self {
        Self::new(area, ButtonContent::Image(image), styles)
    }

    pub fn enable(&mut self, ctx: &mut EventCtx) {
        self.set(ctx, State::Initial)
    }

    pub fn disable(&mut self, ctx: &mut EventCtx) {
        self.set(ctx, State::Disabled)
    }

    pub fn is_enabled(&self) -> bool {
        matches!(
            self.state,
            State::Initial | State::Pressed | State::Released
        )
    }

    pub fn is_disabled(&self) -> bool {
        matches!(self.state, State::Disabled)
    }

    pub fn content(&self) -> &ButtonContent {
        &self.content
    }

    fn style(&self) -> &ButtonStyle {
        match self.state {
            State::Initial | State::Released => self.styles.normal,
            State::Pressed => self.styles.active,
            State::Disabled => self.styles.disabled,
        }
    }

    fn set(&mut self, ctx: &mut EventCtx, state: State) {
        if self.state != state {
            self.state = state;
            ctx.request_paint();
        }
    }
}

impl Component for Button {
    type Msg = ButtonMsg;

    fn event(&mut self, ctx: &mut EventCtx, event: Event) -> Option<Self::Msg> {
        match event {
            Event::TouchStart(pos) => {
                match self.state {
                    State::Disabled => {
                        // Do nothing.
                    }
                    _ => {
                        // Touch started in our area, transform to `Pressed` state.
                        if self.area.contains(pos) {
                            self.set(ctx, State::Pressed);
                        }
                    }
                }
            }
            Event::TouchMove(pos) => {
                match self.state {
                    State::Released if self.area.contains(pos) => {
                        // Touch entered our area, transform to `Pressed` state.
                        self.set(ctx, State::Pressed);
                    }
                    State::Pressed if !self.area.contains(pos) => {
                        // Touch is leaving our area, transform to `Released` state.
                        self.set(ctx, State::Released);
                    }
                    _ => {
                        // Do nothing.
                    }
                }
            }
            Event::TouchEnd(pos) => {
                match self.state {
                    State::Initial | State::Disabled => {
                        // Do nothing.
                    }
                    State::Pressed if self.area.contains(pos) => {
                        // Touch finished in our area, we got clicked.
                        self.set(ctx, State::Initial);

                        return Some(ButtonMsg::Clicked);
                    }
                    _ => {
                        // Touch finished outside our area.
                        self.set(ctx, State::Initial);
                    }
                }
            }
            _ => {}
        };
        None
    }

    #[cfg(feature = "model_t")]
    fn paint(&mut self) {
        let style = self.style();

        if style.border_width > 0 {
            // Paint the border and a smaller background on top of it.
            display::rounded_rect(
                self.area,
                style.border_color,
                style.background_color,
                style.border_radius,
            );
            display::rounded_rect(
                self.area.inset(style.border_width),
                style.button_color,
                style.border_color,
                style.border_radius,
            );
        } else {
            // We do not need to draw an explicit border in this case, just a
            // bigger background.
            display::rounded_rect(
                self.area,
                style.button_color,
                style.background_color,
                style.border_radius,
            );
        }

        match &self.content {
            ButtonContent::Text(text) => {
                let width = display::text_width(text, style.font);
                let height = display::text_height();
                let start_of_baseline = self.area.center() + Offset::new(-width / 2, height / 2);
                display::text(
                    start_of_baseline,
                    text,
                    style.font,
                    style.text_color,
                    style.button_color,
                );
            }
            ButtonContent::Image(_image) => {
                todo!();
            }
        }
    }

    #[cfg(feature = "model_1")]
    fn paint(&mut self) {
        let style = self.style();

        let is_left = self.area.top_left().x < display::width() / 2;
        let button_h = 11;
        let button_y = display::height() - button_h;

        match &self.content {
            ButtonContent::Text(text) => {
                let width = display::text_width(text, style.font);
                let height = display::text_height();

                if style.border_horiz {
                    display::rounded_rect1(
                        Rect::from_top_left_and_size(
                            Point::new(
                                if is_left {
                                    0
                                } else {
                                    display::width() - width - 3
                                },
                                button_y,
                            ),
                            Offset::new(width + 3, button_h),
                        ),
                        style.background_color,
                        theme::BG,
                    );
                } else {
                    display::rect(
                        Rect::from_top_left_and_size(
                            Point::new(
                                if is_left {
                                    0
                                } else {
                                    display::width() - width + 1
                                },
                                button_y,
                            ),
                            Offset::new(width - 1, button_h),
                        ),
                        style.background_color,
                    )
                }

                let h_border = if style.border_horiz { 2 } else { 0 };
                let start_of_baseline = if is_left {
                    Point::new(h_border, button_y + height + 1)
                } else {
                    Point::new(
                        display::width() - h_border + 1 - width,
                        button_y + height + 1,
                    )
                };
                display::text(
                    start_of_baseline,
                    text,
                    style.font,
                    style.text_color,
                    style.background_color,
                );
            }
            ButtonContent::Image(_image) => {
                todo!();
            }
        }
    }
}

#[cfg(feature = "ui_debug")]
impl crate::trace::Trace for Button {
    fn trace(&self, t: &mut dyn crate::trace::Tracer) {
        t.open("Button");
        match &self.content {
            ButtonContent::Text(text) => t.field("text", &text.as_ref()),
            ButtonContent::Image(_) => t.symbol("image"),
        }
        t.close();
    }
}

#[derive(PartialEq, Eq)]
enum State {
    Initial,
    Pressed,
    Released,
    Disabled,
}

pub enum ButtonContent {
    Text(Buffer),
    Image(&'static [u8]),
}

pub struct ButtonStyleSheet {
    pub normal: &'static ButtonStyle,
    pub active: &'static ButtonStyle,
    pub disabled: &'static ButtonStyle,
}

#[cfg(feature = "model_t")]
pub struct ButtonStyle {
    pub font: Font,
    pub text_color: Color,
    pub button_color: Color,
    pub background_color: Color,
    pub border_color: Color,
    pub border_radius: u8,
    pub border_width: i32,
}

#[cfg(feature = "model_1")]
pub struct ButtonStyle {
    pub font: Font,
    pub text_color: Color,
    pub background_color: Color,
    pub border_horiz: bool,
}
