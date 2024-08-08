class_name HealthDerivedAttribute extends Attribute


const ATTRIBUTE_NAME = "Health"


func _init(_attribute_name = ATTRIBUTE_NAME) -> void:
	attribute_name = _attribute_name
	
	
func _get_initial_value(attribute_set: AttributeSet) -> float:
	var constitution = attribute_set.find_by_name(ConstitutionAttribute.ATTRIBUTE_NAME)
	return constitution.initial_value * 6
