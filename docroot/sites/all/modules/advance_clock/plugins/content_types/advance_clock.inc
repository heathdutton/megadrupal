<?php
/**
 * @file
 * sentizens clock per user clock configuration handler
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */

$plugin = array(
  'title' => t('Advance clock'),
  'description' => t('Plugin for advance clock'),
  'render callback' => 'advance_clock_content_type_render',
  'edit form' => 'advance_clock_content_type_edit_form',
  'icon' => 'icon_example.png',
  'category' => array(t('Advance Clocks'), -9),
);

/**
 * Run-time rendering of the body of the block.
 */
function advance_clock_content_type_render($subtype, $conf, $args, $context) {

  global $user;
  if (module_exists('advance_clock')) {
    $clocks = _advance_clock_clock_list();
  }
  $block = new stdClass();
  $block->title   = '';
  $block->content = "<div class='advance-clock-wrapper'>";
  $block->content .= $clocks;
  $block->content .= "</div>";

  // Modal window - Advance clock configuration.
  $link_out =  l('Configure Clocks', 'user/'. $user->uid . '/add-clock');

  if ($conf['advance_clock_use_modal'] == 1) {
    $mini_out = drupal_render(drupal_get_form('advance_clock_add_clock_form'));
    drupal_add_library('system', 'ui.dialog');
    $block->content .= "<div class='minipanel-dialog-wrapper'>";
    $block->content .= "<div class='minipanel-dialog-link-link'>" . $link_out . "</div>";
    $block->content .= "<div class='minipanel-dialog-link-mini' style='display:none; overflow:auto; height:640px;'>" . $mini_out . "</div>";
    $block->content .= "</div>";
  }

  else {
    $block->content .= "<div class='advance-clock-configuration'>" . $link_out . "</div>";
  }

  return $block;
}

/**
 * Implements hook_form().
 */
function advance_clock_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['advance_clock_use_modal'] = array(
    '#title' => t('Display Configure clock in modal window'),
    '#type' => 'checkbox',
    '#description' => t('Use Modal Window to display location'),
    '#default_value' => !empty($conf['advance_clock_use_modal']) ? $conf['advance_clock_use_modal'] : '0',
  );

  return $form;
}

/**
 * Implements hook_form_submit().
 */
function advance_clock_content_type_edit_form_submit(&$form, &$form_state) {
    $form_state['conf'] = $form_state['values'];
}
