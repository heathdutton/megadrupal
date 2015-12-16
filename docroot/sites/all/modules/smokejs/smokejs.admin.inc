<?php

/**
 * @file
 * Administration forms for the jQuery Nicescroll module.
 */

/**
 * Form constructor for jQuery Nicescroll settings form.
 *
 * @todo Figure out railoffset parameter
 *
 * @ingroup forms
 */
function smokejs_settings($form, &$form_state) {
  $library = libraries_detect('smoke.js');

  if ($library['installed']) {
    $form['smoke_js_library'] = array(
      '#type' => 'radios',
      '#title' => t('Smoke.js library'),
      '#default_value' => variable_get('smokejs_library', 'minified'),
      '#options' => array(
        'minified' => t('Minified'),
        'source' => t('Source/Uncompressed'),
      ),
    );

    return system_settings_form($form);
  }
  else {
    drupal_set_message($library['error message'], 'error');
  }
}
