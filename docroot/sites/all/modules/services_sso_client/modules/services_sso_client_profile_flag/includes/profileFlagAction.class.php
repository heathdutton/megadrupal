<?php

/**
 * @file
 * ProfileFlagAction
 */

class ProfileFlagAction {
  public $type = ''; // String for the type of the flag
  public $faid = 0;
  public $fid = 0;
  public $options = array();

  private $namespace = '';

  public static $child_classes = array();

  /**
   * Constructor.
   *
   * @param $obj
   *  A mixed type variable. The constructor will take care of figuring out how to create a ProfileFlagAction object from this.
   */
  function __construct($obj = array()) {
    if (is_object($obj)) {
      // Iterate through the returned record and populate as much as possible.
      foreach ($obj as $key => $val) {
        if (isset($this->$key)) {
          // Take care when $obj->data is serialized
          if ($key == 'options' && is_string($val)) {
            $this->$key = unserialize($val);
          }
          else {
            $this->$key = $val;
          }
        }
      }
    }

    return $this;
  }

  function save($weight = 0) {
    $record = array(
      'type' => $this->type,
      'fid' => $this->fid,
      'options' => serialize($this->options),
      'weight' => $weight,
    );

    if (!empty($this->faid)) {
      $record['faid'] = $this->faid;
      $result = drupal_write_record('services_sso_client_profile_flag_action', $record, 'faid');
    }
    else {
      $result = drupal_write_record('services_sso_client_profile_flag_action', $record);
    }

    $record = (object) $record;

    switch ($result) {
      case SAVED_NEW:
        $this->faid = $record->faid;
        break;
      case SAVED_UPDATED:
        break;
      case FALSE:

        break;
    }

    return $result;
  }

  /**
   * Delete the corresponding record from the database.
   *
   * @return
   *  Return TRUE if deletion worked all the way through.
   */
  function delete() {
    if (!empty($this->faid)) {
      if ($num_deleted = db_delete('services_sso_client_profile_flag_action')->condition('faid', $this->faid)->execute()) {
        // Reset the faid.
        $this->faid = 0;
        return TRUE;
      }
    }

    return FALSE;
  }

  public function actionNamespace() {
    return '';
  }

  function actionable() {
    $namespace = $this->actionNamespace();

    if (!empty($namespace)) {
      return str_ireplace($namespace . '_', '', $this->type);
    }
    return $this->type;
  }

  function apply($uid = 0) {

  }

  /**
   * Will map a ProfileFlagAction object to a inherited sub-class.
   *
   * This function will use the $this->type string member variable, parse out the namcespace, which is
   *  interpreted as the first portion of $this->type before the first occurance of "_" (underscore).
   *  if the namespace corresponding to the sub-class is declared via actionNamespace(), see loadActionPlugins(),
   *  the same object, albeit initialized as the appropriate sub-class will be returned by map(). It is advisable
   *  that map() always be called after every new ProfileFlagAction call on assignment.
   */
  function map() {
    if (empty(self::$child_classes)) {
      self::loadActionPlugins();
    }

    if (!empty($this->type)) {
      $namespace = explode('_', $this->type);
      $namespace = current($namespace);

      if ($key = array_search($namespace, self::$child_classes)) {
        if (!empty($key)) {
          $new_obj = (object) array(
            'type' => $this->type,
            'fid' => $this->fid,
            'options' => $this->options,
            'faid' => $this->faid,
          );
          $new_obj = new $key($new_obj);
          return $new_obj;
        }
      }
    }

    return $this;
  }

  /**
   * Load all sub-classes in the plugins dir.
   *
   * @todo The performance ramifications from using file_scan_directory() needs to be explored.
   */
  public static function loadActionPlugins() {
    $mask = '/class.php$/';
    $dir = drupal_get_path('module', 'services_sso_client_profile_flag') . '/plugins';
    $files = file_scan_directory($dir, $mask);

    foreach ($files as $path => $file) {
      require_once($path);

      $class_name = str_ireplace('.class', '', $file->name);
      self::$child_classes[$class_name] = $class_name::actionNamespace();
    }
  }
}