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
	enum BuffType
	{
		/// @brief One-shot buff. This buff will be removed from the container immediately after it is processed and will affect the attribute directly.
		BT_ONESHOT = 0,
		/// @brief Stackable buff.
		BT_STACKABLE = 1,
	};

	enum OperationType
	{
		/// @brief Add operation.
		OP_ADD = 0,
		/// @brief Divide operation.
		OP_DIVIDE = 1,
		/// @brief Multiply operation.
		OP_MULTIPLY = 2,
		/// @brief Percentage operation.
		OP_PERCENTAGE = 3,
		/// @brief Subtract operation.
		OP_SUBTRACT = 4,
	};

	/// @brief Attribute operation.
	class AttributeOperation : public Resource
	{
		GDCLASS(AttributeOperation, Resource);

	protected:
		/// @brief Bind methods to Godot.
		static void _bind_methods();
		/// @brief Create an attribute operation.
		/// @param p_operand The OperationType instance.
		/// @param p_value The operation value.
		/// @return A new instance of AttributeOperation as a Ref.
		static Ref<AttributeOperation> create(const OperationType p_operand, const float p_value);
		
		/// @brief Operand.
		OperationType operand;
		/// @brief Value.
		float value;

	public:
		bool operator==(const Ref<AttributeOperation> &buff) const;

		AttributeOperation();
		~AttributeOperation();

		/// @brief Returns a new instance of AttributeOperation with the add operation.
		/// @param p_value The value to add.
		/// @return The new instance of AttributeOperation.
		static Ref<AttributeOperation> add(const float p_value);
		/// @brief Returns a new instance of AttributeOperation with the divide operation.
		/// @param p_value The value to divide.
		/// @return The new instance of AttributeOperation.
		static Ref<AttributeOperation> divide(const float p_value);
		/// @brief Returns a new instance of AttributeOperation with the multiply operation.
		/// @param p_value The value to multiply.
		/// @return The new instance of AttributeOperation.
		static Ref<AttributeOperation> multiply(const float p_value);
		/// @brief Returns a new instance of AttributeOperation with the percentage operation.
		/// @param p_value The percentage value.
		/// @return The new instance of AttributeOperation.
		static Ref<AttributeOperation> percentage(const float p_value);
		/// @brief Returns a new instance of AttributeOperation with the subtract operation.
		/// @param p_value The value to subtract.
		/// @return The new instance of AttributeOperation.
		static Ref<AttributeOperation> subtract(const float p_value);

		/// @brief Get the operand.
		/// @return The operand.
		int get_operand() const;
		/// @brief Get the value.
		/// @return The value.
		float get_value() const;
		/// @brief Operate on a base value.
		/// @param p_base_value The base value to operate on.
		float operate(float p_base_value) const;
		/// @brief Set the operand.
		void set_operand(const int p_value);
		/// @brief Set the value.
		void set_value(const float p_value);
	};

	/// @brief Attribute buff.
	class AttributeBuff : public Resource
	{
		GDCLASS(AttributeBuff, Resource);

	protected:
		/// @brief Bind methods to Godot.
		static void _bind_methods();

		/// @brief The affected attribute name.
		String attribute_name;
		/// @brief The buff name.
		String buff_name;
		/// @brief The buff type.
		int buff_type;
		/// @brief The buff duration.
		float duration;
		/// @brief The operation to apply.
		Ref<AttributeOperation> operation;

	public:
		// equal operator overload
		bool operator==(const Ref<AttributeBuff> &buff) const;

		AttributeBuff();
		AttributeBuff(
			const String &p_attribute_name, 
			const String &p_buff_name, 
			const int p_buff_type, 
			const float p_duration, 
			const Ref<AttributeOperation> &p_operation
		);
		~AttributeBuff();

		/// @brief Returns the result of the operation on the base value.
		/// @param base_value The base value to operate on. It's the attribute underlying value.
		/// @return The result of the operation.
		float operate(float base_value) const;
		/// @brief Returns the affected attribute name.
		/// @return The affected attribute name.
		String get_attribute_name() const;
		/// @brief Returns the buff name.
		/// @return The buff name.
		String get_buff_name() const;
		/// @brief Returns the buff type.
		/// @return The buff type.
		int get_buff_type() const;
		/// @brief Returns the buff duration.
		/// @return The buff duration.
		float get_duration() const;
		/// @brief Returns the operation to apply as a Ref.
		/// @return The operation to apply.
		Ref<AttributeOperation> get_operation() const;
		/// @brief Sets the affected attribute name.
		/// @param p_value The affected attribute name.
		void set_attribute_name(const String &p_value);
		/// @brief Sets the buff name.
		/// @param p_value The buff name.
		void set_buff_name(const String &p_value);
		/// @brief Sets the buff type.
		/// @param p_value The buff type.
		void set_buff_type(const int p_value);
		/// @brief Sets the buff duration.
		/// @param p_value The buff duration.
		void set_duration(const float p_value);
		/// @brief Sets the operation to apply.
		/// @param p_value The operation to apply.
		void set_operation(const Ref<AttributeOperation> &p_value);
	};

	/// @brief Attribute.
	class Attribute : public Resource
	{
		GDCLASS(Attribute, Resource);

	protected:
		/// @brief Bind methods to Godot.
		static void _bind_methods();
		/// @brief The buffs affecting the attribute.
		TypedArray<AttributeBuff> buffs;
		/// @brief The attribute name.
		String attribute_name;
		/// @brief The initial value of the attribute.
		float initial_value;
		/// @brief The maximum value of the attribute.
		float max_value;
		/// @brief The minimum value of the attribute.
		float min_value;
		/// @brief The underlying value of the attribute.
		float underlying_value;

	public:
		/// @brief Create an attribute from some parameters.
		/// @param p_attribute_name The attribute name.
		/// @param p_initial_value The initial value.
		/// @param p_min_value The minimum value.
		/// @param p_max_value The maximum value.
		/// @return The new instance of Attribute.
		static Ref<Attribute> create(const String &p_attribute_name, const float p_initial_value, const float p_min_value, const float p_max_value);

		/// @brief Add a buff to the attribute.
		/// @param p_buff The buff to add.
		/// @return True if the buff was added, false otherwise.
		bool add_buff(const Ref<AttributeBuff> &p_buff);
		/// @brief Add buffs to the attribute.
		/// @param p_buffs The buffs to add.
		/// @return The number of buffs added.
		uint16_t add_buffs(const TypedArray<AttributeBuff> &p_buffs);
		/// @brief Check if the attribute can receive a buff.
		/// @param p_buff The buff to check.
		/// @return True if the attribute can receive the buff, false otherwise.
		bool can_receive_buff(const Ref<AttributeBuff> &p_buff) const;
		/// @brief Clear all buffs from the attribute.
		void clear_buffs();
		/// @brief Get the current value of the attribute including buffs modifiers.
		/// @return The current value.
		float current_value() const;
		/// @brief Get the current value of the attribute as a percentage.
		/// @return The current value as a percentage.
		bool has_buff(const Ref<AttributeBuff> &p_buff) const;
		/// @brief Remove a buff from the attribute.
		/// @param p_buff The buff to remove.
		/// @return True if the buff was removed, false otherwise.
		bool remove_buff(const Ref<AttributeBuff> &p_buff);
		/// @brief Remove buffs from the attribute.
		/// @param p_buffs The buffs to remove.
		/// @return The number of buffs removed.
		uint16_t remove_buffs(const TypedArray<AttributeBuff> &p_buffs);
		/// @brief Setup the attribute.
		void setup();

		// getters/setters
		/// @brief Get the attribute name.
		/// @return The attribute name.
		String get_attribute_name() const;
		/// @brief Get the buffs affecting the attribute.
		/// @return The buffs affecting the attribute.
		TypedArray<AttributeBuff> get_buffs() const;
		/// @brief Get the initial value of the attribute.
		/// @return The initial value of the attribute.
		float get_initial_value() const;
		/// @brief Get the maximum value of the attribute.
		/// @return The maximum value of the attribute.
		float get_max_value() const;
		/// @brief Get the minimum value of the attribute.
		/// @return The minimum value of the attribute.
		float get_min_value() const;
		/// @brief Set the attribute name.
		/// @param p_value The attribute name.
		void set_attribute_name(const String &p_value);
		/// @brief Set the buffs affecting the attribute.
		/// @param p_buffs The buffs affecting the attribute.
		void set_buffs(const TypedArray<AttributeBuff> &p_buffs);
		/// @brief Set the initial value of the attribute.
		/// @param p_value The initial value of the attribute.
		void set_initial_value(const float p_value);
		/// @brief Set the maximum value of the attribute.
		/// @param p_value The maximum value of the attribute.
		void set_max_value(const float p_value);
		/// @brief Set the minimum value of the attribute.
		/// @param p_value The minimum value of the attribute.
		void set_min_value(const float p_value);
	};
} //namespace gga

VARIANT_ENUM_CAST(gga::BuffType);
VARIANT_ENUM_CAST(gga::OperationType);

#endif
