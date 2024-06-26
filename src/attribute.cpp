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

	/// binds enum as consts
	BIND_ENUM_CONSTANT(BT_ONESHOT);
	BIND_ENUM_CONSTANT(BT_STACKABLE);
}

bool AttributeBuff::operator==(const Ref<AttributeBuff> &buff) const
{
	return buff->attribute_name == attribute_name && buff->buff_name == buff_name && buff->buff_type == buff_type && buff->duration == duration && buff->operation == operation;
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

AttributeOperation *AttributeBuff::get_operation() const
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
	operation = p_value.ptr();
}

#pragma endregion

#pragma region Attribute

void Attribute::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("add_buff", "p_buff"), &Attribute::add_buff);
	ClassDB::bind_method(D_METHOD("add_buffs", "p_buffs"), &Attribute::add_buffs);
	ClassDB::bind_method(D_METHOD("can_receive_buff", "p_buff"), &Attribute::can_receive_buff);
	ClassDB::bind_method(D_METHOD("clear_buffs"), &Attribute::clear_buffs);
	ClassDB::bind_method(D_METHOD("get_buffed_value"), &Attribute::current_value);
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

	/// properties to bind to godot
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "attribute_name"), "set_attribute_name", "get_attribute_name");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "initial_value"), "set_initial_value", "get_initial_value");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_value"), "set_max_value", "get_max_value");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_value"), "set_min_value", "get_min_value");

	/// adds signals to godot
	ADD_SIGNAL(MethodInfo("attribute_changed", PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "Attribute"), PropertyInfo(Variant::FLOAT, "value"), PropertyInfo(Variant::FLOAT, "value")));
	ADD_SIGNAL(MethodInfo("buff_added", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBuff")));
	ADD_SIGNAL(MethodInfo("buff_removed", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBuff")));
	ADD_SIGNAL(MethodInfo("buffs_cleared"));
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