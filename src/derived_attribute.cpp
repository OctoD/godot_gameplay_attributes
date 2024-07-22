#include "derived_attribute.hpp"

using namespace gga;

#pragma region DerivedAttribute

void DerivedAttribute::_bind_methods()
{
	/// binds virtual methods to godot
	PropertyInfo argument = PropertyInfo(Variant::OBJECT, "attribute_container");
	Vector<StringName> args = Vector<StringName>({ "attribute_container" });

	ClassDB::add_virtual_method(get_class_static(), MethodInfo(Variant::FLOAT, "get_initial_value"), args);
	ClassDB::add_virtual_method(get_class_static(), MethodInfo(Variant::FLOAT, "get_max_value"), args);
	ClassDB::add_virtual_method(get_class_static(), MethodInfo(Variant::FLOAT, "get_min_value"), args);
}

float DerivedAttribute::get_initial_value(const Ref<AttributeContainer> &p_attribute_container) const
{
	return 0.0f;
}

float DerivedAttribute::get_max_value(const Ref<AttributeContainer> &p_attribute_container) const
{
	return 0.0f;
}

float DerivedAttribute::get_min_value(const Ref<AttributeContainer> &p_attribute_container) const
{
	return 0.0f;
}

#pragma endregion