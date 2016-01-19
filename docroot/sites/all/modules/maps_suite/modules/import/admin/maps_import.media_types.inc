<?php

/**
 * @file
 * Administrative UI for media types management.
 */

use Drupal\maps_import\Profile\Profile;

/**
 * Form builder; media_types mapping form.
 *
 * @ingroup forms
 */
function maps_import_settings_media_types_form($form, &$form_state, Profile $profile) {
  drupal_set_title(t('Media types for profile @profile', array('@profile' => $profile->getTitle())));

  $pid = $profile->getPid();

  $form['#tree'] = TRUE;
  $form['#theme'] = 'maps_import_settings_media_types_form';
  $form['#key'] = "maps_import:media_types:$pid";
  $form['#empty'] = t('@type configuration is empty. Please ensure that the profile configuration was correctly imported from MaPS System®.', array('@type' => t('Media types')));
  $form[$form['#key']] = array();

  $default = variable_get($form['#key'], array());

  $drupal_media_types = maps_import_get_drupal_file_types();

  foreach ($profile->getMediaTypes() as $id => $media_type) {
    $form[$form['#key']][$id]['title'] = array('#type' => 'item', '#markup' => check_plain($media_type['title']));

    $form[$form['#key']][$id]['media_types'] = array(
      '#type' => 'select',
      '#options' => array('' => ' - Select - ') + $drupal_media_types,
      '#default_value' => isset($default[$id]['media_types']) ? $default[$id]['media_types'] : NULL,
      '#required' => TRUE,
    );
  }

  return system_settings_form($form);
}

/**
 * Returns HTML for the language overview form.
 *
 * @ingroup themeable
 */
function theme_maps_import_settings_media_types_form($variables) {
  $form = $variables['form'];
  $rows = array();

  foreach (element_children($form[$form['#key']], TRUE) as $key) {
    $element = &$form[$form['#key']][$key];

    $row = array();
    $row[] = drupal_render($element['title']);
    $row[] = drupal_render($element['media_types']);
    $rows[] = array(
      'data' => $row,
      'class' => array('draggable'),
    );
  }

  $output = theme('table', array(
    'header' => array(t('MaPS Media types'), t('Drupal File types')),
    'rows' => $rows,
    'empty' => $form['#empty'],
    'attributes' => array('id' => 'maps-import-media_types'),
  ));

  return $output . drupal_render_children($form);
}
