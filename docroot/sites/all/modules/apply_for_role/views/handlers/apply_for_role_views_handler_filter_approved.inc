<?php

/**
 * @file
 * Definition of views_handler_filter_user_roles.
 */

/**
 * Filter handler for user roles.
 *
 * @ingroup views_filter_handlers
 */
class apply_for_role_views_handler_filter_approved extends views_handler_filter_in_operator {
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
     }
    $this->value_options = array(
      APPLY_FOR_ROLE_REMOVED => t('Removed'),
      APPLY_FOR_ROLE_PENDING => t('Pending'),
      APPLY_FOR_ROLE_APPROVED => t('Approved'),
      APPLY_FOR_ROLE_DENIED => t('Denied'),
     );
  }
}
