#include "cartesRfid.h"
#include <Preferences.h>

namespace { // fonctione qui rend privé les var ou fction 
Preferences prefs;
CarteRfid cartes[MAX_CARTES];
uint8_t nbCartes = 0;

ModeRfid mode = RFID_MODE_NORMAL;
uint32_t debutModeMs = 0;
uint32_t versionUi = 0;
char message[RFID_MESSAGE_SIZE] = "Pret";

void setMessage(const char *texte)
{
	strncpy(message, texte, sizeof(message) - 1);
	message[sizeof(message) - 1] = '\0';
	versionUi++;
}

void setMode(ModeRfid nouveauMode)
{
	mode = nouveauMode;
	debutModeMs = millis();
	versionUi++;
}

int chercherCarte(const uint8_t *uid, uint8_t taille)
{
	for (int i = 0; i < nbCartes; i++) // parcours les carte enregistrée
	{
		if (cartes[i].taille == taille && memcmp(cartes[i].uid, uid, taille) == 0) // on look si la taille de la carte recherche est la mêem que celles enregistré mais aussi si les uid correspondent
			return i; //renvoi l index de la carte recherché
	}
	return -1;
}

void sauvegarderCartes()
{
	prefs.begin("rfid", false);
	prefs.putUChar("nb", nbCartes);
	prefs.putBytes("cartes", cartes, sizeof(cartes));
	prefs.end();
}

void afficherResultat(ResultatCarte resultat, const char *messageOk) //mesg préenregistré
{
	switch (resultat)
	{
	case CARTE_OK:
		setMessage(messageOk);
		break;
	case CARTE_DEJA_PRESENTE:
		setMessage("Carte deja enregistree");
		break;
	case CARTE_LISTE_PLEINE:
		setMessage("Liste pleine (10 max)");
		break;
	case CARTE_INCONNUE:
		setMessage("Carte inconnue");
		break;
	case CARTE_DERNIERE:
		setMessage("Impossible : derniere carte");
		break;
	case CARTE_UID_INVALIDE:
		setMessage("UID invalide");
		break;
	}
}
}

void initCartesRfid()
{
	memset(cartes, 0, sizeof(cartes));
	nbCartes = 0;

	prefs.begin("rfid", false);
	uint8_t nb = prefs.getUChar("nb", 0);
	size_t lu = prefs.getBytes("cartes", cartes, sizeof(cartes));
	prefs.end();

	bool valide = (nb <= MAX_CARTES) && (lu == sizeof(cartes)); // docn si on ne depasse pas le nombre de carte ni 
	for (int i = 0; valide && i < nb; i++)
	{
		if (cartes[i].taille == 0 || cartes[i].taille > UID_MAX_SIZE)
			valide = false;
	}

	if (valide)
	{
		nbCartes = nb;
	}
	else
	{
		memset(cartes, 0, sizeof(cartes));
	}

	Serial.print("Cartes RFID enregistrees : ");
	Serial.println(nbCartes);
	if (nbCartes == 0)
	{
		Serial.println("Aucune carte : la premiere carte presentee sera enregistree");
		setMessage("Presentez la 1ere carte");
	}
}

void updateCartesRfid()
{
	if (mode != RFID_MODE_NORMAL && (millis() - debutModeMs) > RFID_MODE_TIMEOUT_MS) // si no est dans un mode différes de celuio de base on a 15s pour effectuer une action sinon le mode revie par defaut 
	{
		setMode(RFID_MODE_NORMAL);
		setMessage("Delai depasse : annule");
	}
}

bool carteAutorisee(const uint8_t *uid, uint8_t taille)
{
	return chercherCarte(uid, taille) >= 0;
}

ResultatCarte ajouterCarte(const uint8_t *uid, uint8_t taille)
{
	if (taille == 0 || taille > UID_MAX_SIZE) // si format invalide
		return CARTE_UID_INVALIDE; 
	if (chercherCarte(uid, taille) >= 0) // si carte deja enregistré 
		return CARTE_DEJA_PRESENTE;
	if (nbCartes >= MAX_CARTES)	//si trop de carte
		return CARTE_LISTE_PLEINE;

	memset(&cartes[nbCartes], 0, sizeof(CarteRfid));  // vide la case avant de l'utiliser
	cartes[nbCartes].taille = taille; // stocke la taille de l'uid
	memcpy(cartes[nbCartes].uid, uid, taille); // copie l'uid dans la carte
	nbCartes++; // ajout d'une carte 

	sauvegarderCartes();
	versionUi++;
	return CARTE_OK;
}

ResultatCarte supprimerCarte(const uint8_t *uid, uint8_t taille)
{
	int index = chercherCarte(uid, taille);
	if (index < 0)
		return CARTE_INCONNUE;

	// au moins 1 carte
	if (nbCartes <= 1)
		return CARTE_DERNIERE;

	for (int i = index; i < nbCartes - 1; i++)
	{
		cartes[i] = cartes[i + 1];
	}
	nbCartes--;
	memset(&cartes[nbCartes], 0, sizeof(CarteRfid));

	sauvegarderCartes();
	versionUi++;
	return CARTE_OK;
}

int getNombreCartes()
{
	return nbCartes;
}

bool getCarte(int index, CarteRfid &carte)
{
	if (index < 0 || index >= nbCartes)
		return false;
	carte = cartes[index];
	return true;
}

void formatUid(const CarteRfid &carte, char *buffer, size_t taille) //{0xDE,0xAD,0xBE,0xEF} -> "DEADBEEF"
{
	if (taille == 0)
		return;

	buffer[0] = '\0'; // On commence avec un texte vide
	size_t pos = 0;
	for (uint8_t i = 0; i < carte.taille && (pos + 2) < taille; i++)
	{
		pos += snprintf(buffer + pos, taille - pos, "%02X", carte.uid[i]);
	}
}

void demarrerAjoutCarte()
{
	setMode(RFID_MODE_AJOUT_VALIDATION);
	setMessage("Presentez une carte autorisee");
}

void demarrerSuppressionCarte()
{
	setMode(RFID_MODE_SUPPR_VALIDATION);
	setMessage("Presentez une carte autorisee");
}

void annulerModeRfid()
{
	if (mode == RFID_MODE_NORMAL)
		return;
	setMode(RFID_MODE_NORMAL);
	setMessage("Annule");
}

ModeRfid getModeRfid()
{
	return mode;
}

const char *getMessageRfid()
{
	return message;
}

uint32_t getVersionCartesRfid()
{
	return versionUi;
}

bool traiterCarteRfid(const uint8_t *uid, uint8_t taille)
{
	if (taille == 0 || taille > UID_MAX_SIZE)
		return false;

	// Premier demarrage dounc aucune carte
	if (nbCartes == 0)
	{
		afficherResultat(ajouterCarte(uid, taille), "1ere carte enregistree");
		return false;
	}

	const bool autorisee = carteAutorisee(uid, taille);

	switch (mode) //machine etat
	{
	case RFID_MODE_NORMAL:
		if (autorisee)
		{
			setMessage("Acces autorise");
			return true;
		}
		setMessage("Carte refusee");
		return false;

	case RFID_MODE_AJOUT_VALIDATION:
		if (autorisee)
		{
			setMode(RFID_MODE_AJOUT);
			setMessage("Presentez la nouvelle carte");
		}
		else
		{
			setMessage("Carte non autorisee");
		}
		return false;

	case RFID_MODE_AJOUT:
		afficherResultat(ajouterCarte(uid, taille), "Carte ajoutee");
		setMode(RFID_MODE_NORMAL);
		return false;

	case RFID_MODE_SUPPR_VALIDATION:
		if (autorisee)
		{
			setMode(RFID_MODE_SUPPR);
			setMessage("Presentez la carte a supprimer");
		}
		else
		{
			setMessage("Carte non autorisee");
		}
		return false;

	case RFID_MODE_SUPPR:
		afficherResultat(supprimerCarte(uid, taille), "Carte supprimee");
		setMode(RFID_MODE_NORMAL);
		return false;
	}

	return false;
}
