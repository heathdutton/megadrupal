<?php

/**
 * @file
 * Admin page callbacks for the Media: ShareStream module.
 */

/**
 * The Media: ShareStream configuration form.
 *
 * @ingroup forms
 *
 * @see system_settings_form()
 */
function media_sharestream_settings_form() {
  $form['media_sharestream_server'] = array(
    '#type' => 'textfield',
    '#title' => t('ShareStream Server'),
    '#default_value' => variable_get('media_sharestream_server', ''),
    '#description' => t('Public ShareStream Pick-N-Play Server URL, without the protocol. Example: sharestream.example.org'),
  );

  return system_settings_form($form);
}
