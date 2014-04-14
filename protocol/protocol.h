#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h> /* cstdint tylko w C++11 */
#include <ctime>

namespace monitoring_hal {


/* Identyfikator wysłanej próbki. Pola:
	- sensor_id - identyfikator czujnika, czy 255 wystarczy?. 0xFF - będzie oznaczał sam koncentrator.
	- time - znacznik czasowy. time_t wg. dokumentacji może być nieprzenośny, jakiś inny typ? */

struct identifier {
	uint8_t	sensor_id;
	time_t		time; 
};


/* Wartość zczytana przez koncentrator */

struct single_value {
	identifier		id;
	uint32_t		value;
};


/* Za pomocą control_reponse koncentrator będzie zgłaszał błędy i wysyłał wiadomości zwrotne dotyczące konfiguracji. Pola:
	- id - Identyfikator określa czujnik (koncentrator ma zarezerwowany sensor_id=0xFF)
	- code - kod błędu/wiadomości zwrotnej */

struct control_response {
	identifier		id;
	uint8_t		code;
};


/* Wiadomość od koncentratora do serwera.
	- values_amount - ilość wysyłanych wartości zczytanych (values). Czy powinno być ich więcej niż 255?
	- cr_amount - ilość wysyłanych wiadomości kontrolnych
	- values - wartości zczytane wraz z identyfikatorami czujników i znacznikami czasowymi
	- control_responses - informacje zwrotne o wykonanej konfiguracji urządzenia, wiadomości o problemach technicznych */

struct request_message {
	uint8_t		values_amount;
	uint8_t		cr_amount;
	single_value		values[];
	control_response	control_responses[];
	int8_t			checksum;
};


/* Wiadomość zwrotna od serwera do koncentratora
	- ids_amount - ilość ids
	- ids - potwierdzenie odebrania przesłanych wartości zczytanych (jedynie ich identyfikatory). Po odebraniu danego identyfikatora koncentrator może usunąć dany wpis przechowywany w buforze
	- command - rozkaz przekonfigurowania koncentratora */

struct response_message {
	uint16_t		ids_amount;
	identifier		ids[];
	uint8_t		command;
	int8_t			checksum;
};

}

#endif
