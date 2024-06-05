use godot::prelude::*;

use crate::attribute_operation::AttributeOperation;

#[derive(GodotClass)]
#[class(init, base=RefCounted)]
pub struct AttributeBuff {
    base: Base<RefCounted>,
    #[var]
    pub attribute_name: GString,
    #[var]
    pub op: Gd<AttributeOperation>,
}

impl PartialEq for AttributeBuff {
    fn eq(&self, other: &Self) -> bool {
        self.attribute_name == other.attribute_name && self.op == other.op
    }
}

#[godot_api]
impl AttributeBuff {
    #[func]
    pub fn operate(&self, base_value: f64) -> f64 {
        self.op.bind().operate(base_value)
    }
}
