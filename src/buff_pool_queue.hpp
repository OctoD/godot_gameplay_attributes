/**************************************************************************/
/*  buff_pool_queue.hpp                                                   */
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

#ifndef GGA_BUFF_POOL_QUEUE_HPP
#define GGA_BUFF_POOL_QUEUE_HPP

#include <godot_cpp/classes/node.hpp>

using namespace godot;

namespace gga
{
	class AttributeBuff;

	class BuffPoolQueueItem : public RefCounted
	{
		GDCLASS(BuffPoolQueueItem, RefCounted);

	protected:
		/// @brief Binds methods to Godot.
		static void _bind_methods();
		/// @brief The buff enqueued.
		Ref<AttributeBuff> buff;
		/// @brief Whether the buff is eligible for removal.
		bool eligible_for_removal;
		/// @brief The remaining time in seconds.
		float seconds_remaining;

	public:
		/// @brief Removes one second from the remaining time.
		void second_passed();
		/// @brief Returns the buff.
		Ref<AttributeBuff> get_buff();
		/// @brief Returns whether the buff is eligible for removal.
		bool get_eligible_for_removal();
		/// @brief Sets the buff.
		void set_buff(Ref<AttributeBuff> p_buff);
	};

	class BuffPoolQueue : public Node
	{
		GDCLASS(BuffPoolQueue, Node);

	protected:
		/// @brief Binds methods to Godot.
		static void _bind_methods();
		/// @brief The current queue size.
		uint16_t current_queue_size;
		/// @brief The current tick.
		float tick;
		/// @brief The queue of buffs.
		TypedArray<BuffPoolQueueItem> queue;
		/// @brief Whether the queue is server authoritative.
		bool server_authoritative;
		/// @brief Whether the queue has started.
		bool started;

	public:
		/// @brief Overridden _exit_tree method.
		void _exit_tree() override;
		/// @brief Overridden _physics_process method.
		void _physics_process(double p_delta) override;
		/// @brief Adds a buff to the queue.
		void add_attribute_buff(Ref<AttributeBuff> p_buff);
		/// @brief Returns if the queue is server authoritative.
		/// @return Whether the queue is server authoritative.
		bool get_server_authoritative() const;
		/// @brief Clears the queue.
		void clear();
		/// @brief Cleans up the queue and emit signals.
		void cleanup();
		/// @brief Processes the items in the queue.
		void process_items();
		/// @brief Sets the server authoritative flag.
		/// @param p_server_authoritative The server authoritative flag.
		void set_server_authoritative(const bool p_server_authoritative);
		/// @brief Starts the queue.
		void start();
		/// @brief Stops the queue.
		void stop();
	};
} //namespace gga

#endif