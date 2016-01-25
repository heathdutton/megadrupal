<?php

/**
 * @file
 * Hardcoded mapping for specific webforms.
 */

/**
 * Class SalespushWebformMapper.
 */
class SalespushWebformMapper {

  static private $mapperinstance;
  /**
   * @var array
   *   Custom mappings for non standard webforms.
   */
  private $usermappings;

  /**
   * @var array
   *   Custom mappings for standard webforms.
   */
  private $defaultmappings;

  /**
   * Class constructor.
   */
  private function __construct() {
    $this->usermappings = array(
      // This webform has no FirstName. Defaults for event content type.
      0 => array(
        'Email' => 1,
        'FirstName' => 4,
        'LastName' => 4,
        'CompanyName' => 2,
        'Title' => 3,
      ),
      // This webform has no FirstName, CompanyName and LastName.
      446 => array(
        'Email' => 1,
        'FirstName' => 1,
        'LastName' => 2,
        'CompanyName' => 2,
        'Title' => 2,
      ),
      471 => array(
        'Email' => 1,
        'FirstName' => 7,
        'LastName' => 2,
        'CompanyName' => 5,
        'Title' => 6,
      ),
      // This webform has no CompanyName and LastName.
      451 => array(
        'Email' => 1,
        'FirstName' => 5,
        'LastName' => 2,
        'CompanyName' => 2,
        'Title' => 2,
      ),
    );
    $this->defaultmappings = array();
  }

  /**
   * Singleton.
   *
   * @return SalespushWebformMapper
   *   Singleton instance of the class.
   */
  static public function getinstance() {
    if (is_null(self::$mapperinstance)) {
      self::$mapperinstance = new self();
    }

    return self::$mapperinstance;
  }

  /**
   * Avoid instance been cloned.
   */
  protected function __clone() {
  }

  /**
   * Getter for all mappings.
   *
   * @return array
   *   Return all mappings array.
   */
  public function getmappings() {
    return $this->usermappings;
  }

  /**
   * Check if Node ID available for mappings.
   *
   * @param int $nid
   *   Node ID to be checked.
   *
   * @return bool
   *   Return TRUE if nid available in mappings.
   */
  public function ismapped($nid) {
    if (array_key_exists($nid, $this->usermappings) || in_array($nid, $this->defaultmappings)) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Check if Node ID available for default mappings.
   *
   * @param int $nid
   *   Node ID to be checked.
   *
   * @return bool
   *   Return TRUE if nid available in mappings.
   */
  public function isdefault($nid) {
    if (!array_key_exists($nid, $this->usermappings) && in_array($nid, $this->defaultmappings)) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Getter for Email.
   *
   * @param int $nid
   *   Node ID.
   *
   * @return bool|int
   *   FALSE if not found, int otherwise.
   */
  public function getemail($nid) {
    return $this->getidforname($nid, 'Email');
  }

  /**
   * Getter for FirstName.
   *
   * @param int $nid
   *   Node ID.
   *
   * @return bool|int
   *   FALSE if not found, int otherwise.
   */
  public function getfirstname($nid) {
    return $this->getidforname($nid, 'FirstName');
  }

  /**
   * Getter for LastName.
   *
   * @param int $nid
   *   Node ID.
   *
   * @return bool|int
   *   FALSE if not found, int otherwise.
   */
  public function getlastname($nid) {
    return $this->getidforname($nid, 'LastName');
  }

  /**
   * Getter for CompanyName.
   *
   * @param int $nid
   *   Node ID.
   *
   * @return bool|int
   *   FALSE if not found, int otherwise.
   */
  public function getcname($nid) {
    return $this->getidforname($nid, 'CompanyName');

  }

  /**
   * Getter for Title.
   *
   * @param int $nid
   *   Node ID.
   *
   * @return bool|int
   *   FALSE if not found, int otherwise.
   */
  public function gettitle($nid) {
    return $this->getidforname($nid, 'Title');
  }

  /**
   * Getter for node IDs, stored for mappings.
   *
   * @return array
   *   Returns array of stored mappings node IDs.
   */
  public function getnids() {
    return array_keys($this->usermappings);
  }

  /**
   * Adds node ID to default mappings array.
   *
   * @param int $nid
   *   Node ID.
   */
  public function adddefault($nid) {
    if (is_array($nid)) {
      $this->defaultmappings = array_merge($this->defaultmappings, $nid);
      return;
    }
    $this->defaultmappings = array_merge($this->defaultmappings, array($nid));
  }

  /**
   * Getter for nide ID and String.
   *
   * @param int $nid
   *   Node ID to be processed.
   * @param string $name
   *   String representation of the name. FE: Title or Email.
   *
   * @return bool|int
   *   FALSE if not found, integer value otherwise.
   */
  public function getidforname($nid, $name) {
    if (!$nid && !$name) {
      return FALSE;
    }
    if (!$this->ismapped($nid)) {
      return FALSE;
    }
    if ($this->isdefault($nid)) {
      $nid = 0;
    }

    return isset($this->usermappings[$nid][$name]) ? $this->usermappings[$nid][$name] : FALSE;
  }
}
