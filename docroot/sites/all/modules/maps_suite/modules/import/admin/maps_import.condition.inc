<?php

/**
 * @file
 * Contains all admin forms for conditions management.
 */

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Filter\Condition\ConditionInterface;
use Drupal\maps_import\Filter\Filter;
use Drupal\maps_import\Filter\FilterInterface;
use Drupal\maps_import\Profile\Profile;

/**
 * Multistep form builder; add new filter condition form.
 *
 * This form is based on cTools wizard and cTools cache.
 *
 * @ingroup forms
 */
function maps_import_converter_filter_add_condition_form($form, &$form_state, Profile $profile, ConverterInterface $converter, $type = '') {
  drupal_set_title(t('Add filter for converter @converter', array('@converter' => $converter->getTitle())));

  ctools_include('wizard');

  $form_state = array(
    'build_info' => array('args' => array($profile, $converter)),
    'converter' => $converter,
    'type' => !empty($type) ? $type : NULL,
  );

  $step = isset($form_state['type']) ? 'criteria' : NULL;

  $form_info = array(
    'id' => 'maps_import_converter_filter_add_condition_form',
    'path' => 'admin/maps-suite/profiles/' . $profile->getName() . '/' . $converter->getType() . '/' . $converter->getCid() . '/filters',
    'show trail' => TRUE,
    'next text' => t('Continue'),
    'show back' => FALSE,
    'show cancel' => FALSE,
    'show return' => FALSE,
    'order' => array(
      'type' => t('Condition type'),
      'criteria' => t('Criteria'),
    ),
    'forms' => array(
      'type' => array(
        'form id' => 'maps_import_converter_filter_add_condition_type_form',
      ),
      'criteria' => array(
        'form id' => 'maps_import_converter_filter_add_condition_criteria_form',
      ),
    ),
  );

  return ctools_wizard_multistep_form($form_info, $step, $form_state);
}

/**
 * First step: selection of the condition type; add new filter condition form.
 *
 * @see maps_import_converter_filter_add_condition_type_form_submit()
 * @ingroup forms
 */
function maps_import_converter_filter_add_condition_type_form($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  $types = array();

  foreach ($converter->getFilter()->getAvailableConditions() as $key => $value) {
    $types[$key] = $value['title'];
  }

  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Condition type'),
    '#options' => $types,
    '#required' => TRUE,
  );

  return $form;
}

/**
 * First step submission; add new filter condition form.
 *
 * @see maps_import_converter_filter_add_condition_type_form()
 */
function maps_import_converter_filter_add_condition_type_form_submit($form, &$form_state) {
  drupal_goto('admin/maps-suite/profiles/' . $form_state['converter']->getProfile()->getName() . '/' . $form_state['converter']->getType() . '/' . $form_state['converter']->getCid() . '/filters/add/' . $form_state['input']['type']);
}

/**
 * Last step: definition of the condition criteria.
 *
 * Add new filter condition form.
 *
 * @see maps_import_converter_filter_add_condition_criteria_form_submit()
 * @ingroup forms
 */
function maps_import_converter_filter_add_condition_criteria_form($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  $conditions = $converter->getFilter()->getAvailableConditions();
  $condition = new $conditions[$form_state['type']]['class']($converter);
  return $condition->generateForm($form, $form_state);
}

/**
 * Form submission handler; .
 *
 * @see maps_import_converter_filter_add_condition_criteria_form()
 */
function maps_import_converter_filter_add_condition_criteria_form_submit($form, &$form_state) {
  $form_state['condition']->formSave($form, $form_state);
  drupal_set_message(t('The new criteria has been created.'));
  $form_state['redirect'] = 'admin/maps-suite/profiles/' . $form_state['converter']->getProfile()->getName() . '/' . $form_state['converter']->getType() . '/' . $form_state['converter']->getCid() . '/filters';
}

/**
 * Form builder; converter filters form.
 *
 * @see maps_import_converter_conditions_overview_form_validate()
 * @see maps_import_converter_conditions_overview_form_submit()
 * @ingroup forms
 */
function maps_import_converter_conditions_overview_form($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  drupal_set_title(t('Filters for converter @converter', array('@converter' => $converter->getTitle())));

  drupal_set_title($converter->getTitle());

  $form['#converter'] = $converter;

  $pid = $profile->getPid();
  $cid = $converter->getCid();

  foreach ($converter->getFilter()->getFlattenConditions() as $condition) {
    $id = $condition->getId();

    $form['filters'][$id] = array(
      '#depth' => $condition->depth,
      '#container' => $condition->isContainer(),
      '#weight' => $condition->getWeight(),
    );

    $form['filters'][$id]['id'] = array('#type' => 'hidden', '#value' => $id);
    $form['filters'][$id]['parent_id'] = array('#type' => 'hidden', '#default_value' => $condition->getParentId());

    $form['filters'][$id]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => $form['filters'][$id]['#weight'],
    );

    $form['filters'][$id]['depth'] = array(
      '#type' => 'hidden',
      '#default_value' => $form['filters'][$id]['#depth'],
    );

    $form['filters'][$id]['label'] = array(
      '#theme' => 'html_tag',
      '#tag' => 'div',
      '#value' => $condition->getLabel(),
    );

    $form['filters'][$id]['operations'] = array(
      '#theme' => 'links',
      '#links' => array(
        'edit-condition' => array(
          'title' => t('edit'),
          'href' => "admin/maps-suite/profiles/$pid/" . $converter->getType() . "/$cid/filters/$id/edit",
        ),
        'delete-condition' => array(
          'title' => t('delete'),
          'href' => "admin/maps-suite/profiles/$pid/" . $converter->getType() . "/$cid/filters/$id/delete",
        ),
      ),
    );
  }

  if (!empty($form['filters'])) {
    $form['filters']['#tree'] = TRUE;

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 100,
    );
  }

  return $form;
}

/**
 * Form validation handler; MaPS Import Conditions overview.
 *
 * Check for each condition if its depth does not exceed the max definied depth.
 */
function maps_import_converter_conditions_overview_form_validate($form, &$form_state) {
  if (empty($form['filters'])) {
    return TRUE;
  }

  foreach ($form_state['values']['filters'] as $filter) {
    if (isset($filter['depth']) && $filter['depth'] >= FilterInterface::CONDITION_MAX_DEPTH) {
      form_set_error('depth', t('The filter %id depth is too high (maximum: %max)',
        array('%id' => $filter['id'], '%max' => FilterInterface::CONDITION_MAX_DEPTH)));
    }
  }
}

/**
 * Form submission handler; converter filters form.
 *
 * @see maps_import_converter_conditions_overview_form()
 * @see maps_import_converter_conditions_overview_form_validate()
 */
function maps_import_converter_conditions_overview_form_submit($form, &$form_state) {
  $values = $form_state['values']['filters'];

  foreach ($form['#converter']->getFilter()->getFlattenConditions() as $condition) {
    $id = $condition->getId();

    if ($condition->getWeight() != $values[$id]['weight'] || $condition->getParentId() != $values[$id]['parent_id']) {
      $condition->setWeight($values[$id]['weight']);
      $condition->setParentId($values[$id]['parent_id']);
      $condition->save();
    }
  }

  drupal_set_message(t('The changes have been saved.'));
}

/**
 * Return HTML for the filter form overview.
 */
function theme_maps_import_converter_conditions_overview_form($variables) {
  $form = $variables['form'];
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => array(t('Condition'), t('Weight'), t('Operations')),
    '#rows' => array(),
    '#attributes' => array('id' => 'maps-import-converter-filter-conditions'),
    '#empty' => t('There is no condition defined yet.'),
  );

  if (!empty($form['filters'])) {
    foreach (element_children($form['filters']) as $key) {
      $element = &$form['filters'][$key];
      $element['parent_id']['#attributes']['class'] = array('condition-parent-id');
      $element['id']['#attributes']['class'] = array('condition-element-id');
      $element['weight']['#attributes']['class'] = array('condition-element-weight');
      $element['depth']['#attributes']['class'] = array('condition-element-depth');

      $row = array();
      $row[] = theme('indentation', array('size' => $element['#depth'])) . drupal_render($element['label']);

      $row[] = drupal_render($element['depth']) . drupal_render($element['weight']) . drupal_render($element['parent_id']) . drupal_render($element['id']);
      $row[] = array('class' => 'condition-operations', 'data' => drupal_render($element['operations']));

      $row = array('data' => $row) + $element['#attributes'];
      $row['class'][] = 'draggable';

      if (!$element['#container']) {
        $row['class'][] = 'tabledrag-leaf';
      }

      $form['table']['#rows'][] = $row;
    }
  }

  if ($form['table']['#rows']) {
    drupal_add_tabledrag($form['table']['#attributes']['id'], 'match', 'parent', 'condition-parent-id', 'condition-parent-id', 'condition-element-id', TRUE, Filter::CONDITION_MAX_DEPTH);
    drupal_add_tabledrag($form['table']['#attributes']['id'], 'order', 'sibling', 'condition-element-weight');
    drupal_add_tabledrag($form['table']['#attributes']['id'], 'depth', 'group', 'condition-element-depth', NULL, NULL, FALSE);
  }

  uasort($form, 'element_sort');

  return drupal_render_children($form);
}


/**
 * Form builder; add condition operator form.
 *
 * @see maps_import_converter_filter_add_operator_form_submit()
 * @ingroup forms
 */
function maps_import_converter_filter_add_operator_form($form, &$form_state, Profile $profile, ConverterInterface $converter, $op) {
  drupal_set_title(t('Add operator for converter @converter', array('@converter' => $converter->getTitle())));

  $class = 'Drupal\\maps_import\\Filter\\Condition\\Operator\\Operator' . ucfirst($op);

  $condition = new $class($converter, array());

  $form['wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add @operator operator', array('@operator' => strtoupper($op))),
  ) + $condition->conditionForm($form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}


/**
 * Form validation handler; MaPS Import Conditions overview.
 *
 * Check for each condition if its depth does not exceed the max definied depth.
 *
 * @see maps_import_converter_check_depth
 */
function maps_import_converter_filter_add_operator_form_validate($form, &$form_state) {
  maps_import_converter_check_depth($form, $form_state);
}

/**
 * Form submission handler; add condition operator form.
 *
 * @see maps_import_converter_filter_add_operator_form()
 */
function maps_import_converter_filter_add_operator_form_submit($form, &$form_state) {
  $class = 'Drupal\\maps_import\\Filter\\Condition\\Operator\\Operator' . ucfirst($form_state['build_info']['args'][2]);
  $condition = new $class($form_state['converter'], $form_state['values']);

  $condition->formSave($form, $form_state);

  drupal_set_message(t('The new operator has been created.'));
  $form_state['redirect'] = 'admin/maps-suite/profiles/' . $form_state['converter']->getProfile()->getName() . '/' . $form_state['converter']->getType() . '/' . $form_state['converter']->getCid() . '/filters';
}

/**
 * Form builder; condition edit form.
 *
 * @see maps_import_converter_filter_edit_condition_form_submit()
 * @ingroup forms
 */
function maps_import_converter_filter_edit_condition_form($form, &$form_state, Profile $profile, ConverterInterface $converter, ConditionInterface $condition) {
  drupal_set_title(t('Edit filter @condition for converter @converter', array('@converter' => $converter->getTitle(), '@condition' => $condition->getTitle())));

  $form['wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Edit condition of type %type', array('%type' => $condition->getType())),
  ) + $condition->conditionForm($form, $form_state, $converter);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['type'] = array(
    '#type' => 'hidden',
    '#value' => $condition->getType(),
  );

  return $form;
}

/**
 * Form validation handler; MaPS Import Conditions edition form.
 *
 * Check if the condition depth does not exceed the max definied depth.
 *
 * @see maps_import_converter_check_depth
 */
function maps_import_converter_filter_edit_condition_form_validate($form, &$form_state) {
  maps_import_converter_check_depth($form, $form_state);
}

/**
 * Form submission handler; condition edit form.
 *
 * @see maps_import_converter_filter_edit_condition_form()
 */
function maps_import_converter_filter_edit_condition_form_submit($form, &$form_state) {
  $conditions = $form_state['converter']->getFilter()->getAvailableConditions();

  // Get the related class.
  if (isset($conditions[$form_state['values']['type']])) {
    $class = $conditions[$form_state['values']['type']]['class'];
  }
  else {
    // @todo should this be skipped and throw an exception?
    $class = 'Drupal\\maps_import\\Filter\\Condition\\' . ucfirst($form_state['values']['type']);
  }

  if (class_exists($class)) {
    $condition = new $class($form_state['converter'], $form_state['values']);

    $condition->formSave($form, $form_state);
  }

  drupal_set_message(t('The changes have been saved.'));
  $form_state['redirect'] = 'admin/maps-suite/profiles/' . $form_state['converter']->getProfile()->getName() . '/' . $form_state['converter']->getType() . '/' . $form_state['converter']->getCid() . '/filters';
}

/**
 * Form builder; delete condition confirm form.
 *
 * @see maps_import_converter_filter_delete_condition_form_submit()
 * @ingroup forms
 */
function maps_import_converter_filter_delete_condition_form($form, &$form_state, Profile $profile, ConverterInterface $converter, ConditionInterface $condition) {
  drupal_set_title(t('Delete filter @condition for converter @converter', array('@converter' => $converter->getTitle(), '@condition' => $condition->getTitle())));

  $form_state['converter'] = $converter;
  $form_state['condition'] = $condition;
  $form['id'] = array('#type' => 'value', '#value' => $condition->getId());

  $message = array();
  $message[] = t('Are you sure you want to delete the condition "!condition"? This action cannot be undone.', array('!condition' => $condition->getLabel()));

  if ($condition->getChildren()) {
    $message[] = t('All children that belong to current condition will also be deleted.');
  }

  return confirm_form(
    $form,
    t('Delete condition'),
    'admin/maps-suite/profiles/' . $profile->getName() . '/' . $converter->getType() . '/' . $converter->getCid() . '/filters',
    implode(' ', $message),
    t('Delete'),
    t('Cancel'));
}

/**
 * Form submission handler; delete condition confirm form.
 *
 * @see maps_import_converter_filter_delete_condition_form()
 */
function maps_import_converter_filter_delete_condition_form_submit($form, &$form_state) {
  if (!$count = $form_state['condition']->delete()) {
    drupal_set_message(t('An error occured while deleting the condition.'));
  }
  else {
    drupal_set_message(t('The condition has been deleted.'));

    if (--$count) {
      drupal_set_message(format_plural($count, 'A child condition was also deleted.', '@count children conditions were also deleted.'));
    }
  }

  $form_state['redirect'] = 'admin/maps-suite/profiles/' . $form_state['converter']->getProfile()->getName() . '/' . $form_state['converter']->getType() . '/' . $form_state['converter']->getCid() . '/filters';
}

/**
 * Check the condition depth before editing / submitting.
 */
function maps_import_converter_check_depth($form, &$form_state) {
  if ($form_state['values']['parent_id'] == 0) {
    return TRUE;
  }

  foreach ($form_state['converter']->getFilter()->getFlattenConditions() as $condition) {
    if ($condition->getId() == $form_state['values']['parent_id']) {
      if (($condition->depth + 1) >= FilterInterface::CONDITION_MAX_DEPTH) {
        form_set_error('depth', t('The filter depth is too high (maximum: %max)',
          array('%max' => FilterInterface::CONDITION_MAX_DEPTH)));
        break;
      }
    }
  }
}
