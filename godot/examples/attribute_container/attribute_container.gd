extends VBoxContainer

const ATTRIBUTE_NAME = "attribute_000"

const ADD_1_PERCENT = preload("res://examples/attribute_container/buffs/add_1_percent.tres")
const ADD_1_PERCENT_FOR_5_SECONDS = preload("res://examples/attribute_container/buffs/add_1_percent_for_5_seconds.tres")
const ADD_5_FOR_5_SECONDS = preload("res://examples/attribute_container/buffs/add_5_for_5_seconds.tres")
const REMOVE_1_PERCENT = preload("res://examples/attribute_container/buffs/remove_1_percent.tres")
const REMOVE_1_PERCENT_FOR_5_SECONDS = preload("res://examples/attribute_container/buffs/remove_1_percent_for_5_seconds.tres")
const REMOVE_5_FOR_5_SECONDS = preload("res://examples/attribute_container/buffs/remove_5_for_5_seconds.tres")

const BUFFS = [
	ADD_1_PERCENT,
	ADD_1_PERCENT_FOR_5_SECONDS,
	ADD_5_FOR_5_SECONDS,
	REMOVE_1_PERCENT,
	REMOVE_1_PERCENT_FOR_5_SECONDS,
	REMOVE_5_FOR_5_SECONDS,
]

# Vars
@onready var attribute_container: AttributeContainer = $AttributeContainer
@onready var attribute_value_display: ProgressBar = $CenterContainer/VBoxContainer/AttributeValueDisplay
@onready var decrease_value: Button = %DecreaseValue
@onready var increase_value: Button = %IncreaseValue
@onready var buffs_selection: MenuButton = %BuffsSelection


func _on_attribute_buff_added(buff) -> void:
	print("_on_attribute_buff_added", buff)
	draw_attribute()


func _on_attribute_buff_dequeued(buff) -> void:
	print("_on_attribute_buff_dequeued", buff)
	draw_attribute()


func _on_attribute_buff_enqueued(buff) -> void:
	print("_on_attribute_buff_enqueued", buff)
	draw_attribute()


func _on_attribute_buff_removed(buff) -> void:
	print("attribute_buff_removed", buff)
	draw_attribute()


func _on_attribute_changed(attribute: Attribute, previous_value: float, new_value: float) -> void:
	print("_on_attribute_changed", attribute, previous_value, new_value)
	attribute_value_display.max_value = attribute.max_value
	attribute_value_display.min_value = attribute.min_value
	attribute_value_display.value = attribute.current_value()


func _ready():
	var popup = buffs_selection.get_popup()
	
	popup.id_pressed.connect(func (id: int) -> void:
		attribute_container.apply_buff(BUFFS[id])
	)
	
	for buff in BUFFS:
		popup.add_item(buff.buff_name)

	attribute_container.attribute_changed.connect(_on_attribute_changed)
	attribute_container.buff_applied.connect(_on_attribute_buff_added)
	attribute_container.buff_dequed.connect(_on_attribute_buff_dequeued)
	attribute_container.buff_enqued.connect(_on_attribute_buff_enqueued)
	attribute_container.buff_removed.connect(_on_attribute_buff_removed)

	for attr in attribute_container.attributes:
		attr.attribute_changed.connect(_on_attribute_changed)
	
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
	var attribute: Attribute
	
	for attr in attribute_container.attributes:
		if attr.attribute_name == ATTRIBUTE_NAME:
			attribute = attr
			break
	
	if attribute:
		attribute_value_display.max_value = attribute.max_value
		attribute_value_display.min_value = attribute.min_value
		attribute_value_display.value = attribute.current_value()
