#ifndef SYSTEM_H
#define SYSTEM_H

#include "common.h"

namespace vm {

class Allocator {
 public:
  virtual ~Allocator() { }
  virtual void* tryAllocate(unsigned size) = 0;
  virtual void* allocate(unsigned size) = 0;
  virtual void free(const void*) = 0;
};

class System: public Allocator {
 public:
  typedef intptr_t Status;

  class Thread {
   public:
    virtual ~Thread() { }
    virtual void interrupt() = 0;
    virtual void join() = 0;
    virtual void dispose() = 0;
  };

  class Runnable {
   public:
    virtual ~Runnable() { }
    virtual void attach(Thread*) = 0;
    virtual void run() = 0;
    virtual bool interrupted() = 0;
    virtual void setInterrupted(bool v) = 0;
  };

  class Monitor {
   public:
    virtual ~Monitor() { }
    virtual bool tryAcquire(Thread* context) = 0;
    virtual void acquire(Thread* context) = 0;
    virtual void release(Thread* context) = 0;
    virtual bool wait(Thread* context, int64_t time) = 0;
    virtual void notify(Thread* context) = 0;
    virtual void notifyAll(Thread* context) = 0;
    virtual Thread* owner() = 0;
    virtual void dispose() = 0;
  };

  class Library {
   public:
    virtual ~Library() { }
    virtual void* resolve(const char* function) = 0;
    virtual const char* name() = 0;
    virtual Library* next() = 0;
    virtual void dispose() = 0;
  };

  virtual ~System() { }

  virtual bool success(Status) = 0;
  virtual Status attach(Runnable*) = 0;
  virtual Status start(Runnable*) = 0;
  virtual Status make(Monitor**) = 0;
  virtual uint64_t call(void* function, uintptr_t* arguments, uint8_t* types,
                        unsigned count, unsigned size,
                        unsigned returnType) = 0;
  virtual Status load(Library**, const char* name, Library* next) = 0;
  virtual void exit(int code) = 0;
  virtual void abort() = 0;
  virtual void dispose() = 0;

  virtual void* allocate(unsigned size) {
    void* p = tryAllocate(size);
    if (p == 0) {
      abort();
    }
    return p;
  }
};

inline void NO_RETURN
abort(System* s)
{
  s->abort(); // this should not return
  ::abort();
}

inline void NO_RETURN
sysAbort(System* s)
{
  abort(s);
}

inline void
expect(System* s, bool v)
{
  if (UNLIKELY(not v)) abort(s);
}

#ifdef NDEBUG

inline void
assert(System*, bool)
{ }

#else // not NDEBUG

inline void
assert(System* s, bool v)
{
  expect(s, v);
}

#endif // not NDEBUG

System*
makeSystem(unsigned heapSize);

} // namespace vm

#endif//SYSTEM_H
