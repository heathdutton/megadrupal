<?php
/**
 * @file
 * VS Conditions - administration forms.
 */

/**
 * Form constructor for the conditions list form.
 *
 * Path: admin/structure/virtual-site/condition
 *
 * @see virtual_site_condition_menu()
 * @see theme_virtual_site_condition_list_form()
 * @see virtual_site_condition_list_form_submit()
 *
 * @ingroup forms
 */
function virtual_site_condition_list_form($form, &$form_state) {
  foreach (_virtual_site_condition_load() as $condition) {
    $form['conditions'][$condition->cid] = array(
      '#tree' => TRUE,
      'name' => array(
        '#type' => 'item',
        '#markup' => $condition->name,
      ),
      'status' => array(
        '#type' => 'checkbox',
        '#default_value' => $condition->status,
      ),
      'weight' => array(
        '#type' => 'weight',
        '#default_value' => $condition->weight,
        '#delta' => 50,
      ),
      'edit' => array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => 'admin/structure/virtual-site/condition/'. $condition->cid .'/edit',
      ),
      'delete' => array(
        '#type' => 'link',
        '#title' => t('delete'),
        '#href' => 'admin/structure/virtual-site/condition/'. $condition->cid .'/delete',
      ),
    );
  }

  $form['add'] = array(
    '#type' => 'link',
    '#title' => t('Add new condition'),
    '#href' => 'admin/structure/virtual-site/condition/add',
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );

  $form['#theme'] = 'virtual_site_condition_list_form';
  $form['#submit'][] = 'virtual_site_condition_list_form_submit';
  return system_settings_form($form);
}

/**
 * Returns HTML for the conditions list form.
 * Turn the form into a table with tablesorting on.
 *
 * @ingroup themeable
 */
function theme_virtual_site_condition_list_form($variables) {
  $form = &$variables['form'];

  $rows = array();
  if (isset($form['conditions'])) {
    foreach (element_children($form['conditions']) as $cid) {
      $element = &$form['conditions'][$cid];
      $element['weight']['#attributes']['class'][] = 'weight';
      $row = array();
      $row[] = render($element['name']);
      $row[] = array('data' => render($element['status']), 'class' => 'checkbox');
      $row[] = render($element['weight']);
      $row[] = render($element['edit']);
      $row[] = render($element['delete']);
      $rows[] = array(
        'data' => $row,
        'class' => array('draggable'),
        'id' => 'condition-list-row-'. $cid,
      );
    }
  }

  $header = array(
    t('Condition'),
    array('data' => t('Enabled'), 'class' => 'checkbox'),
    t('Weight'),
    array('data' => t('Operations'), 'colspan' => '2'),
  );

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No conditions are defined.'), 'colspan' => '6'));
  }

  drupal_add_tabledrag('order-conditions', 'order', 'sibling', 'weight');
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'id' => 'order-conditions',
      'style' => 'width: 100%',
    ),
  ));
  $output .= drupal_render_children($form);
  $output .= theme('pager');
  return $output;
}

/**
 * Form submission handler for virtual_site_condition_list_form().
 */
function virtual_site_condition_list_form_submit($form, &$form_state) {
  foreach (element_children($form['conditions']) as $cid) {
    db_update('virtual_site_conditions')
      ->fields(array(
        'status' => $form_state['values'][$cid]['status'],
        'weight' => $form_state['values'][$cid]['weight'],
        ))
      ->condition('cid', $cid)
      ->execute();
  }
}

/**
 * Form constructor for the condition edit form.
 *
 * Path: admin/structure/virtual-site/condition/add
 * Path: admin/structure/virtual-site/condition/%/edit
 *
 * @param int $cid
 *   (Optional) The condition to edit.
 *
 * @see virtual_site_condition_menu()
 * @see virtual_site_condition_edit_form_submit()
 *
 * @ingroup forms
 */
function virtual_site_condition_edit_form($form, &$form_state, $cid = NULL) {
  if (isset($cid)) {
    $condition = _virtual_site_condition_load($cid);
    $form['cid'] = array(
      '#type' => 'hidden',
      '#value' => $cid,
    );

    drupal_set_title(t('Edit condition %name', array('%name' => check_plain($condition->name))), PASS_THROUGH);
  }
  else {
    drupal_set_title(t('Add condition'));
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Name to identify the condition with.'),
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => isset($condition->name) ? $condition->name : '',
  );

  module_load_include('inc', 'virtual_site_condition', 'virtual_site_condition.basic');
  $requirements = virtual_site_condition_requirement_info();
  if (is_array($requirements) && count($requirements)) {
    foreach ($requirements as $name => $requirement) {
      $form['requirements'][$name] = array(
        '#type' => 'fieldset',
        '#title' => t('Requirement: @name', array('@name' => $requirement['name'])),
        '#description' => check_plain($requirement['description']),
        '#collapsible' => TRUE,
        '#collapsed' => empty($condition->parameters[$name]),
      );

      $form_function = $name .'_form';
      $context = isset($condition->parameters[$name]) ? $condition->parameters[$name] : array();
      $form['requirements'][$name] = array_merge($form['requirements'][$name], $form_function($context));
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form submission handler for virtual_site_condition_edit_form().
 */
function virtual_site_condition_edit_form_submit($form, &$form_state) {
  $condition = isset($form['cid']['#value']) ? _virtual_site_condition_load($form['cid']['#value']) : new stdClass();
  $condition->name = $form_state['values']['name'];

  foreach (element_children($form['requirements']) as $requirement) {
    $submit_function = $requirement .'_form_submit';
    $condition->parameters[$requirement] = $submit_function($form, $form_state);
  }

  _virtual_site_condition_save($condition);
  drupal_set_message(t('The condition %name has been saved.', array('%name' => $condition->name)));
  $form_state['redirect'] = 'admin/structure/virtual-site/condition';
}

/**
 * Form constructor for the condition delete form.
 *
 * Path: admin/structure/virtual-site/condition/%/delete
 *
 * @param int $cid
 *   (Optional) The condition to delete.
 *
 * @see virtual_site_condition_menu()
 * @see virtual_site_condition_delete_form_submit()
 *
 * @ingroup forms
 */
function virtual_site_condition_delete_form($form, &$form_state, $cid) {
  $form['cid'] = array(
    '#type' => 'hidden',
    '#value' => $cid,
  );

  $condition = _virtual_site_condition_load($cid);
  return confirm_form($form,
    t('Are you sure you wan to delete the condition "!name"?', array('!name' => $condition->name)),
    'admin/structure/virtual-site/condition',
    NULL,
    t('Delete')
    );
}

/**
 * Form submission handler for virtual_site_condition_delete_form().
 */
function virtual_site_condition_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    _virtual_site_condition_delete($form_state['values']['cid']);
    drupal_set_message(t('The condition has been deleted.'));
  }
  $form_state['redirect'] = 'admin/structure/virtual-site/condition';
}
