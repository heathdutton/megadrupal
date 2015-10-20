<?php
// $Id: theme-settings.php,v 1.3 2010/08/16 14:36:19 jarek Exp $

function titan_form_system_theme_settings_alter(&$form, $form_state) {
  // Generate the form using Forms API. http://api.drupal.org/api/7
  $form['custom'] = array(
    '#title' => 'Custom theme settings', 
    '#type' => 'fieldset', 
  );
  $form['custom']['trim_pager'] = array(
    '#type' => 'select',
    '#title' => 'Trim pager after specified number of pages', 
    '#default_value' => theme_get_setting('trim_pager'),
    '#options' => titan_generate_array(4, 15, 1, ''),
  );
  $form['custom']['copyright_information'] = array(
    '#title' => 'Copyright information',
    '#description' => t('Information about copyright holder of the website - will show up at the bottom of the page'), 
    '#type' => 'textfield',
    '#default_value' => theme_get_setting('copyright_information'),
    '#size' => 60, 
    '#maxlength' => 128, 
    '#required' => FALSE,
  );
}

function titan_generate_array($min, $max, $increment, $postfix, $unlimited = NULL) {
  $array = array();
  if ($unlimited == 'first') {
    $array['none'] = 'Unlimited';
  }
  for ($a = $min; $a <= $max; $a += $increment) {
    $array[$a . $postfix] = $a . ' ' . $postfix;
  }
  if ($unlimited == 'last') {
    $array['none'] = 'Unlimited';
  }
  return $array;
}

