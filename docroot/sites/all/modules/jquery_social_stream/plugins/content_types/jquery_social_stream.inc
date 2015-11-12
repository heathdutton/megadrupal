<?php

/**
 * @file
 * Council dashboard overview economical impact pane.
 */
$plugin = array(
  'title' => t('jQuery social stream'),
  'content_types' => 'jquery_social_stream',
  'category' => t('Social stream'),
  'render callback' => 'jquery_social_stream_render',
  'edit form' => 'jquery_social_stream_edit_form',
  'all contexts' => TRUE,
  'admin info' => 'jquery_social_stream_admin_info',
);

/**
 * Render callback for the custom content type
 */
function jquery_social_stream_render($subtype, $conf, $args, $context) {
  $block = jquery_social_stream_block_content($conf);
  return (object) $block;
}

/**
 * 'Edit' callback for the content type.
 */
function jquery_social_stream_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form += jquery_social_stream_settings_form($conf);
  return $form;
}

function jquery_social_stream_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  unset($values['return']);
  unset($values['cancel']);
  unset($values['override_title']);
  unset($values['override_title_text']);
  unset($values['form_build_id']);
  unset($values['form_token']);
  unset($values['form_id']);
  unset($values['op']);
  foreach ($values as $key => $value) {
    $form_state['conf'][$key] = $value;
  }
}

/**
 * Callback to provide administrative info (the preview in panels when building
 * a panel).
 *
 * In this case we'll render the content with a dummy argument and
 * a dummy context.
 */
function jquery_social_stream_admin_info($subtype, $conf, $context = NULL) {
  $block = jquery_social_stream_render($subtype, $conf, array(), $context);
  if (!empty($conf['override_title'])) $block->title = $conf['override_title_text'];
  return $block;
}
