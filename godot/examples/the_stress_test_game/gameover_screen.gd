extends VBoxContainer


func _ready() -> void:
	modulate.a = 0.0


func _physics_process(delta: float) -> void:
	if visible:
		modulate.a = lerp(modulate.a, 1.0, 1 * delta)
