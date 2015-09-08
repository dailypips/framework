#pragma once
#include <samchon\API.hpp>

#include <samchon/protocol/SharedEntityArray.hpp>
#include <samchon/protocol/IProtocol.hpp>

#include <samchon/Map.hpp>

namespace samchon
{
	namespace protocol
	{
		namespace master
		{
			class ExternalSystem;
			class ExternalSystemRole;

			/**
			 * @brief Manager for external system(s)
			 *
			 * @author Jeongho Nam
			 */
			class SAMCHON_FRAMEWORK_API ExternalSystemArray
				: public virtual SharedEntityArray,
				public virtual IProtocol
			{
			private:
				typedef SharedEntityArray super;

			protected:
				virtual auto TAG() const -> String;
				virtual auto CHILD_TAG() const -> String;

			public:
				/**
				 * @Default Constructor
				 */
				ExternalSystemArray();
				virtual ~ExternalSystemArray() = default;

				/**
				 * @brief Start manager of external systems
				 */
				virtual void start() = NULL;

				/**
				 * @brief Factory method of ExternalSystemRole
				 */
				virtual auto createRole(std::shared_ptr<library::XML>) -> ExternalSystemRole* = NULL;

				SHARED_ENTITY_ARRAY_ELEMENT_ACCESSOR_HEADER(ExternalSystem)
				auto hasRole(const String&) const -> bool;
				auto getRole(const String&) const -> ExternalSystemRole*;

			public:
				/**
				 * @brief Send a Invoke message to (an) ExternalSystem(s)
				 */
				virtual void sendData(std::shared_ptr<Invoke>);
			};
		};
	};
};