extends CharacterBody2D



signal died(global_position: Vector2)


@export var chase_target: Node2D
@export var mob_type: MobType


@onready var sprite_2d: Sprite2D = $Sprite2D
@onready var attribute_container: AttributeContainer = $AttributeContainer
@onready var collision_shape_2d: CollisionShape2D = $CollisionShape2D
@onready var label: Label = $Label


var damage_tick: float = 0.0
var latest_position := Vector2.ZERO
var health: RuntimeAttribute
var movement_speed: RuntimeAttribute


func check_if_dead() -> void:
	label.text = str(health.current_value())

	if health and health.current_value() <= 0.01:
		died.emit(global_position)
		queue_free()


func _ready() -> void:
	add_to_group("mobs")

	attribute_container.attribute_set = mob_type.attribute_set
	sprite_2d.texture = mob_type.texture_2d

	health = attribute_container.get_attribute_by_name("health")
	movement_speed = attribute_container.get_attribute_by_name("movement_speed")

	attribute_container.setup()
	
	label.text = str(health.current_value())
	
	attribute_container.attribute_changed.connect(func (_attribute, _old_value, _new_value): 
		check_if_dead()
	)
	
	attribute_container.buff_applied.connect(func (_buff):
		check_if_dead()
	)


func _process(delta: float) -> void:
	if chase_target:
		latest_position = chase_target.transform.origin

		if abs(global_position.distance_to(chase_target.global_position)) < 50.0:
			damage_tick += delta
			
			if damage_tick > 1.0:
				var damage = AttributeBuff.new()
				damage.attribute_name = "health"
				damage.operation = AttributeOperation.subtract(1.0)
				damage.buff_type = damage.BT_ONESHOT
				damage_tick = damage_tick - 1.0
				chase_target.attribute_container.apply_buff(damage)


func _physics_process(_d: float) -> void:
	if movement_speed and chase_target:
		velocity = (latest_position - transform.origin).normalized() * movement_speed.current_value()
	move_and_slide()
