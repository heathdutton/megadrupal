<?php
/**
 * @file
 * 	Contents of amazon_store_search_block
 */
$width = variable_get('amazon_store_search_block_keywords_width', 15);
$form = drupal_get_form('amazon_store_search_form', $width);
print drupal_render($form);
// You could add a link to the cart here.