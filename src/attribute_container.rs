use godot::prelude::*;

use crate::{
    attribute::Attribute, attribute_buff::AttributeBuff, attribute_effect::AttributeEffect,
};

#[derive(GodotClass)]
#[class(init, base=Node)]
pub struct AttributeContainer {
    base: Base<Node>,
    #[var]
    attributes: Array<Gd<Attribute>>,
}

#[godot_api]
impl AttributeContainer {
    fn _try_cast_to<T: FromGodot>(&self, variant: Option<Variant>) -> Option<T> {
        match variant {
            Some(variant) => match variant.try_to::<T>() {
                Ok(value) => Some(value),
                Err(_) => None,
            },
            None => None,
        }
    }

    fn _try_cast_to_or_default<T: FromGodot + Default>(
        &self,
        default: T,
        variant: Option<Variant>,
    ) -> T {
        match variant {
            Some(variant) => match variant.try_to::<T>() {
                Ok(value) => value,
                Err(_) => default,
            },
            None => default,
        }
    }

    #[signal]
    fn attribute_buff_added(attribute: Gd<Attribute>, buff: Gd<AttributeBuff>);

    #[signal]
    fn attribute_buff_removed(attribute: Gd<Attribute>, buff: Gd<AttributeBuff>);

    #[signal]
    fn attribute_changed(attribute: Gd<Attribute>, previous_value: f64, new_value: f64);

    #[func]
    fn add_attribute(&mut self, attribute: Gd<Attribute>) {
        if !self.attributes.contains(&attribute) {
            self.attributes.push(attribute);
        }
    }

    #[func]
    fn apply_effect(&mut self, effect: Gd<AttributeEffect>) {
        effect.bind().buffs.iter_shared().for_each(|buff| {
            self.apply_buff(buff);
        });
    }

    #[func]
    fn apply_buff(&mut self, buff: Gd<AttributeBuff>) {
        for mut attribute in self.attributes.iter_shared() {
            let mut m_attr = attribute.bind_mut();

            if m_attr.can_receive_buff(buff.clone()) {
                m_attr.add_buff(buff.clone());
            }
        }
    }

    #[func]
    fn from_dictionary(&mut self, dict: Dictionary) {
        for key in dict.keys_array().iter_shared() {
            let key = key.to_string();

            if let Some(sub_dict) = dict.get(key.clone()) {
                let mut attribute = Attribute::new_gd();

                attribute.bind_mut().attribute_name = key.into();

                if let Ok(casted_sub_dict) = sub_dict.try_to::<Dictionary>() {
                    let mut mut_attr = attribute.bind_mut();

                    mut_attr.attribute_buffs =
                        self._try_cast_to_or_default(Array::new(), casted_sub_dict.get("buffs"));
                    mut_attr.current_value =
                        self._try_cast_to_or_default(0.0, casted_sub_dict.get("attribute_value"));
                    mut_attr.initial_value =
                        self._try_cast_to_or_default(0.0, casted_sub_dict.get("initial_value"));
                    mut_attr.max_value =
                        self._try_cast_to_or_default(0.0, casted_sub_dict.get("max_value"));
                    mut_attr.min_value =
                        self._try_cast_to_or_default(0.0, casted_sub_dict.get("min_value"));
                }

                self.attributes.push(attribute.into());
            }
        }
    }

    #[func]
    pub fn remove_buff(&mut self, buff: Gd<AttributeBuff>) -> bool {
        for mut attribute in self.attributes.iter_shared() {
            if attribute.bind_mut().remove_buff(buff.clone()) {
                return true;
            }
        }

        false
    }

    #[func]
    pub fn remove_effect(&mut self, effect: Gd<AttributeEffect>) -> u32 {
        let mut count = 0;

        effect.bind().buffs.iter_shared().for_each(|buff| {
            if self.remove_buff(buff) {
                count += 1;
            }
        });

        count
    }

    #[func]
    fn to_dictionary(&self) -> Dictionary {
        let mut dict = Dictionary::new();

        for attr in self.attributes.iter_shared() {
            let mut sub_dict = Dictionary::new();

            sub_dict.insert("attribute_name", attr.bind().attribute_name.clone());
            sub_dict.insert("attribute_value", attr.bind().current_value.clone());
            sub_dict.insert("initial_value", attr.bind().initial_value.clone());
            sub_dict.insert("max_value", attr.bind().max_value.clone());
            sub_dict.insert("min_value", attr.bind().min_value.clone());
            sub_dict.insert("buffs", attr.bind().attribute_buffs.clone());

            dict.insert(attr.bind().attribute_name.clone(), sub_dict);
        }

        dict
    }
}
