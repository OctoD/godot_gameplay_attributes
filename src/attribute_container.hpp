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
	class Attribute;
	class AttributeBuff;
	class AttributeSet;
	class BuffPoolQueue;

	class AttributeContainer : public Node
	{
		GDCLASS(AttributeContainer, Node);

	protected:
		/// @brief Bind methods to Godot.
		static void _bind_methods();
		/// @brief Attributes.
		Ref<AttributeSet> attribute_set;
		/// @brief Buff pool queue. It is used to store buffs that have a limited duration.
		BuffPoolQueue *buff_pool_queue;
		/// @brief Autostart. If set to true, the container will start processing buffs as soon as it is ready.
		bool autostart;
		/// @brief Paused. If set to true, the container will not process buffs.
		bool paused;
		/// @brief Server authoritative. If set to true, the container will only process buffs on the server.
		bool server_authoritative;

		/// @brief Handles the attribute_changed signal.
		/// @param p_attribute The attribute that changed.
		/// @param p_previous_value The previous value of the attribute.
		/// @param p_new_value The new value of the attribute.
		void _on_attribute_changed(Ref<Attribute> p_attribute, const float p_previous_value, const float p_new_value);
		/// @brief Handles the buff_dequeued signal.
		/// @param p_buff The buff that was dequeued.
		void _on_buff_dequeued(Ref<AttributeBuff> p_buff);
		/// @brief Handles the buff_enqueued signal.
		/// @param p_buff The buff that was enqueued.
		void _on_buff_enqueued(Ref<AttributeBuff> p_buff);
		/// @brief Binds an attribute to the container.
		void bind_attribute(Ref<Attribute> p_attribute);
		/// @brief Checks if the container has a specific attribute.
		bool has_attribute(Ref<Attribute> p_attribute);

	public:
		/// @brief Constructor.
		AttributeContainer();
		/// @brief Constructor.
		/// @param p_attribute_set The attribute set to use. 
		/// @param p_autostart The autostart value.
		/// @param p_server_authoritative The server authoritative value.
		AttributeContainer(const Ref<AttributeSet> &p_attribute_set, const bool p_autostart, const bool p_server_authoritative, const bool p_paused);
		/// @brief Destructor.
		~AttributeContainer();
		/// @brief Override of the _ready method.
		void _ready() override;
		/// @brief Adds an attribute to the container.
		/// @param p_attribute The attribute to add.
		void add_attribute(Ref<Attribute> p_attribute);
		/// @brief Adds a buff to the container.
		/// @param p_buff The buff to add.
		void apply_buff(Ref<AttributeBuff> p_buff);
		/// @brief Pauses the container.
		void pause();
		/// @brief Checks if the container is paused.
		/// @return True if the container is paused, false otherwise.
		bool is_paused() const;
		/// @brief Removes an attribute from the container.
		/// @param p_attribute The attribute to remove.
		void remove_attribute(Ref<Attribute> p_attribute);
		/// @brief Removes a buff from the container.
		/// @param p_buff The buff to remove.
		void remove_buff(Ref<AttributeBuff> p_buff);
		/// @brief Resumes the container.
		void resume();

		// getters/setters
		/// @brief Returns the attributes of the container.
		/// @return The attributes of the container.
		Ref<AttributeSet> get_attribute_set() const;
		/// @brief Returns the autostart value.
		/// @return The autostart value.
		bool get_autostart() const;
		/// @brief Returns the server authoritative value.
		/// @return The server authoritative value.
		bool get_server_authoritative() const;
		/// @brief Sets the attributes of the container.
		/// @param p_attributes The attributes to set.
		void set_attribute_set(const Ref<AttributeSet> &p_attributes);
		/// @brief Sets the autostart value.
		/// @param p_autostart The autostart value to set.
		void set_autostart(const bool p_autostart);
		/// @brief Sets the server authoritative value.
		/// @param p_server_authoritative The server authoritative value to set.
		void set_server_authoritative(const bool p_server_authoritative);
	};
} //namespace gga

#endif
