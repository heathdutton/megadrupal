<?php
/**
 @file
 Provides item description for amazon item

 /**
 * Callback function to supply a list of content types.
 */
function amazon_store_item_description_ctools_content_types() {
  $items['item_description'] = array(
    'title' => t('Amazon Item Description'),
    'content_types' => 'amazon_store_item_description',
    'single' => TRUE,
    'render callback' => 'amazon_store_item_description_render',
    'edit form' => 'amazon_store_item_description_edit_form',
    'icon' => 'icon_amazon.png',
    'description' => t('Amazon Item Description (EditorialReview).'),
    'required context' => new ctools_context_required(t('Amazon Item'), 'amazon_item'),
    'category' => array(t('Amazon'), -9),

  );
  return $items;
}



function amazon_store_item_description_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'item_description';

  $block->title = (string) $context->data->EditorialReviews->EditorialReview[0]->Source;
  if (!empty($context->data->EditorialReviews->EditorialReview[0]->Content)) {
    $block->title = $context->data->EditorialReviews->EditorialReview[0]->Source;
    $block->content =  _filter_htmlcorrector(filter_xss_admin($context->data->EditorialReviews->EditorialReview[0]->Content));
 }
  return $block;
}


function amazon_store_item_description_edit_form($form, &$form_state) {

}
