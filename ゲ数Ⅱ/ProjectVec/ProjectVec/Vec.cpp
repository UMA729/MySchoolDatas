#include <stdio.h>
#include <math.h>

//�x�N�g���N���X
class Vec
{
public:
	Vec() {};				//�������Ȃ�
	Vec(float x, float y)
	{
		mX = x;				//�O������̏��𗘗p����Ƃ�
		mY = y;
	}
	Vec(const Vec& v) :mX(v.mX), mY(v.mY) {};//���̂܂܃f�[�^�������Ƃ�

	//���Z
	Vec operator-(Vec v)
	{
		Vec w(mX - v.mX, mY - v.mY);
		return w;
	}
	
	//���Z
	Vec operator+(Vec v)
	{
		Vec w(mX + v.mX, mY + v.mY);
		return w;
	}

	void operator += (Vec v)
	{
		mX += v.mX;
		mY += v.mY;
	}
	void operator -= (Vec v)
	{
		mX -= v.mX;
		mY -= v.mY;
	}

	//�傫�������߂�
	float Size()
	{
		float size{ 0 };
		size = sqrt(mX * mX + mY * mY);

		return size;
	}

	//���K��
	void Normalize()
	{
		float size = Size();
		if (size <= 0.0f)
		{
			return;
		}

		mX /= size;
		mY /= size;

		printf("%.3f,%.3f", mX, mY);
	}

	//�\��
	void View()
	{
		printf("x=%.3f,y=%.3f\n", mX, mY);
	}

private:
	float mX{ 0.0f };
	float mY{ 0.0f };
};

void main()
{
	Vec vec(2,1);
	Vec vec_s[3]{ Vec(1,2),Vec(1,2),Vec(1,2) };
	Vec vec_s2[3]{ Vec(1,2) };
	Vec A (vec_s[1]);

	//vec = vec_s[0] - vec_s[1];
	
	vec.Normalize();

	vec += vec_s[0];

	vec.View();
}