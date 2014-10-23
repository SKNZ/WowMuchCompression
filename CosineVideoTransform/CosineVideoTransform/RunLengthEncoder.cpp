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
	if (!serializedFrame.size()) // frame vide car blockmatché à 100%
		return;

	if (!m_decode)
	{
		/** serializedFrame est un "buffer" de données, une zone de mémoire servant à stocker
		  * les données qui ont été compressées/s'apprêtent à l'être et ont donc "perdu" leur représentation.
		  * On dimensionne le buffer de manière à ce qu'il puisse accueiller tous les composants
		  * 
		  **/

		//// On copie les données de la frame dans le buffer serializedFrame
		//// Le cast est transforme les composantes réelles en entiers moins couteux en stockage.
		//Map<Matrix<CSerializedComponentType, Dynamic, Dynamic>>(serializedFrame.data(), frame.rows(), frame.cols()) = frame.cast<CSerializedComponentType>();
		//
		// On déclare un deuxième buffer, cette fois-ci pour les données après compression
		CSerializableComponentFrame rleFrame;

		for (int i = 0; i < serializedFrame.size() - 1; ++i)
		{
			rleFrame.push_back(serializedFrame[i]); // On stocke le coefficient courrant
			if (0 == serializedFrame[i]) // Si c'est un zero
			{
				int j = i + 1;
				// Alors on compte le nombre de zéros qui le suivent.
				for (; j < serializedFrame.size() - 1 && 0 == serializedFrame[j]; ++j);

				// Et on stocke le nombre de zéros qui le suivent
				rleFrame.push_back(j - i - 1);

				// On incrémente l'index de parcours du tableau de manière à sauter les zéros qui ont été compressés.
				i = j - 1;
			}
		}
		rleFrame.push_back(serializedFrame[serializedFrame.size() - 1]); // Le dernier élément est traité séparément.

		serializedFrame = rleFrame;
	}
	else
	{
		// Buffer de reconstruction
		CSerializableComponentFrame rleFrame;
		for (int i = 0; i < serializedFrame.size() - 1; ++i)
		{
			rleFrame.push_back(serializedFrame[i]); // On récupère le coefficient et le stocke
			if (0 == serializedFrame[i]) // Si c'est un zéro
			{
				int count = serializedFrame[i + 1]; // On lit l'élément suivante qui indique le nb de zéro compressés
				for (int j = 0; j < count; ++j)
					rleFrame.push_back(0); // On rajoute autant de zéros qu'indiqués.

				++i; // On saute une case puisque celle-ci contenait le nb de zéro
			}
		}
		rleFrame.push_back(serializedFrame[serializedFrame.size() - 1]); // Le dernier élément est traité séparément.
		serializedFrame = rleFrame;
		//// On recopie dans notre frame le contenu du buffer
		//frame = Matrix<int16_t, Dynamic, Dynamic>::Map(rleFrame.data(), frame.rows(), frame.cols()).cast<CComponentType>();
	}
}
