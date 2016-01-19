<?php

/**
 * @file
 * New York State Global Navigation module configuration page.
 *
 * Functions that are only called on the admin pages.  Separating
 * from .module file keeps them from being loaded with every page.
 */

/**
 * NYS Universal Navigation module settings form.
 */
function nys_unav_admin_settings() {
  $form['nys_unav_auto'] = _nys_unav_auto_field();
  $form['nys_unav_interactive'] = _nys_unav_interactive_field();
  return system_settings_form($form);
}

/**
 * NYS Universal Navigation footer automatic insertion field.
 *
 * @return array
 *   Form API element for field.
 */
function _nys_unav_auto_field() {
  return array(
    '#type' => 'checkbox',
    '#title' => t('Universal navigation footer automatic insertion'),
    '#default_value' => variable_get('nys_unav_auto', TRUE),
    '#multiple' => FALSE,
    '#description' => t('Select if the universal navigation header and footer are to be automatically inserted into the page.  If not selected, make sure to use the NYS Universal Navigation blocks'),
  );
}

/**
 * NYS Universal Navigation interactive/static header selection.
 *
 * @return array
 *   Form API element for field.
 */
function _nys_unav_interactive_field() {
  $header_options = array(0 => t('Static'), 1 => t('Interactive'));
  return array(
    '#type' => 'radios',
    '#title' => t('Agency navigation header format'),
    '#options' => $header_options,
    '#default_value' => variable_get('nys_unav_interactive', TRUE),
    '#description' => t('Select which header format to use, interactive or static.'),
  );
}
