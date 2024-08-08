extends HBoxContainer


signal decreased()
signal increased()

@onready var attribute_name: Label = %AttributeName
@onready var decrement: Button = %Decrement
@onready var value_display: Label = %ValueDisplay
@onready var increment: Button = %Increment


var bound_attribute: RuntimeAttribute


func _ready() -> void:
	decrement.pressed.connect(func (): decreased.emit())
	increment.pressed.connect(func (): increased.emit())
	


func bind_runtime_attribute(runtime_attribute: RuntimeAttribute) -> void:
	bound_attribute = runtime_attribute
	attribute_name.text = runtime_attribute.attribute.attribute_name
	value_display.text = str(int(runtime_attribute.get_buffed_value()))
	runtime_attribute.attribute_changed.connect(draw)
	
	
func draw(attribute: RuntimeAttribute, _old_value: float, new_value: float) -> void:
	attribute_name.text = attribute.attribute.attribute_name
	value_display.text = str(int(new_value))
	
	
func set_readonly(value: bool) -> void:
	decrement.visible = !value
	increment.visible = !value
