use godot::prelude::*;

use crate::attribute_operation::AttributeOperation;

#[derive(GodotClass)]
#[class(init, base=Resource)]
pub struct AttributeBuff {
    base: Base<Resource>,
    #[var]
    pub attribute_name: GString,
    #[var]
    pub operation: Gd<AttributeOperation>,
}

impl PartialEq for AttributeBuff {
    fn eq(&self, other: &Self) -> bool {
        self.attribute_name == other.attribute_name && self.operation == other.operation
    }
}

#[godot_api]
impl AttributeBuff {
    #[func]
    pub fn operate(&self, base_value: f64) -> f64 {
        self.operation.bind().operate(base_value)
    }
}
