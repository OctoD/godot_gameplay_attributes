extends Area2D


@export var power_up_resource: PowerUpResource


@onready var sprite_2d: Sprite2D = $Sprite2D


func _on_body_entered(body: Node) -> void:
	if body is Player and power_up_resource and power_up_resource.buff:
		body.attribute_container.apply_buff(power_up_resource.buff)
		queue_free()


func _ready() -> void:
	body_entered.connect(_on_body_entered)
	
	if power_up_resource:
		sprite_2d.texture = power_up_resource.texture
