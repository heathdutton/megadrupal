<?php
/**
 * @file
 * Restore operation: dateformat class.
 */

/**
 * The dateformat restore operation class.
 */
class RestoreOperationDateFormats extends RestoreOperation {

  /**
   * Overriden RestoreOperation::item().
   */
  protected function item($key) {
    return $key;
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
    $date_formats = system_get_date_formats('custom');
    return isset($date_formats[$key]) ? RestoreScript::STATUS_ACTIVE : RestoreScript::STATUS_MISSING;
  }

  /**
   * Overriden RestoreOperation::itemRestore().
   */
  protected function itemRestore($key, $item) {
    $date_formats = system_get_date_formats('custom');

    $format = array();
    $format['format'] = trim($key);
    $format['type'] = 'custom';
    $format['locked'] = 1;
    $format['module'] = 'restore';

    if (isset($date_formats[$key])) {
      system_date_format_save($format, $date_formats[$key]['dfid']);
    }
    else {
      $format['is_new'] = 1;
      system_date_format_save($format);
    }
  }

  /**
   * Override available().
   */
  public function available($filter = FALSE, $show_selected = FALSE) {
    $date_formats = system_get_date_formats('custom');
    $dateformats = array();

    if ($date_formats && count($date_formats)) {
      foreach ($date_formats as $dateformat => $format) {
        if (
          (!$show_selected && $this->has($dateformat)) ||
          ($filter && stripos($dateformat, $filter) === FALSE)
        ) {
          continue;
        }

        $dateformats[$dateformat] = $dateformat;
      }
    }

    return $dateformats;
  }
}
