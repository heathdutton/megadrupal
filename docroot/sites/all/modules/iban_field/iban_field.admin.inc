<?php
/**
 * @file
 * Form builder. Configure IBAN Field global settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */

/**
 * Creates settings form for the IBAN field global settings form.
 */
function iban_field_admin_settings_form() {

  $header = array(
    t('Country code'),
    t('Country name'),
    t('IBAN length'),
  );
  $rows = array();

  $query = db_select('iban_field_countries', 'i')
    ->extend('PagerDefault')
    ->fields('i', array(
      'country_code',
      'country_name',
      'iban_length',
      'country_enabled')
    )
    ->orderBy('country_code', 'ASC')
    ->limit('30');

  $result = $query->execute();

  foreach ($result as $country) {
    $rows[$country->country_code] = array(
      $country->country_code,
      $country->country_name,
      $country->iban_length,
    );
    $default_value[$country->country_code] = $country->country_enabled;
  }

  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#empty' => t('No countries available.'),
    '#default_value' => $default_value,
  );

  $form['pager'] = array(
    '#markup' => theme('pager'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for iban field global settings form.
 */
function iban_field_admin_settings_form_submit($form, &$form_state) {
  foreach ($form_state['values']['table'] as $country_code => $value) {
    if ($value == '0') {
      $query = db_update('iban_field_countries')
        ->fields(array(
          'country_enabled' => 0,
        ))
        ->condition('country_code', $country_code, '=')
        ->execute();
    }
    else {
      $query = db_update('iban_field_countries')
        ->fields(array(
          'country_enabled' => 1,
        ))
        ->condition('country_code', $country_code, '=')
        ->execute();
    }
  }
}
