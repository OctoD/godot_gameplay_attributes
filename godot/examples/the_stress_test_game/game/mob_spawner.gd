extends Node

const MOB = preload("res://examples/the_stress_test_game/game/mob/mob.tscn")

@export var mobs_types: Array[MobType] = []
@export var spawn_target: Node = null
@export var player_target: CharacterBody2D
@export var spawn_rate: int = 5
@export var max_mobs_at_once: int = 20


var tick: float = 0.0


func _ready() -> void:
	spawn()


func _physics_process(delta: float) -> void:
	tick += delta
	
	if int(tick) > spawn_rate:
		tick = 0.0
		spawn()


func spawn() -> void:
	var mobs_to_spawn = max_mobs_at_once - spawn_target.get_child_count()
	var player_global_position = player_target.global_position

	for i in range(mobs_to_spawn):
		var mob = mobs_types.pick_random() as MobType
		var instance = MOB.instantiate()
		var rand_x = randi_range(-1, 0)
		var rand_y = randi_range(-1, 0)
		
		instance.mob_type = mob
		instance.global_position.x = (player_global_position.x + randi() % 300 + 40)
		instance.global_position.y = (player_global_position.y + randi() % 300 + 40)
		spawn_target.add_child(instance)
