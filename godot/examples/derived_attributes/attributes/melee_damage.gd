class_name MeleeDamageDerivedAttribute extends DerivedAttribute

const Strength = preload("res://examples/derived_attributes/attributes/strength.gd")

func get_initial_value(attribute_container: AttributeContainer) -> float:
	return 1 + attribute_container.get_attribute_buffed_value_by_name(Strength.ATTRIBUTE_NAME) * 1.5
