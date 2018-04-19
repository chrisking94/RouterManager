#pragma once
class CHuffmanEncoding
{
#define SYSTEM 4//4x8 32λϵͳ��longΪ4λ
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
		BYTE *code;//���ȶ�̬���㣬���ɵı�����ڸ��ֽ�
		BYTE byteN;//�ֽڳ� len/8��
		BYTE bitN;//len%8λ
	}*HaffmanEndoingTable;//���
public:
	CHuffmanEncoding(void);
	CHuffmanEncoding(BYTE *pData,long len);
	CHuffmanEncoding(CString szFilePath);
	~CHuffmanEncoding(void);

	long Encoding(BYTE *&pRet);//���ر������ֽڳ���
	BYTE *Encoding();
	void Decoding(BYTE *&pRet);
	BYTE *Decoding();
	BOOL WriteToFile(CString szPath);//����ʹ�����·��������ļ������·��
private:
	void CalcWeight();
	BOOL CalcWeightStandard();//�Ӹ�ʽ�ֽڴ�����Ȩ��
	BOOL Select2MinWeightTree(int &s1,int &s2);//ѡ������Ȩ��С����
	void CreateTree();//����Huffman Tree
	int GetTreeDepth(int t);//DepthҲ��  �·���ı���+1��������λ��+1
	void CalcCodeTable();
	int CalcMaxWeightLenB();//�������Ȩռ���ֽ���
	int CalcHuffmanWeightByte(BYTE *pWB);//���ڱ���Huffman��������Ȩ���Թ�����ʱ�����������ֽ���
	void RShift(BYTE *pch,int m,int n);//��m��char����nλ
	void LShift(BYTE *pch,int m,int n);//��m��char����nλ
	void RShift_Spec(BYTE *pch,int m,int n);//��m byte����nλ��0���ֽڵĵ�nλ����-1���ֽڵĸ�nλ�����
private:
	BYTE *m_pData;//����
	long m_nLen;//���ݳ�
	HuffTree *m_pHT;//��������
	int m_nHTRoot;//����������
	DataWeight *m_pDW;//Ȩ��
	int m_nDwLen;//Ȩ��
	int m_nHCodeLen;//��������ռ�ֽ�����ȡ���������
	HaffmanEndoingTable m_HET;//�����������
	CString m_szPath;
};

