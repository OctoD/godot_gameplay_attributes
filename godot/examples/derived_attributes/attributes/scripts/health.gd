class_name HealthDerivedAttribute extends Attribute


const ATTRIBUTE_NAME = "Health"


func _init(_attribute_name = ATTRIBUTE_NAME) -> void:
	attribute_name = _attribute_name


func _get_buffed_value(values: PackedFloat32Array) -> float:
	return values[0] * 6


func _derived_from(attribute_set: AttributeSet) -> Array[AttributeBase]:
	return [attribute_set.find_by_name(ConstitutionAttribute.ATTRIBUTE_NAME)]
	
	
func _get_initial_value(values: PackedFloat32Array) -> float:
	return values[0] * 6
	
