extends VBoxContainer

const ATTRIBUTE_NAME = "awawawaawawawawaw"

@onready var attribute_container: AttributeContainer = $AttributeContainer

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var attribute = Attribute.new()
	attribute.attribute_name = ATTRIBUTE_NAME
	attribute.max_value = 100
	attribute.min_value = 0
	attribute.initial_value = 50
	
	attribute_container.add_attribute(attribute)
	
	var buff = AttributeBuff.new()
	buff.attribute_name = ATTRIBUTE_NAME
	buff.operation = AttributeOperation.add(5)
	buff.max_applies = 5

	print_value()
	attribute_container.apply_buff(buff)
	print_value()
	attribute_container.apply_buff(buff)
	print_value()
	attribute_container.apply_buff(buff)
	print_value()
	attribute_container.apply_buff(buff)
	print_value()
	attribute_container.apply_buff(buff)
	print_value()
	attribute_container.apply_buff(buff)
	print_value()
	attribute_container.apply_buff(buff)
	print_value()
	attribute_container.apply_buff(buff)
	print_value()


func print_value() -> void:
	print(attribute_container.get_attribute_buffed_value_by_name(ATTRIBUTE_NAME))
