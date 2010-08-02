// kake2 -- http://code.google.com/p/kake2
// Copyright (c) 2010 Kenton Varda and contributors.  All rights reserved.
// Portions copyright Google, Inc.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of the kake2 project nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef KAKE2_EVENTGROUP_H_
#define KAKE2_EVENTGROUP_H_

#include <tr1/unordered_set>

#include "EventManager.h"

namespace kake2 {

class EventGroup: public EventManager {
public:
  class ExceptionHandler {
  public:
    virtual ~ExceptionHandler();

    virtual void threwException(const std::exception& e) = 0;
    virtual void threwUnknownException() = 0;
  };

  EventGroup(EventManager* inner, ExceptionHandler* exceptionHandler);
  ~EventGroup();

  void cancelAll();

  // implements EventManager -------------------------------------------------------------
  void runAsynchronously(OwnedPtr<Callback>* callbackToAdopt);
  void waitPid(pid_t process, OwnedPtr<ProcessExitCallback>* callbackToAdopt,
               OwnedPtr<Canceler>* output = NULL);
  void read(int fd, void* buffer, int size, OwnedPtr<IoCallback>* callbackToAdopt,
            OwnedPtr<Canceler>* output = NULL);
  void readAll(int fd, void* buffer, int size, OwnedPtr<IoCallback>* callbackToAdopt,
               OwnedPtr<Canceler>* output = NULL);
  void write(int fd, const void* buffer, int size, OwnedPtr<IoCallback>* callbackToAdopt,
             OwnedPtr<Canceler>* output = NULL);
  void writeAll(int fd, const void* buffer, int size, OwnedPtr<IoCallback>* callbackToAdopt,
                OwnedPtr<Canceler>* output = NULL);
  void readContinuously(int fd, OwnedPtr<ContinuousReadCallback>* callbackToAdopt,
                        OwnedPtr<Canceler>* output = NULL);

private:
  class CancelerWrapper;
  class CallbackWrapper;
  class ProcessExitCallbackWrapper;
  class IoCallbackWrapper;
  class ContinuousReadCallbackWrapper;

  struct CallbackContext {
    bool groupCanceled;
    OwnedPtr<Canceler> canceler;

    CallbackContext() : groupCanceled(false) {}
  };

  EventManager* inner;
  ExceptionHandler* exceptionHandler;
  std::tr1::unordered_set<CallbackContext*> activeCallbacks;
};

}  // namespace kake2

#endif  // KAKE2_EVENTGROUP_H_
