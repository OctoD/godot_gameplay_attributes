class_name HealthDerivedAttribute extends DerivedAttribute

const Constitution = preload("res://examples/derived_attributes/attributes/constitution.gd")

func get_initial_value(attribute_container: AttributeContainer) -> float:
	var constitution = attribute_container.get_attribute_buffed_value_by_name(Constitution.ATTRIBUTE_NAME)
	return constitution * 6
