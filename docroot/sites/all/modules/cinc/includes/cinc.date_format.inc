<?php

/**
 * Creates date formats in code.
 *
 * @code
 * $my_date_format = CINC::init('DateFormat')->set('format', 'my_date_format');
 * @endcode
 */
class CINC_DateFormat extends CINC implements CINC_CRUD {

  public $config_machine_name = 'format';
  public $cinc_yaml_prefix = 'system.date.format';
  public static $formats = array();

  function __construct() {

    $this->config = array(
      'type' => 'custom',
      'format' => '',
      'locked' => FALSE,
      'dfid' => 0,
      'is_new' => TRUE,
    );

  }

  /**
   * Saves date format to the database.
   */
  public function create() {
    if (isset($this->config['dfid']) && $this->config['dfid']) {
      $this->config['is_new'] = FALSE;
    }
    system_date_format_save($this->config);
    // Get the DFID, not returned by saving.
    $this->read();
    return $this;
  }

  /**
   * Tests if content type exists.
   */
  public function exists() {
    $exists = intval(db_query('SELECT COUNT(*) AS c FROM date_formats WHERE format=:format AND type=:type', array(':format' => $this->config['format'], ':type' => $this->config['type']))->fetchColumn());
    if ($exists) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Loads date format from the database.
   */
  public function read() {

    if (empty(CINC_DateFormat::$formats)) {
      CINC_DateFormat::$formats = system_get_date_formats();
    }

    if (isset(CINC_DateFormat::$formats[$this->config['type']])) {
      foreach (CINC_DateFormat::$formats[$this->config['type']] as $format => $details) {
        if ($format == $this->config['format']) {
          $this->config = $details;
        }
      }
    }

    return $this;

  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {

    $list = array();

    CINC_DateFormat::$formats = system_get_date_formats();

    foreach (CINC_DateFormat::$formats as $machine_name => $format) {
      $list[$machine_name] = array(
        'display' => $machine_name,
      );
    }

    return $list;

  }

  /**
   * Saves date format to the database.
   */
  public function update() {
    if (empty($this->config['dfid'])) {
      $this->read();
    }
    $this->create();
    return $this;
  }

  /**
   * Removes date format from the database.
   */
  public function delete() {
    if (empty($this->config['dfid'])) {
      $this->read();
    }
    system_date_format_delete($this->config['dfid']);
    return $this;
  }

}

/**
 * Creates date types in code.
 *
 * @code
 * $my_date_type = CINC::init('DateType')->set('type', 'my_date_type');
 * @endcode
 */
class CINC_DateType extends CINC implements CINC_CRUD {

  public $config_machine_name = 'type';
  public static $types = array();
  public $format = FALSE;

  function __construct() {

    $this->config_filters['type'] = 'set_type';

    $this->config = array(
      'type' => '',
      'title' => '',
      'locked' => FALSE,
      'is_new' => TRUE,
    );

  }

  /**
   * Tests if content type exists.
   */
  public function exists() {
    if (empty(CINC_DateType::$types)) {
      CINC_DateType::$types = system_get_date_types();
    }
    if (isset(CINC_DateType::$types[$this->config['type']])) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Auto-adds title when type is set, and cleans type.
   */
  public function set_type($type) {
    if (empty($this->config['title'])) {
      $this->set('title', CINC::display_name_from_machine_name($type));
    }
    return CINC::clean_machine_name($type);
  }

  /**
   * Saves date format type to the database.
   */
  public function create() {
    if ($this->config) {
      variable_set('date_format_' . $this->config['type'], $this->config['format']);
    }
    system_date_format_type_save($this->config);
    return $this;
  }

  /**
   * Loads date format type from the database.
   */
  public function read() {
    if (empty(CINC_DateFormatType::$types)) {
      CINC_DateFormatType::$types = system_get_date_types();
    }

    foreach (CINC_DateFormatType::$types as $type => $details) {
      if ($type == $this->config['type']) {
        $this->config = $details;
      }
    }
    $this->format = variable_get('date_format_' . $this->config['type'], FALSE);

    return $this;
  }

  /**
   * Saves date format type to the database.
   */
  public function update() {
    $this->delete();
    $this->create();
    return $this;
  }

  /**
   * Removes date format type from the database.
   */
  public function delete() {
    if ($this->format) {
      variable_del('date_format_' . $this->config['type']);
    }
    system_date_format_type_delete($this->config['type']);
    return $this;
  }

  /**
   * Sets format.
   */
  public function set_format($format) {
    $this->format = $format;
    return $this;
  }

}
