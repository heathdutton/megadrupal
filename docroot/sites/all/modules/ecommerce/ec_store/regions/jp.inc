<?php

/**
 * @file
 * Japanese regional Settings
 */

/**
 * Implements hook_ec_region_COUNTRYCODE_info().
 */
function ec_store_ec_region_jp_info() {
  return array(
    'use_state_names' => TRUE,
    'states' => array(
      '01' => t('Hokkaido'),
      '02' => t('Aomori'),
      '03' => t('Iwate'),
      '04' => t('Miyagi'),
      '05' => t('Akita'),
      '06' => t('Yamagata'),
      '07' => t('Hukusima (Fukushima)'),
      '08' => t('Ibaraki'),
      '09' => t('Totigi (Tochigi)'),
      '10' => t('Gunma'),
      '11' => t('Saitama'),
      '12' => t('Tiba (Chiba)'),
      '13' => t('Tokyo'),
      '14' => t('Kanagawa'),
      '15' => t('Niigata'),
      '16' => t('Toyama'),
      '17' => t('Isikawa (Ishikawa)'),
      '18' => t('Hukui (Fukui)'),
      '19' => t('Yamanasi (Yamanashi)'),
      '20' => t('Nagano'),
      '21' => t('Gihu  (Gifu)'),
      '22' => t('Sizuoka (Shizuoka)'),
      '23' => t('Aiti (Aichi)'),
      '24' => t('Mie'),
      '25' => t('Siga (Shiga)'),
      '26' => t('Kyoto'),
      '27' => t('Osaka'),
      '28' => t('Hyogo'),
      '29' => t('Nara'),
      '30' => t('Wakayama'),
      '31' => t('Tottori'),
      '32' => t('Simane (Shimane)'),
      '33' => t('Okayama'),
      '34' => t('Hirosima (Hiroshima)'),
      '35' => t('Yamaguti (Yamaguchi)'),
      '36' => t('Tokusima (Tokushima)'),
      '37' => t('Kagawa'),
      '38' => t('Ehime'),
      '39' => t('Koti (Kochi)'),
      '40' => t('Hukuoka (Fukuoka)'),
      '41' => t('Saga'),
      '42' => t('Nagasaki'),
      '43' => t('Kumamoto'),
      '44' => t('Oita'),
      '45' => t('Miyazaki'),
      '46' => t('Kagosima (Kagoshima)'),
      '47' => t('Okinawa'),
    ),
  );
}