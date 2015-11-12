<?php

/**
 * @file
 * Views support for pay_node forms.
 */

/**
 * Implements hook_views_data().
 */
function pay_node_views_data() {

  // Add a join route between nodes and pay_forms via the pay_form_node table.
  $data['pay_form_node']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'pay_form' => array(
      'left_field' => 'pfid',
      'field' => 'pfid',
    ),
  );

  // Add a join route between node and pay_form via the pay_form_node table.
  $data['pay_form']['table']['join']['node'] = array(
    'left_table' => 'pay_form_node',
    'left_field' => 'pfid',
    'field' => 'pfid',
  );

  // Add a join route between pay_form and node via the pay_form_node table.
  $data['node']['table']['join']['pay_form'] = array(
    'left_table' => 'pay_form_node',
    'left_field' => 'nid',
    'field' => 'nid',
  );

  // Add a relationship to pay_form base views.
  $data['pay_form']['nid'] = array(
    'group' => t('Payment'),
    'title' => t('Node'),
    'help' => t('The node that contains a payment form.'),
    'table' => 'node',
    'relationship' => array(
      'base' => 'node',
      'relationship table' => 'pay_form_node',
      'relationship field' => 'nid',
      'label' => t('Payment node'),
    ),
  );

  // Add a relationship to node base views.
  $data['node']['pfid'] = array(
    'group' => t('Payment'),
    'title' => t('Payment form'),
    'help' => t('The payment form attached to a node.'),
    'table' => 'pay_form',
    'relationship' => array(
      'base' => 'pay_form',
      'relationship table' => 'pay_form_node',
      'relationship field' => 'pfid',
      'label' => t('Payment form'),
    ),
  );

  // A field that includes a link to the payment form.
  $data['node']['pay_form_link'] = array(
    'real field' => 'nid',
    'group' => t('Payment form'),
    'title' => t('Link to node payment form'),
    'help' => t('A node-specific link for a payment form attached to the node.'),
    'field' => array(
      'handler' => 'pay_node_handler_field_pay_form_link',
    ),
  );

  // Add a join route between nodes and pay_forms via the pay_form_node table.
  $data['pay_transaction_node']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'pay_transaction' => array(
      'left_field' => 'pxid',
      'field' => 'pxid',
    ),
  );

  // Add a join route between node and pay_transaction.
  $data['pay_transaction']['table']['join']['node'] = array(
    'left_table' => 'pay_transaction_node',
    'left_field' => 'nid',
    'field' => 'pxid',
  );

  // Add a join route between pay_transaction and node.
  $data['node']['table']['join']['pay_transaction'] = array(
    'left_table' => 'pay_transaction_node',
    'left_field' => 'pxid',
    'field' => 'nid',
  );

  // Add a relationship to pay_transaction base views.
  $data['pay_transaction']['nid'] = array(
    'group' => t('Payment transaction'),
    'title' => t('Payment node'),
    'help' => t('The node that this transaction was applied to.'),
    'relationship' => array(
      'base' => 'node',
      'relationship table' => 'pay_transaction_node',
      'relationship field' => 'nid',
      'label' => t('Payment node'),
    ),
  );

  // Add a relationship to node base views.
  $data['node']['pxid'] = array(
    'group' => t('Payment'),
    'title' => t('Payment transaction'),
    'help' => t('The payment transaction attached to a node.'),
    'relationship' => array(
      'base' => 'pay_transaction',
      'relationship table' => 'pay_transaction_node',
      'relationship field' => 'pxid',
      'label' => t('Payment transaction'),
    ),
  );

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function pay_node_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'pay_node') . '/includes/views',
    ),
    'handlers' => array(
      'pay_node_handler_field_pay_form_link' => array(
        'parent' => 'views_handler_field_node_link',
      ),
    ),
  );
}
