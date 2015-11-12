<?php

/**
 * @file
 * Contains comstack_pm_ui_handler_field_users_link_message.
 */

class comstack_pm_ui_handler_field_users_link_message extends views_handler_field {
  function construct() {
    parent::construct();

    $this->real_field = 'uid';
    $this->additional_fields['name'] = array('table' => 'users', 'field' => 'name');
  }

  function render($values) {
    global $user;
    $account_uid = $this->get_value($values);
    $account_name = $this->get_value($values, 'name');
    $user_uid = isset($this->view->args[0]) ? $this->view->args[0] : $user->uid;

    $account = (object) array('uid' => $account_uid, 'name' => $account_name);
    $user = (object) array('uid' => $user_uid);

    if (!$account) {
      return;
    }

    return theme('comstack_pm_ui_message_button', array('account' => $account, 'user' => $user));
  }
}
