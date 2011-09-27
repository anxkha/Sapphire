#include <sddk.h>
#include "mutex.h"

VOID
PsMutexAcquire( PS_MUTEX* pMutex )
{
	ASSERT( pMutex );

	while( pMutex->Locked );

	pMutex->Locked = TRUE;
}

VOID
PsMutexRelease( PS_MUTEX* pMutex )
{
	ASSERT( pMutex );

	pMutex->Locked = FALSE;
}

STATUS
PsMutexCreate( PS_MUTEX** ppMutex )
{
	STATUS result = STATUS_SUCCESS;
	PS_MUTEX* pNewMutex = NULL;

	if( !ppMutex )
	{
		result = STATUS_INVALID_PARAMETER;
		goto done;
	}

	pNewMutex = (PS_MUTEX*)kmalloc( sizeof(PS_MUTEX), MM_TYPE_KERNEL );
	if( !pNewMutex )
	{
		result = STATUS_OUT_OF_MEMORY;
		goto done;
	}

	pNewMutex->Locked = FALSE;

	*ppMutex = pNewMutex;

done:
	return result;
}

STATUS
PsMutexDestroy( PS_MUTEX* pMutex )
{
	STATUS result = STATUS_SUCCESS;

	if( !pMutex )
	{
		result = STATUS_INVALID_PARAMETER;
		goto done;
	}

	if( pMutex->Locked )
	{
		result = STATUS_IN_USE;
		goto done;
	}

	kfree( pMutex, MM_TYPE_KERNEL );

done:
	return result;
}
