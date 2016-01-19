<?php
/**
 * @file
 * Provides helper methods for dealing with webforms.
 */

class WebformApi {
  public $node;

  /**
   * Loads a webform node.
   *
   * @param int $webform_nid
   *   The node id related to webform.
   */
  public function load($webform_nid) {
    $this->node = node_load($webform_nid);
  }

  /**
   * Return an array with all the webforms available in the system.
   *
   * @return array
   *   Associative array containing the webform names, indexed by webform nid.
   */
  public static function getAvailableWebforms() {
    $available_webforms = array();

    $webforms = db_select('node', 'nd')
    ->fields('nd', array('title', 'nid'))
    ->condition('type', 'webform')
    ->execute()
    ->fetchAll();

    foreach ($webforms as $webform) {
      $available_webforms[$webform->nid] = node_load($webform->nid);
    }

    return $available_webforms;
  }

  /**
   * Get the webform components from the given $webform_nid.
   *
   * @return array
   *   Associative array containing the webforms components.
   */
  public function getWebformComponents() {
    if (!$this->node) {
      throw new Exception('Unable to get webform components, node is required.');
    }

    $webform_components = array();

    $is_webform = ($this->node->type == 'webform');
    if ($is_webform) {
      $webform_components = webform_component_list($this->node);
    }

    return $webform_components;
  }

  /**
   * Given a webform component id, return the form key associated to it.
   *
   * This value should be used to validate form using $form_state. For example:
   * $form_state['values'][<$formkey>]
   *
   * @param int $cid
   *   The webform component id to get the form key for.
   *
   * @return string
   *   The form key for the given component id. Empty string if none is found.
   */
  public function getWebformComponentFormkeyByCid($cid) {
    if (!$this->node) {
      throw new Exception('Unable to get webform components Form Key, node is required.');
    }

    $result = db_select('webform_component', 'wc')
    ->fields('wc', array('form_key'))
    ->condition('nid', $this->node->nid)
    ->condition('cid', $cid)
    ->execute()
    ->fetchCol();

    $formkey = '';
    if (isset($result[0])) {
      $formkey = $result[0];
    }

    return $formkey;
  }

  /**
   * Return an associative array with the year options.
   *
   * It ranges from current year down to (current year - $max_years).
   *
   * @return array
   *   An array containing the years.
   */
  public static function getYearOptions($max_years = 100) {
    $current_year = intval(date('Y'));
    $minimum_year = $current_year - $max_years;

    for ($year = $current_year; $year >= $minimum_year; $year--) {
      $years[$year] = $year;
    }

    return $years;
  }
}
