<?php
/**
 * @file
 * File containing configuration form for opac marc module.
 */

/**
 * Form constructor for opac marc configuration form.
 *
 * @ingroup forms
 */
function opac_marc_structure_overview($form, &$form_state) {
  module_load_include('inc', 'opac_marc', "opac_marc.db");

  $structures = array_merge(opac_marc_structure_types_get(), array('none' => 'None'));
  $form['opac_marc_default_structure_type'] = array(
    '#type' => 'select',
    '#title' => t('Type of marc struture', array(), array('context' => 'libraries')),
    '#options' => $structures,
    '#default_value' => variable_get('marc_default_structure_type', 'none'),
    '#description' => t('The marc type structure that marc module will use for marc displaying.', array(), array('context' => 'libraries')),
  );

  $form['marc_edit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Marc edit', array(), array('context' => 'libraries')),
    '#description' => t('Select the marc struture you want to edit:', array(), array('context' => 'libraries')),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  unset($structures['none']);
  foreach ($structures as $type => $label) {
    $form['marc_edit'][$type] = array(
      '#markup' => "<div><b>$label</b> structure (" . l(t('edit'), "admin/config/opac/marc/settings/structure/$type") . ")</div>",
    );
  }

  return system_settings_form($form);
}

/**
 * Form constructor for editing marc structure.
 *
 * @param string $type
 *   Marc structure type to edit.
 *
 * @see user_login_form_submit()
 * @ingroup forms
 */
function opac_marc_structure_edit_form($form, &$form_state, $type) {
  module_load_include('inc', 'opac_marc', "opac_marc.db");

  $form['type'] = array('#type' => 'hidden', '#value' => $type);
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => array(
      'hide' => 'Hide selected tags',
      'show' => 'Show selected tags',
      'delete' => 'Delete selected tags',
    ),
    '#default_value' => 'show',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#submit' => array('opac_marc_structure_edit_form_submit'),
  );

  $headers = array(
    'tag' => array('data' => t('Tag'), 'field' => 'tag'),
    'subfield' => array(
      'data' => t('Subfield', array(), array('context' => 'libraries')),
      'field' => 'subfield',
    ),
    'label' => array('data' => t('Label'), 'field' => 'label'),
    'hidden' => array('data' => t('Hidden ?'), 'field' => 'hidden'),
    'op' => array('data' => t('Operations'), 'field' => 'op'),
  );

  $options = array();
  $query = db_select('opac_marc_subfield_structure', 't')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('t')
    ->condition('t.structure_type', $type, '=')
    ->limit(15);

  $result = $query->execute();
  while ($element = $result->fetchAssoc()) {
    $id = $element['tag'] . "$" . $element['subfield'];
    if ($element['subfield'] == '@') {
      $options[$id] = array(
        'tag' => "<b>" . $element['tag'] . "</b>",
        'subfield' => "<b>" . $element['subfield'] . "</b>",
        'label' => "<b>" . $element['label'] . "</b>",
        'hidden' => $element['hidden'] ? t('Yes') : t('No'),
        'op' => l(t('edit'), '/'),
      );
    }
    else {
      $options[$id] = array(
        'tag' => $element['tag'],
        'subfield' => $element['subfield'],
        'label' => $element['label'],
        'hidden' => $element['hidden'] ? t('Yes') : t('No'),
        'op' => l(t('edit'), '/'),
      );
    }
  }

  $form['marc_structure'] = array(
    '#type' => 'tableselect',
    '#header' => $headers,
    '#options' => $options,
    '#empty' => t('No content available.'),
  );

  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}

/**
 * Form submission handler opac_marc_structure_edit_form().
 */
function opac_marc_structure_edit_form_submit($form, &$form_state) {
  $func = 'opac_marc_structure_subfields_' . $form_state['values']['operation'];
  $type = $form_state['values']['type'];
  $elements = $form_state['values']['marc_structure'];

  foreach ($elements as $element => $checked) {
    if ($checked) {
      list($tag, $subfield) = explode('$', $element);
      if (function_exists($func)) {
        $func($type, $tag, $subfield);
      }
    }
  }
}

/**
 * Form constructor for adding marc type.
 *
 * @see opac_marc_type_add_form_submit()
 * @ingroup forms
 */
function opac_marc_type_add_form($form, &$form_state) {
  module_load_include('inc', 'opac_marc', "opac_marc.db");
  $form['structure_label'] = array(
    '#type' => 'textfield',
    '#size' => 25,
    '#title' => t('Structure label'),
    '#default_value' => '',
    '#description' => t('Enter here the name of the marc structure.', array(), array('context' => 'libraries')),
    '#required' => TRUE,
  );
  $form['structure_type'] = array(
    '#type' => 'machine_name',
    '#default_value' => '',
    '#maxlength' => 255,
    '#machine_name' => array(
      'exists' => 'marc_structure_type_get',
      'source' => array('structure_label'),
    ),
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  return $form;
}

/**
 * Form submission handler for opac_marc_type_add_form().
 */
function opac_marc_type_add_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  opac_marc_structure_type_add($values);
  drupal_goto('admin/config/opac/marc/settings');
}

/**
 * Form constructor for adding marc tag form.
 *
 * @see opac_marc_tag_add_form_submit()
 * @ingroup forms
 */
function opac_marc_tag_add_form($form, &$form_state) {
  module_load_include('inc', 'opac_marc', "opac_marc.db");
  $form['tag'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('Tag'),
    '#default_value' => '',
    '#description' => t('Enter here the tag number. i.e 099'),
    '#required' => TRUE,
  );
  $form['subfield'] = array(
    '#type' => 'textfield',
    '#size' => 3,
    '#title' => t('Subfield', array(), array('context' => 'libraries')),
    '#default_value' => '',
    '#description' => t("Enter here the subfield. i.e 'a'", array(), array('context' => 'libraries')),
    '#required' => TRUE,
  );
  $form['structure_type'] = array(
    '#type' => 'select',
    '#title' => t('Structure type'),
    '#default_value' => 'none',
    '#options' => opac_marc_structure_types_get() + array('none' => 'None'),
    '#description' => t('You must chose the structure type for which the tag is related.'),
    '#required' => TRUE,
  );
  $form['label'] = array(
    '#type' => 'textfield',
    '#size' => 30,
    '#title' => t('Label'),
    '#default_value' => '',
    '#description' => t("Enter here the label. i.e 'International Standard Book Number'", array(), array('context' => 'libraries')),
    '#required' => FALSE,
  );
  $form['hidden'] = array(
    '#type' => 'checkbox',
    '#default_value' => 0,
    '#description' => t('You can hid a tag instead of delete it'),
    '#title' => t('Hidden ?'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form submission handler for opac_marc_tag_add_form().
 */
function opac_marc_tag_add_form_submit($form, &$form_state) {
  module_load_include('inc', 'opac_marc', "opac_marc.db");
  $values = $form_state['values'];
  opac_marc_structure_subfields_add($values);
  drupal_goto("admin/config/opac/marc/settings/structure/" . $values['structure_type']);
}
