<?php

/**
 * Field index entity form.
 * @param array $form
 * @param array $form_state
 * @param FieldIndex $field_index
 * @param string $op
 */
function field_index_form($form, &$form_state, $field_index, $op = 'edit') {
  $form += array();

  if (!empty($form_state['values'])) {
    $field_index = entity_ui_controller('field_index')->entityFormSubmitBuildEntity($form, $form_state);
  }

  $form['label'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Label'),
      '#default_value' => isset($field_index->label) ? $field_index->label : '',
      '#description'   => t('The human-readable name of this field-index.'),
      '#required'      => TRUE,
      '#size'          => 30,
  );

  // Machine-readable type name.
  $form['name'] = array(
      '#type'          => 'machine_name',
      '#default_value' => isset($field_index->name) ? $field_index->name : '',
      '#maxlength'     => 32,
      '#disabled'      => $field_index->isLocked() && $op !== 'clone',
      '#machine_name'  => array(
          'exists' => 'field_index_load',
          'source' => array('label')
      ),
      '#description'   => t('A unique machine-readable name for this field-index. It must only contain lowercase letters, numbers, and underscores.'),
      '#ajax'          => array(
      ),
  );

  $form['published'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Published'),
      '#default_value' => isset($field_index->published) ? $field_index->published : 0,
  );

  $form['index_info'] = array(
      '#prefix' => '<div id="field-index-info-wrapper">',
      '#suffix' => '</div>',
  );

  // Entity type
  $entity_type_options = array('' => '--- ' . t('Select an entity type') . '  ---');
  foreach (field_index_controller()->getEntityTypes() as $type_name => $type_info) {
    $entity_type_options[$type_name] = $type_info['label'];
  }
  $form['index_info']['entity_type'] = array(
      '#type'          => 'select',
      '#title'         => t('Entity type'),
      '#required'      => TRUE,
      '#weight'        => -50,
      '#options'       => $entity_type_options,
      '#default_value' => isset($field_index->entity_type) ? $field_index->entity_type : '',
      '#ajax'          => array(
          'callback' => 'field_index_form_ajax_submit',
          'wrapper'  => 'field-index-info-wrapper',
      ),
  );

  // Bundle
  if (!empty($form['index_info']['entity_type']['#default_value'])) {
    $entity_info = entity_get_info($form['index_info']['entity_type']['#default_value']);
    $bundle_options = array();
    foreach ($entity_info['bundles'] as $name => $bundle) {
      $bundle_options[$name] = $bundle['label'];
    }

    $form['index_info']['bundle'] = array(
        '#type'          => 'select',
        '#title'         => t('Bundle'),
        '#required'      => TRUE,
        '#weight'        => -40,
        '#options'       => $bundle_options,
        '#default_value' => isset($field_index->bundle) ? $field_index->bundle : '',
        '#ajax'          => array(
            'callback' => 'field_index_form_ajax_submit',
            'wrapper'  => 'field-index-info-wrapper',
        ),
    );
  }

  // Field
  if (!empty($form['index_info']['bundle']['#default_value'])) {
    $field_options = array('' => '--- ' . t('Select a field') . '  ---');
    foreach (field_info_instances($form['index_info']['entity_type']['#default_value'], $form['index_info']['bundle']['#default_value']) as $field_name => $field_info) {
      $field_options[$field_name] = $field_info['label'];
    }

    $form['index_info']['field_name'] = array(
        '#type'          => 'select',
        '#title'         => t('Field'),
        '#required'      => TRUE,
        '#weight'        => -30,
        '#options'       => $field_options,
        '#default_value' => isset($field_index->field_name) ? $field_index->field_name : '',
        '#ajax'          => array(
            'callback' => 'field_index_form_ajax_submit',
            'wrapper'  => 'field-index-info-wrapper',
        ),
    );
  }

  // columns
  if (!empty($form['index_info']['field_name']['#default_value'])) {
    $column_options = array();
    $field_info = field_info_field($form['index_info']['field_name']['#default_value']);
    foreach ($field_info['columns'] as $column_name => $column_info) {
      $column_options[$column_name] = ucfirst($column_name) . ' (' . $column_info['type'] . ')';
    }

    $form['index_info']['column_names'] = array(
        '#type'          => 'select',
        '#multiple'      => TRUE,
        '#title'         => t('Fields'),
        '#required'      => TRUE,
        '#weight'        => -20,
        '#options'       => $column_options,
        '#default_value' => isset($field_index->column_names) ? $field_index->column_names : array(),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save field index'), '#weight' => 40);

  return $form;
}

/**
 * Ajax callback for field_index form.
 */
function field_index_form_ajax_submit($form, &$form_state) {
  return $form['index_info'];
}

/**
 * Submit handler for field_index form.
 */
function field_index_form_submit($form, &$form_state) {
  if ($form['actions']['submit']['#value'] = $form_state['clicked_button']['#value']) {
    $field_index = entity_ui_controller('field_index')->entityFormSubmitBuildEntity($form, $form_state);
    $field_index->save();
    $form_state['redirect'] = 'admin/structure/field-indexes';
  }
}
