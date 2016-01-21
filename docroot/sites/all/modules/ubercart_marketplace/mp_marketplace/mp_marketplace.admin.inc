<?php

/**
 * @file
 * mp_marketplace is the central module for marketplace
 *
 * In and of itself mp_marketplace does not do much.  It is meant to be the
 * foundation layer upon which the rest of marketplace is built.
 */

/**
 * Menu callback for user/%user/selling
 * @param $account
 *   The account whose selling overview is to be displayed.
 * @return Ambigous <string, An>
 */
function mp_marketplace_selling_overview($account) {
  $output = '';
  if (module_exists('mp_reports') && user_access('view own reports')) {
    require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'mp_reports') . '/mp_reports.admin.inc';
    $output .= '<h1 class="title">' . t('Sales Summary') . '</h1>';
    $output .= '<p>' . t('Below is a summary of your sales.') . '</p>';
    $output .= mp_reports_summary($account);
  }
  drupal_set_title(t('Sales Summary'));
  return $output;
}

/**
 * Seller settings form
 */
function mp_marketplace_settings_seller_form($form, &$form_state) {
  $form['mp_product_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Product form settings',
  );
  $form['mp_product_fieldset']['mp_products_hide_fields'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide extra fields'),
    '#description' => t('Check to hide product node fields unnecessary for marketplace sellers.'),
    '#default_value' => variable_get('mp_products_hide_fields', TRUE),
  );
  $form['mp_product_fieldset']['mp_products_enable_list'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show list price'),
    '#description' => t('Check to show list price field on product form.  DO NOT uncheck if commission field is read only or autofill is turned on below AND commission field is LIST PRICE.'),
    '#default_value' => variable_get('mp_products_enable_list', TRUE),
  );
  $form['mp_product_fieldset']['mp_products_admin_base'] = array(
    '#type' => 'textfield',
    '#title' => t('Administrator base price'),
    '#description' => t('Here you can type the bare minimum a seller must set their price point to. (Useful for a "base price" for sellers to go from).'),
    '#default_value' => variable_get('mp_products_admin_base', FALSE),
  );
  $form['mp_product_fieldset']['commission_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Commission calculation settings',
  );
  $form['mp_product_fieldset']['commission_fieldset']['mp_products_commission_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum Commission Rate (Default)'),
    '#description' => t('Enter the decimal fraction of the sell price or list price to be used as the seller\'s commission rate.  This is the fraction of the sell price the seller will be paid for a product sale. Note: Commission is ultimately set by the seller on the product node form and can be set by the seller to any value not exceeding this maximum.'),
    '#default_value' => variable_get('mp_products_commission_rate', '.75'),
    '#size' => 10,
  );
  $form['mp_product_fieldset']['commission_fieldset']['mp_products_auto_calc'] = array(
    '#type' => 'checkbox',
    '#title' => t('"Read Only" seller commission field'),
    '#description' => t('Check to make the seller commision field read only (commission will be calculated automatically).  Useful if autofill is selected.  NOTE: Changing this does not refresh existing products.'),
    '#default_value' => variable_get('mp_products_auto_calc', FALSE),
  );
  $form['mp_product_fieldset']['commission_fieldset']['mp_products_auto_calc_field'] = array(
    '#type' => 'select',
    '#title' => t('Commission based on'),
    '#description' => t('Select the field the commission a seller will be paid is based on. ONLY applies if commission is read only or autofill is turned ON. NOTE: Changing this does not refresh existing products.'),
    '#default_value' => variable_get('mp_products_auto_calc_field', 'sell_price'),
    '#options' => array(
      'sell_price' => t('Sell Price'),
      'list_price' => t('List Price'),
    ),
  );
  $form['mp_product_fieldset']['commission_fieldset']['mp_products_admin_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow admin commission override'),
    '#description' => t('Check to ignore automatically calculated commission in favor of administrator specified commission value when entered. Additionally, checking this removes commission field validation for admin.'),
    '#default_value' => variable_get('mp_products_admin_override', FALSE),
  );
  $form['mp_product_fieldset']['commission_fieldset']['mp_products_insert_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autofill commission field'),
    '#description' => t('Check to insert javascript autofill function for seller commission field.'),
    '#default_value' => variable_get('mp_products_insert_js', FALSE),
  );
  if (module_exists('mp_file')) {
    $form['mp_file_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => 'File download feature settings',
    );
    $form['mp_file_fieldset']['mp_file_field_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name of product filefield'),
      '#field_prefix' => 'field_',
      '#default_value' => variable_get('mp_file_field_name', 'download'),
      '#description' => t('Enter the machine-readable name of the filefield field with which sellers will upload files.'),
      '#size' => 100,
    );
    $form['mp_file_fieldset']['mp_file_allow_sellers_perm'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow sellers to commit files'),
      '#description' => t('Check to have files automatically committed to a file download feature.  Useful if products are not moderated. WARNING: Enabling this has a potential security risk.'),
      '#default_value' => variable_get('mp_file_allow_sellers_perm', FALSE),
    );
  }

  return system_settings_form($form);
}

/*
 * Implementation of theme_preprocess_page.
 *
 * Drupal doesn't seem to hide nested menu items even if users don't have access, so...
 */
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
/*
function mp_marketplace_preprocess_page(&$vars) {
  if (arg(2) == 'selling' && !(user_access('administer sellers') || user_access('view own products'))) {
    // The user doesn't have permission to view these page, so don't show the menu tab
    mp_marketplace_removetab('Products', $vars);
    mp_marketplace_removetab('Settings', $vars); // From mp_quote
  }
}
*/

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function mp_marketplace_removetab($label, &$vars) {
  $tabs = explode("\n", $vars['tabs']);
  $vars['tabs'] = '';

  foreach ($tabs as $tab) {
    if (strpos($tab, '>' . $label . '<') === FALSE) {
      $vars['tabs'] .= $tab . "\n";
    }
  }
}
