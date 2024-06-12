use godot::prelude::*;

use crate::attribute_operation::AttributeOperation;

pub enum BuffType {
    Immediate,
    Persistent,
}

impl From<u8> for BuffType {
    fn from(value: u8) -> Self {
        match value {
            0 => BuffType::Immediate,
            1 => BuffType::Persistent,
            _ => BuffType::Immediate,
        }
    }
}

impl Into<u8> for BuffType {
    fn into(self) -> u8 {
        match self {
            BuffType::Immediate => 0,
            BuffType::Persistent => 1,
        }
    }
}

#[derive(GodotClass)]
#[class(init, base=Resource)]
pub struct AttributeBuff {
    base: Base<Resource>,
    #[export]
    pub attribute_name: GString,
    #[export]
    pub buff_name: GString,
    #[export(enum = (Immediate, Persistent))]
    pub buff_type: u8,
    #[export]
    pub duration: f64,
    #[export]
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
