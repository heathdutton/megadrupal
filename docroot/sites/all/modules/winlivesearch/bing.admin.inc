<?php
// $Id: bing.admin.inc,v 1.1.2.2 2010/08/20 02:30:54 mfb Exp $

/**
 * Configuration page for Bing search.
 */
function bing_settings() {
  $form['bing_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Search title'),
    '#description' => t('Enter the desired title of the Bing search tab. This setting will not take effect until all caches are cleared.'),
    '#default_value' => variable_get('bing_title', 'Bing'),
    '#required' => TRUE,
  );
  $form['bing_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of results per page'),
    '#description' => t('Enter the number of results to display on each page.'),
    '#default_value' => variable_get('bing_limit', 15),
    '#maxlength' => 4,
    '#size' => 8,
    '#required' => TRUE,
  );
  $form['bing_appid'] = array(
    '#type' => 'textfield',
    '#title' => t('Bing App ID'),
    '#description' => t('Enter a valid <a href="http://www.bing.com/developers/createapp.aspx">Bing App ID</a>.'),
    '#default_value' => variable_get('bing_appid', ''),
    '#maxlength' => 60,
    '#size' => 60,
    '#required' => TRUE,
  );
  $form['bing_query'] = array(
    '#type' => 'textfield',
    '#title' => t('Appended query string'),
    '#description' => t('You may append an additional query string to all search queries. For example, restrict search results to two sites by entering <em>site:example.org OR site:example.com</em>.  See <a href="http://msdn.microsoft.com/en-us/library/ff795667.aspx">Bing Query Language</a>.'),
    '#default_value' => variable_get('bing_query', ''),
    '#maxlength' => 640,
    '#size' => 80,
  );
  $form['bing_safesearch'] = array(
    '#type' => 'select',
    '#options' => array(
      'Off' => t('Off'),
      'Moderate' => t('Moderate'),
      'Strict' => t('Strict'),
    ),
    '#title' => t('Safe search'),
    '#description' => t('Choose a SafeSearch configuration.'),
    '#default_value' => variable_get('bing_safesearch', 'Strict'),
  );
  $form['locale'] = array(
    '#type' => 'fieldset',
    '#title' => t('Locale settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Choose one of the <a href="http://msdn.microsoft.com/en-us/library/dd251064.aspx">supported "markets" (language and country/region combinations)</a> to be used when the search is performed using the given language.'),
  );
  $locales = drupal_map_assoc(array('ar-XA', 'bg-BG', 'cs-CZ', 'da-DK', 'de-AT', 'de-CH', 'de-DE', 'el-GR', 'en-AU', 'en-CA', 'en-GB', 'en-ID', 'en-IE', 'en-IN', 'en-MY', 'en-NZ', 'en-PH', 'en-SG', 'en-US', 'en-XA', 'en-ZA', 'es-AR', 'es-CL', 'es-ES', 'es-MX', 'es-US', 'es-XL', 'et-EE', 'fi-FI', 'fr-BE', 'fr-CA', 'fr-CH', 'fr-FR', 'he-IL', 'hr-HR', 'hu-HU', 'it-IT', 'ja-JP', 'ko-KR', 'lt-LT', 'lv-LV', 'nb-NO', 'nl-BE', 'nl-NL', 'pl-PL', 'pt-BR', 'pt-PT', 'ro-RO', 'ru-RU', 'sk-SK', 'sl-SL', 'sv-SE', 'th-TH', 'tr-TR', 'uk-UA', 'zh-CN', 'zh-HK', 'zh-TW'));
  foreach (language_list() as $langcode => $language) {
    $form['locale']['bing_locale_' . $langcode] = array(
      '#type' => 'select',
      '#title' => t('Market name for language %langcode (%language)', array('%langcode' => $language->language, '%language' => t($language->name))),
      '#options' => $locales,
      '#default_value' => variable_get('bing_locale_' . $langcode, 'en-US'),
    );
  }
  $form['source'] = array(
    '#type' => 'fieldset',
    '#title' => t('Source settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['source']['bing_source'] = array(
    '#type' => 'select',
    '#title' => t('Default search source'),
    '#description' => t('Select the <a href="http://msdn.microsoft.com/en-us/library/dd250895.aspx">search source</a> to be searched by default.'),
    '#options' => array('Web' => t('Web'), 'Image' => t('Image'), 'Video' => t('Video'), 'News' => t('News')),
    '#default_value' => variable_get('bing_source', 'Web'),
  );
  return system_settings_form($form);
}
