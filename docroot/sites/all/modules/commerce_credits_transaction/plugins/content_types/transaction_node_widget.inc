<?php
/**
 * @file
 * Commerce Credits Transaction Widget for Node.
 */


/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('Commerce Credits Transaction Node Widget'),
  'description' => t('Add the credit transaction widget to nodes for users to pledge credits to nodes.'),
  'single' => TRUE,
  'render callback' => 'commerce_credits_transaction_node_widget',
  'edit form' => 'commerce_credits_transaction_node_widget_form',
  'defaults' => array(
    'override_title' => FALSE,
    'override_title_text' => '',
    'credit_type' => 'credit',
  ),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'category' => t('Node'),
);

/**
 * Commerce Credits Transaction Node Widget.
 */
function commerce_credits_transaction_node_widget($subtype, $conf, $args, $context) {

  if (isset($context->data)) {
    $node = clone $context->data;
  }
  else {
    return FALSE;
  }

  if (!isset($node)) {
    return FALSE;
  }

  ctools_include('ajax');
  ctools_include('modal');
  ctools_modal_add_js();
  drupal_add_js(drupal_get_path('module', 'commerce_credits_transaction') . '/js/message.js', 'file');

  $block = new StdClass();
  $block->title = $conf['override_title'] ? check_plain($conf['override_title_text']) : '';
  $block->content = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('commerce-credits-transaction-widget'),
    ),
  );

  if (commerce_credits_transaction_page_access('node', $node->nid)) {
    $link_text = t('Pledge Credits');
    $link_path = 'credits/transaction/' . $conf['credit_type'] . '/node/' . $node->nid . '/nojs';
    $options = array(
      'html' => FALSE,
      'attributes' => array(
        'class' => array('ctools-use-modal', 'commerce-credits-transaction-widget'),
      ),
    );
    $block->content['link'] = array(
      '#theme' => 'link',
      '#text' => $link_text,
      '#path' => $link_path,
      '#options' => $options,
    );
  }

  return $block;
}

/**
 * Edit form
 */
function commerce_credits_transaction_node_widget_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['credit_type'] = array(
    '#type' => 'radios',
    '#title' => t('Credit type'),
    '#description' => t('Choose the credit type to use for this widget.'),
    '#options' => commerce_credits_types_list(),
    '#default_value' => $conf['credit_type'],
    '#required' => TRUE,
  );

  return $form;
}

function commerce_credits_transaction_node_widget_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function commerce_credits_transaction_node_widget_admin_title($subtype, $conf, $context) {
  $type = commerce_credits_get_group_name($conf['credit_type']);
  $type_name = array_values($type);

  return t('Commerce Credits @type Transaction Widget', array('@type' => $type_name));
}
