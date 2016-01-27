<?php


class injapi_InjectedAPI_hookSchema_Table extends injapi_InjectedAPI_Abstract_Item {

  protected $descriptions = array();

  //                                                                        Meta
  // ---------------------------------------------------------------------------

  function index($name, $columns) {
    $this->data['indexes'][$name] = $columns;
  }

  function uniqueKey($name, $columns) {
    $this->data['unique keys'][$name] = $columns;
  }

  function foreignKey($name, $table, $columns) {
    $this->data['foreign keys'][$name] = array(
      'table' => $table,
      'columns' => $columns,
    );
  }

  function primaryKey($columns) {
    $this->data['primary key'] = $columns;
  }

  function describeFields($descriptions) {
    $this->descriptions = $descriptions;
    foreach ($descriptions as $field => $description) {
      if (isset($this->data['fields'][$field])) {
        $this->data['fields'][$field]['description'] = $description;
      }
    }
  }

  //                                                           Basic field types
  // ---------------------------------------------------------------------------

  /**
   * Add a field with type and info array.
   */
  function field($name, $type, $info = array()) {
    if (isset($this->descriptions[$name]) && !isset($info['description'])) {
      $info['description'] = $this->descriptions[$name];
    }
    $this->data['fields'][$name] = $info + array(
      'not null' => FALSE,
      'type' => $type,
    );
    return new injapi_InjectedAPI_hookSchema_Field($name, $this->data['fields'][$name]);
  }

  function field_int($name, $size = NULL, $unsigned = FALSE) {
    $info = isset($size) ? array('size' => $size) : array();
    return $this->field($name, 'int', $info + array(
      'unsigned' => $unsigned,
    ));
  }

  function field_float($name, $unsigned = FALSE) {
    return $this->field($name, 'float', array(
      'unsigned' => $unsigned,
    ));
  }

  function field_numeric($name, $precision, $scale, $unsigned = FALSE) {
    return $this->field($name, 'numeric', array(
      'precision' => $precision,
      'scale' => $scale,
      'unsigned' => $unsigned,
    ));
  }

  function field_varchar($name, $length = 255) {
    return $this->field($name, 'varchar', array(
      'length' => $length,
    ));
  }

  function field_text($name, $length = NULL) {
    return $this->field($name, 'text', array(
      'length' => $length,
    ));
  }

  function field_blob($name) {
    return $this->field($name, 'blob');
  }

  function field_datetime($name) {
    return $this->field($name, 'datetime');
  }

  //                                                          Derived types: int
  // ---------------------------------------------------------------------------

  /**
   * Add an auto-increment not-null unsigned integer column,
   * and make it the primary key.
   */
  function field_id($name) {
    $this->data += array(
      'primary key' => array($name),
    );
    return $this->field($name, 'serial', array(
      'unsigned' => TRUE,
      'not null' => TRUE,
    ));
  }

  /**
   * Add an unsigned integer field,
   * and register it as a reference to a foreign table.
   */
  function field_idForeign($name, $foreign_table = NULL, $foreign_column = NULL, $key_name = NULL) {
    if (isset($key_name)) {
      $this->data['foreign keys'][$key_name] = array(
        'table' => $foreign_table,
        'columns' => array(
          $name => $foreign_column,
        ),
      );
    }
    return $this->field($name, 'int', array(
      'unsigned' => TRUE,
    ));
  }

  function field_signed($name, $size = NULL) {
    $info = isset($size) ? array('size' => $size) : array();
    return $this->field($name, 'int', $info + array(
      'unsigned' => FALSE,
    ));
  }

  function field_unsigned($name, $size = NULL) {
    $info = isset($size) ? array('size' => $size) : array();
    return $this->field($name, 'int', $info + array(
      'unsigned' => TRUE,
    ));
  }

  /**
   * Typical type to use for fields like 'created' or 'updated'.
   */
  function field_seconds($name) {
    return $this->field_unsigned($name);
  }

  //                                                        Derived types: float
  // ---------------------------------------------------------------------------

  function field_unsignedFloat($name) {
    return $this->field($name, 'float', array(
      'unsigned' => TRUE,
    ));
  }

  //                                                      Derived types: numeric
  // ---------------------------------------------------------------------------

  function field_unsignedNumeric($name, $precision, $scale) {
    return $this->field($name, 'numeric', array(
      'unsigned' => TRUE,
    ));
  }

  //                                                      Derived types: varchar
  // ---------------------------------------------------------------------------

  /**
   * Add a varchar field to be used as primary key, and make it required.
   */
  function field_stringId($name, $length = 255) {
    $this->data += array(
      'primary key' => array($name),
    );
    return $this->field($name, 'varchar', array(
      'length' => $length,
      'not null' => TRUE,
    ));
  }

  /**
   * Add an varchar field,
   * and register it as a reference to a foreign table.
   */
  function field_stringIdForeign($name, $length = 255, $foreign_table = NULL, $foreign_column = NULL, $key_name = NULL) {
    if (isset($key_name)) {
      $this->data['foreign keys'][$key_name] = array(
        'table' => $foreign_table,
        'columns' => array(
          $name => $foreign_column,
        ),
      );
    }
    return $this->field($name, 'varchar', array(
      'length' => $length,
    ));
  }

  function field_varcharSerialized($name, $length = 255) {
    return $this->field($name, 'varchar', array(
      'length' => $length,
      'serialize' => TRUE,
    ));
  }

  //                                                         Derived types: text
  // ---------------------------------------------------------------------------

  function field_serialized($name) {
    return $this->field($name, 'text', array(
      'serialize' => TRUE,
    ));
  }
}
