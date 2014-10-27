#include <stdlib.h>
#include <stdio.h>
#include "gc.h"

VM *newVM()
{
	VM *vm = (VM *) malloc(sizeof(VM));
	vm->stackSize = 0;
	vm->firstObject = NULL;
	vm->numObjects = 0;
	vm->maxObjects = 10;
	return vm;
}

void push(VM *vm, Object *value)
{
	_assert(vm->stackSize < STACK_MAX, "Stack overflow!");
	vm->stack[vm->stackSize++] = value;
}

Object *pop(VM *vm)
{
	_assert(vm->stackSize > 0, "Stack underflow!");
	return vm->stack[--vm->stackSize];
}

Object *newObject(VM *vm, ObjectType type)
{
	Object *obj = NULL;
	
	if (vm->numObjects == vm->maxObjects)
	{
		gc(vm);
	}

	obj = (Object *) malloc(sizeof(Object));
	obj->type = type;
	obj->marked = 0;
	obj->next = vm->firstObject;
	vm->firstObject = obj;

	vm->numObjects++;
	return obj;
}

void pushInt(VM *vm, int intValue)
{
	Object *obj = newObject(vm, OBJ_INT);
	obj->value = intValue;
	push(vm, obj);
}

Object *pushPair(VM *vm)
{
	Object *obj = newObject(vm, OBJ_PAIR);
	obj->tail = pop(vm);
	obj->head = pop(vm);
	push(vm, obj);
	return obj;
}

void markAll(VM *vm)
{
	int i;
	for (i = 0; i < vm->stackSize; i++)
	{
		mark(vm->stack[i]);
	}
}

void mark(Object *obj)
{
	if (obj->marked)
	{
		return;
	}

	obj->marked = 1;

	if (obj->type == OBJ_PAIR)
	{
		mark(obj->head);
		mark(obj->tail);
	}
}

void sweep(VM *vm)
{
	Object **obj = &vm->firstObject;
	while (*obj)
	{
		if (!(*obj)->marked)
		{
			Object *unreached = *obj;
			*obj = unreached->next;
			free(unreached);

			vm->numObjects--;
		}
		else
		{
			(*obj)->marked = 0;
			obj = &(*obj)->next;
		}
	}
}

void gc(VM *vm)
{
	int numObjects = vm->numObjects;

	markAll(vm);
	sweep(vm);

	vm->maxObjects = vm->numObjects * 2;
}

void freeVM(VM *vm)
{
	vm->stackSize = 0;
	gc(vm);
	free(vm);
}

void _assert(int condition, const char* message)
{
	if (!condition) 
	{
		printf("%s\n", message);
		exit(1);
	}
}