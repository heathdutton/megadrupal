<?php

/**
 * @file
 * Configure settings for Magento Product integration into Drupal.
 */

/**
 * Form callback for admin settings for module.
 */
function magento_products_settings_form(&$form, &$form_state) {
  $content_types = array();
  foreach (node_type_get_types() as $node_type) {
    $content_types[$node_type->type] = $node_type->name;
  }

  $form['magento_products'] = array(
    '#type' => 'fieldset',
    '#title' => t('Magento Product Integration Settings'),
  );

  $form['magento_products']['magento_product_node_content_type'] = array(
    '#title' => t('Destination content type'),
    '#description' => t('Incoming products from Magento will be created as this content type'),
    '#type' => 'select',
    '#options' => $content_types,
    '#required' => TRUE,
    '#default_value' => variable_get('magento_product_node_content_type'),
  );

  $form['magento_products']['magento_store_url'] = array(
    '#title' => t('Magento Store URL'),
    '#description' => t('The fully qualified domain name of the Magento Store. This is used to build links to product pages. <strong>Do not</strong> add a trailing slash.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('magento_store_url'),
  );

  $import_limit_options = array(
    1 => 1,
    5 => 5,
    25 => 25,
    50 => 50,
    75 => 75,
    100 => 100,
    200 => 200
  );
  $form['magento_products']['magento_products_import_limit'] = array(
    '#title' => t('Import limit'),
    '#description' => t('Limit the number of products import from the import queue on each import run.'),
    '#type' => 'select',
    '#options' => $import_limit_options,
    '#default_value' => variable_get('magento_products_import_limit', 50),
  );

  $form['magento_products']['magento_products_import_during_cron'] = array(
    '#title' => t('Import Magento Products during cron'),
    '#description' => t('You can run <em>drush magento-import</em> directly and not have the import run during cron.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('magento_products_import_during_cron'),
  );

  $queue_size = db_query('SELECT COUNT(*) FROM {magento_products_queue}')->fetchField();
  $form['magento_products']['status_title'] = array(
    '#markup' => t('Import Status'),
    '#prefix' => '<h2>',
    '#suffix' => '</h2>',
  );

  $form['magento_products']['status'] = array(
    '#prefix' => '<p>',
    '#markup' => t('Magento products were last imported on <strong>@date</strong> (if no products changed on the Magento end this date will not change). There are <strong>@count</strong> waiting to be imported in the import queue. Use the button below to queue all Magento Products for re-updating. No products are removed but all will be added to the queue to be updated. This is helpful when you have changed the field mapping and want to pull in all of this data on existing imported products.', array(
      '@date' => variable_get('magento_product_last_sync', FALSE) ? format_date(variable_get('magento_product_last_sync')) : t('Never'),
      '@count' => format_plural($queue_size, '@count product', '@count products'),
    )),
    '#suffix' => '</p>',
  );
  $form['magento_products']['requeue_all_products'] = array(
    '#type' => 'submit',
    '#value' => t('Requeue all Magento products'),
    '#submit' => array('magento_products_settings_form_queue_all_products'),
  );
  $form['magento_products']['process_batch'] = array(
    '#type' => 'submit',
    '#value' => t('Process @count from the queue', array(
      '@count' => format_plural(variable_get('magento_products_import_limit', 50), '@count product', '@count products'),
    )),
    '#submit' => array('magento_products_settings_form_process_queue'),
  );
}

/**
 * Custom submit handler for adding all products to the queue.
 */
function magento_products_settings_form_process_queue($form, &$form_state) {
  module_load_include('inc', 'magento_products');
  $upserts_count = magento_products_import();
  $upserts_phrase = format_plural($upserts_count, '@count product', '@count products');

  drupal_set_message(t('Processed @count from the import queue.', array('@count' => $upserts_phrase)), 'status');
}

/**
 * Custom submit handler for adding all products to the queue.
 */
function magento_products_settings_form_queue_all_products($form, &$form_state) {
  db_query('REPLACE INTO {magento_products_queue} (magento_product_id, created) SELECT magento_product_id, UNIX_TIMESTAMP() FROM {magento_products}');

  drupal_set_message(t('All Magento Products have been queued for import.'), 'status');
}
