use godot::prelude::*;

use crate::attribute_buff::{AttributeBuff, BuffType};

#[derive(GodotClass)]
#[class(init, base=Resource)]
pub struct Attribute {
    base: Base<Resource>,
    #[export]
    pub attribute_buffs: Array<Gd<AttributeBuff>>,
    #[export]
    pub attribute_name: GString,
    #[var(get = get_buffed_value)]
    pub current_value: f64,
    #[export]
    pub initial_value: f64,
    #[export]
    pub max_value: f64,
    #[export]
    pub min_value: f64,
    // this will never get public. Get away from me.
    underlying_value: f64,
}

impl PartialEq for Attribute {
    fn eq(&self, other: &Self) -> bool {
        self.attribute_name == other.attribute_name
    }
}

#[godot_api]
impl Attribute {
    #[signal]
    fn attribute_changed(self, prev_value: f64, new_value: f64);
    #[signal]
    fn buff_added(buff: Gd<AttributeBuff>);
    #[signal]
    fn buff_removed(buff: Gd<AttributeBuff>);

    /// Adds a buff to the attribute. If the buff is not of the same type as the attribute, it will not be added.
    #[func]
    pub fn add_buff(&mut self, buff: Gd<AttributeBuff>) -> bool {
        if self.can_receive_buff(buff.clone()) {
            if buff.bind().buff_type == BuffType::OneShot as u8 {
                let prev_value = self.underlying_value;
                self.underlying_value = buff.bind().operate(self.underlying_value);
                let underlying_value = self.underlying_value;

                if underlying_value != prev_value {
                    let selfclone = self.to_gd().clone();
                    
                    self.base_mut().emit_signal(
                        StringName::from("attribute_changed"),
                        &[
                            selfclone.to_variant(),
                            prev_value.to_variant(),
                            underlying_value.to_variant(),
                        ],
                    );
                }
            } else {
                self.attribute_buffs.push(buff.clone());
                self.to_gd()
                    .emit_signal(StringName::from("buff_added"), &[buff.to_variant()]);
            }

            return true;
        }

        false
    }

    /// Adds multiple buffs to the attribute. If a buff is not of the same type as the attribute, it will not be added.
    #[func]
    pub fn add_buffs(&mut self, buffs: Array<Gd<AttributeBuff>>) -> u32 {
        let mut count: u32 = 0;

        for buff in buffs.iter_shared() {
            if self.add_buff(buff) {
                count += 1;
            }
        }

        count
    }

    /// Returns true if the attribute can receive the buff, false otherwise.
    #[func]
    pub fn can_receive_buff(&self, buff: Gd<AttributeBuff>) -> bool {
        buff.bind().attribute_name == self.attribute_name
    }

    #[func]
    pub fn clear_buffs(&mut self) {
        self.attribute_buffs.clear();
    }

    /// Returns the buffed value of the attribute.
    #[func]
    pub fn get_buffed_value(&self) -> f64 {
        let underlying_value = self.underlying_value;
        self.attribute_buffs
            .iter_shared()
            .fold(underlying_value, |acc, buff| buff.bind().operate(acc))
    }

    /// Returns true if the attribute has the buff, false otherwise.
    #[func]
    pub fn has_buff(&self, buff: Gd<AttributeBuff>) -> bool {
        for b in self.attribute_buffs.iter_shared() {
            if b == buff {
                return true;
            }
        }

        false
    }

    /// Removes the first buff found of a specific type on the attribute.
    #[func]
    pub fn remove_buff(&mut self, buff: Gd<AttributeBuff>) -> bool {
        let mut index: usize = 0;

        for x in self.attribute_buffs.iter_shared() {
            if x == buff {
                self.attribute_buffs.remove(index);
                self.base_mut()
                    .emit_signal(StringName::from("buff_removed"), &[buff.to_variant()]);
                return true;
            }
            index += 1usize;
        }

        false
    }

    /// Removes all buffs of a specific type on the attribute.
    #[func]
    pub fn remove_all_buffs_of_type(&mut self, buff: Gd<AttributeBuff>) -> u32 {
        let mut indexes: Vec<usize> = Vec::new();

        for (index, x) in self.attribute_buffs.iter_shared().enumerate() {
            if x.bind().buff_name == buff.bind().buff_name {
                indexes.push(index);
            }
        }

        for index in indexes.iter().rev() {
            self.attribute_buffs.remove(*index);
        }

        indexes.len() as u32
    }

    /// Removes multiple buffs from the attribute.
    #[func]
    pub fn remove_buffs(&mut self, buffs: Array<Gd<AttributeBuff>>) -> u32 {
        let mut count: u32 = 0;

        for buff in buffs.iter_shared() {
            if self.remove_buff(buff) {
                count += 1;
            }
        }

        count
    }

    /// Privately sets the underlying value of the attribute.
    #[func]
    pub fn setup_underlying_value(&mut self) {
        self.underlying_value = self.initial_value;
    }
}
