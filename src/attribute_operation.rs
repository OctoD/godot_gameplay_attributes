use godot::prelude::*;

#[derive(GodotConvert, Var, Export, Default, Debug)]
#[godot(via = i32)]
pub enum AttributeOperation {
    #[default]
    Add,
    Multiply,
    Percentage,
    Subtract,
    Divide,
}

impl PartialEq for AttributeOperation {
    fn eq(&self, other: &Self) -> bool {
        core::mem::discriminant(self) == core::mem::discriminant(other)
    }
}

impl AttributeOperation {
    pub fn operate(&self, base_value: f64, op_value: f64) -> f64 {
        match self {
            AttributeOperation::Add => base_value + op_value,
            AttributeOperation::Multiply => base_value * op_value,
            AttributeOperation::Percentage => base_value + ((base_value / 100.0) * op_value),
            AttributeOperation::Subtract => base_value - op_value,
            AttributeOperation::Divide => base_value / op_value,
        }
    }
}
