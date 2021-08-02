use core::convert::TryInto;
use core::slice;

use crate::{
    error::Error,
    micropython::{buffer::Buffer, obj::Obj},
    ui::{
        component::{Button, Child, Component, Dialog, DialogMsg, Never, Text},
        display, theme,
    },
    util,
};

use super::obj::LayoutObj;

impl<T> From<DialogMsg<T>> for Obj
where
    T: Component,
    T::Msg: Into<Obj>,
{
    fn from(val: DialogMsg<T>) -> Self {
        match val {
            DialogMsg::Content(c) => c.into(),
            DialogMsg::LeftClicked => 1.into(),
            DialogMsg::RightClicked => 2.into(),
        }
    }
}

impl From<Never> for Obj {
    fn from(_: Never) -> Self {
        unreachable!()
    }
}

#[no_mangle]
extern "C" fn ui_layout_new_example(param: Obj) -> Obj {
    let param: Buffer = param.try_into().unwrap();

    LayoutObj::new(Child::new(Dialog::new(
        display::screen(),
        |area| {
            Text::new(area, param)
                .with(b"some", "a few")
                .with(b"param", "xx")
        },
        Some(|area| Button::with_text(area, "Left".into(), theme::button_default())),
        Some(|area| Button::with_text(area, "Right".into(), theme::button_default())),
        None,
    )))
    .into()
}

#[no_mangle]
extern "C" fn ui_layout_new_confirm_action(n_args: usize, args: *const Obj) -> Obj {
    util::try_or_raise(|| {
        if n_args != 7 {
            return Err(Error::Missing);
        }
        let args = unpack_args(n_args, args);

        let title: Buffer = args[0].try_into()?;
        let action: Option<Buffer> = args[1].try_into().ok();
        let description: Option<Buffer> = args[2].try_into().ok();
        let verb: Option<Buffer> = args[3].try_into().ok();
        let verb_cancel: Option<Buffer> = args[4].try_into().ok();
        let _hold: bool = args[5].try_into()?;
        let reverse: bool = args[6].try_into()?;

        let format = match (&action, &description, reverse) {
            (Some(_), Some(_), false) => "{bold}{action}\n\r{normal}{description}",
            (Some(_), Some(_), true) => "{normal}{description}\n\r{bold}{action}",
            (Some(_), None, _) => "{bold}{action}",
            (None, Some(_), _) => "{normal}{description}",
            _ => "",
        };

        let left = verb_cancel
            .map(|label| |area| Button::with_text(area, label, theme::button_default()));
        let right =
            verb.map(|label| |area| Button::with_text(area, label, theme::button_default()));

        let obj = LayoutObj::new(Child::new(Dialog::new(
            display::screen(),
            |area| {
                Text::new(area, format)
                    .with(b"action", action.unwrap_or("".into()))
                    .with(b"description", description.unwrap_or("".into()))
            },
            left,
            right,
            Some(title),
        )))
        .into();
        Ok(obj)
    })
}

fn unpack_args<'a>(n_args: usize, args: *const Obj) -> &'a [Obj] {
    // SAFETY:
    // * Aligned?
    // * > The memory referenced by the returned slice must not be mutated for the
    //   duration of lifetime 'a, except inside an UnsafeCell. huh?
    // * Others trivially valid.
    let args = unsafe { slice::from_raw_parts(args, n_args) };
    args
}

#[cfg(test)]
mod tests {
    use crate::trace::{Trace, Tracer};

    use super::*;

    impl Tracer for Vec<u8> {
        fn bytes(&mut self, b: &[u8]) {
            self.extend(b)
        }

        fn string(&mut self, s: &str) {
            self.extend(s.as_bytes())
        }

        fn symbol(&mut self, name: &str) {
            self.extend(name.as_bytes())
        }

        fn open(&mut self, name: &str) {
            self.extend(b"<");
            self.extend(name.as_bytes());
            self.extend(b" ");
        }

        fn field(&mut self, name: &str, value: &dyn Trace) {
            self.extend(name.as_bytes());
            self.extend(b":");
            value.trace(self);
            self.extend(b" ");
        }

        fn close(&mut self) {
            self.extend(b">")
        }
    }

    fn trace(val: &impl Trace) -> String {
        let mut t = Vec::new();
        val.trace(&mut t);
        String::from_utf8(t).unwrap()
    }

    #[test]
    fn trace_example_layout() {
        let layout = Child::new(Dialog::new(
            display::screen(),
            |area| {
                Text::new(
                    area,
                    "Testing text layout, with some text, and some more text. And {param}",
                )
                .with(b"param", b"parameters!")
            },
            Some(|area| Button::with_text(area, "Left".into(), theme::button_default())),
            Some(|area| Button::with_text(area, "Right".into(), theme::button_default())),
            None,
        ));
        assert_eq!(
            trace(&layout),
            r#"<Dialog content:<Text content:Testing text layout, with
some text, and some more
text. And parameters! > left:<Button text:Left > right:<Button text:Right > >"#
        )
    }
}
