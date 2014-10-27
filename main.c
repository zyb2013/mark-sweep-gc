
#include <stdlib.h>
#include <stdio.h>
#include "gc.h"

void test1() {
	VM* vm = NULL;
	printf("Test 1: Objects on stack are preserved.\n");
	vm = newVM();
	pushInt(vm, 1);
	pushInt(vm, 2);

	gc(vm);
	_assert(vm->numObjects == 2, "Should have preserved objects.");
	freeVM(vm);
}

void test2() {
	VM* vm = NULL;
	printf("Test 2: Unreached objects are collected.\n");
	vm = newVM();
	pushInt(vm, 1);
	pushInt(vm, 2);
	pop(vm);
	//pop(vm);

	gc(vm);
	_assert(vm->numObjects == 0, "Should have collected objects.");
	freeVM(vm);
}

void test3() {
	VM* vm = NULL;
	printf("Test 3: Reach nested objects.\n");
	vm = newVM();
	pushInt(vm, 1);
	pushInt(vm, 2);
	pushPair(vm);
	pushInt(vm, 3);
	pushInt(vm, 4);
	pushPair(vm);
	pushPair(vm);

	gc(vm);
	_assert(vm->numObjects == 7, "Should have reached objects.");
	freeVM(vm);
}

void test4() {
	VM* vm = NULL;
	Object *a = NULL;
	Object *b = NULL;
	printf("Test 4: Handle cycles.\n");
	vm = newVM();
	pushInt(vm, 1);
	pushInt(vm, 2);
	a = pushPair(vm);
	pushInt(vm, 3);
	pushInt(vm, 4);
	b = pushPair(vm);

	a->tail = b;
	b->tail = a;

	gc(vm);
	_assert(vm->numObjects == 4, "Should have collected objects.");
	freeVM(vm);
}

void perfTest() {
	int i;
	int j;
	int k;
	VM* vm = NULL;
	printf("Performance Test.\n");
	vm = newVM();

	for (i = 0; i < 1000; i++) {
		for (j = 0; j < 20; j++) {
			pushInt(vm, i);
		}

		for (k = 0; k < 20; k++) {
			pop(vm);
		}
	}
	freeVM(vm);
}

int main() {
	//test1(); 
	test2();
	//test3();
	//test4();
	//perfTest();

	return 0;
}