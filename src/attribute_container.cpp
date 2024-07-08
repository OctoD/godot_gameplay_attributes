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
	ClassDB::bind_method(D_METHOD("_on_buff_dequeued", "p_buff"), &AttributeContainer::_on_buff_dequeued);
	ClassDB::bind_method(D_METHOD("_on_buff_enqueued", "p_buff"), &AttributeContainer::_on_buff_enqueued);
	ClassDB::bind_method(D_METHOD("add_attribute", "p_attribute"), &AttributeContainer::add_attribute);
	ClassDB::bind_method(D_METHOD("apply_buff", "p_buff"), &AttributeContainer::apply_buff);
	ClassDB::bind_method(D_METHOD("get_attribute_set"), &AttributeContainer::get_attribute_set);
	ClassDB::bind_method(D_METHOD("get_attributes"), &AttributeContainer::get_attributes);
	ClassDB::bind_method(D_METHOD("get_autostart"), &AttributeContainer::get_autostart);
	ClassDB::bind_method(D_METHOD("get_server_authoritative"), &AttributeContainer::get_server_authoritative);
	ClassDB::bind_method(D_METHOD("is_paused"), &AttributeContainer::is_paused);
	ClassDB::bind_method(D_METHOD("pause"), &AttributeContainer::pause);
	ClassDB::bind_method(D_METHOD("remove_attribute", "p_attribute"), &AttributeContainer::remove_attribute);
	ClassDB::bind_method(D_METHOD("remove_buff", "p_buff"), &AttributeContainer::remove_buff);
	ClassDB::bind_method(D_METHOD("resume"), &AttributeContainer::resume);
	ClassDB::bind_method(D_METHOD("set_attribute_set", "p_attribute_set"), &AttributeContainer::set_attribute_set);
	ClassDB::bind_method(D_METHOD("set_autostart", "p_autostart"), &AttributeContainer::set_autostart);
	ClassDB::bind_method(D_METHOD("set_server_authoritative", "p_server_authoritative"), &AttributeContainer::set_server_authoritative);
	ClassDB::bind_method(D_METHOD("setup"), &AttributeContainer::setup);

	/// binds properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "attribute_set", PROPERTY_HINT_RESOURCE_TYPE, "AttributeSet"), "set_attribute_set", "get_attribute_set");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "autostart"), "set_autostart", "get_autostart");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "server_authoritative"), "set_server_authoritative", "get_server_authoritative");

	/// signals binding
	ADD_SIGNAL(MethodInfo("attribute_added", PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "Attribute")));
	ADD_SIGNAL(MethodInfo("attribute_changed", PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "Attribute"), PropertyInfo(Variant::FLOAT, "previous_value"), PropertyInfo(Variant::FLOAT, "new_value")));
	ADD_SIGNAL(MethodInfo("attribute_removed", PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "Attribute")));
	ADD_SIGNAL(MethodInfo("buff_applied", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBuff")));
	ADD_SIGNAL(MethodInfo("buff_dequed", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBuff")));
	ADD_SIGNAL(MethodInfo("buff_enqued", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBuff")));
	ADD_SIGNAL(MethodInfo("buff_removed", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBuff")));
}

void AttributeContainer::_on_attribute_changed(Ref<Attribute> p_attribute, const float p_previous_value, const float p_new_value)
{
	emit_signal("attribute_changed", p_attribute, p_previous_value, p_new_value);
}

void AttributeContainer::_on_buff_dequeued(Ref<AttributeBuff> p_buff)
{
	emit_signal("buff_dequed", p_buff);
	remove_buff(p_buff);
}

void AttributeContainer::_on_buff_enqueued(Ref<AttributeBuff> p_buff)
{
	emit_signal("buff_enqued", p_buff);
}

void AttributeContainer::bind_attribute(Ref<Attribute> p_attribute)
{
	Callable callable = Callable::create(this, "_on_attribute_changed");

	if (!p_attribute->is_connected("attribute_changed", callable)) {
		p_attribute->connect("attribute_changed", Callable::create(this, "_on_attribute_changed"));
	}
}

bool AttributeContainer::has_attribute(Ref<Attribute> p_attribute)
{
	for (int i = 0; i < attribute_set->count(); i++) {
		Ref<Attribute> attribute = attribute_set->get_at(i);

		if (!attribute.is_null() && attribute == p_attribute) {
			return true;
		}
	}

	return false;
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

void AttributeContainer::add_attribute(Ref<Attribute> p_attribute)
{
	if (!has_attribute(p_attribute) && !paused) {
		attribute_set->push_back(p_attribute);
		bind_attribute(p_attribute);
		emit_signal("attribute_added", p_attribute);
		p_attribute->setup();
	}
}

void AttributeContainer::apply_buff(Ref<AttributeBuff> p_buff)
{
	if (paused) {
		return;
	}

	for (int i = 0; i < attribute_set->count(); i++) {
		Ref<Attribute> attribute = attribute_set->get_at(i);

		if (attribute->add_buff(p_buff)) {
			if (p_buff->get_duration() != 0.0f) {
				buff_pool_queue->add_attribute_buff(p_buff.ptr());
			}

			emit_signal("buff_applied", p_buff);
		}
	}
}

void AttributeContainer::pause()
{
	paused = true;

	if (buff_pool_queue) {
		buff_pool_queue->stop();
	}
}

bool AttributeContainer::is_paused() const
{
	return paused;
}

void AttributeContainer::remove_attribute(Ref<Attribute> p_attribute)
{
	if (has_attribute(p_attribute) && !paused) {
		int index = attribute_set->find(p_attribute);

		if (index != -1) {
			Ref<Attribute> attribute = attribute_set->get_at(index);
			attribute->disconnect("changed", Callable::create(this, "_on_attribute_changed"));
		}
	}
}

void AttributeContainer::remove_buff(Ref<AttributeBuff> p_buff)
{
	if (paused) {
		return;
	}

	for (int i = 0; i < attribute_set->count(); i++) {
		Ref<Attribute> attribute = attribute_set->get_at(i);

		if (attribute->remove_buff(p_buff)) {
			emit_signal("buff_removed", p_buff);
		}
	}
}

void AttributeContainer::resume()
{
	paused = false;

	if (buff_pool_queue) {
		buff_pool_queue->start();
	}
}

void AttributeContainer::setup()
{
	if (attribute_set.is_valid()) {
		for (int i = 0; i < attribute_set->count(); i++) {
			Ref<Attribute> attribute = attribute_set->get_at(i);
			bind_attribute(attribute);
			attribute->setup();
		}
	}

	if (autostart) {
		resume();
	}
}

Ref<AttributeSet> AttributeContainer::get_attribute_set() const
{
	return attribute_set;
}

TypedArray<Attribute> AttributeContainer::get_attributes() const
{
	if (attribute_set.is_valid()) {
		return attribute_set->get_attributes();
	}

	return TypedArray<Attribute>();
}

bool AttributeContainer::get_autostart() const
{
	return autostart;
}

bool AttributeContainer::get_server_authoritative() const
{
	return server_authoritative;
}

void AttributeContainer::set_attribute_set(const Ref<AttributeSet> &p_attribute_set)
{
	attribute_set = p_attribute_set;
}

void AttributeContainer::set_autostart(const bool p_autostart)
{
	autostart = p_autostart;
}

void AttributeContainer::set_server_authoritative(const bool p_server_authoritative)
{
	server_authoritative = p_server_authoritative;
}
