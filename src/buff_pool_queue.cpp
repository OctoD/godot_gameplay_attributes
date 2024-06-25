#include "buff_pool_queue.hpp"
#include "attribute.hpp"

using namespace gga;

void BuffPoolQueueItem::_bind_methods()
{
}

void BuffPoolQueueItem::second_passed()
{
	seconds_remaining -= 1;
	eligible_for_removal = seconds_remaining <= 0;
}

AttributeBuff *BuffPoolQueueItem::get_buff()
{
	return buff;
}

bool BuffPoolQueueItem::get_eligible_for_removal()
{
	return eligible_for_removal;
}

float BuffPoolQueueItem::get_seconds_remaining()
{
	return seconds_remaining;
}

void BuffPoolQueueItem::set_buff(AttributeBuff *p_buff)
{
	buff = p_buff;
	seconds_remaining = buff->get_duration();
}

void BuffPoolQueue::_bind_methods()
{
	/// adds signals
	ADD_SIGNAL(MethodInfo("attribute_buff_dequeued", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBuff")));
	ADD_SIGNAL(MethodInfo("attribute_buff_enqueued", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "AttributeBuff")));
}

void BuffPoolQueue::_physics_process(double p_delta)
{
	tick += p_delta;

	if (tick >= 1) {
		tick = tick - 1.0f;
		process_items();
	}
}

void BuffPoolQueue::add_attribute_buff(AttributeBuff *p_buff)
{
	if (server_authoritative && !is_multiplayer_authority()) {
		return;
	}

	Ref<BuffPoolQueueItem> item = memnew(BuffPoolQueueItem);
	item->set_buff(p_buff);
	queue.push_back(item);

	emit_signal("attribute_buff_enqueued", p_buff);
}

bool BuffPoolQueue::get_server_authoritative() const
{
	return server_authoritative;
}

void BuffPoolQueue::cleanup()
{
	for (int i = queue.size() - 1; i >= 0; i--) {
		Ref<BuffPoolQueueItem> item = queue[i];

		if (item->get_eligible_for_removal()) {
			emit_signal("attribute_buff_dequeued", item->get_buff());
			queue.remove_at(i);
		}
	}
}

void BuffPoolQueue::process_items()
{
	if (server_authoritative && !is_multiplayer_authority()) {
		return;
	}

	for (int i = 0; i < queue.size(); i++) {
		Ref<BuffPoolQueueItem> item = queue[i];
		item->second_passed();
	}

	cleanup();
}

void BuffPoolQueue::set_server_authoritative(const bool p_server_authoritative)
{
	server_authoritative = p_server_authoritative;
}
