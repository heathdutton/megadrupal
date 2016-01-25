<?php
namespace devtools\components;

class DisplaySuite extends \devtools\patterns\Singleton {
  protected static $_instance;
  protected static $_fields = array();

  /**
   * Adds a field.
   * @param [type] $entity_type [description]
   * @param [type] $data        [description]
   */
  public function addField($entity_type, $data) {
    $required = array(
      'path', 'file',
    );

    foreach ($required as $key) {
      if (!isset($data[$key])) {
        throw new \Exception(t('Field !name missing required key !key', array('!name' => $data['name'], '!key' => $key)));
      }
    }

    static::$_fields[$entity_type][$data['key']] = $data; 
  }

  /**
   * Removes a field.
   * @param  [type] $entity_type [description]
   * @param  [type] $key         [description]
   * @return [type]              [description]
   */
  public function removeField($entity_type, $key) {
    unset(static::$_fields[$entity_type][$key]);
  }

  /**
   * Returns field data.
   * @param  [type] $entity_type [description]
   * @return [type]              [description]
   */
  public function getFields($entity_type = NULL) {
    if (!is_null($entity_type)) {
      if (isset(static::$_fields[$entity_type])) {
        return static::$_fields[$entity_type];
      }
      else {
        return FALSE;
      }
    }

    if (!isset(static::$_fields)) {
      static::$_fields = array();
    }

    return static::$_fields;
  }
}