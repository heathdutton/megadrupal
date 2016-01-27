<?php
/**
 @file
 Item_offers content_type ctools plugin
 */

/**
 * Callback function to supply a list of content types.
 */
function amazon_store_item_offers_ctools_content_types() {
  $items['item_offers'] = array(
    'title' => t('Offers'),
    'content_types' => 'amazon_store_item_offers',
    'single' => TRUE,
    'render callback' => 'amazon_store_item_offers_render',
    'edit form' => 'amazon_store_item_offers_edit_form',
    'icon' => 'icon_amazon.png',
    'description' => t('Amazon Item Offers.'),
    'required context' => new ctools_context_required(t('Amazon Item'), 'amazon_item'),
    'category' => array(t('Amazon'), -9),
  );
  return $items;
}


function amazon_store_item_offers_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'item_offers';
  $amazon_object = $context->data;
  $block->title = t("Buying Options");

  $block->content = theme('amazon_store_item_offers', array('amazon_item' => $amazon_object));
  return $block;
}

function amazon_store_item_offers_edit_form($form, &$form_state) {

}
