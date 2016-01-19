<?php

/**
 * Base class for batch operations.
 */
abstract class StorageApiPopulateBatchOperationBase {

  /**
   * Batch operation callback.
   *
   * Argument number is variable. Last argument is the batch context.
   */
  //static function op(array $args, array &$context) {
  static function op() {
    $args = func_get_args();
    // Get the batch context by reference.
    end($args);
    $context = &$args[key($args)];
    // Get the rest of the arguments.
    $args = array_slice($args, 0, -1);
    $step = variable_get('storage_populate_batch_step', 20);

    $class = new ReflectionClass(get_called_class());
    return $class->newInstanceArgs(array_merge(array(&$context, $step), $args));
  }

  /**
   * Constructor. Initializes the context and runs the operation.
   */
  function __construct(array &$context, $step) {
    $this->context = $context;
    $this->step = $step;

    // If it is the first time, initialize $context.
    if (empty($context['sandbox'])) {
      $context['sandbox']['total'] = $this->count();
      $context['sandbox']['current'] = 0;
      $context['results']['sections'] = 0;
      if ($context['sandbox']['total'] == 0) {
        $context['finished'] = 1;
        return;
      }
    }

    // Execute the operation.
    $this->process($context['sandbox']['current'], $context['sandbox']['total']);

    // Update finished value 0..1
    $context['finished'] = $context['sandbox']['current'] / $context['sandbox']['total'];
    // Manage race condition. It may occur if the total of files changed while running the batch process.
    if ($context['finished'] > 1) {
      $context['finished'] = 1;
    }
  }

  /**
   * Update the $context upon each iteration of this operation.
   *
   * This function is to be called at the end of each iteration
   * in the operation loop.
   */
  function updateContext() {
    $this->context['results']['files']++;
    $this->context['sandbox']['current']++;
    $this->context['message'] = $this->getProgressMessage();
  }
}

