<?php

/**
 * Field handler to present an 'accept' link for a given claim.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_nodeownership_status extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);
    $status_map = array(
      0 => t('Pending'),
      1 => t('Approved'),
      2 => t('Declined'),
    );

    return $status_map[$value];
  }
}

