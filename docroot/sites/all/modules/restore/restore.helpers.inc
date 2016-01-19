<?php
/**
 * @file
 * A file containing some helper functions.
 */

/**
 * A helper function to output the status of a restore script or operation.
 *
 * This will output the status of the operation or script, or if obj is FALSE an
 * unknown status is given, otherwise FALSE is returned.
 *
 * @param RestoreScript/RestoreOperation $obj
 *   The restore script or operation.
 *
 * @return mixed
 *   FALSE if the $obj is not FALSE, RestoreScript or RestoreOperation,
 *   otherwise a renderable array.
 */
function _restore_status($obj) {
  if ($obj === FALSE) {
    // If the $obj is FALSE, usually caused by a failed operation load.
    return array(
      '#type' => 'markup',
      '#prefix' => '<span class="restore-status restore-status-unknown" title="' . t('Unknown operation') . '">',
      '#suffix' => '</span>',
      '#markup' => t('unknown'),
    );
  }

  // Check if $obj is a RestoreScript or RestoreOperation.
  if (!$obj instanceof RestoreScript && !$obj instanceof RestoreOperation) {
    return FALSE;
  }

  // Declare the status details.
  $statuses = array(
    RestoreScript::STATUS_ACTIVE => array(
      'class' => 'active',
      'tip' => t('This script is active.'),
      'label' => t('active'),
    ),
    RestoreScript::STATUS_OVERRIDEN => array(
      'class' => 'overriden',
      'tip' => t('There are parts of the script that have been overriden.'),
      'label' => t('overriden'),
    ),
    RestoreScript::STATUS_MISSING => array(
      'class' => 'missing',
      'tip' => t('There are parts of the script that are missing.'),
      'label' => t('missing'),
    ),
    RestoreScript::STATUS_ERROR => array(
      'class' => 'error',
      'tip' => t('There is an error with this script.'),
      'label' => t('error'),
    ),
  );

  // Get the objects status.
  $status = $obj->status();

  // Check if the status is known.
  if (!isset($statuses[$status])) {
    return FALSE;
  }

  // If $obj is a RestoreScript, check for some conflicts.
  $no_conflicts = 0;
  if ($obj instanceof RestoreScript) {
    $conflicts = $obj->conflicts();
    if ($conflicts) {
      foreach ($conflicts as $script) {
        foreach ($script['operations'] as $operation) {
          if ($operation['conflict'] === TRUE) {
            $no_conflicts++;
          }
          else {
            $no_conflicts += count($operation['conflict']);
          }
        }
      }
    }
  }

  // If theres any conflicts or non-active operations we break the status up
  // with some numbers.
  if ($obj->total() + $no_conflicts != $obj->noActive()) {
    $result = array();

    // The status value.
    $result[] = array(
      '#type' => 'markup',
      '#prefix' => '<span class="restore-status restore-stats restore-status-' . $statuses[$status]['class'] . '" title="' . $statuses[$status]['tip'] . '">',
      '#suffix' => '</span>',
      '#markup' => $statuses[$status]['label'],
    );

    // Display the number of active states.
    if ($obj->noActive() > 0) {
      $result[] = array(
        '#type' => 'markup',
        '#prefix' => '<span class="restore-status-count restore-status-active" title="' . t('There are @count with a state of active.', array('@count' => $obj->noActive())) . '">',
        '#suffix' => '</span>',
        '#markup' => $obj->noActive(),
      );
    }

    // Display the number of overriden states.
    if ($obj->noOverriden() > 0) {
      $result[] = array(
        '#type' => 'markup',
        '#prefix' => '<span class="restore-status-count restore-status-overriden" title="' . t('There are @count with a state of overriden.', array('@count' => $obj->noOverriden())) . '">',
        '#suffix' => '</span>',
        '#markup' => $obj->noOverriden(),
      );
    }

    // Display the number of missing states.
    if ($obj->noMissing() > 0) {
      $result[] = array(
        '#type' => 'markup',
        '#prefix' => '<span class="restore-status-count restore-status-missing" title="' . t('There are @count with a state of missing.', array('@count' => $obj->noMissing())) . '">',
        '#suffix' => '</span>',
        '#markup' => $obj->noMissing(),
      );
    }

    // Display the number of conflicts.
    if ($no_conflicts > 0) {
      $result[] = array(
        '#type' => 'markup',
        '#prefix' => '<span class="restore-status-count restore-status-conflict" title="' . t('There are @count conflicts.', array('@count' => $no_conflicts)) . '">',
        '#suffix' => '</span>',
        '#markup' => $no_conflicts,
      );
    }
  }
  else {
    // Just display the status value.
    $result = array(
      '#type' => 'markup',
      '#prefix' => '<span class="restore-status restore-status-' . $statuses[$status]['class'] . '" title="' . $statuses[$status]['tip'] . '">',
      '#suffix' => '</span>',
      '#markup' => $statuses[$status]['label'],
    );
  }

  return $result;
}

/**
 * Helper function to output the legend, used in the UI forms.
 *
 * @example
 *   $form['actions'] = array('#type' => 'actions');
 *   $form['actions']['submit'] = array(
 *     '#type' => 'submit',
 *     '#value' => t('Submit'),
 *   );
 *
 *   _restore_form_legend($form['actions']);
 *
 * @param array &$form
 *   The form/element to add the legend to.
 */
function _restore_form_legend(&$form) {
  $legend = array();
  $legend_items = array(
    'active' => t('active'), 'overriden' => t('overriden'),
    'missing' => t('missing'), 'error' => t('error'),
    'conflict' => t('conflict'), 'unknown' => t('unknown'),
  );

  foreach ($legend_items as $type => $title) {
    $span = '<span class="' . ($type == 'active' ? 'restore-status restore-stats ' : 'restore-status-count ') . 'restore-status-' . $type . '">' . $title . '</span>';
    $legend[] = $span;
  }

  $form['legend'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="restore-status-legend">',
    '#suffix' => '</div>',
    '#markup' => implode('', $legend),
  );
}
