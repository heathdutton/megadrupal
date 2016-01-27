<?php


/**
 * Implements hook_rules_action_info().
 */
function mass_messaging_rules_action_info() {
  $actions = array();

  $actions['mass_messaging_action_send_mails'] = array(
    'label' => t('Send Mass Mails'),
    'parameter' => array(
      'view' => array(
        'type' => 'text',
        'label' => t('View'),
        'options list' => 'mass_messaging_view_list',
        'description' => t('You need to enter the View and the Display to use. Use the ids of them and separate them by a ":".'),
      ),
      'arguments' => array(
        'type' => 'text',
        'label' => t('Views Arguments'),
        'optional' => TRUE,
        'description' => t('Place on each line one argument.')
      ),
    ),
    'group' => t('Mass Messaging'),
    'base' => 'mass_messaging_action_send_mails',
    'callbacks' => array(
      'execute' => 'mass_messaging_action_send_mails',
    ),
  );  
  return $actions;
}


function mass_messaging_action_send_mails($view, $arguments = NULL) {
  $splits = explode(':', $view);
  $view_id = $splits[0];
  $display_id = $splits[1];
  $args = explode("\n", $arguments);
  foreach($args as $id => $arg) {
    $args[$id] = trim($arg);
  }
  mass_messaging_send_messages($view_id, $display_id, $args);  
}
