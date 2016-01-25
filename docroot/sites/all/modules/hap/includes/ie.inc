<?php

define('HAP_IE_6_UPDATE_URL', 'hap/update/ie6');

/**
 * Implements hook_hap_info().
 */
function hap_ie_hap_info() {
  return array(
    'name' => 'Internet Exploder',
    'description' => 'Lite-weight page redirection for IE 6 users without the need for unreliable server-side detection or slow javascript.',
    'hooks' => array('menu', 'theme', 'page_build'),
    'dependencies' => array(
      'tools' => array('blank'),
    ),
  );
}


/******************************************************************************
 * Drupal hooks
 *****************************************************************************/
/**
 * Implements hook_menu().
 */
function hap_ie_menu() {
  $items[HAP_IE_6_UPDATE_URL] = array(
    'page callback' => 'hap_blank_page',
    'page arguments' => array(hap_blank_arguments('theme', array('hap_ie_update_6'))),
    'access arguments' => array('access content'),
    'file path' => hap_path(),
    'file' => 'blank.inc',
  );

  return $items;
}

/**
 * Implements hook_theme().
 */
function hap_ie_theme($existing, $type, $theme, $path) {
  return array(
    'hap_ie_update_6' => array(
      'path' => hap_path('theme'),
      'template' => 'ie-update-6',
    ),
  );
}

/**
 * Implements hook_page_build().
 */
function hap_ie_page_build() {
  $ie6 = array(
    '#type' => 'element',
    'meta' => array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'http-equiv' => 'refresh',
        'content' => '0;url=/' . HAP_IE_6_UPDATE_URL,
      ),
      '#browsers' => array(
        'IE' => 'lte IE 6',
        '!IE' => FALSE,
      ),
    ),
    '#access' => ($_GET['q'] != HAP_IE_6_UPDATE_URL),
  );

  drupal_add_html_head($ie6, 'ie6');
}
