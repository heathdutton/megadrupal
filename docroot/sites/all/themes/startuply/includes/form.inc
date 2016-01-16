<?php
/**
 * @file
 * Theme and preprocess functions for forms
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function startuply_form_search_form_alter(&$form, &$form_state) {
  $form['basic']['submit']['#button_icon'] = 'search';
}
