<?php

/**
 * @file
 * Contains the Achievements list CTools content-type plugin.
 */

$plugin = array(
  'title' => t('Achievements'),
  'description' => '',
  'category' => 'Achiever',
  'no title override' => TRUE,
  'required context' => new ctools_context_required(t('User'), 'user'),
  'defaults' => array(),
);

/**
 * Implements hook_plugin_content_type_render().
 */
function achiever_achievements_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return null;
  }

  $account = $context->data;
  $achievements = achiever_get_achievements();
  $content = theme('achiever_achievements', array('achievements' => $achievements, 'account' => $account));

  // Build the content type block.
  $block = new stdClass();
  $block->module = '';
  $block->delta = '';
  $block->title = '';
  $block->content = $content;

  return $block;
}

/**
 * Implements hook_plugin_content_type_edit_form().
 */
function achiever_achievements_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}
