<?php

/**
 * @file
 */

/**
 * Build a list of Countries.
 *
 * @return
 *   Array, All countries names and codes
 */
function _ec_store_region_country_list() {
  static $countries;

  if (empty($countries)) {
    $countries = array(
      'AD' => t('Andorra'),
      'AC' => t('Ascension Island'),
      'AE' => t('United Arab Emirates'),
      'AF' => t('Afghanistan'),
      'AG' => t('Antigua and Barbuda'),
      'AI' => t('Anguilla'),
      'AL' => t('Albania'),
      'AM' => t('Armenia'),
      'AN' => t('Netherlands Antilles'),
      'AO' => t('Angola'),
      'AQ' => t('Antarctica'),
      'AR' => t('Argentina'),
      'AS' => t('American Samoa'),
      'AT' => t('Austria'),
      'AU' => t('Australia'),
      'AW' => t('Aruba'),
      'AX' => t('Aland Islands'),
      'AZ' => t('Azerbaijan'),
      'BA' => t('Bosnia and Herzegovina'),
      'BB' => t('Barbados'),
      'BD' => t('Bangladesh'),
      'BE' => t('Belgium'),
      'BF' => t('Burkina Faso'),
      'BG' => t('Bulgaria'),
      'BH' => t('Bahrain'),
      'BI' => t('Burundi'),
      'BJ' => t('Benin'),
      'BM' => t('Bermuda'),
      'BN' => t('Brunei Darussalam'),
      'BO' => t('Bolivia'),
      'BR' => t('Brazil'),
      'BS' => t('Bahamas'),
      'BT' => t('Bhutan'),
      'BV' => t('Bouvet Island'),
      'BW' => t('Botswana'),
      'BY' => t('Belarus'),
      'BZ' => t('Belize'),
      'CA' => t('Canada'),
      'CC' => t('Cocos (Keeling) Islands'),
      'CD' => t('Democratic Republic of the Congo (formerly Zaire)'),
      'CF' => t('Central African Republic'),
      'CG' => t('Congo (Republic of the Congo)'),
      'CH' => t('Switzerland (Confoederatio Helvetica)'),
      'CI' => t('Cote d\'Ivoire (Ivory Coast)'),
      'CK' => t('Cook Islands'),
      'CL' => t('Chile'),
      'CM' => t('Cameroon'),
      'CN' => t('China'),
      'CO' => t('Colombia'),
      'CR' => t('Costa Rica'),
      'CS' => t('Serbia and Montenegro'),
      'CU' => t('Cuba'),
      'CV' => t('Cape Verde'),
      'CX' => t('Christmas Island'),
      'CY' => t('Cyprus'),
      'CZ' => t('Czech Republic'),
      'DE' => t('Germany (Deutschland)'),
      'DJ' => t('Djibouti'),
      'DK' => t('Denmark'),
      'DM' => t('Dominica'),
      'DO' => t('Dominican Republic'),
      'DZ' => t('Algeria'),
      'EC' => t('Ecuador'),
      'EE' => t('Estonia'),
      'EG' => t('Egypt'),
      'EH' => t('Western Sahara (formerly Spanish Sahara)'),
      'ER' => t('Eritrea'),
      'ES' => t('Spain (Espana)'),
      'ET' => t('Ethiopia'),
      'FI' => t('Finland'),
      'FJ' => t('Fiji'),
      'FK' => t('Falkland Islands'),
      'FM' => t('Federated States of Micronesia'),
      'FO' => t('Faroe Islands'),
      'FR' => t('France'),
      'GA' => t('Gabon'),
      'GB' => t('United Kingdom'),
      'GD' => t('Grenada'),
      'GE' => t('Georgia'),
      'GF' => t('French Guiana'),
      'GH' => t('Ghana'),
      'GI' => t('Gibraltar'),
      'GL' => t('Greenland'),
      'GM' => t('Gambia'),
      'GN' => t('Guinea'),
      'GP' => t('Guadeloupe'),
      'GQ' => t('Equatorial Guinea'),
      'GR' => t('Greece'),
      'GS' => t('South Georgia and the South Sandwich Islands'),
      'GT' => t('Guatemala'),
      'GU' => t('Guam'),
      'GW' => t('Guinea-Bissau'),
      'GY' => t('Guyana'),
      'HK' => t('Hong Kong'),
      'HM' => t('Heard Island and McDonald Islands'),
      'HN' => t('Honduras'),
      'HR' => t('Croatia (Hrvatska)'),
      'HT' => t('Haiti'),
      'HU' => t('Hungary'),
      'ID' => t('Indonesia'),
      'IE' => t('Ireland'),
      'IL' => t('Israel'),
      'IN' => t('India'),
      'IO' => t('British Indian Ocean Territory (including Diego Garcia)'),
      'IQ' => t('Iraq'),
      'IR' => t('Iran'),
      'IS' => t('Iceland'),
      'IT' => t('Italy'),
      'JM' => t('Jamaica'),
      'JO' => t('Jordan'),
      'JP' => t('Japan'),
      'KE' => t('Kenya'),
      'KG' => t('Kyrgyzstan'),
      'KH' => t('Cambodia'),
      'KI' => t('Kiribati'),
      'KM' => t('Comoros'),
      'KN' => t('Saint Kitts and Nevis'),
      'KP' => t('North Korea'),
      'KR' => t('South Korea'),
      'KW' => t('Kuwait'),
      'KY' => t('Cayman Islands'),
      'KZ' => t('Kazakhstan'),
      'LA' => t('Laos'),
      'LB' => t('Lebanon'),
      'LC' => t('Saint Lucia'),
      'LI' => t('Liechtenstein'),
      'LK' => t('Sri Lanka'),
      'LR' => t('Liberia'),
      'LS' => t('Lesotho'),
      'LT' => t('Lithuania'),
      'LU' => t('Luxembourg'),
      'LV' => t('Latvia'),
      'LY' => t('Libya'),
      'MA' => t('Morocco'),
      'MC' => t('Monaco'),
      'MD' => t('Moldova'),
      'MG' => t('Madagascar'),
      'MH' => t('Marshall Islands'),
      'MK' => t('Former Yugoslav Republic of Macedonia'),
      'ML' => t('Mali'),
      'MM' => t('Myanmar (Burma)'),
      'MN' => t('Mongolia'),
      'MO' => t('Macao (Macau)'),
      'MP' => t('Northern Mariana Islands'),
      'MQ' => t('Martinique'),
      'MR' => t('Mauritania'),
      'MS' => t('Montserrat'),
      'MT' => t('Malta'),
      'MU' => t('Mauritius'),
      'MV' => t('Maldives'),
      'MW' => t('Malawi'),
      'MX' => t('Mexico'),
      'MY' => t('Malaysia'),
      'MZ' => t('Mozambique'),
      'NA' => t('Namibia'),
      'NC' => t('New Caledonia'),
      'NE' => t('Niger'),
      'NF' => t('Norfolk Island'),
      'NG' => t('Nigeria'),
      'NI' => t('Nicaragua'),
      'NL' => t('Netherlands'),
      'NO' => t('Norway'),
      'NP' => t('Nepal'),
      'NR' => t('Nauru'),
      'NU' => t('Niue'),
      'NZ' => t('New Zealand'),
      'OM' => t('Oman'),
      'PA' => t('Panama'),
      'PE' => t('Peru'),
      'PF' => t('French Polynesia'),
      'PG' => t('Papua New Guinea'),
      'PH' => t('Philippines'),
      'PK' => t('Pakistan'),
      'PL' => t('Poland'),
      'PM' => t('Saint-Pierre and Miquelon'),
      'PN' => t('Pitcairn Islands'),
      'PR' => t('Puerto Rico'),
      'PS' => t('Palestinian Territories (West Bank and Gaza Strip)'),
      'PT' => t('Portugal'),
      'PW' => t('Palau'),
      'PY' => t('Paraguay'),
      'QA' => t('Qatar'),
      'RE' => t('Reunion'),
      'RO' => t('Romania'),
      'RU' => t('Russia'),
      'RW' => t('Rwanda'),
      'SA' => t('Saudi Arabia'),
      'SB' => t('Solomon Islands'),
      'SC' => t('Seychelles'),
      'SD' => t('Sudan'),
      'SE' => t('Sweden'),
      'SG' => t('Singapore'),
      'SH' => t('Saint Helena'),
      'SI' => t('Slovenia'),
      'SJ' => t('Svalbard and Jan Mayen Islands'),
      'SK' => t('Slovakia'),
      'SL' => t('Sierra Leone'),
      'SM' => t('San Marino'),
      'SN' => t('Senegal'),
      'SO' => t('Somalia'),
      'SR' => t('Suriname'),
      'ST' => t('Sao Tome and Principe'),
      'SV' => t('El Salvador'),
      'SW' => t('Sweden'),
      'SY' => t('Syria'),
      'SZ' => t('Swaziland'),
      'TC' => t('Turks and Caicos Islands'),
      'TD' => t('Chad (Tchad)'),
      'TF' => t('French Southern Territories'),
      'TG' => t('Togo'),
      'TH' => t('Thailand'),
      'TJ' => t('Tajikistan'),
      'TK' => t('Tokelau'),
      'TL' => t('Timor-Leste (East Timor)'),
      'TM' => t('Turkmenistan'),
      'TN' => t('Tunisia'),
      'TO' => t('Tonga'),
      'TR' => t('Turkey'),
      'TT' => t('Trinidad and Tobago'),
      'TV' => t('Tuvalu'),
      'TW' => t('Taiwan'),
      'TZ' => t('Tanzania'),
      'UA' => t('Ukraine'),
      'UG' => t('Uganda'),
      //'UK' => t('United Kingdom'),
      'UM' => t('United States Minor Outlying Islands'),
      'US' => t('United States'),
      'UY' => t('Uruguay'),
      'UZ' => t('Uzbekistan'),
      'VA' => t('Vatican City'),
      'VC' => t('Saint Vincent and the Grenadines'),
      'VE' => t('Venezuela'),
      'VG' => t('British Virgin Islands'),
      'VI' => t('U.S. Virgin Islands'),
      'VN' => t('Vietnam'),
      'VU' => t('Vanuatu'),
      'Wf' => t('Wallis and Futuna'),
      'WS' => t('Samoa'),
      'YE' => t('Yemen'),
      'YT' => t('Mayotte'),
      'ZA' => t('South Africa'),
      'ZM' => t('Zambia'),
      'ZW' => t('Zimbabwe'),
    );
    natcasesort($countries);
  }

  return $countries;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_region_build_include_list() {
  global $profile;
  $list = & drupal_static(__FUNCTION__);

  if (empty($list)) {
    $config = conf_path();
    if (!isset($profile)) {
      $profile = variable_get('install_profile', 'default');
    }
    $searchdir = array(drupal_get_path('module', 'ec_store') . '/regions');
    $files = array();

    if (file_exists("profiles/$profile/library/ec/region")) {
      $searchdir[] = "profiles/$profile/library/ec/region";
    }

    // Always search sites/all/* as well as the global directories
    if (file_exists('sites/all/library/ec/region')) {
      $searchdir[] = 'sites/all/library/ec/region';
    }

    if (file_exists("$config/library/ec/region")) {
      $searchdir[] = "$config/library/ec/region";
    }

    // Get current list of items
    foreach ($searchdir as $dir) {
      $files = array_merge($files, file_scan_directory($dir, '/^[a-z]{2,2}\.inc$/', array('key' => 'name')));
    }
    $list = array_map('_ec_store_region_map_filename', $files);
  }

  return $list;
}

function _ec_store_region_map_filename($a) {
  return $a->uri;
}
