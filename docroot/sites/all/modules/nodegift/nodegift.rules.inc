<?php

/**
 * nodegift_rules_action_info - implementation of hook_rules_action_info 
 * 
 * @access public
 * @return array
 */
function nodegift_rules_action_info() {
  
  $names = node_type_get_names();
  $giftContentTypes = array();
  foreach ($names as $key => $name) {
    if (nodegift_is_gift_type($key)) {
      $giftContentTypes[] = $key;
    }
  }
  return array(
    'send_gift_to' => array(
      'label' => t('Send gift'),
      'group' => t('Node Gift'),
      'base'  => 'nodegift_rules_send_gift',
      'parameter' => array(
        'sender' => array('type' => 'user', 'label' => t('Sender of the gift')),
        'receiver' => array('type' => 'user', 'label' => t('Receiver of the gift')),
        'node' => array('type' => 'node', 'label' => t('Gift node'), 'bundles' => $giftContentTypes),
        'comment' => array('type' => 'text', 'label' => t('Gift comment')),
      )
    )
  );
}

/**
 * nodegift_rules_send_gift - callback for sent_gift_to action 
 * 
 * @param user $sender 
 * @param user $receiver 
 * @param node $node 
 * @param mixed $comment 
 * @access public
 * @return void
 */
function nodegift_rules_send_gift($sender, $receiver, $node, $comment) {
  nodegift_send($node->nid, $sender->uid, $receiver->uid, $comment);
}
