class_name UnitySurvivors extends Node2D

signal gamerestarted()
signal gameover()

const PLAYER = preload("res://examples/the_stress_test_game/game/player/player.tscn")
const Mobs = preload("res://examples/the_stress_test_game/game/mobs.gd")
const ProjectileSpawner = preload("res://examples/the_stress_test_game/game/projectile_spawner.gd")
const GameMode = preload("res://examples/the_stress_test_game/game/game_mode.gd")

@onready var mobs: Node2D = $Mobs
@onready var player: Player = $Player
@onready var projectile_spawner: Node = $ProjectileSpawner
@onready var game_mode: Node = $GameMode

var gameover_screen: Node

func _ready() -> void:
	player.projectile_fired.connect(func (f, t, d):
		projectile_spawner.fire_at(f, t, d)
	)
	
	player.died.connect(func ():
		print("Oh no you died!")
		gameover.emit()
	)
