/**
 * @file main.cxx
 * @author NARENJI-SHESHKALANI Floran
 * @author MERINO Mathieu
 * @author MARGIER Thomas
 * @date 18/09/2014
 * @brief WMC Video Compression Codec
 *
 **/

namespace
{
	class Pixel
	{
	public:
		Pixel(int r, int g, int b) : m_r(r), m_g(g), m_b(b) { }

		int getR() { return m_r; }
		int getG() { return m_g; }
		int getB() { return m_b; }

		void setR(int r) { m_r = r; }
		void setG(int g) { m_g = g; }
		void setB(int b) { m_b = b; }

	private:
		int m_r, m_g, m_b;
	};
}

int main(int argc, char** argv)
{

}
