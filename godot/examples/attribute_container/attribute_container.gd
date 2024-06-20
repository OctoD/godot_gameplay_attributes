extends VBoxContainer

const ATTRIBUTE_NAME = "attribute_000"

@onready var attribute_container: AttributeContainer = $AttributeContainer
@onready var attribute_value_display: ProgressBar = $CenterContainer/VBoxContainer/AttributeValueDisplay
@onready var decrease_value: Button = %DecreaseValue
@onready var increase_value: Button = %IncreaseValue

func _on_attribute_buff_added(buff) -> void:
	print("_on_attribute_buff_added", buff)


func _on_attribute_buff_dequeued(buff) -> void:
	print("_on_attribute_buff_dequeued", buff)


func _on_attribute_buff_enqueued(buff) -> void:
	print("_on_attribute_buff_enqueued", buff)


func _on_attribute_buff_removed(buff) -> void:
	print("attribute_buff_removed", buff)


func _on_attribute_changed(attribute, previous_value, new_value) -> void:
	print("_on_attribute_changed", attribute, previous_value, new_value)


func _ready():
	attribute_container.attribute_buff_added.connect(_on_attribute_buff_added)
	attribute_container.attribute_buff_dequeued.connect(_on_attribute_buff_dequeued)
	attribute_container.attribute_buff_enqueued.connect(_on_attribute_buff_enqueued)
	attribute_container.attribute_buff_removed.connect(_on_attribute_buff_removed)
	attribute_container.attribute_changed.connect(_on_attribute_changed)
	
	decrease_value.pressed.connect(func ():
		attribute_container.apply_buff(make_debuff(1.0))	
	)
	
	increase_value.pressed.connect(func ():
		attribute_container.apply_buff(make_buff(1.0))	
	)
	
	draw_attribute()


func make_debuff(value: float) -> AttributeBuff:
	var buff = AttributeBuff.new()

	buff.attribute_name = ATTRIBUTE_NAME
	buff.operation = AttributeOperation.subtract(value)

	return buff

func make_buff(value: float) -> AttributeBuff:
	var buff = AttributeBuff.new()

	buff.attribute_name = ATTRIBUTE_NAME
	buff.operation = AttributeOperation.add(value)

	return buff


func draw_attribute() -> void:
	var attribute = attribute_container.find_attribute_by_name(ATTRIBUTE_NAME)
	
	if attribute:
		attribute_value_display.max_value = attribute.max_value
		attribute_value_display.min_value = attribute.min_value
		attribute_value_display.value = attribute.current_value
