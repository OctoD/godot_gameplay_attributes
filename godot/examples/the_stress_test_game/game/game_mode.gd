extends Node

const MOB = preload("res://examples/the_stress_test_game/game/mob/mob.tscn")
const LOOT_TABLE = preload("res://examples/the_stress_test_game/game/mob/loot_table.tres")


@export var mobs_types: Array[MobType] = []
@export var spawn_target: Node = null
@export var player_target: CharacterBody2D
@export var spawn_rate: int = 5
@export var max_mobs_at_once: int = 20

@export var wave: int = 1
@export var wave_duration: float = 30.0
@export var mobs_per_wave: int = 4


var tick: float = 0.0


func _on_mob_died(position: Vector2) -> void:
	var loot = LOOT_TABLE.spawn()
	
	print("mob killed")
	
	if loot:
		call_deferred("spawn_loot", loot, position)
		
	call_deferred("spawn")


func _ready() -> void:
	var wave_timer = Timer.new()
	
	wave_timer.timeout.connect(func ():
		wave += 1	
	)
	
	wave_timer.wait_time = wave_duration
	add_child(wave_timer)
	wave_timer.start()
	spawn()


func spawn() -> void:
	var current_wave_max_mobs = mobs_per_wave * wave
	var mobs_to_spawn = clamp(max_mobs_at_once - spawn_target.get_child_count(), 0, current_wave_max_mobs)
	
	print("wave: ", wave, " current_wave_max_mobs: ", current_wave_max_mobs, " mobs_to_spawn: ", mobs_to_spawn)

	for i in range(mobs_to_spawn):
		spawn_mob(mobs_types.pick_random() as MobType)


func spawn_loot(loot: Node2D, p_global_position: Vector2) -> void:
	add_child(loot)
	loot.global_position = p_global_position


func spawn_mob(mob_type: MobType) -> void:
	var instance = MOB.instantiate()
	var path = player_target.spawn_safeguard as Path2D
	
	var pathlen = path.curve.get_baked_length()
	var randpoint = randf_range(0, pathlen)
	var xy = path.curve.sample_baked(randpoint)
	instance.mob_type = mob_type
	instance.global_position.x = xy.x
	instance.global_position.y = xy.y
	instance.died.connect(_on_mob_died)
	spawn_target.add_child(instance)
