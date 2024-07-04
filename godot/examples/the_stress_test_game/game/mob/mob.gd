extends CharacterBody2D

@export var chase_target: Node2D


@onready var attribute_container: AttributeContainer = $AttributeContainer


var latest_position := Vector2.ZERO
var health: Attribute
var movement_speed: Attribute


func check_if_dead() -> void:
	if health.current_value() <= 0.0:
		queue_free()


func _ready() -> void:
	add_to_group("mobs")
	
	health = attribute_container.attribute_set.find_by_name("health")
	movement_speed = attribute_container.attribute_set.find_by_name("movement_speed")

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
