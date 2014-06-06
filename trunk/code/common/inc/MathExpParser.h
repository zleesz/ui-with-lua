#pragma once
#include <vector>
#include <string>

enum ExpParserErrorCode
{
	EPEC_NO_ERROR = 0,
	EPEC_INVALID_PARAMETER = -1,
	EPEC_LAST_CHAR_NOT_NUMBER = -2,
	EPEC_RIGHT_PARENTHESIS_NOT_MATCH = -3,
	EPEC_LEFT_PARENTHESIS_NOT_MATCH = -4,
	EPEC_UNRECOGNIZEDCHAR = -5
};

class CMathExpParser
{
public:
	CMathExpParser(void)
	{
	}

	CMathExpParser(const wchar_t* pszExp)
	{
		int nlen = (int)wcslen(pszExp);
		if (nlen <= 0)
		{
			return;
		}
		m_strExp = pszExp;
	}

	~CMathExpParser(void)
	{
	}

private:
	std::wstring m_strExp;
	// 存储运算符栈
	std::vector<wchar_t> m_stackOperator;
	// 存储中间结果栈
	std::vector<std::wstring> m_stackInterResult;
private:
	inline void EmptyStack()
	{
		m_stackOperator.clear();
		m_stackInterResult.clear();
	}

	inline void PreProcess()
	{
		while (m_strExp.length() > 0)
		{
			if (IsUselessChar(m_strExp[0]))
			{
				m_strExp.erase(0);
			}
			else
			{
				break;
			}
		}
		int nlen = (int)m_strExp.length();
		for (int i = nlen - 1; i >= 0; --i)
		{
			if (IsUselessChar(m_strExp[i]))
			{
				m_strExp.erase(i);
			}
			else
			{
				break;
			}
		}
	}

	inline bool IsUselessChar(const wchar_t& c)
	{
		if (c == L' ' || c == L'\t')
		{
			return true;
		}
		return false;
	}

	inline bool IsRecognizedChar(const wchar_t& c)
	{
		if (IsUselessChar(c) || IsNumber(c) || IsOperator(c) || c == L'(' || c == L')')
		{
			return true;
		}
		return false;
	}

	inline bool IsNumber(const wchar_t& c)
	{
		if (c >= L'0' && c <= L'9')
		{
			return true;
		}
		return false;
	}

	inline bool IsOperator(const wchar_t& c)
	{
		if (c == L'+'|| c == L'-' || c == L'/' || c == L'*')
		{
			return true;
		}
		return false;
	}

	inline int CompareOperator(const wchar_t& op1, const wchar_t& op2)
	{
		if (op1 == L'+' || op1 == L'-')
		{
			if (op2 == L'+' || op2 == L'-')
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			if (op2 == L'+' || op2 == L'-')
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}

	inline int CalcByOperator(int op1, int op2, const wchar_t& op)
	{
		switch (op)
		{
		case L'+':
			return op1 + op2;
		case L'-':
			return op1 - op2;
		case L'*':
			return op1 * op2;
		case L'/':
			return op1 / op2;
		}
		ATLASSERT(false && "无效运算符！");
		return 0;
	}


	inline void DealOperator(const wchar_t& c)
	{
		if (m_stackOperator.size() <= 0)
		{
			m_stackOperator.push_back(c);
		}
		else
		{
			const wchar_t& cOp = m_stackOperator.back();
			if (cOp == L')' || CompareOperator(c, cOp) >= 0)
			{
				m_stackOperator.push_back(c);
			}
			else
			{
				m_stackOperator.pop_back();
				std::wstring strOp;
				strOp = cOp;
				m_stackInterResult.push_back(strOp);
				DealOperator(c);
			}
		}
	}

	inline int DealLeftParenthesis(const wchar_t& /*c*/)
	{
		while (m_stackOperator.size() > 0)
		{
			std::wstring strOp;
			strOp = m_stackOperator.back();
			m_stackOperator.pop_back();
			if (strOp == L")")
			{
				return EPEC_NO_ERROR;
			}
			m_stackInterResult.push_back(strOp);
		}
		ATLASSERT(false && "左括号不匹配！");
		return EPEC_LEFT_PARENTHESIS_NOT_MATCH;
	}

public:
	int Parser()
	{
		if (m_strExp.length() <= 0)
		{
			return -1;
		}
		EmptyStack();
		// 预处理,把前后的无效字符去掉
		PreProcess();

		int nLen = (int)m_strExp.length();
		for (int i = nLen - 1; i >= 0; --i)
		{
			wchar_t c = m_strExp[i];
			if (IsUselessChar(c))
			{
				continue;
			}
			if (!IsRecognizedChar(c))
			{
				ATLASSERT(false && "无效字符！");
				return EPEC_UNRECOGNIZEDCHAR;
			}
			if (IsNumber(c))
			{
				std::wstring strNumber;
				strNumber += c;
				if (i == 0)
				{
					m_stackInterResult.push_back(strNumber);
					break;
				}
				while (i >= 1)
				{
					--i;
					c = m_strExp[i];
					if (i == 0)
					{
						if (!IsNumber(c))
						{
							ATLASSERT(false && "表达式解析出错！");
							return EPEC_LAST_CHAR_NOT_NUMBER;
						}
						strNumber.insert(0, 1, c);
						m_stackInterResult.push_back(strNumber);
						break;
					}
					else if (!IsNumber(c))
					{
						m_stackInterResult.push_back(strNumber);
						break;
					}
					strNumber.insert(0, 1, c);
				}
				if (IsUselessChar(c))
				{
					continue;
				}
				else if (!IsRecognizedChar(c))
				{
					ATLASSERT(false && "无效字符！");
					return EPEC_UNRECOGNIZEDCHAR;
				}
			}
			if (IsOperator(c))
			{
				DealOperator(c);
			}
			else if (c == L')')
			{
				m_stackOperator.push_back(c);
			}
			else if (c == L'(')
			{
				int nDeal = DealLeftParenthesis(c);
				if (nDeal != EPEC_NO_ERROR)
				{
					ATLASSERT(false && "左括号不匹配！");
					return nDeal;
				}
			}
		}
		while (m_stackOperator.size() > 0)
		{
			std::wstring strOp;
			strOp = m_stackOperator.back();
			if (strOp == L")")
			{
				ATLASSERT(false && "右括号不匹配！");
				return EPEC_RIGHT_PARENTHESIS_NOT_MATCH;
			}
			m_stackOperator.pop_back();
			m_stackInterResult.push_back(strOp);
		}

		return 0;
	}

	int Parser(const wchar_t* pszExp)
	{
		int nlen = (int)wcslen(pszExp);
		if (nlen <= 0)
		{
			return -1;
		}
		m_strExp = pszExp;
		return Parser();
	}
	int Calc(const wchar_t* pszExp = NULL)
	{
		int nRes = 0;
		if (m_stackInterResult.size() <= 0)
		{
			if (pszExp == NULL)
			{
				nRes = Parser();
			}
			else
			{
				nRes = Parser(pszExp);
			}
		}
		if (nRes != EPEC_NO_ERROR)
		{
			return 0;
		}
		std::vector<int> stackNumber;
		int nResult = 0;
		if (m_stackInterResult.size() == 1)
		{
			nResult = _wtoi(m_stackInterResult.front().c_str());
		}
		else
		{
			while (m_stackInterResult.size() > 0)
			{
				std::wstring strTop;
				strTop = m_stackInterResult.front();
				if (IsOperator(strTop[0]))
				{
					int nTop1, nTop2;
					nTop1 = stackNumber.back();
					stackNumber.pop_back();
					nTop2 = stackNumber.back();
					stackNumber.pop_back();
					nResult = CalcByOperator(nTop1, nTop2, strTop[0]);
					stackNumber.push_back(nResult);
				}
				else
				{
					stackNumber.push_back(_wtoi(strTop.c_str()));
				}
				m_stackInterResult.erase(m_stackInterResult.begin());
			}
		}
		return nResult;
	}
};
