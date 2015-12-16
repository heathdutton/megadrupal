<?php

/**
 * @file
 * Administrative callbacks for the Commerce MailChimp module.
 */

/**
 * General settings for the MailChimp Ecommerce API.
 *
 * @ingroup forms
 */
function commerce_mailchimp_settings() {
  $form['order'] = array(
    '#type' => 'fieldset',
    '#title' => t('Order Information Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['order']['commerce_mailchimp_store_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Store ID'),
    '#maxlength' => 20,
    '#size' => 20,
    '#default_value' => variable_get('commerce_mailchimp_store_id', NULL),
    '#description' => t('A unique id for the store sending the order in (20 bytes max)'),
  );
  $form['order']['commerce_mailchimp_store_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Store Name'),
    '#default_value' => variable_get('commerce_mailchimp_store_name', $_SERVER['SERVER_NAME']),
    '#description' => t('A "nice" name for the store - typically the base web address (ie, "store.mailchimp.com")'),
  );
  $form['order']['product'] = array(
    '#type' => 'fieldset',
    '#title' => t('Product Category'),
  );
  $product_types = commerce_product_types();
  foreach ($product_types as $type => $product_type) {
    $instances = field_info_instances('commerce_product', $type);
    $instance_type_options = array('' => '- None -');
    foreach ($instances as $instance) {
      $field_info = field_info_field($instance['field_name']);
      if ($field_info['type'] == 'taxonomy_term_reference') {
        $instance_type_options[$instance['field_name']] = $instance['label'];
      }
    }
    $form['order']['product']['commerce_mailchimp_category_field_' . $type] = array(
      '#type' => 'select',
      '#title' => $product_type['name'],
      '#options' => $instance_type_options,
      '#default_value' => variable_get('commerce_mailchimp_category_field_' . $type, ''),
    );
  }
  // Check node referenced for taxonomy fields.
  $reference_fields = field_read_fields(array('type' => 'commerce_product_reference'));
  $instance_type_options = array();
  foreach ($reference_fields as $bundle => $bundle_info) {
    // We have field info, but we need to use field_info_field() to get field bundles.
    $reference_field_info = field_info_field($bundle);
    if (isset($reference_field_info['bundles']) && isset($reference_field_info['bundles']['node'])) {
      foreach ($reference_field_info['bundles']['node'] as $node_type) {
        $reference_field_instances = field_info_instances('node', $node_type);
        foreach ($reference_field_instances as $instance) {
          $field_info = field_info_field($instance['field_name']);
          if ($field_info['type'] == 'taxonomy_term_reference') {
            // Get field instance for referenceable types
            $reference_field_instance = field_read_instance('node', $reference_field_info['field_name'], $node_type);
            foreach ($reference_field_instance['settings']['referenceable_types'] as $instance_type => $index) {
              $field_value = $reference_field_info['field_name'] . '|' . $instance['field_name'];
              $instance_type_options[$instance_type][$field_value] = $instance['label'];
            }
          }
        }
      }
    }
  }
  foreach ($instance_type_options as $instance_type => $data) {
    $options = array('' => t('- None -')) + $data;
    $form['order']['product']['commerce_mailchimp_category_reference_field_' . $instance_type] = array(
      '#type' => 'select',
      '#title' => t('"@instance" Commerce Product Reference Fields', array('@instance' => $product_types[$instance_type]['name'])),
      '#options' => $options,
      '#default_value' => variable_get('commerce_mailchimp_category_reference_field_' . $instance_type, ''),
    );
  }
  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('API Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['api']['commerce_mailchimp_campaign_cookie_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Campaign Cookie Name'),
    '#default_value' => variable_get('commerce_mailchimp_campaign_cookie_name', 'mailchimp_campaign_id'),
  );
  $form['api']['commerce_mailchimp_email_cookie_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Cookie Name'),
    '#default_value' => variable_get('commerce_mailchimp_email_cookie_name', 'mailchimp_email_id'),
  );
  $form['api']['commerce_mailchimp_cookie_time'] = array(
    '#type' => 'select',
    '#title' => t('Store Campaign Info For'),
    '#options' => array(
      1 => '1 Day',
      7 => '7 Days',
      14 => '14 Days',
      30 => '30 Days',
      45 => '45 Days',
      60 => '60 Days',
      90 => '90 Days'
    ),
    '#default_value' => variable_get('commerce_mailchimp_cookie_time', 30),
    '#description' => t('The length of time to store the cookie containing the campaign info sent from MailChimp. Shorter times will more accurately reflect the number of purchase made as a direct result of the email campaign, but may miss customers who make a purchase after the cookie expires. Longer times will catch more purchasing decisions but may end up including purchases that were made but were not necessarily influenced by the campaign. MailChimp recommends 30 days'),
  );

  return system_settings_form($form);
}
