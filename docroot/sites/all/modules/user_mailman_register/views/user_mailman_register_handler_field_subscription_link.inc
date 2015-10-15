<?php
class user_mailman_register_handler_field_subscription_link extends views_handler_field_user_link {
  // An example of field level access control.
  function access() {
    return user_access('administer user_mailman_register');
  }
  
  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('Subscriptions');
    $uid = $values->{$this->aliases['uid']};
    return l($text, "user/$uid/user_mailman_register", array('query' => drupal_get_destination()));
  }
}
