<?php

/**
 * @file
 * provides admin pages for Comment Easy Reply Node module.
 */

/**
 * The module's settings form.
 */
function comment_easy_reply_node_settings_form($form, &$form_state) {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment Easy Reply Node settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['settings'] += _comment_easy_reply_node_settings_form($form_state);
  $form['#validate'][] = 'comment_easy_reply_node_settings_form_validate';
  $form['#submit'][] = 'comment_easy_reply_node_settings_form_submit';
  $active_types = _comment_easy_reply_get_active_node_types();
  if (empty($active_types)) {
    drupal_set_message(t('Comment Easy Reply is currently not enabled for any node type. Control node type status on !status_page.',
    array('!status_page' => l(t('status page'), 'admin/config/content/comment-easy-reply/status'))), 'warning');
  }
  return system_settings_form($form);
}

/**
 * Generates the settings form.
 *
 * @param array $form_state
 *   The form values.
 * @param string $node_type
 *   The node type. It is used in node settings form, while in general settings
 *   page, the value is NULL.
 */
function _comment_easy_reply_node_settings_form(&$form_state, $node_type = NULL) {
  $form = array();
  $suffix = '';
  if (!is_null($node_type) && !empty($node_type)) {
    $suffix = '_' . $node_type;
  }
  $form['comment_easy_reply_node_node_type'] = array(
    '#type' => 'hidden',
    '#value' => $node_type,
  );
  $form['node_reply_link'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['node_reply_link']['comment_easy_reply_node_reply_node_link_activated' . $suffix] = array(
    '#type' => 'checkbox',
    '#title' => t("Enable replying to node's author."),
    '#description' => t("If this option is selected, a comment can refer to it's node author."),
    '#default_value' => variable_get('comment_easy_reply_node_reply_node_link_activated' . $suffix, COMMENT_EASY_REPLY_NODE_DEFAULT_REPLY_NODE_LINK_ACTIVATED),
  );
  $entity_info = entity_get_info('node');
  $view_modes = $entity_info['view modes'];
  $displays = array();
  foreach ($view_modes as $name => $value) {
    $displays[$name] = $value['label'];
  }
  $default = variable_get('comment_easy_reply_node_node_display' . $suffix, COMMENT_EASY_REPLY_NODE_DEFAULT_ACTIVE_DISPLAY);
  if (!is_array($default)) {
    $default = array($default);
  }
  $form['node_reply_link']['comment_easy_reply_node_node_display' . $suffix] = array(
    '#type' => 'checkboxes',
    '#title' => t("Select what display must show node reply link."),
    '#options' => $displays,
    '#default_value' => $default,
  );
  $form['node_reply_link']['comment_easy_reply_node_reply_node_link' . $suffix] = array(
    '#type' => 'textfield',
    '#title' => t('Comment node reply link text'),
    '#description' => t("The text of the link inside any node to insert a text with the first comment."),
    '#default_value' => variable_get('comment_easy_reply_node_reply_node_link' . $suffix, COMMENT_EASY_REPLY_NODE_DEFAULT_REPLY_NODE_LINK_TEXT),
  );
  $form['node_reply_link']['comment_easy_reply_node_reply_subject' . $suffix] = array(
    '#type' => 'textfield',
    '#title' => t('Subject of node first comment'),
    '#description' => t("The subject inserted with the first comment."),
    '#default_value' => variable_get('comment_easy_reply_node_reply_subject' . $suffix, COMMENT_EASY_REPLY_NODE_DEFAULT_FIRST_REPLY_SUBJECT),
  );
  return $form;
}

/**
 * Validate callback function for module's settings form.
 */
function comment_easy_reply_node_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $suffix = '';
  if (isset($values['comment_easy_reply_node_node_type']) && !empty($values['comment_easy_reply_node_node_type'])) {
    $suffix = '_' . $values['comment_easy_reply_node_node_type'];
  }
  $link_active = $values['comment_easy_reply_node_reply_node_link_activated' . $suffix];
  if ($link_active) {
    $text = $values['comment_easy_reply_node_reply_node_link' . $suffix];
    if (empty($text)) {
      form_set_error('comment_easy_reply_node_reply_node_link' . $suffix, t('Node reply link text cannot be empty if activated.'));
    }
    $text = $values['comment_easy_reply_node_reply_subject' . $suffix];
    $error = FALSE;
    $messages = _comment_easy_reply_node_validate_node_first_reply_text($text, $error);
    if ($error) {
      foreach ($messages as $msg) {
        form_set_error('comment_easy_reply_node_reply_subject' . $suffix, $msg);
      }
    }
  }
}

/**
 * Submit callback function for module's settings form.
 */
function comment_easy_reply_node_settings_form_submit($form, &$form_state) {
  cache_clear_all('field:comment:', 'cache_field', TRUE);
}

/**
 * Validate a node's first comment default text.
 *
 * @param string $text
 *   The fragment.
 * @param bool &$error
 *   Initiallly FALSE, can be turned to TRUE if fragment is not valid.
 *
 * @return array
 *   An array containing not allowed characters found in fragment.
 */
function _comment_easy_reply_node_validate_node_first_reply_text($text, &$error = FALSE) {
  $match = array();
  $messages = array();
  if (empty($text)) {
    return array();
  }
  $text_tokens = token_scan($text);
  if (!empty($text_tokens)) {
    foreach ($text_tokens as $type => $tokens) {
      $text = str_replace(array_values($tokens), '', $text);
    }
  }
  else {
    $error = TRUE;
    $messages[] = t('At least one token (i.e. !tokens) must be included in Comment text.',
      array('!tokens' => '[node:author]'));
  }
  return $messages;
}
