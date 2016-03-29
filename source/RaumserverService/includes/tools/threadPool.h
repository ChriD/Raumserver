//
// The MIT License (MIT)
//
// Copyright (c) 2016 by ChriD
//
// Permission is hereby granted, free of charge,  to any person obtaining a copy of
// this software and  associated documentation  files  (the "Software"), to deal in
// the  Software  without  restriction,  including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software,  and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this  permission notice  shall be included in all
// copies or substantial portions of the Software.
//
// THE  SOFTWARE  IS  PROVIDED  "AS IS",  WITHOUT  WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE  LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY, WHETHER
// IN  AN  ACTION  OF  CONTRACT,  TORT  OR  OTHERWISE,  ARISING  FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//


#pragma once
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <memory>


class CThreadPool
{
    public:

        template <typename T>
        static void QueueUserWorkItem(void (T::*function)(void), T *object, ULONG flags = WT_EXECUTELONGFUNCTION)
        {
            typedef std::pair<void (T::*)(), T *> CallbackType;
            std::auto_ptr<CallbackType> p(new CallbackType(function, object));

            if (::QueueUserWorkItem(ThreadProc<T>, p.get(), flags))
            {
                // The ThreadProc now has the responsibility of deleting the pair.
                p.release();
            }
            else
            {
                throw GetLastError();
            }
        }

    private:

        template <typename T>
        static DWORD WINAPI ThreadProc(PVOID context)
        {
            typedef std::pair<void (T::*)(), T *> CallbackType;

            std::auto_ptr<CallbackType> p(static_cast<CallbackType *>(context));

            (p->second->*p->first)();
            return 0;
        }
};


#endif