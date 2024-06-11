use godot::prelude::*;

use super::attributes_table_record::AttributeTableRecord;

#[derive(GodotClass)]
#[class(init, base=Resource)]
pub struct AttributeTable {
    base: Base<Resource>,
    /// the attributes in the table.
    #[export]
    attributes: PackedStringArray,
    /// the records of the attributes in the table.
    #[export]
    records: Array<Gd<AttributeTableRecord>>,
}

#[godot_api]
impl AttributeTable {
    /// emitted when a new attribute is defined
    #[signal]
    fn attribute_defined(attribute: StringName);
    /// emitted when an attribute is deleted
    #[signal]
    fn attribute_deleted(attribute: StringName);
    /// emitted when an attribute is renamed
    #[signal]
    fn attribute_renamed(old_name: StringName, new_name: StringName);
    /// emitted when the attributes are sorted
    #[signal]
    fn attribute_sorted();

    #[func]
    pub fn add_attribute(&mut self, attribute_name: StringName) -> bool {
        let as_gstring = GString::from(attribute_name);

        if self.attributes.contains(&as_gstring) {
            return false;
        }

        self.attributes.push(as_gstring.clone());

        self.records.iter_shared().for_each(|mut record| {
            record.bind_mut().create_attribute(as_gstring.clone());
        });

        self.to_gd().emit_signal(
            StringName::from("attribute_defined"),
            &[as_gstring.to_variant()],
        );

        true
    }

    #[func]
    pub fn ensure_attributes(&mut self, attributes: PackedStringArray) {
        self.records.iter_shared().for_each(|mut record| {
            record.bind_mut().ensure_attributes(attributes.clone());
        });
        self.sort_attributes();
    }

    /// Returns true if the record exists in the table, false otherwise.
    #[func]
    pub fn has_record(&self, record_name: StringName) -> bool {
        self.records
            .iter_shared()
            .any(|record| record.bind().record_name == record_name)
    }

    #[func]
    pub fn remove_attribute(&mut self, attribute_name: StringName) -> bool {
        let as_gstring = GString::from(attribute_name);

        if !self.attributes.contains(&as_gstring) {
            return false;
        }

        if let Some(index) = self.attributes.find(&as_gstring, None) {
            self.attributes.remove(index);

            self.records.iter_shared().for_each(|mut record| {
                record.bind_mut().remove_attribute(as_gstring.clone());
            });

            self.to_gd().emit_signal(
                StringName::from("attribute_deleted"),
                &[as_gstring.to_variant()],
            );

            true
        } else {
            false
        }
    }

    /// Sorts the attributes in the table.
    #[func]
    pub fn sort_attributes(&mut self) {
        self.attributes.sort();
        self.to_gd()
            .emit_signal(StringName::from("attribute_sorted"), &[]);
    }
}
