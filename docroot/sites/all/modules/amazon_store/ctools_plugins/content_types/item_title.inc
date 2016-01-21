<?php


/**
 * Callback function to supply a list of content types.
 */
function amazon_store_item_title_ctools_content_types() {
  $items['item_title'] = array(
    'title' => t('Amazon Item Title'),
    'content_types' => 'amazon_store_item_title',
    'single' => TRUE,
    'render callback' => 'amazon_store_item_title_render',
    'edit form' => 'amazon_store_item_title_edit_form',
    'icon' => 'icon_amazon.png',
    'description' => t('Amazon Item Title.'),
    'required context' => new ctools_context_required(t('Amazon Item'), 'amazon_item'),
    'category' => array(t('Amazon'), 9),
    'admin title' => t('Title of Amazon Item'),
    'admin info' => 'amazon_store_item_title_admin_info',
  );
  return $items;
}


function amazon_store_item_title_admin_info() {
  return "This is some admin info for the item title";
}

function amazon_store_item_title_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'item_title';

  $block->title = (string) $context->data->ItemAttributes->Title;
  $block->content = " "; // without something, block doesn't get displayed

  return $block;
}



// Per merlinofchaos, currently a bug there, so you have to have an edit form.
function amazon_store_item_title_edit_form($form, &$form_state) {

}


