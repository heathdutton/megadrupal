<?php
/**
 * @file
 * Restore operation: modules class.
 */

/**
 * The module restore operation class.
 */
class RestoreOperationModules extends RestoreOperation {

  /**
   * Overriden RestoreOperation::item().
   */
  protected function item($key) {
    return module_exists($key) ? 'true' : 'false';
  }

  /**
   * Overriden RestoreOperation::itemLabel().
   */
  protected function itemLabel($key, $item) {
    return "{$key} (" . ($item == 'true' ? t('enabled') : t('disabled')) . ')';
  }

  /**
   * Overriden RestoreOperation::itemStatus().
   */
  protected function itemStatus($key, $item) {
    if (module_exists($key) == ($item == 'true')) {
      return RestoreScript::STATUS_ACTIVE;
    }
    else {
      return RestoreScript::STATUS_OVERRIDEN;
    }
  }

  /**
   * Overriden RestoreOperation::itemRestore().
   */
  protected function itemRestore($key, $item) {
    if ($item == 'true') {
      module_enable($key);
    }
    else {
      module_disable($key);
    }
  }

  /**
   * Overriden RestoreOperation::available().
   */
  public function available($filter = FALSE, $show_selected = FALSE) {
    $modules = array();

    $dbq = db_select('system', 'sys');
    $dbq
      ->fields('sys', array())
      ->condition('type', 'module')
      ->orderBy('name', 'ASC');

    if ($filter) {
      $dbq->condition('name', '%' . db_like($filter) . '%', 'LIKE');
    }

    $records = $dbq
      ->execute()
      ->fetchAll();

    foreach ($records as $record) {
      if ($show_selected == FALSE && $this->has($record->name)) {
        continue;
      }

      $modules[$record->name] = t('@name (@status)', array(
        '@name' => $record->name,
        '@status' => $record->status ? t('enabled') : t('disabled'),
      ));
    }

    return $modules;
  }
}
