<?php

/**
 * @file
 * Page callback and submit handler for the default branch form.
 */

/**
 * Form constructor for the default branch form.
 */
function versioncontrol_project_git_default_branch_form($form, $form_state, $project) {
  $repo = $project->versioncontrol_project['repo'];
  $form['#repo'] = $repo;
  $form['#nid'] = $project->nid;

  // Add the radios widget for default branch selection.
  $form['default_branch'] = array(
    '#title' => t('The default repository branch'),
    '#description' => t('This branch will be checked out by default when someone clones the project.'),
    '#type' => 'radios',
    '#options' => array(),
  );

  // Make all branches available as options.
  $branches = $repo->loadBranches();
  foreach ($branches as $id => $branch) {
    $form['default_branch']['#options'][$id] = check_plain($branch->name);
  }
  uasort($form['default_branch']['#options'], '_versioncontrol_project_git_inverse_version_compare');

  // Make the current default branch the default selection.
  if ($repo->getDefaultBranch()) {
    $default_branch = $repo->loadBranches(NULL, array('name' => $repo->getDefaultBranch()));
    foreach ($default_branch as $id => $branch) {
      $form['default_branch']['#default_value'] = $id;
    }
  }

  // Add a submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form submission handler for versioncontrol_project_git_default_branch_form().
 */
function versioncontrol_project_git_default_branch_form_submit($form, &$form_state) {
  $repo = $form['#repo'];
  $default_branch = $repo->loadBranch(NULL, $form_state['values']['default_branch']);

  if ($default_branch) {
    try {
      $repo->setDefaultBranch($default_branch->name);
      $form_state['redirect'] = 'node/' . $form['#nid'];
      drupal_set_message(t('The default branch has been changed to %branch.', array('%branch' => $default_branch->name)));
    }
    catch (Exception $e) {
      drupal_set_message(filter_xss($e->getMessage()), 'error');
    }
  }
}

/**
 * Compares two branch names.
 *
 * Callback for uasort() within versioncontrol_project_git_default_branch_form().
 *
 * @param $a
 *   The first branch name.
 * @param $b
 *   The secound branch name.
 *
 * @return
 *   The inverse of version_compare().
 */
function _versioncontrol_project_git_inverse_version_compare($a, $b) {
  return version_compare($b, $a);
}
