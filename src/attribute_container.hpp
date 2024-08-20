/**************************************************************************/
/*  attribute_container.hpp                                               */
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

#ifndef GGA_ATTRIBUTE_CONTAINER_HPP
#define GGA_ATTRIBUTE_CONTAINER_HPP

#include <godot_cpp/classes/node.hpp>

using namespace godot;

namespace gga
{
	class AttributeBase;
	class AttributeBuff;
	class AttributeSet;
	class BuffPoolQueue;
	class RuntimeAttribute;
	class RuntimeBuff;

	class AttributeContainer : public Node
	{
		GDCLASS(AttributeContainer, Node);

	protected:
		/// @brief Bind methods to Godot.
		static void _bind_methods();
		/// @brief Attribute's set.
		Ref<AttributeSet> attribute_set;
		/// @brief TypedArray of attributes.
		Dictionary attributes;
		/// @brief Buff pool queue. It is used to store buffs that have a limited duration.
		BuffPoolQueue *buff_pool_queue;
		/// @brief Derived attributes. These are attributes that are calculated from other attributes.
		Dictionary derived_attributes;
		/// @brief Server authoritative. If set to true, the container will only process buffs on the server.
		bool server_authoritative;

		/// @brief Handles the attribute_changed signal.
		/// @param p_attribute The attribute that changed.
		/// @param p_previous_value The previous value of the attribute.
		/// @param p_new_value The new value of the attribute.
		void _on_attribute_changed(Ref<RuntimeAttribute> p_attribute, const float p_previous_value, const float p_new_value);
		/// @brief Handles the buff_applied signal.
		/// @param p_buff The buff that was applied.
		void _on_buff_applied(Ref<RuntimeBuff> p_buff);
		/// @brief Handles the buff_dequeued signal.
		/// @param p_buff The buff that was dequeued.
		void _on_buff_dequeued(Ref<RuntimeBuff> p_buff);
		/// @brief Handles the buff_enqueued signal.
		/// @param p_buff The buff that was enqueued.
		void _on_buff_enqueued(Ref<RuntimeBuff> p_buff);
		/// @brief Handles the buff_removed signal.
		/// @param p_buff The buff that was removed.
		void _on_buff_removed(Ref<RuntimeBuff> p_buff);
		/// @brief Checks if the container has a specific attribute.
		bool has_attribute(Ref<AttributeBase> p_attribute);
		/// @brief Notifies derived attributes that an attribute has changed.
		/// @param p_runtime_attribute The attribute that changed.
		void notify_derived_attributes(Ref<RuntimeAttribute> p_runtime_attribute);

	public:
		/// @brief Override of the _physics_process method.
		/// @param p_delta The delta time.
		void _physics_process(double p_delta) override;
		/// @brief Override of the _ready method.
		void _ready() override;
		/// @brief Adds an attribute to the container.
		/// @param p_attribute The attribute to add.
		void add_attribute(Ref<AttributeBase> p_attribute);
		/// @brief Adds a buff to the container.
		/// @param p_buff The buff to add.
		void apply_buff(Ref<AttributeBuff> p_buff);
		/// @brief Removes an attribute from the container.
		/// @param p_attribute The attribute to remove.
		void remove_attribute(Ref<AttributeBase> p_attribute);
		/// @brief Removes a buff from the container.
		/// @param p_buff The buff to remove.
		void remove_buff(Ref<AttributeBuff> p_buff);
		/// @brief Setups the container.
		void setup();

		/// @brief Finds an attribute in the container.
		/// @param p_predicate The predicate to use to find the attribute.
		/// @return The attribute found.
		Ref<RuntimeAttribute> find(Callable p_predicate) const;
		/// @brief Finds an attribute buffed value in the container.
		/// @param p_predicate The predicate to use to find the attribute buffed value.
		/// @return The attribute buffed value found.
		float find_buffed_value(Callable p_predicate) const;
		/// @brief Finds an attribute value in the container.
		/// @param p_predicate The predicate to use to find the attribute value.
		/// @return The attribute value found.
		float find_value(Callable p_predicate) const;
		// getters/setters
		/// @brief Returns the attributes of the container.
		/// @return The attributes of the container.
		Ref<AttributeSet> get_attribute_set() const;
		/// @brief Returns the attributes of the container.
		/// @return The attributes of the container.
		TypedArray<RuntimeAttribute> get_attributes() const;
		/// @brief Gets an attribute by name.
		/// @param p_name The name of the attribute to get.
		/// @return The attribute with the given name.
		Ref<RuntimeAttribute> get_attribute_by_name(const String &p_name) const;
		/// @brief Gets the buffed value of an attribute by name. It returns 0.0f if the attribute is not found. Or if the actual value is 0.0f.
		/// @param p_name The name of the attribute to get.
		/// @return The buffed value of the attribute with the given name.
		float get_attribute_buffed_value_by_name(const String &p_name) const;
		/// @brief Gets the base value of an attribute by name.
		/// @param p_name The name of the attribute to get.
		/// @return The base value of the attribute with the given name.
		float get_attribute_value_by_name(const String &p_name) const;
		/// @brief Returns the server authoritative value.
		/// @return The server authoritative value.
		bool get_server_authoritative() const;
		/// @brief Sets the attributes of the container.
		/// @param p_attribute_set The attributes to set.
		void set_attribute_set(const Ref<AttributeSet> &p_attribute_set);
		/// @brief Sets the server authoritative value.
		/// @param p_server_authoritative The server authoritative value to set.
		void set_server_authoritative(const bool p_server_authoritative);
	};
} //namespace gga

#endif
