use godot::prelude::*;

use crate::attribute::Attribute;

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

    #[func]
    fn from_dictionary(&mut self, dict: Dictionary) {
        for key in dict.keys_array().iter_shared() {
            let key = key.to_string();

            if let Some(sub_dict) = dict.get(key.clone()) {
                let mut attribute = Attribute::new_gd();

                attribute.bind_mut().attribute_name = key.into();

                if let Ok(casted_sub_dict) = sub_dict.try_to::<Dictionary>() {
                    attribute.bind_mut().attribute_buffs =
                        self._try_cast_to_or_default(Array::new(), casted_sub_dict.get("buffs"));

                    attribute.bind_mut().current_value =
                        self._try_cast_to_or_default(0.0, casted_sub_dict.get("attribute_value"));
                    attribute.bind_mut().initial_value =
                        self._try_cast_to_or_default(0.0, casted_sub_dict.get("initial_value"));
                    attribute.bind_mut().max_value =
                        self._try_cast_to_or_default(0.0, casted_sub_dict.get("max_value"));
                    attribute.bind_mut().min_value =
                        self._try_cast_to_or_default(0.0, casted_sub_dict.get("min_value"));
                }

                self.attributes.push(attribute.into());
            }
        }
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
