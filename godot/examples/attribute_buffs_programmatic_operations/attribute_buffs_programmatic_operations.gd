extends VBoxContainer

@onready var health: Label = %Health
@onready var armor: Label = %Armor
@onready var damage: Button = %Damage
@onready var attribute_container: AttributeContainer = $AttributeContainer


const HEALTH_NAME = "Health"
const ARMOR_NAME = "Armor"


func _draw_attributes() -> void:
	var health_value = attribute_container.get_attribute_buffed_value_by_name(HEALTH_NAME)
	var armor_value = attribute_container.get_attribute_buffed_value_by_name(ARMOR_NAME)

	health.text = "Health: " + str(health_value)
	armor.text = "Armor: " + str(armor_value)


func _ready() -> void:
	attribute_container.attribute_changed.connect(func (_attribute, _oldvalue, _newvalue):
		_draw_attributes()
	)

	damage.pressed.connect(func ():
		var d = DamageToHealthAndArmorBuff.new()
		d.damage_value = 5
		attribute_container.apply_buff(d)
	)
	
	_draw_attributes()
	
