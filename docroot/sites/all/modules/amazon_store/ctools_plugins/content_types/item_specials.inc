<?php
/**
 * @file
 * Right now this provides nothing more than a link
 * allowing clearing the cache for the context-driven
 * amazon product. For debugging.
 */

/**
 * Callback function to supply a list of content types.
 */
function amazon_store_item_specials_ctools_content_types() {
  $items['item_specials'] = array(
    'title' => t('Special Links'),
    'content_types' => 'amazon_store_item_specials',
    'single' => TRUE,
    'render callback' => 'amazon_store_item_specials_render',
    'edit form' => 'amazon_store_item_specials_items_edit_form',
    'icon' => 'icon_amazon.png',
    'description' => t('Debugging Tools - Clear Cache for an item.'),
    'required context' => new ctools_context_required(t('Amazon Item'), 'amazon_item'),
    'category' => array(t('Amazon'), -9),

  );
  return $items;
}



function amazon_store_item_specials_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'item_specials';
  $amazon_object = $context->data;
  $block->title = t("Refresh Page/Clear Cache");

  $output = "<div>" . l(t("Refresh this page"), AMAZON_STORE_PATH . "/clear_cache/{$amazon_object->ASIN}", array('query' => "destination=" . AMAZON_STORE_PATH . "/item/{$amazon_object->ASIN}", 'attributes' => array('rel' => 'nofollow'))) . "</div>";
  $block->content = $output;
  return $block;

}


function amazon_store_item_specials_items_edit_form($form, &$form_state) {

}
