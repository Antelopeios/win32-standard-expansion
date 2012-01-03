// Copyright 2011-2012, 木头云
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
// ImgASM - 图像渲染加速
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-03
// Version:	1.0.0003.1925
//
// History:
//	- 1.0.0002.1814(2011-12-22)	# 修正ExASM_punpap()中的算法错误
//								# 修正ExASM_punpck()中可能的内存访问异常
//	- 1.0.0003.1925(2012-01-03)	+ 添加CImgASM,将所有的图像渲染算法集中
//////////////////////////////////////////////////////////////////

#ifndef __ImgASM_hpp__
#define __ImgASM_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////
// ExASM
//////////////////////////////////////////////////////////////////
// 计算掩码

#define ExASM_mask(xmm) \
	__asm pxor xmm, xmm

#define ExASM_maskcm(xmm, xmsk) \
	__asm pcmpeqd xmm, xmm \
	__asm punpcklbw xmm, xmsk

//////////////////////////////////////////////////////////////////
// 拷贝

#define ExASM_movb(xmm, val) \
	__asm movd xmm, val \
	__asm punpcklbw xmm, xmm \
	__asm punpcklwd xmm, xmm \
	__asm punpckldq xmm, xmm \
	__asm punpcklqdq xmm, xmm

#define ExASM_movw(xmm, val) \
	__asm movd xmm, val \
	__asm punpcklwd xmm, xmm \
	__asm punpckldq xmm, xmm \
	__asm punpcklqdq xmm, xmm

#define ExASM_movd(xmm, val) \
	__asm movd xmm, val \
	__asm punpckldq xmm, xmm \
	__asm punpcklqdq xmm, xmm

#define ExASM_movq(xmm, val) \
	__asm movq xmm, val \
	__asm punpcklqdq xmm, xmm

#define ExASM_copy(reg1, reg2, xtmp) \
	__asm movupd xtmp, [reg1] \
	__asm movupd [reg2], xtmp

//////////////////////////////////////////////////////////////////
// 拓展

#define ExASM_punpck(xmm, reg, xmsk) \
	__asm movlpd xmm, [reg] \
	__asm punpcklbw xmm, xmsk

#define ExASM_punpap(xmm, xreg, xtmp) \
	__asm movupd xmm, xreg \
	__asm punpcklwd xmm, xmm \
	__asm punpckhdq xmm, xmm \
	__asm movupd xtmp, xreg \
	__asm punpckhwd xtmp, xtmp \
	__asm punpckhdq xtmp, xtmp \
	__asm punpckhqdq xmm, xtmp

//////////////////////////////////////////////////////////////////
// 类型转换

#define ExASM_cvtwtofl(xmm, xreg, xmsk) \
	__asm movupd xmm, xreg \
	__asm punpcklwd xmm, xmsk \
	__asm cvtdq2ps xmm, xmm

#define ExASM_cvtwtofh(xmm, xreg, xmsk) \
	__asm movupd xmm, xreg \
	__asm punpckhwd xmm, xmsk \
	__asm cvtdq2ps xmm, xmm

#define ExASM_cvtftowl(xmm, xreg) \
	__asm cvttps2dq xreg, xreg \
	__asm packssdw xreg, xreg \
	__asm punpckhqdq xreg, xmm /*movlpd*/ \
	__asm movupd xmm, xreg

#define ExASM_cvtftowh(xmm, xreg) \
	__asm cvttps2dq xreg, xreg \
	__asm packssdw xreg, xreg \
	__asm punpcklqdq xmm, xreg /*movhpd*/

//////////////////////////////////////////////////////////////////
// 四则运算

#define ExASM_divcm(xmm, xtmp, xmsk_1) \
	__asm movupd xtmp, xmm \
	__asm psrlw xtmp, 8 \
	__asm paddw xtmp, xmsk_1 \
	__asm paddw xmm, xtmp \
	__asm psrlw xmm, 8

#define ExASM_div(xmm0, xmm1, xtmp0, xtmp1, xmsk) \
	__asm pcmpeqw xmm1, xmsk \
	ExASM_cvtwtofl(xtmp0, xmm0, xmsk) \
	ExASM_cvtwtofl(xtmp1, xmm1, xmsk) \
	__asm divps xtmp0, xtmp1 \
	ExASM_cvtftowl(xmm0, xtmp0) \
	ExASM_cvtwtofh(xtmp0, xmm0, xmsk) \
	ExASM_cvtwtofh(xtmp1, xmm1, xmsk) \
	__asm divps xtmp0, xtmp1 \
	ExASM_cvtftowh(xmm0, xtmp0)

#define ExASM_mulcm(xmm, xtmp) \
	__asm movupd xtmp, xmm \
	__asm psllw xmm, 8 \
	__asm psubw xmm, xtmp

#define ExASM_cmsub(xmm, xreg, xmsk) \
	ExASM_maskcm(xmm, xmsk) \
	__asm psubw xmm, xreg

//////////////////////////////////////////////////////////////////
// 合并

#define ExASM_combpx(xmm1, xmm2) \
	__asm psllq xmm1, 15 \
	__asm psrlq xmm1, 15 \
	__asm psrlq xmm2, 47 \
	__asm psllq xmm2, 47 \
	__asm por xmm1, xmm2

//////////////////////////////////////////////////////////////////
// CImgASM
//////////////////////////////////////////////////////////////////

class CImgASM
{
public:
	// 像素拷贝
	EXP_INLINE static void PixCpy(pixel_t* pd, pixel_t* ps, LONG size)
	{
		if (size <= 0) return;
		LONG sse_len = size >> 2;
		if (sse_len)
		__asm
		{
			mov	eax, [ps]
			mov edx, [pd]
			mov ecx, [sse_len]
		sse_loop_1:
			ExASM_copy(eax, edx, xmm0);
			add eax, 10h
			add edx, 10h
			dec ecx
			jnz sse_loop_1
		}
		LONG sse_lst = sse_len << 2;
		if (sse_lst < size)
			memcpy(pd + sse_lst, ps + sse_lst, (size - sse_lst) << 2);
	}
	// 像素赋值
	EXP_INLINE static void PixZero(pixel_t* ps, LONG size)
	{
		if (size <= 0) return;
		ZeroMemory(ps, size << 2);
	}
	EXP_INLINE static void PixSetC(pixel_t* ps, LONG size, chann_t c)
	{
		if (size <= 0) return;
		memset(ps, c, size << 2);
	}
	EXP_INLINE static void PixSetP(pixel_t* ps, LONG size, pixel_t c)
	{
		if (size <= 0) return;
		LONG sse_len = size >> 2;
		if (sse_len)
		__asm
		{
			mov	eax, [ps]
			mov ecx, [sse_len]
		sse_loop_1:
			movupd xmm0, [eax]
			ExASM_movd(xmm0, c)
			movupd [eax], xmm0
			add eax, 10h
			dec ecx
			jnz sse_loop_1
		}
		LONG sse_lst = sse_len << 2;
		for(LONG x = sse_lst; x < size; ++x) ps[x] = c;
	}
	// 图像预乘
	EXP_INLINE static void PixPreMul(pixel_t* ps, LONG size)
	{
		if (size <= 0) return;
		for(LONG x = 0; x < size; ++x)
		{
			chann_t a_s = ExGetA(ps[x]);
			if (a_s == 0)
			{
				ps[x] = 0;
				continue;
			}
			if (a_s == EXP_CM) continue;
			int r = ExGetR(ps[x]) * a_s;
			int g = ExGetG(ps[x]) * a_s;
			int b = ExGetB(ps[x]) * a_s;
			ps[x] = ExRGBA(ExDivCM(r), ExDivCM(g), ExDivCM(b), a_s);
		}
	}
	// 将ps按照alpha的透明度过渡到pd
	EXP_INLINE static void PixTra(pixel_t* pd, pixel_t* ps, LONG size, chann_t alpha)
	{
		if (size <= 0) return;
		if (alpha == 0) return;
		if (alpha == EXP_CM)
		{
			PixCpy(pd, ps, size);
			return;
		}
		LONG sse_len = size >> 1;
		DWORD pa = alpha;
		DWORD pi = EXP_CM - alpha;
		DWORD pt = 1;
		if (sse_len)
		__asm
		{
			ExASM_movw(xmm5, pt)
			ExASM_movw(xmm7, pa)
			ExASM_movw(xmm4, pi)
			ExASM_mask(xmm6)
			mov	eax, [ps]
			mov edx, [pd]
			mov ecx, [sse_len]
		sse_loop_1:
			// 拓展ps
			ExASM_punpck(xmm0, eax, xmm6)
			// 拓展pd	 
			ExASM_punpck(xmm1, edx, xmm6)
			// 混色计算
			pmullw xmm0, xmm7
			pmullw xmm1, xmm4
			paddw xmm0, xmm1
			ExASM_divcm(xmm0, xmm2, xmm5)
			// 恢复像素
			packuswb xmm0, xmm0
			// 保存结果
			movlpd [edx], xmm0
			// 循环扫描
			add eax, 08h
			add edx, 08h
			dec ecx
			jnz sse_loop_1
		}
		LONG sse_lst = sse_len << 1;
		for(LONG x = sse_lst; x < size; ++x)
		{
			if (ps[x] == pd[x]) continue;
			int r = ExGetR(pd[x]) * pi + ExGetR(ps[x]) * alpha;
			int g = ExGetG(pd[x]) * pi + ExGetG(ps[x]) * alpha;
			int b = ExGetB(pd[x]) * pi + ExGetB(ps[x]) * alpha;
			int a = ExGetA(pd[x]) * pi + ExGetA(ps[x]) * alpha;
			pd[x] = ExRGBA(ExDivCM(r), ExDivCM(g), ExDivCM(b), ExDivCM(a));
		}
	}
	// AlphaBlend
	EXP_INLINE static void PixBlend(pixel_t* pd, pixel_t* ps, LONG size)
	{
		if (size <= 0) return;
		LONG sse_len = size >> 1;
		DWORD pt = 1;
		if (sse_len)
		__asm
		{
			ExASM_movw(xmm5, pt)
			ExASM_mask(xmm6)
			ExASM_maskcm(xmm7, xmm6)
			mov	eax, [ps]
			mov edx, [pd]
			mov ecx, [sse_len]
		sse_loop_1:
			// 拓展ps
			ExASM_punpck(xmm0, eax, xmm6)
			// 拓展pd	 
			ExASM_punpck(xmm1, edx, xmm6)
			// 拓展as
			ExASM_punpap(xmm2, xmm0, xmm3)
			// 生成ai
			movupd xmm3, xmm7
			psubw xmm3, xmm2
			// 混色计算
			pmullw xmm1, xmm3
			ExASM_divcm(xmm1, xmm2, xmm5)
			paddw xmm0, xmm1
			// 恢复像素
			packuswb xmm0, xmm0
			// 保存结果
			movlpd [edx], xmm0
			// 循环扫描
			add eax, 08h
			add edx, 08h
			dec ecx
			jnz sse_loop_1
		}
		LONG sse_lst = sse_len << 1;
		for(LONG x = sse_lst; x < size; ++x)
		{
			chann_t a_s = ExGetA(ps[x]);
			if (a_s == EXP_CM)
			{
				pd[x] = ps[x];
				continue;
			}
			chann_t a_i = EXP_CM - a_s;
			int tmp;
			int a = a_s + (tmp = (ExGetA(pd[x]) * a_i), ExDivCM(tmp));
			int r = ExGetR(ps[x]) + (tmp = (ExGetR(pd[x]) * a_i), ExDivCM(tmp));
			int g = ExGetG(ps[x]) + (tmp = (ExGetG(pd[x]) * a_i), ExDivCM(tmp));
			int b = ExGetB(ps[x]) + (tmp = (ExGetB(pd[x]) * a_i), ExDivCM(tmp));
			if (a > EXP_CM) a = EXP_CM;
			if (r > EXP_CM) r = EXP_CM;
			if (g > EXP_CM) g = EXP_CM;
			if (b > EXP_CM) b = EXP_CM;
			pd[x] = ExRGBA(r, g, b, a);
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgASM_hpp__*/