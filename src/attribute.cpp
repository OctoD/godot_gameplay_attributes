/**************************************************************************/
/*  attribute.cpp                                                         */
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

#include "attribute.hpp"

using namespace gga;

#pragma region AttributeOperation

void AttributeOperation::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("get_operand"), &AttributeOperation::get_operand);
	ClassDB::bind_method(D_METHOD("get_value"), &AttributeOperation::get_value);
	ClassDB::bind_method(D_METHOD("operate", "base_value"), &AttributeOperation::operate);
	ClassDB::bind_method(D_METHOD("set_operand", "p_value"), &AttributeOperation::set_operand);
	ClassDB::bind_method(D_METHOD("set_value", "p_value"), &AttributeOperation::set_value);

	/// binds static methods to godot
	ClassDB::bind_static_method("AttributeOperation", D_METHOD("add", "p_value"), &AttributeOperation::add);
	ClassDB::bind_static_method("AttributeOperation", D_METHOD("divide", "p_value"), &AttributeOperation::divide);
	ClassDB::bind_static_method("AttributeOperation", D_METHOD("multiply", "p_value"), &AttributeOperation::multiply);
	ClassDB::bind_static_method("AttributeOperation", D_METHOD("percentage", "p_value"), &AttributeOperation::percentage);
	ClassDB::bind_static_method("AttributeOperation", D_METHOD("subtract", "p_value"), &AttributeOperation::subtract);

	/// binds properties
	ADD_PROPERTY(PropertyInfo(Variant::INT, "operand", PROPERTY_HINT_ENUM, "Add:0,Divide:1,Multiply:2,Percentage:3,Subtract:4"), "set_operand", "get_operand");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "value"), "set_value", "get_value");

	/// binds enum as consts
	BIND_ENUM_CONSTANT(OP_ADD);
	BIND_ENUM_CONSTANT(OP_DIVIDE);
	BIND_ENUM_CONSTANT(OP_MULTIPLY);
	BIND_ENUM_CONSTANT(OP_PERCENTAGE);
	BIND_ENUM_CONSTANT(OP_SUBTRACT);
}

Ref<AttributeOperation> AttributeOperation::create(const OperationType p_operand, const float p_value)
{
	Ref<AttributeOperation> operation = memnew(AttributeOperation);
	operation->set_operand(p_operand);
	operation->set_value(p_value);
	return operation;
}

bool AttributeOperation::operator==(const Ref<AttributeOperation> &buff) const
{
	return operand == buff->operand && value == buff->value;
}

AttributeOperation::AttributeOperation()
{
	operand = OP_ADD;
	value = 0.0f;
}

AttributeOperation::~AttributeOperation()
{
}

Ref<AttributeOperation> AttributeOperation::add(const float p_value)
{
	return create(OP_ADD, p_value);
}

Ref<AttributeOperation> AttributeOperation::divide(const float p_value)
{
	return create(OP_DIVIDE, p_value);
}

Ref<AttributeOperation> AttributeOperation::multiply(const float p_value)
{
	return create(OP_MULTIPLY, p_value);
}

Ref<AttributeOperation> AttributeOperation::percentage(const float p_value)
{
	return create(OP_PERCENTAGE, p_value);
}

Ref<AttributeOperation> AttributeOperation::subtract(const float p_value)
{
	return create(OP_SUBTRACT, p_value);
}

int AttributeOperation::get_operand() const
{
	return (int)operand;
}

float AttributeOperation::get_value() const
{
	return value;
}

float AttributeOperation::operate(float p_base_value) const
{
	switch (operand) {
		case OP_ADD:
			return p_base_value + value;
		case OP_DIVIDE:
			return abs(value) < 0.0001f ? 0 : p_base_value / value;
		case OP_MULTIPLY:
			return p_base_value * value;
		case OP_PERCENTAGE:
			return p_base_value + ((p_base_value / 100) * value);
		case OP_SUBTRACT:
			return p_base_value - value;
		default:
			return p_base_value;
	}
}

void AttributeOperation::set_operand(const int p_value)
{
	ERR_FAIL_COND_MSG(p_value < 0 || p_value > 4, "Invalid operand value");

	switch (p_value) {
		case 0:
			operand = OP_ADD;
			break;
		case 1:
			operand = OP_DIVIDE;
			break;
		case 2:
			operand = OP_MULTIPLY;
			break;
		case 3:
			operand = OP_PERCENTAGE;
			break;
		case 4:
			operand = OP_SUBTRACT;
			break;
	}
}

void AttributeOperation::set_value(const float p_value)
{
	value = p_value;
}

#pragma endregion

#pragma region AttributeBuff

void AttributeBuff::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("get_attribute_name"), &AttributeBuff::get_attribute_name);
	ClassDB::bind_method(D_METHOD("get_buff_name"), &AttributeBuff::get_buff_name);
	ClassDB::bind_method(D_METHOD("get_buff_type"), &AttributeBuff::get_buff_type);
	ClassDB::bind_method(D_METHOD("get_duration"), &AttributeBuff::get_duration);
	ClassDB::bind_method(D_METHOD("get_operation"), &AttributeBuff::get_operation);
	ClassDB::bind_method(D_METHOD("set_attribute_name", "p_value"), &AttributeBuff::set_attribute_name);
	ClassDB::bind_method(D_METHOD("set_buff_name", "p_value"), &AttributeBuff::set_buff_name);
	ClassDB::bind_method(D_METHOD("set_buff_type", "p_value"), &AttributeBuff::set_buff_type);
	ClassDB::bind_method(D_METHOD("set_duration", "p_value"), &AttributeBuff::set_duration);
	ClassDB::bind_method(D_METHOD("set_operation", "p_value"), &AttributeBuff::set_operation);
	ClassDB::bind_method(D_METHOD("operate", "base_value"), &AttributeBuff::operate);

	/// binds properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "attribute_name"), "set_attribute_name", "get_attribute_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "buff_name"), "set_buff_name", "get_buff_name");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "buff_type", PROPERTY_HINT_ENUM, "Stackable,One Shot"), "set_buff_type", "get_buff_type");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "duration"), "set_duration", "get_duration");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "operation", PROPERTY_HINT_RESOURCE_TYPE, "AttributeOperation"), "set_operation", "get_operation");

	/// binds enum as consts
	BIND_ENUM_CONSTANT(BT_STACKABLE);
	BIND_ENUM_CONSTANT(BT_ONESHOT);
}

bool AttributeBuff::operator==(const Ref<AttributeBuff> &buff) const
{
	return buff->attribute_name == attribute_name && buff->buff_name == buff_name && buff->buff_type == buff_type && buff->duration == duration && buff->operation == operation;
}

AttributeBuff::AttributeBuff()
{
	attribute_name = "";
	buff_name = "";
	buff_type = BT_STACKABLE;
	duration = 0.0f;
	operation = AttributeOperation::add(0);
}

AttributeBuff::AttributeBuff(
		const String &p_attribute_name,
		const String &p_buff_name,
		const int p_buff_type,
		const float p_duration,
		const Ref<AttributeOperation> &p_operation) :
		attribute_name(p_attribute_name),
		buff_name(p_buff_name),
		buff_type(p_buff_type),
		duration(p_duration),
		operation(p_operation)
{
}

AttributeBuff::~AttributeBuff()
{
}

float AttributeBuff::operate(float base_value) const
{
	return operation->operate(base_value);
}

String AttributeBuff::get_attribute_name() const
{
	return attribute_name;
}

String AttributeBuff::get_buff_name() const
{
	return buff_name;
}

int AttributeBuff::get_buff_type() const
{
	return (int)buff_type;
}

float AttributeBuff::get_duration() const
{
	return duration;
}

Ref<AttributeOperation> AttributeBuff::get_operation() const
{
	return operation;
}

void AttributeBuff::set_attribute_name(const String &p_value)
{
	attribute_name = p_value;
}

void AttributeBuff::set_buff_name(const String &p_value)
{
	buff_name = p_value;
}

void AttributeBuff::set_buff_type(const int p_value)
{
	ERR_FAIL_COND_MSG(p_value < 0 || p_value > 1, "Invalid buff type value");

	switch (p_value) {
		case 0:
			buff_type = BT_ONESHOT;
			break;
		case 1:
			buff_type = BT_STACKABLE;
			break;
		case 2:
			buff_type = BT_STACKABLE_UNIQUE;
			break;
	}
}

void AttributeBuff::set_duration(const float p_value)
{
	duration = p_value;
}

void AttributeBuff::set_operation(const Ref<AttributeOperation> &p_value)
{
	operation = p_value;
}

#pragma endregion

#pragma region Attribute

void Attribute::_bind_methods()
{
	/// static methods to bind to godot
	ClassDB::bind_static_method("Attribute", D_METHOD("create", "attribute_name", "initial_value", "min_value", "max_value"), &Attribute::create);

	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("get_attribute_name"), &Attribute::get_attribute_name);
	ClassDB::bind_method(D_METHOD("get_initial_value"), &Attribute::get_initial_value);
	ClassDB::bind_method(D_METHOD("get_max_value"), &Attribute::get_max_value);
	ClassDB::bind_method(D_METHOD("get_min_value"), &Attribute::get_min_value);
	ClassDB::bind_method(D_METHOD("set_attribute_name", "p_value"), &Attribute::set_attribute_name);
	ClassDB::bind_method(D_METHOD("set_initial_value", "p_value"), &Attribute::set_initial_value);
	ClassDB::bind_method(D_METHOD("set_max_value", "p_value"), &Attribute::set_max_value);
	ClassDB::bind_method(D_METHOD("set_min_value", "p_value"), &Attribute::set_min_value);
	ClassDB::bind_method(D_METHOD("get_buffs"), &Attribute::get_buffs);
	ClassDB::bind_method(D_METHOD("set_buffs", "p_buffs"), &Attribute::set_buffs);

	/// properties to bind to godot
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "attribute_name"), "set_attribute_name", "get_attribute_name");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "initial_value"), "set_initial_value", "get_initial_value");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_value"), "set_max_value", "get_max_value");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_value"), "set_min_value", "get_min_value");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "buffs"), "set_buffs", "get_buffs");
}

Ref<Attribute> Attribute::create(const String &p_attribute_name, const float p_initial_value, const float p_min_value, const float p_max_value)
{
	Ref<Attribute> attribute = memnew(Attribute);
	attribute->set_attribute_name(p_attribute_name);
	attribute->set_initial_value(p_initial_value);
	attribute->set_min_value(p_min_value);
	attribute->set_max_value(p_max_value);
	return attribute;
}

String Attribute::get_attribute_name() const
{
	return attribute_name;
}

TypedArray<AttributeBuff> Attribute::get_buffs() const
{
	return buffs;
}

float Attribute::get_initial_value() const
{
	return initial_value;
}

float Attribute::get_max_value() const
{
	return max_value;
}

float Attribute::get_min_value() const
{
	return min_value;
}

void Attribute::set_attribute_name(const String &p_value)
{
	attribute_name = p_value;
}

void Attribute::set_buffs(const TypedArray<AttributeBuff> &p_buffs)
{
	buffs = p_buffs;
}

void Attribute::set_initial_value(const float p_value)
{
	initial_value = p_value;
}

void Attribute::set_max_value(const float p_value)
{
	max_value = p_value;
}

void Attribute::set_min_value(const float p_value)
{
	min_value = p_value;
}

#pragma endregion

#pragma region AttributeSet

void AttributeSet::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("add_attribute", "p_attribute"), &AttributeSet::add_attribute);
	ClassDB::bind_method(D_METHOD("add_attributes", "p_attributes"), &AttributeSet::add_attributes);
	ClassDB::bind_method(D_METHOD("find_by_classname", "p_classname"), &AttributeSet::find_by_classname);
	ClassDB::bind_method(D_METHOD("find_by_name", "p_name"), &AttributeSet::find_by_name);
	ClassDB::bind_method(D_METHOD("get_attributes_names"), &AttributeSet::get_attributes_names);
	ClassDB::bind_method(D_METHOD("get_attributes"), &AttributeSet::get_attributes);
	ClassDB::bind_method(D_METHOD("get_set_name"), &AttributeSet::get_set_name);
	ClassDB::bind_method(D_METHOD("has_attribute", "p_attribute"), &AttributeSet::has_attribute);
	ClassDB::bind_method(D_METHOD("remove_attribute", "p_attribute"), &AttributeSet::remove_attribute);
	ClassDB::bind_method(D_METHOD("remove_attributes", "p_attributes"), &AttributeSet::remove_attributes);
	ClassDB::bind_method(D_METHOD("set_attributes", "p_attributes"), &AttributeSet::set_attributes);
	ClassDB::bind_method(D_METHOD("set_set_name", "p_value"), &AttributeSet::set_set_name);

	/// binds properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "attributes", PROPERTY_HINT_RESOURCE_TYPE, "24/17:Attribute"), "set_attributes", "get_attributes");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "set_name"), "set_set_name", "get_set_name");

	/// adds signals to godot
	ADD_SIGNAL(MethodInfo("attribute_added", PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "Attribute")));
	ADD_SIGNAL(MethodInfo("attribute_removed", PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "Attribute")));
}

bool AttributeSet::operator==(const Ref<AttributeSet> &set) const
{
	if (attributes.size() != set->attributes.size()) {
		return false;
	}

	if (set_name != set->set_name) {
		return false;
	}

	for (int i = 0; i < attributes.size(); i++) {
		if (attributes[i] != set->attributes[i]) {
			return false;
		}
	}

	return false;
}

AttributeSet::AttributeSet()
{
	attributes = TypedArray<Attribute>();
}

AttributeSet::AttributeSet(TypedArray<Attribute> p_attributes, String p_set_name)
{
	attributes = p_attributes.duplicate(true);
	set_name = p_set_name;
}

bool AttributeSet::add_attribute(const Ref<Attribute> &p_attribute)
{
	if (!has_attribute(p_attribute)) {
		Ref<Attribute> d_attribute = p_attribute->duplicate(true);

		attributes.push_back(d_attribute);
		emit_signal("attribute_added", d_attribute);
		emit_changed();
		return true;
	}

	return false;
}

uint16_t AttributeSet::add_attributes(const TypedArray<Attribute> &p_attributes)
{
	uint16_t count = 0;

	for (int i = 0; i < p_attributes.size(); i++) {
		if (!has_attribute(p_attributes[i])) {
			Ref<Attribute> d_attribute = p_attributes[i];

			d_attribute = d_attribute->duplicate(true);

			attributes.push_back(d_attribute);
			count++;
			emit_signal("attribute_added", d_attribute);
		}
	}

	if (count > 0) {
		emit_changed();
	}

	return count;
}

int AttributeSet::find(const Ref<Attribute> &p_attribute) const
{
	return attributes.find(p_attribute);
}

Ref<Attribute> AttributeSet::find_by_classname(const String &p_classname) const
{
	for (int i = 0; i < attributes.size(); i++) {
		Ref<Attribute> attribute = attributes[i];

		if (attribute->get_class() == p_classname) {
			return attributes[i];
		}
	}

	return Ref<Attribute>();
}

Ref<Attribute> AttributeSet::find_by_name(const String &p_name) const
{
	for (int i = 0; i < attributes.size(); i++) {
		Ref<Attribute> attribute = attributes[i];

		if (attribute->get_attribute_name() == p_name) {
			return attributes[i];
		}
	}

	return Ref<Attribute>();
}

PackedStringArray AttributeSet::get_attributes_names() const
{
	PackedStringArray names = PackedStringArray();

	for (int i = 0; i < attributes.size(); i++) {
		Ref<Attribute> attribute = attributes[i];
		names.push_back(attribute->get_attribute_name());
	}

	return names;
}

TypedArray<Attribute> AttributeSet::get_attributes() const
{
	return attributes;
}

Ref<Attribute> AttributeSet::get_at(int index) const
{
	if (index >= 0 && index < attributes.size()) {
		return attributes[index];
	}

	return Ref<Attribute>();
}

String AttributeSet::get_set_name() const
{
	return set_name;
}

bool AttributeSet::has_attribute(const Ref<Attribute> &p_attribute) const
{
	for (int i = 0; i < attributes.size(); i++) {
		if (attributes[i] == p_attribute) {
			return true;
		}
	}

	return false;
}

bool AttributeSet::remove_attribute(const Ref<Attribute> &p_attribute)
{
	int index = attributes.find(p_attribute);
	bool result = false;

	if (index != -1) {
		attributes.remove_at(index);
		emit_signal("attribute_removed", p_attribute);
		emit_changed();
		return true;
	}

	return false;
}

int AttributeSet::remove_attributes(const TypedArray<Attribute> &p_attributes)
{
	int count = 0;

	for (int i = 0; i < p_attributes.size(); i++) {
		int index = attributes.find(p_attributes[i]);

		if (index != -1) {
			attributes.remove_at(index);
			count++;
			emit_signal("attribute_removed", p_attributes[i]);
		}
	}

	if (count > 0) {
		emit_changed();
	}

	return count;
}

void AttributeSet::push_back(const Ref<Attribute> &p_attribute)
{
	attributes.push_back(p_attribute);
	emit_signal("attribute_added", p_attribute);
	emit_changed();
}

void AttributeSet::set_attributes(const TypedArray<Attribute> &p_attributes)
{
	attributes = p_attributes;
	emit_changed();
}

void AttributeSet::set_set_name(const String &p_value)
{
	set_name = p_value;
	emit_changed();
}

int AttributeSet::count() const
{
	return attributes.size();
}

#pragma endregion

#pragma region AttributesTable

void AttributesTable::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("add_attribute_set", "p_attribute_set"), &AttributesTable::add_attribute_set);
	ClassDB::bind_method(D_METHOD("get_attribute_names"), &AttributesTable::get_attribute_names);
	ClassDB::bind_method(D_METHOD("get_attribute_sets"), &AttributesTable::get_attribute_sets);
	ClassDB::bind_method(D_METHOD("has_attribute_set", "p_attribute_set"), &AttributesTable::has_attribute_set);
	ClassDB::bind_method(D_METHOD("remove_attribute_set", "p_attribute_set"), &AttributesTable::remove_attribute_set);
	ClassDB::bind_method(D_METHOD("set_attribute_sets", "p_attribute_sets"), &AttributesTable::set_attribute_sets);

	/// binds properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "attribute_sets", PROPERTY_HINT_RESOURCE_TYPE, "24/17:AttributeSet"), "set_attribute_sets", "get_attribute_sets");
}

AttributesTable::AttributesTable()
{
	attribute_sets = TypedArray<AttributeSet>();
}

AttributesTable::AttributesTable(TypedArray<AttributeSet> p_attribute_sets) :
		attribute_sets(p_attribute_sets)
{
}

void AttributesTable::add_attribute_set(const Ref<AttributeSet> &p_attribute_set)
{
	if (!has_attribute_set(p_attribute_set)) {
		attribute_sets.push_back(p_attribute_set);
	}
}

TypedArray<AttributeSet> AttributesTable::get_attribute_sets() const
{
	return attribute_sets;
}

PackedStringArray AttributesTable::get_attribute_names() const
{
	PackedStringArray names = PackedStringArray();

	for (int i = 0; i < attribute_sets.size(); i++) {
		Ref<AttributeSet> attribute_set = attribute_sets[i];
		PackedStringArray set_names = attribute_set->get_attributes_names();

		for (int j = 0; j < set_names.size(); j++) {
			if (!names.has(set_names[j])) {
				names.push_back(set_names[j]);
			}
		}
	}

	names.sort();

	return names;
}

bool AttributesTable::has_attribute_set(const Ref<AttributeSet> &p_attribute_set) const
{
	for (int i = 0; i < attribute_sets.size(); i++) {
		if (attribute_sets[i] == p_attribute_set) {
			return true;
		}
	}

	return false;
}

void AttributesTable::remove_attribute_set(const Ref<AttributeSet> &p_attribute_set)
{
	int index = attribute_sets.find(p_attribute_set);

	if (index != -1) {
		attribute_sets.remove_at(index);
	}
}

void AttributesTable::set_attribute_sets(const TypedArray<AttributeSet> &p_attribute_sets)
{
	attribute_sets = p_attribute_sets;
}

#pragma endregion

#pragma region BuffPoolQueueItem

void RuntimeBuff::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_static_method("RuntimeBuff", D_METHOD("from_buff", "p_buff"), &RuntimeBuff::from_buff);
	ClassDB::bind_static_method("RuntimeBuff", D_METHOD("to_buff", "p_buff"), &RuntimeBuff::to_buff);
	ClassDB::bind_method(D_METHOD("can_dispose"), &RuntimeBuff::can_dispose);
	ClassDB::bind_method(D_METHOD("get_attribute_name"), &RuntimeBuff::get_attribute_name);
	ClassDB::bind_method(D_METHOD("get_buff_name"), &RuntimeBuff::get_buff_name);
	ClassDB::bind_method(D_METHOD("get_buff_type"), &RuntimeBuff::get_buff_type);
	ClassDB::bind_method(D_METHOD("get_duration"), &RuntimeBuff::get_duration);
	ClassDB::bind_method(D_METHOD("get_operation"), &RuntimeBuff::get_operation);
	ClassDB::bind_method(D_METHOD("get_time_left"), &RuntimeBuff::get_time_left);
	ClassDB::bind_method(D_METHOD("set_attribute_name", "p_value"), &RuntimeBuff::set_attribute_name);
	ClassDB::bind_method(D_METHOD("set_buff_name", "p_value"), &RuntimeBuff::set_buff_name);
	ClassDB::bind_method(D_METHOD("set_buff_type", "p_value"), &RuntimeBuff::set_buff_type);
	ClassDB::bind_method(D_METHOD("set_duration", "p_value"), &RuntimeBuff::set_duration);
	ClassDB::bind_method(D_METHOD("set_operation", "p_value"), &RuntimeBuff::set_operation);
	ClassDB::bind_method(D_METHOD("set_time_left", "p_value"), &RuntimeBuff::set_time_left);

	/// binds properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "attribute_name"), "set_attribute_name", "get_attribute_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "buff_name"), "set_buff_name", "get_buff_name");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "buff_type", PROPERTY_HINT_ENUM, "One Shot,Stackable,Stackable Unique"), "set_buff_type", "get_buff_type");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "duration"), "set_duration", "get_duration");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "operation", PROPERTY_HINT_RESOURCE_TYPE, "AttributeOperation"), "set_operation", "get_operation");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_left"), "set_time_left", "get_time_left");
}

Ref<RuntimeBuff> RuntimeBuff::from_buff(const Ref<AttributeBuff> &p_buff)
{
	Ref<RuntimeBuff> runtime_buff = memnew(RuntimeBuff);
	runtime_buff->attribute_name = p_buff->get_attribute_name();
	runtime_buff->buff_name = p_buff->get_buff_name();
	runtime_buff->buff_type = p_buff->get_buff_type();
	runtime_buff->duration = p_buff->get_duration();
	runtime_buff->operation = p_buff->get_operation();
	runtime_buff->time_left = p_buff->get_duration();
	return runtime_buff;
}

Ref<AttributeBuff> RuntimeBuff::to_buff(const Ref<RuntimeBuff> &p_buff)
{
	Ref<AttributeBuff> buff = memnew(AttributeBuff);
	buff->set_attribute_name(p_buff->attribute_name);
	buff->set_buff_name(p_buff->buff_name);
	buff->set_buff_type(p_buff->buff_type);
	buff->set_duration(p_buff->duration);
	buff->set_operation(p_buff->operation);
	return buff;
}

bool RuntimeBuff::operator==(const Ref<AttributeBuff> &p_attribute_buff) const
{
	return p_attribute_buff->get_attribute_name() == attribute_name && p_attribute_buff->get_buff_name() == buff_name && p_attribute_buff->get_buff_type() == buff_type && p_attribute_buff->get_duration() == duration && p_attribute_buff->get_operation() == operation;
}

bool RuntimeBuff::operator==(const Ref<RuntimeBuff> &p_runtime_buff) const
{
	return p_runtime_buff->get_attribute_name() == attribute_name && p_runtime_buff->get_buff_name() == buff_name && p_runtime_buff->get_buff_type() == buff_type && p_runtime_buff->get_duration() == duration && p_runtime_buff->get_operation() == operation;
}

bool RuntimeBuff::can_dispose() const
{
	return time_left <= 0.01f && duration >= 0.01f;
}

String RuntimeBuff::get_attribute_name() const
{
	return attribute_name;
}

String RuntimeBuff::get_buff_name() const
{
	return buff_name;
}

float RuntimeBuff::get_duration() const
{
	return duration;
}

int RuntimeBuff::get_buff_type() const
{
	return buff_type;
}

Ref<AttributeOperation> RuntimeBuff::get_operation() const
{
	return operation;
}

float RuntimeBuff::get_time_left() const
{
	return time_left;
}

void RuntimeBuff::set_attribute_name(const String &p_value)
{
	attribute_name = p_value;
}

void RuntimeBuff::set_buff_name(const String &p_value)
{
	buff_name = p_value;
}

void RuntimeBuff::set_duration(const float p_value)
{
	duration = p_value;
}

void RuntimeBuff::set_buff_type(const int p_value)
{
	buff_type = p_value;
}

void RuntimeBuff::set_operation(const Ref<AttributeOperation> &p_value)
{
	operation = p_value;
}

void RuntimeBuff::set_time_left(const float p_value)
{
	time_left = p_value;
}

#pragma endregion

#pragma region RuntimeAttribute

void RuntimeAttribute::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_static_method("RuntimeAttribute", D_METHOD("from_attribute", "p_attribute"), &RuntimeAttribute::from_attribute);
	ClassDB::bind_static_method("RuntimeAttribute", D_METHOD("to_attribute", "p_attribute"), &RuntimeAttribute::to_attribute);
	ClassDB::bind_method(D_METHOD("add_buff", "p_buff"), &RuntimeAttribute::add_buff);
	ClassDB::bind_method(D_METHOD("add_buffs", "p_buffs"), &RuntimeAttribute::add_buffs);
	ClassDB::bind_method(D_METHOD("can_receive_buff", "p_buff"), &RuntimeAttribute::can_receive_buff);
	ClassDB::bind_method(D_METHOD("get_attribute_name"), &RuntimeAttribute::get_attribute_name);
	ClassDB::bind_method(D_METHOD("get_buffs"), &RuntimeAttribute::get_buffs);
	ClassDB::bind_method(D_METHOD("get_buffed_value"), &RuntimeAttribute::get_buffed_value);
	ClassDB::bind_method(D_METHOD("get_max_value"), &RuntimeAttribute::get_max_value);
	ClassDB::bind_method(D_METHOD("get_min_value"), &RuntimeAttribute::get_min_value);
	ClassDB::bind_method(D_METHOD("get_value"), &RuntimeAttribute::get_value);
	ClassDB::bind_method(D_METHOD("set_attribute_name", "p_value"), &RuntimeAttribute::set_attribute_name);
	ClassDB::bind_method(D_METHOD("set_buffs", "p_buffs"), &RuntimeAttribute::set_buffs);
	ClassDB::bind_method(D_METHOD("set_max_value", "p_value"), &RuntimeAttribute::set_max_value);
	ClassDB::bind_method(D_METHOD("set_min_value", "p_value"), &RuntimeAttribute::set_min_value);
	ClassDB::bind_method(D_METHOD("set_value", "p_value"), &RuntimeAttribute::set_value);

	/// binds properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "attribute_name"), "set_attribute_name", "get_attribute_name");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_value"), "set_max_value", "get_max_value");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_value"), "set_min_value", "get_min_value");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "value"), "set_value", "get_value");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "buffs"), "set_buffs", "get_buffs");

	/// adds signals to godot
	ADD_SIGNAL(MethodInfo("attribute_changed", PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "RuntimeAttribute"), PropertyInfo(Variant::FLOAT, "prev_value"), PropertyInfo(Variant::FLOAT, "value")));
	ADD_SIGNAL(MethodInfo("buff_added", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "RuntimeBuff")));
	ADD_SIGNAL(MethodInfo("buff_removed", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "RuntimeBuff")));
	ADD_SIGNAL(MethodInfo("buffs_cleared"));
}

Ref<RuntimeAttribute> RuntimeAttribute::from_attribute(const Ref<Attribute> &p_attribute)
{
	TypedArray<AttributeBuff> p_attribute_buffs = p_attribute->get_buffs();
	TypedArray<RuntimeBuff> buffs = TypedArray<RuntimeBuff>();

	for (int i = 0; i < p_attribute_buffs.size(); i++) {
		buffs.push_back(RuntimeBuff::from_buff(p_attribute_buffs[i]));
	}

	Ref<RuntimeAttribute> runtime_attribute = memnew(RuntimeAttribute);
	runtime_attribute->attribute_name = p_attribute->get_attribute_name();
	runtime_attribute->buffs = buffs;
	runtime_attribute->initial_value = p_attribute->get_initial_value();
	runtime_attribute->max_value = p_attribute->get_max_value();
	runtime_attribute->min_value = p_attribute->get_min_value();
	runtime_attribute->value = p_attribute->get_initial_value();
	return runtime_attribute;
}

Ref<Attribute> RuntimeAttribute::to_attribute(const Ref<RuntimeAttribute> &p_attribute)
{
	Ref<Attribute> attribute = memnew(Attribute);
	TypedArray<AttributeBuff> buffs = TypedArray<AttributeBuff>();

	for (int i = 0; i < p_attribute->buffs.size(); i++) {
		buffs.push_back(RuntimeBuff::to_buff(p_attribute->buffs[i]));
	}

	attribute->set_attribute_name(p_attribute->attribute_name);
	attribute->set_max_value(p_attribute->max_value);
	attribute->set_min_value(p_attribute->min_value);
	attribute->set_initial_value(p_attribute->initial_value);
	attribute->set_buffs(buffs);
	return attribute;
}

bool RuntimeAttribute::add_buff(const Ref<AttributeBuff> &p_buff)
{
	if (can_receive_buff(p_buff)) {
		if (p_buff->get_buff_type() == BT_ONESHOT) {
			float prev_value = value;
			value = Math::clamp(p_buff->operate(value), min_value, max_value);
			emit_signal("attribute_changed", this, prev_value, value);
		} else {
			Ref<RuntimeBuff> runtime_buff = RuntimeBuff::from_buff(p_buff);

			buffs.push_back(runtime_buff);
			emit_signal("buff_added", runtime_buff);
		}

		return true;
	}

	return false;
}

int RuntimeAttribute::add_buffs(const TypedArray<AttributeBuff> &p_buffs)
{
	int count = 0;

	for (int i = 0; i < p_buffs.size(); i++) {
		if (add_buff(p_buffs[i])) {
			count++;
		}
	}

	return count;
}

bool RuntimeAttribute::can_receive_buff(const Ref<AttributeBuff> &p_buff) const
{
	if (p_buff->get_buff_type() == BT_STACKABLE_UNIQUE && has_buff(p_buff)) {
		return false;
	}

	return p_buff->get_attribute_name() == attribute_name;
}

void RuntimeAttribute::clear_buffs()
{
	buffs.clear();
}

bool RuntimeAttribute::has_buff(const Ref<AttributeBuff> &p_buff) const
{
	for (int i = 0; i < buffs.size(); i++) {
		if (buffs[i] == p_buff) {
			return true;
		}
	}

	return false;
}

bool RuntimeAttribute::remove_buff(const Ref<AttributeBuff> &p_buff)
{
	for (int i = 0; i < buffs.size(); i++) {
		Ref<RuntimeBuff> buff = buffs[i];

		if (buff == p_buff) {
			buffs.remove_at(i);
			emit_signal("buff_removed", buff);
			return true;
		}
	}

	return false;
}

int RuntimeAttribute::remove_buffs(const TypedArray<AttributeBuff> &p_buffs)
{
	int count = 0;

	for (int i = p_buffs.size() - 1; i >= 0; i--) {
		for (int j = buffs.size() - 1; j >= 0; j--) {
			if (buffs[j] == p_buffs[i]) {
				buffs.remove_at(j);
				count++;
			}
		}
	}

	if (count > 0) {
		emit_signal("buffs_cleared");
	}

	return count;
}

String RuntimeAttribute::get_attribute_name() const
{
	return attribute_name;
}

float RuntimeAttribute::get_buffed_value() const
{
	float current_value = value;

	for (int i = 0; i < buffs.size(); i++) {
		Ref<RuntimeBuff> buff = buffs[i];
		current_value = buff->get_operation()->operate(current_value);
	}

	return current_value;
}

float RuntimeAttribute::get_initial_value() const
{
	return initial_value;
}

float RuntimeAttribute::get_max_value() const
{
	return max_value;
}

float RuntimeAttribute::get_min_value() const
{
	return min_value;
}

float RuntimeAttribute::get_value() const
{
	return value;
}

TypedArray<RuntimeBuff> RuntimeAttribute::get_buffs() const
{
	return buffs;
}

void RuntimeAttribute::set_attribute_name(const String &p_value)
{
	attribute_name = p_value;
}

void RuntimeAttribute::set_initial_value(const float p_value)
{
	initial_value = p_value;
}

void RuntimeAttribute::set_max_value(const float p_value)
{
	max_value = p_value;
}

void RuntimeAttribute::set_min_value(const float p_value)
{
	min_value = p_value;
}

void RuntimeAttribute::set_value(const float p_value)
{
	value = p_value;
}

void RuntimeAttribute::set_buffs(const TypedArray<AttributeBuff> &p_value)
{
	buffs.clear();

	for (int i = 0; i < p_value.size(); i++) {
		buffs.push_back(RuntimeBuff::from_buff(p_value[i]));
	}
}

#pragma endregion