extends CharacterBody2D


signal projectile_fired(starting_position: Vector2, target_position: Vector2)


@onready var attribute_container: AttributeContainer = $AttributeContainer
@onready var progress_bar: ProgressBar = $ProgressBar


var health: Attribute
var fire_rate: Attribute
var movement_speed: Attribute
var target: Node2D
var tick: float


func _ready() -> void:
	health = attribute_container.attribute_set.find_by_name("health")
	movement_speed = attribute_container.attribute_set.find_by_name("movement_speed")
	fire_rate = attribute_container.attribute_set.find_by_name("fire_rate")

	progress_bar.max_value = health.max_value
	progress_bar.min_value = health.min_value
	progress_bar.value = health.current_value()
	
	health.attribute_changed.connect(func (_attribute, _old_value, _new_value):
		progress_bar.value = _new_value
	)


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
	
	for child in get_tree().get_nodes_in_group("mobs"):
		if next == null:
			next = child
		elif child.global_position.distance_to(global_position) < next.global_position.distance_to(global_position):
			next = child
	
	if next:
		projectile_fired.emit(global_position, next.global_position)
