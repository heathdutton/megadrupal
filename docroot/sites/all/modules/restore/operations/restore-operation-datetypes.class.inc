<?php
/**
 * @file
 * Restore dateformat operation.
 */

/**
 * The dateformat restore operation class.
 */
class RestoreOperationDateTypes extends RestoreOperation {

  /**
   * Overriden RestoreOperation::item().
   */
  protected function item($key) {
    $date_types = system_get_date_types();
    return array(
      'title' => $date_types[$key]['title'],
      'format' => variable_get("date_format_{$key}"),
    );
  }

  /**
   * Overriden RestoreOperation::itemLabel().
   */
  protected function itemLabel($key, $item) {
    return $item['title'];
  }

  /**
   * Overriden RestoreOperation::itemStatus().
   */
  protected function itemStatus($key, $item) {
    $date_types = system_get_date_types();
    if (isset($date_types[$key]) && $item['format'] == variable_get("date_format_{$key}")) {
      return RestoreScript::STATUS_ACTIVE;
    }
    elseif (isset($date_types[$key])) {
      return RestoreScript::STATUS_OVERRIDEN;
    }
    else {
      return RestoreScript::STATUS_MISSING;
    }
  }

  /**
   * Overriden RestoreOperation::itemRestore().
   */
  public function itemRestore($key, $item) {
    $date_types = system_get_date_types();
    $format_type = isset($date_types[$key]) ? $date_types[$key] : array(
      'is_new' => 1,
      'locked' => 0,
    );

    $format_type['title'] = trim($item['title']);
    $format_type['type'] = $key;

    system_date_format_type_save($format_type);
    variable_set("date_format_{$key}", $item['format']);
  }

  /**
   * Overriden RestoreOperation::available().
   */
  public function available($filter = FALSE, $show_selected = FALSE) {
    $date_types = system_get_date_types();
    $datetypes = array();

    if (count($date_types)) {
      foreach ($date_types as $datetype => $type) {
        if (
          ($show_selected == FALSE && $this->has($datetype)) ||
          ($filter && stripos($type['title'], $filter) === FALSE)
        ) {
          continue;
        }

        $datetypes[$datetype] = $type['title'];
      }
    }

    return $datetypes;
  }
}
