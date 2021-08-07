//#include<../app/graphics-pro/mt/MtRenderer.hpp>
//#include<memory/Allocator.hpp>
//
//namespace disk::gfx
//{
//	MtRenderer::MtRenderer(IAllocator& allocator) :
//		_allocator{ allocator },
//		_frame{ 0u }
//	{
//	}
//
//	void MtRenderer::frame_begin()
//	{
//		++_frame;
//	}
//
//	static thread_local Allocator base_alloc;
//	static thread_local LinearAllocator tls_alloc[] =
//	{
//		{ base_alloc, 10000000 },
//		{ base_alloc, 10000000 },
//	};
//
//	IAllocator& MtRenderer::get_allocator()
//	{
//		static thread_local int frame = 0;
//		static thread_local LinearAllocator* alloc = nullptr;
//		if (frame != _frame) {
//			frame = _frame;
//			alloc = &tls_alloc[frame % 2];
//			alloc->reset();
//		}
//		return *alloc;
//	}
//
//}