// Module:  Log4CPLUS
// File:    internal.h
// Created: 1/2009
// Author:  Vaclav Haisman
//
//
//   Copyright (C) 2009, Vaclav Haisman. All rights reserved.
//   
//   Redistribution and use in source and binary forms, with or without modifica-
//   tion, are permitted provided that the following conditions are met:
//   
//   1. Redistributions of  source code must  retain the above copyright  notice,
//      this list of conditions and the following disclaimer.
//   
//   2. Redistributions in binary form must reproduce the above copyright notice,
//      this list of conditions and the following disclaimer in the documentation
//      and/or other materials provided with the distribution.
//   
//   THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED WARRANTIES,
//   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
//   FITNESS  FOR A PARTICULAR  PURPOSE ARE  DISCLAIMED.  IN NO  EVENT SHALL  THE
//   APACHE SOFTWARE  FOUNDATION  OR ITS CONTRIBUTORS  BE LIABLE FOR  ANY DIRECT,
//   INDIRECT, INCIDENTAL, SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL  DAMAGES (INCLU-
//   DING, BUT NOT LIMITED TO, PROCUREMENT  OF SUBSTITUTE GOODS OR SERVICES; LOSS
//   OF USE, DATA, OR  PROFITS; OR BUSINESS  INTERRUPTION)  HOWEVER CAUSED AND ON
//   ANY  THEORY OF LIABILITY,  WHETHER  IN CONTRACT,  STRICT LIABILITY,  OR TORT
//   (INCLUDING  NEGLIGENCE OR  OTHERWISE) ARISING IN  ANY WAY OUT OF THE  USE OF
//   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/** @file 
 * This header contains declaration internal to log4cplus. They must never be
 * visible from user accesible headers or exported in DLL/shared libray.
 */


#ifndef LOG4CPLUS_INTERNAL_INTERNAL_HEADER_
#define LOG4CPLUS_INTERNAL_INTERNAL_HEADER_

#if ! defined (INSIDE_LOG4CPLUS)
#  error "This header must not be be used outside log4cplus' implementation files."
#endif

#include <memory>
#include <vector>
#include <sstream>
#include <log4cplus/config.hxx>
#include <log4cplus/tstring.h>
#include <log4cplus/streams.h>
#include <log4cplus/ndc.h>
#include <log4cplus/spi/loggingevent.h>
#include <log4cplus/loggingmacros.h>


namespace log4cplus {

namespace internal {


//! Canonical empty string. It is used when the need to return empty string
//! by reference arises.
extern log4cplus::tstring const empty_str;


struct gft_scratch_pad
{
    gft_scratch_pad ();
    ~gft_scratch_pad ();
    
    void
    reset ()
    {
        uc_q_str_valid = false;
        q_str_valid = false;
        s_str_valid = false;
        ret.clear ();
    }
    
    log4cplus::tstring q_str;
    log4cplus::tstring uc_q_str;
    log4cplus::tstring s_str;
    log4cplus::tstring ret;
    log4cplus::tstring fmt;
    log4cplus::tstring tmp;
    std::vector<tchar> buffer;
    bool uc_q_str_valid;
    bool q_str_valid;
    bool s_str_valid;
};


struct appender_sratch_pad
{
    appender_sratch_pad ();
    ~appender_sratch_pad ();

    tostringstream oss;
    tstring str;
};


//! Per thread data.
struct per_thread_data
{
    per_thread_data ();
    ~per_thread_data ();

    tostringstream macros_oss;
    DiagnosticContextStack ndc_dcs;
    log4cplus::tstring thread_name;
    gft_scratch_pad gft_sp;
    appender_sratch_pad appender_sp;
    log4cplus::tstring faa_str;
    spi::InternalLoggingEvent forced_log_ev;
    log4cplus::detail::macro_body_scratch_pad_type macro_body_scratch_pad;
};


per_thread_data * alloc_ptd ();


#if ! defined (LOG4CPLUS_SINGLE_THREADED)


// TLS key whose value is pointer struct per_thread_data.
extern LOG4CPLUS_THREAD_LOCAL_TYPE tls_storage_key;


#if defined (LOG4CPLUS_THREAD_LOCAL_VAR)

extern LOG4CPLUS_THREAD_LOCAL_VAR per_thread_data * ptd;


inline
void
set_ptd (per_thread_data * p)
{
    ptd = p;
}


//! The default value of the \param alloc is false for Win32 DLL builds
//! since per thread data are already initialized by DllMain().
inline
per_thread_data *
get_ptd (bool alloc
#if defined (_WIN32) && (defined (LOG4CPLUS_BUILD_DLL) || defined (log4cplus_EXPORTS))
         = false
#else
         = true
#endif
         )
{
    if (! ptd && alloc)
        return alloc_ptd ();

    return ptd;
}


#else // defined (LOG4CPLUS_THREAD_LOCAL_VAR)


inline
void
set_ptd (per_thread_data * p)
{
    LOG4CPLUS_SET_THREAD_LOCAL_VALUE (tls_storage_key, p);
}


inline
per_thread_data *
get_ptd (bool alloc = true)
{
    per_thread_data * ptd
        = reinterpret_cast<per_thread_data *>(
            LOG4CPLUS_GET_THREAD_LOCAL_VALUE (tls_storage_key));

    if (! ptd && alloc)
        return alloc_ptd ();

    return ptd;
}


#endif // defined (LOG4CPLUS_THREAD_LOCAL_VAR)

#else


extern std::auto_ptr<per_thread_data> ptd;


inline
void
set_ptd (per_thread_data * p)
{
    ptd.reset (p);
}


inline
per_thread_data *
get_ptd (bool alloc = true)
{
    if (! ptd.get () && alloc)
        return alloc_ptd ();

    return ptd.get ();
}


#endif // ! defined (LOG4CPLUS_SINGLE_THREADED)


inline
tstring &
get_thread_name_str ()
{
    return get_ptd ()->thread_name;
}


inline
gft_scratch_pad &
get_gft_scratch_pad ()
{
    return get_ptd ()->gft_sp;
}


inline
appender_sratch_pad &
get_appender_sp ()
{
    return get_ptd ()->appender_sp;
}


} // namespace internal {


namespace detail
{

LOG4CPLUS_EXPORT void clear_tostringstream (tostringstream &);

} // namespace detail


} // namespace log4cplus { 


#endif // LOG4CPLUS_INTERNAL_INTERNAL_HEADER_
