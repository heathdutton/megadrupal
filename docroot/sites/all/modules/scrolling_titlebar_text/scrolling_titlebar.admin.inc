<?php

/**
 * @file
 * Administration page for Scrolling Titlebar title module
 *
 */

function scrolling_titlebar_settings() {
$form = array();

  $form['scrolling_titlebar_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Scrolling Text Speed'),
    '#description' => t('The Speed of the scrolling text effect of browsers title bar.'),
    '#default_value' => variable_get('scrolling_titlebar_speed', '300'),
    '#size' => 5,
    '#maxlength' => 5,
  );
  $form['scrolling_titlebar_endstring'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Title End String'),
    '#description' => t('Page title for scrolling ends with this end string.'),
    '#default_value' => variable_get('scrolling_titlebar_endstring', '  '),
  );
  
  return system_settings_form($form);
}
