class_name MeleeDamageDerivedAttribute extends Attribute


const ATTRIBUTE_NAME = "MeleeDamage"


func _init(_attribute_name = ATTRIBUTE_NAME) -> void:
	attribute_name = _attribute_name
	

func _get_initial_value(attribute_set: AttributeSet) -> float:
	return 1 + attribute_set.find_by_name(StrengthAttribute.ATTRIBUTE_NAME).initial_value * 1.5
