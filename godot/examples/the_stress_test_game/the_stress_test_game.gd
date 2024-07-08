extends Control

@onready var gameover_screen: VBoxContainer = %GameoverScreen
@onready var game: UnitySurvivors = %Game

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	gameover_screen.hide()
	game.gameover.connect(gameover_screen.show.bind())
	game.gamerestarted.connect(gameover_screen.hide.bind())
