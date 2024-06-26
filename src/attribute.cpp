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
	ADD_PROPERTY(PropertyInfo(Variant::INT, "buff_type", PROPERTY_HINT_ENUM, "Oneshot,Stackable"), "set_buff_type", "get_buff_type");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "duration"), "set_duration", "get_duration");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "operation", PROPERTY_HINT_RESOURCE_TYPE, "AttributeOperation"), "set_operation", "get_operation");

	/// binds enum as consts
	BIND_ENUM_CONSTANT(BT_ONESHOT);
	BIND_ENUM_CONSTANT(BT_STACKABLE);
}

bool AttributeBuff::operator==(const Ref<AttributeBuff> &buff) const
{
	return buff->attribute_name == attribute_name && buff->buff_name == buff_name && buff->buff_type == buff_type && buff->duration == duration && buff->operation == operation;
}

AttributeBuff::AttributeBuff()
{
	attribute_name = "";
	buff_name = "";
	buff_type = BT_ONESHOT;
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
	ClassDB::bind_method(D_METHOD("add_buff", "p_buff"), &Attribute::add_buff);
	ClassDB::bind_method(D_METHOD("add_buffs", "p_buffs"), &Attribute::add_buffs);
	ClassDB::bind_method(D_METHOD("can_receive_buff", "p_buff"), &Attribute::can_receive_buff);
	ClassDB::bind_method(D_METHOD("clear_buffs"), &Attribute::clear_buffs);
	ClassDB::bind_method(D_METHOD("current_value"), &Attribute::current_value);
	ClassDB::bind_method(D_METHOD("has_buff", "p_buff"), &Attribute::has_buff);
	ClassDB::bind_method(D_METHOD("remove_buff", "p_buff"), &Attribute::remove_buff);
	ClassDB::bind_method(D_METHOD("remove_buffs", "p_buffs"), &Attribute::remove_buffs);
	ClassDB::bind_method(D_METHOD("setup"), &Attribute::setup);
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

	/// adds signals to godot
	ADD_SIGNAL(MethodInfo("attribute_changed", PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "Attribute"), PropertyInfo(Variant::FLOAT, "value"), PropertyInfo(Variant::FLOAT, "value")));
	ADD_SIGNAL(MethodInfo("buff_added", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBuff")));
	ADD_SIGNAL(MethodInfo("buff_removed", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBuff")));
	ADD_SIGNAL(MethodInfo("buffs_cleared"));
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

bool Attribute::add_buff(const Ref<AttributeBuff> &p_buff)
{
	if (can_receive_buff(p_buff)) {
		if (p_buff->get_buff_type() == BT_ONESHOT) {
			float prev_value = underlying_value;
			underlying_value = Math::clamp(p_buff->operate(underlying_value), min_value, max_value);
			emit_signal("attribute_changed", this, prev_value, underlying_value);
		} else {
			buffs.append(p_buff);
			emit_signal("buff_added", p_buff);
		}

		return true;
	}

	return false;
}

uint16_t Attribute::add_buffs(const TypedArray<AttributeBuff> &p_buffs)
{
	uint16_t count = 0;

	for (int i = 0; i < p_buffs.size(); i++) {
		if (add_buff(p_buffs[i])) {
			count++;
		}
	}

	return 0;
}

bool Attribute::can_receive_buff(const Ref<AttributeBuff> &p_buff) const
{
	return p_buff->get_attribute_name() == attribute_name;
}

void Attribute::clear_buffs()
{
	buffs.clear();
	emit_signal("buffs_cleared");
}

float Attribute::current_value() const
{
	float value = underlying_value;

	for (int i = 0; i < buffs.size(); i++) {
		Ref<AttributeBuff> buff_ref = buffs[i];
		value = buff_ref->operate(value);
	}

	printf("Current value: %f\n", value);

	return value;
}

bool Attribute::has_buff(const Ref<AttributeBuff> &p_buff) const
{
	for (int i = 0; i < buffs.size(); i++) {
		if (buffs[i] == p_buff) {
			return true;
		}
	}

	return false;
}

bool Attribute::remove_buff(const Ref<AttributeBuff> &p_buff)
{
	int index = buffs.find(p_buff);

	if (index != -1) {
		buffs.remove_at(index);
		emit_signal("buff_removed", p_buff);
		return true;
	}

	return false;
}

uint16_t Attribute::remove_buffs(const TypedArray<AttributeBuff> &p_buffs)
{
	uint16_t count = 0;

	for (int i = 0; i < p_buffs.size(); i++) {
		if (remove_buff(p_buffs[i])) {
			count++;
		}
	}

	return count;
}

void Attribute::setup()
{
	underlying_value = initial_value;
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