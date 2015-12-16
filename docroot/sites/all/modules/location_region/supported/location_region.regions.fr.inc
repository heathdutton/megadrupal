<?php

// https://en.wikipedia.org/wiki/Departments_of_France

/**
 * Get FR regions.
 */
function location_region_get_regions_fr() {
  $regions = array(
    'Alsace' => 'Alsace',
    'Aquitaine' => 'Aquitaine',
    'Auvergne' => 'Auvergne',
    'Brittany' => 'Brittany',
    'Burgundy' => 'Burgundy',
    'Centre' => 'Centre',
    'Champagne-Ardenne' => 'Champagne-Ardenne',
    'Corsica' => 'Corsica',
    'Franche-Comte' => 'Franche-Comté',
    'Ile-de-France' => 'Île-de-France',
    'Languedoc-Roussillon' => 'Languedoc-Roussillon',
    'Limousin' => 'Limousin',
    'Lorraine' => 'Lorraine',
    'Lower Normandy' => 'Lower Normandy',
    'Midi-Pyrenees' => 'Midi-Pyrénées',
    'Nord-Pas-de-Calais' => 'Nord-Pas-de-Calais',
    'Pays de la Loire' => 'Pays de la Loire',
    'Picardy' => 'Picardy',
    'Poitou-Charentes' => 'Poitou-Charentes',
    "Provence-Alpes-Cote d'Azur" => "Provence-Alpes-Côte d'Azur",
    'Rhone-Alpes' => 'Rhône-Alpes',
    'Upper Normandy' => 'Upper Normandy',
    'Guadeloupe' => 'Guadeloupe',
    'Martinique' => 'Martinique',
    'French Guiana' => 'French Guiana',
    'Reunion' => 'Réunion',
    'Mayotte' => 'Mayotte',
  );

  return $regions;
}

/**
 * Get FR Provinces by Region.
 */
function location_region_get_provinces_fr($region) {
  // Define de region/provinces array.
  // The province codes are the ones used in module /location/supported/location.fr.inc

  $region_departments = array(
    'Aquitaine' => array(
      'A24',
      'A33',
      'A40',
      'A47',
      'A64',
    ),
    'Auvergne' => array(
      'A03',
      'A15',
      'A43',
      'A63',
    ),
    'Rhone-Alpes' => array(
      'A01',
      'A07',
      'A26',
      'A38',
      'A42',
      'A69',
      'A73',
      'A74',
    ),
    'Picardy' => array(
      'A02',
      'A60',
      'A80',
    ),
    "Provence-Alpes-Cote d'Azur" => array(
      'A04',
      'A05',
      'A06',
      'A13',
      'A83',
      'A84',
    ),
    'Champagne-Ardenne' => array(
      'A08',
      'A10',
      'A51',
      'A52',
    ),
    'Midi-Pyrénées' => array(
      'A09',
      'A12',
      'A31',
      'A32',
      'A46',
      'A65',
      'A81',
      'A82',
    ),
    'Languedoc-Roussillon' => array(
      'A11',
      'A30',
      'A34',
      'A48',
      'A66',
    ),
    'Lower Normandy' => array(
      'A14',
      'A50',
      'A61',
    ),
    'Poitou-Charentes' => array(
      'A16',
      'A17',
      'A79',
      'A86',
    ),
    'Centre' => array(
      'A18',
      'A28',
      'A36',
      'A37',
      'A41',
      'A45',
    ),
    'Limousin' => array(
      'A19',
      'A23',
      'A87',
    ),
    'Corsica' => array(
      'B2A',
      'B2B',
    ),
    'Burgundy' => array(
      'A21',
      'A58',
      'A71',
      'A89',
    ),
    'Brittany' => array(
      'A22',
      'A29',
      'A35',
      'A56',
    ),
    'Nord-Pas-de-Calais' => array(
      'A59',
      'A62',
    ),
    'Franche-Comte' => array(
      'A25',
      'A39',
      'A70',
      'A90',
    ),
    'Upper Normandy' => array(
      'A27',
      'A76',
    ),
    'Pays de la Loire' => array(
      'A44',
      'A49',
      'A53',
      'A72',
      'A85',
    ),
    'Lorraine' => array(
      'A54',
      'A55',
      'A57',
      'A88',
    ),
    'Alsace' => array(
      'A67',
      'A68',
    ),
    'Ile-de-France' => array(
      'A75',
      'A77',
      'A78',
      'A91',
      'A92',
      'A93',
      'A94',
      'A95',
    ),
    'Guadeloupe' => array(
      'C71',
    ),
    'Martinique' => array(
      'C72',
    ),
    'French Guiana' => array(
      'C73',
    ),
    'Reunion' => array(
      'C74',
    ),
    'Mayotte' => array(
      'C76',
    ),
  );

  if (isset($region_departments[$region])) {
    $provinces = array();

    // Set the Province name from Location module.
    $departments = $region_departments[$region];
    $provinces_fr = location_get_provinces('fr');
    foreach ($departments as $id) {
      if (isset($provinces_fr[$id])) {
        $provinces[$id] = $provinces_fr[$id];
      }
    }

    return $provinces;
  }

  return array();
}