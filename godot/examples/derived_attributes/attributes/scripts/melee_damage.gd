class_name MeleeDamageDerivedAttribute extends Attribute


const ATTRIBUTE_NAME = "MeleeDamage"


func _init(_attribute_name = ATTRIBUTE_NAME) -> void:
	attribute_name = _attribute_name
	
	
func _derived_from(attribute_set: AttributeSet) -> Array[AttributeBase]:
	return [attribute_set.find_by_name(StrengthAttribute.ATTRIBUTE_NAME)]


func _get_buffed_value(values: PackedFloat32Array) -> float:
	return 1 + values[0] * 1.5


func _get_initial_value(values: PackedFloat32Array) -> float:
	return 1 + values[0] * 1.5
