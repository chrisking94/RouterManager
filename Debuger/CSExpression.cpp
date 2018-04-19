#include "StdAfx.h"
#include "CSExpression.h"
#include "CSStatement.h"
#include "CharControl.h"
#include "StrParallelCmp.h"
#include "CSException.h"

#define NOT_WHITE_CHARS (*s!=' '&&*s!='	'&&*s!='\r'&&*s!='\n')

CCSExpression::CCSExpression( CCSVarManager *pvm ):m_expTree(NULL),m_pCVM (pvm) 
{
	
}


CCSExpression::~CCSExpression(void)
{
	DestroyTree(m_expTree);
}

CCSVar *CCSExpression::Execute()
{
	InOrder(m_expTree);

	return m_expTree->pVar;
}

BOOL CCSExpression::Parse( char *&s )
{
	enum
	{
		_operand,
		_operator,
	}LastOPType = _operator;
	//��ʽ�����ʽ�ַ���
	char buff[1024],sbuf[1024];
	int i = 0;
	buff[i++] = '#';
	CCSStringParser::SetParseObject(s);
	
	while(*s&&*s!=';')//������
	{
		if(*s=='"')
		{
			buff[i++] = '"';
			CCSStringParser::Parse(buff,&i);
			buff[i++] = '"';
		}
		else if(NOT_WHITE_CHARS)//�˵����ַ�
		{
			buff[i++] = *s++;
		}
		else
		{
			++s;//���ַ�
		}
	}
	buff[i++] = '#';
	buff[i++] = '\0';

	//ת����׼���ʽ
	USES_CONVERSION;
	char *s_std = buff;

	//���ɱ��ʽ��
	CCSVar *in[20];//��׺���ʽ
	CCSVar *post[20];//��׺���ʽ
	int len=0,inI=0,postI=0;//���ʽ����
	CCSVar *stack[20];
	int top=-1;
	CCharControl cc;
	char optID;

	CCSVar *p;
	char sbuff[20],buffI;
	bool bBack = false;//ָ���Ƿ����
	CCSOperator *pOptIC,*pOptIS;//In coming ,In stack
	bool bOnceAgin = true;
	CCSOperator::OPT_ID oID;

	cc.SetFlag('"',1);

	cc.SetFlag('0','9',2);

	cc.SetFlag('a','z',3);
	cc.SetFlag('A','A',3);
	cc.SetFlag('_',3);
	
	pOptIC = new CCSOperator;
	pOptIC->Parse(s_std);//#��ջ
	stack[++top] = new CCSVar((long)pOptIC);//#��ջ
	
	CCSStringParser::SetParseObject(s_std);
	pOptIC = new CCSOperator;
	while(*s_std)
	{
		//�������������⴦���жϽ����
		if(LastOPType == _operand)//����Ľ���Ҫ�õ�
		{
			if(*(s_std+1) == '=')
			{
				pOptIC->SetAsso(CCSOperator::_Right2Left);//�Ա������
			}
			else
			{
				pOptIC->SetAsso(CCSOperator::_Left2Right);
			}
		}
		else
		{
			pOptIC->SetAsso(CCSOperator::_Right2Left);
		}
		JUMP_WHITE_CHARS;
		if(pOptIC->Parse(s_std))//����Ƿ���ֲ�����
		{
			LastOPType = _operator;//���Ϊ������
			do//����һ��
			{
				pOptIS = (CCSOperator*)stack[top]->NumVal();
				bOnceAgin = false;

				if(pOptIC->ICP()==pOptIS->ISP())//( # ֮��
				{
					--top;//ֱ�ӳ�ջ
					len-=2;//�����ַ������ȼ�2����Ϊ������( #
					oID = pOptIC->GetID();
					//ɾ��(	#
					delete pOptIC;
					delete pOptIS;
					if(oID == CCSOperator::_Sharp)//���ʽ����
					{
						bOnceAgin = true;//�����ڱ�������ⲿ��ѭ��
						break;
					}
				}
				else if(pOptIC->ICP()<pOptIS->ISP())//�������ȼ�����
				{
					post[postI++] = stack[top--];
					bOnceAgin = true;//�����鿴���Ƿ񻹿��Զ�
				}
				else
				{
					p = new CCSVar((long)pOptIC);
					p->ForceChangeType(CCSVarType::_void);
					optID = pOptIC->GetID();
					if(optID!=CCSOperator::_LBracket
						&&optID!=CCSOperator::_RBracket
						&&optID!=CCSOperator::_Sharp)	in[inI++] = p;
					stack[++top] = p;//������ȼ�δ֪����ջ�ȴ������
				}
			}while(bOnceAgin);
			
			if(bOnceAgin) break;//����;�������鿴����˵����
			pOptIC = new CCSOperator;
		}
		else//������
		{
			LastOPType = _operand;//���Ϊ������

			if(stack[top]->GetPtr() == ',')//������������ֱ�ӳ�ջ
			{
				post[postI++] = stack[top--];
			}

			switch(cc.Table[*s_std])//���ദ��
			{
			case 1://�ַ���
				CCSStringParser::Parse(sbuf,0);
				p = new CCSVar(sbuf);
				//len++;
				break;
			case 2://����
				buffI = 0;
				while(cc.Table[*s_std]==2)
				{
					sbuff[buffI++] = *s_std++;
				}
				sbuff[buffI]=0;
				p = new CCSVar(sbuff);
				p->ToNumber();
				break;
			case 3://����
				buffI = 0;
				while(cc.Table[*s_std]==2||cc.Table[*s_std]==3)//��ĸ��������ɱ�����
				{
					sbuff[buffI++] = *s_std++;
				}
				sbuff[buffI] = 0;
				//����ָ�����������Ѱַ�������й�����Ѱַ
				p = new CCSVar(CCSVarType::_ptr);
				p->SetName(sbuff);
				break;
			}
			in[inI++] = p;
			post[postI++]=p;
		}
	}

	if(*s==';') ++s;//����������

	//ͨ��ǰ�����кͺ������д������ʽ��
	if(inI>0)//==0�ǿձ��ʽ
	{
		--inI; 
		--postI;
		m_expTree = InPostCreateTree(&in[0],&in[inI],&post[0],&post[postI]);

		return TRUE;
	}
	
	return FALSE;//�ձ��ʽ
}

CCSExpression::ExpTree CCSExpression::InPostCreateTree( CCSVar **in1,CCSVar **in2,CCSVar **post1,CCSVar **post2 )
{
	if(post1>post2) return NULL;
	CCSVar **pin = in1;
	ExpTree pRet;
	while(pin<=in2)//�������������еĽڵ�
	{
		if(*pin == *post2)
		{
			break;
		}
		++pin;
	}
	//�������ڵ�
	pRet = new ExpNode;
	pRet->pVar = *pin;
	if(pRet->pVar->TypeOf() == CCSVarType::_void) pRet->pOpt = (CCSOperator *)pRet->pVar->GetPtr();//��������������������
	else pRet->pOpt = NULL;
	pRet->lchild = InPostCreateTree(in1,pin-1,post1,post1+(pin-in1)-1);
	pRet->rchild = InPostCreateTree(pin+1,in2,post1+(pin-in1),post2-1);

	return pRet;
}

void CCSExpression::InOrder( ExpTree p )
{
	if(!p) return;
	
	if(p->pOpt!=NULL)//�����
	{
		CCSVar *pLOperand,*pROperand;
		
		if(p->lchild)
		{
			InOrder(p->lchild);
			pLOperand = p->lchild->pVar;
			if(pLOperand->TypeOf() == CCSVarType::_ptr)
			{
				pLOperand = (CCSVar *)pLOperand->GetPtr();
			}
		}
		if(p->rchild)
		{
			InOrder(p->rchild);
			pROperand = p->rchild->pVar;
			if(pROperand->TypeOf() == CCSVarType::_ptr)
			{
				pROperand = (CCSVar *)pROperand->GetPtr();
			}
		}
		
		switch(p->pOpt->GetID())
		{
		case CCSOperator::_Add:
  			*(p->pVar) = *(pLOperand) + *(pROperand);
			break;
		case CCSOperator::_Sub:
			*(p->pVar) = *(pLOperand) - *(pROperand);
			break;
		case CCSOperator::_Mul:
			*(p->pVar) = *(pLOperand) * *(pROperand);
			break;
		case CCSOperator::_Div:
			*(p->pVar) = *(pLOperand) / *(pROperand);
			break;
		case CCSOperator::_Mod:
			*(p->pVar) = *(pLOperand) % *(pROperand);
			break;
		case CCSOperator::_BEOR:
			*(p->pVar) = *(pLOperand) ^ *(pROperand);
			break;
		case CCSOperator::_Assign://��ֵ���ʽ
			*(p->pVar) = *(pLOperand) = *(pROperand);
			return;
		case CCSOperator::_At:
			break;

		//�Ա������
		case CCSOperator::_PrePlusPlus:
			*(p->pVar) = ++ (*(pROperand));
			return;
		case CCSOperator::_PostPlusPlus:
			*(p->pVar) = (*(pLOperand)) ++;
			return;
		case CCSOperator::_PreMinusMinus:
			*(p->pVar) = -- (*(pROperand));
			return;
		case CCSOperator::_PostMinusMinus:
			*(p->pVar) = (*(pLOperand)) --;
			return;
		case CCSOperator::_AAdd:
			(*pLOperand) = (*pLOperand) + (*pROperand);
			(*p->pVar) = (*pLOperand);
			return;
		case CCSOperator::_ASub:
			(*pLOperand) = (*pLOperand) - (*pROperand);
			(*p->pVar) = (*pLOperand);
			return;
		case CCSOperator::_AMul:
			(*pLOperand) = (*pLOperand) * (*pROperand);
			(*p->pVar) = (*pLOperand);
			return;
		case CCSOperator::_ADiv:
			(*pLOperand) = (*pLOperand) / (*pROperand);
			(*p->pVar) = (*pLOperand);
			return;
		case CCSOperator::_AMod:
			(*pLOperand) = (*pLOperand) % (*pROperand);
			(*p->pVar) = (*pLOperand);
			return;


		//�Ƚ������
		case CCSOperator::_Equ:
			*(p->pVar) =( *(pLOperand) == *(pROperand) );
			return;
		case CCSOperator::_NEqu:
			*(p->pVar) =( *(pLOperand) != *(pROperand) );
			return;
		case CCSOperator::_GR:
			*(p->pVar) =( *(pLOperand) > *(pROperand) );
			return;
		case CCSOperator::_GE:
			*(p->pVar) =( *(pLOperand) >= *(pROperand) );
			return;
		case CCSOperator::_LS:
			*(p->pVar) =( *(pLOperand) < *(pROperand) );
			return;
		case CCSOperator::_LE:
			*(p->pVar) =( *(pLOperand) <= *(pROperand) );
			return;
		}
	}
	else if(p->pVar&&p->pVar->TypeOf() == CCSVarType::_ptr)//ָ�����ͣ�Ѱַ
	{
		if(p->pVar->IsBadPtr())
		{
			*(p->pVar) = m_pCVM->FindVar(p->pVar->GetName());//����Ѱַ
			if(p->pVar->IsBadPtr())
			{
				CCSException e(CCSException::_UnDeclared);
				e.AppendMsg("CScript.Expression������ ");
				e.AppendMsg(p->pVar->GetName());
				e.AppendMsg(" δ����");
				e.Throw();
			}
		}
	}
}

void CCSExpression::DestroyTree( ExpTree p )
{
	if(!p) return;
	DestroyTree(p->lchild);
	DestroyTree(p->rchild);
	delete p->pOpt;
	delete p->pVar;
	delete p;
	p = NULL;
}
