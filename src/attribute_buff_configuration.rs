use godot::prelude::*;

pub trait IAttributeBuffConfiguration {
    fn can_apply(&self) -> bool;
    fn on_apply(&mut self);
    fn should_remove(&self) -> bool;
}

#[derive(GodotClass)]
#[class(init, base=Resource)]
pub struct AttributeBuffConfiguration {
    base: Base<Resource>,
}

#[derive(GodotClass)]
#[class(init, base=Resource)]
pub struct TimedAttributeBuffConfiguration {
    base: Base<Resource>,
    #[export]
    pub time_duration: f64,
}

#[godot_api]
impl IAttributeBuffConfiguration for TimedAttributeBuffConfiguration {
    fn can_apply(&self) -> bool {
        self.time_duration > 0.0
    }

    fn on_apply(&mut self) {
		
	}

    fn should_remove(&self) -> bool {
        self.time_duration <= 0.0
    }
}
