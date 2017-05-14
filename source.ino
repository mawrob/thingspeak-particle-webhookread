/*

// Add the following Webhook to your account for this to work - see: https://docs.particle.io/guide/tools-and-features/webhooks/

{
"eventName": "TSReadHooks",
"url": "https://api.thingspeak.com/channels/{{c}}/feeds/last.json?results=1&status=true&location=true&api_key={{k}}",
"requestType": "GET",
"headers": null,
"query": null,
"responseTemplate": "{{created_at}}~{{entry_id}}~{{field1}}~{{field2}}~{{field3}}~{{field4}}~{{field5}}~{{field6}}~{{field7}}~{{field8}}~{{latitude}}~{{longitude}}~{{elevation}}~{{status}}",
"json": null,
"auth": null,
"mydevices": true
}

*/

/*
The responseTemplate above parses the following json response from ThingSpeak

{
created_at: "2016-03-25T15:53:24Z",
entry_id: 1581,
field1: "341",
field2: "0.1",
field3: "93",
field4: "42.3",
field5: "0.0",
field6: "100.0",
field7: "4.0",
field8: "61",
latitude: null,
longitude: null,
elevation: "",
status: ""
}

To the following tilde delimited string

2016-03-25T15:53:24Z~1581~341~0.1~93~42.3~0.0~100.0~4.0~61~~~~
 

*/


#define HOOK_RESP	"hook-response/TSReadHooks"
#define HOOK_PUB	"TSReadHooks"

String api_key = ""; // Replace this string with a valid ThingSpeak Read API Key if using a private channel
String channel = ""; // Replace this string with a valid ThingSpeak Channel number
unsigned long readPeriod = 30000;
unsigned long lastRead;
int tildeIndexBegin;
int tildeIndexEnd;
String TSdata;

// List of ThingSpeak Channel Feed Field names
String feedName[] = {"created_at","entry_id","field1","field2","field3","field4","field5","field6","field7","field8","latitude","elevation","status"};

String feedValues[] = {"","","","","","","","","","","","",""};

void setup() {
	Serial.begin(9600);
	Particle.subscribe(HOOK_RESP, ThingSpeakResponse, MY_DEVICES);
	lastRead = millis();
}

void loop(){
	if(millis()-lastRead>readPeriod){
		Particle.publish("TSReadHooks","{\"k\":\""+api_key+"\",\"c\":\""+channel+"\"}",60,PRIVATE);
		lastRead = millis();
	}
}


void ThingSpeakResponse(const char *name, const char *data) {
	Serial.println(name);	
	Serial.println(data);
	
	TSdata = String(data);
	
    tildeIndexBegin = 0;
    tildeIndexEnd = TSdata.indexOf('~');
    // start iteration

    for (int n=0; n <= 12; n++) {
    
        feedValues[n] = TSdata.substring(tildeIndexBegin, tildeIndexEnd);
        tildeIndexBegin = tildeIndexEnd + 1;
        tildeIndexEnd = TSdata.indexOf('~',tildeIndexBegin);
    
    }
    
    for (int i=0; i <= 12; i++) {
        Serial.print(feedName[i]);
        Serial.print(" = ");
        Serial.println(feedValues[i]);
    }
    
}

/*

The output from the serial terminal will look something like this:

hook-response/TSReadHooks/
2016-03-25T20:04:14Z~1827~321~2.1~92~49.3~0.0~99.8~4.0~62~~~~
created_at = 2016-03-25T20:04:14Z
entry_id = 1827
field1 = 321
field2 = 2.1
field3 = 92
field4 = 49.3
field5 = 0.0
field6 = 99.8
field7 = 4.0
field8 = 62
latitude = 
elevation = 
status = 
   
*/
