<?php

/**
 * @file
 * Include file for all Field UI specific functions.
 *
 * @author skilip
 */

/**
 * Menu callback; presents the field instance edit page.
 */
function page_break_field_edit_form($form, &$form_state, $page_break) {

  $form['#page_break'] = $page_break;

  // Create a form structure for the instance values.
  $form['page_break'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page break'),
  );

  // Build the configurable instance values.
  $form['page_break']['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => $page_break->label,
    '#required' => TRUE,
    '#weight' => -20,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save settings'));

  return $form;
}

/**
 * Form submit handler for field instance settings form.
 */
function page_break_field_edit_form_submit($form, &$form_state) {
  $page_break = $form['#page_break'];

  $page_break->label = $form_state['values']['label'];

  // Prepare storage with ctools.
  ctools_include('export');
  ctools_export_crud_save('page_break_field', $page_break);

  drupal_set_message(t('Saved %label configuration.', array('%label' => $page_break->label)));

  module_load_include('inc', 'field_ui', 'field_ui.admin');
  $form_state['redirect'] = field_ui_next_destination($page_break->entity_type, $page_break->bundle);
}

/**
 * Menu callback; present a form for removing a page break from a bundle.
 */
function page_break_field_delete_form($form, &$form_state, $page_break) {
  $form['#page_break'] = $page_break;

  $admin_path = _field_ui_bundle_admin_path($page_break->entity_type, $page_break->bundle);

  $output = confirm_form($form,
    t('Are you sure you want to delete the page break %page_break?', array('%page_break' => $page_break->label)),
    $admin_path . '/fields',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel'),
    'confirm'
  );

  return $output;
}

/**
 * Remove page break bundle.
 */
function page_break_field_delete_form_submit($form, &$form_state) {

  $page_break = $form['#page_break'];

  ctools_include('export');
  ctools_export_crud_delete('page_break_field', $page_break);
  /* dpm($page_break); */

  drupal_set_message(t('The page break %page_break has been deleted.', array('%page_break' => t($page_break->label))));

  module_load_include('inc', 'field_ui', 'field_ui.admin');
  $form_state['redirect'] = field_ui_next_destination($page_break->entity_type, $page_break->bundle);
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function page_break_field_field_ui_overview_form_alter(&$form, &$form_state) {

  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $mode = isset($form['#view_mode']) ? $form['#view_mode'] : 'form';

  $table = &$form['fields'];
  $name = '_add_page_break';

  $table['#parent_options'][$name] = t('Add new page break');

  $page_breaks = page_break_field_info_page_breaks($entity_type, $bundle, $mode, TRUE);
  $form['#page_breaks'] = $page_breaks;

  $parent_options = $table['#parent_options'];
  $table[$name] = array(
    '#attributes' => array('class' => array('draggable', 'page-break-field', 'add-new')),
    '#row_type' => 'add_new_page_break',
    '#js_settings' => array('rowHandler' => 'group'),
    '#region_callback' => 'page_break_field_display_overview_row_region',
    'label' => array(
      '#type' => 'textfield',
      '#size' => 15,
      '#description' => t('Label'),
      '#prefix' => '<div class="label-input"><div class="add-new-placeholder">' . t('Add new page break') . '</div>',
      '#suffix' => '</div>',
    ),
    'weight' => array(
      '#type' => 'textfield',
      '#default_value' => 10,
      '#size' => 3,
      '#title_display' => 'invisible',
      '#title' => t('Weight for new page break'),
      '#attributes' => array('class' => array('field-weight')),
      '#prefix' => '<div class="add-new-placeholder">&nbsp;</div>',
    ),
    'parent_wrapper' => array(
      'parent' => array(
        '#type' => 'select',
        '#options' => $parent_options,
        '#empty_value' => '',
        '#attributes' => array('class' => array('field-parent')),
        '#prefix' => '<div class="add-new-placeholder">&nbsp;</div>',
        '#parents' => array('fields', $name, 'parent'),
      ),
      'hidden_name' => array(
        '#type' => 'hidden',
        '#default_value' => $name,
        '#attributes' => array('class' => array('field-name')),
      ),
    ),
    'spacer' => array(
      '#markup' => '&nbsp;',
      '#cell_attributes' => array('colspan' => 5),
    ),
  );

  $admin_path = _field_ui_bundle_admin_path($entity_type, $bundle);

  foreach ($page_breaks as $name => $page_break) {
    $admin_page_break_field_path = $admin_path . '/page_break/' . $name;

    $table[$name] = array(
      '#attributes' => array('class' => array('draggable', 'field-group'), 'id' => $name),
      '#row_type' => 'page_break',
      '#region_callback' => 'page_break_field_display_overview_row_region',
      '#js_settings' => array('rowHandler' => 'group'),
      'label' => array(
        '#markup' => t($page_break->label),
        '#prefix' => '<span class="group-label"><em>',
        '#suffix' => '</em></span>',
      ),
      'weight' => array(
        '#type' => 'textfield',
        '#default_value' => $page_break->weight,
        '#size' => 3,
        '#attributes' => array('class' => array('field-weight')),
      ),
      'parent_wrapper' => array(
        'parent' => array(
          '#type' => 'select',
          '#options' =>  $parent_options,
          '#empty_value' => '',
          '#attributes' => array('class' => array('field-parent')),
          '#parents' => array('fields', $name, 'parent'),
        ),
        'hidden_name' => array(
          '#type' => 'hidden',
          '#default_value' => $name,
          '#attributes' => array('class' => array('field-name')),
        ),
      ),
      'field_name' => array(
        '#markup' => $name,
        '#cell_attributes' => array('colspan' => 3),
      ),
      'edit' => array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => $admin_page_break_field_path,
        '#options' => array('attributes' => array('title' => t('Edit instance settings.'))),
      ),
      'delete' => array(
        '#type' => 'link',
        '#title' => t('delete'),
        '#href' => $admin_page_break_field_path . '/delete',
        '#options' => array('attributes' => array('title' => t('Delete instance.'))),
      ),
    );
  }
  $form['#submit'][] = 'page_break_field_field_overview_submit';
}

/**
 * Returns the region to which a row in the 'Manage display' screen belongs.
 * @param Array $row A field or page_break_field row
 * @return String the current region.
 */
function page_break_field_display_overview_row_region($row) {
  switch ($row['#row_type']) {
    case 'page_break':
      return 'main';

    case 'add_new_page_break':
      // If no input in 'label', assume the row has not been dragged out of the
      // 'add new' section.
      if (empty($row['label']['#value'])) {
        return 'add_new';
      }
      return 'main';
  }
}

/**
 * Submit handler for the overview screens.
 * @param Array $form The complete form.
 * @param Array $form_state The state of the form.
 */
function page_break_field_field_overview_submit($form, &$form_state) {

  $form_values = $form_state['values']['fields'];
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $mode = (isset($form['#view_mode']) ? $form['#view_mode'] : 'form');

  // Prepare storage with ctools.
  ctools_include('export');

  // Create new page_break.
  if (!empty($form_values['_add_page_break']['label'])) {
    $values = $form_values['_add_page_break'];

    // TODO: create a valid unique ID.
    $index = count($form['#page_breaks']);

    $page_break_name = 'page_break_' . $index;

    $new_page_break = (object) array(
      'identifier' => $page_break_name . '|' . $entity_type . '|' . $bundle . '|' . $mode,
      'page_break_name' => $page_break_name,
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'mode' => $mode,
      'weight' => $values['weight'],
      'label' => $values['label'],
    );

    ctools_export_crud_save('page_break_field', $new_page_break);

    drupal_set_message(t('New page break %label successfully created.', array('%label' => $new_page_break->label)));
  }

  // Update existing page breaks.
  foreach ($form['#page_breaks'] as $name => $page_break) {
    $page_break->weight = $form_values[$name]['weight'];
    ctools_export_crud_save('page_break_field', $page_break);
  }
  cache_clear_all('page_break_fields', 'cache_field');
}
