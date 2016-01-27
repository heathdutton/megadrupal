<?php


/**
 * Callback function to supply a list of content types.
 */
function amazon_store_item_details_ctools_content_types() {
  $items['item_details'] = array(
    'title' => t('Product Details'),
    'content_types' => 'amazon_store_item_details',
    'single' => TRUE,
    'render callback' => 'amazon_store_item_details_render',
    'edit form' => 'amazon_store_item_details_edit_form',
    'icon' => 'icon_amazon.png',
    'description' => t('Amazon Item Details.'),
    'required context' => new ctools_context_required(t('Amazon Item'), 'amazon_item'),
    'category' => array(t('Amazon'), -9),  );
  return $items;
}


function amazon_store_item_details_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'item_details';
  $amazon_object = $context->data;

  $block->title = t("Product Details");
  // TODO: Check the theme array.
  $block->content = theme('amazon_store_details_panel', array('item' => $context->data));

  return $block;
}

function amazon_store_item_details_edit_form($form, &$form_state) {

}
