extends VBoxContainer


func _ready():
	var example_attribute = Attribute.new()
	example_attribute.attribute_name = "Attribute name"
	example_attribute.initial_value = 50.0
	example_attribute.max_value = 100.0
	example_attribute.min_value = 0.0
	
	example_attribute.operate(AttributeOperation.add(10.0))
	
	print(example_attribute.current_value)

	var buff000 = AttributeBuff.new()
	buff000.op = AttributeOperation.add(5.0)
	buff000.attribute_name = example_attribute.attribute_name
	
	example_attribute.add_buff(buff000)
	
	print(example_attribute.current_value)
