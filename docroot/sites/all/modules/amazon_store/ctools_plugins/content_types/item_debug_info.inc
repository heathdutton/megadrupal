<?php
/**
 * @file
 * Debugging info content_type plugin for amazon_store
 */

/**
 * Callback function to supply a list of content types.
 */
function amazon_store_item_debug_info_ctools_content_types() {
  $items['item_debug_info'] = array(
    'title' => t('Debug Info'),
    'content_types' => 'amazon_store_item_debug_info',
    'single' => TRUE,
    'render callback' => 'amazon_store_item_debug_info_render',
    'edit form' => 'amazon_store_item_debug_info_edit_form',
    'icon' => 'icon_amazon.png',
    'description' => t('Debugging data - shows the actual object returned from Amazon and displayed on this page'),
    'required context' => new ctools_context_required(t('Amazon Item'), 'amazon_item'),
    'category' => array(t('Amazon'), -9),
    'no ui' => TRUE,
  );
  return $items;
}


function amazon_store_item_debug_info_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'item_debug_info';
  $amazon_object = $context->data;
  $block->title = 'Debug Info <a href="#" class="togglebtn" >click here</a>';

  $output = "<div id='debug_info'  >
  <div class='item_xml toggle'><pre>". print_r($context->data, TRUE) ."</pre>
  </div>
  </div>";
  $block->content = $output;
  return $block;

}

function amazon_store_item_debug_info_edit_form($form, &$form_state) {

}

