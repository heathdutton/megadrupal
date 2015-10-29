<?php
/**
 * @file
 * Panels specific include file for implementing Gamertags in Panels.
 */

/**
 * Implements hook_ctools_content_types().
 */
function gamertags_gamertags_ctools_content_types() {
  return array(
    'single' => TRUE,
    'title' => t('Gamertags'),
    'icon' => 'icon_user.png',
    'description' => t('Gamertags from the Gamertags module.'),
    'required context' => new ctools_context_required(t('User'), 'user'),
    'category' => t('User'),
  );
}

/**
 * Callback for admin_title.
 */
function gamertags_gamertags_content_type_admin_title($subtype, $conf, $context = NULL) {
  $output = t('Gamertags');
  if ($conf['override_title'] && !empty($conf['override_title_text'])) {
    $output = filter_xss_admin($conf['override_title_text']);
  }
  return $output;
}

/**
 * Callback for admin_info.
 */
function gamertags_gamertags_content_type_admin_info($subtype, $conf, $context = NULL) {
  $block = new stdClass();
  $block->title = !empty($conf['title']) ? check_plain($conf['title']) : '';
  if (!empty($context->data->description)) {
    $block->content = check_plain($context->data->description);
  }
  return $block;
}

/**
 * Callback for edit_form so we pick up %user argument.
 */
function gamertags_gamertags_content_type_edit_form(&$form, &$form_state) {
  $form['gamertags_as_text'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show gamertag as text only'),
    '#description' => t('Only show the user input text for their gamertag ID, not their actual gamercard.'),
    '#default_value' => !empty($form_state['conf']['gamertags_as_text']) ? $form_state['conf']['gamertags_as_text'] : FALSE,
  );
  return $form;
}

/**
 * Submit callback for edit_form, saves 'gamertags_as_text' config into $conf array.
 */
function gamertags_gamertags_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['gamertags_as_text'] = $form_state['values']['gamertags_as_text'];
}

/**
 * Callback which renders our Gamertags.
 */
function gamertags_gamertags_content_type_render($subtype, $conf, $panel_args, $context) {
  $account = isset($context->data) ? clone $context->data : NULL;
  $block = new stdClass();
  $block->title = t('Gamertags');
  $block->content = $account ? theme('gamertags_all', array('account' => $account, 'as_text' => $conf['gamertags_as_text'])) : t('User information not available');
  return $block;
}
