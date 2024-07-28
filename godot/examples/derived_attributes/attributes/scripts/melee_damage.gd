class_name MeleeDamageDerivedAttribute extends DerivedAttribute


func _get_initial_value(attribute_set: AttributeSet) -> float:
	return 1 + attribute_set.find_by_name(StrengthAttribute.ATTRIBUTE_NAME).initial_value * 1.5
