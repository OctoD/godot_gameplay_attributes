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
	class RuntimeBuff;

	class BuffPoolQueue : public Node
	{
		GDCLASS(BuffPoolQueue, Node);

	protected:
		/// @brief Binds methods to Godot.
		static void _bind_methods();
		/// @brief The current tick.
		double tick;
		/// @brief The queue of buffs.
		TypedArray<RuntimeBuff> queue;
		/// @brief Whether the queue is server authoritative.
		bool server_authoritative;

	public:
		/// @brief Overridden _exit_tree method.
		void _exit_tree() override;
		/// @brief Overridden _physics_process method.
		void handle_physics_process(double p_delta);
		/// @brief Adds a buff to the queue.
		void enqueue(Ref<RuntimeBuff> p_buff);
		/// @brief Returns if the queue is server authoritative.
		/// @return Whether the queue is server authoritative.
		bool get_server_authoritative() const;
		/// @brief Clears the queue.
		void clear();
		/// @brief Processes the items in the queue.
		void process_items(const double p_discarded);
		/// @brief Sets the server authoritative flag.
		/// @param p_server_authoritative The server authoritative flag.
		void set_server_authoritative(const bool p_server_authoritative);
	};
} //namespace gga

#endif