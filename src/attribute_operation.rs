use godot::prelude::*;

enum Operand {
    Add,
    Divide,
    Multiply,
    Percentage,
    Subtract,
    None,
}

impl From<u8> for Operand {
    fn from(value: u8) -> Self {
        match value {
            0 => Operand::Add,
            1 => Operand::Divide,
            2 => Operand::Multiply,
            3 => Operand::Percentage,
            4 => Operand::Subtract,
            _ => Operand::None,
        }
    }
}

#[derive(GodotClass)]
#[class(init, base=Resource)]
pub struct AttributeOperation {
    base: Base<Resource>,
    /// the operand used. It's converted to Operand enum in the operate method.
    #[export(enum = (Add, Divide, Multiply, Percentage, Subtract))]
    operand: u8,
    /// the value to operate with.
    #[export]
    value: f64,
}

impl PartialEq for AttributeOperation {
    fn eq(&self, other: &Self) -> bool {
        self.operand == other.operand && self.value == other.value
    }
}

#[godot_api]
impl AttributeOperation {
    #[func]
    fn add(value: f64) -> Gd<Self> {
        Gd::from_init_fn(|base| Self {
            base,
            operand: 0,
            value,
        })
    }

    #[func]
    fn divide(value: f64) -> Gd<Self> {
        Gd::from_init_fn(|base| Self {
            base,
            operand: 1,
            value,
        })
    }

    #[func]
    fn multiply(value: f64) -> Gd<Self> {
        Gd::from_init_fn(|base| Self {
            base,
            operand: 2,
            value,
        })
    }

    #[func]
    fn percentage(value: f64) -> Gd<Self> {
        Gd::from_init_fn(|base| Self {
            base,
            operand: 3,
            value,
        })
    }

    #[func]
    fn subtract(value: f64) -> Gd<Self> {
        Gd::from_init_fn(|base| Self {
            base,
            operand: 4,
            value,
        })
    }
    
    #[func]
    pub fn operate(&self, &base_value: f64) -> f64 {
        match Operand::from(self.operand) {
            Operand::Add => base_value + self.value,
            Operand::Divide => base_value / self.value,
            Operand::Multiply => base_value * self.value,
            Operand::Percentage => base_value + ((base_value / 100.0) * self.value),
            Operand::Subtract => base_value - self.value,
            Operand::None => base_value,
        }
    }
}
