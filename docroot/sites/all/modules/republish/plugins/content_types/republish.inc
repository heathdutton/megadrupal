<?php
/**
 * @file
 * Contains ctools plugin for Republish module.
 */

/**
 * Plugin array() of ctools plugin.
 */
$plugin = array(
  'title' => t('Republish'),
  'description' => t('Republish the content'),
  'single' => TRUE,
  'content_types' => array('republish'),
  'render callback' => 'republish_render',
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'edit form' => 'republish_settings_form',
  'edit form validation' => 'republish_settings_form_submit',
  'category' => 'Widgets',
);

/**
 * Render callback function.
 */
function republish_render($subtype, $conf, $args, $context) {
  $node = $context->data;
  if (empty($node)) {
    return;
  }
  $data = drupal_get_form('republish_form', $context, $conf);
  $block = new stdClass();
  $block->content = $data;
  $block->title = '';
  $block->id = 'republish_form';
  return $block;
}

/**
 * Function returns form to render callback.
 */
function republish_form($form, &$form_state, $context, $conf) {
  // Getting available contexts from menu.
  $context = node_load($context->data->nid);
  _republish_content_form('ctools', $context, $form, $form_state, $conf);
  return $form;
}

/**
 * Ctools edit form.
 */
function republish_settings_form($form, &$form_state) {
  $conf = $form_state['conf'];
  _republish_settings_form('ctools', $form, $form_state, $conf);
  return $form;
}

/**
 * Ctools edit form submit handler.
 */
function republish_settings_form_submit(&$form, &$form_state) {
  $form_state['conf']['link_text'] = $form_state['values']['republish_link_text'];
  $form_state['conf']['title'] = $form_state['values']['republish_title'];
  $form_state['conf']['guidelines'] = $form_state['values']['republish_guidelines'];
  $form_state['conf']['instructions'] = $form_state['values']['republish_instructions'];
  $form_state['conf']['body'] = $form_state['values']['republish_body'];
  $form_state['conf']['body_readonly'] = $form_state['values']['republish_body_readonly'];
  $form_state['conf']['branding'] = $form_state['values']['republish_branding'];
  $form_state['conf']['republish_link_css_class'] = $form_state['values']['republish_link_css_class'];
}
