<?php

/**
 * @file
 * Contains all admin forms for relations management.
 */

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Converter\Child\Relation as RelationConverter;
use Drupal\maps_import\Profile\Profile;
use Drupal\maps_relation\Relation\Relation;

/**
 * Form builder; Relations overview.
 */
function maps_relation_relation_overview($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  $relations = Relation::loadAll($converter);

  $replace = array(
    '%maps_import_profile' => $profile->getName(),
    '%maps_import_converter' => $converter->getCid(),
    '%maps_import_converter_type' => $converter->getType(),
  );

  foreach ($relations as $i => $relation) {
    $replace['%maps_import_relation'] = $relation->getId();
    $replace['%maps_import_mapping_converter'] = $relation->getMappingConverter()->getCid();

    $form['relations'][$i]['relation_id'] = array(
      '#theme' => 'html_tag',
      '#tag' => 'div',
      '#value' => $relation->getId(),
    );

    $form['relations'][$i]['relation_type'] = array(
      '#theme' => 'html_tag',
      '#tag' => 'div',
      '#value' => $relation->getRelationType(),
    );

    $form['relations'][$i]['links'] = array(
      '#theme' => 'links',
    );

    $form['relations'][$i]['links']['#links']['relation-endpoints'] = array(
      'href' => strtr('admin/maps-suite/profiles/%maps_import_profile/%maps_import_converter_type/%maps_import_converter/converter_relation/%maps_import_relation/endpoints', $replace),
      'title' => t('endpoints'),
    );

    $form['relations'][$i]['links']['#links']['relation-mapping'] = array(
      'href' => strtr('admin/maps-suite/profiles/%maps_import_profile/%maps_import_converter_type/%maps_import_mapping_converter/mapping', $replace),
      'title' => t('mapping'),
    );

    $form['relations'][$i]['links']['#links']['relation-delete'] = array(
      'href' => strtr('admin/maps-suite/profiles/%maps_import_profile/%maps_import_converter_type/%maps_import_converter/converter_relation/%maps_import_relation/delete', $replace),
      'title' => t('delete'),
    );
  }

  if (!empty($form['relations'])) {
    $form['relations']['#tree'] = TRUE;
  }

  return $form;
}


/**
 * Multistep form builder.
 *
 * This form is based on cTools wizard and cTools cache.
 *
 * @ingroup forms
 */
function maps_relation_relation_add_form($form, &$form_state, Profile $profile, ConverterInterface $converter, $type = '') {
  ctools_include('wizard');

  $form_state = array(
    'build_info' => array('args' => array($profile, $converter)),
    'converter' => $converter,
    'relation_type' => !empty($type) ? $type : NULL,
  );

  $step = isset($form_state['relation_type']) ? 'endpoints' : NULL;

  $form_info = array(
    'id' => 'maps_relation_relation_form',
    'path' => 'admin/maps-suite/profiles/' . $profile->getName() . '/' . $converter->getType() . '/' . $converter->getCid() . '/converter_relation',
    'show trail' => TRUE,
    'next text' => t('Continue'),
    'show back' => FALSE,
    'show cancel' => FALSE,
    'show return' => FALSE,
    'order' => array(
      'relation_type' => t('Relation type'),
      'endpoints' => t('Endpoints'),
    ),
    'forms' => array(
      'relation_type' => array(
        'form id' => 'maps_relation_relation_type_add_form',
      ),
      'endpoints' => array(
        'form id' => 'maps_relation_relation_endpoints_add_form',
      ),
    ),
  );

  return ctools_wizard_multistep_form($form_info, $step, $form_state);
}

/**
 * Multistep form builder.
 *
 * Builds the relation type form.
 */
function maps_relation_relation_type_add_form($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  $relation_types = relation_get_available_types($converter->getEntityType(), $converter->getBundle());
  if (empty($relation_types)) {
    drupal_set_message(t('No relation available for the converter bundle.'), 'error');
    drupal_goto('admin/maps-suite/profiles/' . $form_state['converter']->getProfile()->getName() . '/' . $form_state['converter']->getType() . '/' . $form_state['converter']->getCid() . '/converter_relation');
  }

  $form['relation_type'] = array(
    '#title' => t('Relation type'),
    '#description' => t('Select the relation type'),
    '#type' => 'select',
    '#options' => array('' => t('- Select -')) + maps_suite_reduce_array($relation_types, 'label', TRUE),
  );

  return $form;
}

/**
 * Form validation: relation type add.
 */
function maps_relation_relation_type_add_form_validate($form, &$form_state) {
  if (!array_key_exists($form_state['input']['relation_type'], relation_get_available_types($form_state['converter']->getEntityType(), $form_state['converter']->getBundle()))) {
    drupal_set_message(t('Relation type not available.'), 'error');
  }
}

/**
 * Form validation: relation type submit.
 */
function maps_relation_relation_type_add_form_submit($form, &$form_state) {
  drupal_goto('admin/maps-suite/profiles/' . $form_state['converter']->getProfile()->getName() . '/' . $form_state['converter']->getType() . '/' . $form_state['converter']->getCid() . '/converter_relation/add/' . $form_state['input']['relation_type']);
}

/**
 * Form builder; add endpoints to the relation.
 */
function maps_relation_relation_endpoints_add_form($form, &$form_state, Profile $profile, ConverterInterface $converter, Relation $relation = NULL) {
  if (!is_null($relation)) {
    $form_state['relation'] = $relation;
    $form_state['relation_type'] = $relation->getRelationType();
    $endpoints = $relation->getEndpoints();
  }

  // Check the min and max arity for the given relation type.
  $relation_type = relation_type_load($form_state['relation_type']);

  // Limit the arity to 5.
  $max_arity = $relation_type->max_arity <= 5 ? $relation_type->max_arity : 5;

  // @todo Attributs object
  // @todo related entities
  // @todo delayed entities
  $options = array('' => t('- Select -')) + Relation::getAvailableEndpoints($converter);

  $form['endpoints']['#tree'] = TRUE;

  for ($i = 0; $i < $max_arity; $i++) {
    $form['endpoints'][$i] = array(
      '#title' => t('r_index %rindex', array('%rindex' => $i)),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => !empty($endpoints[$i]) ? $endpoints[$i] : NULL,
    );
  }

  if (!is_null($relation)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit'),
    );
  }

  return $form;
}

/**
 * Form submit; add endpoints.
 */
function maps_relation_relation_endpoints_add_form_submit($form, &$form_state) {
  $values = array_merge($form_state['values'], $form_state['input']);

  if (empty($form_state['relation'])) {
    $relation = new Relation(array(
      'converter' => $form_state['converter'],
      'relation_type' => $form_state['relation_type'],
      'endpoints' => $values['endpoints'],
    ));

    $id = $relation->save();

    // Create the child converter.
    $record = array(
      'pid' => $relation->getConverter()->getProfile()->getPid(),
      'uid' => NULL,
      'uid_scope' => RelationConverter\RelationInterface::SCOPE_RELATION,
      'entity_type' => 'relation',
      'bundle' => $relation->getRelationType(),
      'options' => array('relation_id' => $id),
      'class' => 'Drupal\\maps_import\\Converter\\Child\\Relation\\' . ucfirst($relation->getConverter()->getType()),
      'name' => "relation_{$id}",
    );

    drupal_write_record('maps_import_converter', $record);
  }
  else {
    $relation = $form_state['relation'];
    $relation->setEndpoints($values['endpoints']);

    $relation->save();
  }

  $form_state['redirect'] = 'admin/maps-suite/profiles/' . $relation->getConverter()->getProfile()->getName() . '/' . $relation->getConverter()->getType() . '/' . $relation->getConverter()->getCid() . '/converter_relation';
}

/**
 * Return HTML for the relation form overview.
 */
function theme_maps_relation_relation_overview($variables) {
  $form = $variables['form'];
  $rows = array();

  if (isset($form['relations'])) {
    foreach (element_children($form['relations']) as $key) {
      $element = &$form['relations'][$key];

      $row = array();

      if (!empty($element['#errors'])) {
        $classes[] = 'error';
      }

      $row[] = drupal_render($element['relation_id']);
      $row[] = drupal_render($element['relation_type']);
      $row[] = drupal_render($element['links']);

      $rows[] = array(
        'data' => $row,
      );
    }
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('There is no relation mapping yet.'), 'colspan' => 4));
  }

  $output = theme('table', array(
    'header' => array(
      t('Id'),
      t('Relation type'),
      t('Actions'),
    ),
    'rows' => $rows,
    'attributes' => array('id' => 'maps-import-relation-overview'),
  ));

  return $output . drupal_render_children($form);
}

/**
 * Form builder; relation deletion form.
 *
 * @see maps_relation_relation_delete_form_submit()
 * @ingroup forms
 */
function maps_relation_relation_delete_form($form, &$form_state, Profile $profile, ConverterInterface $converter, Relation $relation) {
  $form['delete_entities'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete all generated Drupal Relations.'),
    '#default_value' => 0,
  );

  return confirm_form(
    $form,
    t('Delete relation @id for converter @converter', array('@converter' => $converter->getTitle(), '@id' => $relation->getId())),
    'admin/maps-suite/profiles/' . $profile->getName() . '/' . $converter->getType() . '/' . $converter->getCid() . '/converter_relation',
    '',
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form submission handler; relation form.
 *
 * @see maps_relation_relation_delete_form()
 */
function maps_relation_relation_delete_form_submit($form, &$form_state) {
  list($profile, $converter, $relation) = $form_state['build_info']['args'];

  $relation->delete($form_state['values']['delete_entities']);

  drupal_set_message(t('The relation was successfully deleted!'));
  $form_state['redirect'] = array(
    'admin/maps-suite/profiles/' . $profile->getName() . '/' . $converter->getType() . '/' . $converter->getCid() . '/converter_relation',
  );
}
