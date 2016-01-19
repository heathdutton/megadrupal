<?php
/**
 * Implements hook_apps_app_info().
 */
function enterprise_rotator_apps_app_info() {
  return array(
    'configure form' => 'enterprise_rotator_apps_app_configure_form',
    'demo content module' => 'enterprise_rotator_content',
  );
}

function enterprise_rotator_apps_app_configure_form() {
  $form = array();

  $form['create_rotator'] = array(
    '#markup' => 'Create new banner slide on the ' . l(t('Create Banner Slide Page'), 'node/add/oe-slide', array('query' =>drupal_get_destination())) . ".",
  );

  return $form;
}