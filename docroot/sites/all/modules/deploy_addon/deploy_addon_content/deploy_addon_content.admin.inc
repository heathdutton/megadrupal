<?php

/**
 * @file
 * Administration functions for the Deploy Add-on Content module.
 */

/**
 * Page callback to create new session based ad hoc deployment plan.
 */
function deploy_addon_content_adhoc_plan_page() {

  global $user;

  // Display a message telling the user what the current session plan is set to, if set.
  if (!empty($_SESSION['deploy_auto_plan_user_plan'])) {
    drupal_set_message(t('Your current user deploy plan is set to %plan.', array('%plan' => $_SESSION['deploy_auto_plan_user_plan'])));
    // Redirect if a destination was set.
    if (!empty($_GET['destination'])) {
      drupal_goto(drupal_get_destination());
    }
  }

  $options = array(
    'query' => array(
      'deploy_adhoc_plan' => $user->name,
    ),
    'attributes' => array(
      'class' => array('button'),
    ),
  );

  // Add destination link if its passed to this page.
  if (!empty($_GET['destination'])) {
    $options['query']['destination'] = $_GET['destination'];
  }

  // Create a link that looks like a button.
  $link = l(t('Create'), current_path(), $options);

  $page['description'] = array(
    '#markup' => t('<p>Create my own deployment plan and automatically add all changes to this plan.</p>'),
  );

  $page['create'] = array(
    '#markup' => $link,
  );

  return $page;
}
