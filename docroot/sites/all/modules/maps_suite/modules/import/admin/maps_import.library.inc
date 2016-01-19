<?php

/**
 * @file
 * Administrative UI for libraries management.
 */

use Drupal\maps_import\Mapping\Source\MapsSystem\Attribute\Library;
use Drupal\maps_import\Profile\Profile;

/**
 * Form builder; Library mapping overview.
 *
 * @see theme_maps_import_libraries_overview()
 */
function maps_import_libraries_overview($form, &$form_state, Profile $profile) {
  $current_language = $profile->getDefaultLanguage();
  $libraries = array_keys(maps_import_libraries($profile));

  $attributes = $profile->getConfigurationTypes('attribute', $current_language);
  $maps = array();

  foreach (maps_suite_get_records('maps_import_library_map', NULL, array('pid' => $profile->getPid())) as $map) {
    $maps[$map->id_attribute] = $map;
  }

  $form['#tree'] = TRUE;
  $form['#theme'] = 'maps_import_libraries_overview';
  $form['#key'] = "maps_import:libraries:" . $profile->getPid();
  $form[$form['#key']] = array();

  foreach ($libraries as $id_attribute) {
    $form[$form['#key']][$id_attribute] = array('#data' => $id_attribute, '#header' => TRUE);

    $form[$form['#key']][$id_attribute]['id'] = array(
      '#type' => 'item',
      '#markup' => $id_attribute,
    );

    $form[$form['#key']][$id_attribute]['title'] = array(
      '#type' => 'item',
      '#markup' => isset($attributes[$id_attribute]) ?
      $attributes[$id_attribute]['title'] :
      t('Attribute with ID @id is missing in MaPS System® configuration.', array('@id' => $id_attribute)),
    );

    $vocabulary_name = '-';
    if (!empty($maps[$id_attribute])) {
      $vocabulary = taxonomy_vocabulary_load($maps[$id_attribute]->vocabulary);
      $vocabulary_name = $vocabulary->name;
    }

    $form[$form['#key']][$id_attribute]['map'] = array(
      '#type' => 'item',
      '#markup' => $vocabulary_name,
    );

    if (!empty($maps[$id_attribute])) {
      $links = array(
        'edit' => array(
          'href' => "admin/maps-suite/profiles/" . $profile->getName() . "/libraries/$id_attribute/edit",
          'title' => t('edit the map'),
        ),
        'delete' => array(
          'href' => "admin/maps-suite/profiles/" . $profile->getName() . "/libraries/$id_attribute/delete",
          'title' => t('delete the map'),
        ),
      );
    }
    else {
      $links = array(
        'add' => array(
          'href' => "admin/maps-suite/profiles/" . $profile->getName() . "/libraries/$id_attribute/add",
          'title' => t('create a new map'),
        ),
      );
    }

    $form[$form['#key']][$id_attribute]['links'] = array(
      '#type' => 'item',
      '#theme' => 'links',
      '#links' => $links,
    );
  }

  return $form;
}

/**
 * Theme for library mapping overview.
 *
 * @see maps_import_libraries_overview()
 */
function theme_maps_import_libraries_overview($variables) {
  $form = $variables['form'];
  $rows = array();

  foreach (element_children($form[$form['#key']], TRUE) as $key) {
    $element = &$form[$form['#key']][$key];

    $row = array();
    $row[] = drupal_render($element['id']);
    $row[] = drupal_render($element['title']);
    $row[] = drupal_render($element['map']);
    $row[] = drupal_render($element['links']);
    $rows[] = array(
      'data' => $row,
    );
  }

  $output = theme('table', array(
    'header' => array(
      t('Attribute Id'),
      t('Title'),
      t('Vocabulary'),
      t('Actions'),
    ),
    'rows' => $rows,
    'attributes' => array('id' => 'maps-import-libraries'),
    'empty' => t('There is no defined library yet. You may try to process the configuration import.'),
  ));

  return $output . drupal_render_children($form);
}

/**
 * Form builder; Add a library mapping.
 *
 * @see maps_import_libraries_add_form_validate()
 * @see maps_import_libraries_add_form_submit()
 */
function maps_import_libraries_add_form($form, &$form_state, Profile $profile, Library $library, $type) {
  $form['#profile'] = $profile;
  $form['#library'] = $library;
  $form['#edit'] = ($type == 'edit');

  $vocabularies = taxonomy_get_vocabularies();
  $options = array();
  foreach ($vocabularies as $vocabulary) {
    $options[$vocabulary->vid] = $vocabulary->name;
  }

  $form['vocabulary'] = array(
    '#title' => t('Vocabulary'),
    '#type' => 'select',
    '#options' => array('' => t('- Select -')) + $options,
    '#description' => t('Select a vocabulary to map with the library.'),
    '#required' => TRUE,
  );

  if ($type === 'edit') {
    $result = db_select('maps_import_library_map', 'lm')
      ->fields('lm')
      ->condition('pid', $profile->getPid())
      ->condition('id_attribute', $library->getId())
      ->execute();

    $row = $result->fetch();
    $form['vocabulary']['#default_value'] = $row->vocabulary;
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form builder; Validate the adding form.
 *
 * @see maps_import_libraries_add_form()
 * @see maps_import_libraries_add_form_submit()
 */
function maps_import_libraries_add_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  if (empty($values['vocabulary'])) {
    form_set_error('vocabulary', t('No vocabulary selected.'));
  }
}

/**
 * Form builder; Save the new library mapping.
 *
 * @see maps_import_libraries_add_form()
 * @see maps_import_libraries_add_form_validate()
 */
function maps_import_libraries_add_form_submit($form, &$form_state) {
  if ($form['#edit']) {
    $result = db_update('maps_import_library_map')
      ->fields(array(
        'vocabulary' => $form_state['values']['vocabulary'],
      ))
      ->condition('pid', $form['#profile']->getPid())
      ->condition('id_attribute', $form['#library']->getId())
      ->execute();

    $save_state = ($result == SAVED_UPDATED);
  }
  else {
    $result = db_insert('maps_import_library_map')
      ->fields(array(
        'pid' => $form['#profile']->getPid(),
        'id_attribute' => $form['#library']->getId(),
        'vocabulary' => $form_state['values']['vocabulary'],
      ))
      ->execute();

    $save_state = ($result == SAVED_NEW);
  }

  $form_state['redirect'] = array('admin/maps-suite/profiles/' . $form['#profile']->getName() . '/libraries');
}

/**
 * Form builder; Confirmation for deleting a library mapping.
 *
 * @see maps_import_libraries_delete_confirm_form_submit()
 */
function maps_import_libraries_delete_confirm_form($form, &$form_state, Profile $profile, Library $library) {
  // Verify that the entry exists in database.
  $result = db_select('maps_import_library_map', 'lm')
    ->fields('lm')
    ->condition('pid', $profile->getPid())
    ->condition('id_attribute', $library->getId())
    ->execute();

  if ((int) $result->rowCount() == 0) {
    drupal_set_message(t('This mapping does not exist.'), 'error');
  }

  drupal_set_title(t('Delete library mapping for attribute @library', array('@library' => $library->getId())));

  $form['pid'] = array('#type' => 'value', '#value' => $profile->getPid());
  $form['id_attribute'] = array('#type' => 'value', '#value' => $library->getId());

  return confirm_form(
    $form,
    t('Delete library mapping'),
    'admin/maps-suite/profiles/' . $form['pid']['#value'] . '/libraries/' . $form['id_attribute']['#value'] . '/edit',
    t('Are you sure you want to delete the library mapping for library %library ?', array('%library' => $library->getId())),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete a library mapping.
 *
 * @see maps_import_libraries_delete_confirm_form()
 */
function maps_import_libraries_delete_confirm_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  db_delete('maps_import_library_map')
    ->condition('pid', $values['pid'])
    ->condition('id_attribute', $values['id_attribute'])
    ->execute();

  $form_state['redirect'] = array('admin/maps-suite/profiles/' . $values['pid'] . '/libraries');
  drupal_set_message(t('The library mapping was successfully deleted!'));
}
