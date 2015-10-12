<?php
/**
 * @file
 *  Provides the amazon_item context plugin for amazon_store panel
 *
 */
/**
 * Implementation of hook_ctools_contexts().
 */
function amazon_store_amazon_item_ctools_contexts() {

  $items['amazon_item'] = array(
    'title' => t('Amazon Item'),
    'description' => t('An item on Amazon.com (a unique ASIN).'),
    'context' => 'amazon_store_amazon_item_context',
    'keyword' => 'amazon_item',
    'context name' => 'amazon_item',
    'settings form' => 'amazon_store_amazon_item_settings_form',
  );
  return $items;
}

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function amazon_store_amazon_item_context($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('amazon_item');
  $context->plugin = 'amazon_item';
  $context->keyword = 'amazon_item';

  if ($empty) {
    return $context;
  }

  if ($conf) {
    $asin = $data['asin'];
  }
  else {
    $asin = $data;
  }
  if (!empty($asin)) {
    $context->data = amazon_store_retrieve_item($asin);
    if ($context->data) {
      $context->argument = $data;
      $context->title = $context->data->ItemAttributes->Title;
      return $context;
    }
  }
  return NULL;

}

function amazon_store_amazon_item_settings_form($conf, $external = FALSE) {
  $form = array();
  $form['asin'] = array(
    '#type' => 'textfield',
    '#title' => t('ASIN of the Amazon Item'),
    '#size' => 50,
    '#description' => t('The Amazon.com unique identitifier (ASIN) for this pane.'),
    '#default_value' => $conf['asin'],
  );
  return $form;
}


