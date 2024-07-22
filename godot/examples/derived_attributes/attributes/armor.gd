class_name ArmorDerivedAttribute extends DerivedAttribute

const Constitution = preload("res://examples/derived_attributes/attributes/constitution.gd")
const Strength = preload("res://examples/derived_attributes/attributes/strength.gd")

func get_initial_value(attribute_container: AttributeContainer) -> float:
	var constitution = attribute_container.get_attribute_buffed_value_by_name(Constitution.ATTRIBUTE_NAME)
	var strength = attribute_container.get_attribute_buffed_value_by_name(Strength.ATTRIBUTE_NAME)
	return (constitution * 2) + strength
