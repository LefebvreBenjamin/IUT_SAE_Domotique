// Convertisseur Zigbee2MQTT pour la station ESP32-C6
// A copier dans : zigbee2mqtt/external_converters/station_c6.mjs (cote Home Assistant)
import * as m from 'zigbee-herdsman-converters/lib/modernExtend';

export default {
    zigbeeModel: ['StationC6'],   // = setManufacturerAndModel("DIY", "StationC6")
    model: 'StationC6',
    vendor: 'DIY',
    description: 'Station capteurs ESP32-C6 (SAE domotique)',
    extend: [
        m.temperature(),   // endpoint 10
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
};
