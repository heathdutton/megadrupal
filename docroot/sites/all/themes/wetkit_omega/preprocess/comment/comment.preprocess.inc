<?php
/**
 * @file
 * comment.preprocess.inc
 */


/**
 * Implements hook_preprocess_comment().
 */
function wetkit_omega_preprocess_comment(&$variables) {
  $variables['header_attributes_array']['class'][] = 'node-header';
  $variables['footer_attributes_array']['class'][] = 'submitted';
  if ($variables['user_picture']) {
    $variables['footer_attributes_array']['class'][] = 'with-user-picture';
  }
  $variables['links_attributes_array']['class'][] = 'clearfix';
}
