<?php
/**
 * @file
 * An object that describes single Fieldable Panels Pane Bundle.
 */

/**
 * Class FppBundle.
 */
class FppBundle {
  /**
   * The bundle ID.
   *
   * @var int
   */
  private $bid = 0;

  /**
   * The human name of bundle.
   *
   * @var string
   */
  private $name = '';

  /**
   * Indicates that the panel should be placed in top level or not.
   *
   * @var bool
   */
  private $level = FALSE;

  /**
   * An unique bundle machine name.
   *
   * @var string
   */
  private $machine = '';

  /**
   * Category where was placed the bundle.
   *
   * @var string
   */
  private $category = '';

  /**
   * Indicates the state of automatically load the CSS & JS files for bundle.
   *
   * @var bool
   */
  private $assets = TRUE;

  /**
   * Property getter.
   */
  public function getBundleId() {
    return (int) $this->bid;
  }

  /**
   * Property getter.
   */
  public function getHumanName() {
    return $this->name;
  }

  /**
   * Property getter.
   */
  public function isTopLevelPanel() {
    return (bool) $this->level;
  }

  /**
   * Property getter.
   */
  public function getMachineName() {
    return $this->machine;
  }

  /**
   * Property getter.
   */
  public function getCategoryName() {
    return $this->category;
  }

  /**
   * Property getter.
   */
  public function isAssetsLoadAutomatically() {
    return (bool) $this->assets;
  }

  /**
   * Get object representation as an array.
   *
   * @return array
   *   Bundle fields, except the ID.
   */
  public function getAllFieldsAsArray() {
    $parameters = get_object_vars($this);
    // Delete the ID from the property set, because
    // it is an unique and plays no role.
    unset($parameters['bid']);

    return $parameters;
  }

  /**
   * Method for export object parameters as an keyed array.
   *
   * @return array
   *   Representation of the object as an array.
   */
  public function export() {
    return array($this->machine => $this->getAllFieldsAsArray());
  }

  /**
   * Create the bundle object from an array.
   *
   * @param array $bundle
   *   Representation of the bundle as array.
   *
   * @throws \FppBundlesException
   *   When not all necessary properties was set.
   *
   * @return $this
   *   Bundle object.
   */
  public function import(array $bundle) {
    $fields = get_object_vars($this);
    $filled = 0;

    foreach (array_keys($fields) as $field) {
      if (isset($bundle[$field])) {
        $this->$field = $bundle[$field];
        $filled++;
      }
    }

    if (count($fields) != $filled) {
      throw \FppBundlesException::set(t('The bundle cannot to be imported from an array because the number of items is not equal to the properties number.'));
    }

    return $this;
  }
}
