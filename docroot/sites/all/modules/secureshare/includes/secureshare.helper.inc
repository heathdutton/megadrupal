<?php

/**
 * Helper function iterates over the settings array and removes empty values.
 * because we dont need to save any empty keys
 *
 * @param array $settings
 */
function _secureshare_sanitize_values(&$settings = array()) {
  foreach ($settings as $setting_key => $value) {
    if (is_array($value)) {
      _secureshare_sanitize_values($settings[$setting_key]);
    }
    elseif (empty($value)) {
      unset($settings[$setting_key]);
    }
  }
  return $settings;
}

/**
 * Helper method to map the drupal language shortcode to locale code...
 * @param null $lang
 * @return string
 */
function _secureshare_language_mapper($lang = NULL) {
  $languages = array(
    'af' => 'af_ZA',
    'ar' => 'ar_AR',
    'ay' => 'ay_BO',
    'az' => 'az_AZ',
    'be' => 'be_BY',
    'bg' => 'bg_BG',
    'bn' => 'bn_IN',
    'bs' => 'bs_BA',
    'ca' => 'ca_ES',
    'ck' => 'ck_US',
    'cs' => 'cs_CZ',
    'cy' => 'cy_GB',
    'da' => 'da_DK',
    'de' => 'de_DE',
    'el' => 'el_GR',
    'en' => 'en_US',
    'eo' => 'eo_EO',
    'es' => 'es_ES',
    'et' => 'et_EE',
    'eu' => 'eu_ES',
    'fa' => 'fa_IR',
    'fb' => 'fb_FI',
    'fb' => 'fb_LT',
    'fi' => 'fi_FI',
    'fo' => 'fo_FO',
    'fr' => 'fr_CA',
    'fr' => 'fr_FR',
    'ga' => 'ga_IE',
    'gl' => 'gl_ES',
    'gn' => 'gn_PY',
    'gu' => 'gu_IN',
    'he' => 'he_IL',
    'hi' => 'hi_IN',
    'hr' => 'hr_HR',
    'hu' => 'hu_HU',
    'hy' => 'hy_AM',
    'id' => 'id_ID',
    'is' => 'is_IS',
    'it' => 'it_IT',
    'ja' => 'ja_JP',
    'jv' => 'jv_ID',
    'ka' => 'ka_GE',
    'kk' => 'kk_KZ',
    'km' => 'km_KH',
    'kn' => 'kn_IN',
    'ko' => 'ko_KR',
    'ku' => 'ku_TR',
    'la' => 'la_VA',
    'li' => 'li_NL',
    'lt' => 'lt_LT',
    'lv' => 'lv_LV',
    'mg' => 'mg_MG',
    'mk' => 'mk_MK',
    'ml' => 'ml_IN',
    'mn' => 'mn_MN',
    'mr' => 'mr_IN',
    'ms' => 'ms_MY',
    'mt' => 'mt_MT',
    'nb' => 'nb_NO',
    'ne' => 'ne_NP',
    'nl' => 'nl_BE',
    'nl' => 'nl_NL',
    'nn' => 'nn_NO',
    'pa' => 'pa_IN',
    'pl' => 'pl_PL',
    'ps' => 'ps_AF',
    'pt' => 'pt_PT',
    'qu' => 'qu_PE',
    'rm' => 'rm_CH',
    'ro' => 'ro_RO',
    'ru' => 'ru_RU',
    'sa' => 'sa_IN',
    'se' => 'se_NO',
    'sk' => 'sk_SK',
    'sl' => 'sl_SI',
    'so' => 'so_SO',
    'sq' => 'sq_AL',
    'sr' => 'sr_RS',
    'sv' => 'sv_SE',
    'sw' => 'sw_KE',
    'sy' => 'sy_SY',
    'ta' => 'ta_IN',
    'te' => 'te_IN',
    'tg' => 'tg_TJ',
    'th' => 'th_TH',
    'tl' => 'tl_PH',
    'tl' => 'tl_ST',
    'tr' => 'tr_TR',
    'tt' => 'tt_RU',
    'uk' => 'uk_UA',
    'ur' => 'ur_PK',
    'uz' => 'uz_UZ',
    'vi' => 'vi_VN',
    'xh' => 'xh_ZA',
    'yi' => 'yi_DE',
    'zh' => 'zh_CN',
    'zu' => 'zu_ZA',
  );

  return (isset($languages[$lang])) ? $languages[$lang] : 'en_US';
}