<?php

/**
 * @file
 * Administration forms.
 */

/**
 * General settings form.
 */
function open_readspeaker_config($form, &$form_state) {

  $language_options = array(
    'none'  => t('Select a language'),
    'en_us' => t('English (US) - English'),
    'en_uk' => t('English (British) - English'),
    'ar_ar' => t('@arabic - Arabic', array('@arabic' => urldecode('%D8%B9%D8%B1%D8%A8%D9%8A'))),
    'es_eu' => t('Euskara - Basque'),
    'ca_es' => t('Català - Catalan'),
    'da_dk' => t('Dansk - Danish'),
    'nl_nl' => t('Nederlands - Dutch'),
    'fo_fo' => t('Azerbaijani - Faroese'),
    'fi_fi' => t('Suomi - Finnish'),
    'fr_fr' => t('Français - French'),
    'fy_nl' => t('Frysk - Frisian'),
    'gl_es' => t('Galego - Galician'),
    'de_de' => t('Deutsch - German'),
    'el_gr' => t('Ελληνικά - Greek'),
    'hi_in' => t('@hindi - Hindi', array('@hindi' => urldecode('%E0%A4%B9%E0%A4%BF%E0%A4%A8%E0%A5%8D%E0%A4%A6%E0%A5%80%20(%E0%A4%AD%E0%A4%BE%E0%A4%B0%E0%A4%A4)'))),
    'it_it' => t('Italiano - Italian'),
    'ja_jp' => t('日本語 - Japanese'),
    'ko_kr' => t('한국어 - Korean'),
    'zh_cn' => t('@mandarin - Mandarin Chinese', array('@mandarin' => urldecode('%E4%B8%AD%E6%96%87%20(%E7%AE%80%E4%BD%93)'))),
    'no_nb' => t('Bokmål - Norwegian'),
    'pt_pt' => t('Português (Europeu) - Portuguese'),
    'ru_ru' => t('Русский - Russian'),
    'es_es' => t('Español (de España) - Spanish'),
    'sv_se' => t('Svenska - Swedish'),
    'tr_tr' => t('Türkçe - Turkish'),
  );

  $form['settings'] = array(
    '#title'       => t('General settings for ReadSpeaker'),
    '#description' => t('The ReadSpeaker module requires an own account at <a href="@url" target="_blank">ReadSpeaker</a>.', array('@url' => 'http://www.readspeaker.com/')),
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );

  $form['settings']['open_readspeaker_accountid'] = array(
    '#title'         => t('Enter your ReadSpeaker ID'),
    '#description'   => t('Enter your ReadSpeaker ID from <a href="@url" target="_blank">https://admin.readspeaker.com/portal</a>.', array('@url' => 'https://admin.readspeaker.com/portal')),
    '#type'          => 'textfield',
    '#default_value' => variable_get('open_readspeaker_accountid', ''),
    '#required'      => TRUE,
  );

  $form['settings']['open_readspeaker_i18n'] = array(
    '#title'         => t('Language'),
    '#description'   => t('Select which language your ReadSpeaker account supports.'),
    '#type'          => 'select',
    '#options'       => $language_options,
    '#default_value' => variable_get('open_readspeaker_i18n', 'none'),
    '#required'      => TRUE,
  );

  return system_settings_form($form);
}
