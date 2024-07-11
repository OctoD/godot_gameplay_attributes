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

void BuffPoolQueue::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("start"), &BuffPoolQueue::start);
	ClassDB::bind_method(D_METHOD("stop"), &BuffPoolQueue::stop);

	/// adds signals
	ADD_SIGNAL(MethodInfo("attribute_buff_dequeued", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "RuntimeBuff")));
	ADD_SIGNAL(MethodInfo("attribute_buff_enqueued", PropertyInfo(Variant::OBJECT, "buff", PROPERTY_HINT_RESOURCE_TYPE, "RuntimeBuff")));
}

void BuffPoolQueue::_exit_tree()
{
	clear();
}

void BuffPoolQueue::_physics_process(double p_delta)
{
	if (!started) {
		return;
	}

	tick += p_delta;

	if (tick >= 1.0) {
		tick = tick - 1.0f;
		process_items();
	}
}

void BuffPoolQueue::enqueue(Ref<RuntimeBuff> p_buff)
{
	if (server_authoritative && !is_multiplayer_authority()) {
		return;
	}

	queue.push_back(p_buff);
	emit_signal("attribute_buff_enqueued", p_buff);
}

bool BuffPoolQueue::get_server_authoritative() const
{
	return server_authoritative;
}

void BuffPoolQueue::clear()
{
	queue.clear();
}

void BuffPoolQueue::process_items()
{
	if (server_authoritative && !is_multiplayer_authority()) {
		return;
	}

	for (int i = queue.size() - 1; i >= 0; i--) {
		Ref<RuntimeBuff> buff = queue[i];
		buff->set_time_left(buff->get_time_left() - 1.0);

		if (buff->can_dequeue()) {
			emit_signal("attribute_buff_dequeued", buff);
			queue.remove_at(i);
		}
	}
}

void BuffPoolQueue::set_server_authoritative(const bool p_server_authoritative)
{
	server_authoritative = p_server_authoritative;
}

void BuffPoolQueue::start()
{
	started = true;
}

void BuffPoolQueue::stop()
{
	started = false;
}
