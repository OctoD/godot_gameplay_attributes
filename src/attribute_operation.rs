use godot::prelude::*;

#[derive(GodotConvert, Var, Export, Default, Debug)]
#[godot(via = i32)]
pub enum AttributeOperation {
    #[default]
    Add,
    Multiply,
    Subtract,
    Divide,
}

impl PartialEq for AttributeOperation {
    fn eq(&self, other: &Self) -> bool {
        core::mem::discriminant(self) == core::mem::discriminant(other)
    }
}
