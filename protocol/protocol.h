#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <time.h>

namespace monitoring_hal {


/* Identyfikator wysłanej próbki. Pola:
	- sensor_id - identyfikator czujnika, czy 255 wystarczy?. 0xFF - będzie oznaczał sam koncentrator.
	- time - znacznik czasowy. time_t wg. dokumentacji może być nieprzenośny, jakiś inny typ? */

typedef struct {
	uint8_t	sensor_id;
	time_t		time; 
} identifier;


/* Wartość zczytana przez koncentrator */

typedef struct {
	identifier		id;
	uint32_t		value;
} single_value;


/* Za pomocą control_reponse koncentrator będzie zgłaszał błędy i wysyłał wiadomości zwrotne dotyczące konfiguracji. Pola:
	- id - Identyfikator określa czujnik (koncentrator ma zarezerwowany sensor_id=0xFF)
	- code - kod błędu/wiadomości zwrotnej */

typedef struct {
	identifier		id;
	uint8_t		code;
} control_response;


/* Wiadomość od koncentratora do serwera.
	- values_amount - ilość wysyłanych wartości zczytanych (values). Czy powinno być ich więcej niż 255?
	- cr_amount - ilość wysyłanych wiadomości kontrolnych
	- values - wartości zczytane wraz z identyfikatorami czujników i znacznikami czasowymi
	- control_responses - informacje zwrotne o wykonanej konfiguracji urządzenia, wiadomości o problemach technicznych */

typedef struct {
	uint8_t		values_amount;
	uint8_t		cr_amount;
	single_value		values[];
	control_response	control_responses[];
	int8_t			checksum;
} request_message;


/* Wiadomość zwrotna od serwera do koncentratora
	- ids_amount - ilość ids
	- ids - potwierdzenie odebrania przesłanych wartości zczytanych (jedynie ich identyfikatory). Po odebraniu danego identyfikatora koncentrator może usunąć dany wpis przechowywany w buforze
	- command - rozkaz przekonfigurowania koncentratora */

typedef struct {
	uint16_t		ids_amount;
	identifier		ids[];
	uint8_t		command;
	int8_t			checksum;
} response_message;

}

#endif
