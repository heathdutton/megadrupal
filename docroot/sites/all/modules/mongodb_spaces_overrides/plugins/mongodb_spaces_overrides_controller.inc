<?php

/**
 * @file
 * Abstract spaces_controller class with mongodb storage.
 */

/**
 * Abstract base class for a mongodb spaces controller. Classes that extend
 * mongodb_spaces_overrides_controller must implement load_original_values().
 */
abstract class mongodb_spaces_overrides_controller extends spaces_controller {

  /**
   * Protected method that ensures a space-level override for the
   * provided object has been loaded.
   */
  protected function load_space_values($id = NULL) {
    if (!$this->loaded_all['space']) {
      if (!isset($id)) {
        $query = array(
          'type' => $this->space_type,
          'id' => (int) $this->space_id,
          'object_type' => $this->controller,
        );
        if (($cursor = mongodb_collection('spaces_overrides')->find($query))) {
          foreach ($cursor as $mid => $value) {
            $this->values['space'][$value['object_id']] = $value['value'];
          }
        }
        $this->loaded_all['space'] = TRUE;
      }
      elseif (!isset($this->loaded['space'][$id])) {
        $query = array(
          'type' => $this->space_type,
          'id' => (int) $this->space_id,
          'object_type' => $this->controller,
          'object_id' => $id,
        );
        if (($cursor = mongodb_collection('spaces_overrides')->find($query))) {
          foreach ($cursor as $mid => $value) {
            $this->values['space'][$value['object_id']] = $value['value'];
          }
        }
        $this->loaded['space'][$id] = TRUE;
      }
    }
  }

  /**
   * Set override values for a given controller object in this space.
   */
  function set($id, $value) {
    $override = array(
      'type' => $this->space_type,
      'id' => (int) $this->space_id,
      'object_type' => $this->controller,
      'object_id' => $id,
    );
    $override_value = array(
      'value' => $value,
    );
    mongodb_collection('spaces_overrides')
      ->update($override, $override + $override_value, array('upsert' => TRUE));

    $this->values['space'][$id] = $value;
    // @todo Throw an exception if set failed.
    return TRUE;
  }

  /**
   * Delete a controller object override for this space.
   */
  function del($id = NULL) {
    $override = array(
      'type' => $this->space_type,
      'id' => (int) $this->space_id,
      'object_type' => $this->controller,
    );
    if (isset($id)) {
      $override['object_id'] = $id;
      if (isset($this->values['space'][$id])) {
        unset($this->values['space'][$id]);
      }
    }
    return mongodb_collection('spaces_overrides')
      ->remove($override);
  }

}
