<?php
/**
 * @file
 * Contains the node list field access handler.
 */

/**
 * Controls access to a node based on a list field.
 */
class ACKNodeList extends AccessControlKit {
  /**
   * Implements AccessControlKitInterface::getRealms().
   */
  public function getRealms($node) {
    // Make sure the object is correctly configured for a list field.
    if (empty($this->settings['field_name'])) {
      return array();
    }

    // Return the values of the selected list field, if any.
    $field_name = $this->settings['field_name'];
    $realms = array();
    if (isset($node->$field_name)) {
      $list = $node->$field_name;
      if (!empty($list['und'])) {
        foreach ($list['und'] as $item) {
          $realms[] = $item['value'];
        }
      }
    }
    return $realms;
  }

  /**
   * Implements AccessControlKitInterface::settingsForm().
   */
  public function settingsForm($realm_type, array $realm_settings = array()) {
    // Inherit the realm's list field as the handler's connection to the node.
    if (isset($realm_settings['field'])) {
      $form['field_name'] = array('#type' => 'value', '#value' => $realm_settings['field']);
      return $form;
    }
  }

  /**
   * Implements AccessControlKitInterface::formAlter().
   */
  public function formAlter(&$form, &$form_state, $form_id, $create = NULL, $delete = NULL) {
    if (empty($this->settings['field_name'])) {
      return;
    }
    $field_name = $this->settings['field_name'];
    $node = $form['#node'];
    $is_new = empty($node->nid);

    // If the handler field exists on the node, we need to filter it to match
    // the user's create/delete access.
    if (isset($form[$field_name])) {
      $language = $form[$field_name]['#language'];
      $element = &$form[$field_name][$language];
      if (!($this->elementFilter($element, $is_new, $create, $delete))) {
        // @todo User does not have access to create the node.
      }
    }
  }
}
