<?php
/**
 * @file
 * A sample for countries config file.
 */

/**
 * Gets the cities for a certain country.
 *
 * @param string $country
 *   The country machine name.
 *
 * @return array
 *   The array with cities if there are any for the required country, empty
 *   array if not.
 */
function forms_ajax_test_get_cities($country) {
  $cities = array(
    'uk' => array(
      'london' => t('London'),
      'manchester' => t('Manchester'),
      'liverpool' => t('Liverpool'),
      'belfast' => t('Belfast'),
      'glasgow' => t('Glasgow'),
      'edinburgh' => t('Edinburgh'),
      'cardiff' => t('Cardiff'),
    ),
    'france' => array(
      'paris' => t('Paris'),
      'lyon' => t('Lyon'),
      'bordeaux' => t('Bordeaux'),
      'nice' => t('Nice'),
      'montpellier' => t('Montpellier'),
    ),
    'germany' => array(
      'berlin' => t('Berlin'),
      'munchen' => t('Munchen'),
      'stuttgart' => t('Stuttgart'),
      'dortmund' => t('Dortmund'),
      'hockenheim' => t('Hockenheim'),
    ),
    'austria' => array(
      'vienna' => t('Viena'),
      'innsbruck' => t('Innsbruck'),
      'graz' => t('Graz'),
    ),
    'finland' => array(
      'helsinki' => t('Helsinki'),
      'lahti' => t('Lahti'),
      'tampere' => t('Tampere'),
    ),
    'romania' => array(
      'bucharest' => t('Bucharest'),
      'galati' => t('Galati'),
      'iasi' => t('Iasi'),
      'constanta' => t('Constanta'),
      'timisoara' => t('Timisoara'),
    ),
    'czech' => array(
      'prague' => t('Prague'),
      'split' => t('Split'),
      'karlovy' => t('Karlovy')
    ),
    'chile' => array(
      'santiago' => t('Santiago'),
      'temuco' => t('Temuco'),
      'talca' => t('Talca'),
      'arica' => t('Arica'),
    ),
    'peru' => array(
      'lima' => t('Lima'),
      'chachapoyas' => t('Chachapoyas'),
      'puerto_maldonado' => t('Puerto Maldonado'),
    ),
    'bolivia' => array(
      'lapaz' => t('La Paz'),
      'santa_cruz_de_la_sierra' => t('Santa Cruz de la Sierra'),
    ),
    'argentina' => array(
      'buenos_aires' => t('Buenos Aires'),
      'corrientes' => t('Corrientes'),
      'resistencia' => t('Resistencia'),
    ),
    'brasil' => array(
      'rio' => t('Rio de Janeiro'),
      'brasilia' => t('Brasilia'),
      'sao_paolo' => t('Sao Paolo')
    ),
    'venezuela' => array(
      'maturín' => t('Maturín'),
      'valencia' => t('Valencia'),
      'san_felipe' => t('San Felipe'),
    ),
    'ecuador' => array(
      'tulcan' => t('Tulcan'),
      'macas' => t('Macas'),
      'zamora' => t('Zamora'),
      'tena' => t('Tena')
    ),
    'usa' => array(
      'washington' => t('Washington, D.C.'),
      'new_york' => t('New York'),
      'los_angeles' => t('Los Angeles'),
      'chicago' => t('Chicago'),
      'san_francisco' => t('San Francisco'),
    ),
    'mexico' => array(
      'monterrey' => t('Monterrey'),
      'tijuana' => t('Tijuana'),
      'la_paz' => t('La Paz'),
    ),
    'canada' => array(
      'toronto' => t('Toronto'),
      'quebec' => t('Quebec City'),
      'montreal' => t('Montreal'),
    ),
    'cuba' => array(
      'havana' => t('Havana'),
      'santiago_de_cuba' => t('Santiago de Cuba'),
    ),
    'panama' => array(
      'bocas_del_toro' => t('Bocas del Toro'),
      'santiago_de_veraguas' => t('Santiago de Veraguas'),
    ),
    'egipt' => array(
      'alexandria' => t('Alexandria'),
      'cairo' => t('Cairo'),
      'suez' => t('Suez'),
    ),
    'morroco' => array(
      'rabat' => t('Rabat'),
      'casablanca' => t('Casablanca'),
      'marrakesh' => t('Marrakesh'),
    ),
    'niger' => array(
      'niamey' => t('Niamey'),
      'Tillaberi' => t('Tillabéri'),
      'diffa' => t('Diffa'),
    ),
    'nigeria' => array(
      'abuja' => t('Abuja'),
      'lagos' => t('Lagos'),
      'kano' => t('Kano'),
    ),
    'south_africa' => array(
      'pretoria' =>  t('Pretoria'),
      'cape_town' => t('Cape Town'),
      'johannesburg' => t('Johannesburg'),
    ),
    'china' => array(
      'beijing' => t('Beijing'),
      'shanghai' => t('Shanghai'),
      'tianjin' => t('Tianjin')
    ),
    'japan' => array(
      'tokyo' => t('Tokyo'),
      'osaka' => t('Osaka'),
      'fukushima' => t('Fukushima'),
      'sapporo' => t('Sapporo'),
    ),
    'mongolia' => array(
      'ulaanbaatar' => t('Ulaanbaatar'),
      'erdenet' => t('Erdenet'),
      'darkhan' => t('Darkhan'),
      'altai' => t('Altai')
    ),
    'india' => array(
      'new_delhi' => t('New Delhi'),
      'mumbai' => t('Mumbai'),
      'jaipur' => t('Jaipur')
    ),
    'south_korea' => array(
      'seoul' => t('Seoul'),
      'busan' => t('Busan'),
      'daegu' => t('Daegu'),
    ),
    'australia' => array(
      'sidney' => t('Sidney'),
      'canberra' => t('Canberra'),
      'perth' => t('Perth'),
      'melbourne' => t('Melbourne')
    ),
  );

  return isset($cities[$country]) ? $cities[$country] : array();
}