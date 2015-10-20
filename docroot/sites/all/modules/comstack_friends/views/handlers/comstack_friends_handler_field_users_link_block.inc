<?php

/**
 * @file
 * Contains comstack_friends_handler_field_users_link_block.
 */

class comstack_friends_handler_field_users_link_block extends views_handler_field {
  function construct() {
    parent::construct();

    $this->real_field = 'uid';
    $this->additional_fields['name'] = array('table' => 'users', 'field' => 'name');
  }

  function render($values) {
    global $user;
    $requester_uid = isset($this->view->args[0]) ? $this->view->args[0] : $user->uid;
    $requestee_uid = $this->get_value($values);
    $reqestee_name = $this->get_value($values, 'name');

    if (!$requester_uid) {
      return;
    }

    $variables = array(
      'type' => 'comstack_blocked',
      'requester_uid' => $requester_uid,
      'requestee_uid' => $requestee_uid,
      'requestee_username' => $reqestee_name,
      'classes_array' => array('btn', 'btn-link'),
    );

    return theme('comstack_friends_link', $variables);
  }
}
