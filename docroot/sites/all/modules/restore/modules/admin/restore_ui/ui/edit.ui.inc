<?php
/**
 * @file
 * The edit script UI form.
 */

/**
 * The edit form callback.
 */
function restore_ui_script_edit($form, &$form_state, $script) {
  $form = array();

  $form['tabs'] = array(
    '#type' => 'vertical_tabs',
    '#default_tab' => 'details',
  );

  $form['tabs']['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Script details'),
    '#weight' => -10,
  );

  $form['tabs']['details']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $script->title(),
  );

  $form['tabs']['details']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Descripton'),
    '#default_value' => $script->description(),
  );

  $form['tabs']['details']['group'] = array(
    '#type' => 'textfield',
    '#title' => t('Group'),
    '#default_value' => $script->group(),
  );

  $form['tabs']['operations'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Operations'),
  );

  $available_operations = array(t('- Select an operation -'));
  $all_operations = restore_operations();
  foreach ($all_operations as $name => $op) {
    $available_operations[$name] = t('@title - @description', array(
      '@title' => $op['title'],
      '@description' => $op['description'],
    ));
  }

  $form['tabs']['operations']['__add_operation'] = array(
    '#tree' => TRUE,
    '#prefix' => '<div class="add-operation"><h2>' . t('Add an operation') . '</h2>',
    '#suffix' => '</div>',
    '#title' => t('Add an operation'),
  );

  $form['tabs']['operations']['__add_operation']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#options' => $available_operations,
  );

  $form['tabs']['operations']['__add_operation']['add'] = array(
    '#type' => 'submit',
    '#name' => 'add_operation',
    '#value' => t('Add operation'),
    '#ajax' => array(
      'wrapper' => 'operations-tab-content',
      'callback' => 'restore_ui_script_edit_add_operation_ajax',
    ),
  );

  $form['tabs']['operations']['contents'] = array(
    '#theme' => 'restore_ui_script_edit_operations',
    '#prefix' => '<div id="operations-tab-content">',
    '#suffix' => '</div>',
  );

  foreach ($script as $idx => $operation) {
    $form['tabs']['operations']['contents'][$idx + 1] = array('#tree' => TRUE);
    $form['tabs']['operations']['contents'][$idx + 1]['title'] = array(
      '#type' => 'markup',
      '#markup' => t('Operation: @title - @description', array(
        '@title' => $operation ? $operation->title() : t('Unknown'),
        '@description' => $operation ? $operation->description() : t('This is an unknown operation.'),
      )),
    );

    $form['tabs']['operations']['contents'][$idx + 1]['details'] = array(
      '#type' => 'markup',
      '#prefix' => '<div id="operation-' . ($idx + 1) . '-summary">',
      '#suffix' => '</div>',
      '#markup' => implode(' ', $operation->summary()),
    );

    $form['tabs']['operations']['contents'][$idx + 1]['selection'] = array(
      '#tree' => TRUE,
      '#prefix' => '<div class="operation-selection operation-' . $operation->type() . '">',
      '#suffix' => '</div>',
    );

    $form['tabs']['operations']['contents'][$idx + 1]['selection']['filter'] = array(
      '#tree' => TRUE,
      '#prefix' => '<div class="operation-selection-filter">',
      '#suffix' => '</div>',
    );

    $form['tabs']['operations']['contents'][$idx + 1]['selection']['filter']['label'] = array(
      '#type' => 'textfield',
      '#attributes' => array(
        'placeholder' => t('Filter'),
      ),
      '#ajax' => array(
        'wrapper' => 'operation-' . ($idx + 1) . '-selection-items',
        'callback' => 'restore_ui_script_edit_add_operation_item_filter_ajax',
        'progress' => 'throbber',
        'keypress' => true,
      ),
    );

    $form['tabs']['operations']['contents'][$idx + 1]['selection']['filter']['selected'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show selected'),
      '#ajax' => array(
        'wrapper' => 'operation-' . ($idx + 1) . '-selection-items',
        'callback' => 'restore_ui_script_edit_add_operation_item_filter_ajax',
        'progress' => 'throbber',
      ),
    );

    $form['tabs']['operations']['contents'][$idx + 1]['selection']['items'] = array(
      '#prefix' => '<div id="operation-' . ($idx + 1) . '-selection-items" class="operation-selection-items">',
      '#suffix' => '</div>',
    );

    $filter = isset($form_state['values']['operations']['contents'][$idx + 1]['selection']['filter']['label']) ?
      $form_state['values']['operations']['contents'][$idx + 1]['selection']['filter']['label'] :
      FALSE;

    $show_selected = isset($form_state['values']['operations']['contents'][$idx + 1]['selection']['filter']['selected']) ?
      $form_state['values']['operations']['contents'][$idx + 1]['selection']['filter']['selected'] :
      FALSE;

    $options = $operation->available($filter, $show_selected);
    if (count($options)) {
      ksort($options);
      foreach ($options as $id => $label) {
        $form['tabs']['operations']['contents'][$idx + 1]['selection']['items'][$id] = array(
          '#type' => 'checkbox',
          '#title' => $label,
          '#ajax' => array(
            'callback' => 'restore_ui_script_edit_add_operation_item_ajax',
            'progress' => 'throbber',
            'event' => 'change',
          ),
          '#default_value' => $operation->has($id),
        );
      }
    }
    else {
      $form['tabs']['operations']['contents'][$idx + 1]['selection']['items']['__none'] = array(
        '#type' => 'markup',
        '#markup' => t('There are no items available options.'),
      );
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('restore_ui_script_edit_save'),
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'restore') . '/assets/css/restore.css';
  $form['#attached']['css'][] = drupal_get_path('module', 'restore_ui') . '/assets/css/restore-ui.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'restore') . '/assets/js/restore.js';
  $form['#attached']['js'][] = drupal_get_path('module', 'restore_ui') . '/assets/js/restore-ui.js';

  return $form;
}

/**
 * The save button callback.
 */
function restore_ui_script_edit_save($form, &$form_state) {
  $script = $form_state['build_info']['args'][0];
  if (restore_ui_save_script($script)) {
    drupal_set_message(t('The restore script has been saved successfully.'));
  }
  else {
    drupal_set_message(t('There was a problem saving the restore script.'), 'error');
  }
}

/**
 * The "Add operation" AJAX callback.
 */
function restore_ui_script_edit_add_operation_ajax($form, &$form_state) {
  if (!empty($form_state['values']['operations']['__add_operation']['operation'])) {
    $form_state['rebuild'] = TRUE;

    $script = $form_state['build_info']['args'][0];
    $script[] = restore_operation($form_state['values']['operations']['__add_operation']['operation'], array());

    $form = drupal_rebuild_form($form['#form_id'], $form_state, $form);
  }

  return $form['tabs']['operations']['contents'];
}

/**
 * The operation filter AJAX callback.
 */
function restore_ui_script_edit_add_operation_item_filter_ajax($form, $form_state) {
  $form_state['rebuild'] = TRUE;

  $element = $form_state['triggering_element'];
  $op_idx = $element['#array_parents'][3];
  $form = drupal_rebuild_form($form['#form_id'], $form_state, $form);

  return $form['tabs']['operations']['contents'][$op_idx]['selection']['items'];
}

/**
 * The operation option AJAX callback.
 */
function restore_ui_script_edit_add_operation_item_ajax($form, $form_state) {
  $form_state['rebuild'] = TRUE;

  $script = $form_state['build_info']['args'][0];
  $element = $form_state['triggering_element'];

  $op_idx = $element['#array_parents'][3];
  $element['#value'] == FALSE ? $script[$op_idx - 1]->remove($element['#array_parents'][6]) : $script[$op_idx - 1]->add($element['#array_parents'][6]);

  $form = drupal_rebuild_form($form['#form_id'], $form_state, $form);

  $commands = array();
  $commands[] = ajax_command_html("#operation-{$op_idx}-summary", $form['tabs']['operations']['contents'][$op_idx]['details']['#markup']);
  $commands[] = ajax_command_replace("#operation-{$op_idx}-selection-items", drupal_render($form['tabs']['operations']['contents'][$op_idx]['selection']['items']));

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Form theme callback.
 */
function theme_restore_ui_script_edit_operations($variables) {
  $element = $variables['element'];
  $rows = array();

  foreach (element_children($element) as $op_group) {
    $rows[] = array(
      'data' => array(
        array(
          'data' => drupal_render($element[$op_group]['title']),
          'header' => TRUE,
        ),
      ),
      'no_striping' => TRUE,
      'class' => array('selection-header'),
      'data-op-group' => $op_group,
    );

    $rows[] = array(
      drupal_render($element[$op_group]['details']),
    );

    $rows[] = array(
      'data' => array(
        drupal_render($element[$op_group]['selection']),
      ),
      'class' => array('selection-row'),
      'data-op-group' => $op_group,
    );
  }

  return theme('table', array(
    'empty' => t('There are no operations.'),
    'header' => array(),
    'rows' => $rows,
  ));
}
