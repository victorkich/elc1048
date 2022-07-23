#include <asf.h>
#include <string.h>

#define CONF_PWM_MODULE      LED_0_PWM4CTRL_MODULE
#define CONF_PWM_CHANNEL     LED_0_PWM4CTRL_CHANNEL
#define CONF_PWM_OUTPUT      LED_0_PWM4CTRL_OUTPUT
#define CONF_PWM_OUT_PIN     LED_0_PWM4CTRL_PIN
#define CONF_PWM_OUT_MUX     LED_0_PWM4CTRL_MUX
#define CONF_FAULT_EIC_PIN       SW0_EIC_PIN
#define CONF_FAULT_EIC_PIN_MUX   SW0_EIC_PINMUX
#define CONF_FAULT_EIC_LINE      SW0_EIC_LINE
#define CONF_FAULT_EVENT_GENERATOR EVSYS_ID_GEN_EIC_EXTINT_15
#define CONF_FAULT_EVENT_USER      EVSYS_ID_USER_TCC0_MC_0

	struct tcc_module tcc_instance;
	struct events_resource event_resource;

	static void tcc_callback_to_change_duty_cycle(
	struct tcc_module *const module_inst)
	{
		static uint32_t delay = 10;
		static uint32_t i = 0;
		if (--delay) {
			return;
		}
		delay = 10;
		i = (i + 0x0800) & 0xFFFF/2;
		tcc_set_compare_value(module_inst,
		(enum tcc_match_capture_channel)
		(TCC_MATCH_CAPTURE_CHANNEL_0 + CONF_PWM_CHANNEL),
		i + 1);
	}

	static void eic_callback_to_clear_halt(void)
	{
		if (port_pin_get_input_level(CONF_FAULT_EIC_PIN)) {
			tcc_clear_status(&tcc_instance,
			TCC_STATUS_RECOVERABLE_FAULT_OCCUR(CONF_PWM_CHANNEL));
		}
	}

	static void configure_tcc(void)
	{
		struct tcc_config config_tcc;
		tcc_get_config_defaults(&config_tcc, CONF_PWM_MODULE);
		config_tcc.counter.period = 0xFFFF;
		config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
		config_tcc.compare.match[CONF_PWM_CHANNEL] = 0xFFFF;
		config_tcc.wave_ext.recoverable_fault[CONF_PWM_CHANNEL].source =
		TCC_FAULT_SOURCE_ENABLE;
		config_tcc.wave_ext.recoverable_fault[CONF_PWM_CHANNEL].halt_action =
		TCC_FAULT_HALT_ACTION_SW_HALT;
		config_tcc.pins.enable_wave_out_pin[CONF_PWM_OUTPUT] = true;
		config_tcc.pins.wave_out_pin[CONF_PWM_OUTPUT]        = CONF_PWM_OUT_PIN;
		config_tcc.pins.wave_out_pin_mux[CONF_PWM_OUTPUT]    = CONF_PWM_OUT_MUX;
		tcc_init(&tcc_instance, CONF_PWM_MODULE, &config_tcc);
		struct tcc_events events;
		memset(&events, 0, sizeof(struct tcc_events));
		events.on_event_perform_channel_action[CONF_PWM_CHANNEL] = true;
		tcc_enable_events(&tcc_instance, &events);
		tcc_enable(&tcc_instance);
	}
	static void configure_tcc_callbacks(void)
	{
		tcc_register_callback(
		&tcc_instance,
		tcc_callback_to_change_duty_cycle,
		(enum tcc_callback)(TCC_CALLBACK_CHANNEL_0 + CONF_PWM_CHANNEL));
		tcc_enable_callback(&tcc_instance,
		(enum tcc_callback)(TCC_CALLBACK_CHANNEL_0 + CONF_PWM_CHANNEL));
	}

	static void configure_eic(void)
	{
		struct extint_chan_conf config;
		extint_chan_get_config_defaults(&config);
		config.filter_input_signal = true;
		config.detection_criteria  = EXTINT_DETECT_BOTH;
		config.gpio_pin     = CONF_FAULT_EIC_PIN;
		config.gpio_pin_mux = CONF_FAULT_EIC_PIN_MUX;
		extint_chan_set_config(CONF_FAULT_EIC_LINE, &config);
		struct extint_events events;
		memset(&events, 0, sizeof(struct extint_events));
		events.generate_event_on_detect[CONF_FAULT_EIC_LINE] = true;
		extint_enable_events(&events);
		extint_register_callback(eic_callback_to_clear_halt,
		CONF_FAULT_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT);
		extint_chan_enable_callback(CONF_FAULT_EIC_LINE,
		EXTINT_CALLBACK_TYPE_DETECT);
	}

	static void configure_event(void)
	{
		struct events_config config;
		events_get_config_defaults(&config);
		config.generator = CONF_FAULT_EVENT_GENERATOR;
		config.path      = EVENTS_PATH_ASYNCHRONOUS;
		events_allocate(&event_resource, &config);
		events_attach_user(&event_resource, CONF_FAULT_EVENT_USER);
	}

int main (void)
{
	system_init();
	configure_tcc();
    configure_tcc_callbacks();
    configure_eic();
    configure_event();


    system_interrupt_enable_global();
    while (true) {
    }
}
