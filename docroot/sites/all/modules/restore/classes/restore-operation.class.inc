<?php
/**
 * @file
 * Restore script operation class.
 */

/**
 * The restore operation class.
 */
abstract class RestoreOperation {

  /**
   * The operation constructor.
   */
  public function __construct(array $op, $type, array $items) {
    $this->op = $op;
    $this->type = $type;
    $this->items = $items;
  }

  /**
   * Reset the stats details.
   */
  final protected function resetStats() {
    $this->stats = array(
      'active' => 0,
      'overriden' => 0,
      'missing' => 0,
    );
  }

  /**
   * Return the operation type.
   *
   * @return string
   *   The operation type.
   */
  final public function type() {
    return $this->type;
  }

  /**
   * Return the operation items.
   *
   * @return array
   *   The operation items array.
   */
  final public function items() {
    return $this->items;
  }

  /**
   * Return the operation title.
   *
   * @return string
   *   The operations title.
   */
  final public function title() {
    return $this->op['title'];
  }

  /**
   * Return the operation description.
   *
   * @return string
   *   The operations description.
   */
  final public function description() {
    return $this->op['description'];
  }

  /**
   * Return the item data that will be stored in the operations items array.
   *
   * @param mixed $key
   *   THe key/id of the item.
   *
   * @return mixed
   *   The data to store against the key.
   */
  abstract protected function item($key);

  /**
   * The item label, used in the conflict and other summary tables.
   *
   * @param mixed $key
   *   The item key/id.
   * @param mixed $item
   *   The item data.
   *
   * @return string
   *   The items label.
   */
  abstract protected function itemLabel($key, $item);

  /**
   * Get the status of a given operation item.
   *
   * @param mixed $key
   *   The items key.
   * @param mixed $item
   *   The operation item.
   *
   * @return int
   *   The item status.
   */
  abstract protected function itemStatus($key, $item);

  /**
   * Restore the given key/item.
   *
   * @param mixed $key
   *   The item key/id.
   * @param mixed $item
   *   The item data.
   */
  abstract protected function itemRestore($key, $item);

  /**
   * Output the summary details of the operation details.
   *
   * @return array
   *   Returns an array of summary items.
   */
  public function summary($raw = false) {
    $summary = array();
    foreach ($this->items as $key => $item) {
      $status = $this->itemStatus($key, $item);

      switch ($status) {
        case RestoreScript::STATUS_ACTIVE:
          $class = 'active';
          $title = t('This item is defined and active.');
          break;

        case RestoreScript::STATUS_OVERRIDEN:
          $class = 'overriden';
          $title = t('This item is defined but is overriden.');
          break;

        default:
          $class = 'missing';
          $title = t('This item has not been defined.');
          break;
      }

      $summary[$key] = $raw ?
        array(
          'status' => $class,
          'label' => $this->itemLabel($key, $item),
        ) :
        '<span class="opelm-status opelm-status-' . $class . '" title="' . $title . '">' . $this->itemLabel($key, $item) . '</span>';
    }

    ksort($summary);
    return $summary;
  }

  /**
   * Get the overall status of the operation.
   *
   * @return int
   *   The operation status.
   */
  public function status() {
    $this->resetStats();

    $overall_status = RestoreScript::STATUS_ACTIVE;
    foreach ($this->items as $key => $item) {
      $status = $this->itemStatus($key, $item);
      switch ($status) {
        case RestoreScript::STATUS_ACTIVE:
          $this->stats['active']++;
          break;

        case RestoreScript::STATUS_OVERRIDEN:
          $this->stats['overriden']++;
          if ($overall_status != RestoreScript::STATUS_MISSING) {
            $overall_status = RestoreScript::STATUS_OVERRIDEN;
          }
          break;

        default:
          $this->stats['missing']++;
          $overall_status = RestoreScript::STATUS_MISSING;
          break;
      }
    }

    return $overall_status;
  }

  /**
   * An array of available options.
   *
   * @param mixed $filter
   *   The label filter or FALSE to not filter.
   * @param bool $show_selected
   *   TRUE to show selected items (items already available in the details).
   *
   * @return array
   *   An array of options.
   */
  abstract public function available($filter = FALSE, $show_selected = FALSE);

  /**
   * Add an option to the details.
   *
   * @param string $id
   *   The option's id.
   */
  public function add($id) {
    $this->items[$id] = $this->item($id);
  }

  /**
   * Remove an option from the details.
   *
   * @param string $id
   *   The option's id.
   */
  public function remove($id) {
    unset($this->items[$id]);
  }

  /**
   * Determine if an option is already in the details.
   *
   * @param string $id
   *   The option's id.
   *
   * @return bool
   *   TRUE or FALSE.
   */
  public function has($id) {
    return isset($this->items[$id]);
  }

  /**
   * Restore the operation.
   */
  public function restore() {
    foreach ($this->items as $key => $item) {
      $status = $this->itemStatus($key, $item);
      if ($status !== RestoreScript::STATUS_ACTIVE) {
        $this->itemRestore($key, $item);
      }
    }
  }

  /**
   * Returns the current state of the operations details not the stored values.
   *
   * @return array
   *   A copy of the operation details but with the current state of the
   *   options.
   */
  public function current() {
    $items = array();
    foreach ($this->items as $key => $item) {
      $items[$key] = $this->item($key);
    }

    return $items;
  }

  /**
   * The total number of options in this operations details.
   *
   * @return int
   *   The total.
   */
  public function total() {
    return count($this->items);
  }

  /**
   * Check if this operation conflicts with the given operation.
   *
   * @param RestoreOperation $operation
   *   The operation to compare against this one.
   *
   * @return array
   *   An array of conflict details.
   */
  public function conflict(RestoreOperation $operation) {
    $items = $operation->items();

    $errors = array();
    foreach ($items as $key => $item) {
      if (isset($this->items[$key])) {
        $errors[] = $this->itemLabel($key, $item);
      }
    }

    return count($errors) ? $errors : FALSE;
  }


  /**
   * Return the number of active/matching operations.
   *
   * @return int
   *   The count.
   */
  public function noActive() {
    return $this->stats['active'];
  }

  /**
   * Return the number of overriden operations.
   *
   * @return int
   *   The count.
   */
  public function noOverriden() {
    return $this->stats['overriden'];
  }

  /**
   * Return the number of missing operations.
   *
   * @return int
   *   The count.
   */
  public function noMissing() {
    return $this->stats['missing'];
  }
}
