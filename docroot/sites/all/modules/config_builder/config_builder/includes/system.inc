<?php
/**
 * @file
 * System module integration.
 */

/**
 * Implements hook_form_alter().
 */
function config_builder_form_alter(&$form, &$form_state, $form_id) {
  drupal_alter('config_builder_form', $form, $form_state, $form_id);
  drupal_alter("config_builder_form_{$form_id}", $form, $form_state);
}

/**
 * Implements hook_token_info_alter().
 */
function config_builder_token_info_alter(&$info) {
  drupal_alter('config_builder_token_info', $info);
}

/**
 * Implements hook_tokens_alter().
 */
function config_builder_tokens_alter(&$replacements, $context) {
  drupal_alter('config_builder_tokens', $replacements, $context);
}
