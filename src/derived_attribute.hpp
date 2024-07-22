#ifndef GGA_DERIVED_ATTRIBUTE_HPP
#define GGA_DERIVED_ATTRIBUTE_HPP

#include <godot_cpp/classes/resource.hpp>

#include "attribute.hpp"
#include "attribute_container.hpp"

using namespace godot;

namespace gga
{

	class DerivedAttribute : public AttributeBase
	{
		GDCLASS(DerivedAttribute, AttributeBase);

	protected:
		/// @brief Bind methods to Godot.
		static void _bind_methods();

	public:
		/// @brief Get the initial value of the attribute.
		/// @return The initial value of the attribute.
		virtual float get_initial_value(const Ref<AttributeContainer> &p_attribute_container) const;
		/// @brief Get the maximum value of the attribute.
		/// @return The maximum value of the attribute.
		virtual float get_max_value(const Ref<AttributeContainer> &p_attribute_container) const;
		/// @brief Get the minimum value of the attribute.
		/// @return The minimum value of the attribute.
		virtual float get_min_value(const Ref<AttributeContainer> &p_attribute_container) const;
	};
} //namespace gga

#endif
