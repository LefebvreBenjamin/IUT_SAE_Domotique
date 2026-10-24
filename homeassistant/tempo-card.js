// Carte Tempo EDF pour Home Assistant
// Barre 6h -> 22h -> 6h avec un curseur sur l'heure actuelle
//
// 1. Copier ce fichier dans /config/www/tempo-card.js
// 2. Parametres > Tableaux de bord > menu (3 points) > Ressources > Ajouter :
//    URL : /local/tempo-card.js   Type : Module JavaScript
// 3. Dans le dashboard : Ajouter une carte > Manuel, puis :
//      type: custom:tempo-card
//      jour: sensor.station_c6_tempo_jour
//      demain: sensor.station_c6_tempo_demain

const COULEURS = {
  bleu:    { nom: 'Bleu',     hp: '#2f6fdb', hc: '#9dbff2' },
  blanc:   { nom: 'Blanc',    hp: '#f2f2f2', hc: '#c9c9c9' },
  rouge:   { nom: 'Rouge',    hp: '#e0453a', hc: '#f2a29b' },
  inconnu: { nom: 'Inconnue', hp: '#8a8a8a', hc: '#c4c4c4' },
};

const FIN_HP = 16 * 60;   // 22h = 16 h apres 6h (en minutes)

class TempoCard extends HTMLElement {
  setConfig(config) {
    if (!config.jour || !config.demain) {
      throw new Error("Il faut 'jour' et 'demain' (les entites Tempo)");
    }
    this._config = config;
    this._cle = '';
  }

  set hass(hass) {
    this._hass = hass;
    this._dessiner();
  }

  connectedCallback() {
    // Le curseur avance meme si rien ne change dans HA
    this._timer = setInterval(() => this._dessiner(), 30000);
    this._cle = '';
    this._dessiner();
  }

  disconnectedCallback() {
    clearInterval(this._timer);
  }

  getCardSize() {
    return 3;
  }

  static getStubConfig() {
    return { jour: 'sensor.station_c6_tempo_jour', demain: 'sensor.station_c6_tempo_demain' };
  }

  // Etat de l'entite -> cle de COULEURS
  _couleur(entite) {
    const etat = this._hass?.states[entite]?.state;
    return COULEURS[etat] ? etat : 'inconnu';
  }

  _dessiner() {
    if (!this._hass || !this._config) return;

    const jour = this._couleur(this._config.jour);
    const demain = this._couleur(this._config.demain);
    const maintenant = new Date();
    const h = maintenant.getHours();
    const mn = maintenant.getMinutes();

    // Rien n'a change : on ne redessine pas
    const cle = `${jour}|${demain}|${h}:${mn}`;
    if (cle === this._cle) return;
    this._cle = cle;

    const m = (h * 60 + mn - 360 + 1440) % 1440;   // minutes depuis 6h
    const pos = (m / 1440) * 100;                   // position en %
    const hp = m < FIN_HP;
    const reste = hp ? FIN_HP - m : 1440 - m;       // minutes avant le changement
    const resteTxt = reste < 60
      ? `${reste} min`
      : `${Math.floor(reste / 60)} h ${String(reste % 60).padStart(2, '0')}`;
    const heureTxt = `${h}:${String(mn).padStart(2, '0')}`;
    const cJ = COULEURS[jour];
    const cD = COULEURS[demain];

    // L'heure sous le curseur reste dans la carte
    let decal = '-50%';
    if (pos < 6) decal = '0';
    if (pos > 94) decal = '-100%';

    if (!this.shadowRoot) this.attachShadow({ mode: 'open' });
    this.shadowRoot.innerHTML = `
      <style>
        .carte { padding: 16px; }
        .haut { display: flex; justify-content: space-between; align-items: baseline; gap: 8px; flex-wrap: wrap; }
        .titre { font-size: 16px; font-weight: 500; color: var(--primary-text-color); }
        .periode { font-size: 13px; color: var(--secondary-text-color); }
        .pastilles { display: flex; gap: 20px; flex-wrap: wrap; margin: 10px 0 16px; font-size: 14px; color: var(--secondary-text-color); }
        .pastilles b { color: var(--primary-text-color); font-weight: 500; margin-left: 4px; }
        .point { display: inline-block; width: 12px; height: 12px; border-radius: 50%; margin-right: 6px; vertical-align: -1px; border: 1px solid var(--divider-color); }
        .zone { position: relative; height: 64px; margin: 0 4px; }
        .etiq { position: absolute; top: 0; transform: translateX(-50%); font-size: 12px; color: var(--secondary-text-color); }
        .barre { position: absolute; top: 22px; left: 0; right: 0; height: 16px; display: flex; border-radius: 4px; overflow: hidden; border: 1px solid var(--divider-color); box-sizing: border-box; }
        .passe { position: absolute; left: 0; top: 0; bottom: 0; background: rgba(0, 0, 0, 0.28); }
        .trait { position: absolute; top: 17px; width: 2px; height: 26px; margin-left: -1px; background: var(--primary-text-color); }
        .curseur { position: absolute; top: 14px; width: 3px; height: 32px; margin-left: -1.5px; border-radius: 2px; background: var(--primary-text-color); box-shadow: 0 0 0 1px var(--card-background-color, #fff); }
        .heure { position: absolute; top: 46px; font-size: 12px; font-weight: 500; color: var(--primary-text-color); }
      </style>
      <ha-card>
        <div class="carte">
          <div class="haut">
            <span class="titre">${this._config.titre ?? 'Tempo EDF'}</span>
            <span class="periode">${hp ? 'Heures pleines' : 'Heures creuses'} · ${hp ? 'creuses' : 'pleines'} dans ${resteTxt}</span>
          </div>
          <div class="pastilles">
            <span><span class="point" style="background:${cJ.hp}"></span>Aujourd'hui<b>${cJ.nom}</b></span>
            <span><span class="point" style="background:${cD.hp}"></span>Demain<b>${cD.nom}</b></span>
          </div>
          <div class="zone">
            <span class="etiq" style="left:0; transform:none">6h</span>
            <span class="etiq" style="left:66.667%">22h</span>
            <span class="etiq" style="left:100%; transform:translateX(-100%)">6h</span>
            <div class="barre">
              <div style="width:66.667%; background:${cJ.hp}"></div>
              <div style="width:33.333%; background:${cJ.hc}"></div>
              <div class="passe" style="width:${pos}%"></div>
            </div>
            <span class="trait" style="left:0"></span>
            <span class="trait" style="left:66.667%"></span>
            <span class="trait" style="left:100%"></span>
            <span class="curseur" style="left:${pos}%"></span>
            <span class="heure" style="left:${pos}%; transform:translateX(${decal})">${heureTxt}</span>
          </div>
        </div>
      </ha-card>`;
  }
}

customElements.define('tempo-card', TempoCard);

// Pour que la carte apparaisse dans la liste "Ajouter une carte"
window.customCards = window.customCards || [];
window.customCards.push({
  type: 'tempo-card',
  name: 'Tempo EDF',
  description: 'Couleur Tempo du jour et de demain, heures pleines et creuses',
});
