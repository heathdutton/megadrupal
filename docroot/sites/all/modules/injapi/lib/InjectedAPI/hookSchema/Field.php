<?php


class injapi_InjectedAPI_hookSchema_Field extends injapi_InjectedAPI_Abstract_Item {

  /**
   * Set the field's required status, and optionally set a default value.
   *
   * @param boolean $required
   *   If TRUE, the field cannot take NULL values.
   * @param (misc) $default
   *   The default value. If NULL, no default value will be set.
   *
   * @return
   *   The $this pointer.
   */
  function required($required = TRUE, $default = NULL) {
    if (isset($default)) {
      $this->data['default'] = $default; 
    }
    else {
      unset($this->data['default']);
    }
    $this->data['not null'] = $required;
    return $this;
  }

  /**
   * Set a field description.
   *
   * @param string $description
   *   The description for the table column.
   *
   * @return
   *   The $this pointer.
   */
  function description($description) {
    $this->data['description'] = $description;
    return $this;
  }
}
