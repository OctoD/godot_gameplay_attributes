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
	class BuffPoolQueue;

	class AttributeContainer : public Node
	{
		GDCLASS(AttributeContainer, Node);

	protected:
		static void _bind_methods();
		TypedArray<Attribute> attributes;
		BuffPoolQueue *buff_pool_queue;
		bool autostart;
		bool paused;
		bool server_authoritative;

		void _on_attribute_changed(Ref<Attribute> p_attribute, const float p_previous_value, const float p_new_value);
		void _on_buff_dequeued(Ref<AttributeBuff> p_buff);
		void _on_buff_enqueued(Ref<AttributeBuff> p_buff);

		void bind_attribute(Ref<Attribute> p_attribute);
		bool has_attribute(Ref<Attribute> p_attribute);

	public:
		AttributeContainer();
		~AttributeContainer();

		void _ready() override;
		void add_attribute(Ref<Attribute> p_attribute);
		void apply_buff(Ref<AttributeBuff> p_buff);
		void pause();
		bool is_paused() const;
		void remove_attribute(Ref<Attribute> p_attribute);
		void remove_buff(Ref<AttributeBuff> p_buff);
		void resume();

		// getters/setters
		TypedArray<Attribute> get_attributes() const;
		bool get_autostart() const;
		bool get_server_authoritative() const;
		void set_attributes(const TypedArray<Attribute> p_attributes);
		void set_autostart(const bool p_autostart);
		void set_server_authoritative(const bool p_server_authoritative);
	};
} //namespace gga

#endif
