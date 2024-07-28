#include "derived_attribute.hpp"

using namespace gga;

#pragma region DerivedAttribute

void DerivedAttribute::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("get_attribute_set"), &DerivedAttribute::get_attribute_set);
	ClassDB::bind_method(D_METHOD("set_attribute_set", "attribute_set"), &DerivedAttribute::set_attribute_set);

	/// binds virtual methods to godot
	GDVIRTUAL_BIND(_get_initial_value, "attribute_set");
	GDVIRTUAL_BIND(_get_max_value, "attribute_set");
	GDVIRTUAL_BIND(_get_min_value, "attribute_set");

	/// binds properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "attribute_set", PROPERTY_HINT_RESOURCE_TYPE, "AttributeSet"), "set_attribute_set", "get_attribute_set");
}

Ref<AttributeSet> DerivedAttribute::get_attribute_set() const
{
	return attribute_set;
}

float DerivedAttribute::get_initial_value() const
{
	if (GDVIRTUAL_IS_OVERRIDDEN(_get_initial_value)) {
		float ret;

		if (GDVIRTUAL_CALL(_get_initial_value, attribute_set, ret)) {
			return ret;
		}
	}

	return 0.0f;
}

float DerivedAttribute::get_max_value() const
{
	if (GDVIRTUAL_IS_OVERRIDDEN(_get_max_value)) {
		float ret;

		if (GDVIRTUAL_CALL(_get_max_value, attribute_set, ret)) {
			return ret;
		}
	}

	return 0.0f;
}

float DerivedAttribute::get_min_value() const
{
	if (GDVIRTUAL_IS_OVERRIDDEN(_get_min_value)) {
		float ret;

		if (GDVIRTUAL_CALL(_get_min_value, attribute_set, ret)) {
			return ret;
		}
	}

	return 0.0f;
}

void DerivedAttribute::set_attribute_set(const Ref<AttributeSet> &p_attribute_set)
{
	attribute_set = p_attribute_set;
}

#pragma endregion

#pragma region RuntimeDerivedAttribute

void RuntimeDerivedAttribute::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("get_derived_attribute"), &RuntimeDerivedAttribute::get_derived_attribute);
	ClassDB::bind_method(D_METHOD("set_derived_attribute", "derived_attribute"), &RuntimeDerivedAttribute::set_derived_attribute);

	/// binds properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "derived_attribute", PROPERTY_HINT_RESOURCE_TYPE, "DerivedAttribute"), "set_derived_attribute", "get_derived_attribute");
}

Ref<DerivedAttribute> RuntimeDerivedAttribute::get_derived_attribute() const
{
	return derived_attribute;
}

void RuntimeDerivedAttribute::set_derived_attribute(const Ref<DerivedAttribute> &p_derived_attribute)
{
	derived_attribute = p_derived_attribute;
}

#pragma endregion
