class_name ManaDerivedAttribute extends DerivedAttribute

const Intelligence = preload("res://examples/derived_attributes/attributes/intelligence.gd")

func get_initial_value(attribute_container: AttributeContainer) -> float:
	return attribute_container.get_attribute_buffed_value_by_name(Intelligence.ATTRIBUTE_NAME) * 5


func get_max_value(attribute_container: AttributeContainer) -> float:
	return get_initial_value(attribute_container) # it will be always capped at the initial value


func get_min_value() -> float:
	return 0.0
