<?php
/**
 * @file
 * Provides the restore script entity class.
 */

/**
 * The restore script entity.
 */
class RestoreScript implements ArrayAccess, Iterator, Countable {

  const STATUS_ACTIVE = 0x01;
  const STATUS_OVERRIDEN = 0x02;
  const STATUS_MISSING = 0x03;
  const STATUS_ERROR = 0x04;

  /**
   * Array offsetSet method.
   */
  public function offsetSet($offset, $value) {
    if (is_null($offset) && $value instanceof RestoreOperation) {
      $this->details['operations'][] = $value;
    }
    else {
      throw new Exception(t('You can not edit an operation directly or add a non operation.'));
    }
  }

  /**
   * Array offsetExists method.
   */
  public function offsetExists($offset) {
    return isset($this->details['operations'][$offset]);
  }

  /**
   * Array offsetUnset method.
   */
  public function offsetUnset($offset) {
    unset($this->details['operations'][$offset]);
  }

  /**
   * Array offsetGet method.
   */
  public function offsetGet($offset) {
    return isset($this->details['operations'][$offset]) ? $this->details['operations'][$offset] : NULL;
  }

  /**
   * Array count method.
   */
  public function count() {
    return count($this->details['operations']);
  }

  /**
   * The internal array pointer.
   */
  protected $pointer = 0;

  /**
   * Iterator rewind method.
   */
  public function rewind() {
    $this->pointer = 0;
  }

  /**
   * Iterator current method.
   */
  public function current() {
    return $this->details['operations'][$this->pointer];
  }

  /**
   * Iterator key method.
   */
  public function key() {
    return $this->pointer;
  }

  /**
   * Iterator next method.
   */
  public function next() {
    return $this->pointer++;
  }

  /**
   * Iterator valid method.
   */
  public function valid() {
    return isset($this->details['operations'][$this->pointer]);
  }

  /**
   * status stats.
   */
  protected $stats = array(
    'total' => 0,
    'active' => 0,
    'overriden' => 0,
    'missing' => 0,
  );

  /**
   * The restore script constructor.
   */
  public function __construct($name, array $details) {
    $this->name = $name;
    $this->details = array_merge(array(
      'title' => '',
      'description' => '',
      'group' => '',
      'status' => TRUE,
      'operations' => array(),
      'locked' => FALSE,
    ), $details);

    if (count($this->details['operations'])) {
      $operations = array();

      foreach ($this->details['operations'] as $operation) {
        $op = restore_operation($operation['type'], $operation['items']);
        if ($op->total() === 0) {
          continue;
        }

        $operations[] = $op;
      }

      $this->details['operations'] = $operations;
    }
  }

  /**
   * Get the module from the details.
   *
   * @return boolean|string
   *   Returns the module name or FALSE if the property doesn't exist.
   */
  public function module() {
    return isset($this->details['module']) ? $this->details['module'] : FALSE;
  }

  /**
   * Get the scripts locked state.
   *
   * @return bool
   *   Returns TRUE or FALSE.
   */
  public function locked() {
    return isset($this->details['locked']) && $this->details['locked'];
  }

  /**
   * Is this script stored in the database.
   *
   * @return bool
   *   Returns TRUE or FALSE.
   */
  public function database() {
    return isset($this->details['database']) && $this->details['database'];
  }

  /**
   * Get the storage method title.
   *
   * @return string
   *   Returns the name of the storage method.
   */
  public function storage() {
    if ($this->module() && $this->database()) {
      $storage = array(
        '#type' => 'markup',
        '#prefix' => '<span class="restore-status restore-storage-module overriden" title="' . t('Stored in a module, but has been overriden in the database.') . '">',
        '#suffix' => '</span>',
        '#markup' => t('overriden'),
      );
    }
    elseif ($this->module()) {
      $storage = array(
        '#type' => 'markup',
        '#prefix' => '<span class="restore-status restore-storage-module" title="' . t('Stored in a module.') . '">',
        '#suffix' => '</span>',
        '#markup' => t('module'),
      );
    }
    elseif ($this->database()) {
      $storage = array(
        '#type' => 'markup',
        '#prefix' => '<span class="restore-status restore-storage-database" title="' . t('Stored in the database.') . '">',
        '#suffix' => '</span>',
        '#markup' => t('database'),
      );
    }
    else {
      $storage = array(
        '#type' => 'markup',
        '#prefix' => '<span class="restore-status restore-storage-file" title="' . t('Stored in a restore script fle.') . '">',
        '#suffix' => '</span>',
        '#markup' => t('file'),
      );
    }
    
    return drupal_render($storage);
  }

  /**
   * Determine if this script conflicts with any other script.
   *
   * @return array
   *   Returns an array detailing the conflicting scripts and operations.
   */
  public function conflicts() {
    if (isset($this->conflicts)) {
      return $this->conflicts;
    }

    $this->conflicts = array();

    $scripts = restore_scripts();
    foreach ($scripts as $name => $script) {
      // We don't want to compare with ourself so check the name.
      if ($name == $this->name) {
        continue;
      }

      $details = $script->details();
      foreach ($this->details['operations'] as $source_operation) {
        $source_class = get_class($source_operation);

        foreach ($details['operations'] as $operation) {
          $operation_class = get_class($operation);
          if ($source_class != $operation_class) {
            continue;
          }

          if ($conflict = $source_operation->conflict($operation)) {
            if (!isset($this->conflicts[$name])) {
              $this->conflicts[$name] = array(
                'script' => $script,
                'operations' => array(),
              );
            }

            $this->conflicts[$name]['operations'][] = array(
              'source_operation' => $source_operation,
              'operation' => $operation,
              'conflict' => $conflict,
            );
          }
        }
      }
    }

    return $this->conflicts;
  }

  /**
   * Returns the name of the script.
   *
   * @return string
   *   The name of the script.
   */
  public function name() {
    return $this->name;
  }

  /**
   * Returns the scripts details array.
   *
   * @return array
   *   The scripts details.
   */
  public function details() {
    return $this->details;
  }

  /**
   * Get the scripts title.
   *
   * @return string
   *   The scripts title.
   */
  public function title() {
    return $this->details['title'];
  }

  /**
   * Get the scripts description.
   *
   * @return string
   *   The scripts description.
   */
  public function description() {
    return $this->details['description'];
  }

  /**
   * Get the scripts group.
   *
   * @return string
   *   The scripts group.
   */
  public function group() {
    return !empty($this->details['group']) ? $this->details['group'] : t('Default');
  }

  /**
   * Run the script.
   */
  public function restore(array $operations = array()) {
    if (empty($operations)) {
      $operations = array_keys($this->details['operations']);
    }

    foreach ($operations as $op) {
      $this->details['operations'][$op]->restore();
    }
  }

  /**
   * Determine the scripts status.
   *
   * @return int
   *   Returns a status flag.
   */
  public function status() {
    $stats = array(
      'total' => 0,
      'active' => 0,
      'overriden' => 0,
      'missing' => 0,
    );

    $status = RestoreScript::STATUS_ACTIVE;
    if (count($this->details['operations'])) {
      foreach ($this->details['operations'] as $operation) {
        if ($operation === FALSE) {
          continue;
        }

        $op_status = $operation->status();
        if ($op_status != $status) {
          $status = RestoreScript::STATUS_ERROR;
        }

        $stats['total'] += $operation->total();
        $stats['active'] += $operation->noActive();
        $stats['overriden'] += $operation->noOverriden();
        $stats['missing'] += $operation->noMissing();
      }
    }

    $this->stats = $stats;

    return $status;
  }

  /**
   * The total number of operations and operation options.
   *
   * @return int
   *   The value.
   */
  public function total() {
    return $this->stats['total'];
  }

  /**
   * The number of active/matching operations and operation options.
   *
   * @return int
   *   The value.
   */
  public function noActive() {
    return $this->stats['active'];
  }

  /**
   * The number of overriden operations and operation options.
   *
   * @return int
   *   The value.
   */
  public function noOverriden() {
    return $this->stats['overriden'];
  }

  /**
   * The number of missing operations and operation options.
   *
   * @return int
   *   The value.
   */
  public function noMissing() {
    return $this->stats['missing'];
  }
}
