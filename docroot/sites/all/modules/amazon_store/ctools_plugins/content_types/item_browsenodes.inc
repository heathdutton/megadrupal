<?php
/**
 @file
 Implements browsenodes content type for amazon_store
 */

/**
 * Callback function to supply a list of content types.
 */
function amazon_store_item_browsenodes_ctools_content_types() {
  $items['item_browsenodes'] = array(
    'title' => t('Amazon Browse'),
    'content_types' => 'amazon_store_item_browsenodes',
    'single' => TRUE,
    'render callback' => 'amazon_store_item_browsenodes_render',
    'edit form' => 'amazon_store_item_browsenodes_edit_form',
    'icon' => 'icon_amazon.png',
    'description' => t('Amazon Item Reviews (CustomerReview).'),
    'required context' => new ctools_context_required(t('Amazon Item'), 'amazon_item'),
    'category' => array(t('Amazon'), -9),
  );
  return $items;
}


function amazon_store_item_browsenodes_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'item_browsenodes';
  $amazon_object = $context->data;
  $block->title = t('Browse Categories');
  $block->content = theme('amazon_store_browsenodes_panel', array('item' => $context->data));
  return $block;
}


// Per merlinofchaos, currently a bug there, so you have to have an edit form.
function amazon_store_item_browsenodes_edit_form($form, &$form_state) {

}
