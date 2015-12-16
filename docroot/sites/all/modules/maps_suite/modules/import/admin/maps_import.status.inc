<?php

/**
 * @file
 * Administrative UI for statuses management.
 */

use Drupal\maps_import\Profile\Profile;

/**
 * Form builder; statuses mapping form.
 *
 * @ingroup forms
 */
function maps_import_settings_statuses_form($form, &$form_state, Profile $profile) {
  drupal_set_title(t('Statuses for profile @profile', array('@profile' => $profile->getTitle())));

  $pid = $profile->getPid();

  $form['#tree'] = TRUE;
  $form['#theme'] = 'maps_import_settings_statuses_form';
  $form['#key'] = "maps_import:statuses:$pid";
  $form['#empty'] = t('@type configuration is empty. Please ensure that the profile configuration was correctly imported from MaPS System®.', array('@type' => t('Statuses')));
  $form[$form['#key']] = array();

  $default = variable_get($form['#key'], array());

  foreach ($profile->getStatuses() as $id => $status) {
    $weight = isset($default[$id]['weight']) ? $default[$id]['weight'] : 0;
    $form[$form['#key']][$id] = array('#weight' => $weight);

    $form[$form['#key']][$id]['title'] = array('#type' => 'item', '#markup' => check_plain($status['title']));

    $form[$form['#key']][$id]['status'] = array(
      '#type' => 'select',
      '#options' => array(t('Unpublished'), t('Published')),
      '#default_value' => isset($default[$id]['status']) ? $default[$id]['status'] : 0,
      '#required' => TRUE,
    );

    $form[$form['#key']][$id]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => $weight,
    );
  }

  return system_settings_form($form);
}

/**
 * Returns HTML for the language overview form.
 *
 * @ingroup themeable
 */
function theme_maps_import_settings_statuses_form($variables) {
  $form = $variables['form'];
  $rows = array();

  drupal_add_tabledrag('maps-import-statuses', 'order', 'sibling', 'status-weight');

  foreach (element_children($form[$form['#key']], TRUE) as $key) {
    $element = &$form[$form['#key']][$key];
    $element['weight']['#attributes']['class'] = array('status-weight');

    $row = array();
    $row[] = drupal_render($element['title']);
    $row[] = drupal_render($element['status']);
    $row[] = drupal_render($element['weight']);
    $rows[] = array(
      'data' => $row,
      'class' => array('draggable'),
    );
  }

  $output = theme('table', array(
    'header' => array(t('MaPS status'), t('Drupal status'), t('Weight')),
    'rows' => $rows,
    'empty' => $form['#empty'],
    'attributes' => array('id' => 'maps-import-statuses'),
  ));

  return $output . drupal_render_children($form);
}
