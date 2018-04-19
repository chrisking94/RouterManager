#pragma once
class CHuffmanEncoding
{
#define SYSTEM 4//4x8 32位系统，long为4位
private:
	typedef struct HTNode
	{
		long weight;
		int lchild,rchild,parent;
	}*HuffTree;

	typedef struct
	{
		long weight;
		BYTE data;
	}DataWeight;

	typedef struct HuffmanEncodingNode
	{
		BYTE *code;//长度动态计算，生成的编码存在高字节
		BYTE byteN;//字节长 len/8个
		BYTE bitN;//len%8位
	}*HaffmanEndoingTable;//码表
public:
	CHuffmanEncoding(void);
	CHuffmanEncoding(BYTE *pData,long len);
	CHuffmanEncoding(CString szFilePath);
	~CHuffmanEncoding(void);

	long Encoding(BYTE *&pRet);//返回编码后的字节长度
	BYTE *Encoding();
	void Decoding(BYTE *&pRet);
	BYTE *Decoding();
	BOOL WriteToFile(CString szPath);//可以使用相对路径，相对文件读入的路径
private:
	void CalcWeight();
	BOOL CalcWeightStandard();//从格式字节串创建权表
	BOOL Select2MinWeightTree(int &s1,int &s2);//选择两棵权最小的树
	void CreateTree();//创建Huffman Tree
	int GetTreeDepth(int t);//Depth也是  最长路径的边数+1，编码的最长位数+1
	void CalcCodeTable();
	int CalcMaxWeightLenB();//计算最大权占的字节数
	int CalcHuffmanWeightByte(BYTE *pWB);//用于保存Huffman树，保存权表，以供译码时创建，返回字节数
	void RShift(BYTE *pch,int m,int n);//将m个char左移n位
	void LShift(BYTE *pch,int m,int n);//将m个char右移n位
	void RShift_Spec(BYTE *pch,int m,int n);//将m byte右移n位，0号字节的低n位存入-1号字节的高n位，大端
private:
	BYTE *m_pData;//数据
	long m_nLen;//数据长
	HuffTree *m_pHT;//哈弗曼树
	int m_nHTRoot;//哈弗曼树根
	DataWeight *m_pDW;//权表
	int m_nDwLen;//权表长
	int m_nHCodeLen;//编码后的码占字节数，取决于最长编码
	HaffmanEndoingTable m_HET;//哈弗曼编码表
	CString m_szPath;
};

