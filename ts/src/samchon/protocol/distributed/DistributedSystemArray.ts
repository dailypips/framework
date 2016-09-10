﻿/// <reference path="../../API.ts" />

/// <reference path="../parallel/ParallelSystemArray.ts" />

namespace samchon.protocol.distributed
{
	export abstract class DistributedSystemArray extends parallel.ParallelSystemArray
	{
		/**
		 * @hidden
		 */
		private role_map_: collection.HashMapCollection<string, DistributedSystemRole>;

		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		public constructor()
		{
			super();

			// CREATE ROLE MAP AND ENROLL COLLECTION EVENT LISTENRES
			this.role_map_ = new collection.HashMapCollection<string, DistributedSystemRole>();
			this.role_map_.addEventListener("insert", this.handle_role_insert, this);
			this.role_map_.addEventListener("erase", this.handle_role_erase, this);
		}

		public construct(xml: library.XML): void
		{
			//--------
			// CONSTRUCT ROLES
			//--------
			// CLEAR ORDINARY ROLES
			this.role_map_.clear();

			// CREATE ROLES
			if (xml.has("roles") == true && xml.get("roles").front().has("role") == true)
			{
				let role_xml_list: library.XMLList = xml.get("roles").front().get("role");
				for (let i: number = 0; i < role_xml_list.size(); i++)
				{
					let role_xml: library.XML = role_xml_list.at(i);

					// CONSTRUCT ROLE FROM XML
					let role: DistributedSystemRole = this.createRole(role_xml);
					role.construct(role_xml);

					// AND INSERT TO ROLE_MAP
					this.role_map_.insert([role.getName(), role]);
				}
			}

			//--------
			// CONSTRUCT SYSTEMS
			//--------
			super.construct(xml);
		}

		public abstract createRole(xml: library.XML): DistributedSystemRole;

		private handle_role_insert(event: collection.MapCollectionEvent<string, DistributedSystemRole>): void
		{
			for (let it = event.first; !it.equal_to(event.last); it = it.next())
				for (let i: number = 0; i < this.size(); i++)
					this.at(i).push_back(it.second);
		}
		private handle_role_erase(event: collection.MapCollectionEvent<string, DistributedSystemRole>): void
		{
			for (let it = event.first; !it.equal_to(event.last); it = it.next())
				for (let i: number = 0; i < this.size(); i++)
				{
					let system: DistributedSystem = this.at(i) as DistributedSystem;
					std.remove(system.begin(), system.end(), it.second as external.ExternalSystemRole);
				}
		}

		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		/**
		 * @inheritdoc
		 */
		public at(index: number): DistributedSystem
		{
			return super.at(index) as DistributedSystem;
		}

		public getRoleMap(): std.HashMap<string, DistributedSystemRole>
		{
			return this.role_map_;
		}

		/**
		 * @inheritdoc
		 */
		public hasRole(name: string): boolean
		{
			return this.role_map_.has(name);
		}

		/**
		 * @inheritdoc
		 */
		public getRole(name: string): DistributedSystemRole
		{
			return this.role_map_.get(name);
		}
	}
}