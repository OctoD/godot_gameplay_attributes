use godot::prelude::*;

use crate::attribute_operation::AttributeOperation;


#[derive(GodotClass)]
#[class(init, base=RefCounted)]
pub struct AttributeBuff {
    base: Base<RefCounted>,
    #[var]
    pub id: u32,
    #[var]
    pub op: AttributeOperation,
    #[var]
    pub value: f32,
}

impl PartialEq for AttributeBuff {
    fn eq(&self, other: &Self) -> bool {
        self.id == other.id && self.op == other.op && self.value == other.value
    }
}
