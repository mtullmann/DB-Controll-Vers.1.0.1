#pragma once

#include "DB_serial.h"
#include "utility.h"
#include <iostream>
#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>
#include "sheet.h"

#define DB_MC_TICKS_PER_TURN       468
#define DB_MC_WHEEL_CIRCUMFERENCE  217
uint8_t sysloud = 10;
uint16_t db_circumference_value = DB_MC_WHEEL_CIRCUMFERENCE;
bool dbLs_setLoud(uint8_t loud);
bool dbLs_getLoud(uint8_t* loud);
bool db_Beep() {
	uint8_t loud = 0;
	if (!dbLs_getLoud(&loud))
		return 0;
	if (!dbLs_setLoud(sysloud))
		return 0;
	uint8_t in[DB_MAXLENGTH];
	in[0] = 0;
	in[1] = 0;
	in[2] = 0;
	if (!db_cmd(DB_LS, 3, in, NULL)) {
		return 0;
	}
	wait(300);
	return dbLs_setLoud(loud);
}
bool db_Siren() {
	uint8_t loud = 0;
	if (!dbLs_getLoud(&loud))
		return 0;
	if (!dbLs_setLoud(sysloud))
		return 0;
	uint8_t in[DB_MAXLENGTH];
	in[0] = 1;
	in[1] = 0;
	in[2] = 0;
	if (!db_cmd(DB_LS, 3, in, NULL)) {
		return 0;
	}
	wait(300);
	return dbLs_setLoud(loud);
}

bool bt_os(bool val);
void exitS() {
	color(TURKEYSEh);
	printf("Push 'e' to exit this sketch!!\n");
	rcolor;
	while ('e' != _getch());
}
bool db_load_song_to_bot(uint8_t num);
bool bt_get_cmd(const char* in, char* out);
enum DB_LED_ID
{
	DB_LED_FRONT_RIGHT_OUT = 0,
	DB_LED_FRONT_RIGHT_IN = 1,
	DB_LED_FRONT_LEFT_IN = 2,
	DB_LED_FRONT_LEFT_OUT = 3,

	DB_LED_BUTTON_BLUE = 4,
	DB_LED_BUTTON_GREEN = 5,
	DB_LED_BUTTON_RED = 6,

	DB_LED_BACK_LEFT_OUT = 7,
	DB_LED_BACK_LEFT_IN = 8,
	DB_LED_BACK_RIGHT_IN = 9,
	DB_LED_BACK_RIGHT_OUT = 10
};
bool dbLed_setColor(uint8_t led, uint8_t r, uint8_t g, uint8_t b);
bool db_clearBuffer() {
	uint8_t paramin[10] = { 0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00 };
	printf("Initialisyse direct buffer clearing (Chip and PC)! (For secure connection and safty!)\n");
	if (db_cmd(DB_BUFFER_C, 10, paramin, NULL)) {
		color(GRUENh);
		printf("Buffer cleared!\n");
		rcolor; return 1;
	}
	else {
		color(ROTh);
		printf("Fail clearing buffer\n");
		rcolor;
		return 0;
	}
}
int db_unknown() {
	static uint8_t first = 1;
	uint8_t i1[DB_MAXLENGTH];
	int lps = 0;
	int lps0 = 0;
	//printf("This is a test if we get a usefully commands from Discbot!\nNow we should get an unknown request\n");
	do {

		lps0++;
		db_cmd(DB_UNKNOWN, 0, NULL, i1);
		if ((i1[1] != 0) || (i1[0] != 0)) {
			lps++;
			color(ROTh);
		}
		else
			color(GELBh);


		printf("\rEstablish a secure connection %03.2f%%", (float)percent(80, lps));

		wait(25);

		if (lps0 == 80) {
			color(GRUENh);

			printf("\rEstablish a secure connection %03.2f%%", (float)percent(80, lps));
			rcolor;
			if (first) {
				color(ROTh);
				printf("\nMake sure your COM port is right, flash the %s and restart the program!\n", PER_DEVICE);
				bell();
				color(rcol);
				first = 0;
				return -1;
			}
			else {
				color(ROTh);
				printf("\nDiscBot malefunction!!\nPlease do a hardware reset manually!!!\nThan push any key!\n");
				printf("%c", BELL);
				color(rcol);
				_getch();
			}
			first = 0;
			wait(500);
		}
	} while (lps0 != 80 && (i1[1] != 0 || i1[0] != 0));
	if (lps0 != lps)
		color(GRUENh);
	else
		color(ROTh);
	printf("\rEstablish a secure connection %03.2f%%", (float)percent(80, 80));
	rcolor;
	printf("\n");
	if (lps == 80) {
		color(ROTh);
		printf("\tThe connection is not ready! (%i loops)\n", lps);
		color(rcol);
	}
	else {
		color(GRUENh);
		printf("\tThe connection is now ready! (%i loops)\n", lps);
		color(rcol);
	}
	//printf("\n");
	if (lps == 0)
		return 1;
	return 0;
}
void reset() {
	printf("Initialisyse Hardware reset!\n");
	db_cmd(DB_RESET, 0, NULL, NULL);
	waitS(2000, 1);
	int i = 2;
	while (!waitS(2000, 0)) {
		printf("\rWait %i seconds", i);
		wait(1000);
		i--;
	}
	printf("\r");
	color(GRUENh);
	printf("Hardware reset done!\n");
	color(rcol);
}
bool db_reset() {
	db_clearBuffer();
	reset();
	uint8_t kl = 0;
	//db_cmd(DB_CLEAR, 0, NULL, NULL);
	for (uint8_t i = 0; kl != 3; i++) {
		switch (db_unknown()) {
		case 1:kl++; break;
		case 0:kl = 0; break;
		case -1:return 0; break;
		default:return 0; break;
		}
	}
	db_clearBuffer();
	return 1;
}
bool db_ledOnOff() {
	char title[100] = PER_TITLE;
	strcat(title, PER_VERS);
	strcat(title, " - ");

	strcat(title, "LED_On_Off");
	//title[strlen(title) - strlen(cx)] = title[strlen(title) - strlen(cx)] + ('A' - 'a');
	strcat(title, " test");
	system(title);
	color(GELBh);
	printf("Start LED_On_Off\n");
	rcolor;
	bool ret = 1;
	printf("All LED's are flashing in same colors!\n");
	printf("Do you want to change the bink delay (100 ms) ? (y/n)\n");
	int del = 100;
	if (_getch() == 'y') {
		printf("Put in the number of milliseconds yuo want to wait, then push enter: ");
		do {
			if (del < 0 || del>999999999) {
				printf("invalid number please try again! :");
			}
			del=int_inp();
			flush();

		} while (del < 0 || del>999999999);
	}
	color(GELBh);
	printf("Press any key to start!\n");
	printf("Press enter to stop!\n");
	rcolor;
	_getch();
	uint8_t param[100];
	uint8_t ln = 0;
	uint8_t inOn[] = { 0, 255, 255, 255 };
	uint8_t inOff[] = { 0, 0, 0, 0 };
	uint8_t col = 0;
	bool en = 0;
	//char colo[6] = "white";
	//flush();
	while (anyKey());
	do {
		if ((col % 30) == 0) {
			waitS(300, 1);
			while (!waitS(300, 0) && en == 0) {
				en |= anyKey();
			}
		}

		printf("\nON : ");
		switch (col % 4) {
		case 0:inOn[1] = 255; inOn[2] = 255; inOn[3] = 255; rcolor; printf("white"); break;
		case 1:inOn[1] = 255; inOn[2] = 0; inOn[3] = 0; color(ROTh); printf("red"); rcolor; break;
		case 2:inOn[1] = 0; inOn[2] = 255; inOn[3] = 0; color(GRUENh); printf("green"); rcolor; break;
		case 3:inOn[1] = 0; inOn[2] = 0; inOn[3] = 255; color(BLAUh); printf("blue"); rcolor; break;
		}
		//printf(colo);
		col++;
		printf("\n");
		for (uint8_t i = 0; i != 11; i++) {
			inOn[0] = i;
			//printf("\tLED %i", i);
			if (!db_cmd(DB_LED, 4, inOn, param)) {
				en = 1;
			}
			if (param[0] != DB_LED || param[1] != 0) {
				color(ROTh);
				printf("Callback ERROR by Discbot!");
				rcolor;
				return  0;
			}
			//wait(25);
			if (i == 10)
				color(GRUENh);
			else
				color(GELBh);
			printf("\r Executation status: %4.2f%%", (float)percent(10, i));
			rcolor;
			en |= anyKey();
		}
		waitS(del, 1);
		while (!waitS(del, 0) && en == 0) {
			en |= anyKey();
		}

		//printf("\n");
		printf("\tOFF");
		for (uint8_t i = 0; i != 11; i++) {
			//printf("\tLED %i", i);
			inOff[0] = i;
			if (!db_cmd(DB_LED, 4, inOff, param)) {
				en = 1;
			}

			//wait(25);
			en |= anyKey();
		}
		waitS(del, 1);
		while (!waitS(del, 0) && en == 0) {
			en |= anyKey();
		}
		//printf("\n");
	} while (!en);
	color(GELBd);
	printf("\nEnd LED_On_Off\n");
	rcolor;
	flush();
	return ret;
}

bool db_Tbtn() {
	uint8_t ou[21];
	char s[51] = "";
	char col[3][21] = { "red","green","blue" };
	//char gcol[3][21] = { "Red","Green","Blue" };
	printf("Switch all switches to uC and do not push any buttons!\nThen push any key\n");
	_getch();
	db_cmd(DB_BTN, 0, NULL, ou);
	//printf("ou=%i\n", ou[2]);
	if (ou[2] == 0) {
		printf("No button was pushed\n");
		db_Beep();
		//cScError("No button was pushed\n");
	}
	else {
		for (uint8_t i = 0; i != 3; i++) {
			if ((ou[2] & (1 << i))) {
				printf("The ");
				switch (i) {
				case 0:color(ROTh); break;
				case 1:color(GRUENh); break;
				case 2:color(BLAUh); break;
				}
				printf(col[i]);
				color(rcol);
				printf(" button was pushed instead of none!\n");
				sprintf(s, "The button %s was pushed instead of none!\n", col[i]);
				cScError(s);
				db_Siren();
			}
		}
	}
	bool f = 0;
	for (uint8_t oi = 0; oi != 3; oi++) {
		printf("Push the ");
		switch (oi) {
		case 0:color(ROTh); dbLed_setColor(6, 255, 0, 0); break;
		case 1:color(GRUENh); dbLed_setColor(6, 0, 0, 0);  dbLed_setColor(5, 0, 255, 0); break;
		case 2:color(BLAUh); dbLed_setColor(5, 0, 0, 0); dbLed_setColor(4, 0, 0, 255); break;
		}
		printf(col[oi]);
		color(rcol);
		printf(" button, if nothing is changeing push enter and the %s button!\n", col[oi]);
		do {
			if (!db_cmd(DB_BTN, 0, NULL, ou))
				return 0;
			wait(100);
		} while (!(f = anyKey()) && !(ou[2] & (1 << oi)));
		dbLed_setColor(6, 0, 0, 0);
		dbLed_setColor(5, 0, 0, 0);
		dbLed_setColor(4, 0, 0, 0);
		while (f)
		{
			f = 0;
			wait(500);
			while ('\n' == _getch()) {
				//wait(50);
			}
		}
		if (anyKey()) {
			color(VIOLETTh);
			printf("Release the Enter key to continue!!!\n");
			rcolor;
			while (anyKey()) {
				wait(500);
				while ('\n' == _getch()) {

				}
			}
		}

		char s[80] = "";
		//printf("ou=%i\n", ou[2]);
		if (ou[2] == 0) {
			printf("No button was pushed instead of the ");
			switch (oi) {
			case 0:color(ROTh); break;
			case 1:color(GRUENh); break;
			case 2:color(BLAUh); break;
			}
			printf(col[oi]);
			color(rcol);
			printf(" Button\n");
			sprintf(s, "No button was pushed instead of the %s Button\n", col[oi]);
			cScError(s);
			db_Siren();
		}
		else {
			for (uint8_t i = 0; i != 3; i++) {
				if ((ou[2] & (1 << i))) {
					printf("The button ");
					switch (i) {
					case 0:color(ROTh); break;
					case 1:color(GRUENh); break;
					case 2:color(BLAUh); break;
					}
					printf(col[i]);
					color(rcol);
					printf(" was pushed!\n");
					if (oi != i) {
						sprintf(s, "The button %s was pushed instead of the %s button\n", col[i], col[oi]);
						cScError(s);
						db_Siren();
					}
					else {
						sprintf(s, "The %s button was pushed\n", col[i]);
						cSaddMsg(s);
						db_Beep();
					}
				}
				else {
					if (oi == i) {
						sprintf(s, "The button %s was not pushed\n", col[i]);
						cScError(s);
						db_Siren();
					}
				}
			}
		}
		//wait(1000);
	}
	for (uint8_t i = 0; i != 3; i++) {
		dbLed_setColor(4 + i, 0, 0, 0);
	}
	exitS();
	return 1;
}
bool db_Trfid() {
	uint8_t out[DB_MAXLENGTH];
	uint8_t stcd[6] = { DB_RFID,4, 0,0,0,0 };
	int in = 0, kt = 0;
	bool jk = 0;
	char s[201] = "";
	printf("In this sketch you have to put a RFID-card to the reader (under the DiscBot)!\nThen the UUID of the RFID-card appare on the screen!\nYou can compare them with your smartphone or any other RFID (or NFC) reader!\n");
	printf("Do you want to open an App (Android and IOS) ? (y/n)\n");
	if ('y' == ynrespond()) {
		system("start https://play.google.com/store/apps/details?id=com.wakdev.wdnfc");
		system("start https://apps.apple.com/at/app/nfc-tools/id1252962749");
	}
	color(GELBh);
	printf("To start push enter!\nTo stop press enter!\n");
	rcolor;
	while (!anyKey());
	flush();

	do {
		in++;

		for (kt = 0; !db_cmd(DB_RFID, 0, NULL, out) && kt != 10; kt++, wait(25));
		if (kt == 10) {
			printf("Please test the connection between DiscBot and the computer!\n");
			return 0;
		}
		else {
			if (out[1] != 0 && (memcmp(out, stcd, 6) != 0)) {

				for (uint8_t i = 0; i != 6; i++)
					stcd[i] = out[i];
				printf("Card found\tUUID : ");

				cSaddMsg("Card found\tUUID : ");
				for (uint8_t i = 0; i != 4; i++) {
					printf(" %02X ", out[(3 - i) + 2]);
					sprintf(s, " %02X ", out[(3 - i) + 2]);
					cSaddMsg(s);
				}
				cSaddMsg("\n");
				jk = 0;
				printf("\n");
				if (!dbLed_setColor(DB_LED_BUTTON_GREEN, 0, 255, 0))
					return 0;
				if (!db_Beep())
					return 0;
			}
			else {
				if (out[1] == 0 && !jk) {
					jk = 1;
					printf("No card found yet!\n");
					cSaddMsg("No card found yet!\n");
					if (!dbLed_setColor(DB_LED_BUTTON_GREEN, 255, 0, 0))
						return 0;
					if (!db_Siren())
						return 0;
				}
			}
			if ((in % 8) == 0)
				for (uint8_t i = 0; i != 6; i++)
					stcd[i] = out[i];
		}

		waitS(3, 1);
		while (!waitS(3, 0) && !anyKey()) {
		}
	} while (!anyKey());
	flush();
	if (!dbLed_setColor(DB_LED_BUTTON_GREEN, 0, 0, 0))
		return 0;
	cSaskIfFine();


	return 1;
}
bool db_Tcs() {
	uint8_t kt = 0;
	uint8_t out[DB_MAXLENGTH];
	int timo = 1000;
	char cx[12] = "";
	printf("This sketch show you the colors from the color sensor of the DiscBot!\n");
	color(GELBh);
	printf("Do you want to change the timeout from %i milliseconds? (y/n)\n", timo);
	rcolor;
	if ('y' == ynrespond()) {
		color(GELBh);
		printf("Type in an intervall of milliseconds you want the requests: ");
		rcolor;
		do {
			timo = 0;
			timo = int_inp();
		} while (timo <= 0);
	}
	color(GELBh);
	printf("To stop push the enter key!\nTo start push any key!\n");
	rcolor;
	_getch();
	do {
		for (kt = 0; !db_cmd(DB_CS, 0, NULL, out) && kt != 10; kt++, wait(25));
		if (kt == 10) {
			printf("Please test the connection between DiscBot and the computer!\n");

		}
		else {
			int v = 0;
			int sum = 0;
			for (uint8_t i = 0; i != 3; i++) {
				switch (i) {
				case 0:strcpy(cx, "left"); v = 5; break;
				case 1:strcpy(cx, "middle"); v = 11; break;
				case 2:strcpy(cx, "right"); v = 17; break;
				}
				printf("\n\tColor %s\n", cx);
				printf("Color index: %i\n", out[2 + i]);
				//printf("Color quality: %i %i\n", out[4 + i], out[3 + i]);
				printf("Color quality: %i\n", (out[4 + i] << 8) + out[3 + i]);


				for (uint8_t ii = 0; ii != 3; ii++) {
					sum = v + ii;
					switch (ii) {
					case 0:strcpy(cx, "red");
						if (out[sum] >= 150)
							color(ROTh);
						break;
					case 1:strcpy(cx, "green");
						if (out[sum] >= 150)
							color(GRUENh);
						break;
					case 2:strcpy(cx, "blue");
						if (out[sum] >= 150)
							color(BLAUh);
						break;
					}
					printf("Color %s: %i\n", cx, out[sum]);
					rcolor;
					//printf("Color %s: %02X num:%i  %i\n", cx, out[sum], sum, v);
				}
				dbLed_setColor(6 - i, out[v + 0], out[v + 1], out[v + 2]);
			}

		}
		waitS(timo, 1);
		while (!waitS(timo, 0) && !anyKey());
	} while (!anyKey());
	for (uint8_t i = 0; i != 3; i++)
		dbLed_setColor(6 - i, 0, 0, 0);

	flush();
	cSaskIfFine();
	return 1;
}
bool dbLed_setColor(uint8_t led, uint8_t r, uint8_t g, uint8_t b) {
	uint8_t in[12] = { led, r,g,b };
	uint8_t out[DB_MAXLENGTH];
	if (!db_cmd(DB_LED, 4, in, out))
		return 0;
	return 1;
}
bool db_Tled() {
	int tiver = 500;

	do {
		printf("In this sketch the Led's are flashing!\nIf you are ready push any key!\n");
		_getch();
		do {
			printf("Do you want to wait for each LED %i milliseconds? (y/n)\n", tiver);
			if ('n' == _getch()) {
				tiver = 999999;
				printf("Set your timeout: ");
				do {
					tiver=int_inp();
				} while (tiver == 999999);
				flush();
			}
			else {
				tiver = 50;
			}
		} while (tiver <= 0);
		uint8_t cntW = 0;
		uint8_t colorx[3] = { 255,255,255 };
		for (uint8_t i = 0; i != 4; i++) {
			printf("Color (%i) : ", i);
			switch (i) {
			case 0:colorx[0] = 255; colorx[1] = 255; colorx[2] = 255; rcolor; printf("white"); break;
			case 1:colorx[0] = 255; colorx[1] = 0; colorx[2] = 0; color(ROTh); printf("red"); rcolor; break;
			case 2:colorx[0] = 0; colorx[1] = 255; colorx[2] = 0; color(GRUENh); printf("green"); rcolor; break;
			case 3:colorx[0] = 0; colorx[1] = 0; colorx[2] = 255; color(BLAUh); printf("blue"); rcolor; break;
			}
			printf(" :\t");

			for (cntW = 0; cntW != 12; cntW++) {
				printf("LED %i\t", cntW + 1);
				switch (cntW) {
				case 0:dbLed_setColor(cntW, colorx[0], colorx[1], colorx[2]); break;
				case 1:
				case 2:
				case 3:
					dbLed_setColor(cntW, colorx[0], colorx[1], colorx[2]);
					dbLed_setColor(cntW - 1, 0, 0, 0); break;
				case 4:dbLed_setColor(cntW - 1, 0, 0, 0);
				case 5:
				case 6:
				case 7:
					dbLed_setColor(cntW + 3, colorx[0], colorx[1], colorx[2]);
					dbLed_setColor(cntW + 2, 0, 0, 0); break;
				case 8:dbLed_setColor(cntW + 2, 0, 0, 0);
				case 9:
				case 10:dbLed_setColor(14 - cntW, colorx[0], colorx[1], colorx[2]);
					dbLed_setColor(15 - cntW, 0, 0, 0); break;
				case 11:dbLed_setColor(15 - cntW, 0, 0, 0); break;

				}
				if (cntW != 11) {
					wait(tiver);
				}
				else {
					wait(25);
					printf("\n");
				}
			}
		}
		printf("Again? (y/n)\n");
	} while (_getch() == 'y');
	cSaskIfFine();
	return 1;
}
bool dbLs_setLoud(uint8_t loud) {
	uint8_t in[3] = { loud,0,0 };
	return db_cmd(DB_LOUD, 1, in, NULL);
}
bool dbLs_getLoud(uint8_t* loud) {
	uint8_t out[3] = { 0,0,0 };
	*loud = 0;
	if (!db_cmd(DB_LOUD, 0, NULL, out))
		return 0;
	*loud = out[2];
	return 1;
}

bool db_Tls() {
	dbLs_setLoud(102);
	int loud = -1;
	int son = 1000;
	int mod = 1000;
	uint8_t in[DB_MAXLENGTH];
	do {
		printf("This sketch plays a song from the DiscBot loudspeaker!\nTurn the switch to uC\n");

		do {

			color(GELBh);
			printf("Please set your volume (0-100=sound) (101=mute) (102=standard) and punch enter:");

			rcolor;
			do {
				loud = -1;
				loud = int_inp();
			} while (loud < 0);
			if (loud == -1) {
				color(ROTh);
				printf("Wrong value set!\ttry again!\n");
				rcolor;
			}
		} while (loud < 0 || loud>102);
		if (!dbLs_setLoud(loud))
			return 0;
		printf("Repeating the song continuously? (y/n)\n");
		in[2] = (ynrespond() == 'y');
		do {

			printf("Select a song:\n");
			for (uint8_t i = 0; i != MaxSongs; i++) {
				if (i % 2) {
					color(GELBh);
				}
				printf("(%02i) ... ", i + 1);
				uint16_t len = 0;
				for (len = 0; dbLs_ALL[i][len].pitch != DB_LS_LAST; len++);
				printf("%4.01f sec (real %5.01f sec) -", (float)((float)sonlen((DbLsNote*)dbLs_ALL[i]) / 1000), (float)((float)sonlenDB((DbLsNote*)dbLs_ALL[i]) / 1000));
				printf("%3i note%s", len, (len == 1) ? "  " : "s ");
				printf(" - \"");
				switch (i) {
				case 0:printf("Beep"); break;
				case 1:printf("Sirene"); break;
				case 2:printf("muss i denn zum st%cdtele hinaus", 132); break;
				case 3:printf("AURA LEE (America)"); break;
				case 4:printf("BIG BEN (England)"); break;
				case 5:printf("ROW, ROW, ROW YOUR BOAT (Traditional)"); break;
				case 6:printf("AH! Vous DIRAI-JE, MAMAN (Morgen kommt der Weihnachtsmann) (France melody from 1771)"); break;
				case 7:printf("SUR LE PONT D'AVIGNON (France)"); break;
				case 8:printf("YANKEE DOODLE (America)"); break;
				case 9:printf("KAISER-WALZER (Johann Strauss, sun 1825-1899)"); break;
				case 10:printf("SUPER MARIO LAND (1989) (Martin Tullmann)"); break;
				case 11:printf("SUPER MARIO BROS (1983)"); break;
				case 12:printf("SUPER MARIO BROS UNDERWORLD (1983)"); break;
				case 13:printf("PAC-MAN (1980)"); break;
				case 14:printf("Dart Vader theme (Imperial March) - Star wars (1980)"); break;
				case 15:printf("Astronomia (2010) (Coffin-Dance-Theme)"); break;
				case 16:printf("Entertainer (1902) (Scott Joplin)"); break;
				case 17:printf("Take on Me (1985)"); break;
				case 18:printf("Pirates Of The Caribbean Theme Song (2003)"); break;
				case 19:printf("Star Trek (1966)"); break;
				case 20:printf("Tetris (1984)"); break;
				case 21:printf("WICKI (1974) (Martin Tullmann)"); break;
				case 22:printf("The Campfire Song Song (2002)"); break;
				case 23:printf("Harry Potter (2001)"); break;
				case 24:printf("Simpsons"); break;
				case 25:printf("KNIGHT RIDER"); break;
				case 26:printf("Mii Chanel"); break;
				case 27:printf("Cantina Band (Star Wars)"); break;
				case 28:printf("Pink Panter"); break;
				case 29:printf("F%cr Elise",ue); break;
				default:printf("undefined"); break;
				}
				printf("\"\n");
				if (!((i + 1) % 5) && (MaxSongs > (i + 1))) {
					printf("\n");

				}
				rcolor;
			}
			printf("Type in any number and press enter: ");
			son=int_inp();
			son--;
			if (son > MaxSongs || son < 0) {
				color(ROTh);
				printf("Wrong value set!\n\ttry again!\n");
				rcolor;
			}
		} while (son > MaxSongs - 1 || son < 0);
		double lenmax = sonlen((DbLsNote*)dbLs_ALL[son]);
		color(GRUENh);
		printf("The song lasts %0.3lf seconds!\n", lenmax / 1000);
		rcolor;
		if (son >= 3) {
			if (!db_load_song_to_bot(son)) {
				color(ROTh);
				printf("Transmission FAIL!!\n");
				rcolor;
				return 0;
			}
			else {
				color(GRUENh);
				printf("Transmission PASS!!\n");
				rcolor;
			}
		}
		printf("Do you want to play this song on the computer? (y/n)\n");
		if ('n' != ynrespond()) {
			do {

				color(GRUENh);
				printf("Song is playing please wait ...\n");
				rcolor;
				playSong((DbLsNote*)dbLs_ALL[son]);
				printf("Song was played, we can go on!\nAgain? (y/n)\n");
			} while ('y' == ynrespond());
		}
		//in[0] = son;
		if (son < 3)
			in[0] = son;
		else
			in[0] = 3;

		mod = 0;
		do {
			if (mod > 10 || mod < 0) {
				color(ROTh);
				printf("Wrong value set!\ttry again!\n");
				rcolor;
			}
			printf("Set mode (0...regular, 1...stacato1, ..., 9...stacato9, 10...smooth) and push enter: ");
			mod = int_inp();
		} while (mod > 10 || mod < 0);
		in[1] = mod;
		if (!db_cmd(DB_LS, 3, in, NULL))
			return 0;
		color(GELBh);
		printf("Push enter to stop the song!\n");
		rcolor;
		if (son == 25) {
			//_getch();
			wait(1000);
			flush();
			flush();
			flush();
			while (!anyKey()) {

				dbLed_setColor(DB_LED_FRONT_LEFT_OUT, 255, 0, 0);
				dbLed_setColor(DB_LED_FRONT_LEFT_OUT, 0, 0, 0);
				dbLed_setColor(DB_LED_FRONT_LEFT_IN, 255, 0, 0);
				dbLed_setColor(DB_LED_FRONT_LEFT_IN, 0, 0, 0);

				//if (!anyKey())
				//	break;

				dbLed_setColor(DB_LED_BUTTON_RED, 255, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_RED, 0, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_GREEN, 255, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_GREEN, 0, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_BLUE, 255, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_BLUE, 0, 0, 0);
				//if (!anyKey())
				//	break;


				dbLed_setColor(DB_LED_FRONT_RIGHT_IN, 255, 0, 0);
				dbLed_setColor(DB_LED_FRONT_RIGHT_IN, 0, 0, 0);
				dbLed_setColor(DB_LED_FRONT_RIGHT_OUT, 255, 0, 0);
				dbLed_setColor(DB_LED_FRONT_RIGHT_OUT, 0, 0, 0);
				dbLed_setColor(DB_LED_FRONT_RIGHT_IN, 255, 0, 0);
				dbLed_setColor(DB_LED_FRONT_RIGHT_IN, 0, 0, 0);
				//if (!anyKey())
				//	break;

				dbLed_setColor(DB_LED_BUTTON_BLUE, 255, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_BLUE, 0, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_GREEN, 255, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_GREEN, 0, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_RED, 255, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_RED, 0, 0, 0);
				//if (!anyKey())
				//	break;

				dbLed_setColor(DB_LED_FRONT_LEFT_IN, 255, 0, 0);
				dbLed_setColor(DB_LED_FRONT_LEFT_IN, 0, 0, 0);


			}
			for (uint8_t i = 0; i != 7; i++) {
				dbLed_setColor(i, 0, 0, 0);
			}
		}
		else {
			_getch();
			flush();
			while (!anyKey());
			flush();
		}
		color(ROTh);
		printf("Stop song!\n");
		rcolor;
		if (!db_cmd(DB_LSS, NULL, NULL, NULL)) return 0;
		printf("Again? (y/n)\n");
	} while (ynrespond() == 'y');
	cSaskIfFine();

	return 1;
}
bool db_cBtsend(char* data) {
	return db_cmd(DB_BT, strlen(data), (uint8_t*)data, NULL);
	//return 1;
}
bool db_cBtsend(char* data, int len) {
	return db_cmd(DB_BT, (uint8_t)len, (uint8_t*)data, NULL);
	//return 1;
}
bool db_cBtget(char* data) {
	wait(400);
	uint8_t param[256] = "";
	if (db_cmd(DB_BT, NULL, NULL, param)) {
		for (uint8_t i = 0; i != param[1]; i++)
			data[i] = (char)param[i + 2];
		data[param[1]] = '\0';
		return 1;
	}
	return 0;
}
bool db_cBtAnalysis(char* datin, uint8_t* saetze, char* datout) {
	if (datin[0] != '\n' || datin[1] != '+' || datin[2] != '+' || datin[3] != '+') {
		return 0;
	}
	if (datin[4] == '\0') {
		datout[0] = '\0';
		*saetze = 0;
		return 1;
	}
	uint8_t i = 4;
	uint8_t io = 0;

	*saetze = 0;

	//	printf("Sat0: %i\n", *saetze);
	do {
		*saetze = *saetze + 1;
		if (*saetze == 255) {
			return 0;
		}
		//printf("Sat: %i\n", *saetze);
		do {
			datout[io] = datin[i];
			////	printf("%c", datin[i]);
			io++;
			i++;
		} while (datin[i] != '\n' || datin[i + 1] != '+' || datin[i + 2] != '+' || datin[i + 3] != '+');
		//printf("%c%c%c%c", datin[i], datin[i + 1], datin[i + 2], datin[i + 3]);
		i += 4;
		datout[io] = '\0';
		io++;
		//printf("if (%i (%c) ==%i)\n", datin[i], datin[i], '\0');
	} while (datin[i] != '\0');
	return 1;
}

int db_BTAT() {
	char cm[20] = "";
	char cm1[20] = "";
	char cm2[20] = "";
	uint8_t num = 0;
	if (!bt_get_cmd("ATS", cm))
		return 0;
	if (!db_cBtsend(cm))
		return -1;
	if (!bt_get_cmd("ATG", cm))
		return 0;
	if (!db_cBtget(cm1))
		return -1;
	if (!db_cBtAnalysis(cm1, &num, cm2))
		return 3;
	//printf("%s == %s\n",cm2,cm);

	if (!strcmp(cm2, cm))
		return 1;
	color(ROTh);
	printf("Callback should be: \"");
	printf(cm);
	printf("\" instead of \"");
	printf(cm2);
	printf("\"\n");
	rcolor;
	return 2;
}
bool db_BTWC(const char* cmd, char* out, char* paramin, char* paramout) {
	bt_os(0);
	char cm1[51];
	char cm[51];
	char ou[51];
	char cp[51];
	char* cpp = NULL;
	strcpy(cm1, cmd);
	strcat(cm1, "S");
	if (paramin == NULL) {
		strcat(cm1, "?");
	}
	uint8_t num = 0;
	if (paramout != NULL) {
		strcpy(paramout, "nan");
	}
	char oux[51];
	if (!bt_get_cmd((char*)cm1, cm)) {
		printf("%s is not defined in HM file! (%s) \n", cmd, cm1);
		return 0;
	}
	if (!strcmp(cm, "\\-")) {
		color(BLAUh);
		printf("(\"%s\") command is not supportet by bluetooth!!\n", cmd);
		rcolor;
		return 1;
	}
	//PARAM SEARCH
	for (uint8_t i = 0; cm[i] != '\0'; i++) {
		if (cm[i] == '<' && cm[i + 1] != '\0' && cm[i + 2] != '\0' && cm[i + 1] == 'P' && cm[i + 2] == '>') {
			//	printf("Parameter found!\n");
				//printf("%s , (char) %c , %s\n", cm, cm[i], &cm[i + 3]);
			strcpy(cp, cm);
			cm[i] = '\0';

			//printf("%p \"%c\"\n", &cp[i + 3], cp[i + 3]);
			cpp = &cp[i + 3];
			//printf("%p\n", cpp);


			//printf("Replace parameter to %s!\n", paramin);
			strcat(cm, paramin);
			strcat(cm, cpp);
			//printf("param after=\"%s\"\n", cpp);
			//printf("Str = \"%s\"\n", cm);
		}
	}
	//printf("PUT IN %s\n", cm);
	if (!db_cBtsend(cm))
		return 0;
	if (!db_cBtget(ou))
		return 0;
	bool aparam = 0;
	if (!db_cBtAnalysis(ou, &num, oux)) {
		color(ROTh);
		printf("Analystic error!\n");
		strcpy(paramout, "nan");
		rcolor;
		return 1;
	}
	if (num != 0) {
		//printf("GET = %s\n", oux);

		//printf("GET OUT %s\n", oux);
		//PARAM SEARCH
		char c = '\0';
		strcpy(cm, cm1);
		c = cm[strlen(cm) - 1];
		//printf("Char end %c\n", c);
		if (c == '?')
			cm[strlen(cm) - 2] = 'G';
		else
			cm[strlen(cm) - 1] = 'G';
		//printf("Search cmd : %s\n", cm);
		if (bt_get_cmd((char*)cm, ou)) {
			cm[0] = '\0';
			/*printf("Recive should be: \"");
			printf(ou);
			printf("\"\n");*/
			for (uint8_t i = 0; ou[i] != '\0'; i++) {
				if (ou[i] == '<' && ou[i + 1] != '\0' && ou[i + 2] != '\0' && ou[i + 1] == 'P' && ou[i + 2] == '>') {
					cpp = &ou[i + 3];
					/*printf("At end \"");
					printf(cpp);
					printf("\"\n");*/
					memcpy(cm, ou, i);
					cm[i] = '\0';
					/*printf("At begin \"");
					printf(cm);
					printf("\"\n");*/
					aparam = 1;
				}
			}
			char c2[2] = { '\0','\0' };

			if (aparam) {
				if (cm[0] != '\0' && 0 != memcmp(cm, oux, strlen(cm))) {
					color(ROTh);
					printf("Begin str:\"");
					for (uint8_t i = 0; i != strlen(cm); i++) {
						printf("%c", oux[i]);
					}
					printf("\" should be: \"");
					for (uint8_t i = 0; i != strlen(cm); i++) {
						printf("%c", cm[i]);
					}
					printf("\"\n\tHave a look in HM file (command \"%s\")\n", cm1);
					rcolor;
					return 1;
				}
				if (cm[0] == '\0') {
					if (cpp[0] == '\0') {
						strcpy(paramout, oux);
						return 1;
					}
					strcpy(paramout, "");
					for (uint8_t i = 0; 0 != strcmp(oux + i, cpp) && oux[i] != '\0'; i++) {
						c2[0] = oux[i];
						strcat(paramout, c2);
						if (oux[i + 1] == '\0' && cpp[0] != '\0') {
							color(ROTh);
							printf("No ending line detected after paremeter by BT input!!!\n");
							cScError("No ending line detected after paremeter by BT input!!!\n");
							rcolor;
						}
					}
					return 1;
				}
				else {
					strcpy(paramout, "");
					for (uint8_t i = strlen(cm); 0 != strcmp(oux + i, cpp) && oux[i] != '\0'; i++) {
						c2[0] = oux[i];
						strcat(paramout, c2);
						if (oux[i + 1] == '\0' && cpp[0] != '\0') {
							color(ROTh);
							printf("No ending line detected after paremeter by BT input!!!\n");
							cScError("No ending line detected after paremeter by BT input!!!\n");
							rcolor;
						}
					}
					return 1;
				}
			}
		}
		else {
			printf("%s is not defined in HM file!\n", cm);
		}

	}
	return 1;
}
void bT_CONVERT(char* in) {
	int i = -1;
	char c = '\0';
	char* cp;
	char ca[101] = "";
	char cx[101] = "";
	do {
		i++;
		//printf("char %c\n", cy[i]);
		switch (in[i]) {
		case '\\':
			//printf("active (%c) ", cy[i + 1]);
			switch (in[i + 1]) {
			case '_':c = ' ';  break;
			case 'n':c = '\n'; break;
			case 't':c = '\t'; break;
			case 'r':c = '\r'; break;
			case '\\':c = '\\'; break;
			default:c = in[i + 1];
			}
			//printf(": will :\"%c\"\n", c);
			cx[0] = c;
			cx[1] = '\0';
			in[i] = '\0';
			if (c != ' ')
				cp = (in + i + 2);
			else
				cp = (in + i + 2);

			//printf("cp = %s\n", cp);
			strcpy(ca, in);
			strcat(ca, cx);
			strcat(ca, cp);
			strcpy(in, ca);
			//printf("OUT \"");
			//printf(cy);
			//printf("\"\n");
			break;
		default:break;
		}
	} while (in[i] != '\0');

}
bool db_cBT() {
	printf("In this sketch you can send and read data from the bluetooth module!\n");
	printf("Do you want to skip this? (y/n)\n");
	if ('y' == _getch())
		return 1;
	printf("Do you need an App for BLE terminal (Android and IOS)? (y/n)\n");
	if ('y' == ynrespond()) {
		system("start https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal");
		system("start https://apps.apple.com/de/app/bluetooth-terminal/id1058693037");
	}
	do {
		color(GELBh);
		printf("Send data to BT ...\n");
		rcolor;
		char err[255] = "";
		/*color(GELBh);
		printf("Type in 'h' in order to use hexadecimal\n");
		rcolor;*/
		printf("\tType in your message for BT\n:");

		if (1 || 'h' != _getch()) {
			printf("ASCII\t '\\t', '\\n', '\\r' ' ' ...\n");
			do {
				gets_s(err, 251);
			} while (err[1] == '\0');
			bT_CONVERT(err);
			if (!db_cBtsend(err))return 0;
			err[0] = '\0';
		}/*
		else {
			color(GELBh);
			printf("HEX:");
			rcolor;
			printf("Type in the length:");
			char* datP = NULL;
			char* datPB = NULL;
			int len = 0;
			do {
				len=int_inp();
			} while (len - 2 > 255);
			datP = (char*)malloc(len);
			if (datP == NULL) {
				color(ROTh);
				printf("No RAM aviable!\n");
				rcolor;
				return 0;
			}
			for (int i = 0; i != len; i++) {
				printf("NUM %02i: ");
				datPB[i] = 0;
					scanf("%x", &datP[i]);
				flush();
				datPB
			}
			db_cBtsend(datP, len);
			free(datP);
			free(datPB);
		}*/

		printf("BT get data ...\n");

		if (!db_cBtget(err)) return 0;
		//printf(err);
		//printf("\"\n");
		uint8_t sat;
		char out[300];
		char* pout;
		pout = out;
		if (!db_cBtAnalysis(err, &sat, out)) {
			color(ROTh);
			printf("Data ERROR!!!\n");
			rcolor;
			cScError("Data ERROR!!!\n");
		}
		else {
			color(GRUENh);
			printf("Data packages: %i\n", sat);
			rcolor;
			for (uint8_t i = 0; i != sat; i++) {
				color(GRUENd);
				printf("Pack %i : \"", i + 1);
				color(GRUENh);
				printf(pout);
				color(GRUENd);
				printf("\"\n");
				rcolor;
				pout += strlen(pout) + 1;
			}
		}
		printf("Again? (y/n)\n");
	} while ('n' != _getch());
	return 1;
}
#define IRS_FRONT 0
#define IRS_BACK 1
#define IRS_LEFT 2
#define IRS_RIGHT 3
bool db_irs(uint8_t* value) {
	uint8_t in[12];
	uint8_t out[DB_MAXLENGTH];
	uint8_t x = 255;
	/*switch (sensor) {
	case IRS_FRONT:x = 0x80; break;
	case IRS_BACK:x = 0x20; break;
	case IRS_LEFT:x = 0x10; break;
	case IRS_RIGHT:x = 0x40; break;
	default:return 0;
	}*/
	x = 0;
	x |= 0x80;
	x |= 0x20;
	x |= 0x10;
	x |= 0x40;
	in[0] = x;
	if (!db_cmd(DB_IRS, 1, in, out))
		return 0;

	memcpy(value, &(out[2]), 4);

	/*	printf("input %i\n", sensor);
		for (uint8_t i = 0; i != 4; i++)
			printf("%i sas %i\n", i, out[2 + i]);*/

			//printf("Choose %i (%i)\n", x, *value);
	return 1;
}
uint8_t analysis_irs(int sensor, uint8_t value[]) {
	uint8_t x = 0;
	switch (sensor) {
	case IRS_FRONT: x = 0; break;
	case IRS_BACK: x = 1; break;
	case IRS_LEFT: x = 2; break;
	case IRS_RIGHT:x = 3; break;
	}
	return value[x];
}


bool db_Tirs() {
	uint8_t mess[DB_MAXLENGTH];
	printf("In this sketch the infreared sensors are going to messureing the distances of the next object!\n");
	bool b = 0;
	unsigned int in = 0;
	color(GRUENh);

	printf("Press any key to start!\n");
	rcolor;

	_getch();
	do {
		in++;
		b = 0;
		uint8_t i = 0;
		//printf("Status %3.3lf%%\r", 0);
		/*for (i = 0; i != 4 && db_irs(i, &mess[i]); i++) {
			printf("Status %3.3lf%%\r", percent(4, i));
		}*/
		if (!db_irs(mess))
			return 0;
		//analysis_irs(0,mess);
		//printf("Status %3.3lf%%\n", percent(4, 4));
		printf("The messurements are: front: %i cm\tback: %i cm\tleft: %i cm\tright: %i cm\n", mess[0], mess[1], mess[2], mess[3]);
		if (in < 5) {
			color(ROTd);
			printf("\tWarning these values are messured from ADC, no security if they are realy connected!!\n\tRetry this process several times!!\n");
			rcolor;
		}

		printf("Again? (y/n)\n");
	} while ('y' == _getch());
	cSaskIfFine();
	return 1;
}
#define USS_FRONT 0x08
#define USS_BACK 0x02
#define USS_LEFT 0x01
#define USS_RIGHT 0x04
bool db_uss_result(uint8_t sensor, uint8_t* value) {
	uint8_t in[DB_MAXLENGTH];
	uint8_t out[DB_MAXLENGTH];/*
	if (!db_cmd(DB_CLEAR, 1, in, out))
		return 0;
	*/
	uint8_t x = 0;
	//printf("RESULT sensor %i\n ", sensor);
	switch (sensor) {
	case 0:x = USS_FRONT; break;
	case 1:x = USS_BACK; break;
	case 2:x = USS_LEFT; break;
	case 3:x = USS_RIGHT; break;
	}
	in[0] = x;
	//printf("\tval %02X\n", in[0]);

	if (!db_cmd(DB_USS_G, 1, in, out))
		return 0;
	if (out[1] != 0)
		*value = out[2];
	else
		*value = 0;
	return 1;
}
bool db_uss(uint8_t sensor) {
	uint8_t in[DB_MAXLENGTH];
	//uint8_t out[DB_MAXLENGTH];
	/*
	if (!db_cmd(DB_CLEAR, 1, in, out))
		return 0;
	*/
	uint8_t x = 0;
	//printf("sensor %i \n", sensor);
	switch (sensor) {
	case 0:x = USS_FRONT; break;
	case 1:x = USS_BACK; break;
	case 2:x = USS_LEFT; break;
	case 3:x = USS_RIGHT; break;
	}
	in[0] = x;
	//printf("\t val %02X\n", in[0]);
	if (!db_cmd(DB_USS_M, 1, in, NULL))
		return 0;
	//wait(200);
	//if (!db_cmd(DB_USS_G, 1, in, out))
	//	return 0;
	//*value = out[2];
	return 1;
}
bool db_Tuss() {
	printf("In this sketch the values of the uss will be displayed on screen!\n");
	uint8_t sensou[4] = { 0 ,0 ,0, 0 };
	uint8_t i = 0;
	char cv[100] = "";
	char s[100] = "";

	printf("Push any key to start!\n");
	_getch();
	do {
		/*for (i = 0; i != 4; i++) {
			switch (i) {
			case 0: strcpy(cv, "front"); break;
			case 1: strcpy(cv, "back"); break;
			case 2: strcpy(cv, "left"); break;
			case 3: strcpy(cv, "right"); break;
			}
			color(GELBh);
			printf("Is the %s sensor connected? (y/n)\n", cv);
			rcolor;
			if ('y' == _getch()) {
				if (!db_uss(i)) {
					db_reset();
					return 0;
				}
			}
			//if (i == 2)
			//	db_reset();
		}
		*/
		for (uint8_t i = 0; i != 4; i++) {
			if (!db_uss(i)) {
				db_reset();
				return 0;
			}
		}
		for (i = 0; i != 4; i++) {
			if (!db_uss_result(i, &sensou[i])) {
				db_reset();
				return 0;
			}
		}
		//if (i == 2)
		//	db_reset();
	/*}
	for (i = 0; i != 4; i++) {
		switch (i) {
		case 0: strcpy(cv, "front"); break;
		case 1: strcpy(cv, "back"); break;
		case 2: strcpy(cv, "left"); break;
		case 3: strcpy(cv, "right"); break;
		}
		printf("%s", cv);
		printf(" : ");
		if (sensou[i] == 0) {
			printf("nan");
			cv[0] += 'A' - 'a';
			sprintf(s, "%s sensor is not connected or had to much distance to messure\n", cv);
			cScError(s);
		}
		else {
			printf("%i cm", sensou[i]);
			sprintf(s, "Sensor %s was messureing %i cm\n", cv, sensou[i]);
			cSaddMsg(s);
		}
		printf("\t");
	}*/

		printf("The messurements are: ");
		for (i = 0; i != 4; i++) {
			switch (i) {
			case 0: strcpy(cv, "front"); break;
			case 1: strcpy(cv, "back"); break;
			case 2: strcpy(cv, "left"); break;
			case 3: strcpy(cv, "right"); break;
			}
			printf("%s", cv);
			printf(" : ");
			if (sensou[i] == 0) {
				color(ROTh);
				printf("nan");
				rcolor;
				cv[0] += 'A' - 'a';
				sprintf(s, "%s sensor is not connected or had to much distance to messure!\n", cv);
				cScError(s);
			}
			else {
				color(GRUENh);
				printf("%i cm", sensou[i]);
				rcolor;
				sprintf(s, "Sensor %s was messureing %i cm\n", cv, sensou[i]);
				cSaddMsg(s);
			}
			printf("\t");
		}
		printf("\n");
		for (i = 0; i != 4; i++) {
			if (sensou[i] == 0) {
				color(ROTd);
				printf("nan means the sensor is not connected\n\t or give no request\n\tor the distance is to high!\n");
				rcolor;
				break;
			}
		}

		printf("Again? (y/n)\n");
	} while ('y' == _getch());
	cSaskIfFine();
	//free(&cv);
	return 1;
}
bool db_Tbm() {
	uint8_t out[DB_MAXLENGTH];
	char s[201] = "";

	printf("In this sketch the brightness sensors are going to messureing the brightness of the front!\n");
	do {
		if (!db_cmd(DB_BM, 0, NULL, out))
			return 0;
		printf("The messurements are: left: %0.2f%% (%i of 255)\tright: %0.2f%% (%i of 255)\n", (float)((float)out[2] / 255) * 100, out[2], (float)((float)out[3] / 255) * 100, out[3]);
		sprintf(s, "The messurements are:\n\tleft: %0.2f %% (%i of 255)\n\tright: %0.2f%% (%i of 255)\n", (float)((float)out[2] / 255) * 100, out[2], (float)((float)out[3] / 255) * 100, out[3]);
		cSaddMsg(s);
		printf("Again? (y/n)\n");
	} while (_getch() == 'y');
	cSaskIfFine();
	return 1;
}
bool bt_askUseF(char* filename) {
	filename[0] = '\0';
	DIR* dir;
	struct dirent* ent;
	if ((dir = opendir("HM\\")) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			printf("%s\n", ent->d_name);
		}
		printf("END dir");
		closedir(dir);
		return 1;
	}
	else {
		printf("Fail open dir\n");
		return 0;
	}
}
int strlcmp(char* x1, char* x2) {
	int i = 0;
	for (i = 0; x1[i] != '\0' && x2[i] != '\0'; i++) {
		if (x1[i] != x2[i]) {
			if (x1[i] > 'a' && x1[i] < 'z') {
				if ((x1[i] + 'A' - 'a') != x2[i]) {
					break;
				}
			}
			else
				if (x1[i] > 'A' && x1[i] < 'z´Z') {
					if ((x1[i] + 'a' - 'A') != x2[i]) {
						break;
					}
				}
				else {
					break;
				}

		}
		else
			break;
	}
	if (x1[i] != x2[i])
		return 0;
	return 1;
}
bool show_bt_dir(char* filename) {
	DIR* dir;
	FILE* f = NULL;
	struct dirent* ent;
	const int max_len = 103;
	char tex[max_len] = "";
	int maxFiles = 0;
	char* names;
	char* ccp = NULL;
	int namesp = 0;
	char text[201] = "";
	if ((f = fopen("HM\\use.TXT", "r")) != NULL) {
		strcpy(text, "HM\\");
		int i = strlen(text);
		while ((text[i] = fgetc(f)) != EOF) {
			i++;
		}
		text[i] = '\0';
		//printf("FILE : %s\n", text);
		strcat(filename, &text[strlen("HM\\")]);
		fclose(f);
		if ((f = fopen(text, "r")) != NULL) {

			//	printf("FILE : %s\n", text);
			fclose(f);
			return 1;
		}
		else {
			color(ROTh);
			printf("Invalid input at file \"HM\\use.TXT\"\n");
			rcolor;
			color(GELBh);
			printf("Fix bug...\n");
			rcolor;
		}
	}
	else {
		color(ROTh);
		printf("File \"HM\\use.TXT\" not found!\n");
		color(GELBh);
		printf("Create one...\n");
		rcolor;
	}
	if ((dir = opendir("HM\\")) != NULL) {
		color(GRUENh);
		printf("Open \"HM\\\" directory...\n");
		rcolor;
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_name[0] != '.') {
				if (strlcmp((char*)".TXT", &ent->d_name[ent->d_namlen - 4])) {
					if (strcmp(ent->d_name, "use.TXT")) {
						strcpy(tex, ent->d_name);
						tex[ent->d_namlen - 4] = '\0';
						color(GELBh);
						printf("(%i)", ++maxFiles);
						rcolor;
						printf("\t %s\n", tex);
					}
				}
			}
		}
		rewinddir(dir);
		if ((names = (char*)malloc(maxFiles * max_len)) == NULL) {
			color(ROTh);
			printf("Not enought free RAM-memory available!!(Error)\n");
			rcolor;
			printf("Press any key!\n");
			_getch();
			return 0;
		}
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_name[0] != '.') {
				if (strlcmp((char*)".TXT", &ent->d_name[ent->d_namlen - 4])) {
					if (strcmp(ent->d_name, "use.TXT")) {
						strcpy(tex, ent->d_name);
						tex[ent->d_namlen - 4] = '\0';
						//	printf("\t %s\n", tex);
						memcpy(&names[namesp], tex, strlen(tex) + 1);
						namesp += (strlen(tex) + 1);

					}
				}
			}
		}
		//printf("END dir");
		closedir(dir);


		printf("%i file%c found\n", maxFiles, (maxFiles == 1) ? '\0' : 's');
		if (maxFiles == 1) {
			color(GRUENh);
			printf("Auto choose \"%s\"! If you want to change delete \"\\HM\\use.txt\"!\n", names);
			rcolor;
			printf("Press any key!\n");
			_getch();
			strcpy(filename, names);
			strcat(filename, ".TXT");
			f = fopen("HM\\use.TXT", "w+");
			if (f != NULL) {
				fprintf(f, "%s", filename);
				fclose(f);
			}
			free(names);
			return 1;
		}
		else {

			int sel = 0;
			do {
				color(GELBh);
				printf("Select a file: ");
				rcolor;
				sel = int_inp();
				if (sel <= 0 || sel > maxFiles) {
					color(ROTh);
					printf("Invalid input!!\n");
					rcolor;
				}
			} while (sel <= 0 || sel > maxFiles);
			ccp = &names[strlen(names) + 1];
			if (sel != 1)
			{
				//	printf("ccp %s\n", ccp);
				for (sel -= 2; sel != 0; sel--) {
					ccp = &ccp[strlen(ccp) + 1];
					//printf("ccp %s\n", ccp);
				}
				strcpy(filename, ccp);
			}
			else
			{
				strcpy(filename, names);
				printf("name: %s\n", filename);
			}
			color(GRUENh);
			printf("Selected file \"%s\"\n", filename);
			rcolor;
			free(names);
			strcat(filename, ".TXT");
			f = fopen("HM\\use.TXT", "w+");
			if (f != NULL) {
				fprintf(f, "%s", filename);
				fclose(f);
			}
			return 1;
		}
	}
	color(ROTh);
	printf("Open \"HM\\\" directory...\n");
	printf("No dir!\n");
	rcolor;
	_getch();
	return 0;
}
bool bt_get_cmd(const char* in, char* out) {
	FILE* f;
	char filename[101] = "";
	char filenameX[101] = "";
	char line[101] = "";
	show_bt_dir(filename);
	if ((f = fopen("HM\\use.TXT", "r")) == NULL) {

		if ((f = fopen("HM\\use.TXT", "a+")) == NULL) {
			color(ROTh);
			printf("File can't be generated!\n");
			rcolor;
			return 0;
		}
		else {
			fprintf(f, "BT_version.TXT");
			fclose(f);
			return bt_get_cmd(in, out);
		}
	}
	fclose(f);
	f = fopen("HM\\use.TXT", "a+");
	if (f == NULL) {
		color(ROTh);
		printf("File can't be generated!\n");
		rcolor;
		return 0;
	}
	fscanf(f, "%s", filename);
	if (filename[0] == '\0')
		fprintf(f, "BT_version.TXT");

	fclose(f);
	if (filename[0] != '\0') {
		strcpy(filenameX, "HM\\");
		strcat(filenameX, filename);

		f = fopen(filenameX, "r");
		//	printf("Open file %s\n", filenameX);
		if (f == NULL) {
			color(ROTh);
			printf("File can't be open!!\n");
			rcolor;
			bell();
		}
	}

	if (filename[0] == '\0' || f == NULL) {
		color(ROTh);
		printf("The setting in File \"%s\" to open \"%s\" is invalid!! (directory: HM\\)\nType in an valid File!!\n", "use.TXT", filename);
		if (f != NULL)
			fclose(f);
		bell();
		system("HM\\use.txt");
		return bt_get_cmd(in, out);
	}

	char c = '\n';
	int i = 0;
	char l[101] = "";
	char cx[101] = "";
	char cy[101] = "";
	char ca[101] = "";
	char* cp;
	do {
		line[0] = '\0';
		c = '\n';
		i = 0;
		strcpy(cx, "");
		strcpy(cy, "");
		do {
			fscanf(f, "%s %s", cx, cy);
		} while (c != '\n' && c != EOF);
		if (c != EOF) {


			//printf("%s %s\n", cx, cy);
			if (cy[0] == '\0' && cx[0] == '\0')
			{
				//printf("NONE\n");
				return 0;
			}
			else
			{
				//printf("CMP \"%s\"==\"%s\"\n", in, cx);

				if (!strcmp(in, cx)) {
					//printf("YASSS!\n");
					if (!strcmp(cy, "\\-")) {
						strcpy(out, cy);
						return 1;
					}
					bT_CONVERT(cy);
					strcpy(out, cy);
					//printf("out be : \"");
					//printf(out);
					//printf("\"\n");
					return 1;
				}
			}
		}
	} while (c != EOF);
	color(ROTh);
	printf("NO IDEA!\n");
	rcolor;
	return 0;
}
bool bt_chng_config(char* cmd, bool askneed) {
	char out[22] = "";
	char paramin[22] = "";
	char paramout[22] = "";
	char text[51] = "";
	if (askneed) {
		color(GELBh);
		printf("Type in your parameter: ");
		rcolor;
		do {
			gets_s(paramin, sizeof(paramin));
		} while (!strcmp(paramin, ""));
	}
	if (!db_BTWC(cmd, out, paramin, paramout))
	{
		return 0;
	}
	if (askneed) {

		color(GRUENh);
		printf("Verify\n");
		rcolor;
		if (!db_BTWC(cmd, out, NULL, paramout)) {
			return 0;
		}
		if (!strcmp(paramin, paramout)) {
			color(GRUENh);
			sprintf(text, "Parameter verified, written \"%s\"\n", paramout);
			printf(text);
			cSaddMsg(text);
			rcolor;
			return 1;
		}
		else {
			color(ROTh);
			sprintf(text, "Parameter verified, written \"%s\" insted of \"%s\"\n", paramout, paramin);
			printf(text);
			cScError(text);
			rcolor;
			return 0;
		}
	}
	//printf("Returned : %s\n", paramout);
	return 1;
}
bool bt_configchng() {
	color(GELBh);
	printf("Select option:\n");
	rcolor;
	char chang[][101] = { {"name"},{"pin"},{"type"},{"sleepmode"},{"system LED"},{"role"},{"UUID"},{"notify"},{"stoppbits"},{"parity"},{"at begin"},{"connection timeout"},{"reset"},{"factory reset"},{""} };
	char changbt[][101] = { {"NAME"},{"PASS"},{"TYPE"},{"PWR"},{"LED"},{"ROLE"},{"UUID"},{"NOTI"},{"STOP"},{"PARI"},{"IMME"} ,{"TCON"},{"RESET"},{"RENEW"} };
	bool chanbtb[100];
	bool ask[101];
	for (int i = 0; i != 12; i++) {
		ask[i] = 1;
	}
	ask[13] = 0;
	ask[12] = 0;
	int enter = 0;
	int d = 0;
	//bool askneed = 1;
	char xin[101] = "";
	char xout[101] = "";
	int cntd = 0;
	for (d = 0; chang[d][0] != '\0'; d++) {
		chanbtb[d] = 0;
		strcpy(xin, changbt[d]);
		strcat(xin, "S");

		bt_get_cmd(xin, xout);
		if (strcmp(xout, "\\-")) {
			chanbtb[d] = 1;
			cntd++;
			printf("(%02i) change :\t\"", cntd);
			color(GELBh);
			printf(chang[d]);
			rcolor;
			printf("\" \n");
		}
		else {
			color(ROTh);
			printf("\t\t%s not suportet\n", chang[d]);
			rcolor;
		}
	}
	do {
		color(GELBh);
		printf("Select your operation: ");
		rcolor;
		enter = int_inp();
	} while (enter <= 0 || enter > cntd);
	int x = 0;
	int xc = 0;
	for (xc = 0; (enter) != x; xc++) {
		if (chanbtb[xc]) {
			x++;
		}
		//	printf("%s is %i cnt %i\n", chang[xc], chanbtb[xc], x);
	}
	enter = xc - 1;

	//printf("num %i\n", enter);

	//printf("Change %s\n", chang[enter - 1]);
	color(GRUENh);
	printf("Change %s is in process...\n", chang[enter]);
	rcolor;
	if (!bt_chng_config(changbt[enter], ask[enter])) {
		color(ROTh);
		printf("An error occurred!\n");
		bell();
		rcolor;
	}
	else {
		color(GRUENh);
		printf("Settings successfully written!\n");
		rcolor;
	}

	return 1;
}
bool bt_os(bool val) {
	uint8_t paramin = val;
	return db_cmd(DB_BT_OS, 1, &paramin, NULL);
}
bool bt_config() {
	char out[36];
	char paramin[36];
	char paramout[36];
	for (uint8_t i = 0; i != strlen("Bluetooth configuration!!!\n") - 1; i++) {
		printf("*");
	}
	printf("\n");
	printf("Bluetooth configuration!!!\n");
	for (uint8_t i = 0; i != strlen("Bluetooth configuration!!!\n") - 1; i++) {
		printf("*");
	}
	printf("\n");
	printf("Here you can see the basic information of your BLE module!\n");
	color(GELBh);
	printf("\tThis might need some time please be patient!\n");
	rcolor;
	bool b = 0;
	bool bxx = 0;
	int err = 0;
	bool x = 0;
	char filenam[201] = "";
	do {
		b = 0;
		x = 0;
		char cx[101] = "";
		color(GELBh);
		printf("Sending an test sequence to module!...\n");
		rcolor;
		switch (err = db_BTAT()) {
		case -1:color(ROTh); printf("Connection error's!\n"); rcolor; return 0; break;
		case 0:color(ROTh); printf("BT file error!\n"); rcolor; b = 1; break;
		case 1:color(GRUENh); printf("Everything work fine!\n"); rcolor; break;
		case 2:color(ROTh); printf("Wrong or no request!\n"); rcolor;
			color(GELBh);
			printf("Select other modules? (y/n)\n");
			rcolor;
			b = 1;
			if (ynrespond() == 'y') {
				FILE* f;
				if ((f = fopen("HM\\use.TXT", "w+")) != NULL) {
					if (show_bt_dir(filenam)) {
						fprintf(f, "%s", filenam);
						x = 1;
						b = 0;
					}

					fclose(f);
				}
				else {
					color(ROTh);
					printf("File error!!!\n");
					rcolor;
				}

			}
			break;
		case 3:color(ROTh); printf("Analystic error!\n"); rcolor; b = 1; break;
		}
		rcolor;
		if (b) {
			bxx = 1;
			printf("Do you want to retry? (y/n)\n");
			if (_getch() == 'n')
				bxx = 0;
		}
	} while (b && bxx || x);
	switch (err) {
	case -1:return 0; break;
	case 0:cScError("BT file error!\n"); break;
	case 1:cSaddMsg("AT request works fine!\n"); break;
	case 2:cScError("AT request works not fine (wrong/no request)!\n"); break;
	case 3:cSaddMsg("AT request works not fine (data analystic)!\n");  break;
	}
	if (b)
		return 1;

	color(GELBh);
	printf("Other information's might be shown here:\n");
	rcolor;

	printf("\n");
	color(GELBh);
	printf("Basic:\n");
	rcolor;
	db_BTWC("VERS", out, NULL, paramout);
	color(GRUENh);
	printf("Version: \""); printf(paramout); printf("\"\n");
	rcolor;

	db_BTWC("NAME", out, NULL, paramout);
	color(GRUENh);
	printf("Name: \""); printf(paramout); printf("\"\n");
	rcolor;

	db_BTWC("PASS", out, NULL, paramout);
	color(GRUENh);
	printf("Pin: \""); printf(paramout); printf("\"\n");
	rcolor;// 

	db_BTWC("TYPE", out, NULL, paramout);
	color(GRUENh);
	printf("Type: \"");
	if (!ablechint(paramout[0]))
	{
		switch (paramout[0] - '0') {
		case 0:printf("Not need PIN Code"); break;
		case 1:printf("Auth not need PIN"); break;
		case 2:printf("Auth with PIN"); break;
		case 3:printf("Auth and bond"); break;
		default:printf("nan"); break;
		}
	}
	else
		printf(paramout);
	printf("\"\n");
	rcolor;

	db_BTWC("PWR", out, NULL, paramout);
	color(GRUENh);
	//printf(paramout);
	printf("Sleepmode: ");
	switch (paramout[0] - '0') {
		//case :color(ROTh); printf("wrong callback! \""); printf(paramout); printf("\""); break;
	case 1:printf("auto"); break;
	case 0:printf("off"); break;
	default: printf(paramout); break;
	}
	printf("\n");
	rcolor;

	db_BTWC("LED", out, NULL, paramout);
	color(GRUENh);
	printf("System LED: \"");
	if (!ablechint(paramout[0]))
	{
		switch (paramout[0] - '0') {
		case 0:printf("Connected High; Unconnected blink (500ms)"); break;
		case 1:printf("Connected High; Unconnected Low"); break;
		default:printf("nan ("); printf(paramout); printf(")"); break;
		}
	}
	else
		printf(paramout);
	printf("\"\n");
	rcolor;

	db_BTWC("TEMP", out, NULL, paramout);
	color(GRUENh);
	printf("Temperature: %s%cC\n", paramout, DEGREE);
	rcolor;

	db_BTWC("BATT", out, NULL, paramout);
	color(GRUENh);
	printf("Battery: "); printf(paramout); printf("%%\n");
	rcolor;

	db_BTWC("HELP", out, NULL, paramout);
	color(GRUENh);
	printf("Help: "); printf(paramout); printf("\n");
	rcolor;


	color(GELBh);
	printf("\nConnection BT:\n");
	rcolor;
	db_BTWC("ROLE", out, NULL, paramout);
	color(GRUENh);
	printf("Role: \"");
	if (!ablechint(paramout[0]))
	{
		switch (paramout[0] - '0') {
		case 0:printf("Peripheral"); break;
		case 1:printf("Central"); break;
		default:printf(paramout); break;
		}
	}
	else
		printf(paramout);
	printf("\"\n");
	rcolor;

	db_BTWC("UUID", out, NULL, paramout);
	color(GRUENh);
	printf("UUID of service: %s\n", paramout);
	rcolor;

	db_BTWC("RSSI", out, NULL, paramout);
	color(GRUENh);
	printf("RSSI: %s\n", paramout);
	rcolor;

	db_BTWC("ADDR", out, NULL, paramout);
	color(GRUENh);
	printf("Mac address: \""); printf(paramout); printf("\"\n");
	rcolor;

	db_BTWC("RADD", out, NULL, paramout);
	color(GRUENh);
	printf("Mac address of last connected device: \""); printf(paramout); printf("\"\n");
	rcolor;

	db_BTWC("TCON", out, NULL, paramout);
	color(GRUENh);
	printf("Time how long the connection should be hold to address: ");
	uint8_t xlen = 0;
	for (xlen = 0; paramout[xlen] != '\0'; xlen++) {
		if (paramout[xlen] != '0') {
			break;
		}
	}
	if (paramout[xlen] != '\0') {
		printf(paramout);
		printf(" ms");
	}
	else {
		printf("\"Connect forever\"");
	}
	printf("\n");
	rcolor;


	color(GELBh);
	printf("\nConnection UART:\n");
	rcolor;

	db_BTWC("NOTI", out, NULL, paramout);
	color(GRUENh);
	printf("Notify: %s\n", strcmp(paramout, "1") == 0 ? "true" : (strcmp(paramout, "0") == 0 ? "false" : "nan"));
	rcolor;

	db_BTWC("STOP", out, NULL, paramout);
	color(GRUENh);
	printf("Stopbits: ");
	if (ablechint(paramout[0])) { printf(paramout); }
	else {
		printf("%c", paramout[0] + 1);
		printf(" bit");
		if (paramout[0] + 1 != '1')
			printf("'s");
	} printf(" (recommend 2 bit's)\n");
	rcolor;

	db_BTWC("PARI", out, NULL, paramout);
	color(GRUENh);
	printf("Parity: \"");
	if (!ablechint(paramout[0]))
	{
		switch (paramout[0] - '0') {
		case 0:printf("None"); break;
		case 1:printf("EVEN"); break;
		case 2:printf("ODD"); break;
		default:printf(paramout); break;
		}
	}
	else
		printf(paramout);
	printf("\" (recomment: None)\n");
	rcolor;

	db_BTWC("IMME", out, NULL, paramout);
	color(GRUENh);
	printf("At beginn: \"");
	if (!ablechint(paramout[0]))
	{
		switch (paramout[0] - '0') {
		case 1:printf("Only use AT Commands works, till the start command"); break;
		case 0:printf("Module work immediately"); break;
		default:printf(paramout); break;
		}
	}
	else
		printf(paramout);
	printf("\"\n");
	rcolor;
	color(ROTd);
	printf("Warning!! If you change \"Stopbits, Bautrate or Parity\" the module might be not ready to work!\n");
	printf("\tIf your module does not work please load (CCLoader) the \"HMSoft V540\" to the module, in order to get it work!\n");
	printf("\t\tDelete \"HM\\use.TXT\" and select \"HMSoft_V540\" for BT configuration at programm restart!\n");
	rcolor;

	printf("\n");
	for (uint8_t i = 0; i != strlen("Bluetooth configuration!!!\n") - 1; i++) {
		printf("*");
	}
	printf("\n");
	return 1;
}
void db_MC_length(uint16_t ticks, double* len_mm) {
	*len_mm = (double)((double)(ticks / DB_MC_TICKS_PER_TURN) * db_circumference_value);
}
void data_8216(uint8_t* in, uint16_t* out) {
	*out = (in[0] << 8) + in[1];
}
void data_1628(uint16_t in, uint8_t* out) {
	out[0] = (uint8_t)(in >> 8);
	out[1] = (uint8_t)(in & 0xff);
}
bool db_EEP(uint16_t addr, uint8_t* value, uint8_t get) {
	uint8_t paramin[3];
	paramin[2] = *value;
	uint8_t paramout[3];
	data_1628(addr, paramin);
	if (!get) {
		return db_cmd(DB_EEPROM, 3, paramin, NULL);
	}
	if (!db_cmd(DB_EEPROM, 2, paramin, paramout))
		return 0;
	*value = paramout[2];
	return 1;
}
bool db_circumference() {
	color(GELBh);
	printf("\t***Circumference settings\n");
	rcolor;
	uint8_t circumference[2];
	uint16_t circumferencex = 0;
	uint16_t circumferenceset = 0;
	int x = 0;
	if (!db_EEP(1, &circumference[0], 1))
		return 0;
	if (!db_EEP(0, &circumference[1], 1))
		return 0;
	data_8216(circumference, &circumferencex);
	color(GRUENh);
	if ((circumferencex < 180) || (circumferencex >= 230))
		printf("Circumference = %i mm (on default not saved in Memory)\n", (circumferencex = DB_MC_WHEEL_CIRCUMFERENCE));
	else
		printf("Circumference = %i mm\n", circumferencex);
	if (circumference[0] == 255 && circumference[1] == 255) {
		printf("\tThe chip was erased by this adress!\n");
	}
	db_circumference_value = circumferencex;
	char s[101] = "";
	sprintf(s, "The circumference is: %i mm\n", db_circumference_value);
	cSaddMsg(s);
	color(GELBh);
	printf("Do you wat to set a other circumference (unit mm)? (y/n)\n");
	rcolor;
	if ('y' == _getch()) {
		circumferencex = 0;
		do {
			printf("Enter valid value: ");
			x=int_inp();
			circumferencex = (uint16_t)x;
		} while ((circumferencex < 180) || (circumferencex >= 230));
		printf("Set value = %i\n", circumferencex);
		circumferenceset = circumferencex;
		if (db_circumference_value != circumferencex) {
			data_1628(circumferencex, circumference);
			if (!db_EEP(1, &circumference[0], 0))
				return 0;
			if (!db_EEP(0, &circumference[1], 0))
				return 0;
			color(GRUENh);
			printf("Verify...\n");
			rcolor;
			circumferencex = 0;
			if (!db_EEP(1, &circumference[0], 1))
				return 0;
			if (!db_EEP(0, &circumference[1], 1))
				return 0;
			data_8216(circumference, &circumferencex);
			sprintf(s, "\tThe circumference is changed to: %i mm\n", circumferencex);
			cSaddMsg(s);
		}
		else {
			color(GRUENh);
			printf("Writeing not nessersary!\n");
			rcolor;
		}

		color(GRUENh);
		if ((circumferencex < 180) || (circumferencex >= 230))
			printf("Circumference = %i (on default not saved in Memory)\n", (circumferencex = DB_MC_WHEEL_CIRCUMFERENCE));
		else
			printf("Circumference = %i\n", circumferencex);
		if (circumference[0] == 255 && circumference[1] == 255) {
			printf("\tThe chip was erased by this adress!\n");
		}
		rcolor;
		db_circumference_value = circumferencex;
		if (circumferenceset != circumferencex) {
			color(ROTh);
			cScError("The chip was unsuccessfully written!!!\n");
			printf("The chip was unsuccessfully written!!!\n");
		}
		else {
			color(GRUENh);
			cSaddMsg("The chip was successfully written!!!\n");
			printf("The chip was successfully written!!!\n");
		}
		rcolor;

	}
	return 1;
}
bool get_MC_Data(uint16_t* data) {
	uint8_t out[12 * 2 + 2] = { 0x46, 0x16, 0x0b, 0x2f, 0x03, 0x54, 0x0e, 0x83, 0x07, 0xf1, 0x06, 0x9d, 0x0e, 0x8e, 0x0a, 0x4e, 0x03, 0x18, 0x0e, 0x11, 0x07, 0x8f, 0x06, 0x78, 0x0e, 0xa7
	};
	//db_cmd(DB_MC_D, 0, NULL, out);
	//data[i] = data[i];

	for (uint8_t i = 0; i != 12 * 2 + 2; i++) {
		out[i] = 0;
	}
	db_cmd(DB_MC_D, 0, NULL, out);
	for (uint8_t i = 0; i != 12; i++) {
		data[i] = (out[(i * 2) + 2] << 8) + out[(i * 2) + 1 + 2];
		/*switch (i % 6) {
		case 0:printf("Dedected as forward Left:"); break;
		case 1:printf("Dedected as Backward Left:"); break;
		case 2:printf("Dedected as general Left:"); break;
		case 3:printf("Dedected as forward Right:"); break;
		case 4:printf("Dedected as Backward Right:"); break;
		case 5:printf("Dedected as general Right:"); break;
		}
		printf("Data %i %i\n", i, data[i]);*/
	}
	return 1;
}
void db_McAnaly(uint16_t* mc_dat, const char* xc) {
	color(GELBh);
	char s[101] = "";
	printf("***\tGeneral spin\n");
	rcolor;
	char dirs[2][26] = { "left","right" };
	for (uint8_t i = 0; i != 2; i++) {
		uint8_t diff = (((i == 1) ? 1 : 0) * 3);
		if ((mc_dat[0 + diff] + mc_dat[1 + diff]) == mc_dat[2 + diff]) {
			color(GRUENh);

			if (mc_dat[2 + diff] == 0) {
				color(ROTh);
			}
		}
		else
			color(ROTh);


		printf("%4i + %4i = %4i\t(%s wheel)\n", mc_dat[0 + diff], mc_dat[1 + diff], mc_dat[2 + diff], dirs[i]);
		if ((mc_dat[0 + diff] + mc_dat[1 + diff]) != mc_dat[2 + diff]) {
			//cScError("Motor forward buffer overdriven\n");
			sprintf(s, "Motor %s buffer overdriven\n\tThe wheels were to fast or wrong/not connected with the DiscBot!!!\n", xc);
			printf("\t%s", s);
			cScError(s);
		}
		if (mc_dat[2 + diff] == 0) {
			color(ROTh);
			sprintf(s, "The motor %s was not turning!\n", dirs[i]);
			printf("\t%s", s);
			cScError(s);

		}
		rcolor;
	}
	color(GRUENh);
	if (mc_dat[0 + 2] < mc_dat[3 + 2]) {
		printf("Left wheel spinned %3.3lf%% more than right!\n", 100 - percent(mc_dat[3 + 2], mc_dat[0 + 2]));
	}
	else {
		if (mc_dat[0 + 2] == mc_dat[3 + 2]) {
			printf("Both wheel's spinned exact the same distance!!\n");
		}
		else {
			printf("Right wheel spinned %3.3lf%% more than left!\n", 100 - percent(mc_dat[0 + 2], mc_dat[3 + 2]));
		}
	}
	rcolor;
	color(GELBh);
	printf("***\t%s drive\n", xc);

	double perc = 0;
	//percent(mc_dat[2], mc_dat[1]);

	if (!strcmp(xc, "Forward"))
		perc = percent(mc_dat[2], mc_dat[0]);
	else
		perc = percent(mc_dat[2], mc_dat[1]);
	const char xcv[] = "%% (should be a low value, under 50%)\n";
	sprintf(s, "%s left drive %3.3lf", xc, perc);
	strcat(s, xcv);
	if (50 > perc) {
		color(GRUENh);
		cSaddMsg(s);
	}
	else {
		color(ROTh);
		cScError(s);
	}
	printf(s);
	if (!strcmp(xc, "Forward"))
		perc = percent(mc_dat[2 + 3], mc_dat[0 + 3]);
	else
		perc = percent(mc_dat[2 + 3], mc_dat[1 + 3]);
	sprintf(s, "%s right drive %3.3lf", xc, perc);

	strcat(s, xcv);
	if (50 > perc) {
		color(GRUENh);
		cSaddMsg(s);
	}
	else {
		color(ROTh);
		cScError(s);
	}
	printf(s);
	rcolor;

}
#define LS_BYTES sizeof(DbLsNote)
#define N_P_T 38

bool db_transportSong(uint8_t len, const DbLsNote* notes, bool cat) {
	len++;
	DbLsNote noteL;
	noteL.pitch = DB_LS_LAST;
	noteL.loud = 0;
	uint8_t in[(N_P_T) * LS_BYTES+ (2)*LS_BYTES];
	uint16_t lenc = 0;
	uint8_t notx[LS_BYTES+ (2) * LS_BYTES];
	color(GELBh);

	for (lenc = 0; lenc != len; lenc++) {
		//printf("Frequency : %i Hz, length %i , loud: %i %%\n", notes[lenc].pitch, notes[lenc].length, notes[lenc].loud);
		printf("\t  Export of data %02.02lf %%\r", percent(len, lenc));
		struct DbLsNote notesx;
		notesx.pitch = notes[lenc].pitch;
		if (notesx.pitch > 1) {
			notesx.pitch = (uint16_t)(16000000 / notesx.pitch);
		}
		data_1628(notesx.pitch, notx);
		//notx[0] = notesx.pitch;
		//notx[1]=notesx.pitch;
		//data_1628(notes[lenc].length, &(notx[2]));
		notx[2] = notLenDB[notes[lenc].length];
		notx[3] = notes[lenc].loud;
		//printf("\n%i %i pitch %i Length %i loud\n", notx[0], notx[1], notx[2], notx[3]);
		for (uint8_t u = 0; u != LS_BYTES; u++) {
			in[u + (lenc * LS_BYTES)] = notx[u];
		}
	}
	color(GRUENh);
	printf("\t  Export of data %02.02lf %%\n", percent(len, len));
	rcolor;
	
	if (cat == 1)
		return db_cmd(DB_CAT_SONG, (len) * (LS_BYTES), in, NULL);
	return db_cmd(DB_COPY_SONG, (len ) * (LS_BYTES), in, NULL);

}
bool db_load_song_to_bot(uint8_t num) {
	color(ROTh);
	printf("\tEXPORT song nr %i to %s\n", num + 1, PER_DEVICE);
	rcolor;
	uint16_t len = 0;
	uint16_t lenc = 0;
	for (len = 0; dbLs_ALL[num][len].pitch != DB_LS_LAST; len++);
	printf("\t**Start transport of %i package%s %i note%s", len / N_P_T + ((len % N_P_T) != 0), ((len / N_P_T + ((len % N_P_T) != 0)) > 1) ? "s," : ",", len, ((len) > 1) ? "s\n" : "\n");
	if (len > N_P_T) {
		len -= N_P_T;
		if (!db_transportSong(N_P_T, dbLs_ALL[num], 0)) return 0;
		lenc += N_P_T;
	}
	else {
		if (!db_transportSong(len, dbLs_ALL[num], 0)) return 0;
		len = 0;
	}
	while (len != 0) {
		if (len > N_P_T) {
			if (!db_transportSong(N_P_T, &dbLs_ALL[num][lenc], 1)) return 0;
			lenc += N_P_T;
			len -= N_P_T;
		}
		else {
			if (!db_transportSong(len, &dbLs_ALL[num][lenc], 1)) return 0;
			lenc += len;
			len -= len;
		}
	}
	return 1;
}
bool db_mc() {
	char s[101] = "";
	char s1[101] = "";
	char s2[101] = "";
	char s3[101] = "";
	uint16_t mc_dat[12];
	if (!db_circumference()) {
		color(ROTh);
		printf("Circumference connection error\n");
		cScError("Circumference connection error\n");
		rcolor;
		return 0;
	}
	printf("In this sketch we're testing the functionallity of the motors and it's sensors!\n");
	color(ROTd);
	printf("\tWarning!! %s will start moveing!!!\n", PER_DEVICE);
	bool forw = 0;
	bool back = 0;
	bool right = 0;
	bool left = 0;
	do {
		forw = 0;
		back = 0;
		right = 0;
		left = 0;
		color(GELBh);
		printf("Push 'x' to start!!\n");
		rcolor;
		while (_getch() != 'x');
		color(GRUENh);
		printf("Wheel spining...\n");
		rcolor;
		if (db_cmd(DB_MC, 0, NULL, NULL) == 0) {
			return 0;
		}
		color(GELBh);
		for (int i = 0; i != 1750; i++) {
			printf("\rWork in process (%2.2lf%%)", percent(1750, i));
			wait(1);
		}
		color(GRUENh);
		printf("\rWork in process (100%%)");
		rcolor;
		printf(" - Finish\n");
		color(GRUENh);
		printf("Start analystics...\n");
		rcolor;
		if (!get_MC_Data(mc_dat)) {
			return 0;
		}
		color(GELBh);
		printf("\n***\tSpin forward\n");
		cSaddMsg("\n***\tSpin forward\n");
		rcolor;
		//todo: cScError("ERROR");
		double len[12];
		for (uint8_t i = 0; i != 12; i++) {

			sprintf(s, "Receive as ");
			bool imp = 0;

			switch (i % 6) {
			case 0:strcat(s, "forward lef");
				//important not 0
				forw = 1;
				left = 1;
				if (i < 6)
					imp = 1;
				break;
			case 1:strcat(s, "backward lef");
				back = 1;
				left = 1;
				//important not 0
				if (i > 6)
					imp = 1;
				break;
			case 2:strcat(s, "general lef"); break;
			case 3:strcat(s, "forward righ");
				forw = 1;
				right = 1;
				//important not 0
				if (i < 6)
					imp = 1;
				break;
			case 4:strcat(s, "backward righ");
				//important not 0
				back = 1;
				right = 1;
				if (i > 6)
					imp = 1;
				break;
			case 5:strcat(s, "general righ"); break;
			}

			//start color
			if (!imp || mc_dat[i]) {//mc_dat!=0
				color(GRUENh);
			}
			else {
				color(ROTh);
			}
			//end color
			if ((i % 3) == 0 && (i != 0) && i != 5) {
				printf("\n");
				cSaddMsg("\n");
			}

			db_MC_length(mc_dat[i], &len[i]);
			sprintf(s1, "t:\t%4i ticks\t(turn %4.1lf mm)\n", mc_dat[i], len[i]);
			strcat(s, s1);
			printf(s);

			if (mc_dat[i] == 0 && left && forw && mc_dat[i + 1] == 0) {
				printf("Fix PE4\n");
			}
			if (mc_dat[i] == 0 && right && forw && mc_dat[i + 1] == 0) {
				printf("Fix PE5\n");
			}
			if (mc_dat[i] == 0 && imp) {
				printf("\tWheel was not turning\n");
				cScError(s);
			}
			else {
				cSaddMsg(s);

			}

			rcolor;
			if (i == 5) {
				color(GELBh);
				printf("\n***\tSpin backward");
				cSaddMsg("\n***\tSpin backward");
				rcolor;
			}
		}
		color(GRUENh);
		printf("\nMain analcstics...\n");
		color(GELBh);
		printf("***Forward spin\n");
		db_McAnaly(mc_dat, "Backward");//forward
		color(GELBh);
		printf("\n***Backward spin\n");
		db_McAnaly(&mc_dat[6], "Forward");//Backward
		rcolor;
		printf("Again? (y/n)\n");
	} while (ynrespond() == 'y');

	return 1;
}

bool db_ircClearB() {
	uint8_t in[2] = { 0 ,0 };
	return db_cmd(DB_IRC_BUFFER, 1, in, NULL);
}
bool db_irc_send(uint8_t code[4]) {
	return db_cmd(DB_IRC_SEND, 4, code, NULL);
}
bool db_irc_recive(uint8_t* len, uint8_t* code) {
	uint8_t out[255];
	*len = 0;
	if (!db_cmd(DB_IRC_BUFFER, 0, NULL, out))
		return 0;
	*len = (out[1] / 4);
	for (uint8_t i = 0; i != out[1]; i++) {
		code[i] = out[i + 2];
	}
	return 1;
}
void db_irc_analystic(uint8_t len, uint8_t* code, uint8_t out[4]) {
	uint8_t lenx = 0;
	lenx = (len) * 4;
	for (uint8_t i = 0; i != 4; i++) {
		out[i] = code[len + i];
	}
}
bool db_ircT() {
	printf("In this scketch we're testing the InfraRed Communication!\n");
	printf("Make the IR module ready, then");
	color(GELBh);
	printf(" push any key!\n");
	rcolor;
	_getch();
	bool rec = 0;

	if (!db_ircClearB())
		return 0;
	color(GRUENh);
	printf("IRC buffer cleared!\n");
	rcolor;
	uint8_t len = 0;
	uint8_t cod[255];
	uint8_t codout[4];
	printf("Now you can test your ");
	color(GELBh);
	printf("IR remote");
	printf("!\n");
	color(GELBh);
	printf("Push enter to stop!\n");
	rcolor;
	do {
		if (!db_irc_recive(&len, cod))
			return 0;
		if (!db_ircClearB())
			return 0;
		for (uint8_t i = 0; i != len; i++) {
			db_irc_analystic(i, cod, codout);
			printf("The module recieves:");
			for (uint8_t u = 0; u != 4; u++)
				printf(" %02X", codout[u]);
			printf("\n");
			if (!rec)
				if (!dbLed_setColor(DB_LED_BUTTON_GREEN, 0, 255, 0))
					return 0;
			rec = 1;
			waitS(500, 1);
			if (!db_Beep())
				return 0;
		}
		if (rec)
			if (waitS(1500, 0)) {
				rec = 0;
				if (!dbLed_setColor(DB_LED_BUTTON_GREEN, 0, 0, 0))
					return 0;
			}

	} while (!anyKey());
	//wait(500);
	flush();
	if (!dbLed_setColor(DB_LED_BUTTON_GREEN, 0, 0, 0))
		return 0;
	printf("Now you can test your ");
	color(GELBh);
	printf("IR transmitter LED");
	rcolor;
	printf("!\n");
	color(ROTd);
	printf("\tYou might need a other %s (with IR reciver module) in order to test this\n\tDBOS / dbosX command 0x57 0x00\n", PER_DEVICE);
	rcolor;
	do {
		printf("Type in your Data to send via IR!\n");
		int inp = 0;
		for (uint8_t i = 0; i != 4; i++) {
			do {
				if (inp == 1000)
				{
					color(ROTh);
					printf("Wrong value try again!\n\tTry from \"00\" up to \"FF\"\n");
					rcolor;
				}
				inp = 1000;
				printf("Type in data byte %i (in HEX): ", i + 1);
				scanf("%X", &inp);
				flush();
			} while (inp == 1000);
			cod[i] = inp;
		}
		do {
			if (!db_irc_send(cod))
				return 0;
			color(GRUENh);
			printf("Succsessfully transmitted\n");
			color(ROTd);
			printf("\tCheck if this command would be sent!!!\n");
			rcolor;
			printf("Send same again? (y/n)\n");
		} while (ynrespond() == 'y');
		printf("Again? (y/n)\n");
	} while (ynrespond() == 'y');
	cSaskIfFine();
}

int wtn(char* c) {
	if (!strcmp(c, "btn"))
		return 0;
	if (!strcmp(c, "rfid"))
		return 1;
	if (!strcmp(c, "irc"))
		return 2;
	if (!strcmp(c, "mc"))
		return 3;
	if (!strcmp(c, "cs"))
		return 4;
	if (!strcmp(c, "gs"))
		return 5;
	if (!strcmp(c, "irs"))
		return 6;
	if (!strcmp(c, "led"))
		return 7;
	if (!strcmp(c, "ls"))
		return 8;
	if (!strcmp(c, "uss"))
		return 9;
	if (!strcmp(c, "bt"))
		return 10;
	if (!strcmp(c, "bm"))
		return 11;
	return -1;
}
bool manager(char* ex) {
	char* pex;
	char cx[9] = "";
	char c = '\0', cy = -1;
	uint8_t i = 0;
	pex = ex;
	bool res = 1;
	bool allok = 1;

	//printf("EXE: \"%s\"\n", ex);
	//c = ex[0];
	do {
		allok = 1;
		c = *ex;
		if (c != ' ') {
			cx[i] = c;
			i++;
		}
		if (c == ' ' || c == '\0') {
			//printf("command : \"%s\" number: %i\n", cx, wtn(cx));
			cx[i] = '\0';
			if (cx[0] != '\0') {
				//printf("command : \"%s\" number: %i\n", cx, wtn(cx));
				if ((cy = wtn(cx)) != -1) {
					color(GELBh);
					printf("\n\t****Start %s_Check****\n\n", cx);
					rcolor;
					cScategory(cx);
					char title[100] = PER_TITLE;
					strcat(title, PER_VERS);
					strcat(title, " - ");

					strcat(title, cx);
					title[strlen(title) - strlen(cx)] = title[strlen(title) - strlen(cx)] + ('A' - 'a');
					strcat(title, " check");
					system(title);
				}
				switch (cy) {
				case 0:allok = db_Tbtn(); break;
				case 1:allok = db_Trfid(); break;
				case 2:allok = db_ircT(); break;
				case 3:allok = db_mc(); break;
				case 4:allok = db_Tcs(); break;
				case 5:allok = printf("Gyro sensor is not supported yet!\n"); break;
				case 6:allok = db_Tirs(); break;
				case 7:allok = db_Tled(); break;
				case 8:allok = db_Tls(); break;
				case 9:allok = db_Tuss(); break;
				case 10:
					bt_os(1);
					printf("BLE can be tested in the DiscBot mobile app!\nDo you need or want the link? (y/n)\n");
					if ('y' == _getch()) {
						system("start https://www.bikebird.at/StepByStep/");
						printf("Type in your email and password to access Bikebird StepByStep\n\tThen klick on \"20_MikroController\"-\"85_DiscBot\"-\"App Downloaden\"!\n\tWarning: it is an Android App only!!!\n");
					}
					printf("If you finished disconnect your module from your phone and press any key!\n");
					_getch();
					bt_os(0);

					bt_config();
					printf("Do you want to change the configuration? (y/n)\n");
					while (_getch() == 'y') {
						bt_configchng();
						printf("View configuration? (y/n)\n");
						if (_getch() == 'y')
							bt_config();
						printf("Change config again? (y/n)\n");
					}

					db_cBT();

					cSaskIfFine();
					bt_os(1);
					break;
				case 11:allok = db_Tbm(); break;
				default:color(ROTh); printf("Command \"%s\" is not defined!\nPlease delete it from the configuration File!\n", cx); system(PER_CONFIG); color(rcol); break;
				}
				if (!allok) {

					db_reset();
					color(ROTh);
					printf("%s connection invalid!!!\nTest your connection for issues!!!\n", PER_DEVICE);
					color(rcol);
					cScError("Connection Error from DiscBot!!\n");
				}
				if (cy != -1) {

					cScategoryEnd();
					color(GELBd);
					printf("\n\n\t****End %s_Check****\n\n", cx);
					rcolor;
					color(GRUENd);
					printf("Push enter to go on!!\n");
					rcolor;
					flushW();
				}
			}
			db_clearBuffer();

			i = 0;
			cx[i] = '\0';
		}
		ex++;
		//printf("Push enter if the program is waiting here!\n");
	} while (c != '\0');

	printf("\n");


	return 1;
}
