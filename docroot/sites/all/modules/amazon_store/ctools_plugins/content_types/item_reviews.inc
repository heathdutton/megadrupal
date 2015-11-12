<?php
/**
 @file
 item_reviews ctools plugin for amazon_store
 */

/**
 * Callback function to supply a list of content types.
 */
function amazon_store_item_reviews_ctools_content_types() {
  $items['item_reviews'] = array(
    'title' => t('Amazon Customer Reviews'),
    'content_types' => 'amazon_store_item_reviews',
    'single' => TRUE,
    'render callback' => 'amazon_store_item_reviews_render',
    'edit form' => 'amazon_store_item_reviews_edit_form',
    'icon' => 'icon_amazon.png',
    'description' => t('Amazon Item Reviews (CustomerReview).'),
    'required context' => new ctools_context_required(t('Amazon Item'), 'amazon_item'),
    'category' => array(t('Amazon'), -9),
  );
  return $items;
}


function amazon_store_item_reviews_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'item_reviews';

  $block->title = t('Customer Reviews');
  $block->content = theme('amazon_store_item_reviews_panel', array('item' => $context->data));


  return $block;
}

function amazon_store_item_reviews_edit_form($form, &$form_state) {

}
