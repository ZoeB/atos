/* ASCII to Sines, by Zoe Blade */

/* Convert columns of a bitmap, 8 pixels high, 1 bit per pixel, into
a series of sine waves.  */

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "wavfile.c"

void usage();

FILE *wav;

uint8_t charset[768] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18,
  0x00, 0x00, 0x18, 0x00, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x66, 0x66, 0xff, 0x66, 0xff, 0x66, 0x66, 0x00, 0x18, 0x3e, 0x60, 0x3c,
  0x06, 0x7c, 0x18, 0x00, 0x62, 0x66, 0x0c, 0x18, 0x30, 0x66, 0x46, 0x00,
  0x3c, 0x66, 0x3c, 0x38, 0x67, 0x66, 0x3f, 0x00, 0x06, 0x0c, 0x18, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x00,
  0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x18, 0x30, 0x00, 0x00, 0x66, 0x3c, 0xff,
  0x3c, 0x66, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00, 0x7e,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00,
  0x00, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x00, 0x3c, 0x66, 0x6e, 0x76,
  0x66, 0x66, 0x3c, 0x00, 0x18, 0x18, 0x38, 0x18, 0x18, 0x18, 0x7e, 0x00,
  0x3c, 0x66, 0x06, 0x0c, 0x30, 0x60, 0x7e, 0x00, 0x3c, 0x66, 0x06, 0x1c,
  0x06, 0x66, 0x3c, 0x00, 0x06, 0x0e, 0x1e, 0x66, 0x7f, 0x06, 0x06, 0x00,
  0x7e, 0x60, 0x7c, 0x06, 0x06, 0x66, 0x3c, 0x00, 0x3c, 0x66, 0x60, 0x7c,
  0x66, 0x66, 0x3c, 0x00, 0x7e, 0x66, 0x0c, 0x18, 0x18, 0x18, 0x18, 0x00,
  0x3c, 0x66, 0x66, 0x3c, 0x66, 0x66, 0x3c, 0x00, 0x3c, 0x66, 0x66, 0x3e,
  0x06, 0x66, 0x3c, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00,
  0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30, 0x0e, 0x18, 0x30, 0x60,
  0x30, 0x18, 0x0e, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x00, 0x00,
  0x70, 0x18, 0x0c, 0x06, 0x0c, 0x18, 0x70, 0x00, 0x3c, 0x66, 0x06, 0x0c,
  0x18, 0x00, 0x18, 0x00, 0x3c, 0x66, 0x6e, 0x6e, 0x60, 0x62, 0x3c, 0x00,
  0x18, 0x3c, 0x66, 0x7e, 0x66, 0x66, 0x66, 0x00, 0x7c, 0x66, 0x66, 0x7c,
  0x66, 0x66, 0x7c, 0x00, 0x3c, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3c, 0x00,
  0x78, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x78, 0x00, 0x7e, 0x60, 0x60, 0x78,
  0x60, 0x60, 0x7e, 0x00, 0x7e, 0x60, 0x60, 0x78, 0x60, 0x60, 0x60, 0x00,
  0x3c, 0x66, 0x60, 0x6e, 0x66, 0x66, 0x3c, 0x00, 0x66, 0x66, 0x66, 0x7e,
  0x66, 0x66, 0x66, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00,
  0x1e, 0x0c, 0x0c, 0x0c, 0x0c, 0x6c, 0x38, 0x00, 0x66, 0x6c, 0x78, 0x70,
  0x78, 0x6c, 0x66, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7e, 0x00,
  0x63, 0x77, 0x7f, 0x6b, 0x63, 0x63, 0x63, 0x00, 0x66, 0x76, 0x7e, 0x7e,
  0x6e, 0x66, 0x66, 0x00, 0x3c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00,
  0x7c, 0x66, 0x66, 0x7c, 0x60, 0x60, 0x60, 0x00, 0x3c, 0x66, 0x66, 0x66,
  0x66, 0x3c, 0x0e, 0x00, 0x7c, 0x66, 0x66, 0x7c, 0x78, 0x6c, 0x66, 0x00,
  0x3c, 0x66, 0x60, 0x3c, 0x06, 0x66, 0x3c, 0x00, 0x7e, 0x18, 0x18, 0x18,
  0x18, 0x18, 0x18, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00,
  0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x00, 0x63, 0x63, 0x63, 0x6b,
  0x7f, 0x77, 0x63, 0x00, 0x66, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0x66, 0x00,
  0x66, 0x66, 0x66, 0x3c, 0x18, 0x18, 0x18, 0x00, 0x7e, 0x06, 0x0c, 0x18,
  0x30, 0x60, 0x7e, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c,
  0x0c, 0x0c, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x06, 0x3e, 0x66, 0x3e, 0x00,
  0x00, 0x60, 0x60, 0x7c, 0x66, 0x66, 0x7c, 0x00, 0x00, 0x00, 0x3c, 0x60,
  0x60, 0x60, 0x3c, 0x00, 0x00, 0x06, 0x06, 0x3e, 0x66, 0x66, 0x3e, 0x00,
  0x00, 0x00, 0x3c, 0x66, 0x7e, 0x60, 0x3c, 0x00, 0x00, 0x0e, 0x18, 0x3e,
  0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x3e, 0x66, 0x66, 0x3e, 0x06, 0x7c,
  0x00, 0x60, 0x60, 0x7c, 0x66, 0x66, 0x66, 0x00, 0x00, 0x18, 0x00, 0x38,
  0x18, 0x18, 0x3c, 0x00, 0x00, 0x06, 0x00, 0x06, 0x06, 0x06, 0x06, 0x3c,
  0x00, 0x60, 0x60, 0x6c, 0x78, 0x6c, 0x66, 0x00, 0x00, 0x38, 0x18, 0x18,
  0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x66, 0x7f, 0x7f, 0x6b, 0x63, 0x00,
  0x00, 0x00, 0x7c, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x3c, 0x66,
  0x66, 0x66, 0x3c, 0x00, 0x00, 0x00, 0x7c, 0x66, 0x66, 0x7c, 0x60, 0x60,
  0x00, 0x00, 0x3e, 0x66, 0x66, 0x3e, 0x06, 0x06, 0x00, 0x00, 0x7c, 0x66,
  0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x3e, 0x60, 0x3c, 0x06, 0x7c, 0x00,
  0x00, 0x18, 0x7e, 0x18, 0x18, 0x18, 0x0e, 0x00, 0x00, 0x00, 0x66, 0x66,
  0x66, 0x66, 0x3e, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x00,
  0x00, 0x00, 0x63, 0x6b, 0x7f, 0x3e, 0x36, 0x00, 0x00, 0x00, 0x66, 0x3c,
  0x18, 0x3c, 0x66, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x3e, 0x0c, 0x78,
  0x00, 0x00, 0x7e, 0x0c, 0x18, 0x30, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int c;
int i = 0;

uint8_t byte;
int charOffset;

float sample;
int freq;
int col;
int row;
int numberOfCopies = 4;
int copy;
float pixel;
float mix;
short out;
char var;

int main(int argc, char *argv[]) {
	FILE *filePointer;

	while ((var = getopt(argc, argv, "cdfhosw")) != -1) {
		switch (var) {
		case 'c':
			break;

		case 'd':
			break;

		case 'f':
			break;

		case 'h':
			break;

		case 'o':
			break;

		case 's':
			break;

		case 'w':
			break;

		default:
			usage();
		}
	}

	wav = wavfile_open("out.wav");

	if (!wav) {
		printf("Error: unable to write to out.wav.\n");
	}

	if (argc != 1) {
		printf("Error: please specify a text string via stdin.\n");
	}

	while ((c = getc(stdin)) != EOF) {
		/* Only display printable ASCII characters */
		if (c < 32 || c >= 160) {
			continue;
		}

		/* The character set file should only contain printable ASCII characters */
		charOffset = 8 * (c - 32);

		for (col = 0; col < 8; col++) {
			byte = 0;

			for (row = 0; row < 8; row++) {
				if (charset[charOffset + row] & (1 << (7 - col))) {
					byte |= 1 << row;
				}
			}

			/* Loop through each sample */
			for (sample = 0; sample < 11025; sample++) { /* Hardwire each pixel width as 1/4 of a CD quality second for now */
				mix = 0;

				/* Work out which oscillators are on for this column */
				for (row = 0; row < 8; row++) {
					if (byte & (1 << (7 - row))) { /* Lowest frequency oscillator first */
						freq = 16000 + (500 * row); /* Hardwire each pixel height as a single sine wave "beam" 500Hz apart from its neighbours, starting at 16kHz, for now */
						pixel = 0;

						for (copy = 0; copy < numberOfCopies; copy++) {
							/* TODO: Work out why the frequency needs to be multiplied by 4. */
							pixel += sin((freq + (500 / numberOfCopies * copy)) * 4 * (sample / 44100.0) * M_PI_2); /* The number of cycles per second is multiplied by the number of seconds.  Even though the latter's between 0 and 0.25, the frequencies bring it up.  Hardwire CD quality sample rate for now. */
						}

						pixel /= numberOfCopies;

						/* Fuzz off the edges of each pixel to avoid noise bursts */
						if (sample < 2757) {
							pixel = pixel / 2727 * sample;
						} else if (sample > 8268) {
							pixel = pixel / 2727 * (11025 - sample);
						}

						/* Fuzz off the top and bottom rows too */
						if (copy == 0 || copy == numberOfCopies - 1) {
							pixel /= 2;
						}

						mix += (1.0 / 9.0) * pixel; /* Each sine wave should be 1/9th volume, for mixing with headroom */
					}
				}

				out = floor(mix * 32767); /* Convert waveform into signed 16-bit integer */
				wavfile_write(wav, &out, 1);
			}
		}
	}

	wavfile_close(wav);
	return 0;
}

void usage() {
	fprintf(stderr, "ASCII to Sines, by Zoe Blade\n\n");
	fprintf(stderr, "usage: atos [-cdfhosw] message\n\n");
	fprintf(stderr, "OPTIONS:\n");
	fprintf(stderr, "    -c charset      charset to use, default c64\n");
	fprintf(stderr, "    -d n            duplicates of each line, including original, default 4\n");
	fprintf(stderr, "    -f n            fundamental harmonic in Hz, default 16000\n");
	fprintf(stderr, "    -h n            harmonic spacing in Hz (height of each pixel in Hz), default\n");
	fprintf(stderr, "                    500\n");
	fprintf(stderr, "    -o filename     file to write to, default out.wav\n");
	fprintf(stderr, "    -s n            samplerate in Hz, default 44100\n");
	fprintf(stderr, "    -w n            width of each pixel in samples, default 11025\n\n");
	exit(1);
}
