<?php
/**
 * Field handler to provide a message.
 */
class apply_for_role_views_handler_field_message extends views_handler_field_xss {
  function pre_query() {
    if (!variable_get('apply_for_role_allow_message', 0)) {
      unset($this->view->field[$this->field]);
    }
  }
}
