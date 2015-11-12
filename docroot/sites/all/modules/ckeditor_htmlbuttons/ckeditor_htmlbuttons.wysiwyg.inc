<?php

/**
 * @file
 * Init buttons for Wysiwyg module.
 */

/**
 * Implements hook_wysiwyg_editor_settings_alter().
 *
 * Add Omega HTML5+RDFa doctype to + body class to ckeditor.
 */
function ckeditor_htmlbuttons_wysiwyg_editor_settings_alter(&$settings, &$context) {
  // These changes are only for ckeditor, so skip others.
  if ($context['profile']->editor != 'ckeditor') {
    return;
  }

  $buttons = ckeditor_htmlbutton_load_all();
  $buttons_wysiwyg_arr = array();
  foreach ($buttons as $key => $var) {
    $buttons_wysiwyg_arr[] = array(
      'name' => $var['name'],
      'icon' => $var['icon'],
      'html' => $var['html'],
      'title' => $var['title'],
    );
  }

  $settings['htmlbuttons'] = $buttons_wysiwyg_arr;
}

/**
 * Implements hook_wysiwyg_plugin().
 */
function ckeditor_htmlbuttons_wysiwyg_plugin($editor, $version) {
  if ($editor == 'ckeditor') {
    $buttons = ckeditor_htmlbutton_load_all();
    $buttons_wysiwyg_arr = array();
    foreach ($buttons as $key => $var) {
      $buttons_wysiwyg_arr[$var['name']] = 'HTML Buttons ' . $var['title'];
    }

    return array(
      'htmlbuttons' => array(
        'url' => 'https://www.drupal.org/project/ckeditor_htmlbuttons',
        'path' => drupal_get_path('module', 'ckeditor_htmlbuttons') . '/ckeditor/htmlbuttons',
        'buttons' => $buttons_wysiwyg_arr,
        'load' => TRUE,
      ),
    );
  }
}
