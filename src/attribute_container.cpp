#include "attribute_container.hpp"

#include "attribute.hpp"
#include "buff_pool_queue.hpp"

using namespace gga;

void AttributeContainer::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("add_attribute", "p_attribute"), &AttributeContainer::add_attribute);
	ClassDB::bind_method(D_METHOD("apply_buff", "p_buff"), &AttributeContainer::apply_buff);
	ClassDB::bind_method(D_METHOD("remove_attribute", "p_attribute"), &AttributeContainer::remove_attribute);
	ClassDB::bind_method(D_METHOD("remove_buff", "p_buff"), &AttributeContainer::remove_buff);
	ClassDB::bind_method(D_METHOD("get_attributes"), &AttributeContainer::get_attributes);
	ClassDB::bind_method(D_METHOD("get_server_authoritative"), &AttributeContainer::get_server_authoritative);
	ClassDB::bind_method(D_METHOD("set_attributes", "p_attributes"), &AttributeContainer::set_attributes);
	ClassDB::bind_method(D_METHOD("set_server_authoritative", "p_server_authoritative"), &AttributeContainer::set_server_authoritative);

	/// binds properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "attributes"), "set_attributes", "get_attributes");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "server_authoritative"), "set_server_authoritative", "get_server_authoritative");

	/// signals binding
	ADD_SIGNAL(MethodInfo("attribute_added", PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "Attribute")));
	ADD_SIGNAL(MethodInfo("attribute_changed", PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "Attribute"), PropertyInfo(Variant::FLOAT, "previous_value"), PropertyInfo(Variant::FLOAT, "new_value")));
	ADD_SIGNAL(MethodInfo("attribute_removed", PropertyInfo(Variant::OBJECT, "attribute", PROPERTY_HINT_RESOURCE_TYPE, "Attribute")));
	ADD_SIGNAL(MethodInfo("buff_applied", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBuff")));
	ADD_SIGNAL(MethodInfo("buff_enqued", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBuff")));
	ADD_SIGNAL(MethodInfo("buff_dequed", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBuff")));
	ADD_SIGNAL(MethodInfo("buff_removed", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBuff")));
}

void AttributeContainer::_on_attribute_changed(Ref<Attribute> p_attribute, const float p_previous_value, const float p_new_value)
{
	emit_signal("attribute_changed", p_attribute, p_previous_value, p_new_value);
}

void AttributeContainer::_on_buff_dequeued(Ref<AttributeBuff> p_buff)
{
	emit_signal("buff_dequed", p_buff);
}

void AttributeContainer::_on_buff_enqueued(Ref<AttributeBuff> p_buff)
{
	emit_signal("buff_enqued", p_buff);
}

void AttributeContainer::bind_attribute(Ref<Attribute> p_attribute)
{
	p_attribute->connect("changed", Callable::create(this, "_on_attribute_changed"));
}

bool AttributeContainer::has_attribute(Ref<Attribute> p_attribute)
{
	for (int i = 0; i < attributes.size(); i++) {
		if (attributes[i] == p_attribute) {
			return true;
		}
	}

	return false;
}

void AttributeContainer::_ready()
{
	/// initializes the BuffPoolQueue
	buff_pool_queue = memnew(BuffPoolQueue);
	buff_pool_queue->set_server_authoritative(server_authoritative);
	buff_pool_queue->connect("buff_dequeued", Callable::create(this, "_on_buff_dequeued"));
	buff_pool_queue->connect("buff_enqueued", Callable::create(this, "_on_buff_enqueued"));

	add_child(buff_pool_queue);

	for (int i = 0; i < attributes.size(); i++) {
		bind_attribute(attributes[i]);
	}
}

void AttributeContainer::add_attribute(Ref<Attribute> p_attribute)
{
	if (!has_attribute(p_attribute)) {
		attributes.push_back(p_attribute);
		bind_attribute(p_attribute);
		emit_signal("attribute_added", p_attribute);
	}
}

void AttributeContainer::apply_buff(Ref<AttributeBuff> p_buff)
{
	for (int i = 0; i < attributes.size(); i++) {
		Ref<Attribute> attribute = attributes[i];

		if (attribute->add_buff(p_buff)) {
			if (p_buff->get_duration() != 0.0f) {
				buff_pool_queue->add_attribute_buff(p_buff.ptr());
			}

			emit_signal("buff_applied", p_buff);
		}
	}
}

void gga::AttributeContainer::remove_attribute(Ref<Attribute> p_attribute)
{
	if (has_attribute(p_attribute)) {
		int index = attributes.find(p_attribute);

		if (index != -1) {
			Ref<Attribute> attribute = attributes[index];
			attribute->disconnect("changed", Callable::create(this, "_on_attribute_changed"));
		}
	}
}

void AttributeContainer::remove_buff(Ref<AttributeBuff> p_buff)
{
	for (int i = 0; i < attributes.size(); i++) {
		Ref<Attribute> attribute = attributes[i];

		if (attribute->remove_buff(p_buff)) {
			emit_signal("buff_removed", p_buff);
		}
	}
}

TypedArray<Attribute> AttributeContainer::get_attributes() const
{
	return attributes;
}

bool AttributeContainer::get_server_authoritative() const
{
	return server_authoritative;
}

void AttributeContainer::set_attributes(const TypedArray<Attribute> p_attributes)
{
	attributes = p_attributes;
}

void AttributeContainer::set_server_authoritative(const bool p_server_authoritative)
{
	server_authoritative = p_server_authoritative;
}
