<?php

/**
 * @file
 * Language mappings for Cloudwords and Drupal.
 */

/**
 * Maps languages from Cloudwords to Drupal.
 *
 * @return array
 *   A language map in the form Cloudwords => Drupal.
 */
function _cloudwords_map_cloudwords_drupal() {
  $map = array(
    'af' => 'af',
    'am' => 'am',
    'ar' => 'ar',
    'hy' => 'hy',
    'as' => 'as',
    'eu' => 'eu',
    'be' => 'be',
    'bn-bd' => 'bn',
    'bs' => 'bs',
    'bg' => 'bg',
    'my' => 'my',
    'ca' => 'ca',
    'zh-cn' => 'zh-hans',
    'zh-tw' => 'zh-hant',
    'hr' => 'hr',
    'cs' => 'cs',
    'da' => 'da',
    'dv' => 'dv',
    'nl' => 'nl',
    'en' => 'en',
    'en-gb' => 'en-gb',
    'et' => 'et',
    'fo' => 'fo',
    'fa' => 'fa',
    'fi' => 'fi',
    'fr' => 'fr',
    'fr-fr' => 'fr-fr',
    'gd-ie' => 'ga',
    'gd' => 'gd',
    'gl' => 'gl',
    'ka' => 'ka',
    'de' => 'de',
    'el' => 'el',
    'gn' => 'gn',
    'gu' => 'gu',
    'he' => 'he',
    'hi' => 'hi',
    'hu' => 'hu',
    'is' => 'is',
    'id' => 'id',
    'it' => 'it',
    'ja' => 'ja',
    'kn' => 'kn',
    'ks' => 'ks',
    'kk' => 'kk',
    'km' => 'km',
    'ko' => 'ko',
    'lo' => 'lo',
    'la' => 'la',
    'lv' => 'lv',
    'lt' => 'lt',
    'mk' => 'mk',
    'ms-my' => 'ms',
    'ml' => 'ml',
    'mt' => 'mt',
    'mi' => 'mi',
    'mr' => 'mr',
    'mn' => 'mn',
    'ne' => 'ne',
    'no-nb' => 'nb',
    'no-nn' => 'nn',
    'or' => 'or',
    'pl' => 'pl',
    'pt' => 'pt',
    'pt-br' => 'pt-br',
    'pt-pt' => 'pt-pt',
    'pa' => 'pa',
    'rm' => 'rm',
    'ro' => 'ro',
    'ru' => 'ru',
    'sa' => 'sa',
    'sr-cyrl' => 'sr',
    'tn' => 'tn',
    'sd' => 'sd',
    'si' => 'si',
    'sk' => 'sk',
    'sl' => 'sl',
    'so' => 'so',
    'es' => 'es',
    'sq' => 'sq',
    'sv' => 'sv',
    'sw' => 'sw',
    'tg' => 'tg',
    'ta' => 'ta',
    'tt' => 'tt',
    'te' => 'te',
    'th' => 'th',
    'bo' => 'bo',
    'ts' => 'ts',
    'tr' => 'tr',
    'tk' => 'tk',
    'uk' => 'uk',
    'ur' => 'ur',
    'uz-cyrl' => 'uz',
    'vi' => 'vi',
    'cy' => 'cy',
    'xh' => 'xh',
    'yi' => 'yi',
    'zu' => 'zu',
  );

  drupal_alter('cloudwords_languages_map', $map);

  return $map;
}

/**
 * Maps languages from Drupal to Cloudwords.
 *
 * @return array
 *   A language map in the form Drupal => Cloudwords.
 */
function _cloudwords_map_drupal_cloudwords() {
  return array_flip(_cloudwords_map_cloudwords_drupal());
}
