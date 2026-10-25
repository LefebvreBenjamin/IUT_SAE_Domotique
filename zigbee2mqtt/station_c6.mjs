// Convertisseur Zigbee2MQTT pour la station ESP32-C6
// A copier dans : zigbee2mqtt/external_converters/station_c6.mjs (cote Home Assistant)
import * as m from 'zigbee-herdsman-converters/lib/modernExtend';
import * as exposes from 'zigbee-herdsman-converters/lib/exposes';
import * as reporting from 'zigbee-herdsman-converters/lib/reporting';

const e = exposes.presets;
const ea = exposes.access;

// Codes envoyes par l'ESP : 0 inconnue, 1 bleu, 2 blanc, 3 rouge
const COULEURS = ['inconnu', 'bleu', 'blanc', 'rouge'];

// Tempo : endpoint 14 = aujourd'hui, 15 = demain
const fzTempo = {
    cluster: 'genMultistateInput',
    type: ['attributeReport', 'readResponse'],
    convert: (model, msg, publish, options, meta) => {
        if (msg.data.presentValue === undefined) return;
        const nom = {14: 'tempo_jour', 15: 'tempo_demain'}[msg.endpoint.ID];
        if (!nom) return;
        return {[nom]: COULEURS[msg.data.presentValue] ?? 'inconnu'};
    },
};

export default {
    zigbeeModel: ['StationC6'],   // = setManufacturerAndModel("DIY", "StationC6")
    model: 'StationC6',
    vendor: 'DIY',
    description: 'Station capteurs ESP32-C6 (SAE domotique)',
    extend: [
        // 2 capteurs de temperature : on nomme leurs endpoints
        m.deviceEndpoints({
            endpoints: {ambiant: 10, cpu: 16},
            // Valeurs uniques : jamais de suffixe d'endpoint (sinon pressure_11...)
            multiEndpointSkip: ['pressure', 'illuminance', 'co'],
        }),
        // -> temperature_ambiant (BMP280) et temperature_cpu (puce ESP)
        m.temperature({endpointNames: ['ambiant', 'cpu']}),
        m.pressure(),      // endpoint 11
        m.illuminance(),   // endpoint 12
        // Gaz MQ-9, endpoint 13 (valeur libre)
        m.numeric({
            name: 'co',
            cluster: 'genAnalogInput',
            attribute: 'presentValue',
            reporting: {min: 10, max: 3600, change: 1},
            description: 'Monoxyde de carbone (MQ-9)',
            unit: 'ppm',
            access: 'STATE_GET',
        }),
    ],
    // Tempo EDF, endpoints 14 et 15
    fromZigbee: [fzTempo],
    exposes: [
        e.enum('tempo_jour', ea.STATE, COULEURS).withDescription("Couleur Tempo d'aujourd'hui"),
        e.enum('tempo_demain', ea.STATE, COULEURS).withDescription('Couleur Tempo de demain'),
    ],
    configure: async (device, coordinatorEndpoint) => {
        // Les rapports Tempo arrivent au coordinateur
        for (const id of [14, 15]) {
            await reporting.bind(device.getEndpoint(id), coordinatorEndpoint, ['genMultistateInput']);
        }
    },
};
