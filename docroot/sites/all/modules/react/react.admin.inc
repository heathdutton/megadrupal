<?php

/**
 * @file
 * Administration page for React
 */

/**
 * Menu callback: displays the React module settings page.
 *
 * @ingroup forms
 */
function react_admin_settings($form) {

  $form['library'] = array(
    '#type' => 'fieldset',
    '#title' => t('React Library'),
  );

  if (function_exists('libraries_detect')) {
    $info = libraries_detect('react');
    if ($info['installed'] == FALSE) {
      drupal_set_message(t('React library not installed. Download it from <a href="@react-url" target="_blank">the official page</a>. This module expects the library to be at sites/all/libraries/react/build/react.min.js , along with other .js files.', array('@react-url' => 'http://facebook.github.io/react/')));
    }
    else {
      $form['library']['#description'] = t('React library installed. <strong>>Version detected: @version</strong>.<p>After changing the settings, flush caches.</p>', array('@version' => check_plain($info['version'])));
    }
  }

  $form['library']['react_with_addons'] = array(
    '#type' => 'checkbox',
    '#title' => t('React with addons'),
    '#default_value' => variable_get('react_with_addons', FALSE),
    '#description' => t('Load <strong>react-with-addons.min.js</strong> file instead of <strong>react.min.js</strong>. <a href="@react-url-addons" target="_blank">More info about addons.</a> Default: disabled.', array('@react-url-addons' => 'http://facebook.github.io/react/docs/addons.html')),
  );
  $form['library']['react_with_jsxtransformer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load JSX Transformer'),
    '#default_value' => variable_get('react_with_jsxtransformer', TRUE),
    '#description' => t('Load <strong>JSXTransfomer.js</strong> file. You can disable it if you are pre-compiling the templates. Default: enabled.'),
  );
  return system_settings_form($form);
}
