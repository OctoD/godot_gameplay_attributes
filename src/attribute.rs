use godot::prelude::*;

use crate::{attribute_buff::AttributeBuff, attribute_operation::AttributeOperation};

#[derive(GodotClass)]
#[class(init, base=Resource)]
pub struct Attribute {
    base: Base<Resource>,
    #[var]
    pub attribute_buffs: Array<Gd<AttributeBuff>>,
    #[var]
    pub attribute_name: GString,
    #[var(get = get_buffed_value)]
    pub current_value: f64,
    #[var]
    pub initial_value: f64,
    #[var]
    pub max_value: f64,
    #[var]
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
    #[func]
    pub fn add_buff(&mut self, buff: Gd<AttributeBuff>) {
        if self.can_receive_buff(buff.clone()) {
            self.attribute_buffs.push(buff);
        }
    }

    #[func]
    pub fn add_buffs(&mut self, buffs: Array<Gd<AttributeBuff>>) {
        for buff in buffs.iter_shared() {
            if self.can_receive_buff(buff.clone()) {
                self.attribute_buffs.push(buff);
            }
        }
    }

    #[func]
    pub fn can_receive_buff(&self, buff: Gd<AttributeBuff>) -> bool {
        buff.bind().attribute_name == self.attribute_name
    }

    #[func]
    pub fn get_buffed_value(&self) -> f64 {
        self.attribute_buffs
            .iter_shared()
            .fold(self.underlying_value, |acc, buff| buff.bind().operate(acc))
    }

    #[func]
    pub fn has_buff(&self, buff: Gd<AttributeBuff>) -> bool {
        for b in self.attribute_buffs.iter_shared() {
            if b == buff {
                return true;
            }
        }

        false
    }

    #[func]
    pub fn operate(&mut self, op: AttributeOperation, value: f64) {
        self.underlying_value = op.operate(self.underlying_value, value)
    }

    #[func]
    pub fn remove_buff(&mut self, buff: Gd<AttributeBuff>) {
        let mut index: usize = 0;

        for x in self.attribute_buffs.iter_shared() {
            if x == buff {
                break;
            }

            index += 1usize;
        }

        self.attribute_buffs.remove(index);
    }

    #[func]
    pub fn remove_buffs(&mut self, buffs: Array<Gd<AttributeBuff>>) {
        for buff in buffs.iter_shared() {
            self.remove_buff(buff);
        }
    }
}
