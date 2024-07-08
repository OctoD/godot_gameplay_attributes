Godot Gameplay Attributes
=========================

This document describes the gameplay attributes of the game. It is intended to be a reference for the game designers and developers to understand the game mechanics and how they interact with each other.

# Installation

1. Download the latest version of the addon from the [releases page](https://github.com/OctoD/godot_gameplay_attributes/releases).
2. Extract the contents of the zip file into your project's `addons` folder.
3. Enjoy!

# Usage

The addon provides a set of nodes and resources that can be used to define gameplay attributes. These nodes can be added to any scene in your game to define the attributes of the objects in that scene.

## Attribute

The `Attribute` resource defines a single attribute that can be attached to an object. It has the following properties:

- **attribute_name**: The name of the attribute.
- **initial_value**: The initial value of the attribute.
- **min_value**: The minimum value of the attribute.
- **max_value**: The maximum value of the attribute.

Each attribute can receive a set of `AttributeBuff` resources that modify its value. Buffs and debuffs can be added to the attribute using the `add_buff` methods. 

Each `AttributeBuff` resource affect a single attribute, and can be one shot or persistent. Buffs can be added to the attribute using the `add_buff` `Attribute` method. Each `AttributeBuff` has an `operation` property that defines how the buff affects the attribute value. The possible operations are:

- **OP_ADD**: Adds a value to the attribute.
- **OP_DIVIDE**: Divides the attribute value by a value.
- **OP_MULTIPLY**: Multiplies the attribute value by a value.
- **OP_PERCENTAGE**: Adds (or subtract if the value is negative) a percentage of the attribute value.
- **OP_SUBTRACT**: Subtracts a value from the attribute.

If the AttributeBuff is persistent, it will be applied until it is removed. If it is one shot, it will be applied only once and will modify the attribute directly.

An example of using an `Attribute` resource programmatically:

```gdscript
func _ready() -> void:
    var attribute = Attribute.new()

    attribute.attribute_name = "health"
    attribute.initial_value = 100
    attribute.min_value = 0
    attribute.max_value = 120

    var buff = AttributeBuff.new()
    var debuff = AttributeBuff.new()

    buff.attribute_name = attribute.attribute_name
    buff.operation = AttributeBuff.Operation.Add(10)
    debuff.attribute_name = attribute.attribute_name
    debuff.operation = AttributeBuff.Operation.Subtract(5)

    print(attribute.current_value()) # 100

    attribute.add_buff(buff)

    print(attribute.current_value()) # 110
    
    attribute.add_buff(buff)

    print(attribute.current_value()) # 120

    attribute.add_buff(buff)

    print(attribute.current_value()) # 120, since the attribute is capped at 120

    attribute.add_buff(debuff)

    print(attribute.current_value()) # 105
```

## AttributeContainer

To manage multiple attributes in a single object, you can use the `AttributeContainer` node. This node has an `AttributeSet`, which holds many `Attribute` resources. It also has a method to add new attribute programmatically to the container, to add and remove buffs, signals and more.

It handles time based buffs so you do not need to worry about managing them manually.

I would recommend using the `AttributeContainer` node to manage the attributes of your objects, as it provides a more convenient way to manage multiple attributes.

An example of using an `AttributeContainer` node is located in the `godot` folder of this repository.
