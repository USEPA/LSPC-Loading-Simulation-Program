/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#include <AFXWIN.H>

#if !defined(ALX_DBFDEF_H__INCLUDED_)
#define ALX_DBFDEF_H__INCLUDED_

#include "DBFExceptionRes.h"

#ifdef ALX_DLL
	#define ALX_EXT_CLASS	AFX_EXT_CLASS
#else
	#define ALX_EXT_CLASS
#endif

#define MIN_DATE		(-657434L)  // about year 100
#define MAX_DATE		2958465L    // about year 9999
#define ALX_DBF_CENTURY     1990

// Memo File types
#define	MEMO_TYPE_NOMEMO		0x00	// not memo
#define	MEMO_TYPE_DBASE			0x01	// .DBT - dBASE (lower dBASEIV)
#define	MEMO_TYPE_DBASEIV		0x03	// .DBT - dBASE (dBASEIV or higher)
#define	MEMO_TYPE_FOXPRO		0x04	// .FPT - FoxPro
#define	MEMO_TYPE_VFOXPRO		0x08	// .FPT - Visual FoxPro
#define	MEMO_TYPE_FOX_DATABASE	0x0C	// .DCT - Visual FoxPro Database
#define	MEMO_TYPE_SIX			0x10	// .SMT - HiPer-SIx

// Memo flag
#define	MF_BINARYREC		0x00000000	// binary memo record in FoxPro
#define	MF_TEXTREC			0x00000001	// text memo record in FoxPro
#define	MF_USEDREC			0x0008FFFF	// used memo record in dBASEIV

// Field types
#define	FLD_TYPE_CHARACTER	'C'		// Character
#define	FLD_TYPE_CURRENCY	'Y'		// Currency
#define	FLD_TYPE_NUMERIC	'N'		// Float or integer
#define	FLD_TYPE_FLOAT		'F'		// Float
#define	FLD_TYPE_DATE		'D'		// Date
#define	FLD_TYPE_DATETIME	'T'		// Date and time
#define	FLD_TYPE_DOUBLE		'B'		// Double in FoxPro
#define	FLD_TYPE_BINARY		'B'		// Binary in dBASE
#define	FLD_TYPE_INTEGER	'I'		// Integer
#define	FLD_TYPE_LOGICAL	'L'		// Logical
#define	FLD_TYPE_MEMO		'M'		// Memo
#define	FLD_TYPE_GENERAL	'G'		// General
#define	FLD_TYPE_PICTURE	'P'		// Picture
#define	FLD_TYPE_NULLFLAGS	'0'		// _NullFlags

// Lock Style
#define	LS_NOLOCK	0x00
#define	LS_DBASE	0x01
#define	LS_CLIPPER	0x02
#define	LS_FOX		0x04

// Lock Type
#define	LT_PESSIMISTIC	0x01
#define	LT_OPTIMISTIC	0x02

// Record flags
#define	REC_FLAG_DELETED	'*'		// record deleted
#define	REC_FLAG_NORMAL		' '		// record normal

#define	REC_EOF				0x1A	// eof of head record or memo record

// Table flags
#define	TBL_FLAG_STRUCTURAL	0x01	// file has structural .cdx or .mdx
#define	TBL_FLAG_MEMO		0x02	// file has a Memo field
#define	TBL_FLAG_DATABASE	0x04	// file is a database .dbc

// Field flags
#define	FLD_FLAG_SYSTEM		0x01	// System Column (not visible to user)
#define	FLD_FLAG_CANBENULL	0x02	// 0x02 Column can store null values
#define	FLD_FLAG_BINARY		0x04	// 0x04 Binary column (for CHAR and MEMO only)

// Open flags
#define ALX_DBF_USE_SHARED        (CFile::shareDenyNone | CFile::modeReadWrite)
#define ALX_DBF_USE_EXCLUSIVE     (CFile::shareExclusive | CFile::modeReadWrite)
#define ALX_DBF_USE_READ_ONLY     (CFile::shareDenyNone | CFile::modeRead)

// DBF file types
#define DBF_BOOL	unsigned char	// 1 byte
#define DBF_CHAR	char			// 1 byte
#define DBF_UCHAR	unsigned char	// 1 byte
#define DBF_INT		short			// 2 bytes
#define DBF_UINT	unsigned short	// 2 bytes
#define DBF_LONG	long			// 4 bytes
#define DBF_ULONG	unsigned long	// 4 bytes
#define DBF_DOUBLE	double			// 8 bytes

#define QWORD		__int64			// 4 WORD - 8 bytes

// Swap byte definition
#define _SWAP_WORD_BYTE(w)	 (/**/ (((w)<<8) & 0xFF00)|(((w)>>8) & 0x00FF) /**/)
#define SWAP_WORD_BYTE(l)	 (_SWAP_WORD_BYTE((WORD)(l)))
#define _SWAP_DWORD_BYTE(dw) (/**/ ((dw)<<24)|(((dw)<<8) & 0x00FF0000)|(((dw)>>8) & 0x0000FF00)|((dw)>>24) /**/)
#define SWAP_DWORD_BYTE(l)	 (_SWAP_DWORD_BYTE((DWORD)(l)))
#define _SWAP_QWORD_BYTE(qw) (/**/ ((QWORD)SWAP_DWORD_BYTE(qw>>32)) | ((QWORD)SWAP_DWORD_BYTE(qw))<<32 /**/)
#define SWAP_QWORD_BYTE(l)	 (_SWAP_QWORD_BYTE((QWORD)(l)))

#define HIDWORD(l)   ((DWORD) (((QWORD) (l) >> 32) & 0xFFFFFFFF)) 
#define LODWORD(l)   ((DWORD) (l)) 

//static VARIANT NullVariant = { VT_NULL, 0, 0, 0, 0 };

#pragma pack(1)

typedef struct
{
	DBF_UCHAR dbf_id;		// 0x02 FoxBASE
							// 0x03 FoxBASE+/dBASE III PLUS, no memo
							// 0x30 Visual FoxPro
							// 0x43 dBASE IV SQL table files, no memo
							// 0x63 dBASE IV SQL system files, no memo
							// 0x83 FoxBASE+/dBASE III PLUS, with memo
							// 0x8B dBASE IV with memo
							// 0xCB dBASE IV SQL table files, with memo
							// 0xE5 HiPer-SIx with memo (.SMT)
							// 0xF5 FoxPro 2.x (or earler) with memo
							// 0xFB FoxBASE
	DBF_CHAR last_update[3];// Last update (YYMMDD)
							// Fox: curent century + YY
							// dBase: 1900 + YY
							// SIx: 0x700(1792) + YY
	DBF_LONG last_rec;		// Number of record in file
	DBF_UINT data_offset;	// Position of first data record
	DBF_UINT rec_size;		// Lenght of one data record (including delete flag)
	DBF_CHAR filler[16];	// Reserved
	DBF_CHAR table_flag;	// Table Flags (Only Visual Foxpro)
							// 0x01 file has structural .cdx or .mdx
							// 0x02 file has a Memo field
							// 0x04 file is a database .dbc
							// Note that this byte can contain the sum of
							// any of the above values.
	DBF_UCHAR code_page;	// Code page mark
							// 0x01 437    U.S. MS-DOS
							// 0x69	620    Mazovia (Polish) MS-DOS
							// 0x6A 737    Greek MS-DOS (437G)
							// 0x02 850    International MS-DOS
							// 0x64 852    Eastern European MS-DOS
 							// 0x67 861    Icelandic MS-DOS
 							// 0x66 865    Nordic MS-DOS
 							// 0x65 866    Russian MS-DOS
 							// 0x68 895    Kamenicky (Czech) MS-DOS
 							// 0x6B 857    Turkish MS-DOS
 							// 0xC8 1250   Eastern European Windows
 							// 0xC9 1251   Russian Windows
 							// 0x03 1252   Windows ANSI
 							// 0xCB 1253   Greek Windows
 							// 0xCA 1254   Turkish Windows
 							// 0x04	10000  Standard Macintosh
 							// 0x98 10006  Greek Macintosh
 							// 0x96 10007  Russian Macintosh
 							// 0x97	10029  Eastern European Macintosh
	DBF_UINT reserved;		// Recerved, contains 0x00
} DBF_HEAD;

typedef struct
{
	DBF_ULONG	next_block;	// Next free block
	DBF_UINT	reserved;	// Reserved
	DBF_UINT	block_size;	// Block size
} FPT_HEAD;

typedef struct
{
	DBF_ULONG	next_block;			// Next free block
	DBF_LONG	reserved1;			// Reserved
	DBF_CHAR	dbf_file_name[9];	// Name parent DBF table
	DBF_CHAR	reserved2[3];		// Reserved
	DBF_UINT	block_size;			// Block size
} DBT_HEAD;

typedef struct
{
	DBF_ULONG	next_block;			// Next free block
	DBF_LONG	block_size;			// Block size
	DBF_CHAR	description[8];		// Description
	DBF_UINT	reserved;			// Reserved
	DBF_UINT	free_rec_count;		// Free rec count
	DBF_CHAR	free_rec_ptr[490];	// Pointers on free record
} SMT_HEAD;

typedef struct
{
	DBF_CHAR field_name[11];// Field name
	DBF_CHAR field_type;	// Field type
	DBF_LONG field_offset;  // Pointer on field offset in the record
	union LEN_INFO
	{
		DBF_UINT char_len;
		struct NUM_SIZE
		{
			DBF_CHAR len;	// Field len
			DBF_CHAR dec;	// Field decimal
		} num_size;
	} len_info;
	DBF_CHAR field_flag;	// Field flag
							// 0x01 System Column (not visible to user)
							// 0x02 Column can store null values
							// 0x04 Binary column (for CHAR and MEMO only)
	DBF_CHAR filler[13];	// Non used
} FIELD_REC;

#pragma pack()
/*
void IEEE2DBF(double* db);
double DBF2IEEE(DBF_CHAR* key);
*/

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_DBFDEF_H__INCLUDED_)

