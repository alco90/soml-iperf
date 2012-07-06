/*
 * Iperf reporter using the OML library.
 * Copyright (c) 2010, Nicta, Olivier Mehani <olivier.mehani@nicta.com.au>
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
	OmlValueU v[5];

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

	omlc_set_uint32(v[0], OML_main_iperf_pid);
	omlc_set_string(v[1], IPERF_VERSION);
	omlc_set_string(v[2], cmdline);
	omlc_set_uint32(v[3], (uint32_t) tv.tv_sec);
	omlc_set_uint32(v[4], (uint32_t) tv.tv_usec);
	omlc_inject(g_oml_mps->application, v);
}

void OML_inject_connection(int ID, const char *local_addr, int local_port,
		const char *remote_addr, int remote_port) {
	OmlValueU v[6];

	omlc_set_uint32(v[0], OML_main_iperf_pid);
	omlc_set_uint32(v[1], ID);
	omlc_set_const_string(v[2], (char*) local_addr);
	omlc_set_uint32(v[3], local_port);
	omlc_set_const_string(v[4], (char*) remote_addr);
	omlc_set_uint32(v[5], remote_port);

	omlc_inject(g_oml_mps->connection, v);
}

void OML_inject_settings(int server_mode, const char *bind_addr, int multicast, int mcast_ttl,
		int proto, int window_size, int buffer_size) {
	OmlValueU v[8];

	omlc_set_uint32(v[0], OML_main_iperf_pid);
	omlc_set_uint32(v[1], server_mode);
	omlc_set_const_string(v[2], (char*) bind_addr);
	omlc_set_uint32(v[3], multicast);
	omlc_set_uint32(v[4], mcast_ttl);
	omlc_set_uint32(v[5], proto);
	omlc_set_uint32(v[6], window_size);
	omlc_set_uint32(v[7], buffer_size);

	omlc_inject(g_oml_mps->settings, v);
}

void OML_inject_transfer(int ID, double begin_interval, double end_interval, max_size_t size) {
	OmlValueU v[5];

	omlc_set_uint32(v[0], OML_main_iperf_pid);
	omlc_set_uint32(v[1], ID);
	omlc_set_double(v[2], begin_interval);
	omlc_set_double(v[3], end_interval);
	omlc_set_uint64(v[4], size);

	omlc_inject(g_oml_mps->transfer, v);
}

void OML_inject_losses(int ID, double begin_interval, double end_interval,
		int total_datagrams, int lost_datagrams) {
	OmlValueU v[6];

	omlc_set_uint32(v[0], OML_main_iperf_pid);
	omlc_set_uint32(v[1], ID);
	omlc_set_double(v[2], begin_interval);
	omlc_set_double(v[3], end_interval);
	omlc_set_uint32(v[4], total_datagrams);
	omlc_set_uint32(v[5], lost_datagrams);

	omlc_inject(g_oml_mps->losses, v);
}

void OML_inject_jitter(int ID, double begin_interval, double end_interval, double jitter) {
	OmlValueU v[5];

	omlc_set_uint32(v[0], OML_main_iperf_pid);
	omlc_set_uint32(v[1], ID);
	omlc_set_double(v[2], begin_interval);
	omlc_set_double(v[3], end_interval);
	omlc_set_double(v[4], jitter);

	omlc_inject(g_oml_mps->jitter, v);
}
	
void OML_inject_packets(int ID, int packetID, int packetLen,
			time_t receivedtime_s, suseconds_t receivedtime_us,
			time_t senttime_s, suseconds_t senttime_us) {
	OmlValueU v[8];	

	omlc_set_uint32(v[0], OML_main_iperf_pid);
	omlc_set_uint32(v[1], ID);
	omlc_set_uint32(v[2], packetID);
	omlc_set_uint32(v[3], packetLen);
	omlc_set_uint32(v[4], receivedtime_s);
	omlc_set_uint32(v[5], receivedtime_us);
	omlc_set_uint32(v[6], senttime_s);
	omlc_set_uint32(v[7], senttime_us);

	omlc_inject(g_oml_mps->packets, v);
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

	OML_inject_connection(ID, local_addr, local_port, remote_addr, remote_port);

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


    OML_inject_settings(server_mode, bind_addr, multicast, multicast?mcast_ttl:0, proto, 
		    window_size, buffer_size);
}

void OML_stats(Transfer_Info *stats) {
	/* Skip summary stats at the end when an interval has been defined */
	if (interval <= 0. || (stats->endTime - stats->startTime <= interval)) {
		OML_inject_transfer(stats->transferID, stats->startTime, stats->endTime,
				stats->TotalLen);
		/* This should really be conditionned by whether the transport is
		 * - unreliable and,
		 * - datagram-oriented...
		 */
		if (stats->mUDP == (char)kMode_Server) {
			OML_inject_losses(stats->transferID, stats->startTime, stats->endTime,
					stats->cntDatagrams, stats->cntError);
			OML_inject_jitter(stats->transferID, stats->startTime, stats->endTime,
					stats->jitter * 1000.0);
		}
	}

}

void OML_serverstats(Connection_Info *conn, Transfer_Info *stats) {
	OML_inject_transfer(stats->transferID, stats->startTime, stats->endTime,
			stats->TotalLen);
	OML_inject_losses(stats->transferID, stats->startTime, stats->endTime,
			stats->cntDatagrams, stats->cntError);
	OML_inject_jitter(stats->transferID, stats->startTime, stats->endTime,
			stats->jitter * 1000.0);
}

void OML_handle_packet(Transfer_Info *stats, ReportStruct *packet) {
	OML_inject_packets(stats->transferID, packet->packetID, packet->packetLen,
			packet->packetTime.tv_sec, packet->packetTime.tv_usec,
			packet->sentTime.tv_sec, packet->sentTime.tv_usec);
}

