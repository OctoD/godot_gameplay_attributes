class_name HealthDerivedAttribute extends DerivedAttribute

func _get_initial_value(attribute_set: AttributeSet) -> float:
	var constitution = attribute_set.find_by_name(ConstitutionAttribute.ATTRIBUTE_NAME)
	return constitution * 6
