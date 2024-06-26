/**************************************************************************/
/*  buff_pool_queue.cpp                                                   */
/**************************************************************************/
/*                         This file is part of:                          */
/*                        Godot Gameplay Systems                          */
/*              https://github.com/OctoD/godot-gameplay-systems           */
/**************************************************************************/
/* Copyright (c) 2020-present Paolo "OctoD"      Roth (see AUTHORS.md).   */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

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

Ref<AttributeBuff> BuffPoolQueueItem::get_buff()
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

void BuffPoolQueueItem::set_buff(Ref<AttributeBuff> p_buff)
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

void BuffPoolQueue::add_attribute_buff(Ref<AttributeBuff> p_buff)
{
	if (server_authoritative && !is_multiplayer_authority()) {
		return;
	}

	Ref<BuffPoolQueueItem> item = memnew(BuffPoolQueueItem);
	item->set_buff(p_buff);
	queue.push_back(item.ptr());

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
