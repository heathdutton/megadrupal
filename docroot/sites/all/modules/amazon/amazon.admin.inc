<?php

/**
 * @file
 * Amazon Admin pages.
 */
/**
 * Basic Amazon settings form.
 */
function amazon_settings_form($form, &$form_state) {
  $cache = amazon_data_cache();
  $options = array();
  foreach ($cache['locales'] as $locale => $data) {
    $options[$locale] = $data['name'];
  }
  $form['amazon_locale'] = array(
    '#type' => 'select',
    '#title' => t('Amazon locale'),
    '#default_value' => variable_get('amazon_locale', 'US'),
    '#options' => $options,
    '#description' => t('Amazon.com uses separate product databases and Ecommerce features in different locales; pricing and availability information, as well as product categorization, differs from one locale to the next. Be sure to select the locale your site will be running in.'),
  );

  $form['associate_setting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Amazon referral settings'),
  );
  $form['associate_setting']['amazon_associate_setting'] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('What Amazon associate ID do you want to use?'),
    '#default_value' => variable_get('amazon_associate_setting', 'association'),
    '#options' => array(
      'association' => t('Use the Drupal Association\'s associate ID'),
      'custom' => t('Use your own associate ID'),
    ),
    '#description' => t('Outgoing requests and links to Amazon.com must include an associate ID code. When shoppers purchase Amazon products via one of those links, the associate ID is used to determine who should receive a referral bonus from Amazon.'),
  );

  $form['associate_setting']['amazon_custom_associate_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Your associate ID'),
    '#description' => t('Enter your own associate ID to receive referral bonuses when shoppers purchase Amazon products via your site.'),
    '#default_value' => variable_get('amazon_custom_associate_id', ''),
    '#states' => array(
      'visible' => array(
        "select[name=amazon_associate_setting]" => array('value' => 'custom'),
      ),
    ),

  );

  $form['amazon_aws_access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Amazon AWS Access Key ID'),
    '#description' => t('You must sign up for an Amazon AWS account to use the Product Advertising Service. See !more_info for information and a registration form.', array('!more_info' => l(t('the AWS home page'), 'https://aws-portal.amazon.com/gp/aws/developer/account/index.html?ie=UTF8&action=access-key', array('html' => TRUE)))),
    '#default_value' => variable_get('amazon_aws_access_key', ''),
    '#required' => TRUE,
  );

  $form['amazon_aws_secret_access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Amazon AWS Secret Access Key'),
    '#description' => t('You must sign up for an Amazon AWS account to use the Product Advertising Service. See !more_info for information and a registration form.', array('!more_info' => l(t('the AWS home page'), 'https://aws-portal.amazon.com/gp/aws/developer/account/index.html?ie=UTF8&action=access-key', array('html' => TRUE)))),
    '#default_value' => variable_get('amazon_aws_secret_access_key', ""),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Validate the keys and trim whitespace.
 */
function amazon_settings_form_validate($form, &$form_state) {
  $form_state['values']['amazon_aws_access_key'] = trim($form_state['values']['amazon_aws_access_key']);
  $form_state['values']['amazon_aws_secret_access_key'] = trim($form_state['values']['amazon_aws_secret_access_key']);
}

/**
 * Form to determine how long things get cached.
 */
function amazon_storage_settings_form($form, &$form_state) {

  $period = drupal_map_assoc(array(3600, 7200, 14400, 21600, 43200, 86400), 'format_interval');
  $form['details']['amazon_refresh_schedule'] = array(
    '#type' => 'select',
    '#title' => t('Amazon refresh schedule'),
    '#description' => t('Cached information on Amazon items must be refreshed regularly to keep pricing and stock information up to date. Cron must be enabled for this function to work properly.'),
    '#default_value' => variable_get('amazon_refresh_schedule', 86400),
    '#options' => $period
  );

  $form['details']['amazon_core_data'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Store extended product data'),
    '#default_value' => variable_get('amazon_core_data', array('creators', 'images', 'editorial_reviews')),
    '#options' => array(
      'creators' => t('Book authors, film actors, etc.'),
      'images' => t('Product image links'),
      'editorial_reviews' => t('Editorial reviews'),
    ),
  );

  return system_settings_form($form);
}

/**
 * Form for testing a single ASIN.
 *
 */
function amazon_test_form($form, &$form_state) {
  $form = array();

  $form['asin'] = array(
    '#type' => 'textfield',
    '#title' => t('Amazon Product ID'),
    '#description' => t('The ASIN (ISBN-10) or EAN (ISBN-13) of a product listed on Amazon.'),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Look Up Product'),
  );

  if (isset($form_state['amazon_item'])) {
    $form['item'] = array(
      '#type' => 'fieldset',
      '#title' => t('Result'),
      '#collapsible' => FALSE,
    );
    $form['item']['display'] = array(
      '#markup' => theme('amazon_item', array('item' => $form_state['amazon_item'], 'style' => 'details')),
      '#weight' => 9,
    );
    $form['item']['details'] = array(
      '#type' => 'fieldset',
      '#title' => t('Details'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 10,
    );
    $form['item']['details']['data'] = array(
      '#markup' => '<pre><small>' . print_r($form_state['amazon_item'], TRUE) . '</small></pre>',
    );
  }
  return $form;
}

function amazon_test_form_validate($form, &$form_state) {
  // Normalize input and look up
  $asin = amazon_convert_to_asin($form_state['values']['asin']);
  $items = amazon_item_lookup_from_web(array($asin));
  if (!empty($items) && is_array($items)) {
    $form_state['amazon_item'] = array_pop($items);
  }
  else {
    form_set_error('asin', t("%input is no valid ASIN or EAN. Please check the !link for messages.", array('%input' => $form_state['values']['asin'], '!link' => l(t("error log"), 'admin/reports/dblog'))));
  }
}

function amazon_test_form_submit($form, &$form_state) {
  $item = $form_state['amazon_item'];
  amazon_item_delete($item['asin']);
  amazon_item_insert($item);
  $form_state['rebuild'] = TRUE;
}
