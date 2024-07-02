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
		/// @brief Stackable buff.
		BT_STACKABLE = 0,
		/// @brief Unique buff.
		BT_UNIQUE = 1,
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
				const Ref<AttributeOperation> &p_operation);
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

	class AttributeSet : public Resource
	{
		GDCLASS(AttributeSet, Resource);

	protected:
		/// @brief Bind methods to Godot.
		static void _bind_methods();
		/// @brief The attributes in the set.
		TypedArray<Attribute> attributes;
		/// @brief The set name.
		String set_name;
		/// @brief Sort the attributes by name.
		/// @param p_a The first attribute.
		/// @param p_b The second attribute.
		/// @return The comparison result.
		int sort_attributes_by_name(const Ref<Attribute> &p_a, const Ref<Attribute> &p_b) const;
		/// @brief Sort the attributes in the set.
		void sort_attributes();

	public:
		/// @brief Equal operator overload.
		/// @param set The AttributeSet to compare.
		/// @return True if the AttributeSet is equal, false otherwise.
		bool operator==(const Ref<AttributeSet> &set) const;

		/// @brief Create an attribute set.
		AttributeSet();
		/// @brief Create an attribute set.
		/// @param p_attributes The attributes in the set.
		/// @param p_set_name The set name.
		AttributeSet(TypedArray<Attribute> p_attributes, String p_set_name);

		/// @brief Add an attribute to the set.
		/// @param p_attribute The attribute to add.
		/// @return True if the attribute was added, false otherwise.
		bool add_attribute(const Ref<Attribute> &p_attribute);
		/// @brief Add attributes to the set.
		/// @param p_attributes The attributes to add.
		/// @return The number of attributes added.
		uint16_t add_attributes(const TypedArray<Attribute> &p_attributes);
		/// @brief Finds the index of an attribute in the set.
		/// @param p_attribute The attribute to find.
		/// @return The index of the attribute.
		int find(const Ref<Attribute> &p_attribute) const;
		/// @brief Gets all the attributes names in the set.
		/// @return The attributes names.
		PackedStringArray get_attributes_names() const;
		/// @brief Get the attributes in the set.
		/// @return The attributes.
		TypedArray<Attribute> get_attributes() const;
		/// @brief Get an attribute from the set.
		/// @param index The index of the attribute.
		/// @return The attribute.
		Ref<Attribute> get_at(int index) const;
		/// @brief Get the set name. I dunno if it gets or sets but the pun is intended.
		/// @return The set name.
		String get_set_name() const;
		/// @brief Check if the set has an attribute.
		/// @param p_attribute The attribute to check.
		/// @return True if the set has the attribute, false otherwise.
		bool has_attribute(const Ref<Attribute> &p_attribute) const;
		/// @brief Remove an attribute from the set.
		/// @param p_attribute The attribute to remove.
		/// @return True if the attribute was removed, false otherwise.
		bool remove_attribute(const Ref<Attribute> &p_attribute);
		/// @brief Remove attributes from the set.
		/// @param p_attributes The attributes to remove.
		/// @return The number of attributes removed.
		uint16_t remove_attributes(const TypedArray<Attribute> &p_attributes);
		/// @brief Push an attribute to the set.
		/// @param p_attribute The attribute to push.
		void push_back(const Ref<Attribute> &p_attribute);
		/// @brief Set the attributes in the set.
		/// @param p_attributes The attributes in the set.
		void set_attributes(const TypedArray<Attribute> &p_attributes);
		/// @brief Set the set name. Sorry for the pun.
		/// @param p_value The set name.
		void set_set_name(const String &p_value);
		/// @brief Get the number of attributes in the set.
		/// @return The number of attributes.
		int count() const;
	};

	class AttributesTable : public Resource
	{
		GDCLASS(AttributesTable, Resource);

	protected:
		static void _bind_methods();
		/// @brief The attribute sets in the table.
		TypedArray<AttributeSet> attribute_sets;

	public:
		AttributesTable();
		AttributesTable(TypedArray<AttributeSet> p_attribute_sets);

		/// @brief Add an attribute set to the table.
		/// @param p_attribute_set The attribute set to add.
		void add_attribute_set(const Ref<AttributeSet> &p_attribute_set);
		/// @brief Get the attribute sets in the table.
		/// @return The attribute sets.
		TypedArray<AttributeSet> get_attribute_sets() const;
		/// @brief Get the attribute names in the table.
		/// @return The attribute names.
		PackedStringArray get_attribute_names() const;
		/// @brief Check if the table has an attribute set.
		/// @param p_attribute_set The attribute set to check.
		bool has_attribute_set(const Ref<AttributeSet> &p_attribute_set) const;
		/// @brief Remove an attribute set from the table.
		/// @param p_attribute_set The attribute set to remove.
		void remove_attribute_set(const Ref<AttributeSet> &p_attribute_set);
		/// @brief Remove an attribute set from the table.
		/// @param p_attribute_set The attribute set to remove.
		void set_attribute_sets(const TypedArray<AttributeSet> &p_attribute_sets);
	};
} //namespace gga

VARIANT_ENUM_CAST(gga::BuffType);
VARIANT_ENUM_CAST(gga::OperationType);

#endif
