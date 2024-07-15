extends Node
# in this example, we will create a simple attribute that will represent the health of a character, which
# can be buffed or debuffed programmatically

const ATTRIBUTE_NAME = "health"


var attribute_container := AttributeContainer.new() 


func _ready():
	# create the attribute
	var health_attribute = Attribute.new()
	health_attribute.attribute_name = ATTRIBUTE_NAME
	health_attribute.initial_value = 100
	health_attribute.min_value = 0
	health_attribute.max_value = 100

	attribute_container.attribute_set = AttributeSet.new()
	attribute_container.attribute_set.add_attribute(health_attribute)

	add_child(attribute_container)

	# add a buff to the attribute
	var buff = AttributeBuff.new()
	buff.attribute_name = ATTRIBUTE_NAME
	buff.operation = AttributeOperation.add(10)
	
	var debuff = AttributeBuff.new()
	debuff.attribute_name = ATTRIBUTE_NAME
	debuff.operation = AttributeOperation.subtract(10)

	attribute_container.apply_buff(debuff)

	print(attribute_container.get_attribute_buffed_value_by_name(ATTRIBUTE_NAME)) # 90

	attribute_container.apply_buff(debuff)

	print(attribute_container.find_buffed_value(func (x): return x.attribute.attribute_name == ATTRIBUTE_NAME)) # 80

	attribute_container.apply_buff(buff)

	print(attribute_container.get_attribute_by_name(ATTRIBUTE_NAME).get_buffed_value()) # 90

	attribute_container.apply_buff(buff)

	print(attribute_container.get_attribute_by_name(ATTRIBUTE_NAME).get_buffed_value()) # 100

	attribute_container.apply_buff(buff)

	print(attribute_container.get_attribute_buffed_value_by_name(ATTRIBUTE_NAME)) # 100
