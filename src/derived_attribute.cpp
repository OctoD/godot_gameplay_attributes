#include "derived_attribute.hpp"

using namespace gga;

#pragma region DerivedAttribute

void DerivedAttribute::_bind_methods()
{
	/// binds virtual methods to godot
	PropertyInfo argument = PropertyInfo(Variant::OBJECT, "attribute_set", PROPERTY_HINT_RESOURCE_TYPE, "AttributeSet");
	Vector<StringName> args = Vector<StringName>({ "attribute_set" });

	ClassDB::add_virtual_method(get_class_static(), MethodInfo(Variant::FLOAT, "get_initial_value", argument), args);
	ClassDB::add_virtual_method(get_class_static(), MethodInfo(Variant::FLOAT, "get_max_value", argument), args);
	ClassDB::add_virtual_method(get_class_static(), MethodInfo(Variant::FLOAT, "get_min_value", argument), args);
}

float DerivedAttribute::get_initial_value(const Ref<AttributeSet> &p_attribute_container) const
{
	return 0.0f;
}

float DerivedAttribute::get_max_value(const Ref<AttributeSet> &p_attribute_container) const
{
	return 0.0f;
}

float DerivedAttribute::get_min_value(const Ref<AttributeSet> &p_attribute_container) const
{
	return 0.0f;
}

#pragma endregion

#pragma region RuntimeDerivedAttribute

void gga::RuntimeDerivedAttribute::_bind_methods()
{
}

Ref<RuntimeDerivedAttribute> gga::RuntimeDerivedAttribute::from(const Ref<DerivedAttribute> &p_derived_attribute)
{
	return Ref<RuntimeDerivedAttribute>();
}

Ref<DerivedAttribute> gga::RuntimeDerivedAttribute::to(const Ref<RuntimeDerivedAttribute> &p_runtime_derived_attribute)
{
	return Ref<DerivedAttribute>();
}

#pragma endregion
