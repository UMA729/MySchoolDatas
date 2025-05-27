#include <stdio.h>
#include <math.h>

//ベクトルクラス
class Vec
{
public:
	Vec() {};				//何もしない
	Vec(float x, float y)
	{
		m_x = x;				//外部からの情報を利用するとき
		m_y = y;
	}
	Vec(const Vec& v) :m_x(v.m_x), m_y(v.m_y) {};//そのままデータを扱うとき

	//減算
	Vec operator-(Vec v)
	{
		Vec w(m_x - v.m_x, m_y - v.m_y);
		return w;
	}
	
	//加算
	Vec operator+(Vec v)
	{
		Vec w(m_x + v.m_x, m_y + v.m_y);
		return w;
	}

	void operator += (Vec v)
	{
		m_x += v.m_x;
		m_y += v.m_y;
	}
	void operator -= (Vec v)
	{
		m_x -= v.m_x;
		m_y -= v.m_y;
	}

	//大きさを求める
	float Size()
	{
		float size{ 0 };
		size = sqrt(m_x * m_x + m_y * m_y);

		return size;
	}

	//正規化
	void Normalize()
	{
		float size = Size();
		if (size <= 0.0f)
		{
			return;
		}

		m_x /= size;
		m_y /= size;

		printf("%.3f,%.3f", m_x, m_y);
	}

	//内積
	float Dot(Vec vec)
	{
		return((m_x * vec.m_x) + (m_y * vec.m_y) );
	}

	//外積2D
	float Cross2D(Vec* vec)
	{
		return m_x * vec->m_y - m_y * vec->m_x;
	}



	//表示
	void View()
	{

		
		printf("x=%.3f,y=%.3f\n", m_x, m_y);
	}

private:
	float m_x{ 0.0f };
	float m_y{ 0.0f };
	float m_e{ 0.0f };
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