extends Node2D


@onready var player: CharacterBody2D = $Player
@onready var projectile_spawner: Node = $ProjectileSpawner


func _ready() -> void:
	player.projectile_fired.connect(func (f, t):
		projectile_spawner.fire_at(f, t)
	)
