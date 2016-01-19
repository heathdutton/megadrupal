<?php
/**
 * @file
 * Jquery_clocks configuration page code.
 *
 * Build the jquery_clocks configuration page with the help of drupal form api.
 */

/**
 * Menu callback function for jqery_clocks configurations.
 * @see jquery_clocks_menu()
 */
function jquery_clocks_config_page() {
  // Check library exist or not.
  if (!is_dir(jquery_clocks_get_library_path('jquery-clock-plugin'))) {
    $library = drupal_get_library('jquery_clocks', 'jquery-clock-plugin');
    drupal_set_message(t("Jquery-clock-plugin library missing. Please donwload from <a href='@url' target='_blank' >here</a> and rename it to 'jquery-clock-plugin' and place it into libraries directory.", array('@url' => $library['website'])), 'warning');
  }
  if (!is_dir(jquery_clocks_get_library_path('jClocksGMT-master'))) {
    $library = drupal_get_library('jquery_clocks', 'jClocksGMT-master');
    drupal_set_message(t("jClocksGMT-master library missing. Please donwload from <a href='@url' target='_blank' >here</a> and rename it to 'jClocksGMT-master' and place it into libraries directory.", array('@url' => $library['website'])), 'warning');
  }
  if (!is_dir(jquery_clocks_get_library_path('jQuery-Clock-Plugin-master'))) {
    $library = drupal_get_library('jquery_clocks', 'jQuery-Clock-Plugin-master');
    drupal_set_message(t("jQuery-Clock-Plugin-master library missing. Please donwload from <a href='@url' target='_blank' >here</a> and rename it to 'jQuery-Clock-Plugin-master' and place it into libraries directory.", array('@url' => $library['website'])), 'warning');
  }
  $form = array();
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Jquery clocks styles'),
  );
  $form['jquery_clocks_styles'] = array(
    '#type' => 'checkboxes',
    '#options' => jquery_clocks_styles_list(),
    '#default_value' => variable_get('jquery_clocks_styles', array()),
  );
  return system_settings_form($form);
}
