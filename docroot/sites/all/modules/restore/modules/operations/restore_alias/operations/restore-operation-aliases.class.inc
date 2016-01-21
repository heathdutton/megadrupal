<?php
/**
 * @file
 * Restore operation: alias class.
 */

/**
 * The alias restore operation class.
 */
class RestoreAliasOperationAliases extends RestoreOperation {

  /**
   * Overriden RestoreOperation::item().
   */
  protected function item($key) {
    return path_load(array('source' => $key));
  }

  /**
   * Overriden RestoreOperation::itemLabel().
   */
  protected function itemLabel($key, $item) {
    return "{$item} => <em>{$key}</em>";
  }

  /**
   * Overriden RestoreOperation::itemStatus().
   */
  protected function itemStatus($key, $item) {
    $path = path_load(array('source' => $key));
    if ($path && $path == $item) {
      return RestoreScript::STATUS_ACTIVE;
    }
    elseif ($path) {
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
    $path = path_load(array('source' => $key));
    if (!isset($path['pid'])) {
      $path = array();
    }

    // Create the path data.
    $path['source'] = $key;
    $path['alias'] = $item;
    $path['language'] = LANGUAGE_NONE;

    // Save and return the path.
    path_save($path);
  }

  /**
   * Overriden RestoreOperation::available().
   */
  public function available($filter = FALSE, $show_selected = FALSE) {
    $aliases = array();
    $dbq = db_select('url_alias', 'ua')
      ->fields('ua');

    if ($filter) {
      $dbq->condition('alias', '%' . db_like($filter) . '%', 'LIKE');
    }

    $dbs = $dbq->execute();
    while ($row = $dbs->fetchAssoc()) {
      if (!$show_selected && $this->has($row['source'])) {
        continue;
      }

      $aliases[$row['source']] = "{$row['alias']} => <em>{$row['source']}</em>";
    }

    return $aliases;
  }
}
