/**************************************************************************/
/*  attribute_container.cpp                                               */
/**************************************************************************/
/*                         This file is part of:                          */
/*                        Godot Gameplay Systems                          */
/*              https://github.com/OctoD/godot-gameplay-systems           */
/**************************************************************************/
/* Copyright (c) 2020-present Paolo "OctoD"      Roth (see AUTHORS.md).   */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "attribute_container.hpp"

#include "attribute.hpp"
#include "buff_pool_queue.hpp"

using namespace gga;

void AttributeContainer::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("_on_attribute_changed", "p_attribute", "p_previous_value", "p_new_value"), &AttributeContainer::_on_attribute_changed);
	ClassDB::bind_method(D_METHOD("_on_buff_applied", "p_buff"), &AttributeContainer::_on_buff_applied);
	ClassDB::bind_method(D_METHOD("_on_buff_dequeued", "p_buff"), &AttributeContainer::_on_buff_dequeued);
	ClassDB::bind_method(D_METHOD("_on_buff_enqueued", "p_buff"), &AttributeContainer::_on_buff_enqueued);
	ClassDB::bind_method(D_METHOD("_on_buff_removed", "p_buff"), &AttributeContainer::_on_buff_removed);
	ClassDB::bind_method(D_METHOD("add_attribute", "p_attribute"), &AttributeContainer::add_attribute);
	ClassDB::bind_method(D_METHOD("apply_buff", "p_buff"), &AttributeContainer::apply_buff);
	ClassDB::bind_method(D_METHOD("find", "p_predicate"), &AttributeContainer::find);
	ClassDB::bind_method(D_METHOD("find_buffed_value", "p_predicate"), &AttributeContainer::find_buffed_value);
	ClassDB::bind_method(D_METHOD("find_value", "p_predicate"), &AttributeContainer::find_value);
	ClassDB::bind_method(D_METHOD("get_attribute_set"), &AttributeContainer::get_attribute_set);
	ClassDB::bind_method(D_METHOD("get_attributes"), &AttributeContainer::get_attributes);
	ClassDB::bind_method(D_METHOD("get_attribute_by_name", "p_name"), &AttributeContainer::get_attribute_by_name);
	ClassDB::bind_method(D_METHOD("get_attribute_buffed_value_by_name", "p_name"), &AttributeContainer::get_attribute_buffed_value_by_name);
	ClassDB::bind_method(D_METHOD("get_attribute_value_by_name", "p_name"), &AttributeContainer::get_attribute_value_by_name);
	ClassDB::bind_method(D_METHOD("get_server_authoritative"), &AttributeContainer::get_server_authoritative);
	ClassDB::bind_method(D_METHOD("remove_attribute", "p_attribute"), &AttributeContainer::remove_attribute);
	ClassDB::bind_method(D_METHOD("remove_buff", "p_buff"), &AttributeContainer::remove_buff);
	ClassDB::bind_method(D_METHOD("set_attribute_set", "p_attribute_set"), &AttributeContainer::set_attribute_set);
	ClassDB::bind_method(D_METHOD("set_server_authoritative", "p_server_authoritative"), &AttributeContainer::set_server_authoritative);
	ClassDB::bind_method(D_METHOD("setup"), &AttributeContainer::setup);

	/// binds properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "attribute_set", PROPERTY_HINT_RESOURCE_TYPE, "AttributeSet"), "set_attribute_set", "get_attribute_set");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "server_authoritative"), "set_server_authoritative", "get_server_authoritative");

	/// signals binding
	ADD_SIGNAL(MethodInfo("attribute_changed", PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "RuntimeAttributeBase"), PropertyInfo(Variant::FLOAT, "previous_value"), PropertyInfo(Variant::FLOAT, "new_value")));
	ADD_SIGNAL(MethodInfo("buff_applied", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "RuntimeBuff")));
	ADD_SIGNAL(MethodInfo("buff_dequed", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "RuntimeBuff")));
	ADD_SIGNAL(MethodInfo("buff_enqued", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "RuntimeBuff")));
	ADD_SIGNAL(MethodInfo("buff_removed", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "RuntimeBuff")));
}

void AttributeContainer::_on_attribute_changed(Ref<RuntimeAttribute> p_attribute, const float p_previous_value, const float p_new_value)
{
	emit_signal("attribute_changed", p_attribute, p_previous_value, p_new_value);
	notify_derived_attributes(p_attribute);
}

void AttributeContainer::_on_buff_applied(Ref<RuntimeBuff> p_buff)
{
	emit_signal("buff_applied", p_buff);
}

void AttributeContainer::_on_buff_dequeued(Ref<RuntimeBuff> p_buff)
{
	emit_signal("buff_dequed", p_buff);
	remove_buff(RuntimeBuff::to_buff(p_buff));
}

void AttributeContainer::_on_buff_enqueued(Ref<RuntimeBuff> p_buff)
{
	emit_signal("buff_enqued", p_buff);
}

void AttributeContainer::_on_buff_removed(Ref<RuntimeBuff> p_buff)
{
	emit_signal("buff_removed", p_buff);
}

bool AttributeContainer::has_attribute(Ref<AttributeBase> p_attribute)
{
	return attributes.has(p_attribute->get_attribute_name());
}

void AttributeContainer::notify_derived_attributes(Ref<RuntimeAttribute> p_runtime_attribute)
{
	if (derived_attributes.has(p_runtime_attribute->get_attribute()->get_attribute_name())) {
		TypedArray<RuntimeAttribute> derived = derived_attributes[p_runtime_attribute->get_attribute()->get_attribute_name()];
		Callable attribute_changed_callable = Callable::create(this, "_on_attribute_changed");

		for (int i = 0; i < derived.size(); i++) {
			Ref<RuntimeAttribute> derived_attribute = derived[i];
			float previous_value = derived_attribute->get_value();
			float current_value = derived_attribute->get_buffed_value();

			if (previous_value != current_value) {
				derived_attribute->emit_signal("attribute_changed", derived_attribute, previous_value, current_value);
			}
		}
	}
}

void AttributeContainer::_physics_process(double p_delta)
{
	if (buff_pool_queue) {
		buff_pool_queue->handle_physics_process(p_delta);
	}
}

void AttributeContainer::_ready()
{
	/// initializes the BuffPoolQueue
	buff_pool_queue = memnew(BuffPoolQueue);
	buff_pool_queue->set_server_authoritative(server_authoritative);
	buff_pool_queue->connect("attribute_buff_dequeued", Callable::create(this, "_on_buff_dequeued"));
	buff_pool_queue->connect("attribute_buff_enqueued", Callable::create(this, "_on_buff_enqueued"));

	add_child(buff_pool_queue);
	setup();
}

void AttributeContainer::add_attribute(Ref<AttributeBase> p_attribute)
{
	ERR_FAIL_NULL_MSG(p_attribute, "Attribute cannot be null, it must be an instance of a class inheriting from AttributeBase abstract class.");

	if (!has_attribute(p_attribute)) {
		Ref<RuntimeAttribute> runtime_attribute = memnew(RuntimeAttribute);

		runtime_attribute->attribute_container = this;
		runtime_attribute->set_attribute(p_attribute);
		runtime_attribute->set_attribute_set(attribute_set);
		runtime_attribute->set_buffs(p_attribute->get_buffs());
		runtime_attribute->set_value(runtime_attribute->get_initial_value());

		Callable attribute_changed_callable = Callable::create(this, "_on_attribute_changed");
		Callable buff_applied_callable = Callable::create(this, "_on_buff_applied");
		Callable buff_removed_callable = Callable::create(this, "_on_buff_removed");

		if (!runtime_attribute->is_connected("attribute_changed", attribute_changed_callable)) {
			runtime_attribute->connect("attribute_changed", attribute_changed_callable);
		}

		if (!runtime_attribute->is_connected("buff_added", buff_applied_callable)) {
			runtime_attribute->connect("buff_added", buff_applied_callable);
		}

		if (!runtime_attribute->is_connected("buff_removed", buff_removed_callable)) {
			runtime_attribute->connect("buff_removed", buff_removed_callable);
		}

		TypedArray<AttributeBase> base_attributes = runtime_attribute->get_derived_from();

		if (base_attributes.size() > 0) {
			int i = 0;

			for (i; i < base_attributes.size(); i++) {
				Ref<AttributeBase> base_attribute = base_attributes[i];
				Array _derived;

				if (derived_attributes.has(base_attribute->get_attribute_name())) {
					_derived = derived_attributes[base_attribute->get_attribute_name()];
				} else {
					_derived = Array();
					derived_attributes[base_attribute->get_attribute_name()] = _derived;
				}

				_derived.push_back(runtime_attribute);
			}
		}

		attributes[p_attribute->get_attribute_name()] = runtime_attribute;
	}
}

void AttributeContainer::apply_buff(Ref<AttributeBuff> p_buff)
{
	ERR_FAIL_NULL_MSG(p_buff, "Buff cannot be null, it must be an instance of a class inheriting from AttributeBuff abstract class.");

	if (p_buff->is_operate_overridden()) {
		TypedArray<RuntimeAttribute> _attributes = get_attributes();

		for (int i = 0; i < _attributes.size(); i++) {
			Ref<RuntimeAttribute> runtime_attribute = _attributes[i];

			if (runtime_attribute->add_buff(p_buff) && !Math::is_zero_approx(p_buff->get_duration())) {
				buff_pool_queue->enqueue(RuntimeBuff::from_buff(p_buff));
			}
		}
	} else {
		Ref<RuntimeAttribute> runtime_attribute = get_attribute_by_name(p_buff->get_attribute_name());

		if (runtime_attribute.is_valid() && !runtime_attribute.is_null() && runtime_attribute->add_buff(p_buff)) {
			if (!Math::is_zero_approx(p_buff->get_duration())) {
				buff_pool_queue->enqueue(RuntimeBuff::from_buff(p_buff));
			}
		}
	}
}

void AttributeContainer::remove_attribute(Ref<AttributeBase> p_attribute)
{
	ERR_FAIL_NULL_MSG(p_attribute, "Attribute cannot be null, it must be an instance of a class inheriting from AttributeBase abstract class.");

	if (has_attribute(p_attribute)) {
		Ref<RuntimeAttribute> runtime_attribute = get_attribute_by_name(p_attribute->get_name());

		ERR_FAIL_COND_MSG(!runtime_attribute.is_valid(), "Attribute not found in the container.");

		String attribute_name = runtime_attribute->get_attribute()->get_attribute_name();

		if (attributes.has(attribute_name)) {
			runtime_attribute->disconnect("attribute_changed", Callable::create(this, "_on_attribute_changed"));
			runtime_attribute->disconnect("buff_added", Callable::create(this, "_on_buff_applied"));
			runtime_attribute->disconnect("buff_removed", Callable::create(this, "_on_buff_removed"));
			attributes.erase(attribute_name);
		}
	}
}

void AttributeContainer::remove_buff(Ref<AttributeBuff> p_buff)
{
	ERR_FAIL_NULL_MSG(p_buff, "Buff cannot be null, it must be an instance of a class inheriting from AttributeBuff abstract class.");

	Array _attributes = attributes.values();

	for (int i = 0; i < _attributes.size(); i++) {
		Ref<RuntimeAttribute> attribute = _attributes[i];
		attribute->remove_buff(p_buff);
	}
}

void AttributeContainer::setup()
{
	attributes.clear();

	if (attribute_set.is_valid()) {
		for (int i = 0; i < attribute_set->count(); i++) {
			add_attribute(attribute_set->get_at(i));
		}
	}
}

Ref<RuntimeAttribute> AttributeContainer::find(Callable p_predicate) const
{
	Array _attributes = attributes.values();

	for (int i = 0; i < _attributes.size(); i++) {
		if (p_predicate.call(_attributes[i])) {
			return _attributes[i];
		}
	}

	return nullptr;
}

float AttributeContainer::find_buffed_value(Callable p_predicate) const
{
	Ref<RuntimeAttribute> attribute = find(p_predicate);
	return attribute.is_valid() && !attribute.is_null() ? attribute->get_buffed_value() : 0.0f;
}

float AttributeContainer::find_value(Callable p_predicate) const
{
	Ref<RuntimeAttribute> attribute = find(p_predicate);
	return attribute.is_valid() && !attribute.is_null() ? attribute->get_value() : 0.0f;
}

Ref<AttributeSet> AttributeContainer::get_attribute_set() const
{
	return attribute_set;
}

TypedArray<RuntimeAttribute> AttributeContainer::get_attributes() const
{
	return (TypedArray<RuntimeAttribute>)attributes.values();
}

Ref<RuntimeAttribute> AttributeContainer::get_attribute_by_name(const String &p_name) const
{
	if (attributes.has(p_name)) {
		return attributes[p_name];
	}

	return Ref<RuntimeAttribute>();
}

float AttributeContainer::get_attribute_buffed_value_by_name(const String &p_name) const
{
	Ref<RuntimeAttribute> attribute = get_attribute_by_name(p_name);
	return attribute.is_valid() && !attribute.is_null() ? attribute->get_buffed_value() : 0.0f;
}

float AttributeContainer::get_attribute_value_by_name(const String &p_name) const
{
	Ref<RuntimeAttribute> attribute = get_attribute_by_name(p_name);
	return attribute.is_valid() && !attribute.is_null() ? attribute->get_value() : 0.0f;
}

bool AttributeContainer::get_server_authoritative() const
{
	return server_authoritative;
}

void AttributeContainer::set_attribute_set(const Ref<AttributeSet> &p_attribute_set)
{
	attribute_set = p_attribute_set;
	setup();
}

void AttributeContainer::set_server_authoritative(const bool p_server_authoritative)
{
	server_authoritative = p_server_authoritative;

	if (buff_pool_queue != nullptr) {
		buff_pool_queue->set_server_authoritative(server_authoritative);
	}
}
