extends VBoxContainer

# note, if handled manually, the initial value of the attribute will be always 0.
# it is set privately by the AttributesContainer node when ready
# if you need to setup the attribute, call the [method Attribute.setup_underlying_value] method

func _on_attribute_changed(attribute: Attribute, previous_value: float, new_value: float) -> void:
	print("attribute changed: ", attribute.attribute_name, " previous value was: ", previous_value, " new value is: ", new_value)


func _on_buff_added(attribute_buff: AttributeBuff) -> void:
	print("attribute buff added on: ", attribute_buff.attribute_name, " the buff is called: ", attribute_buff.buff_name)


func _on_buff_removed(attribute_buff: AttributeBuff) -> void:
	print("attribute buff removed from: ", attribute_buff.attribute_name, " the buff is called: ", attribute_buff.buff_name)


func _ready():
	var example_attribute = Attribute.new()
	example_attribute.attribute_changed.connect(self._on_attribute_changed)
	example_attribute.buff_added.connect(self._on_buff_added)
	example_attribute.buff_removed.connect(self._on_buff_removed)
	example_attribute.attribute_name = "the_wonderful_attribute_®️"
	example_attribute.initial_value = 50.0
	example_attribute.max_value = 100.0
	example_attribute.min_value = 0.0
	example_attribute.setup_underlying_value()
	
	print(example_attribute.current_value)

	var buff000 = AttributeBuff.new()
	buff000.operation = AttributeOperation.add(5.0)
	buff000.attribute_name = example_attribute.attribute_name
	buff000.buff_name = "the_buff_000!"
	buff000.buff_type = AttributeBuff.STACKABLE
	
	example_attribute.add_buff(buff000)
	
	print(example_attribute.current_value)

	example_attribute.remove_buff(buff000)
	
	print(example_attribute.current_value)

	var buff001 = AttributeBuff.new()
	buff001.operation = AttributeOperation.add(5.0)
	buff001.attribute_name = example_attribute.attribute_name
	buff001.buff_name = "the_unnamed_buff_001?"
	buff001.buff_type = AttributeBuff.ONE_SHOT
	
	example_attribute.add_buff(buff001)

	print(example_attribute.has_buff(buff001))
	print(example_attribute.current_value)
	
	example_attribute.remove_buff(buff001) # this will do nothing since the buff is already gone
	print(example_attribute.current_value)
	
