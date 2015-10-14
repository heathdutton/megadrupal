<?php

/**
 * @file
 * Definition of CommentAbuseHandlerFieldUserIp.
 */

/**
 * @ingroup views_field_handlers
 */
class CommentAbuseHandlerFieldUserIp extends views_handler_field {

  /**
   * Construct a new field handler.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['user_ip'] = array('table' => 'comment_abuse', 'field' => 'ip');
  }

  /**
   * Render the field.
   */
  function render($values) {
    $value = $values->{$this->field_alias};
    return long2ip($value);
  }
}
