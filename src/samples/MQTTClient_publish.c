/*******************************************************************************
 * Copyright (c) 2012, 2013 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution. 
 *
 * The Eclipse Public License is available at 
 *   http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at 
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial contribution
 *******************************************************************************/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"
#include <unistd.h>
#include <sys/time.h>

#define ADDRESS     "tcp://10.3.5.48:1883"
#define CLIENTID    "JulienEtBlaise"
#define TOPIC       "DATE"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

int main(int argc, char* argv[])
{

    int sec, usec;

    printf("\nTimestamp\n");
    struct timeval NTP_value;

    

    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    while(1==1){
	// that value is since 1970.
	    gettimeofday(&NTP_value, NULL);
	    sec = NTP_value.tv_sec;
	    usec = NTP_value.tv_usec;

	    printf("\n%d\n",sec);

	    char str[10];
	    sprintf(str, "%d", sec);
	    pubmsg.payload = str;
	    pubmsg.payloadlen = strlen(str);
	    pubmsg.qos = QOS;
	    pubmsg.retained = 0;
	    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
	    printf("Waiting for up to %d seconds for publication of %s\n"
		    "on topic %s for client with ClientID: %s\n",
		    (int)(TIMEOUT/1000), str, TOPIC, CLIENTID);
	    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
	    printf("Message with delivery token %d delivered\n", token);
            sleep(1);
	}
//end while
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;

}
