<?php

/**
 * @file
 * Module settings.
 */

/**
 * Page callback for admninistrative form.
 *
 * @return array
 *   A drupal form array.
 */
function opendata_settings() {
  $form = array();

  $form['opendata_help'] = array(
    '#markup' => t('Use this form to change various OpenData settings.'),
  );

  return system_settings_form($form);
}
