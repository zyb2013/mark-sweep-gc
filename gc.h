
#define STACK_MAX 256

typedef enum
{
	OBJ_INT,
	OBJ_PAIR
} ObjectType;


typedef struct sObject
{
	ObjectType type;
	unsigned char marked;
	struct sObject *next;
	union
	{
		int value;

		struct 
		{
			struct sObject *head;
			struct sObject *tail;
		};
	};
} Object;

typedef struct 
{
	Object *stack[STACK_MAX];
	Object *firstObject;
	int stackSize;
	int numObjects;
	int maxObjects;
} VM;

VM *newVM();

void push(VM *vm, Object *value);

Object *pop(VM *vm);

Object *newObject(VM *vm, ObjectType type);

void pushInt(VM *vm, int intValue);

Object *pushPair(VM *vm);

void markAll(VM *vm);

void mark(Object *obj);

void sweep(VM *vm);

void gc(VM *vm);

void freeVM(VM *vm);

void _assert(int condition, const char* message);