<?php

/**
 * @file
 * Admin settings for uc_tax_wa.
 *
 * Copyright (C) 2008 by Jennifer Hodgdon, Poplar ProductivityWare LLC
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

/**
 * Admin settings menu callback.
 */
function uc_tax_wa_admin_settings() {

  $form = array();

  // Set default tax rate and location code.

  $form['uc_tax_wa_default_rate'] = array(
    '#title' => t('Default tax rate'),
    '#description' => t('The default tax rate to use, if the delivery address is in Washington state, but the rate lookup fails. Enter as a decimal, such as 9.8 for a 9.8% tax rate'),
    '#type' => 'textfield',
    '#default_value' => variable_get('uc_tax_wa_default_rate', '0'),
  );

  $form['uc_tax_wa_default_loc'] = array(
    '#title' => t('Default location code'),
    '#description' => t('The default tax location code to use, if the delivery address is in Washington state, but the rate lookup fails. This is a 4-digit number, obtained from the !dor. Note that providing this location does NOT override the default rate (they should agree).', array('!dor' => l('Department of Revenue', 'http://dor.wa.gov/content/findtaxesandrates/salesandusetaxrates/lookupataxrate/'))),
    '#type' => 'textfield',
    '#default_value' => variable_get('uc_tax_wa_default_loc', '0'),
  );

  $form['uc_tax_wa_show_loc'] = array(
    '#title' => t('Show location code'),
    '#description' => t('Set to Yes if you want to show the location code on the tax line item, so you and the customer can see what location was used.'),
    '#type' => 'radios',
    '#options' => array('0' => t('No'), '1' => t('Yes')),
    '#default_value' => variable_get('uc_tax_wa_show_loc', '0'),
  );

  // Wholesale users - exclude users with these role from tax.

  $form['uc_tax_wa_wholesale_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Wholesale roles, to exclude from tax'),
    '#description' => t('Exclude tax from any user who has one of the checked roles'),
    '#multiple' => TRUE,
    '#options' => user_roles(TRUE),
    '#default_value' => variable_get('uc_tax_wa_wholesale_roles', array()),
  );

  // Define taxed product types.

  $form['uc_tax_wa_product_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Taxed product types'),
    '#description' => t('Product types to apply Washington state sales taxes to.'),
    '#multiple' => TRUE,
    '#options' => uc_product_type_names(),
    '#default_value' => variable_get('uc_tax_wa_product_types', array()),
  );

  // taxed line items
  // ripped from the uc_taxes module

  $options = array();
  foreach (_uc_line_item_list() as $line_item) {
    if (!in_array($line_item['id'], array('subtotal', 'tax_subtotal', 'total'))) {
      $options[$line_item['id']] = $line_item['title'];
    }
  }

  $form['uc_tax_wa_line_items'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Taxed line items'),
    '#description' => t('Line items to apply Washington state sales taxes to.'),
    '#multiple' => TRUE,
    '#options' => $options,
    '#default_value' => variable_get('uc_tax_wa_line_items', array()),
  );

  return system_settings_form($form);
}
