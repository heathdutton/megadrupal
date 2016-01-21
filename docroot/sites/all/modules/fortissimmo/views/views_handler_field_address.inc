<?php

/**
 * @file
 * Views handler to show an address field, if it's allowed to be shown
 */
class views_handler_field_address extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields['address_visible'] = array('table' => $view->base_table, 'field' => 'address_visible');
  }

  function render($values) {
    if (isset($this->aliases['address_visible'])) {
      $address_visible = $values->{$this->aliases['address_visible']};
      if ($address_visible) {
        return $values->{$this->field_alias};
      }
    }
    return NULL;
  }
}