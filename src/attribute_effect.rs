use godot::prelude::*;

#[derive(GodotClass)]
#[class(init, base=Resource)]
pub struct AttributeEffect {
    base: Base<Resource>,
    #[export]
    pub attribute_name: GString,
    #[export]
    pub buffs: Array<Gd<AttributeEffect>>,
    #[export]
    pub value: f64,
}

impl PartialEq for AttributeEffect {
    fn eq(&self, other: &Self) -> bool {
        self.attribute_name == other.attribute_name && self.buffs == other.buffs
    }
}

#[godot_api]
impl AttributeEffect {}
