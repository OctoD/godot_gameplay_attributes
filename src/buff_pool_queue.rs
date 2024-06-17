use godot::prelude::*;

#[derive(GodotClass)]
#[class(init, base=RefCounted)]
pub struct BuffPoolQueueItem {
    base: Base<RefCounted>,
    #[var]
    pub buff_id: i16,
    #[var]
    pub duration: f64,
}

#[derive(GodotClass)]
#[class(init, base=Node)]
pub struct BuffPoolQueue {
    base: Base<Node>,
    #[var]
    pub queue: Array<Gd<BuffPoolQueueItem>>,
    #[var]
    pub server_authoritative: bool,
    #[var]
    pub tick: f64,
}

#[godot_api]
impl INode for BuffPoolQueue {
    fn physics_process(&mut self, delta: f64) {
        if self.server_authoritative && self.get_server_authoritative() {
			// call the rpc to notify the clients of the tick
            self._update_tick(delta);
        } else if !self.server_authoritative {
            self._update_tick(delta);
        }
    }
}

#[godot_api]
impl BuffPoolQueue {
    #[signal]
    fn pool_tick_set(tick: f64);
    #[signal]
    fn second_ticked(tick: f64);

    fn _update_tick(&mut self, delta: f64) {
        self.tick += delta;

        let current_tick = self.tick;

        if current_tick >= 1.0 {
            self.base_mut()
                .emit_signal("second_ticked".into(), &[current_tick.to_variant()]);
            self.tick = 1.0 - current_tick;
        }
    }

    #[func]
    fn set_pool_tick(&mut self, tick: f64) {
        self.tick = tick;
        self.base_mut()
            .emit_signal("pool_tick_set".into(), &[tick.to_variant()]);
    }
}
