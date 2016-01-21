<?php

function eventbrite_api_currencies() {
  $currencies = array(
    'AFN' => t('Afghanistani Afghani'),
    'ALL' => t('Albanian Lek'),
    'DZD' => t('Algerian Dinar'),
    'ARS' => t('Argentine Peso'),
    'AWG' => t('Aruba Florin'),
    'AUD' => t('Australian Dollar'),
    'AZN' => t('Azerbaijan New Maneat'),
    'BSD' => t('Bahamian Dollar'),
    'BHD' => t('Bahraini Dinar'),
    'BDT' => t('Bangladeshi Taka'),
    'BBD' => t('Barbadian Dollar'),
    'BYR' => t('Belarus Ruble'),
    'BZD' => t('Belize Dollar'),
    'BMD' => t('Bermuda Dollar'),
    'BTN' => t('Bhutanese Ngultrum'),
    'BOB' => t('Bolivian Boliviano'),
    'BAM' => t('Bosnia and Herzegovina Convertible Marka'),
    'BWP' => t('Botswana Pula'),
    'BRL' => t('Brazilian Real'),
    'GBP' => t('British Pound'),
    'BND' => t('Brunei Dollar'),
    'BGN' => t('Bulgarian Lev'),
    'BIF' => t('Burundi Franc'),
    'KHR' => t('Cambodia Riel'),
    'CAD' => t('Canadian Dollar'),
    'CVE' => t('Cape Verdean Escudo'),
    'KYD' => t('Cayman Islands Dollar'),
    'XOF' => t('CFA Franc'),
    'XAF' => t('CFA Franc'),
    'CLP' => t('Chilean Peso'),
    'CNY' => t('Chinese Yuan'),
    'COP' => t('Colombian Peso'),
    'KMF' => t('Comoros Franc'),
    'CRC' => t('Costa Rica Colon'),
    'HRK' => t('Croatian Kuna'),
    'CUP' => t('Cuban Peso'),
    'CYP' => t('Cyprus Pound'),
    'CZK' => t('Czech Koruna'),
    'DKK' => t('Danish Krone'),
    'DJF' => t('Dijiboutian Franc'),
    'DOP' => t('Dominican Peso'),
    'XCD' => t('East Caribbean Dollar'),
    'EGP' => t('Egyptian Pound'),
    'SVC' => t('El Salvador Colon'),
    'ERN' => t('Eritrean Nakfa'),
    'EEK' => t('Estonian Kroon'),
    'ETB' => t('Ethiopian Birr'),
    'EUR' => t('Euro'),
    'FKP' => t('Falkland Islands Pound'),
    'FJD' => t('Fiji Dollar'),
    'GMD' => t('Gambian Dalasi'),
    'GHC' => t('Ghanian Cedi'),
    'GIP' => t('Gibraltar Pound'),
    'XAU' => t('Gold Ounces'),
    'GTQ' => t('Guatemala Quetzal'),
    'GGP' => t('Guernsey Pound'),
    'GNF' => t('Guinea Franc'),
    'GYD' => t('Guyana Dollar'),
    'HTG' => t('Haiti Gourde'),
    'HNL' => t('Honduras Lempira'),
    'HKD' => t('Hong Kong Dollar'),
    'HUF' => t('Hungarian Forint'),
    'ISK' => t('Iceland Krona'),
    'INR' => t('Indian Rupee'),
    'IDR' => t('Indonesian Rupiah'),
    'IRR' => t('Iran Rial'),
    'IQD' => t('Iraqi Dinar'),
    'ILS' => t('Israeli Shekel'),
    'JMD' => t('Jamaican Dollar'),
    'JPY' => t('Japanese Yen'),
    'JOD' => t('Jordanian Dinar'),
    'KZT' => t('Kazakhstan Tenge'),
    'KES' => t('Kenyan Shilling'),
    'KRW' => t('Korean Won'),
    'KWD' => t('Kuwaiti Dinar'),
    'KGS' => t('Kyrgyzstan Som'),
    'LAK' => t('Lao Kip'),
    'LVL' => t('Latvian Lat'),
    'LBP' => t('Lebanese Pound'),
    'LSL' => t('Lesotho Loti'),
    'LRD' => t('Liberian Dollar'),
    'LYD' => t('Libyan Dinar'),
    'LTL' => t('Lithuanian Lita'),
    'MOP' => t('Macau Pataca'),
    'MKD' => t('Macedonian Denar'),
    'MGA' => t('Malagasy ariary'),
    'MWK' => t('Malawian Kwacha'),
    'MYR' => t('Malaysian Ringgit'),
    'MVR' => t('Maldives Rufiyaa'),
    'MTL' => t('Maltese Lira'),
    'MRO' => t('Mauritania Ougulya'),
    'MUR' => t('Mauritius Rupee'),
    'MXN' => t('Mexican Peso'),
    'MDL' => t('Moldovan Leu'),
    'MNT' => t('Mongolian Tugrik'),
    'MAD' => t('Moroccan Dirham'),
    'MZM' => t('Mozambique Metical'),
    'MMK' => t('Myanmar Kyat'),
    'NAD' => t('Namibian Dollar'),
    'NPR' => t('Nepalese Rupee'),
    'ANG' => t('Neth Antilles Guilder'),
    'NZD' => t('New Zealand Dollar'),
    'NIO' => t('Nicaragua Cordoba'),
    'NGN' => t('Nigerian Naira'),
    'KPW' => t('North Korean Won'),
    'NOK' => t('Norwegian Krone'),
    'OMR' => t('Omani Rial'),
    'XPF' => t('Pacific Franc'),
    'PKR' => t('Pakistani Rupee'),
    'XPD' => t('Palladium Ounces'),
    'PAB' => t('Panama Balboa'),
    'PGK' => t('Papua New Guinea Kina'),
    'PYG' => t('Paraguayan Guarani'),
    'PEN' => t('Peruvian Nuevo Sol'),
    'PHP' => t('Philippine Peso'),
    'XPT' => t('Platinum Ounces'),
    'PLN' => t('Polish Zloty'),
    'QAR' => t('Qatar Rial'),
    'RON' => t('Romanian New Leu'),
    'RUB' => t('Russian Rouble'),
    'RWF' => t('Rwandese Franc'),
    'WST' => t('Samoan Tala'),
    'STD' => t('Sao Tome Dobra'),
    'SAR' => t('Saudi Arabian Riyal'),
    'SCR' => t('Seychelles Rupee'),
    'RSD' => t('Serbian Dinar'),
    'SLL' => t('Sierra Leone Leone'),
    'XAG' => t('Silver Ounces'),
    'SGD' => t('Singapore Dollar'),
    'SKK' => t('Slovak Koruna'),
    'SBD' => t('Solomon Islands Dollar'),
    'SOS' => t('Somali Shilling'),
    'ZAR' => t('South African Rand'),
    'LKR' => t('Sri Lanka Rupee'),
    'SHP' => t('St Helena Pound'),
    'SDG' => t('Sudanese Pound'),
    'SRD' => t('Surinam Dollar'),
    'SZL' => t('Swaziland Lilageni'),
    'SEK' => t('Swedish Krona'),
    'CHF' => t('Swiss Franc'),
    'SYP' => t('Syrian Pound'),
    'TWD' => t('Taiwan Dollar'),
    'TZS' => t('Tanzanian Shilling'),
    'THB' => t('Thai Baht'),
    'TOP' => t("Tonga Pa'anga"),
    'TTD' => t('Trinidad & Tobago Dollar'),
    'TND' => t('Tunisian Dinar'),
    'TRY' => t('Turkish Lira'),
    'USD' => t('U.S. Dollar'),
    'AED' => t('UAE Dirham'),
    'UGX' => t('Ugandan Shilling'),
    'UAH' => t('Ukraine Hryvnia'),
    'UYU' => t('Uruguayan New Peso'),
    'UZS' => t('Uzbekistan Sum'),
    'VUV' => t('Vanuatu Vatu'),
    'VEB' => t('Venezuelan Bolivar'),
    'VND' => t('Vietnam Dong'),
    'YER' => t('Yemen Riyal'),
    'YUM' => t('Yugoslav Dinar'),
    'ZMK' => t('Zambian Kwacha'),
    'ZWD' => t('Zimbabwe Dollar'),
  );
  return $currencies;
}

function eventbrite_api_countries() {
  $countries = array(
    'AD' => 'Andorra',
    'AE' => 'United Arab Emirates',
    'AF' => 'Afghanistan',
    'AG' => 'Antigua and Barbuda',
    'AI' => 'Anguilla',
    'AL' => 'Albania',
    'AM' => 'Armenia',
    'AO' => 'Angola',
    'AQ' => 'Antarctica',
    'AR' => 'Argentina',
    'AS' => 'American Samoa',
    'AT' => 'Austria',
    'AU' => 'Australia',
    'AW' => 'Aruba',
    'AX' => 'Åland Islands',
    'AZ' => 'Azerbaijan',
    'BA' => 'Bosnia and Herzegovina',
    'BB' => 'Barbados',
    'BD' => 'Bangladesh',
    'BE' => 'Belgium',
    'BF' => 'Burkina Faso',
    'BG' => 'Bulgaria',
    'BH' => 'Bahrain',
    'BI' => 'Burundi',
    'BJ' => 'Benin',
    'BL' => 'Saint Barthélemy',
    'BM' => 'Bermuda',
    'BN' => 'Brunei Darussalam',
    'BO' => 'Bolivia',
    'BQ' => 'Bonaire, Sint Eustatius and Saba',
    'BR' => 'Brazil',
    'BS' => 'Bahamas',
    'BT' => 'Bhutan',
    'BV' => 'Bouvet Island',
    'BW' => 'Botswana',
    'BY' => 'Belarus',
    'BZ' => 'Belize',
    'CA' => 'Canada',
    'CC' => 'Cocos (Keeling) Islands',
    'CD' => 'Congo, the Democratic Republic of the',
    'CF' => 'Central African Republic',
    'CG' => 'Congo',
    'CH' => 'Switzerland',
    'CI' => "Côte d'Ivoire",
    'CK' => 'Cook Islands',
    'CL' => 'Chile',
    'CM' => 'Cameroon',
    'CN' => 'China',
    'CO' => 'Colombia',
    'CR' => 'Costa Rica',
    'CU' => 'Cuba',
    'CV' => 'Cape Verde',
    'CW' => 'Curaçao',
    'CX' => 'Christmas Island',
    'CY' => 'Cyprus',
    'CZ' => 'Czech Republic',
    'DE' => 'Germany',
    'DJ' => 'Djibouti',
    'DK' => 'Denmark',
    'DM' => 'Dominica',
    'DO' => 'Dominican Republic',
    'DZ' => 'Algeria',
    'EC' => 'Ecuador',
    'EE' => 'Estonia',
    'EG' => 'Egypt',
    'EH' => 'Western Sahara',
    'ER' => 'Eritrea',
    'ES' => 'Spain',
    'ET' => 'Ethiopia',
    'FI' => 'Finland',
    'FJ' => 'Fiji',
    'FK' => 'Falkland Islands (Malvinas)',
    'FM' => 'Micronesia, Federated States of',
    'FO' => 'Faroe Islands',
    'FR' => 'France',
    'GA' => 'Gabon',
    'GB' => 'United Kingdom',
    'GD' => 'Grenada',
    'GE' => 'Georgia',
    'GF' => 'French Guiana',
    'GG' => 'Guernsey',
    'GH' => 'Ghana',
    'GI' => 'Gibraltar',
    'GL' => 'Greenland',
    'GM' => 'Gambia',
    'GN' => 'Guinea',
    'GP' => 'Guadeloupe',
    'GQ' => 'Equatorial Guinea',
    'GR' => 'Greece',
    'GS' => 'South Georgia and the South Sandwich Islands',
    'GT' => 'Guatemala',
    'GU' => 'Guam',
    'GW' => 'Guinea-Bissau',
    'GY' => 'Guyana',
    'HK' => 'Hong Kong',
    'HM' => 'Heard Island and McDonald Islands',
    'HN' => 'Honduras',
    'HR' => 'Croatia',
    'HT' => 'Haiti',
    'HU' => 'Hungary',
    'ID' => 'Indonesia',
    'IE' => 'Ireland',
    'IL' => 'Israel',
    'IM' => 'Isle of Man',
    'IN' => 'India',
    'IO' => 'British Indian Ocean Territory',
    'IQ' => 'Iraq',
    'IR' => 'Iran, Islamic Republic of',
    'IS' => 'Iceland',
    'IT' => 'Italy',
    'JE' => 'Jersey',
    'JM' => 'Jamaica',
    'JO' => 'Jordan',
    'JP' => 'Japan',
    'KE' => 'Kenya',
    'KG' => 'Kyrgyzstan',
    'KH' => 'Cambodia',
    'KI' => 'Kiribati',
    'KM' => 'Comoros',
    'KN' => 'Saint Kitts and Nevis',
    'KP' => "Korea, Democratic People's Republic of",
    'KR' => 'Korea, Republic of',
    'KW' => 'Kuwait',
    'KY' => 'Cayman Islands',
    'KZ' => 'Kazakhstan',
    'LA' => "Lao People's Democratic Republic",
    'LB' => 'Lebanon',
    'LC' => 'Saint Lucia',
    'LI' => 'Liechtenstein',
    'LK' => 'Sri Lanka',
    'LR' => 'Liberia',
    'LS' => 'Lesotho',
    'LT' => 'Lithuania',
    'LU' => 'Luxembourg',
    'LV' => 'Latvia',
    'LY' => 'Libya',
    'MA' => 'Morocco',
    'MC' => 'Monaco',
    'MD' => 'Moldova, Republic of',
    'ME' => 'Montenegro',
    'MF' => 'Saint Martin (French part)',
    'MG' => 'Madagascar',
    'MH' => 'Marshall Islands',
    'MK' => 'Macedonia, the former Yugoslav Republic of',
    'ML' => 'Mali',
    'MM' => 'Myanmar',
    'MN' => 'Mongolia',
    'MO' => 'Macao',
    'MP' => 'Northern Mariana Islands',
    'MQ' => 'Martinique',
    'MR' => 'Mauritania',
    'MS' => 'Montserrat',
    'MT' => 'Malta',
    'MU' => 'Mauritius',
    'MV' => 'Maldives',
    'MW' => 'Malawi',
    'MX' => 'Mexico',
    'MY' => 'Malaysia',
    'MZ' => 'Mozambique',
    'NA' => 'Namibia',
    'NC' => 'New Caledonia',
    'NE' => 'Niger',
    'NF' => 'Norfolk Island',
    'NG' => 'Nigeria',
    'NI' => 'Nicaragua',
    'NL' => 'Netherlands',
    'NO' => 'Norway',
    'NP' => 'Nepal',
    'NR' => 'Nauru',
    'NU' => 'Niue',
    'NZ' => 'New Zealand',
    'OM' => 'Oman',
    'PA' => 'Panama',
    'PE' => 'Peru',
    'PF' => 'French Polynesia',
    'PG' => 'Papua New Guinea',
    'PH' => 'Philippines',
    'PK' => 'Pakistan',
    'PL' => 'Poland',
    'PM' => 'Saint Pierre and Miquelon',
    'PN' => 'Pitcairn',
    'PR' => 'Puerto Rico',
    'PS' => 'Palestine, State of',
    'PT' => 'Portugal',
    'PW' => 'Palau',
    'PY' => 'Paraguay',
    'QA' => 'Qatar',
    'RE' => 'Réunion',
    'RO' => 'Romania',
    'RS' => 'Serbia',
    'RU' => 'Russian Federation',
    'RW' => 'Rwanda',
    'SA' => 'Saudi Arabia',
    'SB' => 'Solomon Islands',
    'SC' => 'Seychelles',
    'SD' => 'Sudan',
    'SE' => 'Sweden',
    'SG' => 'Singapore',
    'SH' => 'Saint Helena, Ascension and Tristan da Cunha',
    'SI' => 'Slovenia',
    'SJ' => 'Svalbard and Jan Mayen',
    'SK' => 'Slovakia',
    'SL' => 'Sierra Leone',
    'SM' => 'San Marino',
    'SN' => 'Senegal',
    'SO' => 'Somalia',
    'SR' => 'Suriname',
    'SS' => 'South Sudan',
    'ST' => 'Sao Tome and Principe',
    'SV' => 'El Salvador',
    'SX' => 'Sint Maarten (Dutch part)',
    'SY' => 'Syrian Arab Republic',
    'SZ' => 'Swaziland',
    'TC' => 'Turks and Caicos Islands',
    'TD' => 'Chad',
    'TF' => 'French Southern Territories',
    'TG' => 'Togo',
    'TH' => 'Thailand',
    'TJ' => 'Tajikistan',
    'TK' => 'Tokelau',
    'TL' => 'Timor-Leste',
    'TM' => 'Turkmenistan',
    'TN' => 'Tunisia',
    'TO' => 'Tonga',
    'TR' => 'Turkey',
    'TT' => 'Trinidad and Tobago',
    'TV' => 'Tuvalu',
    'TW' => 'Taiwan, Province of China',
    'TZ' => 'Tanzania, United Republic of',
    'UA' => 'Ukraine',
    'UG' => 'Uganda',
    'UM' => 'United States Minor Outlying Islands',
    'US' => 'United States',
    'UY' => 'Uruguay',
    'UZ' => 'Uzbekistan',
    'VA' => 'Holy See (Vatican City State)',
    'VC' => 'Saint Vincent and the Grenadines',
    'VE' => 'Venezuela, Bolivarian Republic of',
    'VG' => 'Virgin Islands, British',
    'VI' => 'Virgin Islands, U.S.',
    'VN' => 'Viet Nam',
    'VU' => 'Vanuatu',
    'WF' => 'Wallis and Futuna',
    'WS' => 'Samoa',
    'YE' => 'Yemen',
    'YT' => 'Mayotte',
    'ZA' => 'South Africa',
    'ZM' => 'Zambia',
    'ZW' => 'Zimbabwe',
  );
  // Sort the array by the country name.
  asort($countries);
  return $countries;
}

function eventbrite_api_ctime($timestamp = NULL) {
  if (empty($timestamp)) {
    $timestamp = time();
  }
  $time = date('Y-m-d H:i:s', $timestamp);
  return $time;
}

function eventbrite_api_datef($date) {
  return $date . ':00';
}

function event_create() {
  $event = (object) array(
    'eid' => '',
    'oid' => '',
    'vid' => '',
    'is_new' => TRUE,
    'locale' => '',
    'timezone' => '',
    'category' => '',
    'capacity' => '',
    'num_attendee_rows' => '',
    'title' => '',
    'start_date' => '',
    'status' => '',
    'description' => '',
    'end_date' => '',
    'tags' => '',
    'timezone_offset' => '',
    'password' => '',
    'created' => '',
    'url' => '',
    'privacy' => '',
    'modified' => '',
    'repeats' => '',
  );
  return $event;
}

function event_save(&$event) {
  return entity_save('event', $event);
}

function event_delete($entity) {
  // Delete the entity locally.
  entity_delete('event', $entity->eid);
  // Update the event on Eventbrite.
  $auth = array(
    'app_key' => variable_get('eventbrite_api_appkey', ''),
    'user_key' => variable_get('eventbrite_api_userkey', '')
  );
  $eb_client = new Eventbrite($auth);
  $response = $eb_client->event_update(array(
    'id' => $entity->eid,
    'status' => 'deleted'
  ));
}

function organizer_create() {
  $organizer = (object) array(
    'oid' => '',
    'name' => '',
    'description' => '',
    'long_description' => '',
    'url' => '',
    'is_new' => TRUE,
  );
  return $organizer;
}

function organizer_save(&$organizer) {
  return entity_save('organizer', $organizer);
}

function organizer_delete($entity) {
  // Delete the entity locally.
  entity_delete('organizer', $entity->oid);
}

function ticket_create() {
  $ticket = (object) array(
    'tid' => '',
    'eid' => '',
    'name' => '',
    'type' => '',
    'description' => '',
    'start_date' => '',
    'end_date' => '',
    'min' => '',
    'max' => '',
    'price' => '',
    'quantity_sold' => '',
    'visible' => '',
    'currency' => '',
    'quantity_available' => '',
    'display_price' => '',
    'is_new' => TRUE,
  );
  return $ticket;
}

function ticket_save(&$ticket) {
  return entity_save('ticket', $ticket);
}

function ticket_delete($entity) {
  entity_delete('ticket', $entity->tid);
  // MAKE THIS CANCEL EVENT WITH EVENTBRITE.
}

function venue_create() {
  $venue = (object) array(
    'vid' => '',
    'oid' => '',
    'name' => '',
    'address' => '',
    'address_2' => '',
    'city' => '',
    'region' => '',
    'postal_code' => '',
    'country_code' => '',
    'is_new' => TRUE,
  );
  return $venue;
}

function venue_save(&$venue) {
  return entity_save('venue', $venue);
}

function venue_delete($entity) {
  entity_delete('venue', $entity->tid);
  // MAKE THIS CANCEL EVENT WITH EVENTBRITE.
}

function eventbrite_api_import_all() {
  if (($library = libraries_load('eventbrite')) && !empty($library['loaded'])) {
    // Authorisation details.
    $auth = array(
      'app_key' => variable_get('eventbrite_api_appkey', ''),
      'user_key' => variable_get('eventbrite_api_userkey', '')
    );
    // Connect to Eventbrite.
    $eb_client = new Eventbrite($auth);
    // Get all data by using the user_list_events method.
    $import = $eb_client->user_list_events();
    // Now get all attendee data.
    $results = array();
    // Go through the events.
    foreach ($import->events as $entity) {
      // Try to load attendees from this event by the event ID.
      try {
        $id = $entity->event->id;
        $results[$id] = $eb_client->event_list_attendees(array('id' => $id));
      } // If we can't, set an error.
      catch (Exception $e) {
        $error = $e->getMessage();
        // But make sure the error isn't just no attendees to load.
        if ($error == 'No records were found with the given parameters..' || strpos($error, 'Event deleted or cancelled.') !== FALSE) {
          drupal_set_message('Event "' . $entity->event->title . '" (ID ' . $entity->event->id . ') has no attendees to load.', 'warning§');
        }
        else {
          drupal_set_message($error, 'error');
        }
      }
    }
    // If we have attendees.
    if ($results) {
      // Add the attendees to our import data.
      $import->attendees = $results;
    }
    // Format it nicely.
    $events = eventbrite_api_format_all($import);
    // Should we delete everything before importing?
    if (variable_get('eventbrite_api_import_delete', 1) == 1) {
      // Yes.
      $result = db_truncate('eventbrite_api_events')->execute();
      $result = db_truncate('eventbrite_api_organizers')->execute();
      $result = db_truncate('eventbrite_api_tickets')->execute();
      $result = db_truncate('eventbrite_api_venues')->execute();
      $result = db_truncate('eventbrite_api_attendees')->execute();
      $result = db_truncate('eventbrite_api_attendees_answers')->execute();
      // Set a count array for our entities.
      $count = array(
        'events' => 0,
        'organizers' => 0,
        'tickets' => 0,
        'venues' => 0,
        'attendees' => 0,
      );
      // Run through the array.
      foreach ($events as $type => $data) {
        // Count how many entities we have.
        $count[$type] = count($data);
        // Run through the sub arrays.
        foreach ($data as $record) {
          // Make sure all records have the correct keys.
          $record = (object) array_merge((array) eventbrite_api_entity_prepare($type), (array) $record);
          // Write the record.
          $write = drupal_write_record('eventbrite_api_' . $type, $record);
        }
      }
      // Set a message and let the user know what's been imported.
      $placeholders = array(
        '@events' => $count['events'],
        '@organizers' => $count['organizers'],
        '@tickets' => $count['tickets'],
        '@venues' => $count['venues'],
        '@attendees' => $count['attendees'],
      );
      drupal_set_message(t('Import successful. @events events, @organizers organizers,
      @tickets tickets, @venues venues and @attendees attendees imported.', $placeholders));
    }
    else {
      // Update.
    }
  }
  else {
    drupal_set_message(t('Please install the Eventbrite API library. Check the !readme for further information.', array('!readme' => l(t('README'), 'admin/help/eventbrite_api'))), 'error');
  }
}

function eventbrite_api_format_all($data) {
  // Create an array for our events.
  $events = array();
  foreach ($data->events as $row) {
    // Set the eid.
    $eid = $row->event->id;
    // Check for an organizer.
    if (!empty($row->event->organizer)) {
      // Set the oid.
      $oid = $row->event->organizer->id;
      // Add the oid and remove the id.
      $row->event->organizer->oid = $oid;
      unset($row->event->organizer->id);
      // Assign the organizer object as an array to $events.
      $events['organizers'][$oid] = (array) $row->event->organizer;
      // Remove the organizer from the data.
      unset($row->event->organizer);
    }
    // Check for a venue.
    if (!empty($row->event->venue)) {
      // Set the vid.
      $vid = $row->event->venue->id;
      // Add the vid and remove the id.
      $row->event->venue->vid = $vid;
      unset($row->event->venue->id);
      // Assign the venue object as an array to $events.
      $events['venues'][$vid] = (array) $row->event->venue;
      // Change Lat-Long to lat_long.
      $events['venues'][$vid]['lat_long'] = $events['venues'][$vid]['Lat-Long'];
      unset($events['venues'][$vid]['Lat-Long']);
      // Remove the venue from the data.
      unset($row->event->venue);
    }
    // Check for tickets.
    if (!empty($row->event->tickets)) {
      // Run through each ticket.
      foreach ($row->event->tickets as $ticket) {
        // Set the tid.
        $tid = $ticket->ticket->id;
        // Add the tid and remove the id.
        $ticket->ticket->tid = $tid;
        unset($ticket->ticket->id);
        // Add the eid.
        $ticket->ticket->eid = $eid;
        // Remove commas from ticket prices.
        if (!empty($ticket->ticket->price)) {
          $ticket->ticket->price = str_replace(',', '', $ticket->ticket->price);
        }
        // Remove commas from display prices.
        if (!empty($ticket->ticket->display_price)) {
          $ticket->ticket->display_price = str_replace(',', '', $ticket->ticket->display_price);
        }
        // Assign the venue object as an array to $events.
        $events['tickets'][$tid] = (array) $ticket->ticket;
      }
      // Remove the tickets from the data.
      unset($row->event->tickets);
    }
    // Add the eid and remove the id.
    $row->event->eid = $eid;
    unset($row->event->id);
    // Assign the event object as an array to $events.
    $events['events'][$eid] = (array) $row->event;
    // Add vid and oid.
    if (!empty($oid)) {
      $events['events'][$eid]['oid'] = $oid;
    }
    if (!empty($vid)) {
      $events['events'][$eid]['vid'] = $vid;
    }
    // Remove the HTML from the description if the user wishes.
    if (variable_get('eventbrite_api_import_strip', 1) == 1) {
      $events['events'][$eid]['description'] = strip_tags($events['events'][$eid]['description']);
    }
  }
  // Run through the attendee data.
  $attendees = array();
  $answers = array();
  foreach ($data->attendees as $result) {
    foreach ($result->attendees as $record) {
      // Move the answers to their own array.
      if (!empty($record->attendee->answers)) {
        $answers[$record->attendee->id] = $record->attendee->answers;
      }
      unset($record->attendee->answers);
      // Add the attendees to their array.
      $attendees[$record->attendee->id] = (array) $record->attendee;
    }
  }
  // Format the attendees.
  foreach ($attendees as $attendee) {
    // Set a quick ID var.
    $id = $attendee['id'];
    // Add 'aid' to the array.
    $attendees[$id]['aid'] = $id;
    // Add 'tid' to the array.
    $attendees[$id]['tid'] = $attendees[$id]['ticket_id'];
    // Add 'orid' to the array.
    $attendees[$id]['orid'] = $attendees[$id]['order_id'];
    // Add 'eid' to the array.
    $attendees[$id]['eid'] = $attendees[$id]['event_id'];
    // Remove the id key, the ticket_id key, the order_id key and the event_id key.
    unset($attendees[$id]['id'], $attendees[$id]['ticket_id'], $attendees[$id]['order_id'], $attendees[$id]['event_id']);
    // Finally, if the array has empty values, remove them too.
    foreach ($attendee as $key => $value) {
      if (empty($attendees[$id][$key])) {
        unset($attendees[$id][$key]);
      }
    }
  }
  // Add the attendees to the data we're returning.
  $events['attendees'] = $attendees;
  // Format the questions/answers.
  $questions = array();
  foreach ($answers as $key => $answer) {
    foreach ($answer as $a) {
      $a->answer->aid = $key;
      $questions[$key] = (array) $a->answer;
      // Add qid.
      $questions[$key]['qid'] = $questions[$key]['question_id'];
      // Unset question_id.
      unset($questions[$key]['question_id']);
    }
  }
  // Add the attendees questions/answers to the data we're returning.
  $events['attendees_answers'] = $questions;
  ksort($events);
  return $events;
}

/**
 * This prepares an empty entity object for Eventbrite
 * events, organizers, tickets and venues.
 */
function eventbrite_api_entity_prepare($type) {
  $event = array(
    'eid' => '',
    'oid' => '',
    'vid' => '',
    'locale' => '',
    'timezone' => '',
    'category' => '',
    'capacity' => '',
    'num_attendee_rows' => '',
    'title' => '',
    'start_date' => eventbrite_api_ctime(),
    'status' => '',
    'description' => '',
    'end_date' => eventbrite_api_ctime(),
    'tags' => '',
    'timezone_offset' => '',
    'password' => '',
    'created' => eventbrite_api_ctime(),
    'url' => '',
    'privacy' => '',
    'modified' => eventbrite_api_ctime(),
    'repeats' => '',
    'is_new' => TRUE,
  );
  $organizer = array(
    'oid' => '',
    'name' => '',
    'description' => '',
    'long_description' => '',
    'url' => '',
    'is_new' => TRUE,
  );
  $ticket = array(
    'tid' => '',
    'eid' => '',
    'name' => '',
    'type' => '',
    'description' => '',
    'start_date' => eventbrite_api_ctime(),
    'end_date' => eventbrite_api_ctime(),
    'min' => '',
    'max' => '',
    'price' => '0.00',
    'quantity_sold' => '',
    'visible' => '',
    'currency' => '',
    'quantity_available' => '',
    'display_price' => '0.00',
    'is_new' => TRUE,
  );
  $venue = array(
    'vid' => '',
    'city' => '',
    'name' => '',
    'country' => '',
    'region' => '',
    'address' => '',
    'address_2' => '',
    'postal_code' => '',
    'country_code' => '',
    'longitude' => '',
    'latitude' => '',
    'lat_long' => '',
    'is_new' => TRUE,
  );
  $attendee = array(
    'aid' => '',
    'tid' => '',
    'orid' => '',
    'eid' => '',
    'work_postal_code' => '',
    'last_name' => '',
    'home_address' => '',
    'suffix' => '',
    'home_postal_code' => '',
    'tax' => '',
    'currency' => '',
    'prefix' => '',
    'first_name' => '',
    'order_type' => '',
    'work_phone' => '',
    'home_address_2' => '',
    'work_region' => '',
    'work_address_2' => '',
    'blog' => '',
    'email' => '',
    'work_address' => '',
    'event_date' => eventbrite_api_ctime(),
    'eventbrite_fee' => '',
    'amount_paid' => '',
    'job_title' => '',
    'website' => '',
    'home_country_code' => '',
    'home_city' => '',
    'home_phone' => '',
    'company' => '',
    'barcode' => '',
    'home_region' => '',
    'work_country' => '',
    'discount' => '0.00',
    'work_city' => '',
    'home_country' => '',
    'created' => '',
    'gender' => '',
    'notes' => '',
    'modified' => '',
    'cc_fee' => '0.00',
    'affiliate' => '',
    'work_country_code' => '',
    'birth_date' => '',
    'cell_phone' => '',
    'age' => '',
    'quantity' => '',
    'is_new' => TRUE,
  );
  $attendee_answer = array(
    'qid' => '',
    'aid' => '',
    'answer_text' => '',
    'question' => '',
    'question_type' => '',
    'is_new' => TRUE,
  );
  switch ($type) {
    case 'event':
      $entity = $event;
      break;
    case 'events':
      $entity = $event;
      break;
    case 'organizer':
      $entity = $organizer;
      break;
    case 'organizers':
      $entity = $organizer;
      break;
    case 'ticket':
      $entity = $ticket;
      break;
    case 'tickets':
      $entity = $ticket;
      break;
    case 'venue':
      $entity = $venue;
      break;
    case 'venues':
      $entity = $venue;
      break;
    case 'attendee':
      $entity = $attendee;
      break;
    case 'attendees':
      $entity = $attendee;
      break;
    case 'attendee_answer':
      $entity = $attendee;
      break;
    case 'attendees_answers':
      $entity = $attendee;
      break;
  }
  return (object) $entity;
}

/**
 * Entity reference fields store entity target IDs as 10
 * character integers but Eventbrite event IDs are longer
 * than this. We use this hook to alter the field size
 * to a "bigint" if it references an event entity.
 */
function _eventbrite_api_alter_field($data) {
  // Set some shorthand vars.
  $type = $data['type'];
  $target_type = isset($data['settings']['target_type']) ? $data['settings']['target_type'] : '';
  // Check to see if this is the correct field.
  if ($type == 'entityreference' && $target_type == 'event') {
    // Get all of the necessary variables together.
    $table = 'field_data_' . $data['field_name'];
    $field = $data['storage']['details']['sql']['FIELD_LOAD_CURRENT'][$table]['target_id'];
    $field_new = $field;
    $spec = array('type' => 'int', 'size' => 'big');
    // Update the field.
    db_change_field($table, $field, $field_new, $spec);
    // Now do the revisions table.
    $table = 'field_revision_' . $data['field_name'];
    db_change_field($table, $field, $field_new, $spec);
  }
}

/**
 * Autocomplete results for the ticket form event field.
 */
function _eventbrite_event_autocomplete($string) {
  $matches = array();
  // Select the organizers table.
  $query = db_select('eventbrite_api_events', 'e');
  // Select rows that match the string.
  $return = $query->fields('e', array('eid', 'title'))
    ->condition('e.title', '%' . db_like($string) . '%', 'LIKE')
    ->range(0, 10)
    ->execute();
  // Add matches to $matches .
  foreach ($return as $row) {
    $string = check_plain($row->title) . ' (' . $row->eid . ')';
    $matches[$string] = $string;
  }
  // Return for JS.
  drupal_json_output($matches);
}

/**
 * Autocomplete results for the event form organizer field.
 */
function _eventbrite_organizer_autocomplete($string) {
  $matches = array();
  // Select the organizers table.
  $query = db_select('eventbrite_api_organizers', 'o');
  // Select rows that match the string.
  $return = $query->fields('o', array('oid', 'name'))
    ->condition('o.name', '%' . db_like($string) . '%', 'LIKE')
    ->range(0, 10)
    ->execute();
  // Add matches to $matches .
  foreach ($return as $row) {
    $string = check_plain($row->name) . ' (' . $row->oid . ')';
    $matches[$string] = $string;
  }
  // Return for JS.
  drupal_json_output($matches);
}

/**
 * Autocomplete results for the event form venue field.
 */
function _eventbrite_venue_autocomplete($string) {
  $matches = array();
  // Select the organizers table.
  $query = db_select('eventbrite_api_venues', 'v');
  // Select rows that match the string.
  $return = $query->fields('v', array('vid', 'name'))
    ->condition('v.name', '%' . db_like($string) . '%', 'LIKE')
    ->range(0, 10)
    ->execute();
  // Add matches to $matches .
  foreach ($return as $row) {
    $string = check_plain($row->name) . ' (' . $row->vid . ')';
    $matches[$string] = $string;
  }
  // Return for JS.
  drupal_json_output($matches);
}

/**
 * Returns the text from between parenthesis ().
 */
function eventbrite_api_autocomplete_filter($string) {
  preg_match('#\((.*?)\)#', $string, $match);
  return !empty($match[1]) ? $match[1] : $string;
}

/**
 * Returns a formatted string instead of just an ID for autocomplete fields.
 */
function eventbrite_api_autocomplete_build($id, $entity_type) {
  $entity = entity_load($entity_type, array($id));
  if (empty($entity)) {
    return '';
  }
  $entity = array_shift($entity);
  switch ($entity_type) {
    case 'event':
      $text = $entity->title;
      break;
    default:
      $text = $entity->name;
      break;
  }
  $string = $text . ' (' . $id . ')';
  return $string;
}
