#include <windows.h>
#include <gdiplus.h>	//GDI+���g�p
#include <string>
#include <codecvt>
#include <vector>
#include<array>

#include "../DrectX12/DX12Library.h"
#include "Buffer.h"

//�t�H���g�C���[�W�̍쐬
void FontTex::LoadFontImage(ComPtr<ID3D12Device>& dev, wstring str[15])
{
	//��e�N�X�`�����쐬
	//�A�b�v���[�h�p
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT  footprint{ 0 };		//�z�u��T�u���\�[�X�̃t�b�g�v�����g
	UINT64								m_total_bytes{ 0 };
	ID3D12Resource* resource_up_load{ nullptr };//�A�b�v���[�h�p�e�N�X�`�����\�[�X
	//�A�b�v���[�h�Əo�̓e�N�X�`���쐬
	CreateOutPutTexResourceView(dev, footprint, m_total_bytes);
	CreateUpLoadResource(dev, &resource_up_load, m_total_bytes);

	void* ptr = nullptr;
	resource_up_load->Map(0, nullptr, &ptr);//���\�[�X���}�b�v

	//�A�b�v���[�h�p�e�N�X�`�����\�[�X�̃C���[�W���̐擪�A�h���X�擾
	UINT32* p_rgb = (reinterpret_cast<UINT32*>(ptr) + footprint.Offset);
	memset(p_rgb, 0x00, sizeof(UINT32) * TEX_SIZE * TEX_SIZE);


	//�P�������쐬
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < str[i].size(); j++)
		{
			//�����񂩂當�������o��
			wstring c = wstring(str[i].substr(j, CHAR_STEP));
			//���o�����������w�肵���ʒu�ɃC���[�W��
			LoadCharImage(c, p_rgb, (j % (FONT_SIZE / 2)) * FONT_SIZE, i * FONT_SIZE);
		}
	}
	CopyImageDataTexture(dev, footprint, resource_up_load);

	resource_up_load->Unmap(0, 0);//���\�[�X�̃A���}�b�v
	resource_up_load->Release();
}

//�o�^���������Ƀe�N�X�`�����쐬
void FontTex::LoadCharImage(wstring c, UINT32* p, int pos_x, int pos_y)
{
	//�����t�H���g���
	BYTE* ptr;
	DWORD size;
	GLYPHMETRICS GM;

	//�����쐬�p
	UINT code = (UINT)c[0];//�����𕶎��R�[�h�ɂ���
	//�����t�H���g���r�b�g�}�b�v���������̃T�C�Y�𑪂�
	size = GetGlyphOutline(m_hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &MAT);
	ptr = new BYTE[size];
	//�r�b�g�}�b�v�擾�i�����t�H���g�̃A�E�g���C�����r�b�g�}�b�v�ɂ��Ď擾�j
	GetGlyphOutline(m_hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &MAT);

	//�������݈ʒu
	int x = pos_x;
	int y = pos_y;

	// �t�H���g���̏�������
	int iOfs_x = GM.gmptGlyphOrigin.x;
	int iOfs_y = m_TM.tmAscent - GM.gmptGlyphOrigin.y;
	int iBmp_w = GM.gmBlackBoxX + (4 - (GM.gmBlackBoxX % 4)) % 4;
	int iBmp_h = GM.gmBlackBoxY;
	int Level = 17;

	//���\�[�X�ɕ�������������
	UINT32 Alpha, Color;
	for (unsigned int i = iOfs_y; i < iOfs_y + iBmp_h; i++)
	{
		for (unsigned int j = iOfs_x; j < iOfs_x + GM.gmBlackBoxX; j++)
		{
			Alpha = (255 * ptr[j - iOfs_x + iBmp_w * (i - iOfs_y)]) / (Level - 1);
			Color = 0x00ffffff | (Alpha << 24);
			(reinterpret_cast<UINT32*>(p))[(i + y) * TEX_SIZE + (j + x)] = Color;
		}
	}

	//�r�b�g�}�b�v���j��
	delete[] ptr;
}

//--------------------------------------------------------------------------//