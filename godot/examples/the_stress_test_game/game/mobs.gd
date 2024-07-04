extends Node2D


@export var chase_target: CharacterBody2D


func _process(_d: float) -> void:
	for child in get_children():
		child.chase_target = chase_target
