<?php

/**
 * @file
 * The default format for Cambodia adresses.
 */
$plugin = array(
  'title' => t('Address form (specific for Cambodia)'),
  'format callback' => 'addressfield_kh_format_address_generate',
  'type' => 'address',
  'weight' => -100,
);

function addressfield_kh_format_address_process($element) {
  $element['#limit_validation_errors'] = array($element['#parents']);

  return $element;
}

/**
 * Format callback for Cambodia addresses.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function addressfield_kh_format_address_generate(&$format, $address, $context = array()) {
  if ($address['country'] == 'KH') {
    unset($format['locality_block']['administrative_area'], $format['locality_block']['postal_code']);

    $format['locality_block']['locality'] = array(
      '#title' => t('Province'),
      '#options' => drupal_map_assoc(_addressfield_kh_province_list()),
      '#required' => TRUE,
      '#attributes' => array('class' => array('locality')),
      '#weight' => 1,
    );
    if (!empty($address['locality'])) {
      //allow empty for dependent locality
      $op = drupal_map_assoc(array('') + _addressfield_kh_district_list($address['locality']));
      $format['locality_block']['dependent_locality'] = array(
        '#title' => t('District'),
        '#options' => $op,
        '#required' => TRUE,
        '#attributes' => array('class' => array('dependent', 'district')),
        '#weight' => 2,
      );
      $format['locality_block']['postal_code'] = array(
        '#title' => t('Postal Code'),
        '#size' => 10,
        '#required' => TRUE,
        '#attributes' => array('class' => array('postal-code')),
        '#weight' => 3,
      );
    }
    if ($context['mode'] == 'render') {
      $format['locality_block']['dependent_locality']['#weight'] = 1;
      $format['locality_block']['locality']['#weight'] = 2;
      $format['locality_block']['postal_code']['#weight'] = 3;
      $format['locality_block']['postal_code']['#prefix'] = ' ';
    }
    // Check ajax form in Viet Nam
    if ($context['mode'] == 'form' && $address['country'] == 'KH') {
      $format['locality_block']['locality']['#ajax'] = array(
        'callback' => 'addressfield_standard_widget_refresh',
        'wrapper' => $format['#wrapper_id'],
        'method' => 'replace',
      );
    }
    else {
      if (isset($format['locality_block']['locality'])) {
        $format['locality_block']['locality']['#ajax'] = array();
      }
    }
  }
}

/**
 * The list of provinces in Cambodia.
 */
function _addressfield_kh_province_list() {
  // Using http://en.wikipedia.org/wiki/List_of_Cambodian_districts_and_sections
  $data = array(
    0 => '',
    1 => t('Banteay Meanchey'),
    2 => t('Battambang'),
    3 => t('Kampong Cham'),
    4 => t('Kampong Chhnang'),
    5 => t('Kampong Speu'),
    6 => t('Kampong Thom'),
    7 => t('Kampot'),
    8 => t('Kandal'),
    9 => t('Koh Kong'),
    10 => t('Kratié'),
    11 => t('Mondulkiri'),
    12 => t('Phnom Penh'),
    13 => t('Preah Vihear'),
    14 => t('Prey Veng'),
    15 => t('Pursat'),
    16 => t('Ratanakiri'),
    17 => t('Siem Reap'),
    18 => t('Preah Sihanouk (Kampongsam)'),
    19 => t('Stung Treng'),
    20 => t('Svay Rieng'),
    21 => t('Takéo'),
    22 => t('Oddar Meancheay'),
    23 => t('Kep'),
    24 => t('Pailin'),
  );
  asort($data);
  return $data;
}

function _addressfield_kh_district_list($province) {
  $provinces = _addressfield_kh_province_list();
  switch (array_search($province, $provinces)) {
    case 1 :
      return array(
        t('Mongkol Borei'),
        t('Phnom Srok'),
        t('Preah Netr Preah'),
        t('Ou Chrov'),
        t('Serei Saophoan'),
        t('Thma Puok'),
        t('Svay Chek'),
        t('Malai'),
        t(''),
      );
      break;
    case 2 :
      return array(
        t('Banan'),
        t('Thma Koul'),
        t('Bat Dambang Battambang'),
        t('Bavel'),
        t('Ek Phnom'),
        t('Moung Ruessi'),
        t('Rotanak Mondol'),
        t('Sangkae'),
        t('Samlout'),
        t('Sampov Loun'),
        t('Phnum Proek'),
        t('Kamrieng'),
        t('Koas Krala'),
        t('RukhaKiri'),
      );
      break;
    case 3 :
      return array(
        t('Batheay'),
        t('Chamkar Leu'),
        t('Cheung Prey'),
        t('Dambae'),
        t('Kampong Cham'),
        t('Kampong Siem'),
        t('Kang Meas'),
        t('Koh Sotin'),
        t('Krouch Chhmar'),
        t('Memot'),
        t('Ou Reang Ov'),
        t('Ponhea Kraek'),
        t('Prey Chhor'),
        t('Srey Santhor'),
        t('Stueng Trang'),
        t('Tbuong Kmoum'),
      );
      break;
    case 4 :
      return array(
        t('Baribour'),
        t('Chol Kiri'),
        t('Kampong Chhnang'),
        t('Kampong Leaeng'),
        t('Kampong Tralach'),
        t('Rolea B\'ier'),
        t('Sameakki Mean Chey'),
        t('Tuek Phos'),
      );
    case 5 :
      return array(
        t('Basedth'),
        t('Chbar Mon'),
        t('Kong Pisei'),
        t('Aoral'),
        t('Odongk'),
        t('Phnom Sruoch'),
        t('Samraong Tong'),
        t('Thpong'),
      );
    case 6 :
      return array(
        t('Baray'),
        t('Kampong Svay'),
        t('Stueng Saen'),
        t('Prasat Balangk'),
        t('Prasat Sambour'),
        t('Sandaan'),
        t('Santuk'),
        t('Stoung'),
      );
    case 7 :
      return array(
        t('Angkor Chey'),
        t('Banteay Meas'),
        t('Chhuk'),
        t('Chum Kiri'),
        t('Dang Tong'),
        t('Kampong Trach'),
        t('Kampot'),
        t('Kampong Bay'),
      );
    case 8 :
      return array(
        t('Kandal Stueng'),
        t('Kien Svay'),
        t('Khsach Kandal'),
        t('Koh Thum'),
        t('Leuk Daek'),
        t('Lvea Aem'),
        t('Mukh Kamphool'),
        t('Angk Snuol'),
        t('Ponhea Leu'),
        t('S\'ang'),
        t('Ta Khmau'),
      );
    case 9 :
      return array(
        t('Botum Sakor'),
        t('Kiri Sakor'),
        t('Koh Kong'),
        t('Smach Mean Chey'),
        t('Mondol Seima'),
        t('Srae Ambel'),
        t('Thma Bang'),
        t('Kampong Seila'),
      );
    case 10 :
      return array(
        t('Chhloung'),
        t('Kracheh'),
        t('Preaek Prasab'),
        t('Sambour'),
        t('Snuol'),
      );
    case 11 :
      return array(
        t('Kaev Seima'),
        t('Kaoh Nheaek'),
        t('Ou Reang'),
        t('Pechr Chenda'),
        t('Saen Monourom'),
      );
    case 12 :
      return array(
        t('Chamkarmon'),
        t('Daun Penh'),
        t('Prampir Makara'),
        t('Toul Kork'),
        t('Dangkor'),
        t('Meanchey'),
        t('Russey Keo'),
        t('Sen Sok'),
        t('Por Sen Chey'),
      );
    case 13 :
      return array(
        t('Chey Saen'),
        t('Chhaeb'),
        t('Choam Khsant'),
        t('Kuleaen'),
        t('Rovieng'),
        t('Sangkom Thmei'),
        t('Tbaeng Mean Chey'),
      );
    case 14 :
      return array(
        t('Ba Phnum'),
        t('Kamchay Mear'),
        t('Kampong Trabaek'),
        t('Kanhchriech'),
        t('Me Sang'),
        t('Peam Chor'),
        t('Peam Ro'),
        t('Pea Reang'),
        t('Preah Sdach'),
        t('Prey Veaeng'),
        t('Kampong Leav'),
        t('Sithor Kandal'),
      );
    case 15 :
      return array(
        t('Bakan'),
        t('Kandieng'),
        t('Krakor'),
        t('Phnum Kravanh'),
        t('Pursat'),
        t('Veal Veaeng'),
      );
    case 16 :
      return array(
        t('Andoung Meas'),
        t('Banlung'),
        t('Bar Kaev'),
        t('Koun Mom'),
        t('Lumphat'),
        t('Ou Chum'),
        t('Ou Ya Dav'),
        t('Ta Veaeng'),
        t('Veun Sai'),
      );
    case 17 :
      return array(
        t('Angkor Chum'),
        t('Angkor Thom'),
        t('Banteay Srei'),
        t('Chi Kraeng'),
        t('Kralanh'),
        t('Puok'),
        t('Prasat Bakong'),
        t('Siem Reap'),
        t('Sout Nikom'),
        t('Srei Snam'),
        t('Svay Leu'),
        t('Varin'),
      );
    case 18 :
      return array(
        t('Mittakpheap'),
        t('Prey Nob'),
        t('Stueng Hav'),
      );
    case 19 :
      return array(
        t('Sesan'),
        t('Siem Bouk'),
        t('Siem Pang'),
        t('Stung Treng'),
        t('Thala Barivat'),
      );
    case 20 :
      return array(
        t('Chantrea'),
        t('Kampong Rou'),
        t('Rumduol'),
        t('Romeas Haek'),
        t('Svay Chrum'),
        t('Svay Rieng'),
        t('Svay Teab'),
      );
    case 21 :
      return array(
        t('Angkor Borei'),
        t('Bati'),
        t('Bourei Cholsar'),
        t('Kiri Vong'),
        t('Kaoh Andaet'),
        t('Prey Kabbas'),
        t('Samraong'),
        t('Doun Kaev'),
        t('Tram Kak'),
        t('Treang'),
      );
    case 22 :
      return array(
        t('Anlong Veaeng'),
        t('Banteay Ampil'),
        t('Chong Kal'),
        t('Samraong'),
        t('Trapeang Prasat'),
      );
    case 23 :
      return array(
        t('Damnak Chang\'aeur'),
        t('Kep'),
      );
    case 24 :
      return array(
        t('Pailin'),
        t('Sala Krau'),
      );
  }
}