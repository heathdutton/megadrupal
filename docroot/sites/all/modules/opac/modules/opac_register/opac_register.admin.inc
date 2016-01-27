<?php
/**
 * @file
 * Configuration forms for opac register module.
 */

/**
 * Form constructor for opac users settings form.
 *
 * @see opac_users_settings_form_submit()
 * @ingroup forms
 */
function opac_register_settings_form($form, $form_state) {
  $form['fields_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mandatory fields'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $fields = array(
    'email' => array(
      'default_title' => 'Email',
    ),
    'zip-code' => array(
      'default_title' => 'Zip Code',
    ),
    'city' => array(
      'default_title' => 'City',
    ),
    'address' => array(
      'default_title' => 'Address',
    ),
    'phone' => array(
      'default_title' => 'Phone',
    ),

  );
  foreach ($fields as $id => $field) {
    $default_title = t($field['default_title'], array(), array('context' => 'libraries'));

    $form['fields_fieldset']["opac_register_field_${id}_mandatory"] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get("opac_register_field_${id}_mandatory", TRUE),
      '#title' => t("Is <em>@default_title</em> Mandatory?", array('@default_title' => $default_title), array('context' => 'libraries')),
    );
  }

  $form['terms_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Terms & conditions'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['terms_fieldset']["opac_register_terms"] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get("opac_register_terms", TRUE),
    '#title' => t("Display <em>Terms and conditions</em> validation checkbox?"),
  );

  $form['library_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Library registration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['library_fieldset']["opac_register_library_registration_expanded"] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get("opac_register_library_registration_expanded", TRUE),
    '#title' => t("Do the <em>Library registration</em> fieldset has to be expanded by default?"),
  );

  $form['library_fieldset']["opac_register_library_city_use_taxonomy"] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get("opac_register_library_city_use_taxonomy", FALSE),
    '#title' => t("Use a taxonomy for the <em>City</em> field?"),
  );

  $taxos = taxonomy_get_vocabularies();
  $taxos_options = array();
  foreach ($taxos as $taxo) {
    $taxos_options[$taxo->vid] = $taxo->name;
  }
  $form['library_fieldset']['opac_register_library_city_taxonomy'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy'),
    '#options' => $taxos_options,
    '#default_value' => variable_get('opac_register_library_city_taxonomy'),
  );

  $form['messages_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Registration messages'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['messages_fieldset']['opac_register_before_registration_text'] = array(
    '#type' => 'textarea',
    '#title' => t('A text displayed before the registration process'),
    '#default_value' => variable_get("opac_register_before_registration_text", ''),
  );

  $form['messages_fieldset']['opac_register_after_registration_text'] = array(
    '#type' => 'textarea',
    '#title' => t('A text displayed after the registration process'),
    '#default_value' => variable_get("opac_register_after_registration_text", ''),
  );

  return system_settings_form($form);
}
