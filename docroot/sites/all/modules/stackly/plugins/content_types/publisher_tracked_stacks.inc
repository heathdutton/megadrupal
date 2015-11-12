<?php
/**
 * @file
 * Stackly My Publisher Tracked Stacks Widget.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('stackly: My Publisher Tracked Stacks'),
  'description' => t('Widget to track stacks.'),
  'category' => t('stackly'),
  'render callback'  => 'stackly_tracked_stacks_render',
  'edit form'        => 'stackly_tracked_stacks_edit_form',
  'required context' => new ctools_context_optional(t('Current Issue'), 'current_issue'),
);

/**
 * Output function for the 'add_to_stackly' content type.
 */
function stackly_tracked_stacks_render($subtype, $conf, $panel_args, &$context) {
  if (!$conf['stackly_publisher_id']) {
    return FALSE;
  }

  $theme_vars = array();
  $theme_vars['stackly_publisher_id'] = $conf['stackly_publisher_id'];

  $block = new stdClass();
  $block->title = '';

  $block->content = theme('stackly_tracked_stacks', $theme_vars);

  return $block;
}

/**
 * Configuration form.
 */
function stackly_tracked_stacks_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['stackly_publisher_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Publisher id'),
    '#default_value' => ($conf['stackly_publisher_id']) ? $conf['stackly_publisher_id'] : '',
    '#size' => 60,
    '#maxlength' => 128,
  );

  return $form;
}

/**
 * Configuration form submit handler.
 */
function stackly_tracked_stacks_edit_form_submit(&$form, &$form_state) {
  foreach ($form_state['values'] as $key => $value) {
    $form_state['conf'][$key] = $value;
  }
}
