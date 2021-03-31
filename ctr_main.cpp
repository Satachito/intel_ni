#include	"aes.h"
typedef	unsigned char	UI1;

//	test vectors were taken from http://w3.antd.nist.gov/iip_pubs/rfc3602.txt

ALIGN16 UI1
AES128_TEST_KEY[]	= {
	0x7E,0x24,0x06,0x78,0x17,0xFA,0xE0,0xD7,
	0x43,0xD6,0xCE,0x1F,0x32,0x53,0x91,0x63
};
ALIGN16 UI1
AES192_TEST_KEY[]	= {
	0x7C,0x5C,0xB2,0x40,0x1B,0x3D,0xC3,0x3C,
	0x19,0xE7,0x34,0x08,0x19,0xE0,0xF6,0x9C,
	0x67,0x8C,0x3D,0xB8,0xE6,0xF6,0xA9,0x1A
};
ALIGN16 UI1
AES256_TEST_KEY[]	= {
	0xF6,0xD6,0x6D,0x6B,0xD5,0x2D,0x59,0xBB,
	0x07,0x96,0x36,0x58,0x79,0xEF,0xF8,0x86,
	0xC6,0x6D,0xD5,0x1A,0x5B,0x6A,0x99,0x74,
	0x4B,0x50,0x59,0x0C,0x87,0xA2,0x38,0x84
};

ALIGN16 UI1
AES_TEST_VECTOR[]	= {
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
	0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
	0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F
};

ALIGN16 UI1
CTR128_IV[]			= { 0xC0,0x54,0x3B,0x59,0xDA,0x48,0xD9,0x0B };

ALIGN16 UI1
CTR192_IV[]			= { 0x02,0x0C,0x6E,0xAD,0xC2,0xCB,0x50,0x0D };

ALIGN16 UI1
CTR256_IV[]			= { 0xC1,0x58,0x5E,0xF1,0x5A,0x43,0xD8,0x75 };

ALIGN16 UI1
CTR128_NONCE[]		= { 0x00,0x6C,0xB6,0xDB };

ALIGN16 UI1
CTR192_NONCE[]		= { 0x00,0x96,0xB0,0x3B };

ALIGN16 UI1
CTR256_NONCE[]		= { 0x00,0xFA,0xAC,0x24 };

ALIGN16 UI1
CTR128_EXPECTED[]	= {
	0x51,0x04,0xA1,0x06,0x16,0x8A,0x72,0xD9,
	0x79,0x0D,0x41,0xEE,0x8E,0xDA,0xD3,0x88,
	0xEB,0x2E,0x1E,0xFC,0x46,0xDA,0x57,0xC8,
	0xFC,0xE6,0x30,0xDF,0x91,0x41,0xBE,0x28
};
ALIGN16 UI1
CTR192_EXPECTED[]	= {
	0x45,0x32,0x43,0xFC,0x60,0x9B,0x23,0x32,
	0x7E,0xDF,0xAA,0xFA,0x71,0x31,0xCD,0x9F,
	0x84,0x90,0x70,0x1C,0x5A,0xD4,0xA7,0x9C,
	0xFC,0x1F,0xE0,0xFF,0x42,0xF4,0xFB,0x00
};
ALIGN16 UI1
CTR256_EXPECTED[]	= {
	0xF0,0x5E,0x23,0x1B,0x38,0x94,0x61,0x2C,
	0x49,0xEE,0x00,0x0B,0x80,0x4E,0xB2,0xA9,
	0xB8,0x30,0x6B,0x50,0x8F,0x83,0x9D,0x6A,
	0x55,0x30,0x83,0x1D,0x93,0x44,0xAF,0x1C
};
/*****************************************************************************/
#define	LENGTH	64
#include	<iostream>
#include	<string>
#include	<vector>
using namespace std;
#include	"../Hex.h"

void
Test(
	UI1*	PLAINTEXT
,	UI1*	CIPHER_KEY
,	int		key_length
,	UI1*	EXPECTED_CIPHERTEXT
,	UI1*	IV
,	UI1*	NONCE
) {
	UI1		CIPHERTEXT[ LENGTH ];
	AES_KEY	key;
	AES_set_encrypt_key( CIPHER_KEY, key_length, &key );
	AES_CTR_encrypt(
		PLAINTEXT
	,	CIPHERTEXT
	,	IV
	,	NONCE
	,	LENGTH
	,	key.KEY
	,	key.nr
	);

	UI1		DECRYPTEDTEXT[ LENGTH ];
//	CTR uses key as decrypt_key
//	AES_KEY	decrypt_key;
//	AES_set_decrypt_key( CIPHER_KEY, key_length, &decrypt_key );
	AES_CTR_encrypt(
		CIPHERTEXT
	,	DECRYPTEDTEXT
	,	IV
	,	NONCE
	,	LENGTH
	,	key.KEY
	,	key.nr
	);
	cout << "The Cipher Key:\n";
	cout << '\t' << EncodeHex( CIPHER_KEY, key_length / 8 ) << endl;
	cout << "The Key Schedule:\n";
	for ( auto i = 0; i < key.nr; i++ ) cout << '\t' << EncodeHex( &key.KEY[ i * 16 ], 16 ) << endl;
	cout << "The CIPHERTEXT:\n";
	for ( auto i = 0; i < LENGTH / 16; i++ ) cout << '\t' << EncodeHex( &CIPHERTEXT[ i * 16 ], 16 ) << endl;
	if ( LENGTH%16 ) cout << '\t' << EncodeHex( &CIPHERTEXT[ LENGTH / 16 * 16 ], LENGTH % 16 ) << endl;

	for( auto i=0; i< ((32<LENGTH)? 32 : LENGTH); i++) if (CIPHERTEXT[i] != EXPECTED_CIPHERTEXT[i%(16*2)]) throw "The ciphertext is not equal to the expected ciphertext.\n\n";
	for( auto i=0; i<LENGTH; i++) if (DECRYPTEDTEXT[i] != PLAINTEXT[i]) throw "The DECRYPTED TEXT is not equal to the original PLAINTEXT.\n\n";
}

int
main() {

	if ( !Check_CPU_support_AES() ) throw "Cpu does not support AES instruction set. Bailing out.";

	UI1		PLAINTEXT[ LENGTH ];
	auto	i = 0;
	for ( i = 0; i < LENGTH / 16 / 2; i++ ) {
		for ( auto j = 0; j < 2; j++ ) {
			_mm_storeu_si128(
				&((__m128i*)PLAINTEXT)[i*2+j]
			,	((__m128i*)AES_TEST_VECTOR)[j]
			);
		}
	}
	auto	j = i * 2;
	for ( ; j < LENGTH / 16; j++ ) {
		_mm_storeu_si128(
			&((__m128i*)PLAINTEXT)[j]
		,	((__m128i*)AES_TEST_VECTOR)[j%4]
		);
	}
	if ( LENGTH % 16 ) {
		_mm_storeu_si128(
			&((__m128i*)PLAINTEXT)[j]
		,	((__m128i*)AES_TEST_VECTOR)[j%4]
		);
	}

	cout << "The PLAINTEXT:\n";
	for ( auto i = 0; i< LENGTH / 16; i++ ) cout << '\t' << EncodeHex( &PLAINTEXT[ i * 16 ], 16 ) << endl;
	cout << endl;

	try {
		cerr << "Performing AES128 CTR.\n";
		Test(
			PLAINTEXT
		,	AES128_TEST_KEY
		,	128
		,	CTR128_EXPECTED
		,	CTR128_IV
		,	CTR128_NONCE
		);
		cerr << endl;
		cerr << "Performing AES192 CTR.\n";
		Test(
			PLAINTEXT
		,	AES192_TEST_KEY
		,	192
		,	CTR192_EXPECTED
		,	CTR192_IV
		,	CTR192_NONCE
		);
		cerr << endl;
		cerr << "Performing AES256 CTR.\n";
		Test(
			PLAINTEXT
		,	AES256_TEST_KEY
		,	256
		,	CTR256_EXPECTED
		,	CTR256_IV
		,	CTR256_NONCE
		);
		cerr << endl;
	} catch ( const char* _ ) { cerr << _ << endl; }
}


