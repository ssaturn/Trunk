#pragma once

#include <mutex>
#include <atomic>
#include <assert.h>

namespace sst
{
	namespace memory
	{
		template<size_t SIZE>
		constexpr bool validate_buffer_size()
		{
			if (SIZE == 1) return true;
			if (SIZE % 2 == 1) return false;
			return validate_buffer_size<SIZE / 2>();
		}

		template<size_t SIZE>
		class circular_buffer
		{
		public:
			struct argments
			{
				char* buf = nullptr;
				size_t len = 0;
			};

			circular_buffer()
				: _count(0)
				, _head(0)
				, _tail(0)
				, _tail_done(0)
			{
				static_assert(validate_buffer_size<SIZE>());

				_buf = new char[SIZE];
			}

			/*
			 * tail을 미리 밀어놓고 데이터를 마음놓고 쓰기
			 */
			bool write(const char* buf, size_t len)
			{
				assert(buf != nullptr);
				assert(len > 0);

				// check overflow thread unsafe
				if (SIZE - _count.fetch_add(len) < len)
				{
					_count.fetch_sub(len);
					return false;
				}

				auto tail = _tail.fetch_add(len);
				if (tail > SIZE)
				{
					tail = tail & (SIZE - 1);
					_tail.fetch_and(SIZE - 1);
				}

				size_t write_size = len;
				size_t written_size = 0;
				if (tail + len > SIZE)
				{
					memcpy(_buf + tail, buf, SIZE - tail);
					written_size = (SIZE - tail);
					write_size = len - written_size;
					tail = 0;
				}
				memcpy(_buf + tail, buf + written_size, write_size);

				tail = _tail_done.fetch_add(len);
				if (tail > SIZE)
				{
					_tail_done.fetch_and(SIZE - 1);
				}
				
				return true;
			}

			bool read(size_t len)
			{
				assert(len > 0 && len < SIZE);

				// check empty thread unsafe
				if (len > _count.load())
				{
					return false;
				}
				
				size_t read_size = 0;
				if (head > SIZE)
				{
					head = head % SIZE;
					_head.fetch_and(SIZE - 1);
				}

				_count.fetch_sub(len);

				return true;
			}

			bool get(argments* args, unsigned& buffer_size)
			{
				// init argment
				args[0].buf = nullptr;
				args[0].len = 0;
				args[1].buf = nullptr;
				args[1].len = 0;

				auto head = _head.load() % SIZE;
				auto tail = _tail_done.load() % SIZE;
				if (head == 0 && tail == 0)
				{
					return false;
				}
				
				buffer_size++;
				auto read_size = tail - head;
				int index = 0;
				if (head > tail)
				{
					read_size = _size - head;
					args[index].buf = _buf + head;
					args[index].len = read_size;
					
					head = 0;
					read_size = tail - head;
					if (read_size > 0)
					{
						buffer_size++;
						index++;
					}
				}

				if (read_size > 0)
				{
					args[index].buf = _buf + head;
					args[index].len = tail - head;

					assert(args[index].len > 0);
				}
				

				return true;
			}

			size_t remain()
			{
				return _count.load();
			}

		private:
			char* _buf;
			std::atomic<size_t> _head;
			std::atomic<size_t> _tail;
			std::atomic<size_t> _tail_done;
			std::atomic<size_t> _count;
		};
	}
}