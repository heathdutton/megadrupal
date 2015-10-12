<?php


/**
 * @file arguments/asin.inc
 *
 * Plugin to provide an argument for Amazon ASIN
 */
function amazon_store_asin_ctools_arguments() {
  $args['asin'] = array(
    'title' => t("ASIN (Amazon item identifier)"),
    // keyword to use for %substitution
    'keyword' => 'ASIN',
    'description' => t('Creates an Amazon item from the ASIN.'),
    'context' => 'amazon_store_create_asin_context',
    'native path' => AMAZON_STORE_PATH .'/item',
  );
  return $args;
}

function amazon_store_create_asin_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  // If unset it wants a generic, unfilled context.
  if ($empty) {
    return ctools_context_create_empty('amazon_item');
  }
  return ctools_context_create('amazon_item', $arg);
}


