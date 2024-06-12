use godot::prelude::*;

use crate::attribute_buff::AttributeBuff;

#[derive(GodotClass)]
#[class(init, base=Resource)]
pub struct AttributeEffect {
    base: Base<Resource>,
    #[export]
    pub buffs: Array<Gd<AttributeBuff>>,
}

impl PartialEq for AttributeEffect {
    fn eq(&self, other: &Self) -> bool {
        self.buffs == other.buffs
    }
}

#[godot_api]
impl AttributeEffect {
    #[func]
    pub fn get_instant_buffs(&self) -> Array<Gd<AttributeBuff>> {
        self.buffs
            .iter_shared()
            .filter(|buff| buff.bind().duration == 0.0)
            .collect()
    }

    #[func]
    pub fn get_timed_buffs(&self) -> Array<Gd<AttributeBuff>> {
        self.buffs
            .iter_shared()
            .filter(|buff| buff.bind().duration > 0.0)
            .collect()
    }
}
