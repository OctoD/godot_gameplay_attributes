extends ProgressBar

@onready var label: Label = $Label


func _physics_process(delta: float) -> void:
	if max_value <= value + delta:
		queue_free()
	else:
		value = value + delta


func set_buff(p_buff: AttributeBuff) -> void:
	max_value = p_buff.duration
	min_value = 0
	label.text = p_buff.buff_name
