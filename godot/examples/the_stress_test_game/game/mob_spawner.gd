extends Node

@export var mobs_types: Array[PackedScene] = []
@export var spawn_target: Node = null
@export var spawn_rate: int = 5
@export var max_mobs_at_once: int = 20


var tick: float = 0.0


func _physics_process(delta: float) -> void:
	tick += delta
	
	if int(tick) > spawn_rate:
		tick = 0.0
		spawn()


func spawn() -> void:
	var mobs_to_spawn = max_mobs_at_once - spawn_target.get_child_count()
	
	for i in range(mobs_to_spawn + 1):
		spawn_target.add_child(mobs_types.pick_random().instantiate())
