<?php
/**
 * @file
 * Restore operation: variable class.
 */

/**
 * The variable restore operation class.
 */
class RestoreOperationVariables extends RestoreOperation {

  /**
   * Overriden RestoreOperation::item().
   */
  protected function item($key) {
    return $GLOBALS['conf'][$key];
  }

  /**
   * Overriden RestoreOperation::itemLabel().
   */
  protected function itemLabel($key, $item) {
    return $key;
  }

  /**
   * Overriden RestoreOperation::itemStatus().
   */
  protected function itemStatus($key, $item) {
    if (isset($GLOBALS['conf'][$key]) && $GLOBALS['conf'][$key] == $item) {
      return RestoreScript::STATUS_ACTIVE;
    }
    elseif (isset($GLOBALS['conf'][$key])) {
      return RestoreScript::STATUS_OVERRIDEN;
    }
    else {
      return RestoreScript::STATUS_MISSING;
    }
  }

  /**
   * Overriden RestoreOperation::itemRestore().
   */
  protected function itemRestore($key, $item) {
    variable_set($key, $item);
  }

  /**
   * Overriden RestoreOperation::available().
   */
  public function available($filter = FALSE, $show_selected = FALSE) {
    $items = array();

    foreach ($GLOBALS['conf'] as $name => $value) {
      if (
        ($show_selected == FALSE && $this->has($name)) ||
        ($filter && stripos($name, $filter) === FALSE)
      ) {
        continue;
      }

      $items[$name] = $name;
    }

    return $items;
  }
}
