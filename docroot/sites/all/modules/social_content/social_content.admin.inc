<?php
/**
 * @file
 * Social Content administration area.
 * Provides menu callbacks for the Social Content administration area.
 */

/**
 * Callback for hook_menu.
 *
 * Build the overview page for all the registered globals.
 */
function social_content_global_overview() {

  $classes = social_content_get_classes();
  $header = array(t('Name'), t('Instances'), t('Settings'));
  $rows = array();
  $destination = drupal_get_destination();
  foreach ($classes as $class) {
    $class = new $class();
    $machine_name = $class->getMachineName();
    $instances_link = l(t('Instances'), 'admin/config/services/social-content/global/' . $machine_name . '/instances');
    $edit_link = l(t('Edit template'), 'admin/config/services/social-content/global/' . $machine_name . '/edit', array('query' => $destination));
    $rows[] = array(
      $class->getLabel(),
      $instances_link,
      $edit_link,
    );
  }
  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No Social Content submodules are enabled. You must enable at least one.'),
  );

  return $build;
}

/**
 * Callback for hook_menu.
 *
 * Build the overview page for all the instances for a given global.
 *
 * @param mixed $class
 *   The social content class for this global.
 */
function social_content_instances_overview($class) {

  // $classes = social_content_get_classes();
  $instances = $class->getInstances();

  $header = array(t('Name'), t('Cron'), t('Count'), t('Settings'));
  $rows = array();
  $destination = drupal_get_destination();

  drupal_set_title($class->getLabel());
  foreach ($instances as $id => $instance) {
    $edit_link = l(t('edit'), 'admin/config/services/social-content/instance/' . $id . '/edit', array('query' => $destination));
    $delete_link = l(t('delete'), 'admin/config/services/social-content/instance/' . $id . '/delete', array('query' => $destination));
    $run_link = l(t('run'), 'admin/config/services/social-content/instance/' . $id . '/run', array('query' => $destination));
    $enabled = $instance->settings['enabled'] ? '&#10004;' : t('disabled');
    $rows[] = array(
      check_plain($instance->settings['title']),
      $enabled,
      $instance->settings['count'],
      $run_link . ' | ' . $edit_link . ' | ' . $delete_link,
    );
  }
  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No instances found.'),
  );

  $build['add_instance'] = array(
    '#type' => 'link',
    '#title' => t('Add instance'),
    '#href' => 'admin/config/services/social-content/instance/' . $class->getMachineName() . '/add',
    '#options' => array(
      'query' => $destination,
    ),
    '#attributes' => array(
      'class' => array('button'),
    ),
  );

  return $build;
}


/**
 * Form handler for global / instance settings.
 */
function social_content_form($form, &$form_state, $type, $class, $action = NULL) {

  drupal_set_title($class->getLabel());

  $form['#storage']['class'] = $class;
  $form += $class->getForm($type);
  $form['Save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * Submit handler for the social_content_form.
 */
function social_content_form_submit($form, $form_state) {

  $values = $form_state['values'];
  if ($values['op'] == t('Save')) {
    $class = $form['#storage']['class'];
    $class->saveForm($form_state['values']);
    drupal_set_message(t('Your changes have been saved.'));
  }
}

/**
 * Form handler for instance deletion.
 */
function social_content_delete_instance_form($form, $form_state, $class) {
  $form['#storage']['class'] = $class;
  $arguments = array(
    '!global' => $class->getLabel(),
    '%title'  => $class->getInstanceTitle(),
  );
  $question = t('Are you sure you want to delete the !global %title instance?', $arguments);

  return confirm_form($form, $question, drupal_get_destination());
}

/**
 * Submit handler for the social_content_delete_instance_form.
 */
function social_content_delete_instance_form_submit(&$form, $form_state) {

  if (isset($form['#storage']) && isset($form['#storage']['class'])) {
    $class = $form['#storage']['class'];
    $class->deleteInstance();
    drupal_set_message(t('%instance history has been purged.', array('%instance' => $class->getInstanceTitle())));
  }
}


/**
 * Form callback for running a social content import.
 *
 * Provides a simple confirm form.
 */
function social_content_import_run_form($form, &$form_state, $class) {

  $form['description'] = array(
    '#markup' => '<p>' .
    t('Run the %title import. Note that only new items will be imported.', array('%title' => $class->getInstanceTitle())) . '</p>',
  );

  $form['#storage']['class'] = $class;

  $form['run_import'] = array(
    '#title' => t('Run Import'),
    '#value' => t('Run Import'),
    '#type' => 'submit',
  );

  return $form;
}

/**
 * Submit handler for social_content_import_run_form().
 *
 * Run the selected import (regardless of whether it's enabled on cron).
 * Report back results to the user.
 *
 * NOTE: This should really be in a batch job, but because cron doesn't
 * currently run on batch it was decided to simulate the request here.
 */
function social_content_import_run_form_submit($form, &$form_state) {
  if (isset($form['#storage']) && isset($form['#storage']['class'])) {
    $class = $form['#storage']['class'];
    $stats = $class->import();
    $args = array(
      '%title' => $class->getInstanceTitle(),
      '!processed' => $stats['processed'],
      '!imported' => $stats['imported'],
      '!skipped' => $stats['skipped'],
    );
    drupal_set_message(t('%title run successfully, processed !processed, imported !imported, skipped !skipped nodes', $args));
  }
}
