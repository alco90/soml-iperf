/*
 * Iperf reporter using the OML library.
 * Copyright (c) 2010-2013, Nicta, Olivier Mehani <olivier.mehani@nicta.com.au>
 * All rights reserved.
 *
 * $Id: ReportOML.patch 1273 2010-04-28 09:20:33Z omehani $
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of Nicta nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <oml2/omlc.h>
#include <stdint.h>
#include "headers.h"
#include "Settings.hpp"
#include "SocketAddr.h"
#include "Reporter.h"
#include "Locale.h"
#include "version.h"
#define OML_FROM_MAIN
#include "report_OML.h"

// YUCK!
// Only valid in this scope, though...
static pid_t OML_main_iperf_pid;
// Does it have to be this way?
static double interval;

int OML_init(int *argc, const char **argv) {
	OML_main_iperf_pid = getpid();
	return omlc_init("iperf", argc,  argv, NULL);
}

int OML_set_measurement_points(thread_Settings *mSettings) {
	interval = mSettings->mInterval;
	oml_register_mps();
	return omlc_start();
}

void OML_inject_application(int argc, char **argv) {
	int i, cmdlen=argc;
	char* cmdline;
	struct timeval tv;

	gettimeofday(&tv, NULL);

	for (i=0; i<argc; i++)
		cmdlen += strlen(argv[i]);

	cmdline = malloc(cmdlen+1);
	if (cmdline) {
		cmdline[0] = '\0';
		for (i=0; i<argc; i++) {
			if (i>0) strcat(cmdline, " ");
			strcat(cmdline, argv[i]);
		}
	}
	oml_inject_application(g_oml_mps->application,
			OML_main_iperf_pid,
			IPERF_VERSION,
			cmdline,
			(uint32_t) tv.tv_sec,
			(uint32_t) tv.tv_usec);
}
void *OML_peer(Connection_Info *stats, int ID) {
	/*
	 * It is somewhat better to store address strings statically in this
	 * function rather than somewhere unknown in the libc. This will have
	 * to do until omlc_set_const_string() makes copies or the entire
	 * matter is handled differently.
	 */
	static char local_addr[REPORT_ADDRLEN], remote_addr[REPORT_ADDRLEN];
	int local_port, remote_port;

	/* YUCK! (cf. include/headers.h) */
	inet_ntop(
#ifdef HAVE_IPV6
			((struct sockaddr_storage) stats->local).ss_family,
			SockAddr_isIPv6(&stats->local)?
				(void *)SockAddr_get_in6_addr(&stats->local):
#else
			((struct sockaddr_in) stats->local).sin_family,
#endif
			(void *)SockAddr_get_in_addr(&stats->local),
			local_addr, REPORT_ADDRLEN);
	inet_ntop(
#ifdef HAVE_IPV6
			((struct sockaddr_storage) stats->peer).ss_family,
			SockAddr_isIPv6(&stats->peer)?
				(void *)SockAddr_get_in6_addr(&stats->peer):
#else
			((struct sockaddr_in) stats->peer).sin_family,
#endif
				(void *)SockAddr_get_in_addr(&stats->peer),
			remote_addr, REPORT_ADDRLEN);

	local_port = SockAddr_getPort(&stats->local);
	remote_port = SockAddr_getPort(&stats->peer);

	oml_inject_connection(g_oml_mps->connection,
			OML_main_iperf_pid,
			ID,
			local_addr,
			local_port,
			remote_addr,
			remote_port);

	return NULL;
}

void OML_settings( ReporterData *data ) {
    int server_mode = (data->mThreadMode == kMode_Listener ? 0 : 1); //XXX
    int multicast = SockAddr_isMulticast( &data->connection.local );
    int mcast_ttl = multicast?0:data->info.mTTL;
    int proto = IPPROTO_TCP;
    int window_size = getsock_tcp_windowsize( data->info.transferID, server_mode );
    int buffer_size = data->mBufLen;
    static char bind_addr[REPORT_ADDRLEN];

    if ( data->mLocalhost != NULL ) {
        strncpy( (char*) &bind_addr, data->mLocalhost, REPORT_ADDRLEN);
	bind_addr[REPORT_ADDRLEN-1] = '\0';
    } else {
        strncpy( (char*) &bind_addr, "0.0.0.0", REPORT_ADDRLEN);
    }

    if ( isUDP( data ) ) {
	proto = IPPROTO_UDP;
    } else { // Assume TCP...
	// Window size different than requested
	if ( data->mTCPWin > 0 && data->mTCPWin != window_size) {
	    byte_snprintf( buffer, sizeof(buffer), data->mTCPWin,
			    toupper( data->info.mFormat));
	    printf( warn_window_requested_OML, buffer );
	}
    }


    oml_inject_settings(g_oml_mps->settings,
		    OML_main_iperf_pid,
		    server_mode,
		    bind_addr,
		    multicast,
		    multicast?mcast_ttl:0,
		    proto,
		    window_size, buffer_size);
}

void OML_stats(Transfer_Info *stats) {
	/* Skip summary stats at the end when an interval has been defined */
	if (interval <= 0. || (stats->endTime - stats->startTime <= interval)) {
		oml_inject_transfer(g_oml_mps->transfer,
				OML_main_iperf_pid,
				stats->transferID,
				stats->startTime,
				stats->endTime,
				stats->TotalLen);
		/* This should really be conditionned by whether the transport is
		 * - unreliable and,
		 * - datagram-oriented...
		 */
		if (stats->mUDP == (char)kMode_Server) {
			oml_inject_losses(g_oml_mps->losses,
					OML_main_iperf_pid,stats->transferID,
					stats->startTime,
					stats->endTime,
					stats->cntDatagrams, stats->cntError);

			oml_inject_jitter(g_oml_mps->jitter,
					OML_main_iperf_pid,
					stats->transferID,
					stats->startTime,
					stats->endTime,
					stats->jitter * 1000.0);
		}
	}

}

void OML_serverstats(Connection_Info *conn, Transfer_Info *stats) {
	oml_inject_transfer(g_oml_mps->transfer,
			OML_main_iperf_pid,stats->transferID,
			stats->startTime,
			stats->endTime,
			stats->TotalLen);
	oml_inject_losses(g_oml_mps->losses,
			OML_main_iperf_pid,
			stats->transferID,
			stats->startTime,
			stats->endTime,
			stats->cntDatagrams,
			stats->cntError);
	oml_inject_jitter(g_oml_mps->jitter,
			OML_main_iperf_pid,
			stats->transferID,
			stats->startTime,
			stats->endTime,
			stats->jitter * 1000.0);
}

void OML_handle_packet(Transfer_Info *stats, ReportStruct *packet) {
	oml_inject_packets(g_oml_mps->packets,
			OML_main_iperf_pid,
			stats->transferID,
			packet->packetID,
			packet->packetLen,
			packet->packetTime.tv_sec,
			packet->packetTime.tv_usec,
			packet->sentTime.tv_sec,
			packet->sentTime.tv_usec);
}

