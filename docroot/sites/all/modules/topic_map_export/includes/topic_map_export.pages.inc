<?php

/**
 * @file
 * Topic Map export form, which upon submission launches the batch processing.
 *
 * @see export_batch.inc
 */

/**
 * Topic map export configuration form.
 *
 * Topic map export configuration form. Used to select exportable entitites
 * along with bundles and associations.
 *
 * @see topic_map_export_export_form_submit()
 * @ingroup forms
 */
function topic_map_export_export_form($form, &$form_state) {

  $entity_info = entity_get_info();
  $entities_options = array();

  foreach ($entity_info as $key => $info) {
    $entities_options[$key] = $info['label'];
  }

  _topic_map_export_include_hook_files();
  $associations = module_invoke_all('topic_map_export_association_info');
  drupal_alter('topic_map_export_association_info', $associations);

  $available_associations = array();

  foreach ($associations as $info) {
    $key = _topic_map_export_create_key_from_association($info);
    $available_associations[$key] = $info['description'];
    $associations[$key] = $info;
  }


  $entities_defaults = array(
    'comment' => 'comment',
    'node' => 'node',
    'user' => 'user',
  );
  $associations_defaults = array(
    'node-created-by==creation--creator' => TRUE,
    'comment-comment_target==comment_target' => TRUE,
    'node-reference==node-referrer--node-referred' => TRUE,
  );


  $entities_defaults = array_intersect_key($entities_defaults, $entities_options);
  $associations_defaults = array_intersect_key($associations_defaults, $available_associations);

  $form['export_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Topic Map export settings'),
    '#prefix' => '<div id="tm-export-settings">',
    '#suffix' => '</div>',
  );

  $form['export_settings']['entities'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select entities you wish to export as topic(s).'),
    '#options' => $entities_options,
    '#default_value' => $entities_defaults,
  );

  $selected_entities = $entities_defaults;

  if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#ajax']['callback'] == 'topic_map_export_export_callback_update_bundles') {
    $selected_entities = (!isset($form_state['values'])) ? $entities_defaults : $form_state['values']['entities'];
  }

  $form['export_settings']['sub'] = array(
    '#prefix' => '<div id="tm-export-sub-settings">',
    '#suffix' => '</div>',
  );

  $custom_selector_callback = array(
    'callback' => 'topic_map_export_export_callback_custom_callback',
    'progress' => array('type' => 'none'),
  );


  $form['export_settings']['sub']['scope'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bundles to export'),
    '#prefix' => '<div id="tm-export-scope">',
    '#suffix' => '</div>',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  foreach ($entities_options as $entity_type => $value) {
    if ($value && !empty($entity_info[$entity_type]['entity keys']['bundle'])) {

      $bundles = $entity_info[$entity_type]['bundles'];

      $options = array();
      $defaults = array();
      foreach ($bundles as $key => $bundle) {
        $defaults[$key] = $key;
        $options[$key] = $bundle['label'];
      }

      $form['export_settings']['sub']['scope']['bundle_' . $entity_type] = array(
        '#type' => 'checkboxes',
        '#title' => t('Select @entity bundles you wish to have exported.', array('@entity' => $entity_info[$entity_type]['label'])),
        '#options' => $options,
        '#default_value' => $defaults,
        '#states' => array(
          'visible' => array(
            ':input[name="entities[' . $entity_type . ']"]' => array('checked' => TRUE),
          ),
        ),
      );

    }
  }

  $form['export_settings']['sub']['associations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select associations you wish to have described.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  // Needed for field type associations.
  $fields_info = field_info_fields();

  // Trough all available associations and creates the association checkboxes.
  foreach ($available_associations as $key => $value) {
    $form['export_settings']['sub']['associations'][$key] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($value),
      '#default_value' => (isset($associations_defaults[$key])) ? TRUE : FALSE,
    );

    // Set visibility rules for the association checkboxes.
    if (!empty($associations[$key]['requirements'])) {

      // Go trough all required entities if defined.
      if (isset($associations[$key]['requirements']['entities'])) {
        foreach ($associations[$key]['requirements']['entities'] as $entity_type) {
          $form['export_settings']['sub']['associations'][$key]['#states']['visible'][':input[name="entities[' . $entity_type . ']"]'] = array('checked' => TRUE);
        }
      }

      // Go trough all required fields.
      if (isset($associations[$key]['requirements']['field'])) {
        // Go trough all known field definitions in Drupal.
        foreach ($fields_info as $field) {

          if ($field['module'] == $associations[$key]['requirements']['field']['module']
          && $field['type'] == $associations[$key]['requirements']['field']['type']) {

            foreach ($field['bundles'] as $entity_key => $bundle_set) {

              // Is the entity using this field checked?
              $form['export_settings']['sub']['associations'][$key]['#states']['visible'][':input[name="entities[' . $entity_key . ']"]'] = array('checked' => TRUE);

              foreach ($bundle_set as $bundle) {
                // Is the bundle using this field checked?
                $form['export_settings']['sub']['associations'][$key]['#states']['visible'][':input[name="bundle_' . $entity_key . '[' . $bundle . ']"]'] = array('checked' => TRUE);
              }
            }
          }
        }
      }
    }

  }

  $form_state['input']['entities'] = $entities_defaults;
  $form_state['input']['associations'] = $associations_defaults;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
  );

  return $form;
}


/**
 * Form submission handler for topic_map_export_export_form().
 */
function topic_map_export_export_form_submit($form, &$form_state) {
  module_load_include('inc', 'topic_map_export', 'includes/export_batch');
  topic_map_export_topic_map($form_state['values']);
}
