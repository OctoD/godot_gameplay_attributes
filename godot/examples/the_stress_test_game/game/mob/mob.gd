extends CharacterBody2D



signal died(global_position: Vector2)


@export var chase_target: Node2D
@export var mob_type: MobType


@onready var sprite_2d: Sprite2D = $Sprite2D
@onready var attribute_container: AttributeContainer = $AttributeContainer


var latest_position := Vector2.ZERO
var health: Attribute
var movement_speed: Attribute


func check_if_dead() -> void:
	if health and health.current_value() <= 0.0:
		died.emit(global_position)
		queue_free()


func _ready() -> void:
	add_to_group("mobs")
	
	attribute_container.attribute_set = mob_type.attribute_set
	sprite_2d.texture = mob_type.texture_2d

	health = mob_type.attribute_set.find_by_name("health")
	movement_speed = mob_type.attribute_set.find_by_name("movement_speed")

	attribute_container.setup()
	
	attribute_container.attribute_changed.connect(func (_attribute, _old_value, _new_value): 
		check_if_dead()
	)
	
	attribute_container.buff_applied.connect(func (_buff):
		check_if_dead()
	)
	
func _process(_d: float) -> void:
	if chase_target:
		latest_position = chase_target.transform.origin


func _physics_process(_d: float) -> void:
	if movement_speed and chase_target:
		velocity = (latest_position - transform.origin).normalized() * movement_speed.current_value()
	move_and_slide()
