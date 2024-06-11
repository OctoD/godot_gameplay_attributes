use godot::prelude::*;

use super::attributes_record::AttributesRecord;

#[derive(GodotClass)]
#[class(init, base=Resource)]
pub struct AttributeTableRecord {
    base: Base<Resource>,
    /// the name of the record.
    #[export]
    pub record_name: StringName,
    /// the attributes in the record.
    #[export]
    pub attributes: Array<Gd<AttributesRecord>>,
}

#[godot_api]
impl AttributeTableRecord {
    #[signal]
    fn attribute_added(attribute: Gd<AttributesRecord>);

    #[func]
    pub fn create_attribute(&mut self, attribute_name: GString) {
        let mut new_attribute = AttributesRecord::new_gd();

        new_attribute.bind_mut().attribute_name = attribute_name.into();

        self.attributes.push(new_attribute.clone());

        self.to_gd().emit_signal(
            StringName::from("attribute_added"),
            &[new_attribute.to_variant()],
        );
    }

    #[func]
    pub fn ensure_attributes(&mut self, attributes: PackedStringArray) {
        for attribute in attributes.as_slice() {
            if !self
                .attributes
                .iter_shared()
                .any(|attr| attr.bind().attribute_name == attribute.clone())
            {
                self.create_attribute(attribute.clone());
            }
        }

        self.sort_attributes(attributes);
    }

    #[func]
    pub fn get_attribute(&self, attribute_name: GString) -> Option<Gd<AttributesRecord>> {
        self.attributes
            .iter_shared()
            .find(|attr| attr.bind().attribute_name == attribute_name)
    }

    #[func]
    pub fn sort_attributes(&mut self, attributes: PackedStringArray) {
        let mut sorted_attributes = Array::new();

        for attribute in attributes.as_slice() {
            if let Some(attr) = self.get_attribute(attribute.clone()) {
                sorted_attributes.push(attr);
            }
        }

        self.attributes = sorted_attributes;
    }

    #[func]
    pub fn remove_attribute(&mut self, attribute_name: GString) -> bool {
        if let Some(index) = self
            .attributes
            .iter_shared()
            .position(|attr| attr.bind().attribute_name == attribute_name)
        {
            self.attributes.remove(index);

            true
        } else {
            false
        }
    }
}
