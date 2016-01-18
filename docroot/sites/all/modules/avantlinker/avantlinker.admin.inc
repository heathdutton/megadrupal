<?php

/**
 * @file
 * Admin include file to handle AvantLinker configuration.
 */

/**
 * Define the settings admin form.
 *
 */
function avantlinker_settings($form, &$form_state) {
  $form['avantlinker_api_key'] = array(
    '#title' => t('API Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('avantlinker_api_key'),
    '#description' => t('Insert your AvantLink API Authorization Key.<br />
      Get your Key ') . l(t('here.'), 'https://www.avantlink.com/affiliate/view_edit_auth_key.php', array(
        'attributes' => array('target' => '_blank'))),
    '#required' => TRUE,
    '#size' => 45,
    '#maxlength' => 45,
  );
  $form['avantlinker_affiliate_id'] = array(
    '#title' => t('Affiliate ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('avantlinker_affiliate_id'),
    '#description' => t('To utilize the AvantLinker module you must identify your AvantLink.com affiliate account..'),
    '#required' => TRUE,
    '#size' => 10,
    '#maxlength' => 10,
  );
    $form['avantlinker_website_id'] = array(
    '#title' => t('Website ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('avantlinker_website_id'),
    '#description' => t('Insert your Website ID. You may select it') . '&nbsp;' . l(t('here.'),
      'https://www.avantlink.com/affiliate/edit_websites.php', array('attributes' => array('target' => '_blank'))),
    '#required' => TRUE,
    '#size' => 10,
    '#maxlength' => 10,
  );

  $form['#validate'][] = 'avantlinker_settings_validate';

  return system_settings_form($form);
}

/**
 * Validation for the settings admin form.
 *
 */
function avantlinker_settings_validate($form, &$form_state) {
  $id = $form_state['values']['avantlinker_affiliate_id'];
  if (!ctype_digit($id) && !empty($id)) {
    form_set_error('avantlinker_affiliate_id', t('Affiliate ID Must be an integer.'));
  }
}

/**
 * Define the Affiliate Link Encoder admin form.
 *
 */
function avantlinker_ui_activate_link_encoder_form($form, &$form_state) {
  $options = array(
    'deactivate' => t('(The Affiliate Link Encoder is enabled by default with activation of this plugin.)'),
  );
  $form['avantlinker_deactivate_ale'] = array(
    '#title' => t('Deactivate ALE'),
    '#type' => 'checkboxes',
    '#default_value' => variable_get('avantlinker_deactivate_ale', array('deactivate' => 0)),
    '#description' => t('Review/manage your Affiliate Link Encoder Subscriptions') . '&nbsp;' . l(t('here.'),
      'https://www.avantlink.com/affiliate/affiliate_link_encoder_list.php', array('attributes' => array('target' => '_blank'))),
    '#required' => FALSE,
    '#weight' => 20,
    '#options' => $options,
  );
  $form['avantlinker_ale_subscription_id'] = array(
    '#title' => t('ALE Subscription'),
    '#type' => 'textfield',
    '#default_value' => variable_get('avantlinker_ale_subscription_id'),
    '#description' => t('You must have a valid ALE subscription here.'),
    '#required' => TRUE,
    '#size' => 10,
    '#weight' => 25,
    '#maxlength' => 10,
  );

  $form['#validate'][] = 'avantlinker_ui_activate_link_encoder_form_validate';

  return system_settings_form($form);
}

/**
 * Validation for the affiliate link encoder admin form.
 *
 */
function avantlinker_ui_activate_link_encoder_form_validate($form, &$form_state) {
  $id = $form_state['values']['avantlinker_ale_subscription_id'];
  if (!ctype_digit($id) && !empty($id)) {
    form_set_error('avantlinker_ale_subscription_id', t('ALE Subscription ID Must be an integer.'));
  }
}

/**
 * Define the product search admin form.
 *
 */
function avantlinker_ui_product_search_form($form, &$form_state) {
  $options = array(
    'apply' => t('Open in new tab or window.'),
  );
  $form['avantlinker_product_search_path'] = array(
    '#title' => t('Search Page Path:'),
    '#type' => 'textfield',
    '#default_value' => variable_get('avantlinker_product_search_path', 'avantlink-search-results'),
    '#description' => t('You may leave this default if you wish. This is a Drupal path and does not take a slash at either end.<br />
      It will form part of the URL that will return the product search results like example.com/avantlink-search-results.<br />
      To display the product search form the block named "AvantLinker Product Search"<br />
      should be added to your theme\'s sidebar in configuration.'),
    '#required' => TRUE,
    '#weight' => 20,
    '#size' => 60,
    '#maxlength' => 200,
  );
  $form['avantlinker_search_results_number'] = array(
    '#title' => t('Number of Results:'),
    '#type' => 'textfield',
    '#default_value' => variable_get('avantlinker_search_results_number', 10),
    '#description' => t('(The number of search results to display.)'),
    '#required' => TRUE,
    '#size' => 3,
    '#weight' => 25,
    '#maxlength' => 3,
  );
  $form['avantlinker_search_link_target'] = array(
    '#title' => t('Open Links in New Window:'),
    '#type' => 'checkboxes',
    '#default_value' => variable_get('avantlinker_search_link_target', array('apply' => 0)),
    '#description'  =>t('Check to have link open in new window: target="_blank"'),
    '#weight' => 30,
    '#required' => FALSE,
    '#options' => $options,
  );

  $form['#validate'][] = 'avantlinker_ui_product_search_form_validate';

  return system_settings_form($form);
}

/**
 * Validation for the affiliate product search admin form.
 *
 */
function avantlinker_ui_product_search_form_validate($form, &$form_state) {
  $id = $form_state['values']['avantlinker_search_results_number'];
  if (!ctype_digit($id) && !empty($id)) {
    form_set_error('avantlinker_search_results_number', t('This does not appear to be an integer.'));
  }
}

/**
 * Define the related products admin form.
 *
 */

function avantlinker_related_products_form() {
  $options = array(
    'apply' => t('(Display related products in all posts using the default keywords defined below.)'),
  );
  $form['avantlinker_apply_to_all'] = array(
    '#title' => t('Apply To All Posts:'),
    '#type' => 'checkboxes',
    '#default_value' => variable_get('avantlinker_apply_to_all', array('apply' => 'apply')),
    '#description' => t('You will be able to add keywords on a per post basis<br />
      or use the global keywords defined below, but not both.'),
    '#weight' => 5,
    '#required' => FALSE,
    '#options' => $options,
  );
  $form['avantlinker_related_products_title'] = array(
    '#title' => t('Title Text:'),
    '#type' => 'textfield',
    '#default_value' => variable_get('avantlinker_related_products_title', 'Related Products'),
    '#description' => t('(Defaults to "Related Products". 25 characters max.)'),
    '#weight' => 10,
    '#required' => FALSE,
    '#size' => 20,
    '#maxlength' => 25,
  );
    $form['avantlinker_global_keywords'] = array(
    '#title' => t('Global Keywords/Phrases:'),
    '#type' => 'textfield',
    '#default_value' => variable_get('avantlinker_global_keywords', array()),
    '#description' => t('Use comma seperated values. These become global defaults<br />
      in case no keywords are listed on the individual post page. When per post keywords are used these are ignored. '),
    '#weight' => 15,
    '#required' => FALSE,
    '#size' => 50,
  );
    $form['avantlinker_global_negative_keywords'] = array(
    '#title' => t('Global Negative Keywords:'),
    '#type' => 'textfield',
    '#default_value' => variable_get('avantlinker_global_negative_keywords', array()),
    '#description' => t('(Comma separated values allowed. Default keywords to exclude<br />
       from search results. Negative keywords defined within the "AvantLink Related Products"<br />
       meta box of specific posts will override these.) '),
    '#weight' => 20,
    '#required' => FALSE,
    '#size' => 50,
  );
    $form['avantlinker_products_number'] = array(
    '#title' => t('Number of Products:'),
    '#type' => 'textfield',
    '#default_value' => variable_get('avantlinker_products_number', 5),
    '#description' => t('Products to display with a max of 10. Default is 5.'),
    '#weight' => 25,
    '#required' => FALSE,
    '#size' => 2,
    '#maxlength' => 2,
  );
    $options_2 = array(
      'pick' => t('Open in new tab or window.'));
    $form['avantlinker_block_link_target'] = array(
    '#title' => t('Open Links in New Window:'),
    '#type' => 'checkboxes',
    '#default_value' => variable_get('avantlinker_block_link_target', array('pick' => 0)),
    '#description'  =>t('Check to have link open in new window: target="_blank"'),
    '#weight' => 30,
    '#options' => $options_2,
    '#required' => FALSE,
  );
  $form['#validate'][] = 'avantlinker_related_products_form_validate';

  return system_settings_form($form);
}

/**
 * Validation for the related products admin form.
 *
 */
function avantlinker_related_products_form_validate($form, &$form_state) {
  $number = $form_state['values']['avantlinker_products_number'];
  if (!ctype_digit($number) && !empty($number) && $number < 11) {
    form_set_error('avantlinker_ale_subscription_id', t('Number of Products must be an integer not greater than 10.'));
  }
}
