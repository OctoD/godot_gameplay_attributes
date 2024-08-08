class_name ArmorDerivedAttribute extends Attribute


const ATTRIBUTE_NAME = "Armor"


func _init(_attribute_name = ATTRIBUTE_NAME) -> void:
	attribute_name = _attribute_name


func _get_initial_value(attribute_set: AttributeSet) -> float:
	var constitution = attribute_set.find_by_name(ConstitutionAttribute.ATTRIBUTE_NAME).initial_value
	var strength = attribute_set.find_by_name(StrengthAttribute.ATTRIBUTE_NAME).initial_value
	return (constitution * 2) + strength
