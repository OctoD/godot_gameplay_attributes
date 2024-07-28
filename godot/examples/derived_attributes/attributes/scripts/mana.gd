class_name ManaDerivedAttribute extends DerivedAttribute


func _get_initial_value(attribute_set: AttributeSet) -> float:
	return attribute_set.find_by_name(IntelligenceAttribute.ATTRIBUTE_NAME).initial_value * 5.0


func _get_max_value(attribute_set: AttributeSet) -> float:
	return _get_initial_value(attribute_set)


func _get_min_value(attribute_set: AttributeSet) -> float:
	return 0.0
