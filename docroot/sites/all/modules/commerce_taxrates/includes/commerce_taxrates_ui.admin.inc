<?php

/**
 * @file
 * Admin settings for commerce_taxrates
 *
 * Copyright (C) Alexander Bischoff, adTumbler.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

/**
 * Admin settings menu callback.
 */
function commerce_taxrates_ui_admin_settings($form, &$form_state) {
  $form = array();

  // Set TaxRates license details.
  $form['commerce_taxrates_product_version'] = array(
    '#title' => t('Select TaxRates Version'),
    '#description' => t('Select TaxRates Basic or TaxRates Pro'),
    '#type' => 'select',
    '#options' => array(
      COMMERCE_TAXRATES_BASIC_VERSION => t('TaxRates Basic'),
      COMMERCE_TAXRATES_PRO_VERSION => t('TaxRates Pro'),
    ),
    '#default_value' => variable_get('commerce_taxrates_product_version', COMMERCE_TAXRATES_BASIC_VERSION),
    '#ajax' => array(
      'callback' => 'commerce_taxrates_ui_update_form_options',
      'wrapper' => 'commerce_taxrates_options',
    ),
  );

  $version = variable_get('commerce_taxrates_product_version', COMMERCE_TAXRATES_BASIC_VERSION);
  if (isset($form_state['values']['commerce_taxrates_product_version']) && $form_state['values']['commerce_taxrates_product_version']) {
    $version = $form_state['values']['commerce_taxrates_product_version'];
  }

  $form['options'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="commerce_taxrates_options">',
    '#suffix' => '</div>',
    '#tree' => FALSE,
  );

  // Set Sales Tax Descriptions to be shown to users.
  $form['options']['commerce_taxrates_tax_description'] = array(
    '#title' => t('Sales Tax Description'),
    '#description' => t('The Sales Tax description to be displayed on the order check out form'),
    '#type' => 'textfield',
    '#default_value' => variable_get('commerce_taxrates_tax_description', 'Sales tax'),
  );

  $form['options']['commerce_taxrates_show_loc'] = array(
    '#title' => t('Show location code'),
    '#description' => t('Select Yes to include the City name in your Sales Tax description'),
    '#type' => 'select',
    '#options' => array('0' => t('No'), '1' => t('Yes')),
    '#default_value' => variable_get('commerce_taxrates_show_loc', '1'),
  );

  $form['options']['commerce_taxrates_show_zero'] = array(
    '#title' => t('Show zero taxes'),
    '#description' => t('Select Yes to display a sales tax line for zero tax results'),
    '#type' => 'select',
    '#options' => array('0' => t('No'), '1' => t('Yes')),
    '#default_value' => variable_get('commerce_taxrates_show_zero', '1'),
  );

  $form['options']['tables'] = array(
    '#type' => 'fieldset',
    '#title' => t('TaxRate tables'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => FALSE,
  );

  $tax_table_year = array(
    '2013' => t('2013'),
    '2014' => t('2014'),
    '2015' => t('2015'),
  );

  $form['options']['tables']['commerce_taxrates_tax_table_year'] = array(
    '#title' => t('TaxRate tax table year'),
    '#description' => t('Select the downloaded tax table year'),
    '#type' => 'select',
    '#options' => $tax_table_year,
    '#default_value' => variable_get('commerce_taxrates_tax_table_year', array()),
  );

  $tax_table_month = array(
    '01' => t('January'),
    '02' => t('February'),
    '03' => t('March'),
    '04' => t('April'),
    '05' => t('May'),
    '06' => t('June'),
    '07' => t('July'),
    '08' => t('August'),
    '09' => t('September'),
    '10' => t('October'),
    '11' => t('November'),
    '12' => t('December'),
  );

  $form['options']['tables']['commerce_taxrates_tax_table_month'] = array(
    '#title' => t('TaxRate tax table month'),
    '#description' => t('Select the downloaded tax table month'),
    '#type' => 'select',
    '#options' => $tax_table_month,
    '#default_value' => variable_get('commerce_taxrates_tax_table_month', array()),
  );

  $form['options']['shipping'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipping settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => FALSE,
  );

  $states = array(
    'AL' => t('Alabama'),
    'AK' => t('Alaska'),
    'AZ' => t('Arizona'),
    'AR' => t('Arkansas'),
    'CA' => t('California'),
    'CO' => t('Colorado'),
    'CT' => t('Connecticut'),
    'DE' => t('Delaware'),
    'DC' => t('District Of Columbia'),
    'FL' => t('Florida'),
    'GA' => t('Georgia'),
    'HI' => t('Hawaii'),
    'ID' => t('Idaho'),
    'IL' => t('Illinois'),
    'IN' => t('Indiana'),
    'IA' => t('Iowa'),
    'KS' => t('Kansas'),
    'KY' => t('Kentucky'),
    'LA' => t('Louisiana'),
    'ME' => t('Maine'),
    'MD' => t('Maryland'),
    'MA' => t('Massachusetts'),
    'MI' => t('Michigan'),
    'MN' => t('Minnesota'),
    'MS' => t('Mississippi'),
    'MO' => t('Missouri'),
    'MT' => t('Montana'),
    'NE' => t('Nebraska'),
    'NV' => t('Nevada'),
    'NH' => t('New Hampshire'),
    'NJ' => t('New Jersey'),
    'NM' => t('New Mexico'),
    'NY' => t('New York'),
    'NC' => t('North Carolina'),
    'ND' => t('North Dakota'),
    'OH' => t('Ohio'),
    'OK' => t('Oklahoma'),
    'OR' => t('Oregon'),
    'PA' => t('Pennsylvania'),
    'RI' => t('Rhode Island'),
    'SC' => t('South Carolina'),
    'SD' => t('South Dakota'),
    'TN' => t('Tennessee'),
    'TX' => t('Texas'),
    'UT' => t('Utah'),
    'VT' => t('Vermont'),
    'VA' => t('Virginia'),
    'WA' => t('Washington'),
    'WV' => t('West Virginia'),
    'WI' => t('Wisconsin'),
    'WY' => t('Wyoming'),
    'AA' => t('Armed Forces (Americas)'),
    'AE' => t('Armed Forces (Europe, Canada, Middle East, Africa)'),
    'AP' => t('Armed Forces (Pacific)'),
    'AS' => t('American Samoa'),
    'FM' => t('Federated States of Micronesia'),
    'GU' => t('Guam'),
    'MH' => t('Marshall Islands'),
    'MP' => t('Northern Mariana Islands'),
    'PW' => t('Palau'),
    'PR' => t('Puerto Rico'),
    'VI' => t('Virgin Islands'),
  );

  // Select the states in which sales tax will be calculated.
  $form['options']['shipping']['commerce_taxrates_select_states'] = array(
    '#title' => t('TaxRates Selected States'),
    '#description' => t('Select States in which to collect sales tax'),
    '#type' => 'select',
    '#required' => TRUE,
    '#multiple' => TRUE,
    '#options' => $states,
    '#default_value' => variable_get('commerce_taxrates_select_states', array()),
  );

  $taxable_address = array(
    'Billing' => t('Billing'),
    'Shipping' => t('Shipping'),
  );

  // Configure address to use for sales tax.
  $form['options']['shipping']['commerce_taxrates_tax_address'] = array(
    '#title' => t('Select Destination Address to use for Sales Tax'),
    '#description' => t('Select Shipping address if you have physical goods to ship'),
    '#type' => 'select',
    '#options' => $taxable_address,
    '#default_value' => variable_get('commerce_taxrates_tax_address', 'Billing'),
  );

  if ($version == COMMERCE_TAXRATES_PRO_VERSION) {
    $form['options']['pro'] = array(
      '#type' => 'fieldset',
      '#title' => t('TaxRates Pro settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => FALSE,
    );
    $product_types = commerce_product_types();
    foreach ($product_types as $product_type => $product_type_info) {
      $product_types[$product_type] = $product_type_info['name'];
    }
    $form['options']['pro']['commerce_taxrates_product_types'] = array(
      '#title' => t('Product types'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $product_types,
      '#default_value' => variable_get('commerce_taxrates_product_types', array()),
      '#description' => t('Which product types need a sales tax code assigned? Leave empty to enable for all product types.'),
    );
  }

  $form['#submit'][] = 'commerce_taxrates_ui_add_pro_tax_code_fields';

  return system_settings_form($form);
}

/**
 * Ajax callback, returns updated shipping states form element.
 */
function commerce_taxrates_ui_update_form_options($form, &$form_state) {
  return $form['options'];
}

/**
 * Adds taxrates_code term reference field to the selected content types.
 */
function commerce_taxrates_ui_add_pro_tax_code_fields($form, &$form_state) {
  // Get out of here if a button other than 'Submit' was clicked.
  if ($form_state['clicked_button']['#value'] != $form['actions']['submit']['#value']) {
    return;
  }

  $version = $form_state['values']['commerce_taxrates_product_version'];
  if ($version == COMMERCE_TAXRATES_PRO_VERSION) {
    // Create the taxonomy vocabulary and default taxrates code terms.
    $vocab = taxonomy_vocabulary_machine_name_load('taxrates_tax_codes');
    if (!$vocab) {
      $vocab = (object) array(
        'name' => t('TaxRates Tax codes'),
        'machine_name' => 'taxrates_tax_codes',
      );
      taxonomy_vocabulary_save($vocab);
      taxonomy_term_save((object) array(
                'name' => 'P0000000',
                'vid' => $vocab->vid,
      ));
      taxonomy_term_save((object) array(
                'name' => 'NT',
                'vid' => $vocab->vid,
      ));
    }

    // Get the old configuration.
    $old_product_types = variable_get('commerce_taxrates_product_types', array());
    if (empty($old_product_types)) {
      $old_product_types = array_keys(commerce_product_types());
    }

    $field_name = 'taxrates_code';
    $new_product_types = $form_state['values']['commerce_taxrates_product_types'];
    if (empty($new_product_types)) {
      $new_product_types = array_keys(commerce_product_types());
    }

    // If the old field name and the new field name are identical,
    // just remove the field from product types that are no longer selected.
    if ($old_product_types != $new_product_types) {
      // Remove the field from the product types that have been unselected.
      foreach ($old_product_types as $old_type) {
        if (!in_array($old_type, $new_product_types)) {
          $instance = field_info_instance('commerce_product', $field_name, $old_type);
          if ($instance) {
            field_delete_instance($instance);
          }
        }
      }
    }

    // Create the field type if it doesn't exist.
    $field = field_info_field($field_name);
    if (!$field) {
      $field = field_create_field(array(
        'cardinality' => 1,
        'field_name' => check_plain($field_name),
        'settings' => array(
          'allowed_values' => array(
            0 => array(
              'vocabulary' => 'taxrates_tax_codes',
            ),
          ),
        ),
        'type' => 'taxonomy_term_reference',
      ));
    }

    // Create the field instance for the selected product types.
    foreach ($new_product_types as $type) {
      $instance = field_info_instance('commerce_product', $field_name, $type);
      if (!$instance) {
        field_create_instance(array(
          'bundle' => $type,
          // @todo: set default_value
          'default_value' => NULL,
          'display' => array(
            'default' => array(
              'type' => 'hidden',
            ),
            'teaser' => array(
              'type' => 'hidden',
            ),
          ),
          'entity_type' => 'commerce_product',
          'field_name' => check_plain($field_name),
          'label' => 'TaxRates code',
          'required' => TRUE,
          'widget' => array(
            'module' => 'taxonomy',
            'settings' => array(
              'autocomplete_path' => 'taxonomy/autocomplete',
              'size' => 60,
            ),
            'type' => 'taxonomy_autocomplete',
            'weight' => -4,
          ),
        ));
      }
    }
  }
}
