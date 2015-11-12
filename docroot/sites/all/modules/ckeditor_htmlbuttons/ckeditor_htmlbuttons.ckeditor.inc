<?php

/**
 * @file
 * Init buttons for CKEditor module.
 */

/**
 * Implements hook_init().
 */
function _ckeditor_htmlbuttons_ckeditor_add_buttons_js() {
  if (module_exists('ckeditor')) {
    $buttons = ckeditor_htmlbutton_load_all();
    drupal_add_js(array('ckeditorHtmlbuttons' => $buttons), 'setting');
  }
}

/**
 * Implements hook_ckeditor_plugin().
 */
function ckeditor_htmlbuttons_ckeditor_plugin() {
  $buttons = ckeditor_htmlbutton_load_all();
  $buttons_ckeditor = array();
  foreach ($buttons as $key => $var) {
    $buttons_ckeditor[$var['name']] = array(
      'icon' => $var['icon'],
      'label' => $var['title'],
    );
  }

  return array(
    'htmlbuttons' => array(
      'name' => 'htmlbuttons',
      'path' => drupal_get_path('module', 'ckeditor_htmlbuttons') . '/ckeditor/htmlbuttons/',
      'desc' => 'HTML Buttons',
      'buttons' => $buttons_ckeditor,
    ),
  );
}
