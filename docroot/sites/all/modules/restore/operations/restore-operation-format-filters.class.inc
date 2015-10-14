<?php
/**
 * @file
 * Restore operation: format filter class.
 */

/**
 * The format filter restore operation class.
 */
class RestoreOperationFormatFilters extends RestoreOperation {

  /**
   * Load a format and filters.
   */
  protected function format($key) {
    $format = filter_format_load($key);
    if (!$format) {
      return FALSE;
    }

    $format = (array) $format;

    // Get the filters used by this format.
    $filters = filter_list_format($key);

    // Build the $format->filters array...
    $format['filters'] = array();
    foreach ($filters as $name => $filter) {
      $format['filters'][$name] = (array) $filter;
    }

    return $format;
  }

  /**
   * Overriden RestoreOperation::item().
   */
  protected function item($key) {
    $format = $this->format($key);
    return array(
      'name' => $format['name'],
      'cache' => $format['cache'],
      'status' => $format['status'],
      'weight' => $format['weight'],
      'filters' => $format['filters'],
    );
  }

  /**
   * Overriden RestoreOperation::itemLabel().
   */
  protected function itemLabel($key, $item) {
    return filter_xss($item['name']);
  }

  /**
   * Overriden RestoreOperation::itemStatus().
   */
  protected function itemStatus($key, $item) {
    $format = $this->format($key);
    if ($format) {
      $matches = $item['name'] == $format['name'] &&
        $item['cache'] == $format['cache'] &&
        $item['status'] == $format['status'] &&
        $item['weight'] == $format['weight'] &&
        count($item['filters']) == count($format['filters']);

      if ($matches) {
        foreach ($format['filters'] as $id => $filter) {
          if (!isset($item['filters'][$id]) || $item['filters'][$id] != $filter) {
            $matches = FALSE;
            break;
          }
        }
      }

      if ($matches) {
        return RestoreScript::STATUS_ACTIVE;
      }
      else {
        return RestoreScript::STATUS_OVERRIDEN;
      }
    }
    else {
      return RestoreScript::STATUS_MISSING;
    }
  }

  /**
   * Overriden RestoreOperation::itemRestore().
   */
  protected function itemRestore($key, $item) {
    $item['filter'] = $key;
    filter_format_save((object) $item);
  }

  /**
   * Overriden RestoreOperation::available().
   */
  public function available($filter = FALSE, $show_selected = FALSE) {
    $items = array();

    $formats = filter_formats();
    foreach ($formats as $name => $format) {
      if (
        ($show_selected == FALSE && $this->has($name)) ||
        ($filter && stripos($name, $filter) === FALSE)
      ) {
        continue;
      }

      $items[$name] = filter_xss($format->name);
    }

    return $items;
  }
}
