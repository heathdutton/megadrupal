<?php

/**
 * @file
 * Administrative page callbacks for the permafilelink module.
 */

/**
 * Settings form.
 */
function permafilelink_settings_form() {
  $form['permafilelink_verbose_reporting'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable verbose reporting'),
    '#default_value' => variable_get('permafilelink_verbose_reporting', 0),
    '#description' => t('Enable to make Permanent Filelink report its activities in more detail.'),
  );

  return system_settings_form($form);
}
