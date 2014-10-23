#include "stdafx.h"

using namespace nsWMC;
using namespace std;
using namespace Eigen;

CRunLengthEncoder::CRunLengthEncoder(bool decode)
	: m_decode(decode)
{
}

void CRunLengthEncoder::operator()(CSerializableComponentFrame& serializedFrame)
{
	if (!serializedFrame.size()) // frame vide car blockmatch� � 100%
		return;

	if (!m_decode)
	{
		/** serializedFrame est un "buffer" de donn�es, une zone de m�moire servant � stocker
		  * les donn�es qui ont �t� compress�es/s'appr�tent � l'�tre et ont donc "perdu" leur repr�sentation.
		  * On dimensionne le buffer de mani�re � ce qu'il puisse accueiller tous les composants
		  * 
		  **/

		//// On copie les donn�es de la frame dans le buffer serializedFrame
		//// Le cast est transforme les composantes r�elles en entiers moins couteux en stockage.
		//Map<Matrix<CSerializedComponentType, Dynamic, Dynamic>>(serializedFrame.data(), frame.rows(), frame.cols()) = frame.cast<CSerializedComponentType>();
		//
		// On d�clare un deuxi�me buffer, cette fois-ci pour les donn�es apr�s compression
		CSerializableComponentFrame rleFrame;

		for (int i = 0; i < serializedFrame.size() - 1; ++i)
		{
			rleFrame.push_back(serializedFrame[i]); // On stocke le coefficient courrant
			if (0 == serializedFrame[i]) // Si c'est un zero
			{
				int j = i + 1;
				// Alors on compte le nombre de z�ros qui le suivent.
				for (; j < serializedFrame.size() - 1 && 0 == serializedFrame[j]; ++j);

				// Et on stocke le nombre de z�ros qui le suivent
				rleFrame.push_back(j - i - 1);

				// On incr�mente l'index de parcours du tableau de mani�re � sauter les z�ros qui ont �t� compress�s.
				i = j - 1;
			}
		}
		rleFrame.push_back(serializedFrame[serializedFrame.size() - 1]); // Le dernier �l�ment est trait� s�par�ment.

		serializedFrame = rleFrame;
	}
	else
	{
		// Buffer de reconstruction
		CSerializableComponentFrame rleFrame;
		for (int i = 0; i < serializedFrame.size() - 1; ++i)
		{
			rleFrame.push_back(serializedFrame[i]); // On r�cup�re le coefficient et le stocke
			if (0 == serializedFrame[i]) // Si c'est un z�ro
			{
				int count = serializedFrame[i + 1]; // On lit l'�l�ment suivante qui indique le nb de z�ro compress�s
				for (int j = 0; j < count; ++j)
					rleFrame.push_back(0); // On rajoute autant de z�ros qu'indiqu�s.

				++i; // On saute une case puisque celle-ci contenait le nb de z�ro
			}
		}
		rleFrame.push_back(serializedFrame[serializedFrame.size() - 1]); // Le dernier �l�ment est trait� s�par�ment.
		serializedFrame = rleFrame;
		//// On recopie dans notre frame le contenu du buffer
		//frame = Matrix<int16_t, Dynamic, Dynamic>::Map(rleFrame.data(), frame.rows(), frame.cols()).cast<CComponentType>();
	}
}
