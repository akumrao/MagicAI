/* This file is part of mediaserver. A webrtc sfu server.
 * Copyright (C) 2018 Arvind Umrao <akumrao@yahoo.com> & Herman Umrao<hermanumrao@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include "net/netInterface.h"
#include "net/SslConnection.h"
#include "base/logger.h"
//#include "net/sslmanager.h"
#include <assert.h>

using namespace std;


namespace base {
namespace net {


SslConnection::SslConnection()
    : TcpConnectionBase(nullptr, true)
//    , _sslContext(nullptr)
//    , _sslSession(nullptr)
    , _sslAdapter(this)
  
{
    LTrace("Create")
     _sslAdapter.initSSL();
 
}


SslConnection::SslConnection( bool server)
    : TcpConnectionBase(nullptr, true)
  //  , _sslSession(nullptr)
    , _sslAdapter(this)
    ,serverMode(server)
{
    if(server)
        _sslAdapter.server = server;
        _sslAdapter.initSSL();
    LTrace("Create")
}

/*
SslConnection::SslConnection(Listener* listener, SSLContext::Ptr context, SSLSession::Ptr session)
    :  TcpConnection(listener)
    , _sslContext(context)
    , _sslSession(session)
    , _sslAdapter(this)
    ,listener(listener)
{
    LTrace("Create")
}
*/

SslConnection::~SslConnection()
{
    _sslAdapter.shutdown();
    LTrace("Destroy")
}



void SslConnection::send(const char* data, size_t len)
{
    SInfo << "Send " <<  data   << " len "  << len;
    
    if (len > MBEDTLS_SSL_MAX_CONTENT_LEN)
    {
        SError <<  "encode data is too large" ;
        return;
    }


    size_t rv = (size_t)mbedtls_ssl_write( &_sslAdapter._ssl,(const unsigned char *) data, len);

    size_t pending = 0;
    
    char *encoded_data = nullptr;
    
    
    if( (pending = BIO_ctrl_pending(_sslAdapter.app_bio_) ) > 0 ) {

        encoded_data = (char*)malloc(pending);
       // encoded_data.len = pending;

        rv = BIO_read(_sslAdapter.app_bio_, encoded_data , pending);
       // data2encode->len = rv;
        //assert(rv == len);
        Write(  encoded_data, rv , _sslAdapter.cb);
        _sslAdapter.cb = nullptr;
        free(encoded_data);
    }
    else
    {
        SError <<  "SSL Error Encoding" ;
    }
    //return encoded_data;
    

    
    return ;
}


void SslConnection::tcpsend(const char* data, size_t len, onSendCallback _cb)
{
//     assert(_sslAdapter._ssl);
    _sslAdapter.cb = _cb;
    send( data, len);
    return ;
}
//
// Callbacks

void SslConnection::on_tls_read(const char* data, size_t len)
{
    SInfo << "on_tls_read: " << len << " data"  <<  data ; 

    
    BIO_write( _sslAdapter.app_bio_,data , len);

   
    _sslAdapter.addIncomingData(data, len);
    
    
//    int ret =0;
//    while( ( ret = mbedtls_ssl_write( & _sslAdapter._ssl, (const unsigned char*) data, len ) ) <= 0 )
//    {
//        if( ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE )
//        {
//            SError <<  " failed\n  ! mbedtls_ssl_write failed " <<  ret ;
//            break;
//        }
//    }
    
}

void SslConnection::on_read(const char* data, size_t len)
{
   // LTrace("SslConnection::on_read ", len)
   // LTrace("SslConnection::on_read ", data)

//    listener->on_read(this,data,len );
    
    throw(" Never code should reach here, please override this function");
  
}

void SslConnection::on_connect()
{
    LTrace("SSL On connect")
 
    _sslAdapter.handshake();        
}


} // namespace net
} // namespace base

