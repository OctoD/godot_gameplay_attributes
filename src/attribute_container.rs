use godot::{obj::WithBaseField, prelude::*};

use crate::{
    attribute::Attribute, attribute_buff::AttributeBuff, attribute_effect::AttributeEffect,
    buff_pool_queue::BuffPoolQueue,
};

#[derive(GodotClass)]
#[class(init, base=Node)]
pub struct AttributeContainer {
    base: Base<Node>,
    #[var]
    buff_pool_queue: Option<Gd<BuffPoolQueue>>,
    #[export]
    attributes: Array<Gd<Attribute>>,
    #[export]
    server_authoritative: bool,
}

#[godot_api]
impl INode for AttributeContainer {
    fn ready(&mut self) {
        self._install_pool_queue();
        self._connect_attributes();
    }
}

#[godot_api]
impl AttributeContainer {
    fn _connect_attribute(&self, mut attribute: Gd<Attribute>) {
        attribute.connect(
            "attribute_changed".into(),
            Callable::from_object_method(&self.base(), "_on_attribute_changed"),
        );
    }

    fn _connect_attributes(&self) {
        for mut attribute in self.attributes.iter_shared() {
            attribute.bind_mut().setup_underlying_value();
            // self._connect_attribute(attribute);
        }
    }

    fn _install_pool_queue(&mut self) {
        let mut buff_pool_queue = BuffPoolQueue::new_alloc();
        buff_pool_queue.bind_mut().server_authoritative = self.server_authoritative;
        buff_pool_queue.connect(
            "attribute_buff_dequeued".into(),
            Callable::from_object_method(&self.base(), "_on_attribute_buff_dequeued"),
        );
        buff_pool_queue.connect(
            "attribute_buff_enqueued".into(),
            Callable::from_object_method(&self.base(), "_on_attribute_buff_enqueued"),
        );
        self.buff_pool_queue = Some(buff_pool_queue.clone());
        self.base_mut().add_child(buff_pool_queue.upcast());
    }

    #[func]
    fn _emit_attribute_buff_added(&mut self, buff: Gd<AttributeBuff>) {
        self.base_mut()
            .emit_signal("attribute_buff_added".into(), &[buff.to_variant()]);
    }

    #[func]
    fn _on_attribute_buff_dequeued(&mut self, buff: Gd<AttributeBuff>) {
        self.base_mut()
            .emit_signal("attribute_buff_dequeued".into(), &[buff.to_variant()]);

        for mut attribute in self.attributes.iter_shared() {
            attribute.bind_mut().remove_buff(buff.clone());
        }
    }

    #[func]
    fn _on_attribute_buff_enqueued(&mut self, buff: Gd<AttributeBuff>) {
        self.base_mut()
            .emit_signal("attribute_buff_enqueued".into(), &[buff.to_variant()]);
    }

    #[func]
    fn _on_attribute_changed(
        &mut self,
        attribute: Gd<Attribute>,
        previous_value: f64,
        new_value: f64,
    ) {
        self.base_mut().emit_signal(
            "attribute_changed".into(),
            &[
                attribute.to_variant(),
                previous_value.to_variant(),
                new_value.to_variant(),
            ],
        );
    }

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

    #[signal]
    fn attribute_buff_enqueued(buff: Gd<AttributeBuff>);

    #[signal]
    fn attribute_buff_dequeued(buff: Gd<AttributeBuff>);

    #[func]
    fn add_attribute(&mut self, mut attribute: Gd<Attribute>) {
        if !self.attributes.contains(&attribute) {
            attribute.bind_mut().setup_underlying_value();
            self.attributes.push(attribute.clone());
            self._connect_attribute(attribute.clone());
        }
    }

    #[func]
    fn apply_buff(&mut self, buff: Gd<AttributeBuff>) {
        let mut added = false;

        for mut attribute in self.attributes.iter_shared() {
            let mut m_attr = attribute.bind_mut();

            if m_attr.can_receive_buff(buff.clone()) {
                added = true;

                m_attr.add_buff(buff.clone());

                if buff.bind().duration > 0.0 {
                    if let Some(buff_pool_queue) = self.buff_pool_queue.as_mut() {
                        buff_pool_queue.bind_mut().add_attribute_buff(buff.clone());
                    }
                }
            }
        }

        if added {
            self._emit_attribute_buff_added(buff.clone());
        }
    }

    #[func]
    fn apply_effect(&mut self, effect: Gd<AttributeEffect>) {
        effect.bind().buffs.iter_shared().for_each(|buff| {
            self.apply_buff(buff);
        });
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
            let attrbound = attr.bind();

            sub_dict.insert("attribute_name", attrbound.attribute_name.clone());
            sub_dict.insert("attribute_value", attrbound.current_value.clone());
            sub_dict.insert("initial_value", attrbound.initial_value.clone());
            sub_dict.insert("max_value", attrbound.max_value.clone());
            sub_dict.insert("min_value", attrbound.min_value.clone());
            sub_dict.insert("buffs", attrbound.attribute_buffs.clone());

            dict.insert(attr.bind().attribute_name.clone(), sub_dict);
        }

        dict
    }
}
