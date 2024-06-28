extends VBoxContainer

@export var examples: Array[PackedScene] = []

@onready var back_button: Button = %BackButton
@onready var example_selection: VBoxContainer = %ExampleSelection
@onready var quit_button: Button = %QuitButton
@onready var toolbar: HBoxContainer = %Toolbar
@onready var running_example: SubViewport = %RunningExample
@onready var main_view: VBoxContainer = %MainView
@onready var sub_viewport_container: SubViewportContainer = %SubViewportContainer


func _ready() -> void:
	back_button.pressed.connect(func ():
		for child in running_example.get_children():
			child.queue_free()
			
		main_view.visible = true
		toolbar.visible = false
		sub_viewport_container.visible = false
	)
	
	toolbar.visible = false
	
	for example in examples:
		var button = Button.new()

		button.pressed.connect(func ():
			load_example(example)	
		)
		
		button.text = example.resource_path.replacen("_", " ").replace(".tscn", "").get_file().get_basename().capitalize()
		example_selection.add_child(button)

	quit_button.pressed.connect(func ():
		get_tree().quit()
	)


func load_example(example: PackedScene) -> void:
	main_view.visible = false
	toolbar.visible = true
	sub_viewport_container.visible = true
	running_example.add_child(example.instantiate())
