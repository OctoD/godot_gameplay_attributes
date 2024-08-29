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
#include "attribute_container.hpp"

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
		default:
			operand = OP_ADD;
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
	ClassDB::bind_method(D_METHOD("get_duration"), &AttributeBuff::get_duration);
	ClassDB::bind_method(D_METHOD("get_operation"), &AttributeBuff::get_operation);
	ClassDB::bind_method(D_METHOD("get_max_applies"), &AttributeBuff::get_max_applies);
	ClassDB::bind_method(D_METHOD("get_transient"), &AttributeBuff::get_transient);
	ClassDB::bind_method(D_METHOD("get_unique"), &AttributeBuff::get_unique);
	ClassDB::bind_method(D_METHOD("operate", "base_value"), &AttributeBuff::operate);
	ClassDB::bind_method(D_METHOD("set_attribute_name", "p_value"), &AttributeBuff::set_attribute_name);
	ClassDB::bind_method(D_METHOD("set_buff_name", "p_value"), &AttributeBuff::set_buff_name);
	ClassDB::bind_method(D_METHOD("set_duration", "p_value"), &AttributeBuff::set_duration);
	ClassDB::bind_method(D_METHOD("set_operation", "p_value"), &AttributeBuff::set_operation);
	ClassDB::bind_method(D_METHOD("set_max_applies", "p_value"), &AttributeBuff::set_max_applies);
	ClassDB::bind_method(D_METHOD("set_transient", "p_value"), &AttributeBuff::set_transient);
	ClassDB::bind_method(D_METHOD("set_unique", "p_value"), &AttributeBuff::set_unique);

	/// binds virtuals to godot
	GDVIRTUAL_BIND(_applies_to, "attribute_set");
	GDVIRTUAL_BIND(_operate, "values");

	/// binds properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "attribute_name"), "set_attribute_name", "get_attribute_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "buff_name"), "set_buff_name", "get_buff_name");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "duration"), "set_duration", "get_duration");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "operation", PROPERTY_HINT_RESOURCE_TYPE, "AttributeOperation"), "set_operation", "get_operation");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_applies"), "set_max_applies", "get_max_applies");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "transient"), "set_transient", "get_transient");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "unique"), "set_unique", "get_unique");
}

bool AttributeBuff::operator==(const Ref<AttributeBuff> &buff) const
{
	return buff->attribute_name == attribute_name && buff->buff_name == buff_name && buff->duration == duration && buff->operation == operation;
}

float AttributeBuff::operate(float base_value) const
{
	ERR_FAIL_COND_V_MSG(operation.is_null(), 0.0f, "AttributeBuff operation is null, cannot operate on base value.");
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

float AttributeBuff::get_duration() const
{
	return duration;
}

bool AttributeBuff::get_transient() const
{
	if (Math::is_zero_approx(duration)) {
		return transient;
	}

	return !Math::is_equal_approx(duration, 0.05f);
}

bool AttributeBuff::get_unique() const
{
	return unique;
}

bool AttributeBuff::is_operate_overridden() const
{
	return GDVIRTUAL_IS_OVERRIDDEN_PTR(this, _operate);
}

Ref<AttributeOperation> AttributeBuff::get_operation() const
{
	return operation;
}

int AttributeBuff::get_max_applies() const
{
	return max_applies;
}

bool AttributeBuff::is_time_limited() const
{
	return abs(1.0f - duration) > 0.0001f;
}

void AttributeBuff::set_attribute_name(const String &p_value)
{
	attribute_name = p_value;
}

void AttributeBuff::set_buff_name(const String &p_value)
{
	buff_name = p_value;
}

void AttributeBuff::set_duration(const float p_value)
{
	duration = p_value;
}

void AttributeBuff::set_operation(const Ref<AttributeOperation> &p_value)
{
	operation = p_value;
}

void AttributeBuff::set_max_applies(const int p_value)
{
	max_applies = p_value;
}

void AttributeBuff::set_transient(const bool p_value)
{
	transient = p_value;
}

void AttributeBuff::set_unique(const bool p_value)
{
	unique = p_value;
}

#pragma endregion

#pragma region AttributeBase

void AttributeBase::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("get_attribute_name"), &AttributeBase::get_attribute_name);
	ClassDB::bind_method(D_METHOD("get_buffs"), &AttributeBase::get_buffs);
	ClassDB::bind_method(D_METHOD("set_attribute_name", "p_value"), &AttributeBase::set_attribute_name);
	ClassDB::bind_method(D_METHOD("set_buffs", "p_buffs"), &AttributeBase::set_buffs);

	/// binds virtuals to godot
	GDVIRTUAL_BIND(_derived_from, "attribute_set");
	GDVIRTUAL_BIND(_get_buffed_value, "values");
	GDVIRTUAL_BIND(_get_initial_value, "attribute_set");
	GDVIRTUAL_BIND(_get_max_value, "attribute_set");
	GDVIRTUAL_BIND(_get_min_value, "attribute_set");

	/// binds properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "attribute_name"), "set_attribute_name", "get_attribute_name");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "buffs"), "set_buffs", "get_buffs");
}

String AttributeBase::get_attribute_name() const
{
	if (attribute_name.is_empty()) {
		return get_class_static();
	}

	return attribute_name;
}

TypedArray<AttributeBuff> AttributeBase::get_buffs() const
{
	return buffs;
}

void AttributeBase::set_attribute_name(const String &p_value)
{
	attribute_name = p_value;
}

void AttributeBase::set_buffs(const TypedArray<AttributeBuff> &p_buffs)
{
	buffs = p_buffs;
}

#pragma endregion

#pragma region Attribute

void Attribute::_bind_methods()
{
	/// static methods to bind to godot
	ClassDB::bind_static_method("Attribute", D_METHOD("create", "attribute_name", "initial_value", "min_value", "max_value"), &Attribute::create);

	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("get_initial_value"), &Attribute::get_initial_value);
	ClassDB::bind_method(D_METHOD("get_max_value"), &Attribute::get_max_value);
	ClassDB::bind_method(D_METHOD("get_min_value"), &Attribute::get_min_value);
	ClassDB::bind_method(D_METHOD("set_initial_value", "p_value"), &Attribute::set_initial_value);
	ClassDB::bind_method(D_METHOD("set_max_value", "p_value"), &Attribute::set_max_value);
	ClassDB::bind_method(D_METHOD("set_min_value", "p_value"), &Attribute::set_min_value);

	/// properties to bind to godot
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "initial_value"), "set_initial_value", "get_initial_value");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_value"), "set_max_value", "get_max_value");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_value"), "set_min_value", "get_min_value");
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
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "attributes", PROPERTY_HINT_RESOURCE_TYPE, "24/17:AttributeBase"), "set_attributes", "get_attributes");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "set_name"), "set_set_name", "get_set_name");

	/// adds signals to godot
	ADD_SIGNAL(MethodInfo("attribute_added", PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBase")));
	ADD_SIGNAL(MethodInfo("attribute_removed", PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBase")));
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
	attributes = TypedArray<AttributeBase>();
}

AttributeSet::AttributeSet(TypedArray<AttributeBase> p_attributes, String p_set_name)
{
	attributes = p_attributes.duplicate(true);
	set_name = p_set_name;
}

bool AttributeSet::add_attribute(const Ref<AttributeBase> &p_attribute)
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

uint16_t AttributeSet::add_attributes(const TypedArray<AttributeBase> &p_attributes)
{
	uint16_t count = 0;

	for (int i = 0; i < p_attributes.size(); i++) {
		if (!has_attribute(p_attributes[i])) {
			Ref<AttributeBase> d_attribute = p_attributes[i];

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

int AttributeSet::find(const Ref<AttributeBase> &p_attribute) const
{
	return attributes.find(p_attribute);
}

Ref<AttributeBase> AttributeSet::find_by_classname(const String &p_classname) const
{
	for (int i = 0; i < attributes.size(); i++) {
		Ref<AttributeBase> attribute = attributes[i];

		if (attribute->get_class() == p_classname) {
			return attributes[i];
		}
	}

	return Ref<Attribute>();
}

Ref<AttributeBase> AttributeSet::find_by_name(const String &p_name) const
{
	for (int i = 0; i < attributes.size(); i++) {
		Ref<AttributeBase> attribute = attributes[i];

		if (attribute->get_attribute_name() == p_name) {
			return attributes[i];
		}
	}

	return Ref<AttributeBase>();
}

PackedStringArray AttributeSet::get_attributes_names() const
{
	PackedStringArray names = PackedStringArray();

	for (int i = 0; i < attributes.size(); i++) {
		Ref<AttributeBase> attribute = attributes[i];
		names.push_back(attribute->get_attribute_name());
	}

	return names;
}

TypedArray<AttributeBase> AttributeSet::get_attributes() const
{
	return attributes;
}

Ref<AttributeBase> AttributeSet::get_at(int index) const
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

bool AttributeSet::has_attribute(const Ref<AttributeBase> &p_attribute) const
{
	for (int i = 0; i < attributes.size(); i++) {
		if (attributes[i] == p_attribute) {
			return true;
		}
	}

	return false;
}

bool AttributeSet::remove_attribute(const Ref<AttributeBase> &p_attribute)
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

int AttributeSet::remove_attributes(const TypedArray<AttributeBase> &p_attributes)
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

void AttributeSet::push_back(const Ref<AttributeBase> &p_attribute)
{
	attributes.push_back(p_attribute);
	emit_signal("attribute_added", p_attribute);
	emit_changed();
}

void AttributeSet::set_attributes(const TypedArray<AttributeBase> &p_attributes)
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

#pragma region BuffPoolQueueItem

void RuntimeBuff::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_static_method("RuntimeBuff", D_METHOD("from_buff", "p_buff"), &RuntimeBuff::from_buff);
	ClassDB::bind_static_method("RuntimeBuff", D_METHOD("to_buff", "p_buff"), &RuntimeBuff::to_buff);
	ClassDB::bind_method(D_METHOD("get_attribute_name"), &RuntimeBuff::get_attribute_name);
	ClassDB::bind_method(D_METHOD("get_buff_name"), &RuntimeBuff::get_buff_name);
	ClassDB::bind_method(D_METHOD("get_duration"), &RuntimeBuff::get_duration);
	ClassDB::bind_method(D_METHOD("get_time_left"), &RuntimeBuff::get_time_left);
	ClassDB::bind_method(D_METHOD("set_time_left", "p_value"), &RuntimeBuff::set_time_left);
	ClassDB::bind_method(D_METHOD("get_buff"), &RuntimeBuff::get_buff);
	ClassDB::bind_method(D_METHOD("set_buff", "p_value"), &RuntimeBuff::set_buff);

	/// binds properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBuff"), "set_buff", "get_buff");
}

bool RuntimeBuff::equals_to(const Ref<AttributeBuff> &p_buff) const
{
	return buff == p_buff;
}

TypedArray<RuntimeAttribute> RuntimeBuff::applies_to(const AttributeContainer *p_attribute_container) const
{
	TypedArray<RuntimeAttribute> attributes = TypedArray<RuntimeAttribute>();
	Ref<AttributeSet> attribute_set = p_attribute_container->get_attribute_set();

	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(buff, _applies_to)) {
		TypedArray<AttributeBase> _attributes = TypedArray<AttributeBase>();

		if (GDVIRTUAL_CALL_PTR(buff, _applies_to, attribute_set, _attributes)) {
			for (int i = 0; i < _attributes.size(); i++) {
				Ref<AttributeBase> attribute_base = _attributes[i];
				Ref<RuntimeAttribute> attribute = p_attribute_container->get_attribute_by_name(attribute_base->get_attribute_name());
				ERR_FAIL_COND_V_MSG(attribute.is_null(), attributes, "Attribute not found in attribute set.");
				attributes.push_back(attribute);
			}
		}
	} else {
		Ref<RuntimeAttribute> attribute = p_attribute_container->get_attribute_by_name(buff->get_attribute_name());

		ERR_FAIL_COND_V_MSG(attribute.is_null(), attributes, "Attribute not found in attribute set.");
		ERR_FAIL_COND_V_MSG(!attribute.is_valid(), attributes, "Attribute reference is not valid.");

		attributes.push_back(attribute);
	}

	ERR_FAIL_COND_V_EDMSG(attributes.size() == 0, attributes, "Overloaded _applies_to returned 0 attributes.");

	return attributes;
}

TypedArray<float> RuntimeBuff::operate(const TypedArray<RuntimeAttribute> &p_runtime_attributes) const
{
	ERR_FAIL_COND_V_MSG(!buff.is_valid(), TypedArray<float>(), "Buff is not valid, cannot operate on runtime attributes.");
	ERR_FAIL_COND_V_MSG(p_runtime_attributes.size() == 0, TypedArray<float>(), "Runtime attributes are empty, cannot operate on them.");

	TypedArray<float> values = TypedArray<float>();

	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(buff, _operate)) {
		TypedArray<AttributeOperation> operations = TypedArray<AttributeOperation>();
		TypedArray<float> attribute_values = TypedArray<float>();

		for (int i = 0; i < p_runtime_attributes.size(); i++) {
			Ref<RuntimeAttribute> runtime_attribute = p_runtime_attributes[i];
			attribute_values.push_back(runtime_attribute->get_value());
		}

		ERR_FAIL_COND_V_MSG(attribute_values.size() == 0, attribute_values, "_operate returning values are empty, cannot operate on them.");

		if (GDVIRTUAL_CALL_PTR(buff, _operate, attribute_values, operations)) {
			ERR_FAIL_COND_V_MSG(operations.size() == 0, values, "_operate returning operations are empty, cannot operate on them.");

			for (int i = 0; i < operations.size(); i++) {
				Ref<AttributeOperation> operation = operations[i];
				float attribute_value = attribute_values[i];
				float operation_result = operation->operate(attribute_value);

				values.push_back(operation->operate(attribute_value));
			}
		}
	} else if (p_runtime_attributes.size() > 0) {
		Ref<AttributeBuff> attribute_buff = buff;
		Ref<RuntimeAttribute> first_attribute = p_runtime_attributes[0];

		values.push_back(attribute_buff->operate(first_attribute->get_value()));
	}

	return values;
}

Ref<RuntimeBuff> RuntimeBuff::from_buff(const Ref<AttributeBuff> &p_buff)
{
	Ref<RuntimeBuff> runtime_buff = memnew(RuntimeBuff);
	runtime_buff->buff = p_buff;
	runtime_buff->time_left = p_buff->get_duration();
	return runtime_buff;
}

Ref<AttributeBuff> RuntimeBuff::to_buff(const Ref<RuntimeBuff> &p_buff)
{
	return p_buff->buff;
}

bool RuntimeBuff::operator==(const Ref<AttributeBuff> &p_attribute_buff) const
{
	return equals_to(p_attribute_buff);
}

bool RuntimeBuff::operator==(const Ref<RuntimeBuff> &p_runtime_buff) const
{
	return buff == p_runtime_buff->buff && time_left == p_runtime_buff->time_left;
}

bool RuntimeBuff::can_apply_to_attribute(const Ref<RuntimeAttribute> &p_attribute) const
{
	TypedArray<RuntimeAttribute> _applies_to = applies_to(p_attribute->attribute_container);

	for (int i = 0; i < _applies_to.size(); i++) {
		Ref<RuntimeAttribute> runtime_attribute = _applies_to[i];

		if (runtime_attribute->attribute == p_attribute->attribute) {
			return true;
		}
	}

	return false;
}

bool RuntimeBuff::can_dequeue() const
{
	return Math::is_zero_approx(time_left);
}

String RuntimeBuff::get_attribute_name() const
{
	return buff->attribute_name;
}

String RuntimeBuff::get_buff_name() const
{
	return buff->buff_name;
}

Ref<AttributeBuff> RuntimeBuff::get_buff() const
{
	return buff;
}

float RuntimeBuff::get_duration() const
{
	return buff->duration;
}

float RuntimeBuff::get_time_left() const
{
	return time_left;
}

bool RuntimeBuff::is_operate_overridden() const
{
	return GDVIRTUAL_IS_OVERRIDDEN_PTR(buff, _operate);
}

void RuntimeBuff::set_buff(const Ref<AttributeBuff> &p_value)
{
	buff = p_value;
}

void RuntimeBuff::set_time_left(const float p_value)
{
	time_left = p_value;
}

#pragma endregion

#pragma region RuntimeAttributeBase

void RuntimeAttribute::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("add_buff", "p_buff"), &RuntimeAttribute::add_buff);
	ClassDB::bind_method(D_METHOD("add_buffs", "p_buffs"), &RuntimeAttribute::add_buffs);
	ClassDB::bind_method(D_METHOD("can_receive_buff", "p_buff"), &RuntimeAttribute::can_receive_buff);
	ClassDB::bind_method(D_METHOD("clear_buffs"), &RuntimeAttribute::clear_buffs);
	ClassDB::bind_method(D_METHOD("get_attribute"), &RuntimeAttribute::get_attribute);
	ClassDB::bind_method(D_METHOD("get_attribute_set"), &RuntimeAttribute::get_attribute_set);
	ClassDB::bind_method(D_METHOD("get_buffed_value"), &RuntimeAttribute::get_buffed_value);
	ClassDB::bind_method(D_METHOD("get_buffs"), &RuntimeAttribute::get_buffs);
	ClassDB::bind_method(D_METHOD("get_derived_from"), &RuntimeAttribute::get_derived_from);
	ClassDB::bind_method(D_METHOD("get_min_value"), &RuntimeAttribute::get_min_value);
	ClassDB::bind_method(D_METHOD("get_initial_value"), &RuntimeAttribute::get_initial_value);
	ClassDB::bind_method(D_METHOD("get_max_value"), &RuntimeAttribute::get_max_value);
	ClassDB::bind_method(D_METHOD("get_value"), &RuntimeAttribute::get_value);
	ClassDB::bind_method(D_METHOD("remove_buff", "p_buff"), &RuntimeAttribute::remove_buff);
	ClassDB::bind_method(D_METHOD("remove_buffs", "p_buffs"), &RuntimeAttribute::remove_buffs);
	ClassDB::bind_method(D_METHOD("set_attribute", "p_value"), &RuntimeAttribute::set_attribute);
	ClassDB::bind_method(D_METHOD("set_attribute_set", "p_value"), &RuntimeAttribute::set_attribute_set);
	ClassDB::bind_method(D_METHOD("set_buffs", "p_buffs"), &RuntimeAttribute::set_buffs);
	ClassDB::bind_method(D_METHOD("set_value", "p_value"), &RuntimeAttribute::set_value);

	/// binds properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBase"), "set_attribute", "get_attribute");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "value"), "set_value", "get_value");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "buffs"), "set_buffs", "get_buffs");

	/// adds signals to godot
	ADD_SIGNAL(MethodInfo("attribute_changed", PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "RuntimeAttributeBase"), PropertyInfo(Variant::FLOAT, "previous_value"), PropertyInfo(Variant::FLOAT, "new_value")));
	ADD_SIGNAL(MethodInfo("buff_added", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "RuntimeBuff")));
	ADD_SIGNAL(MethodInfo("buff_removed", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "RuntimeBuff")));
	ADD_SIGNAL(MethodInfo("buffs_cleared"));
}

bool RuntimeAttribute::add_buff(const Ref<AttributeBuff> &p_buff)
{
	if (!can_receive_buff(p_buff)) {
		return false;
	}

	Ref<RuntimeBuff> runtime_buff = RuntimeBuff::from_buff(p_buff);
	ERR_FAIL_COND_V_MSG(runtime_buff.is_null(), false, "Failed to create runtime buff from attribute buff.");

	if (p_buff->get_transient()) {
		buffs.push_back(runtime_buff);
		emit_signal("buff_added", runtime_buff);
	} else {
		TypedArray<RuntimeAttribute> affected_attributes = runtime_buff->applies_to(attribute_container);
		float max = attribute->get_max_value();
		float min = attribute->get_min_value();
		float prev_value = value;

		ERR_FAIL_COND_V_EDMSG(affected_attributes.size() == 0, false, "Runtime buff does not apply to any attribute.");

		if (runtime_buff->can_apply_to_attribute(this)) {
			TypedArray<float> values = runtime_buff->operate(affected_attributes);
			ERR_FAIL_COND_V_MSG(values.size() == 0, false, "Failed to operate on affected attributes.");
			ERR_FAIL_COND_V_MSG(values.size() != affected_attributes.size(), false, "Operated values size does not match affected attributes size.");

			for (int i = 0; i < affected_attributes.size(); i++) {
				Ref<RuntimeAttribute> affected_attribute = affected_attributes[i];
				if (affected_attribute->attribute == attribute) {
					float new_value = values[i];

					if (Math::is_equal_approx(max, min)) {
						value = new_value;
					} else if (min < max) {
						value = Math::clamp(new_value, min, max);
					} else if (max == 0) {
						value = new_value > min ? new_value : min;
					} else {
						return false;
					}

					break;
				}
			}
		}

		emit_signal("attribute_changed", this, prev_value, value);
	}

	return true;
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
	if (p_buff->get_unique() && has_buff(p_buff)) {
		return false;
	}

	int buffs_count = 0;

	for (int i = 0; i < buffs.size(); i++) {
		Ref<RuntimeBuff> buff = buffs[i];
		if (buff->equals_to(p_buff)) {
			buffs_count++;
		}
	}

	if (buffs_count >= p_buff->get_max_applies() && p_buff->get_max_applies() > 0) {
		return false;
	}

	if (p_buff->is_operate_overridden()) {
		TypedArray<RuntimeAttribute> applied_to_attributes = RuntimeBuff::from_buff(p_buff)->applies_to(attribute_container);

		for (int i = 0; i < applied_to_attributes.size(); i++) {
			Ref<RuntimeAttribute> applied_to_attribute = applied_to_attributes[i];

			if (applied_to_attribute->attribute == attribute) {
				return true;
			}
		}

		return false;
	}

	return p_buff->get_attribute_name() == attribute->get_attribute_name();
}

void RuntimeAttribute::clear_buffs()
{
	buffs.clear();
}

bool RuntimeAttribute::has_buff(const Ref<AttributeBuff> &p_buff) const
{
	for (int i = 0; i < buffs.size(); i++) {
		Ref<RuntimeBuff> buff = buffs[i];

		if (buff->equals_to(p_buff)) {
			return true;
		}
	}

	return false;
}

bool RuntimeAttribute::remove_buff(const Ref<AttributeBuff> &p_buff)
{
	for (int i = 0; i < buffs.size(); i++) {
		Ref<RuntimeBuff> buff = buffs[i];

		if (buff->equals_to(p_buff)) {
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
			Ref<RuntimeBuff> buff = buffs[j];

			if (buff->equals_to(p_buffs[i])) {
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

Ref<Attribute> RuntimeAttribute::get_attribute() const
{
	return attribute;
}

Ref<AttributeSet> RuntimeAttribute::get_attribute_set() const
{
	return attribute_set;
}

float RuntimeAttribute::get_buffed_value() const
{
	float current_value = value;

	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(attribute, _get_buffed_value)) {
		TypedArray<AttributeBase> derived_from = get_derived_from();
		TypedArray<float> values = TypedArray<float>();

		// todo: rework this. It makes impossible to apply an overridden AttributeBuff to a derived attribute.

		if (derived_from.size() > 0) {
			for (int i = 0; i < derived_from.size(); i++) {
				Ref<AttributeBase> derived_attribute = derived_from[i];
				values.push_back(attribute_container->get_attribute_buffed_value_by_name(derived_attribute->get_attribute_name()));
			}
		}

		GDVIRTUAL_CALL_PTR(attribute, _get_buffed_value, values, current_value);
	}

	for (int i = 0; i < buffs.size(); i++) {
		Ref<RuntimeBuff> buff = buffs[i];
		Ref<AttributeBuff> attribute_buff = buff->get_buff();

		if (attribute_buff.is_valid() && !attribute_buff.is_null()) {
			current_value = attribute_buff->operate(current_value);
		}
	}

	return current_value;
}

TypedArray<AttributeBase> RuntimeAttribute::get_derived_from() const
{
	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(attribute, _derived_from)) {
		TypedArray<AttributeBase> derived_attributes = TypedArray<AttributeBase>();

		if (GDVIRTUAL_CALL_PTR(attribute, _derived_from, attribute_set, derived_attributes)) {
			return derived_attributes;
		}
	}

	return TypedArray<AttributeBase>();
}

float RuntimeAttribute::get_min_value() const
{
	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(attribute, _get_min_value)) {
		float ret;

		if (GDVIRTUAL_CALL_PTR(attribute, _get_min_value, attribute_set, ret)) {
			return ret;
		}
	}

	return attribute->get_min_value();
}

float RuntimeAttribute::get_initial_value() const
{
	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(attribute, _get_initial_value)) {
		float ret;
		TypedArray<AttributeBase> base_attributes = get_derived_from();

		ERR_FAIL_COND_V_MSG(base_attributes.size() == 0, 0, "Attribute set must be set to get initial value. Please override _derived_from method.");

		PackedFloat32Array values = PackedFloat32Array();

		for (int i = 0; i < base_attributes.size(); i++) {
			Ref<AttributeBase> base_attribute = base_attributes[i];
			values.push_back(base_attribute->get_initial_value());
		}

		if (GDVIRTUAL_CALL_PTR(attribute, _get_initial_value, values, ret)) {
			return ret;
		}
	}

	return attribute->get_initial_value();
}

float RuntimeAttribute::get_max_value() const
{
	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(attribute, _get_max_value)) {
		float ret;

		if (GDVIRTUAL_CALL_PTR(attribute, _get_max_value, attribute_set, ret)) {
			return ret;
		}
	}

	return attribute->get_max_value();
}

float RuntimeAttribute::get_value()
{
	return value;
}

TypedArray<RuntimeBuff> RuntimeAttribute::get_buffs() const
{
	return buffs;
}

void RuntimeAttribute::set_attribute(const Ref<AttributeBase> &p_value)
{
	attribute = p_value;
}

void RuntimeAttribute::set_value(const float p_value)
{
	float max_value = get_max_value();

	if (Math::is_zero_approx(max_value)) {
		value = p_value > get_min_value() ? p_value : get_min_value();
	} else {
		value = Math::clamp(p_value, get_min_value(), get_max_value());
	}
}

void RuntimeAttribute::set_buffs(const TypedArray<AttributeBuff> &p_value)
{
	buffs.clear();

	for (int i = 0; i < p_value.size(); i++) {
		buffs.push_back(RuntimeBuff::from_buff(p_value[i]));
	}
}

void RuntimeAttribute::set_attribute_set(const Ref<AttributeSet> &p_value)
{
	attribute_set = p_value;
}

#pragma endregion
