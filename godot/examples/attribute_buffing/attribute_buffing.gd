extends Control

var ATTRIBUTE_000 = load("res://examples/attribute_buffing/attributes/attribute_000.tres")
var BUFF_5 = load("res://examples/attribute_buffing/buffs/buff_5.tres")
var BUFF_10 = load("res://examples/attribute_buffing/buffs/buff_10.tres")

@onready var add_buff_5 = %AddBuff5
@onready var add_buff_10 = %AddBuff10
@onready var remove_buff_5 = %RemoveBuff5
@onready var remove_buff_10 = %RemoveBuff10
@onready var current_value = %CurrentValue
@onready var applied_buffs = %AppliedBuffs

var attribute: Attribute


func _draw_buffs():
	for child in applied_buffs.get_children():
		child.queue_free()
		
	for buff in attribute.attribute_buffs:
		var label = Label.new()
		label.text = buff.buff_name
		applied_buffs.add_child(label)


func _draw_current_value():
	current_value.text = "current value is {0}".format({ 0: attribute.current_value })


func _ready():
	attribute = ATTRIBUTE_000.duplicate(true)
	attribute.attribute_changed.connect(func (x, y, z):
		_draw_current_value()
	)
	attribute.buff_added.connect(func (x):
		_draw_buffs()
		_draw_current_value()
	)
	attribute.buff_removed.connect(func (x):
		_draw_buffs()
		_draw_current_value()
	)

	attribute.setup_underlying_value()

	_draw_current_value()

	add_buff_5.pressed.connect(func ():
		attribute.add_buff(BUFF_5)
	)
	
	add_buff_10.pressed.connect(func ():
		attribute.add_buff(BUFF_10)	
	)
	
	remove_buff_5.pressed.connect(func ():
		attribute.remove_buff(BUFF_5)	
	)
	
	remove_buff_10.pressed.connect(func ():
		attribute.remove_buff(BUFF_10)	
	)
