#include <stdlib.h>
#include <stdio.h>
#include <aisl/aisl.h>

static void handler(const struct aisl_evt *evt, void *p_ctx);

static const struct aisl_cfg_srv server = {
		.host = "0.0.0.0",
		.port = 8080
};

static const struct aisl_cfg config = {
		AISL_CFG_DEFAULTS,
		.srv = &server,
		.srv_cnt = 1,
		.callback = handler,
		.p_ctx = NULL
};

int main(int argc, char **argv) {
    AislInstance aisl;
    if((aisl = aisl_new(&config)) != NULL) {
    	fprintf(stdout, "Starting server at http://127.0.0.1:8080/\n");
    	for(;;) {
    		AislStatus status = aisl_run_cycle(aisl);
    		if (status != AISL_SUCCESS)
    			aisl_sleep(aisl, 500);
    	}
    	aisl_free(aisl);
    } else {
    	fprintf(stderr, "could not initialize AISL\n");
    	return -1;
    }
    return 0;
}

static void handler(const struct aisl_evt *evt, void *p_ctx) {
	const char output[] = "Hello World";

	if(evt->code != AISL_EVENT_STREAM_REQUEST)
		return;

	AislStream s = evt->source;
	if (aisl_response(s, AISL_HTTP_OK, sizeof(output) - 1) == AISL_SUCCESS) {
		if (aisl_write(s, output, sizeof(output) - 1) != -1) {
			aisl_flush(s);
			return;
		}
	}

	aisl_reject(s);
}
