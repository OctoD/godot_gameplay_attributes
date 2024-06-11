use godot::prelude::*;

#[derive(GodotClass)]
#[class(init, base=Resource)]
pub struct AttributesRecord {
    base: Base<Resource>,
    /// the name of the attribute.
    #[export]
    pub attribute_name: GString,
    /// the value of the attribute.
    #[export]
    pub initial_value: f64,
    /// the minimum value of the attribute.
    /// If the attribute is set to a value lower than this, it will be clamped to this value.
    #[export]
    pub min_value: f64,
    /// the maximum value of the attribute.
    /// If the attribute is set to a value higher than this, it will be clamped to this value.
    #[export]
    pub max_value: f64,
}

impl PartialEq for AttributesRecord {
    fn eq(&self, other: &Self) -> bool {
        self.attribute_name == other.attribute_name
            && self.initial_value == other.initial_value
            && self.min_value == other.min_value
            && self.max_value == other.max_value
    }
}

#[godot_api]
impl AttributesRecord {
}
