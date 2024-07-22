#ifndef GGA_DERIVED_ATTRIBUTE_HPP
#define GGA_DERIVED_ATTRIBUTE_HPP

#include <godot_cpp/classes/resource.hpp>

#include "attribute.hpp"

using namespace godot;

namespace gga
{
	/// @brief A derived attribute is an attribute that is calculated based on other attributes given by the AttributeSet.
	class DerivedAttribute : public AttributeBase
	{
		GDCLASS(DerivedAttribute, AttributeBase);

	protected:
		/// @brief Bind methods to Godot.
		static void _bind_methods();

	public:
		/// @brief Get the initial value of the attribute.
		/// @return The initial value of the attribute.
		virtual float get_initial_value(const Ref<AttributeSet> &p_attribute_container) const;
		/// @brief Get the maximum value of the attribute.
		/// @return The maximum value of the attribute.
		virtual float get_max_value(const Ref<AttributeSet> &p_attribute_container) const;
		/// @brief Get the minimum value of the attribute.
		/// @return The minimum value of the attribute.
		virtual float get_min_value(const Ref<AttributeSet> &p_attribute_container) const;
	};

	class RuntimeDerivedAttribute : public DerivedAttribute
	{
		GDCLASS(RuntimeDerivedAttribute, DerivedAttribute);

	protected:
		/// @brief Bind methods to Godot.
		static void _bind_methods();

		Ref<DerivedAttribute> derived_attribute;

	public:
		static Ref<RuntimeDerivedAttribute> from(const Ref<DerivedAttribute> &p_derived_attribute);
		static Ref<DerivedAttribute> to(const Ref<RuntimeDerivedAttribute> &p_runtime_derived_attribute);
	};
} //namespace gga

#endif
