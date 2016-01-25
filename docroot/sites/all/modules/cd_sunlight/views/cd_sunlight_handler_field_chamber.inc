<?php

/**
 * Field handler to expand the chamber abbreviations.
 *
 * @ingroup views_field_handlers
 */
class cd_sunlight_handler_field_chamber extends views_handler_field {

  /**
   * Add all the fields that we need.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['title'] = 'title';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    if (empty($values->{$this->aliases['title']})) {
      return '';
    }

    $title = $values->{$this->aliases['title']};
    if ($title == 'Sen') {
      return 'Senate';
    }
    else {
      return 'House';
    }

  }

}
