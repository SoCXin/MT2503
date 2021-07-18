
#include <string.h>

#include "aliot_platform.h"
#include "aliot_net.h"
#include "aliot_log.h"
#include "aliot_hexdump.h"


/*** TCP connection ***/
int read_tcp(aliot_network_pt pNetwork, char *buffer, uint32_t len, uint32_t timeout_ms)
{
    return aliot_platform_tcp_read(pNetwork->handle, buffer, len, timeout_ms);
}


static int write_tcp(aliot_network_pt pNetwork, const char *buffer, uint32_t len, uint32_t timeout_ms)
{
    return aliot_platform_tcp_write(pNetwork->handle, buffer, len, timeout_ms);
}

static int disconnect_tcp(aliot_network_pt pNetwork)
{
    if ( NULL == pNetwork) {
        return -1;
    }

    aliot_platform_tcp_destroy(pNetwork->handle);
    pNetwork->handle = 0;
    return 0;
}


static int connect_tcp(aliot_network_pt pNetwork)
{
    if (NULL == pNetwork) {
        ALIOT_LOG_ERROR("network is null");
        return 1;
    }

    pNetwork->handle = aliot_platform_tcp_establish(pNetwork->pHostAddress, pNetwork->port);
    if (NULL == pNetwork->handle) {
		disconnect_tcp(pNetwork);
        return -1;
    }

    return 0;
}


/*** SSL connection ***/

static int read_ssl(aliot_network_pt pNetwork, char *buffer, uint32_t len, uint32_t timeout_ms)
{
    if (NULL == pNetwork) {
        ALIOT_LOG_ERROR("network is null");
        return -1;
    }

    return (uintptr_t)aliot_platform_ssl_read((uintptr_t)pNetwork->handle, buffer, len, timeout_ms);
}

static int write_ssl(aliot_network_pt pNetwork, const char *buffer, uint32_t len, uint32_t timeout_ms)
{
    if (NULL == pNetwork) {
        ALIOT_LOG_ERROR("network is null");
        return -1;
    }

    return aliot_platform_ssl_write((uintptr_t)pNetwork->handle, buffer, len, timeout_ms);
}

static int disconnect_ssl(aliot_network_pt pNetwork)
{
    if (NULL == pNetwork) {
        ALIOT_LOG_ERROR("network is null");
        return -1;
    }

    aliot_platform_ssl_destroy((uintptr_t)pNetwork->handle);
    pNetwork->handle = 0;

    return 0;
}

static int connect_ssl(aliot_network_pt pNetwork)
{
    if (NULL == pNetwork) {
        ALIOT_LOG_ERROR("network is null");
        return 1;
    }

    if (0 != (pNetwork->handle = (intptr_t)aliot_platform_ssl_establish(
                                            pNetwork->pHostAddress,
                                            pNetwork->port,
                                            pNetwork->ca_crt,
                                            pNetwork->ca_crt_len + 1))) {
        return 0;
    } else {
        //TODO SHOLUD not remove this handle space
        // The space will be freed by calling disconnect_ssl()
        //aliot_memory_free((void *)pNetwork->handle);
        disconnect_ssl(pNetwork);
        return -1;
    }
}



/****** network interface ******/

int aliot_net_read(aliot_network_pt pNetwork, char *buffer, uint32_t len, uint32_t timeout_ms)
{
    int rc = -1;
    if (NULL == pNetwork->ca_crt) { //TCP connection
        rc = read_tcp(pNetwork, buffer, len, timeout_ms);
    } else { //SSL connection
        rc = read_ssl(pNetwork, buffer, len, timeout_ms);
    }
    ALIOT_LOG_HEXDUMP("aliot_net_read", buffer, len);
    return rc;
}


int aliot_net_write(aliot_network_pt pNetwork, const char *buffer, uint32_t len, uint32_t timeout_ms)
{
    ALIOT_LOG_HEXDUMP("aliot_net_write", (char*)buffer, len);
    if (NULL == pNetwork->ca_crt) { //TCP connection
        return write_tcp(pNetwork, buffer, len, timeout_ms);
    } else { //SSL connection
        return write_ssl(pNetwork, buffer, len, timeout_ms);
    }
}


int aliot_net_disconnect(aliot_network_pt pNetwork)
{
    if (NULL == pNetwork->ca_crt) { //TCP connection
        return disconnect_tcp(pNetwork);
    } else { //SSL connection
        return disconnect_ssl(pNetwork);
    }
}


int aliot_net_connect(aliot_network_pt pNetwork)
{
    if (NULL == pNetwork->ca_crt) { //TCP connection
        return connect_tcp(pNetwork);
    } else { //SSL connection
        return connect_ssl(pNetwork);
    }
}


int aliot_net_init(aliot_network_pt pNetwork, const char *host, uint16_t port, const char *ca_crt)
{
    pNetwork->pHostAddress = host;
    pNetwork->port = port;
    pNetwork->ca_crt = ca_crt;
    if (NULL == ca_crt) {
        pNetwork->ca_crt_len = 0;
    } else {
        pNetwork->ca_crt_len = STRLEN(ca_crt);
    }

    pNetwork->handle = 0;
    pNetwork->read = aliot_net_read;
    pNetwork->write = aliot_net_write;
    pNetwork->disconnect = aliot_net_disconnect;
    pNetwork->connect = aliot_net_connect;

    return 0;
}
