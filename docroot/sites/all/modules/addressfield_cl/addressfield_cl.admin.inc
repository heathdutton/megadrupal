<?php

/**
 * @file
 * This file has the administrative pages for this module.
 */

/**
 * Form for the address format.
 */
function addressfield_cl_admin_form($form, &$form_state) {

  $form['addressfield_cl_provinces'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Select Provinces'),
    '#description'    => t('Enables the province dropdown field between the region and commune fields.'),
    '#default_value'  => variable_get('addressfield_cl_provinces', FALSE),
  );

  $form['addressfield_cl_postal_code'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Use the postal code'),
    '#description'    => t('Enables the postal code field'),
    '#default_value'  => variable_get('addressfield_cl_postal_code', FALSE),
  );

  return system_settings_form($form);
}
