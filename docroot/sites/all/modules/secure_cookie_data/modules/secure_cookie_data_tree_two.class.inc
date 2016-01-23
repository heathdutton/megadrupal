<?php
/**
 * @file   secure_cookie_data_tree_two.class.inc
 * @author António P. P. Almeida <appa@perusio.net>
 * @date   Mon Dec  2 11:59:02 2013
 *
 * @brief Extends the secureCookieBasic class for handling two level deep
 *        trees.
 *
 */

class secureCookieTreeTwo extends secureCookieBasic {

  // The class specific secret.
  static public $__secret = 'afe76e816d2486f2ec796ec199af1a613202a24a';

  // The cookie name.
  static public $__cookie_name = 'SecureDataCookie32';

  /**
   * Setter for the class.
   *
   * @param object|array $data
   *   The data to be stored in the cookie in the form of an object or a keyed
   *   array.
   * @return boolen
   *   TRUE if the cookie was modified/set, FALSE if not.
   */
  static public function set_tree($data) {

    // If given an object then set the cookie immediately.
    if (is_object($data)) {
      // Set the cookie.
      return self::set($data);
    }

    // Check if the cookie exists. If so then recover the existing value.
    if (isset($_COOKIE[static::$__cookie_name])) {
      $previous_data = parent::decode($_COOKIE[static::$__cookie_name]);
      // Unset the hmac.
      if (isset($previous_data->hmac)) {
        unset($previous_data->hmac);
      }
    }

    // If there's no previous data, meaning that no cookie exists then set it
    // now.
    if (!isset($previous_data)) {
      $previous_data = new stdClass();
      $previous_data->data = new stdClass();
    }

    // See if we have already set or not the child object. Just in case.
    if (!isset($previous_data->data)) {
      $previous_data->data = new stdClass();
    }

    // Setting the values for the tree node (level 0 and 1).
    if (!isset($previous_data->data->$data[$data['label_level0']])) {
      // Go one level down by creating a child element.
      $previous_data->data->$data[$data['label_level0']] = new stdClass();
    }

    // Handle the values list. Either is an array or is brand new. If an array
    // check that the value isn't already there.
    if (is_array($previous_data->data->$data[$data['label_level0']]->$data['label_level1'])) {
      // We have to copy the array to another variable. Yes, PHP sucks!
      $values = $previous_data->data->$data[$data['label_level0']]->$data['label_level1'];
      // Check to see if the value is different.
      if (!in_array($data['value'], $values)) {
        $values[] = $data['value'];
        $previous_data->data->$data[$data['label_level0']]->$data['label_level1']= $values;
      }
    }
    else {
      $previous_data->data->$data[$data['label_level0']]->$data['label_level1'] = array($data['value']);
    }
    // Set the cookie.
    self::set($previous_data);
  }

  /**
   * Removes a certain node from the tree stored as cookie data.
   *
   * @param array $element
   *   Specification of the element to delete.
   * @param object $data
   *   The data tree as an object.
   * @return nothing
   *   Side effects only.
   */
  static public function delete_node($element, $data = NULL) {

    // Get the cookie data if not given.
    if (empty($data)) {
      $cookie_data = self::get();
    }
    else {
      $cookie_data = $data;
    }

    // Get the element description to know at which depth a tree branch should
    // be pruned.
    $keys = array_keys($element);

    // Check to see if we are at the first or at the second level and proceed
    // accordingly.
    if (in_array('label_level1', $keys)) {
      if (isset($cookie_data->data->$element[$element['label_level0']]->$element['label_level1'])) {
        unset($cookie_data->data->$element[$element['label_level0']]->$element['label_level1']);
      }
    }
    elseif (in_array('label_level0', $keys)) {
      if (isset($cookie_data->data->$element[$element['label_level0']])) {
        unset($cookie_data->data->$element[$element['label_level0']]);
      }
    }
    // In the special case of the key 'ALL' set to a TRUE value delete the
    // cookie completely.
    elseif (count($keys) == 1 && $keys[0] == 'ALL' && $element['ALL']) {
      if (isset($cookie_data)) {
        self::delete();
      }
    }
    else {
      return FALSE;
    }
    // Set the cookie without the deleted node.
    self::set_tree($cookie_data);
  }
}

