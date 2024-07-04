extends CharacterBody2D


signal projectile_fired(starting_position: Vector2, target_position: Vector2)


@export var enemies_container: Node2D

@onready var attribute_container: AttributeContainer = $AttributeContainer


var fire_rate: Attribute
var movement_speed: Attribute
var target: Node2D
var tick: float


func _ready() -> void:
	movement_speed = attribute_container.attribute_set.find_by_name("movement_speed")
	fire_rate = attribute_container.attribute_set.find_by_name("fire_rate")


func _physics_process(delta: float) -> void:
	var current_speed = movement_speed.current_value()
	var move_vector = Input.get_vector("move_left", "move_right", "move_up", "move_down").normalized()

	tick += delta
	velocity =  (move_vector * current_speed)
	
	if tick > fire_rate.current_value():
		tick = tick - fire_rate.current_value()
		fire_projectile()

	move_and_slide()


func fire_projectile() -> void:
	var next: Node2D = null
	
	if enemies_container:
		for child in enemies_container.get_children():
			if next == null:
				next = child
			elif (transform.origin - next.transform.origin) < child.transform.origin:
				next = child
	
	if next:
		projectile_fired.emit(global_position, next.global_position)
