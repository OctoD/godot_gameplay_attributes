extends Node

const PROJECTILE = preload("res://examples/the_stress_test_game/game/projectile/projectile.tscn")


@export var spawn_target: Node2D


func fire_at(from: Vector2, to: Vector2, damage: float) -> void:
	var instance = PROJECTILE.instantiate()
	print("This pewpew will deal this damage: ", damage)
	spawn_target.add_child(instance)
	instance.global_position = from
	instance.direction = (to - from).normalized()
	instance.damage = damage
	
