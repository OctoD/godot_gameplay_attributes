class_name ManaDerivedAttribute extends Attribute


const ATTRIBUTE_NAME = "Mana"


func _init(_attribute_name = ATTRIBUTE_NAME) -> void:
	attribute_name = _attribute_name


func _derived_from(attribute_set: AttributeSet) -> Array[AttributeBase]:
	return [attribute_set.find_by_name(IntelligenceAttribute.ATTRIBUTE_NAME)]


func _get_buffed_value(values: PackedFloat32Array) -> float:
	return values[0] * 5.0
	

func _get_initial_value(values: PackedFloat32Array) -> float:
	return values[0] * 5.0
