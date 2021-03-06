#ifndef CONFIG_H
#define CONFIG_H

#include <uci.h>
#include <libubox/uloop.h>
#include <libubus.h>
#include <mosquitto.h>
#ifdef WITH_YKW
#include <ykw.h>
#endif

#define CONFIG_MAX_PORTS			7
#define CONFIG_MAX_ANALOG_PORTS		3
#define CONFIG_MAX_PORT_PARAMS		4
#define CONFIG_MAX_SENSOR_PARAMS	3
#define CONFIG_STR_MAX				64
#define CONFIG_MAX_SENSORS			39
#define CONFIG_UCI_DEVICE			"system.@system[0].device"
#define CONFIG_UCI_PHASE			"flx.main.phase"
#define CONFIG_UCI_LED_MODE			"flx.main.led_mode"
#define CONFIG_UCI_THETA			"flx.main.theta"
#define CONFIG_ULOOP_TIMEOUT		1000 /* ms */
#define CONFIG_UBUS_EV_SIGHUP		"flukso.sighup"
#define CONFIG_UBUS_EV_SHIFT_CALC 	"flx.shift.calc"
#define CONFIG_LED_MODE_DEFAULT		255
#define CONFIG_TOPIC_BRIDGE_STAT	"$SYS/broker/connection/flukso-%.6s.flukso/state"
#define CONFIG_GLOBE_LED_PATH		"/sys/class/leds/globe/brightness"

#define CONFIG_MQTT_ID_TPL			"flxd-p%d"
#define CONFIG_MQTT_ID_LEN			16

#define ltobs(A) ((((uint16_t)(A) & 0xff00) >> 8) | \
	              (((uint16_t)(A) & 0x00ff) << 8))
#define ltobl(A) ((((uint32_t)(A) & 0xff000000) >> 24) | \
	              (((uint32_t)(A) & 0x00ff0000) >> 8)  | \
	              (((uint32_t)(A) & 0x0000ff00) << 8)  | \
	              (((uint32_t)(A) & 0x000000ff) << 24))

enum {
	CONFIG_SENSOR_TYPE_ELECTRICITY,
	CONFIG_SENSOR_TYPE_GAS,
	CONFIG_SENSOR_TYPE_WATER,
	CONFIG_SENSOR_TYPE_OTHER
};

enum {
	CONFIG_1PHASE,
	CONFIG_3PHASE_PLUS_N,
	CONFIG_3PHASE_MINUS_N
};

struct mqtt {
	char *host;
	int port;
	int keepalive;
	char id[CONFIG_MQTT_ID_LEN];
	bool clean_session;
	int qos;
	int retain;
};

struct sensor {
	char id[CONFIG_STR_MAX];
	uint8_t type;
	uint8_t enable;
};

struct port {
	uint16_t constant;
	uint16_t fraction;
	uint8_t current;
	uint8_t shift;
	uint8_t enable;
	uint8_t padding;
};

struct main {
	uint8_t phase;
	uint8_t led;
};

struct config {
	int verbosity;
	char *me;
	char device[CONFIG_STR_MAX];
	char topic_bridge_stat[CONFIG_STR_MAX];
	int fd_globe;
	struct sensor sensor[CONFIG_MAX_SENSORS];
	struct port port[CONFIG_MAX_PORTS];
	struct main main;
	struct uci_context *uci_ctx;
	struct uloop_fd flx_ufd;
	struct uloop_timeout timeout;
	struct ubus_context *ubus_ctx;
	struct ubus_event_handler ubus_ev_sighup;
	struct ubus_event_handler ubus_ev_shift_calc;
	struct mqtt mqtt;
	struct mosquitto *mosq;
#ifdef WITH_YKW
	int theta;
	unsigned int enabled;
	struct ykw_ctx *ykw;
#endif
};

extern struct config conf;

bool config_init(void);
bool config_load_all(void);
void config_push(void);

#endif
