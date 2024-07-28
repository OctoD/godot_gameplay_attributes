#ifndef GGA_DERIVED_ATTRIBUTE_HPP
#define GGA_DERIVED_ATTRIBUTE_HPP

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

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
		/// @brief The attribute set that contains the attributes used to calculate the derived attribute.
		Ref<AttributeSet> attribute_set;

	public:
		/// @brief Get the attribute set.
		/// @return The attribute set.
		Ref<AttributeSet> get_attribute_set() const;
		/// @brief Get the initial value of the attribute.
		/// @return The initial value of the attribute.
		GDVIRTUAL1RC(float, _get_initial_value, const Ref<AttributeSet> &);
		virtual float get_initial_value() const override;
		/// @brief Get the maximum value of the attribute.
		/// @return The maximum value of the attribute.
		GDVIRTUAL1RC(float, _get_max_value, const Ref<AttributeSet> &);
		virtual float get_max_value() const override;
		/// @brief Get the minimum value of the attribute.
		/// @return The minimum value of the attribute.
		GDVIRTUAL1RC(float, _get_min_value, const Ref<AttributeSet> &);
		virtual float get_min_value() const override;
		/// @brief Set the attribute set.
		/// @param p_attribute_set The attribute set.
		void set_attribute_set(const Ref<AttributeSet> &p_attribute_set);
	};

	class RuntimeDerivedAttribute : public RuntimeAttribute
	{
		GDCLASS(RuntimeDerivedAttribute, RuntimeAttribute);

	protected:
		/// @brief Bind methods to Godot.
		static void _bind_methods();

		Ref<DerivedAttribute> derived_attribute;

	public:
		/// @brief Get the derived attribute.
		Ref<DerivedAttribute> get_derived_attribute() const;
		/// @brief Set the derived attribute.
		void set_derived_attribute(const Ref<DerivedAttribute> &p_derived_attribute);
	};
} //namespace gga

#endif
