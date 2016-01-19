<?php

/**
 * @file
 * Admin settings for uc_avatax.
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
function uc_taxrates_ui_admin_settings($form, &$form_state) {
  $form = array();

  $tax_table_year = array(
    '2013' => t('2013'),
    '2014' => t('2014'),
    '2015' => t('2015'),
  );

  $form['uc_taxrates_tax_table_year'] = array(
    '#title' => t('TaxRate tax table year'),
    '#description' => t('Select the downloaded tax table year'),
    '#type' => 'select',
    '#options' => $tax_table_year,
    '#default_value' => variable_get('uc_taxrates_tax_table_year', array()),
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

  $form['uc_taxrates_tax_table_month'] = array(
    '#title' => t('TaxRate tax table month'),
    '#description' => t('Select the downloaded tax table month'),
    '#type' => 'select',
    '#options' => $tax_table_month,
    '#default_value' => variable_get('uc_taxrates_tax_table_month', array()),
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
  $form['uc_taxrates_select_states'] = array(
    '#title' => t('TaxRates Selected States'),
    '#description' => t('Select States in which to collect sales tax'),
    '#type' => 'select',
    '#required' => TRUE,
    '#multiple' => TRUE,
    '#options' => $states,
    '#default_value' => variable_get('uc_taxrates_select_states', array()),
  );

  $taxable_address = array(
    'Billing' => t('Billing'),
    'Shipping' => t('Shipping'),
  );

  // Configure address to use for sales tax.
  $form['options']['shipping']['uc_taxrates_tax_address'] = array(
    '#title' => t('Select Destination Address to use for Sales Tax'),
    '#description' => t('Select Shipping address if you have physical goods to ship'),
    '#type' => 'select',
    '#options' => $taxable_address,
    '#default_value' => variable_get('uc_taxrates_tax_address', 'Billing'),
  );

  // Set Sales Tax Descriptions to be shown to users.
  $form['uc_taxrates_tax_description'] = array(
    '#title' => t('Sales Tax Description'),
    '#description' => t('The Sales Tax description to be displayed on the order check out form'),
    '#type' => 'textfield',
    '#default_value' => variable_get('uc_taxrates_tax_description', 'Sales tax'),
  );

  $form['uc_taxrates_show_loc'] = array(
    '#title' => t('Show location code'),
    '#description' => t('Select Yes to include the City name in your Sales Tax description'),
    '#type' => 'select',
    '#options' => array('0' => t('No'), '1' => t('Yes')),
    '#default_value' => variable_get('uc_taxrates_show_loc', '1'),
  );

  $form['uc_taxrates_show_zero'] = array(
    '#title' => t('Show zero taxes'),
    '#description' => t('Select Yes to display a sales tax line for zero tax results'),
    '#type' => 'select',
    '#options' => array('0' => t('No'), '1' => t('Yes')),
    '#default_value' => variable_get('uc_taxrates_show_zero', '1'),
  );

  return system_settings_form($form);
}
