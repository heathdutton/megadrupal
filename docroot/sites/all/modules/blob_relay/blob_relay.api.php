<?php

/**
 * @file
 *
 * This file describes how to use bolb_relay in your own code.
 */

/**
 * You need to add the blob_relay_input form
 * to pages of your need.
 * @return type
 */
function my_page_form_needing_a_blob_download() {
  $blob_relay = "";
  if (function_exists('blob_relay_input')) {
    $blob_relay = drupal_get_form('blob_relay_input');
  }
  $form = drupal_get_form('my_page_callback_needing_a_blob_form');
  return drupal_render($form)
    . drupal_render($blob_relay);
  
}

/**
 * You need to add glue code to trigger the form values and submit.
 *
 * You may submit ie some svg xml which is probably base64 encoded.
 *
 * @code
 * Drupal.blobRelay.FillForm({
 *   encoding:'base64',
 *   data:'SVG DATA',
 *   mimetype:'image/svg+xml',
 *   filename: 'image.svg'
 * });
 * @code
 *
 * @return string JavaScript
 */
function my_pages_needing_blob_glue_js_code() {
  return <<< EOL
Drupal.blobRelay.FillForm({
  mimetype: 'text/test',
  data: 'Text file test content',
  encoding: '',
  filename: 'text.txt'
});
EOL;
}
