<?php
/**
 * @file
 * Contains ctools plugin for Quora module.
 */

/**
 * Plugin array() of ctools plugin.
 */
$plugin = array(
  'title' => t('Quora'),
  'description' => t('Quora - Related Questions'),
  'single' => TRUE,
  'content_types' => array('quora'),
  'render callback' => 'quora_render',
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'edit form' => 'quora_settings_form',
  'edit form validation' => 'quora_settings_form_submit',
  'category' => 'Widgets',
);

/**
 * Render callback function.
 */
function quora_render($subtype, $conf, $args, $context) {
  $node = $context->data;
  if (empty($node)) {
    return;
  }
  $data = drupal_get_form('quora_form', $context, $conf);
  $block = new stdClass();
  $block->content = $data;
  $block->title = '';
  $block->id = 'quora_form';
  return $block;
}

/**
 * Function returns form to render callback.
 */
function quora_form($form, &$form_state, $context, $conf) {
  // Getting available contexts from menu.
  $context = node_load($context->data->nid);
  return _quora_build_content('ctools', $context, $conf);
}

/**
 * Ctools edit form.
 */
function quora_settings_form($form, &$form_state) {
  $conf = $form_state['conf'];
  _quora_settings_form('ctools', $form, $form_state, $conf);
  return $form;
}

/**
 * Ctools edit form submit handler.
 */
function quora_settings_form_submit(&$form, &$form_state) {
  $form_state['conf']['quora_no_questions'] = $form_state['values']['quora_no_questions'];
  $form_state['conf']['quora_description'] = $form_state['values']['quora_description'];
  $form_state['conf']['quora_description_size'] = $form_state['values']['quora_description_size'];
  $form_state['conf']['quora_search_sensitivity'] = $form_state['values']['quora_search_sensitivity'];
  $form_state['conf']['quora_include'] = $form_state['values']['quora_include'];
  $form_state['conf']['quora_exclude'] = $form_state['values']['quora_exclude'];
}
