<?php

/**
 * @file
 * Returns a content type with
 * Facebook total + plus one count + tweets count + linkedin shares number
 * of facebook shares of the node. Only available on on node view i.e. with
 * required context node being viewed
 */

$plugin = array(
  'title' => t('Total shares across social networks'),
  'content_types' => 'total',
  'single' => TRUE,
  'category' => array('Social Stats'),
  'edit form' => 'social_stats_total_form_edit',
  'render callback' => 'social_stats_total_form_render',
  'required context' => array(new ctools_context_required(t('Node'), 'node')),
);

/**
 * Function to render the share count.
 */
function social_stats_total_form_render($subtype, $conf, $args, &$context) {
  $block = new stdClass();
  $query = db_query('SELECT s.total_virality FROM {social_stats_total} s WHERE s.nid = :current_node_id', array(':current_node_id' => $context[0]->data->nid))->fetchAssoc();
  if (isset($query['total_virality'])) {
    $block->content = format_plural($query['total_virality'], $conf['singular'], $conf['plural']);
  }
  else {
    $block->content = t('0 Shares');
  }
  return $block;
}

/**
 * Function to render the Share text configuration form.
 */
function social_stats_total_form_edit($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['social_stats'] = array(
    '#type' => 'fieldset',
    '#title' => t('Total Shares'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['social_stats']['total_shares_singular'] = array(
    '#type' => 'textfield',
    '#title' => t('Share Text - Singular'),
    '#description' => t('This text will appear when there the node is shared once'),
    '#default_value' => '1 Share',
    '#required' => TRUE,
  );
  $form['social_stats']['total_shares_plural'] = array(
    '#type' => 'textfield',
    '#title' => t('Share Text - Plural'),
    '#description' => t('This text will appear when there the node is shared more than once. @count will be replaced by the actual share numbers'),
    '#default_value' => '@count Shares',
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Validate function to the Share text configuration form.
 */
function social_stats_total_form_edit_validate($form, &$form_state) {
  if (!(strstr($form_state['values']['total_shares_plural'] , "@count"))) {
    form_set_error('total_shares_plural', '"Share Text - Plural" must contain the word "@count".');
  }
}

/**
 * Submit function to the Share text configuration form.
 */
function social_stats_total_form_edit_submit($form, &$form_state) {
  $form_state['conf']['singular'] = $form_state['values']['total_shares_singular'];
  $form_state['conf']['plural'] = $form_state['values']['total_shares_plural'];
}
