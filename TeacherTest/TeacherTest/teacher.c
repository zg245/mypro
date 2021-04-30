#include "teacher.h"
#include "itcast_asn1_der.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	typedef struct _Teacher
	{
		char name[64];
		int age;
		char *p;
		long plen;
	}Teacher;
*/
int encodeTeacher(Teacher * p, char ** outData, int * outlen)
{
	ITCAST_ANYBUF *head = NULL;
	ITCAST_ANYBUF *temp = NULL;
	ITCAST_ANYBUF *next = NULL;

	//����name
	//ITCAST_INT DER_ItAsn1_WritePrintableString(ITASN1_PRINTABLESTRING *pPrintString, ITASN1_PRINTABLESTRING **ppDerPrintString);
	//char *---->ITCAST_ANYBUF
	DER_ITCAST_String_To_AnyBuf(&temp, p->name, strlen(p->name)+1);
	DER_ItAsn1_WritePrintableString(temp, &head);
	DER_ITCAST_FreeQueue(temp);
	next = head;

	//����age
	DER_ItAsn1_WriteInteger(p->age, &next->next);
	next = next->next;

	//����p
	//int EncodeChar(char *pData, int dataLen, ITCAST_ANYBUF **outBuf);
	EncodeChar(p->p, strlen(p->p)+1, &next->next);
	next = next->next;

	//����plen
	DER_ItAsn1_WriteInteger(p->plen, &next->next);

	//���л�
	DER_ItAsn1_WriteSequence(head, &temp);

	//���������ֵ
	*outData = temp->pData;
	*outlen = temp->dataLen;

	//�ͷ��ڴ�
	DER_ITCAST_FreeQueue(head);

	return 0;
}

/*
	typedef struct _Teacher
	{
		char name[64];
		int age;
		char *p;
		long plen;
	}Teacher;
*/

int decodeTeacher(char * inData, int inLen, Teacher ** p)
{
	ITCAST_ANYBUF *head = NULL;
	ITCAST_ANYBUF *temp = NULL;
	ITCAST_ANYBUF *next = NULL;

	Teacher *pt = (Teacher *)malloc(sizeof(Teacher));
	if (pt == NULL)
	{
		return -1;
	}

	//��inData�����л�������
	//��char *--->ITCAST_ANYBUF����
	DER_ITCAST_String_To_AnyBuf(&temp, inData, inLen);
	DER_ItAsn1_ReadSequence(temp, &head);
	DER_ITCAST_FreeQueue(temp);
	next = head;

	//����name
	DER_ItAsn1_ReadPrintableString(next, &temp);
	memcpy(pt->name, temp->pData, temp->dataLen);
	next = next->next;
	DER_ITCAST_FreeQueue(temp);

	//����age
	DER_ItAsn1_ReadInteger(next, &pt->age);
	next = next->next;

	//����p
	int len = 0;
	DecodeChar(next, &pt->p, &len);
	next = next->next;

	//����plen
	DER_ItAsn1_ReadInteger(next, &pt->plen);

	//�����������ֵ
	*p = pt;

	//�ͷ��ڴ�
	DER_ITCAST_FreeQueue(head);

	return 0;
}

void freeTeacher(Teacher ** p)
{
	if ((*p) != NULL)
	{
		if ((*p)->p != NULL)
		{
			free((*p)->p);
		}
		free(*p);
	}
}
