#include "StdAfx.h"
#include "HuffmanEncoding.h"


CHuffmanEncoding::CHuffmanEncoding(void)
{
	m_nLen=0;
	m_nHTRoot=-1;
	m_nHCodeLen=0;
	m_nDwLen=0;
}

CHuffmanEncoding::CHuffmanEncoding( BYTE *pData,long len )
{
	m_pData = pData;
	m_nLen = len;
}

CHuffmanEncoding::CHuffmanEncoding( CString szFilePath )
{
	m_szPath = szFilePath;

	CFile rfile(szFilePath,CFile::modeRead|CFile::shareExclusive);
	m_nLen = rfile.GetLength();
	m_pData=new BYTE[m_nLen];
	rfile.Read(m_pData,m_nLen);
	rfile.Close();
}


CHuffmanEncoding::~CHuffmanEncoding(void)
{
}

void CHuffmanEncoding::CalcWeight()
{
	long lst[256]={0};//���ڴ洢�ַ���Ȩ
	long len=m_nLen;
	m_pData[m_nLen-1]=0;
	
	BYTE *pch=m_pData;
	while(len--)
	{
		lst[*pch++]++;
	}

	m_nDwLen = 0;
	for(int i=0;i<256;i++)
	{
		if(lst[i]) m_nDwLen++;
	}

	m_pDW=new DataWeight[m_nDwLen];
	int j=m_nDwLen;
	for(int i=0;i<256;i++)
	{
		if(lst[i])
		{
			m_pDW[--j].data=i;
			m_pDW[j].weight=lst[i];
		}
	}
}

BOOL CHuffmanEncoding::Select2MinWeightTree( int &s1,int &s2 )
{
	long w=0,w1=0;
	int fitValCount=0;

	int c=m_nDwLen*2-1;
	while(c)
	{
		if(m_pHT[--c]&&m_pHT[c]->parent==-1)
		{
			if(w==0)//��һ�θ�ֵ
			{
				w=m_pHT[c]->weight;
				s1=c;
			}
			else if(w1==0)
			{
				w1=m_pHT[c]->weight;
				s2=c;
			}

			if(m_pHT[c]->weight<w)//s1��һ��ʼ������С�ģ��Ͳ�������������s2�������ҵ�
			{
				s2=s1;
				s1=c;
				w=m_pHT[c]->weight;
			}
			else if(m_pHT[c]->weight<w1)//�������ﱣ��
			{
				s2=c;
				w1=m_pHT[c]->weight;
			}
			fitValCount++;
		}
	}
	TRACE("%d %d\r\n",s1,s2);
	return fitValCount>1;
}

void CHuffmanEncoding::CreateTree()
{
	//n��Ҷ�ӵ�huffman tree��2n-1����㣬����HuffTreeָ���б�
	int nodes_c=m_nDwLen*2-1;
	m_pHT=new HuffTree[nodes_c];
	memset(m_pHT,NULL,nodes_c*sizeof(HuffTree));//ȫ����ʼ��Ϊ��ָ��

	//����HuffTree List,��Ȩ���ƽ�HTNode
	for(int i=0;i<m_nDwLen;i++)
	{
		m_pHT[i]=new HTNode;
		m_pHT[i]->weight=m_pDW[i].weight;
		m_pHT[i]->lchild=m_pHT[i]->rchild=m_pHT[i]->parent=-1;
	}

	//����huffman��
	int l,r;
	for(int i=m_nDwLen;i<nodes_c;i++)
	{
		if(!Select2MinWeightTree(l,r))
		{
			AfxMessageBox(_T("Select2MinWeightHT Error!"));
			//return;
		}
		m_pHT[i]=new HTNode;
		m_pHT[i]->parent=-1;//���ڵ�ָ��
		m_pHT[i]->lchild=l;//������
		m_pHT[i]->rchild=r;
		m_pHT[l]->parent=i;//���Ӹ�
		m_pHT[r]->parent=i;
		m_pHT[i]->weight=m_pHT[l]->weight+m_pHT[r]->weight;//��Ȩ
	}
	
	m_nHTRoot = nodes_c-1;//���ڵ�����
	//�������
}

void CHuffmanEncoding::CalcCodeTable()
{
	//�������
	m_HET = new HuffmanEncodingNode[m_nDwLen];
	//��ȡ�������Ҫ�ֽ���
	m_nHCodeLen =(GetTreeDepth(m_nHTRoot)+7-1)/8;//-1��˼�� ���볤=����-1
	//TRACE("depth=%d\r\n",GetTreeDepth(m_nHTRoot));
	//�����������
	int parent;
	int bitI=0,chI=0,k,bitN;
	//��ʼ����
	for(long i=0;i<m_nDwLen;i++)
	{
		k=i;
		parent=m_pHT[i]->parent;
		bitI=0;//��λ����
		chI=0;
		m_HET[i].code=new BYTE[m_nHCodeLen];
		memset(m_HET[i].code,0,m_nHCodeLen);
		bitN=0;
		while(parent!=-1)
		{
			if(m_pHT[parent]->lchild==k)//��Ҷ��Ϊ���ӣ�����0
			{
				bitI++;
				//cout<<"0";
			}
			else//�Һ��ӣ�����1
			{
				m_HET[i].code[m_nHCodeLen-chI-1] |= 0x80>>bitI++;
				//cout<<"1";
			}
			if(bitI==8)
			{
				chI++;//����1�ֽ�
				bitI=0;
			}
			bitN++;
			k=parent;//ָ������
			parent=m_pHT[k]->parent;
		}
		//���һ���ַ�������ͷ�Ƶ���λ
		//TRACE("%d %d \r\n",*m_HET[i].code,*(m_HET[i].code+1));
		m_HET[i].bitN = bitN%8;
		m_HET[i].byteN = (bitN+7)/8;
		TRACE("m_HET[code 0 1]=%d %d ��Shift bit=%d\r\n",*m_HET[i].code,*(m_HET[i].code+1),8*(m_nHCodeLen-1)-m_HET[i].bitN);
		RShift(m_HET[i].code,m_nHCodeLen,8*(m_nHCodeLen-1)-m_HET[i].bitN);
		//TRACE("After shift,m_HET[code 0 1]=%d %d \r\n",*m_HET[i].code,*(m_HET[i].code+1));
		int c=0;
		//cout<<"  Char="<<dw[i].data<<"  byteN="<<(int)het[i].byteN<<"  bitN="<<(int)het[i].bitN<<endl;
	}
}

int CHuffmanEncoding::GetTreeDepth( int t )
{
	if(t==-1)
	{
		return 0;
	}
	else
	{
		int l=0,r=0;
		l=GetTreeDepth(m_pHT[t]->lchild);
		r=GetTreeDepth(m_pHT[t]->rchild);
		//TRACE("%d depth\r\n",(l>r?l:r)+1);
		return (l>r?l:r)+1;
	}
}

long CHuffmanEncoding::Encoding( BYTE *&pRet )
{
	//ǰ�����
	CalcWeight();
	CreateTree();
	CalcCodeTable();
	//�������ڲ��ҵĿ��
	BYTE table[256];
	for(long i=0;i<m_nDwLen;i++) table[m_pDW[i].data]=i;

	//�������뻺����
	long nl=CalcMaxWeightLenB();
	nl=m_nLen+2+m_nDwLen*(1+nl);
	pRet=new BYTE[m_nLen];
	memset(pRet,0,m_nLen);
	//д��ͷ
	BYTE *p=pRet+CalcHuffmanWeightByte(pRet);//ƫ���ֽ�
	int k;
	long bitI=0,chI=0,lastChI;//chIָ������һ��Ϊ�յ��ֽ�
	//��ʼ����
	for(long i=0;i<m_nLen;i++)
	{
		k=table[m_pData[i]];//�������������е�λ��
		lastChI=chI;//����ָ��Ŀ�λ�������µ��ֽں󣬴����￪ʼ��λ
		for(int j=0;j<m_HET[k].byteN;j++)
		{
			p[chI++]=m_HET[k].code[j];//���Ʊ����ֽڵ�������
			//TRACE("code[%d]=%d ",j,m_HET[k].code[j]);
		}
		//TRACE("\r\n");
		RShift_Spec(p+lastChI,m_HET[k].byteN,bitI);
		
		if(bitI>m_HET[k].bitN)//�ϴε��ֽڻ�ʣ��bitI���ռ䣬������ռ��������µ�λ����chI������һ��
		{
			chI--;
			bitI=bitI-m_HET[k].bitN;
		}
		else
		{
			bitI=(8-(m_HET[k].bitN-bitI))%8;
		}
		//TRACE("ch%d=%d \r\n",lastChI,p[lastChI]);
	}
	//for(long i=0;i<m_nLen;i++) TRACE("%d ",p[i]);
	return chI;
}

BYTE * CHuffmanEncoding::Encoding()
{
	return NULL;
}

int CHuffmanEncoding::CalcMaxWeightLenB()
{
	long m=0;
	for(int i=0;i<m_nDwLen;i++)//�������ֵ
	{
		if(m_pDW[i].weight>m) m=m_pDW[i].weight;
	}
	
	for(int i=0;i<4;i++)
	{
		if(m>0xffffffff>>i*8)//�Ӹ�λ���£����ҵ�һ����Ϊ0���ֽ�
		{
			//TRACE(" %d \r\n",4-i);
			return 4-i;
		}
	}
	return 1;
}

int CHuffmanEncoding::CalcHuffmanWeightByte( BYTE *pWB )
{
	//��һ���ֽڴ� Ȩ����
	//�ڶ����ֽڴ� Ȩ���ȣ�ռ���ֽ�����
	//�����ֽ� һ�ֽ����ݣ�Ȩ��һ�ֽ����ݣ�Ȩ��...
	//����Ҫ 1+1+Ȩ��*(1+Ȩ��) ���ֽ�

	//����洢�ռ�
	int wl=CalcMaxWeightLenB();
	int bufl=2+m_nDwLen*(1+wl);
	//pWB = new BYTE[bufl];

	pWB[0] = m_nDwLen;//Ȩ��
	pWB[1] = wl;//Ȩ��

	char *pW;
	for(int i=2,k=0;i<bufl;i+=1+wl,k++)//�ӵ�3���ֽڿ�ʼ
	{
		pW=(char*)&m_pDW[k].weight;
		pWB[i] = m_pDW[k].data;
		pWB[i+1]= pW[0];
		if(wl>1)//��С����Ϊ1�ֽڣ����Բ����ж�0�ֽڵ����
		{
			pWB[i+2]= pW[1];
			if(wl>2)
			{
				pWB[i+3]= pW[2];
				if(wl>3)
				{
					pWB[i+4]= pW[3];
				}
			}
		}
	}
	//for(int i=0;i<bufl;i++) TRACE("%c",pWB[i]);
	return bufl;
}

BOOL CHuffmanEncoding::CalcWeightStandard()
{
	//��һ���ֽڴ� Ȩ����
	//�ڶ����ֽڴ� Ȩ���ȣ�ռ���ֽ�����
	//�����ֽ� һ�ֽ����ݣ�Ȩ��һ�ֽ����ݣ�Ȩ��...
	//����Ҫ 1+1+Ȩ��*(1+Ȩ��) ���ֽ�
	if(m_nLen<2) return FALSE;
	m_nDwLen = m_pData[0];
	int wl = m_pData[1];//Ȩ��
	int bufl=2+m_nDwLen*(1+wl);
	if(m_nLen<bufl+m_nDwLen)
	{
		return FALSE;
	}
	else
	{
		//����Ȩ��
		m_pDW = new DataWeight[m_nDwLen];
		char *pW;
		for(int i=2,k=0;i<bufl;i+=1+wl,k++)//�ӵ�3���ֽڿ�ʼ
		{
			pW=(char*)&m_pDW[i].weight;
			m_pDW[i].data = m_pData[i];
			pW[0]=m_pData[i+1];
			if(wl>1)//��С����Ϊ1�ֽڣ����Բ����ж�0�ֽڵ����
			{
				pW[1]=m_pData[i+2];
				if(wl>2)
				{
					pW[2]=m_pData[i+3];
					if(wl>3)
					{
						pW[3]=m_pData[i+4];
					}
				}
			}
		}
		return TRUE;
	}
}

void CHuffmanEncoding::Decoding( BYTE *&pRet )
{

}

BYTE * CHuffmanEncoding::Decoding()
{
	return NULL;
}

BOOL CHuffmanEncoding::WriteToFile( CString szPath )
{
	CFile rfile(szPath,CFile::modeWrite|CFile::shareExclusive|CFile::modeCreate);
	BYTE *write;
	long nCount=Encoding(write);
	rfile.Write(write,nCount);
	rfile.Close();

	return TRUE;
}

void CHuffmanEncoding::RShift( BYTE *pch,int m,int n )
{
	int bn=n/8;//���ֽ�����
	for(int i=0;i<m;i++)
	{
		if(i+bn<m)
		{
			pch[i]=pch[i+bn];
		}
		else
		{
			pch[i]=0;//���ֽ���������
		}
	}
	n%=8;
	for(int i=0;i<m-1;i++)
	{
		*(pch)>>=n;//ǰһ���ַ�����nλ
		*(pch)|=((*(pch+1))<<(8-n));//���ַ���nλ������ַ���nλ
		*(pch)>>=n;//��һ���ַ�����nλ
	}
}

void CHuffmanEncoding::LShift( BYTE *pch,int m,int n )
{
	int bn=n/8;//���ֽ�����
	for(int i=m-1;i>0;i++)
	{
		if(i+bn>m)
		{
			pch[i]=pch[i-bn];
		}
		else
		{
			pch[i]=0;//���ֽ���������
		}
	}
	n%=8;
	pch=pch+m-1;
	for(int i=0;i<m;i++)
	{
		*(pch)<<=n;//���ַ�����nλ
		*(pch)|=(*pch-1)>>(8-n);//���ַ���nλ������ַ���nλ
		*(pch-1)<<=n;//���ַ�����nλ
		pch--;
	}
}

void CHuffmanEncoding::RShift_Spec( BYTE *pch,int m,int n )
{
	int bn=n/8;//���ֽ�����
	for(int i=0;i<m;i++)
	{
		if(i+bn<m)
		{
			pch[i]=pch[i+bn];
		}
		else
		{
			pch[i]=0;//���ֽ���������
		}
	}
	n%=8;
	pch--;
	for(int i=0;i<m;i++)
	{
		*(pch)|=((*(pch+1))<<(8-n));//���ַ���nλ������ַ���nλ
		*(pch+1)>>=n;//���ַ�����nλ
		pch++;
	}

	//TRACE("%d %d %d\r\n",*(pch-2),*(pch-1),*(pch-0));
	int c=0;
}
