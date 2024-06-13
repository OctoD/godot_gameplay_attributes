extends VBoxContainer

# note, if handled manually, the initial value of the attribute will be always 0.
# it is set privately by the AttributesContainer node when ready
# if you need to setup the attribute, call the [method Attribute.setup_underlying_value] method

func _ready():
	var example_attribute = Attribute.new()
	example_attribute.attribute_name = "Attribute name"
	example_attribute.initial_value = 50.0
	example_attribute.max_value = 100.0
	example_attribute.min_value = 0.0
	example_attribute.setup_underlying_value()
	
	print(example_attribute.current_value)

	var buff000 = AttributeBuff.new()
	buff000.operation = AttributeOperation.add(5.0)
	buff000.attribute_name = example_attribute.attribute_name
	buff000.buff_type = AttributeBuff.STACKABLE
	
	example_attribute.add_buff(buff000)
	
	print(example_attribute.current_value)

	example_attribute.remove_buff(buff000)
	
	print(example_attribute.current_value)

	var buff001 = AttributeBuff.new()
	buff001.operation = AttributeOperation.add(5.0)
	buff001.attribute_name = example_attribute.attribute_name
	buff001.buff_type = AttributeBuff.ONE_SHOT
	
	example_attribute.add_buff(buff001)
	
	print(example_attribute.has_buff(buff001))
	print(example_attribute.current_value)
	
	example_attribute.remove_buff(buff001) # this will do nothing since the buff is already gone
	print(example_attribute.current_value)
	
