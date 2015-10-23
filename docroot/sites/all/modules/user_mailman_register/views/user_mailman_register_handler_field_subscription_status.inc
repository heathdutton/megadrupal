<?php
class user_mailman_register_handler_field_subscription_status extends views_handler_field {
  function render($values) {
    return _user_mailman_register_t_status($values->{$this->field_alias});
  }
}