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
    #[var]
    pub current_value: f32,
    #[var]
    pub initial_value: f32,
    #[var]
    pub max_value: f32,
    #[var]
    pub min_value: f32,
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
        if !self.has_buff(buff.clone()) {
            self.attribute_buffs.push(buff);
        }
    }

    #[func]
    pub fn add_buffs(&mut self, buffs: Array<Gd<AttributeBuff>>) {
        for buff in buffs.iter_shared() {
            self.attribute_buffs.push(buff);
        }
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
	pub fn operate(&mut self, op: AttributeOperation, value: f32) {
		match op {
			AttributeOperation::Add => self.current_value += value,
			AttributeOperation::Multiply => self.current_value *= value,
			AttributeOperation::Subtract => self.current_value -= value,
			AttributeOperation::Divide => self.current_value /= value,
		}
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

    #[func]
    pub fn get_buffed_value(&self) -> f32 {
        self.attribute_buffs
            .iter_shared()
            .fold(self.current_value, |acc, buff| {
                let b = buff.bind();

                match &b.op {
                    AttributeOperation::Add => acc + b.value,
                    AttributeOperation::Multiply => acc * b.value,
                    AttributeOperation::Subtract => acc - b.value,
                    AttributeOperation::Divide => acc / b.value,
                }
            })
    }
}
