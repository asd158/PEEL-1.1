///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains the base class.
 *	\file		IceCell.h
 *	\author		Pierre Terdiman
 *	\date		February, 5, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICECELL_H
#define ICECELL_H

	// Loop through all owners
	#define FOREACH_OWNER(cell, owner)										\
		{for(udword __idx=0;__idx<cell->GetNbOwners();__idx++){				\
			owner = cell->GetOwner(__idx);									\
			if(owner){														\

	// Loop through all references
	#define FOREACH_REF(cell, ref)											\
		{for(udword __idx=0;__idx<cell->GetNbRefs();__idx++){				\
			ref = cell->GetRef(__idx);										\
			if(ref){														\

	// Loop through all typed references
	#define FOREACH_TREF(cell, ref, type)									\
		{for(udword __idx=0;__idx<cell->GetNbRefs();__idx++){				\
			ref = cell->GetRef(__idx);										\
			if(ref && ref->GetType()==type){								\

	// Loop through all registered cells
	#define FOREACH_CELL(cell, type)										\
		{for(udword __idx=0;__idx<GetKernel()->GetNbEntries();__idx++){		\
			cell = GetKernel()->GetEntries()[__idx];						\
			if(cell && cell->GetType()==type){								\

	// End loop
	#define END_FOREACH	}}}

	//! Cell flags
	enum CellFlag
	{
		CF_VALID				=	(1<<0),		//!< Cell has successfully been initialized
		CF_KERNEL_MODIF			=	(1<<1),		//!< Kernel control: enable direct cell members modifications by the kernel
		CF_KERNEL_DESTRUCT		=	(1<<2),		//!< Kernel control: enable direct cell destruction by the kernel
		CF_KERNEL_MSGS			=	(1<<3),		//!< Enable kernel messages
		CF_KERNEL_INVALIDREF	=	(1<<4),		//!< Enable OnInvalidReference calls
		CF_SERIALIZE			=	(1<<5),		//!< Enable serialization
		CF_LOCKED				=	(1<<6),		//!< The cell has been locked and shouldn't be modified

		CF_FORCE_DWORD			=	0x7fffffff
	};

	//! Request types
	enum KernelRequest
	{
		KR_FIELDSIZE			=	1,			//!< Request the size of a dynamic field

		KR_FORCE_DWORD			=	0x7fffffff
	};

	// Forward declarations
	class Cell;
	class Kernel;
	class DataBlock;
	class SmartContainer;
	class IceMsg;

	class ICECORE_API CellsContainer : public Allocateable
	{
		public:
		// Constructor / Destructor
											CellsContainer(const SmartContainer& container);
											CellsContainer()			{}
											~CellsContainer()			{}

		inline_			udword				GetNbCells()		const	{ return mContainer.GetNbEntries();				}
		inline_			Cell**				GetCells()			const	{ return (Cell**)mContainer.GetEntries();		}
		inline_			Cell*				GetCell(udword i)	const	{ return (Cell*)mContainer.GetEntry(i);			}

		inline_			CellsContainer&		AddCell(const Cell* cell)	{ mContainer.Add(udword(cell));	return *this;	}

		inline_			void				ResetArray()				{ mContainer.Reset();							}

		//! Operator for "Container A = SmartContainer B"
						void				operator=(const SmartContainer& container);

											PREVENT_COPY(CellsContainer)
		private:
						Container			mContainer;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Notification-callback, called by the system each time a registered notification is hit.
	 *	\param		owner		[in] object to notify
	 *	\param		param		[in] a parameter transfered from the ExecNotif call
	 *	\param		bit_mask	[in] notification bitmask
	 *	\param		user_data	[in] registered user-defined data for this callback
	 *	\return		usually, true to override a possible default behaviour
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef bool (*NotifCallback)(Cell* owner, udword param, udword bit_mask, void* user_data);

	struct ICECORE_API NotifBundle : public Allocateable
	{
		inline_			NotifBundle() : mCB(null), mUserData(null), mBitmask(0)	{}

		NotifCallback	mCB;		//!< Notification callback
		void*			mUserData;	//!< User-defined data
		udword			mBitmask;	//!< Event bitmask
	};

	class ICECORE_API Cell : public Allocateable
	{
										_DECLARE_PROTECTED_CLASS(Cell)
										DECLARE_FIELDS
										DECLARE_RTTI_BASE(Cell)
										PREVENT_COPY(Cell)
		public:
		//! TO BE DOCUMENTED
		virtual			udword			Import(ImportContext& ic);
		//! TO BE DOCUMENTED
		virtual			udword			Export(ExportContext& ec);
		//! TO BE DOCUMENTED
		virtual			bool			GetFields(FieldDescriptors& edit, udword flags)					const;
		virtual			bool			GetFields(FieldDescriptors& edit, FieldType type)				const;
		virtual			bool			GetFields(FieldDescriptors& edit, FieldType type, udword user)	const;
		virtual			bool			GetFields(FieldDescriptors& edit)								const;
		virtual	const FieldDescriptor*	GetFieldDescriptor(FieldType type, udword user)					const;
		virtual	const FieldDescriptor*	GetFieldDescriptor(const char* name)							const;

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	This method is called by the kernel after a declared field has been modified.
		 *	\param		field			[in] descriptor for the modified field
		 *	\return		true if the method has been overriden
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			bool			OnModifiedField(const FieldDescriptor& field)						{ return false;				}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	This method is called by the kernel to request various dynamic information.
		 *	\param		kr				[in] kernel request code
		 *	\param		context			[in] request context - depends on kr
		 *	\param		data			[out] a place for the user to answer the request
		 *	\return		true if the method has been overriden
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			bool			OnKernelRequest(KernelRequest kr, udword context, udword& data)		{ return false;				}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	This method is called by the kernel after a referenced cell has been dereferenced or deleted.
		 *	\param		invalid_ref		[in] the now invalid referenced cell
		 *	\return		true if the method has been overriden
		 *	\warning	Only called if CF_KERNEL_INVALIDREF is enabled
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			bool			OnInvalidReference(const Cell* invalid_ref)				{ return false;							}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	This method is called by the kernel to send a message to a given cell.
		 *	\param		msg		[in] the message structure
		 *	\return		true if the message has been caught
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			bool			Message(const IceMsg& msg);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Sends a message to all cell owners whose type is given.
		 *	\param		msg		[in] the message structure
		 *	\param		type	[in] pointer to the type, or null to send the message to all owners
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool			SendMsgToOwners(const IceMsg& msg, const char** type=null);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Sends a message to all cell references whose type is given.
		 *	\param		msg		[in] the message structure
		 *	\param		type	[in] pointer to the type, or null to send the message to all references
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool			SendMsgToRefs(const IceMsg& msg, const char** type=null);

		//! TO BE DOCUMENTED
						bool			CreateRef(Cell* ref);
						bool			DeleteRef(Cell* ref);
						bool			CreateRef(KID kid);
						bool			DeleteRef(KID kid);
						bool			CreateTypedRef(Cell* ref, RTYPE type);

		//! TO BE DOCUMENTED
						bool			ReplaceRef(Cell* oldref, Cell* newref);

		//! TO BE DOCUMENTED
						udword			GetNbRefs()									const;
						udword			GetNbOwners()								const;
						Cell*			GetOwner(udword i=0)						const;
						Cell*			GetRef(udword i=0)							const;

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets the cell dependencies, i.e. its references whose type is given.
		 *	\param		dependencies	[out] referenced cells whose type matches
		 *	\param		type			[in] wanted type, or null to dump all dependencies
		 *	\return		number of cells added to the container
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						udword			GetDependencies(CellsContainer& dependencies, const char* type=null);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets the cell's kernel ID.
		 *	\return		kernel ID
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline_			KID				GetKernelID()								const		{ return m__KernelID;					}
		//! High-speed KernelID-to-cell translation
						Cell*			KIDToCell(KID kid)							const;
		// Type
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Sets the cell's type. Despite it's a const char, you can use it as an ID by supplying addresses of fixed strings in memory.
		 *	This is the recommended way, since it provides best of both worlds:
		 *	- type comparisons are fast since you just compare 2 dwords instead of 2 strings
		 *	- yet it remains human-readable, not as if you were using GUIDs or something
		 *
		 *	\param		type	[in] cell's type
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline_			void			SetType(const char* type)								{ m__Type = type;						}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets the cell's type.
		 *	\return		cell's type
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline_			const char*		GetType()									const		{ return m__Type;						}

		// Flags
		inline_			udword			GetCellFlag()								const		{ return m__Flags;						}
		inline_			bool			IsSet(CellFlag flag)						const		{ return (m__Flags&flag)!=0;			}
		inline_			Cell&			Enable(CellFlag flag)									{ m__Flags |= flag;		return *this;	}
		inline_			Cell&			Disable(CellFlag flag)									{ m__Flags &= ~flag;	return *this;	}

		//! Shared data blocks [experimental - don't use]
						DataBlock*		CreateDataBlock(udword length);
						DataBlock*		ShareDataBlock(DataBlock* block);
		// Naming
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Sets the cell's name. You can override it to impose particular naming conventions.
		 *	\param		name	[in] cell's name
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			bool			SetName(const char* name);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets the cell's name.
		 *	\param		name	[out] cell's name
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline_			void			GetName(String& name)						const		{ name = m__Name;						}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets the cell's name.
		 *	\return		cell's name
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline_			const String&	GetName()									const		{ return m__Name;						}

		// Destruction
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Checks a cell can be destroyed.
		 *	\return		true if the cell can be destroyed
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			bool			CanDestruct()								const		{ return GetNbOwners()==0;				}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	The self-destruction method. The kernel always deletes cells indirectly, using this method. That way the app has a way to
		 *	definitely prevent the kernel from deleting cells, by overriding this method. This should not be needed nonetheless.
		 *	\param		context		[in] destruction context
		 *	\param		user_data	[in] possible user-defined data
		 *	\return		true if the cell has been deleted
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			bool			SelfDestruct(udword context=0, void* user_data=0);

		//! TO BE DOCUMENTED
						Cell*			Clone(bool copymembers=true);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets the size of the object.
		 *	\return		size of object
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			udword			GetSize()									const		{ return SIZEOFOBJECT;					}

		// Notifications & events
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Registers a user-notification for this cell.
		 *	\param		notif_index	[in] notification index
		 *	\param		callback	[in] notification callback
		 *	\param		user_data	[in] user-defined data
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			bool			RegisterNotif(udword notif_index, NotifCallback callback, void* user_data);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Validates an event for a given notification.
		 *	\param		notif_index	[in] notification index
		 *	\param		bit_mask	[in] event bitmask
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			bool			ValidateEvent(udword notif_index, udword bit_mask);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Invalidates an event for a given notification.
		 *	\param		notif_index	[in] notification index
		 *	\param		bit_mask	[in] event bitmask
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			bool			InvalidateEvent(udword notif_index, udword bit_mask);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Unregisters a user-notification for this cell.
		 *	\param		notif_index	[in] notification index
		 *	\param		callback	[in] notification callback
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			bool			RemoveNotif(udword notif_index, NotifCallback callback=null);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Unregisters all user-notifications for this cell.
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			bool			RemoveNotif();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Executes a user-notification.
		 *	\param		notif_index	[in] notification index
		 *	\param		bit_mask	[in] notification bitmask
		 *	\param		param		[in] a parameter sent to the notification callback
		 *	\param		result		[out] returned from notification callback
		 *	\return		true if a callback has been found
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool			ExecuteNotif(udword notif_index, udword bit_mask, udword param, bool& result);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets a priority. The higher the value, the less the priority.
		 *	\return		priority value
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			udword			GetPriority()											{ return 0;								}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Assigns a script to the object.
		 *	\param		filename	[in] script filename
		 *	\param		script_name	[in] possible script name
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			bool			AssignScript(const char* filename, const char* script_name);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Executes a given command.
		 *	\param		command		[in] the command to execute, or null. If null, EnumCommand gets called for each command found.
		 *	\param		user_data	[in] user-defined data, sent to EnumCommand if needed.
		 *	\return		true if success
		 *	\see		EnumCommand(const char* command, void* user_data)
		 *	\see		DispatchCommand(const char* command, void* user_data, bool refs)
		 *	\warning	No commands defined for Cell.
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			bool			ExecCommand(const char* command, void* user_data=null)	{ return false;							}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Callback used by ExecCommand to enumerate all registered commands.
		 *	\param		command		[in] current command found
		 *	\param		user_data	[in] user-defined data, coming from ExecCommand
		 *	\return		Self-reference
		 *	\see		ExecCommand(const char* command, void* user_data)
		 *	\see		DispatchCommand(const char* command, void* user_data, bool refs)
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			Cell&			EnumCommand(const char* command, void* user_data=null)	{ return *this;							}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Command dispatcher. Sends command to all references or all owners.
		 *	\param		command		[in] the command to send
		 *	\param		user_data	[in] user-defined data
		 *	\param		bool		[in] true to send the command to references, else sent to owners
		 *	\return		true if success
		 *	\see		ExecCommand(const char* command, void* user_data)
		 *	\see		EnumCommand(const char* command, void user_data)
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool			DispatchCommand(const char* command, void* user_data=null, bool refs=true);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Binds a named user-data to the object.
		 *	\param		data_name	[in] a string used as data identifier
		 *	\param		user_data	[in] user-defined data
		 *	\return		true if success
		 *	\see		RemoveData(const char* data_name, udword* user_data)
		 *	\see		GetData(const char* data_name, udword* user_data)
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool			BindData(const char* data_name, udword user_data);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Removes a named user-data from the object.
		 *	\param		data_name	[in] a string used as data identifier
		 *	\param		user_data	[out] retrieved user-defined data, if needed
		 *	\return		true if success
		 *	\see		BindData(const char* data_name, udword user_data)
		 *	\see		GetData(const char* data_name, udword* user_data)
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool			RemoveData(const char* data_name, udword* user_data=null);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets a named user-data using its string identifier
		 *	\param		data_name	[in] a string used as data identifier
		 *	\param		user_data	[out] retrieved user-defined data, if needed
		 *	\return		true if data has been found
		 *	\see		BindData(const char* data_name, udword user_data)
		 *	\see		RemoveData(const char* data_name, udword* user_data)
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool			GetData(const char* data_name, udword* user_data=null);

		private:
		// NB: the m__XXXX member naming convention is used so that you can reuse names like "mType" and "mFlags" in your
		// derived classes, in a non-ambiguous way.
						NotifBundle*	m__NB;			//!< Notification array
						KID				m__KernelID;	//!< Global unique kernel identifier
#ifdef USE_HANDLE_MANAGER
						udword			m__Flags;		//!< Cell flags
#else
						uword			m__Flags;		//!< Cell flags
#endif
						const char*		m__Type;		//!< Cell type
						String			m__Name;		//!< Cell name

						Constants*		m__UserData;	//!< Named user-data

		static			Kernel*			m__Kernel;		//!< Shared kernel shortcut

//		virtual			Cell*			CreateInstance()										{ return new Cell;						}

		friend			class			Kernel;
	};

#endif // ICECELL_H
