class_name PowerUpSpawnTable extends Resource


const POWER_UP = preload("res://examples/the_stress_test_game/game/power_ups/power_up.tscn")


@export var power_ups: Array[PowerUpResource]


func spawn() -> Node2D:
	var random = power_ups.pick_random() as PowerUpResource
	var number = randf_range(0, 100)
	
	if random and random.spawn_chance < number:
		var item_2d = POWER_UP.instantiate()
		
		item_2d.power_up_resource = random
		
		return item_2d
	else:
		return null
