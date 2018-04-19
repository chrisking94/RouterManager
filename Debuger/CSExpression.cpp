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
	//格式化表达式字符串
	char buff[1024],sbuf[1024];
	int i = 0;
	buff[i++] = '#';
	CCSStringParser::SetParseObject(s);
	
	while(*s&&*s!=';')//结束符
	{
		if(*s=='"')
		{
			buff[i++] = '"';
			CCSStringParser::Parse(buff,&i);
			buff[i++] = '"';
		}
		else if(NOT_WHITE_CHARS)//滤掉白字符
		{
			buff[i++] = *s++;
		}
		else
		{
			++s;//白字符
		}
	}
	buff[i++] = '#';
	buff[i++] = '\0';

	//转换标准表达式
	USES_CONVERSION;
	char *s_std = buff;

	//生成表达式树
	CCSVar *in[20];//中缀表达式
	CCSVar *post[20];//后缀表达式
	int len=0,inI=0,postI=0;//表达式长度
	CCSVar *stack[20];
	int top=-1;
	CCharControl cc;
	char optID;

	CCSVar *p;
	char sbuff[20],buffI;
	bool bBack = false;//指针是否回退
	CCSOperator *pOptIC,*pOptIS;//In coming ,In stack
	bool bOnceAgin = true;
	CCSOperator::OPT_ID oID;

	cc.SetFlag('"',1);

	cc.SetFlag('0','9',2);

	cc.SetFlag('a','z',3);
	cc.SetFlag('A','A',3);
	cc.SetFlag('_',3);
	
	pOptIC = new CCSOperator;
	pOptIC->Parse(s_std);//#入栈
	stack[++top] = new CCSVar((long)pOptIC);//#入栈
	
	CCSStringParser::SetParseObject(s_std);
	pOptIC = new CCSOperator;
	while(*s_std)
	{
		//操作符属性特殊处理，判断结合性
		if(LastOPType == _operand)//下面的解析要用到
		{
			if(*(s_std+1) == '=')
			{
				pOptIC->SetAsso(CCSOperator::_Right2Left);//自变运算符
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
		if(pOptIC->Parse(s_std))//检测是否出现操作符
		{
			LastOPType = _operator;//标记为操作符
			do//至少一次
			{
				pOptIS = (CCSOperator*)stack[top]->NumVal();
				bOnceAgin = false;

				if(pOptIC->ICP()==pOptIS->ISP())//( # 之类
				{
					--top;//直接出栈
					len-=2;//后序字符串长度减2，因为不包括( #
					oID = pOptIC->GetID();
					//删除(	#
					delete pOptIC;
					delete pOptIS;
					if(oID == CCSOperator::_Sharp)//表达式结束
					{
						bOnceAgin = true;//①用于标记跳出外部大循环
						break;
					}
				}
				else if(pOptIC->ICP()<pOptIS->ISP())//将高优先级顶出
				{
					post[postI++] = stack[top--];
					bOnceAgin = true;//连续查看，是否还可以顶
				}
				else
				{
					p = new CCSVar((long)pOptIC);
					p->ForceChangeType(CCSVarType::_void);
					optID = pOptIC->GetID();
					if(optID!=CCSOperator::_LBracket
						&&optID!=CCSOperator::_RBracket
						&&optID!=CCSOperator::_Sharp)	in[inI++] = p;
					stack[++top] = p;//相对优先级未知，入栈等待运算符
				}
			}while(bOnceAgin);
			
			if(bOnceAgin) break;//多用途变量，查看上面说明①
			pOptIC = new CCSOperator;
		}
		else//操作数
		{
			LastOPType = _operand;//标记为操作数

			if(stack[top]->GetPtr() == ',')//逗号遇操作数直接出栈
			{
				post[postI++] = stack[top--];
			}

			switch(cc.Table[*s_std])//分类处理
			{
			case 1://字符串
				CCSStringParser::Parse(sbuf,0);
				p = new CCSVar(sbuf);
				//len++;
				break;
			case 2://数字
				buffI = 0;
				while(cc.Table[*s_std]==2)
				{
					sbuff[buffI++] = *s_std++;
				}
				sbuff[buffI]=0;
				p = new CCSVar(sbuff);
				p->ToNumber();
				break;
			case 3://变量
				buffI = 0;
				while(cc.Table[*s_std]==2||cc.Table[*s_std]==3)//字母和数字组成变量名
				{
					sbuff[buffI++] = *s_std++;
				}
				sbuff[buffI] = 0;
				//创建指针变量，但不寻址，在运行过程中寻址
				p = new CCSVar(CCSVarType::_ptr);
				p->SetName(sbuff);
				break;
			}
			in[inI++] = p;
			post[postI++]=p;
		}
	}

	if(*s==';') ++s;//跳过结束符

	//通过前序序列和后序序列创建表达式树
	if(inI>0)//==0是空表达式
	{
		--inI; 
		--postI;
		m_expTree = InPostCreateTree(&in[0],&in[inI],&post[0],&post[postI]);

		return TRUE;
	}
	
	return FALSE;//空表达式
}

CCSExpression::ExpTree CCSExpression::InPostCreateTree( CCSVar **in1,CCSVar **in2,CCSVar **post1,CCSVar **post2 )
{
	if(post1>post2) return NULL;
	CCSVar **pin = in1;
	ExpTree pRet;
	while(pin<=in2)//查找中序序列中的节点
	{
		if(*pin == *post2)
		{
			break;
		}
		++pin;
	}
	//创建根节点
	pRet = new ExpNode;
	pRet->pVar = *pin;
	if(pRet->pVar->TypeOf() == CCSVarType::_void) pRet->pOpt = (CCSOperator *)pRet->pVar->GetPtr();//这种是特殊的运算符变量
	else pRet->pOpt = NULL;
	pRet->lchild = InPostCreateTree(in1,pin-1,post1,post1+(pin-in1)-1);
	pRet->rchild = InPostCreateTree(pin+1,in2,post1+(pin-in1),post2-1);

	return pRet;
}

void CCSExpression::InOrder( ExpTree p )
{
	if(!p) return;
	
	if(p->pOpt!=NULL)//运算符
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
		case CCSOperator::_Assign://赋值表达式
			*(p->pVar) = *(pLOperand) = *(pROperand);
			return;
		case CCSOperator::_At:
			break;

		//自变运算符
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


		//比较运算符
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
	else if(p->pVar&&p->pVar->TypeOf() == CCSVarType::_ptr)//指针类型，寻址
	{
		if(p->pVar->IsBadPtr())
		{
			*(p->pVar) = m_pCVM->FindVar(p->pVar->GetName());//重新寻址
			if(p->pVar->IsBadPtr())
			{
				CCSException e(CCSException::_UnDeclared);
				e.AppendMsg("CScript.Expression：变量 ");
				e.AppendMsg(p->pVar->GetName());
				e.AppendMsg(" 未声明");
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
