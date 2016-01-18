<?php
/**
 @file
 Similar items ctools plugin for amazon_store
 */

/**
 * Callback function to supply a list of content types.
 */
function amazon_store_item_similar_items_ctools_content_types() {
  $items['item_similar_items'] = array(
    'title' => t('Similar Items'),
    'content_types' => 'amazon_store_item_similar_items',
    'single' => TRUE,
    'render callback' => 'amazon_store_item_similar_items_render',
    'edit form' => 'amazon_store_item_similar_items_edit_form',
    'icon' => 'icon_amazon.png',
    'description' => t('Amazon Item Reviews (CustomerReview).'),
    'required context' => new ctools_context_required(t('Amazon Item'), 'amazon_item'),
    'category' => array(t('Amazon'), -9),

  );
  return $items;
}


function amazon_store_item_similar_items_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'item_similar_items';
  $amazon_object = $context->data;
  $block->title = t('Similar Products');
  $block->content = theme('amazon_store_similar_items_panel', array('item' => $context->data));
  return $block;
}


function amazon_store_item_similar_items_edit_form($form, &$form_state) {

}
