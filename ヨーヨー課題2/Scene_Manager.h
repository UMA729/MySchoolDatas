[
	//シーン管理
#pragma once
#include "Scene_Base.h"

//シーン管理クラス
class CManager {
public:
	//コンストラクタ
	CManager() {};

	//シーンポインタ
	CScene* scene{ nullptr };

	//シーン更新
	int Update() { scene->Update(); return 0; };
	//シーン描画
	void Draw() { scene->Draw(); };

	//シーンの削除
	void Scene_Delete() { scene->~CScene(); }

	~CManager() { delete scene; };
};

/*著作権について

 libjpeg　Copyright (C) 1991-2013, Thomas G. Lane, Guido Vollbeding.

 libpng　Copyright (C) 2004, 2006-2012 Glenn Randers-Pehrson.
 zlib　Copyright (C) 1995-2012 Jean-loup Gailly and Mark Adler.

 libtiff　Copyright (c) 1988-1997 Sam Leffler
 libtiff　Copyright (c) 1991-1997 Silicon Graphics, Inc.

 Copyright (C) 2002-2009 Xiph.org Foundation
 Redistribution and use in source and binary forms, with or without
	　　　modification, are permitted provided that the following conditions
	　　　are met:
	   - Redistributions of source code must retain the above copyright
	　　　notice, this list of conditions and the following disclaimer.
	   - Redistributions in binary form must reproduce the above copyright
	　　　notice, this list of conditions and the following disclaimer in the
	　　　documentation and/or other materials provided with the distribution.

	　　　- Neither the name of the Xiph.org Foundation nor the names of its
	　　　contributors may be used to endorse or promote products derived from
	　　　this software without specific prior written permission.

	　　　THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	　　　``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	　　　LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	　　　A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE FOUNDATION
	　　　OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	　　　SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	　　　LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	　　　DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	　　　THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	　　　(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	　　　OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

		　　　Mersenne Twister
	　　　Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
	　　　All rights reserved.

	　　　Redistribution and use in source and binary forms, with or without
	　　　modification, are permitted provided that the following conditions
	　　　are met:

	　　　1. Redistributions of source code must retain the above copyright
	　　　notice, this list of conditions and the following disclaimer.

	　　　2. Redistributions in binary form must reproduce the above copyright
	　　　notice, this list of conditions and the following disclaimer in the
	　　　documentation and/or other materials provided with the distribution.

	　　　3. The name of the author may not be used to endorse or promote products
	　　　derived from this software without specific prior written permission.

	　　　THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	　　　IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	　　　OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	　　　IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
	　　　INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	　　　NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	　　　DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	　　　THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	　　　(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	　　　THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Bullet　Copyright (c) 2003-2006 Erwin Coumans.
*/