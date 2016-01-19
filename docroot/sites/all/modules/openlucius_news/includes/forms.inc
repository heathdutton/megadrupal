<?php
/**
 * @file
 * This file contains all forms for the news.
 */

/**
 * Implements hook_form_comment_node_ol_news_form_alter().
 */
function openlucius_news_form_comment_node_ol_news_form_alter(&$form, &$form_state, $form_id) {
  if (isset($form['field_wrapper'])) {
    unset($form['field_wrapper']);
  }
}

/**
 * Implements hook_form_ol_news_node_form_alter().
 */
function openlucius_news_form_ol_news_node_form_alter(&$form, &$form_state, $form_id) {

  // Add extra form submit for redirecting.
  $form['actions']['submit']['#submit'][] = 'openlucius_news_custom_save';

  if (user_access('promote news')) {
    // Allow users to promote content.
    $form['options']['#access'] = TRUE;
    $form['options']['promote']['#access'] = TRUE;
    $form['options']['sticky']['#access'] = FALSE;
  }

  // Check the node form.
  if ($form['#node'] && $form['#node']->type . '_node_form' === $form_id) {
    if ($form['#node']->nid) {
      $form['options']['status']['#access'] = FALSE;
      $form['actions']['preview_changes']['#access'] = FALSE;
    }
  }
}

/**
 * Implements hook_node_submit().
 */
function openlucius_news_custom_save($form, &$form_state) {
  if ($form_state['values']['type'] == 'ol_news') {
    drupal_goto('recent-stuff', array(
      'fragment' => 'news',
    ));
  }
}

/**
 * Form constructor for the news configuration.
 */
function openlucius_news_configuration_form() {

  $form = array();

  $form['openlucius_news_news_limit'] = array(
    '#title' => t('News limit'),
    '#description' => t('The amount of news items to show on each page'),
    '#type' => 'textfield',
    '#default_value' => variable_get('openlucius_news_news_limit', 20),
  );

  return system_settings_form($form);
}
