class_name Player extends CharacterBody2D


signal died()
signal projectile_fired(starting_position: Vector2, target_position: Vector2)


@onready var attribute_container: AttributeContainer = $AttributeContainer
@onready var progress_bar: ProgressBar = $ProgressBar
@onready var camera_2d: Camera2D = $Camera2D


var health: RuntimeAttribute
var fire_rate: RuntimeAttribute
var is_dead: bool = false
var movement_speed: RuntimeAttribute
var pickup_radius: RuntimeAttribute
var target: Node2D
var tick: float


func _ready() -> void:
	health = attribute_container.get_attribute_by_name("health")
	movement_speed = attribute_container.get_attribute_by_name("movement_speed")
	fire_rate = attribute_container.get_attribute_by_name("fire_rate")
	pickup_radius = attribute_container.get_attribute_by_name("pickup_radius")

	if health:
		progress_bar.max_value = health.attribute.max_value
		progress_bar.min_value = health.attribute.min_value
		progress_bar.value = health.get_buffed_value()
	
	attribute_container.attribute_changed.connect(func (attribute, _old, new_value):
		if attribute is HealthAttribute:
			progress_bar.value = new_value
			if new_value <= 0.01:
				is_dead = true
				died.emit()	
	)


func _input(event: InputEvent) -> void:
	if event.is_action_pressed("die_immediately"):
		print("uh oh")
		var instantdeath = AttributeBuff.new()
		instantdeath.attribute_name = "health"
		instantdeath.buff_type = instantdeath.BT_ONESHOT
		instantdeath.operation = AttributeOperation.subtract(99999999)
		attribute_container.apply_buff(instantdeath)


func _process(delta: float) -> void:
	tick += delta

	if tick > fire_rate.get_buffed_value():
		tick = tick - fire_rate.get_buffed_value()
		fire_projectile()


func _physics_process(delta: float) -> void:
	if is_dead:
		progress_bar.modulate.a = lerp(progress_bar.modulate.a, 0.0, 1.0 * delta)
		return
	
	var current_speed = movement_speed.get_buffed_value()
	var move_vector = Input.get_vector("move_left", "move_right", "move_up", "move_down").normalized()

	velocity =  (move_vector * current_speed)
	
	move_and_slide()


func fire_projectile() -> void:
	if is_dead: 
		return

	var next: Node2D = null

	for child in get_tree().get_nodes_in_group("mobs"):
		if next == null:
			next = child
		elif child.global_position.distance_to(global_position) < next.global_position.distance_to(global_position):
			next = child
	
	if next:
		projectile_fired.emit(global_position, next.global_position, attribute_container.get_attribute_by_name("damage").get_buffed_value())
