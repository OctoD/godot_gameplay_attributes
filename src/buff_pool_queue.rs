use godot::prelude::*;

use crate::attribute_buff::AttributeBuff;

#[derive(GodotClass)]
#[class(init, base=RefCounted)]
pub struct BuffPoolQueueItem {
    base: Base<RefCounted>,
    buff: Gd<AttributeBuff>,
    eligible_for_removal: bool,
    seconds_remaining: f64,
}

impl BuffPoolQueueItem {
    pub fn set_buff(&mut self, buff: Gd<AttributeBuff>) -> &mut Self {
        self.seconds_remaining = buff.bind().duration;
        self.buff = buff;
        self
    }

    pub fn second_passed(&mut self) -> &mut Self {
        self.seconds_remaining -= 1.0;
        self.eligible_for_removal = self.seconds_remaining <= 0.0;
        self
    }
}

#[derive(GodotClass)]
#[class(init, base=Node)]
pub struct BuffPoolQueue {
    base: Base<Node>,
    #[var]
    pub queue: Array<Gd<BuffPoolQueueItem>>,
    #[var]
    pub server_authoritative: bool,
    /// Ticks counter
    tick: f64,
}

#[godot_api]
impl INode for BuffPoolQueue {
    fn physics_process(&mut self, delta: f64) {
        self.tick += delta;

        if self.tick >= 1.0 {
            self.tick = self.tick - 1.0;
            self.process_items();
        }
    }
}

#[godot_api]
impl BuffPoolQueue {
    #[signal]
    fn attribute_buff_dequeued(buff: Gd<AttributeBuff>);
    #[signal]
    fn attribute_buff_enqueued(buff: Gd<AttributeBuff>);

    #[func]
    pub fn add_attribute_buff(&mut self, buff: Gd<AttributeBuff>) {
        if self.server_authoritative && !self.base().is_multiplayer_authority() {
            return;
        }

        let gdbuff = BuffPoolQueueItem::new_gd()
            .bind_mut()
            .set_buff(buff)
            .to_gd();
        self.base_mut().emit_signal(
            "attribute_buff_enqueued".into(),
            &[gdbuff.clone().to_variant()],
        );
        self.queue.push(gdbuff);
    }

    #[func]
    pub fn cleanup(&mut self) {
        if self.server_authoritative && !self.base().is_multiplayer_authority() {
            return;
        }

        let mut indexes: Vec<usize> = vec![];
        let index = 0;

        for item in self.queue.iter_shared() {
            if item.bind().eligible_for_removal {
                indexes.push(index);
            }
        }

        for i in indexes.iter().rev() {
            if let Some(item) = self.queue.get(*i) {
                self.base_mut().emit_signal(
                    "attribute_buff_dequeued".into(),
                    &[item.bind().buff.clone().to_variant()],
                );
                self.queue.remove(*i);
            }
        }
    }

    fn process_items(&mut self) {
        if self.server_authoritative && !self.base().is_multiplayer_authority() {
            return;
        }

        for mut item in self.queue.iter_shared() {
            item.bind_mut().second_passed();
        }

        self.cleanup();
    }
}
