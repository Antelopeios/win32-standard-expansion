// Copyright 2011, 木头云
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////////////
// SSE - SSE指令优化算法
//
// Author:	木头云
// Home:	http://hi.baidu.com/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-07-12
// Version:	1.0.0000.1033
//
// History:
//	- 1.0.0000.1033(2011-07-12)	@ 开始构建CSSE
//////////////////////////////////////////////////////////////////

#ifndef __SSE_h__
#define __SSE_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <xmmintrin.h>

EXP_BEG

//////////////////////////////////////////////////////////////////

class CSSE
{
protected:
	static int m_nSupport;

public:
	EXP_INLINE static bool IsSupport()
	{
		if (m_nSupport == -1)
		{
			__try 
			{
				__asm 
				{
					xorpd xmm0, xmm0	// executing SSE instruction
				}
			}
			__except (EXCEPTION_EXECUTE_HANDLER) 
			{
				if (_exception_code() == STATUS_ILLEGAL_INSTRUCTION) 
				{
					m_nSupport = 0;
				}
				m_nSupport = 0;
			}
			if(m_nSupport == -1)
			{
				m_nSupport = 1;
			}
		}
		return m_nSupport == 1;
	}

	//////////////////////////////////////////////////////////////////

	// 赋值
	EXP_INLINE static void SetNor(__m128& pr, float p1, float p2, float p3, float p4)
	{
		pr.m128_f32[3] = p1;
		pr.m128_f32[2] = p2;
		pr.m128_f32[1] = p3;
		pr.m128_f32[0] = p4;
	}
	EXP_INLINE static void SetSSE(__m128& pr, float p1, float p2, float p3, float p4)
	{
		pr = _mm_set_ps(p1, p2, p3, p4);
	}
	EXP_INLINE static void Set(__m128& pr, float p1, float p2, float p3, float p4)
	{
		if (IsSupport())
			SetSSE(pr, p1, p2, p3, p4);
		else
			SetNor(pr, p1, p2, p3, p4);
	}

#define ExSetSSE(pr, p1, p2, p3, p4) \
	{ \
		pr = _mm_set_ps((float)p1, (float)(p2), (float)(p3), (float)(p4)); \
	}

	//////////////////////////////////////////////////////////////////

	// 拷贝
	EXP_INLINE static void MovNor(float* pr, float* p1, int nLen = 4)
	{
		if (nLen <= 0) return;
		memcpy(pr, p1, nLen * sizeof(float));
	}
	EXP_INLINE static void MovSSE(float* pr, float* p1, int nLen = 4)
	{
		int sse_len = nLen >> 2;
		if (sse_len)
		{
			__asm
			{
				mov eax, [p1]
				mov esi, [pr]
				mov ecx, [sse_len]

			start_loop:

				movups xmm0, [eax]	// xmm0 = [eax]
				movups [esi], xmm0	// [esi] = xmm0

				add eax, 10h
				add esi, 10h

				dec ecx
				jnz start_loop
			}
		}
		sse_len <<= 2;
		MovNor(pr + sse_len, p1 + sse_len, nLen - sse_len);
	}
	EXP_INLINE static void Mov(float* pr, float* p1, int nLen = 4)
	{
		if (IsSupport())
			MovSSE(pr, p1, nLen);
		else
			MovNor(pr, p1, nLen);
	}

	//////////////////////////////////////////////////////////////////

	// 相加
	EXP_INLINE static void AddNor(float* pr, float* p1, float* p2, int nLen = 4)
	{
		if (nLen <= 0) return;
		for(int i = 0; i < nLen; ++i)
		{
			*pr = (*p1) + (*p2);
			++p1;
			++p2;
			++pr;
		}
	}
	EXP_INLINE static void AddSSE(float* pr, float* p1, float* p2, int nLen = 4)
	{
		int sse_len = nLen >> 2;
		if (sse_len)
		{
			__asm
			{
				mov eax, [p1]
				mov esi, [p2]
				mov ecx, [pr]
				mov edx, [sse_len]

			start_loop:

				movups xmm0, [eax]	// xmm0 = [eax]
				movups xmm1, [esi]	// xmm1 = [esi]
				addps xmm0, xmm1	// xmm0 = xmm0 + xmm1
				movups [ecx], xmm0	// [ecx] = xmm0

				add eax, 10h
				add esi, 10h
				add ecx, 10h

				dec edx
				jnz start_loop
			}
		}
		sse_len <<= 2;
		AddNor(pr + sse_len, p1 + sse_len, p2 + sse_len, nLen - sse_len);
	}
	EXP_INLINE static void Add(float* pr, float* p1, float* p2, int nLen = 4)
	{
		if (IsSupport())
			AddSSE(pr, p1, p2, nLen);
		else
			AddNor(pr, p1, p2, nLen);
	}

#define ExAddSSE(pr, p1, p2) \
	{ \
		float* sr = (float*)(pr); \
		float* s1 = (float*)(p1); \
		float* s2 = (float*)(p2); \
		__asm \
		{ \
			mov eax, [s1] \
			mov esi, [s2] \
			mov ecx, [sr] \
			movups xmm0, [eax] \
			movups xmm1, [esi] \
			addps xmm0, xmm1 \
			movups [ecx], xmm0 \
		} \
	}

	//////////////////////////////////////////////////////////////////

	// 相减
	EXP_INLINE static void SubNor(float* pr, float* p1, float* p2, int nLen = 4)
	{
		if (nLen <= 0) return;
		for(int i = 0; i < nLen; ++i)
		{
			*pr = (*p1) - (*p2);
			++p1;
			++p2;
			++pr;
		}
	}
	EXP_INLINE static void SubSSE(float* pr, float* p1, float* p2, int nLen = 4)
	{
		int sse_len = nLen >> 2;
		if (sse_len)
		{
			__asm
			{
				mov eax, [p1]
				mov esi, [p2]
				mov ecx, [pr]
				mov edx, [sse_len]

			start_loop:

				movups xmm0, [eax]	// xmm0 = [eax]
				movups xmm1, [esi]	// xmm1 = [esi]
				subps xmm0, xmm1	// xmm0 = xmm0 + xmm1
				movups [ecx], xmm0	// [ecx] = xmm0

				add eax, 10h
				add esi, 10h
				add ecx, 10h

				dec edx
				jnz start_loop
			}
		}
		sse_len <<= 2;
		SubNor(pr + sse_len, p1 + sse_len, p2 + sse_len, nLen - sse_len);
	}
	EXP_INLINE static void Sub(float* pr, float* p1, float* p2, int nLen = 4)
	{
		if (IsSupport())
			SubSSE(pr, p1, p2, nLen);
		else
			SubNor(pr, p1, p2, nLen);
	}

#define ExSubSSE(pr, p1, p2) \
	{ \
		float* sr = (float*)(pr); \
		float* s1 = (float*)(p1); \
		float* s2 = (float*)(p2); \
		__asm \
		{ \
			mov eax, [s1] \
			mov esi, [s2] \
			mov ecx, [sr] \
			movups xmm0, [eax] \
			movups xmm1, [esi] \
			subps xmm0, xmm1 \
			movups [ecx], xmm0 \
		} \
	}

	//////////////////////////////////////////////////////////////////

	// 相乘
	EXP_INLINE static void MulNor(float* pr, float* p1, float* p2, int nLen = 4)
	{
		if (nLen <= 0) return;
		for(int i = 0; i < nLen; ++i)
		{
			*pr = (*p1) * (*p2);
			++p1;
			++p2;
			++pr;
		}
	}
	EXP_INLINE static void MulSSE(float* pr, float* p1, float* p2, int nLen = 4)
	{
		int sse_len = nLen >> 2;
		if (sse_len)
		{
			__asm
			{
				mov eax, [p1]
				mov esi, [p2]
				mov ecx, [pr]
				mov edx, [sse_len]

			start_loop:

				movups xmm0, [eax]	// xmm0 = [eax]
				movups xmm1, [esi]	// xmm1 = [esi]
				mulps xmm0, xmm1	// xmm0 = xmm0 * xmm1
				movups [ecx], xmm0	// [ecx] = xmm0

				add eax, 10h
				add esi, 10h
				add ecx, 10h

				dec edx
				jnz start_loop
			}
		}
		sse_len <<= 2;
		MulNor(pr + sse_len, p1 + sse_len, p2 + sse_len, nLen - sse_len);
	}
	EXP_INLINE static void Mul(float* pr, float* p1, float* p2, int nLen = 4)
	{
		if (IsSupport())
			MulSSE(pr, p1, p2, nLen);
		else
			MulNor(pr, p1, p2, nLen);
	}

#define ExMulSSE(pr, p1, p2) \
	{ \
		float* sr = (float*)(pr); \
		float* s1 = (float*)(p1); \
		float* s2 = (float*)(p2); \
		__asm \
		{ \
			mov eax, [s1] \
			mov esi, [s2] \
			mov ecx, [sr] \
			movups xmm0, [eax] \
			movups xmm1, [esi] \
			mulps xmm0, xmm1 \
			movups [ecx], xmm0 \
		} \
	}

	//////////////////////////////////////////////////////////////////

	// 相除
	EXP_INLINE static void DivNor(float* pr, float* p1, float* p2, int nLen = 4)
	{
		if (nLen <= 0) return;
		for(int i = 0; i < nLen; ++i)
		{
			*pr = (*p1) / (*p2);
			++p1;
			++p2;
			++pr;
		}
	}
	EXP_INLINE static void DivSSE(float* pr, float* p1, float* p2, int nLen = 4)
	{
		int sse_len = nLen >> 2;
		if (sse_len)
		{
			__asm
			{
				mov eax, [p1]
				mov esi, [p2]
				mov ecx, [pr]
				mov edx, [sse_len]

			start_loop:

				movups xmm0, [eax]	// xmm0 = [eax]
				movups xmm1, [esi]	// xmm1 = [esi]
				divps xmm0, xmm1	// xmm0 = xmm0 * xmm1
				movups [ecx], xmm0	// [ecx] = xmm0

				add eax, 10h
				add esi, 10h
				add ecx, 10h

				dec edx
				jnz start_loop
			}
		}
		sse_len <<= 2;
		DivNor(pr + sse_len, p1 + sse_len, p2 + sse_len, nLen - sse_len);
	}
	EXP_INLINE static void Div(float* pr, float* p1, float* p2, int nLen = 4)
	{
		if (IsSupport())
			DivSSE(pr, p1, p2, nLen);
		else
			DivNor(pr, p1, p2, nLen);
	}

#define ExDivSSE(pr, p1, p2) \
	{ \
		float* sr = (float*)(pr); \
		float* s1 = (float*)(p1); \
		float* s2 = (float*)(p2); \
		__asm \
		{ \
			mov eax, [s1] \
			mov esi, [s2] \
			mov ecx, [sr] \
			movups xmm0, [eax] \
			movups xmm1, [esi] \
			divps xmm0, xmm1 \
			movups [ecx], xmm0 \
		} \
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__SSE_h__*/