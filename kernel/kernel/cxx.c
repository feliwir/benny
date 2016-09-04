// these calls are highly compiler specific, maybe look into this in the future

// pure virtual function, called when call to virtual function can't be
// performed
void __cxa_pure_virtual() {}

// following code is for constructors of global objects
typedef void (*constructor)();

// start_ctors and end_ctors is defined inside of our linker script
constructor start_ctors;
constructor end_ctors;

// called from assembly before kernel_main
void initialiseConstructors() {
  for (constructor *i = &start_ctors; i != &end_ctors; ++i)
    (*i)();
}

// following code is for destructors of global objects
void *__dso_handle;

struct object {
  void (*f)(void *);
  void *p;
  void *d;
} object[32] = {0};
unsigned int iObject = 0;

// calls the destructor of the object with parameters p
int __cxa_atexit(void (*f)(void *), void *p, void *d) {
  if (iObject >= 32)
    return -1;
  object[iObject].f = f;
  object[iObject].p = p;
  object[iObject].d = d;
  ++iObject;
  return 0;
}

// just destroy all objects
void __cxa_finalize(void *d) {
  unsigned int i = iObject;
  for (; i > 0; --i) {
    --iObject;
    object[iObject].f(object[iObject].p);
  }
}