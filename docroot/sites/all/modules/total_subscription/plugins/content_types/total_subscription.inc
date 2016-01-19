<?php
/**
 * @file
 * This is for total_subscription Module.
 * @var array
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Total Subscription'),
  'description' => t('This is for the Subscription'),
  'category' => t('Total Subscription'),
  'edit form' => 'total_subscription_edit_form',
  'render callback' => 'total_subscription_render',
  'required context' => array(
    new ctools_context_optional(t('Node'), 'node'),
    new ctools_context_optional(t('Taxonomy'), 'taxonomy_term'),
  ),
);

/**
 * An edit form for the pane's settings.
 */
function total_subscription_edit_form($form, &$form_state) {
  return $form;
}

/**
 * Run-time rendering of the body of the block (content type).
 */
function total_subscription_render($subtype, $conf, $args, $contexts) {
  $block = new stdClass();
  $form = drupal_get_form('total_subscription_subscribe_form', $contexts);
  $block->content = $form;
  return $block;
}
