<?php
/**
 * @file
 * Debugging functions
 */

/**
 * Settings form for QD Core.
 *
 * @return array
 *   The form array.
 */
function qd_core_settings_form() {
  $form = array();

  $form['qd_core_settings'] = array(
    '#type' => 'markup',
    '#markup' => t('There are no settings for QD Core yet.'),
  );

  return $form;
}
