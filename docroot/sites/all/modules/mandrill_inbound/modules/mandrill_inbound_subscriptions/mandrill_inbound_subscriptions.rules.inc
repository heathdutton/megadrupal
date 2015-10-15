<?php

/**
 * Implements hook_rules_condition_info().
 *
 * @ingroup rules
 */
function mandrill_inbound_subscriptions_rules_condition_info() {
  return array(
    'mandrill_inbound_subscriptions_able_to_comment' => array(
      'label' => t('Node is able to be commented.'),
      'parameter' => array(
        'nid' => array(
          'type' => 'integer',
          'label' => t('Specify the node ID that should be checked.'),
        ),
      ),
      'group' => t('Node'),
    ),
  );
}

/**
 * Check is a node able be to comment.
 *
 * @param $nid
 *   Which node to check.
 *
 * @return bool
 *   TRUE if the node able to comment, FALSE otherwise.
 */
function mandrill_inbound_subscriptions_able_to_comment($nid) {
  $node = node_load($nid);
  return $node->status && $node->comment == COMMENT_NODE_OPEN;
}
