<?php
// $Id: indymedia_cities.admin.inc,v 1.1.4.2 2010/08/14 07:38:58 mfb Exp $

/**
 * @file
 * Admin functions for Indymedia cities module.
 */

/**
 * Configuration options for Indymedia cities list.
 */
function indymedia_cities_settings() {
  $languages = indymedia_cities_languages();
  include_once DRUPAL_ROOT . '/includes/iso.inc';
  $predefined = _locale_get_predefined_list();
  foreach ($languages as $language => $url) {
    // Include native name in output, if possible
    if (count($predefined[$language]) > 1) {
      $tname = t($predefined[$language][0]);
      $languages[$language] = ($tname == $predefined[$language][1]) ? $tname : "$tname ({$predefined[$language][1]})";
    }
    else {
      $languages[$language] = t($predefined[$language][0]);
    }
  }
  asort($languages);
  $form['indymedia_cities_languages'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Languages'),
    '#options' => $languages,
    '#description' => t('The cities list is available in several languages.  Choose which of the available languages to use on this site.'),
    '#default_value' => variable_get('indymedia_cities_languages', array('en')),
  );
  $form['indymedia_cities_refresh'] = array(
    '#type' => 'select',
    '#title' => t('Refresh rate'),
    '#options' => drupal_map_assoc(array(0, 86400, 172800, 345600, 604800, 1209600, 2419200), 'format_interval'),
    '#description' => t('Select how often to refresh the cities list. Note, the cities list will only be refreshed when cron (cron.php) runs.'),
    '#default_value' => variable_get('indymedia_cities_refresh', 24 * 60 * 60),
  );
  $form['indymedia_cities_style_block'] = array(
    '#type' => 'radios',
    '#options' => array(
      'item_list' => t('Item list'),
      'fieldset' => t('Collapsed fieldset'),
      'accordion' => t('Accordion'),
    ),
    '#title' => t('List type'),
    '#description' => t('The cities list block can be formatted as an item list, collapsible fieldset, or accordion.'),
    '#default_value' => variable_get('indymedia_cities_style_block', 'item_list'),
  );
  return system_settings_form($form);
}
