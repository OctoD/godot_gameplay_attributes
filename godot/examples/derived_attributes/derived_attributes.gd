extends VBoxContainer


@onready var attribute_container: AttributeContainer = $AttributeContainer
@onready var base_attributes_list: VBoxContainer = %BaseAttributesList
@onready var derived_attributes_list: VBoxContainer = %DerivedAttributesList


const ATTRIBUTE = preload("res://examples/derived_attributes/attribute.tscn")


func _append_items(items, target: Node, readonly: bool) -> void:
	for item in items:
		var attr_scene = ATTRIBUTE.instantiate()
		
		target.add_child(attr_scene)
		attr_scene.bind_runtime_attribute(item)
		attr_scene.set_readonly(readonly)
		print(item.get_buffed_value())
		if (!readonly):
			attr_scene.decreased.connect(func ():
				var buff = AttributeBuff.new()
				buff.attribute_name = item.attribute.attribute_name
				buff.operation = AttributeOperation.subtract(1)
				attribute_container.apply_buff(buff)
			)
			attr_scene.increased.connect(func ():
				var buff = AttributeBuff.new()
				buff.attribute_name = item.attribute.attribute_name
				buff.operation = AttributeOperation.add(1)
				attribute_container.apply_buff(buff)
			)


func _ready() -> void:
	var base_attributes    = [
		attribute_container.get_attribute_by_name(ConstitutionAttribute.ATTRIBUTE_NAME),
		attribute_container.get_attribute_by_name(DexterityAttribute.ATTRIBUTE_NAME),
		attribute_container.get_attribute_by_name(IntelligenceAttribute.ATTRIBUTE_NAME),
		attribute_container.get_attribute_by_name(StrengthAttribute.ATTRIBUTE_NAME),
	]
	var derived_attributes = [
		attribute_container.get_attribute_by_name(ArmorDerivedAttribute.ATTRIBUTE_NAME),
		attribute_container.get_attribute_by_name(HealthDerivedAttribute.ATTRIBUTE_NAME),
		attribute_container.get_attribute_by_name(ManaDerivedAttribute.ATTRIBUTE_NAME),
		attribute_container.get_attribute_by_name(MeleeDamageDerivedAttribute.ATTRIBUTE_NAME),
	]

	_append_items(base_attributes, 		base_attributes_list,		false)
	_append_items(derived_attributes, 	derived_attributes_list,	true)
