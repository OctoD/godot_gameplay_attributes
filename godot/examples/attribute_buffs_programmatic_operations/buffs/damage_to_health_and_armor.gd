class_name DamageToHealthAndArmorBuff extends AttributeBuff


@export var damage_value: float = 1.0


func _applies_to(attribute_set: AttributeSet) -> Array[AttributeBase]:
	return [
		attribute_set.find_by_name("Health"),
		attribute_set.find_by_name("Armor")
	]


func _operate(values: Array[float]) -> Array[AttributeOperation]:
	if (values.size() == 0):
		return [AttributeOperation.add(0), AttributeOperation.add(0)]
	
	var armor_value = values[1];
	var diff = armor_value - damage_value
	var armor_operation: AttributeOperation
	var health_operation: AttributeOperation

	if diff <= 0.0:
		armor_operation = AttributeOperation.subtract(armor_value)
		health_operation = AttributeOperation.subtract(abs(diff))
	else:
		armor_operation = AttributeOperation.subtract(damage_value)
		health_operation = AttributeOperation.subtract(0)
	
	return [
		health_operation,
		armor_operation
	]
