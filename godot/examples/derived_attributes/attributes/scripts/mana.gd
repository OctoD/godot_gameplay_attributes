class_name ManaDerivedAttribute extends Attribute


const ATTRIBUTE_NAME = "Mana"


func _init(_attribute_name = ATTRIBUTE_NAME) -> void:
	attribute_name = _attribute_name
	

func _get_initial_value(attribute_set: AttributeSet) -> float:
	return attribute_set.find_by_name(IntelligenceAttribute.ATTRIBUTE_NAME).initial_value * 5.0


func _get_max_value(attribute_set: AttributeSet) -> float:
	return _get_initial_value(attribute_set)


func _get_min_value(_attribute_set: AttributeSet) -> float:
	return 0.0
