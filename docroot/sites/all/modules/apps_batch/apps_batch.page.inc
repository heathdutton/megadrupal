<?php

/**
 * Operation on module and dependencies
 */
function _enable_dependencies_batch_operation($modules, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($modules);
    $context['sandbox']['modules'] = $modules;
  }

  // Process each module
  $module = array_shift($context['sandbox']['modules']);
  _apps_batch_enable_module($module);

  // Store result for post-processing in the finished callback.
  $context['results'][] = $module . ' is enabled.';

  // Update our progress information.
  $context['sandbox']['progress']++;

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Operation on features component
 */
function _enable_components_batch_operation($components, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $components = _flaten_components_tree($components);
    $context['sandbox']['max'] = count($components);
    $context['sandbox']['components'] = $components;
  }

  // Process each component
  $item = array_shift($context['sandbox']['components']);
  _apps_batch_restore_feature_component($item['module'], $item['component']);

  // Store result for post-processing in the finished callback.
  $context['results'][] = $item['component'] . ' is reverted.';

  // Update our progress information.
  $context['sandbox']['progress']++;

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Operation on app callback
 */
function _execute_callbacks_batch_operation($callbacks, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($callbacks);
    $context['sandbox']['apps_callbacks'] = $callbacks;
  }

  // Process nodes by groups of 1.
  // For each nid, load the node, reset the values, and save it.
  $item = array_shift($context['sandbox']['apps_callbacks']);
  if (isset($item['post install callback']) && function_exists($item['post install callback'])) {
    call_user_func($item['post install callback']);
  }

  // Store result for post-processing in the finished callback.
  $context['results'][] = $item['post install callback'] . ' is executed.';

  // Update our progress information.
  $context['sandbox']['progress']++;

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * 
 */
function _enable_app_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('The app is enabled.'));
  } else {
    drupal_set_message(t('An error occurred.'), 'error');
  }
}

/**
 * 
 */
function _flaten_components_tree($hierarchy) {
  $tree = array();
  foreach ($hierarchy as $module => $components) {
    foreach ($components as $component) {
      $tree[] = array(
        'module' => $module,
        'component' => $component
      );
    }
  }
  return $tree;
}