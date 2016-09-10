#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/external/ExternalSystemArray.hpp>
#	include <samchon/protocol/parallel/ParallelSystem.hpp>

namespace samchon
{
namespace protocol
{
namespace parallel
{
	class SAMCHON_FRAMEWORK_API ParallelSystemArray
		: public virtual external::ExternalSystemArray
	{
		friend class ParallelSystem;

	private:
		typedef external::ExternalSystemArray super;

		size_t history_sequence;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		ParallelSystemArray();

		virtual ~ParallelSystemArray();

		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		SHARED_ENTITY_DEQUE_ELEMENT_ACCESSOR_INLINE(ParallelSystem)

		/* ---------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------- */
		void sendPieceData(std::shared_ptr<Invoke> invoke, size_t size)
		{
			sendPieceData(invoke, 0, size);
		};
		virtual void sendPieceData(std::shared_ptr<Invoke>, size_t, size_t);

	private:
		virtual auto _Notify_end(std::shared_ptr<InvokeHistory>) -> bool;

		void normalize_performance();
	};
};
};
};