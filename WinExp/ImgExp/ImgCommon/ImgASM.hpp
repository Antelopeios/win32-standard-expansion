// Copyright 2011, Ä¾Í·ÔÆ
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
// ImgASM - Í¼ÏñäÖÈ¾¼ÓËÙ
//
// Author:	Ä¾Í·ÔÆ
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-12-16
// Version:	1.0.0001.1520
//////////////////////////////////////////////////////////////////

#ifndef __ImgASM_h__
#define __ImgASM_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

#define ExASM_mov(ptr, xmm, reg) \
	__asm mov reg, [ptr] \
	__asm movups xmm, [reg]

#define ExASM_copy(reg1, reg2, xtmp) \
	__asm movups xtmp, [reg1] \
	__asm movups [reg2], xtmp

#define ExASM_punpckl(xmm, reg, xmsk) \
	__asm movups xmm, [reg] \
	__asm punpcklbw xmm, xmm \
	__asm pand xmm, xmsk

#define ExASM_punpckh(xmm, reg, xmsk) \
	__asm movups xmm, [reg] \
	__asm punpckhbw xmm, xmm \
	__asm pand xmm, xmsk

#define ExASM_punpap(xmm, xreg, xtmp) \
	__asm movups xmm, xreg \
	__asm punpcklwd xmm, xmm \
	__asm punpckhwd xmm, xmm \
	__asm punpckhwd xmm, xmm \
	__asm psrlq xmm, 63 \
	__asm movups xtmp, xreg \
	__asm punpckhwd xtmp, xtmp \
	__asm punpckhwd xtmp, xtmp \
	__asm punpckhwd xtmp, xtmp \
	__asm psllq xmm, 63 \
	__asm por xmm, xtmp

#define ExASM_divcm(xmm, xtmp, xmsk_257) \
	__asm movups xtmp, xmm \
	__asm paddw xtmp, xmsk_257 \
	__asm psrlw xtmp, 8 \
	__asm paddw xmm, xtmp \
	__asm psrlw xmm, 8 \

#define ExASM_cmsub(xmm, xreg, xmsk_cm) \
	__asm movups xmm, xmsk_cm \
	__asm psubw xmm, xreg

#define ExASM_combpx(xmm1, xmm2) \
	__asm psllq xmm1, 15 \
	__asm psrlq xmm1, 15 \
	__asm psrlq xmm2, 47 \
	__asm psllq xmm2, 47 \
	__asm por xmm1, xmm2

//////////////////////////////////////////////////////////////////

#endif/*__ImgASM_h__*/