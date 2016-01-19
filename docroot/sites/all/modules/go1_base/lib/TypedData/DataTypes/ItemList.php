<?php
namespace Drupal\go1_base\TypedData\DataTypes;

class ItemList extends Base {
  /**
   * @var string
   */
  protected $element_type = NULL;

  public function isEmpty() {
    if (!is_null($this->value)) {
      return is_empty($this->value);
    }
  }

  public function setDef($def) {
    $this->def = $def;

    if (!empty($def['element_type'])) {
      $this->element_type = $def['element_type'];
    }

    return $this;
  }

  public function validateInput(&$error = NULL) {
    if (!is_array($this->value)) {
      $error = 'Input must be an array.';
      return FALSE;
    }

    if (!is_null($this->element_type)) {
      $this->validateElementType($error);
      if (!empty($error)) {
        return FALSE;
      }
    }

    return parent::validateInput($error);
  }

  private function validateElementType(&$error = NULL) {
    $data = go1_data(array('type' => $this->element_type));

    foreach ($this->value as $k => $v) {
      $data->setValue($v);
      if (!$data->validate()) {
        $error = "Element <strong>{$k}</strong> is not type of {$this->element_type}";
        return FALSE;
      }
    }

    return TRUE;
  }
}
