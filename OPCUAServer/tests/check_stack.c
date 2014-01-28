/*
 ============================================================================
 Name        : opcuaServer.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "../src/opcua_transportLayer.h"

#include "../src/opcua_advancedDatatypes.h"
#include "check.h"

START_TEST(test_getPacketType_validParameter)
{

	char buf[] = {'C','L','O'};
	AD_RawMessage rawMessage;
	rawMessage.message = buf;
	rawMessage.length = 3;

	ck_assert_int_eq(TL_getPacketType(&rawMessage),packetType_CLO);

}
END_TEST


START_TEST(test_decodeRequestHeader_validParameter)
{
		char testMessage = {0x00,0x00,0x72,0xf1,0xdc,0xc9,0x87,0x0b,
							0xcf,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
							0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,
							0x00,0x00,0x00,0x00,0x00};
		AD_RawMessage rawMessage;
		rawMessage.message = testMessage;
		rawMessage.length = 29;
		Int32 position = 0;
		T_RequestHeader requestHeader;
		decodeRequestHeader(rawMessage,&position,requestHeader);

		ck_assert_int_eq(requestHeader.authenticationToken.EncodingByte,0);

		ck_assert_int_eq(requestHeader.returnDiagnostics,0);

		ck_assert_int_eq(requestHeader.authenticationToken.EncodingByte,0);
}
END_TEST

START_TEST(test_binaryEncDec_encoding)
{

	AD_RawMessage rawMessage;
	Int32 position = 0;
	//EncodeByte
		char testChar = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
		rawMessage.message = &testChar;
		Byte testByte = 0x08;
		rawMessage.length = 0;
		position = 0;
		encodeByte(testByte, &position, &rawMessage);

		ck_assert_int_eq(rawMessage.message[0], 0x08);
		ck_assert_int_eq(rawMessage.length, 1);
		ck_assert_int_eq(position, 1);
	//EncodeUInt16
		char testChar1 = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
		rawMessage.message = &testChar1;
		UInt16 testUInt16 = 0XABCD;
		rawMessage.length = 0;
		position = 0;
		encodeUInt16(testUInt16, &position, &rawMessage);

		ck_assert_int_eq(rawMessage.length, 2);
		ck_assert_int_eq(position, 2);
		ck_assert_int_eq((Byte) rawMessage.message[1], 0xAB);
		ck_assert_int_eq((Byte) rawMessage.message[0], 0xCD);
}
END_TEST


Suite* TL_testSuite_getPacketType(void)
{
	Suite *s = suite_create("getPacketType");
	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core,test_getPacketType_validParameter);
	suite_add_tcase(s,tc_core);
	return s;
}

Suite* TL_testSuite_encode(void)
{
	Suite *s = suite_create("encoding");
	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core,test_binaryEncDec_encoding);
	suite_add_tcase(s,tc_core);
	return s;
}


/*
Suite* TL_<TESTSUITENAME>(void)
{
	Suite *s = suite_create("<TESTSUITENAME>");
	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core,<TEST_NAME>);
	suite_add_tcase(s,tc_core);
	return s;
}
*/

int main (void)
{
	int number_failed;

	Suite *s = TL_testSuite_getPacketType();
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr,CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	s = TL_testSuite_encode();
	sr = srunner_create(s);
	srunner_run_all(sr,CK_NORMAL);
	number_failed += srunner_ntests_failed(sr);
	srunner_free(sr);


	/* <TESTSUITE_TEMPLATE>
	s =  <TESTSUITENAME>;
	sr = srunner_create(s);
	srunner_run_all(sr,CK_NORMAL);
	number_failed += srunner_ntests_failed(sr);
	srunner_free(sr);
	*/
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


