/**************************************************************************/
/*  attribute.hpp                                                         */
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

#ifndef GODOT_GAMEPLAY_ATTRIBUTES_ATTRIBUTE_HPP
#define GODOT_GAMEPLAY_ATTRIBUTES_ATTRIBUTE_HPP

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

namespace gga
{
	enum BuffType : uint8_t
	{
		BT_ONESHOT = 0,
		BT_STACKABLE = 1,
	};

	enum OperationType : uint8_t
	{
		OP_ADD = 0,
		OP_DIVIDE = 1,
		OP_MULTIPLY = 2,
		OP_PERCENTAGE = 3,
		OP_SUBTRACT = 4,
	};

	class AttributeOperation : public Resource
	{
		GDCLASS(AttributeOperation, Resource);

	protected:
		static void _bind_methods();
		static Ref<AttributeOperation> create(const OperationType p_operand, const float p_value);

		OperationType operand;
		float value;

	public:
		bool operator==(const Ref<AttributeOperation> &buff) const;
		static Ref<AttributeOperation> add(const float p_value);
		static Ref<AttributeOperation> divide(const float p_value);
		static Ref<AttributeOperation> multiply(const float p_value);
		static Ref<AttributeOperation> percentage(const float p_value);
		static Ref<AttributeOperation> subtract(const float p_value);

		int get_operand() const;
		float get_value() const;
		float operate(float p_base_value) const;
		void set_operand(const int p_value);
		void set_value(const float p_value);
	};

	class AttributeBuff : public Resource
	{
		GDCLASS(AttributeBuff, Resource);

	protected:
		static void _bind_methods();

		String attribute_name;
		String buff_name;
		int buff_type;
		float duration;
		AttributeOperation *operation;

	public:
		// equal operator overload
		bool operator==(const Ref<AttributeBuff> &buff) const;

		float operate(float base_value) const;

		String get_attribute_name() const;
		String get_buff_name() const;
		int get_buff_type() const;
		float get_duration() const;
		AttributeOperation *get_operation() const;
		void set_attribute_name(const String &p_value);
		void set_buff_name(const String &p_value);
		void set_buff_type(const int p_value);
		void set_duration(const float p_value);
		void set_operation(const Ref<AttributeOperation> &p_value);
	};

	class Attribute : public Resource
	{
		GDCLASS(Attribute, Resource);

	protected:
		static void _bind_methods();

		TypedArray<AttributeBuff> buffs;
		String attribute_name;
		float initial_value;
		float max_value;
		float min_value;
		float underlying_value;

	public:
		bool add_buff(const Ref<AttributeBuff> &p_buff);
		uint16_t add_buffs(const TypedArray<AttributeBuff> &p_buffs);
		bool can_receive_buff(const Ref<AttributeBuff> &p_buff) const;
		void clear_buffs();
		float current_value() const;
		bool has_buff(const Ref<AttributeBuff> &p_buff) const;
		bool remove_buff(const Ref<AttributeBuff> &p_buff);
		uint16_t remove_buffs(const TypedArray<AttributeBuff> &p_buffs);
		void setup();

		// getters/setters
		String get_attribute_name() const;
		float get_initial_value() const;
		float get_max_value() const;
		float get_min_value() const;
		void set_attribute_name(const String &p_value);
		void set_initial_value(const float p_value);
		void set_max_value(const float p_value);
		void set_min_value(const float p_value);
	};
} //namespace gga

VARIANT_ENUM_CAST(gga::BuffType);
VARIANT_ENUM_CAST(gga::OperationType);

#endif
