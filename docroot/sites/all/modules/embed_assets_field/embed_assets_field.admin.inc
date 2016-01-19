<?php
/**
 * @file
 *   Admin functions for Embed Assets Field module
 */

/**
 * Menu callback for config page
 */
function embed_assets_field_settings() {
  $page = array(
    'config_form' => array('#markup' => theme('embed_assets_field_settings_form', array('form' => drupal_get_form('embed_assets_field_settings_form')))),
  );

  return $page;
}

/**
 * The config form for the module
 */
function embed_assets_field_settings_form($form, &$form_state) {
  $form = array(
    '#title' => 'Select embeddable asset fields for each entity and bundle',
    'instruction' => array(
      '#markup' => '<p>' . t('For each bundle below, please select the field which will contain embeddable assets') . '</p>',
    ),
    'embeddable_fields' => array(
      '#type' => 'item',
      '#tree' => TRUE,
    ),
  );

  $embeddable_fields = embed_assets_field_get_embeddable_fields();

  foreach ($embeddable_fields as $entity_name => $bundles) {
    $entity_fields = variable_get('embed_assets_field_' . $entity_name, array());
    foreach ($bundles as $bundle_name => $bundle_types) {
      foreach ($bundle_types as $type => $bundle) {
        $options = array('' => 'None');

        foreach ($bundle['fields'] as $field_name => $field) {
          $options[$field_name] = $field[0]['label'];
        }

        $form['embeddable_fields'][$entity_name][$bundle_name][$type] = array(
          '#type'          => 'select',
          '#title'         => 'Select embeddable field',
          '#options'       => $options,
          '#default_value' => !empty($entity_fields[$bundle_name][$type]) ? $entity_fields[$bundle_name][$type] : '',
          '#required'      => FALSE,
          'entity' => array('#markup' => $bundle['entity']['label']),
          'bundle' => array('#markup' => $bundle['bundle']['label']),
        );
      }
    }
  }

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => 'Save',
  );

  return $form;
}

/**
 * Return HTML for the config form
 */
function theme_embed_assets_field_settings_form($variables) {
  $form = $variables['form'];
  $type_info = embed_assets_field_get_field_type_info();
  $type_names = array_keys($type_info);
  $header =  array(
    t('Entity'),
    t('Bundle'),
  );

  foreach ($type_info as $type_name => $type_info) {
    $header[] = $type_info['label'];
  }

  $table = array(
    'header' => $header,
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => t('There are no bundles and / or fields available for embedding assets'),
    'rows' => array(),
  );

  $rows = array();
  foreach (element_children($form['embeddable_fields']) as $entity_name) {
    $bundles = &$form['embeddable_fields'][$entity_name];
    foreach (element_children($bundles) as $bundle_name) {
      $bundle = &$bundles[$bundle_name];
      $row = array();
      $field = FALSE;
      foreach ($type_names as $type_name) {
        if (empty($bundle[$type_name])) {
          $row[] = t('None Available');
          continue;
        }
        $field = &$bundle[$type_name];
        $row[] = drupal_render($field);
      }
      if (!empty($field)) {
        array_unshift($row, drupal_render($field['bundle']));
        array_unshift($row, drupal_render($field['entity']));
        $rows[] = $row;
      }
    }
  }

  $table['rows'] = $rows;

  $form['embeddable_fields']['table']['#markup'] = theme('table', $table);

  $output = drupal_render_children($form);
  return $output;
}

/**
 * Submit hook for embed_assets_field_settings_form
 */
function embed_assets_field_settings_form_submit($form, &$form_state) {
  $num_saved = 0;
  foreach ($form_state['values']['embeddable_fields'] as $entity_name => $bundles) {
    $value = NULL;
    // Check that there is at least one field chosen for this entity type
    foreach ($bundles as $bundle => $types) {
      foreach ($types as $type => $field) {
        if ($field) {
          $value = $bundles;
          // There is at least one field chosen
          break;
        }
      }
    }
    $variable_name = "embed_assets_field_$entity_name";
    $num_saved += $value ? count($value) : 0;

    if ($value) {
      variable_set($variable_name, $value);
    }
    else {
      variable_del($variable_name);
    }
  }

  $message = format_plural($num_saved, 'Saved settings for 1 entity bundle.', 'Saved settings for @count entity bundles.');
  drupal_set_message($message);
}

/**
 * Returns a nested array of all entities, bundles and their associated fields which can be used as an embed assets field
 *
 * @return
 *   Array in the format:
 *     array(
 *       'entity_name' => array(
 *         'bundle_name' => array(
 *           'type' => array(
 *             'field_name' => array(
 *               'entity' => $entity,
 *               'bundle' => $bundle,
 *               'fields' => array($field1, [$field2 [...]])
 *             )
 *           )
 *         )
 *       )
 *     )
 */
function embed_assets_field_get_embeddable_fields() {
  $embeddable_fields = array();
  $info = entity_get_info();

  foreach ($info as $entity_name => $entity) {
    foreach ($entity['bundles'] as $bundle_name => $bundle) {
      $bundle_fields = field_info_instances($entity_name, $bundle_name);
      foreach ($bundle_fields as $field_name => $field) {
        foreach (embed_assets_field_get_field_type_info() as $type => $type_info) {
          if ($field['widget']['module'] == $type_info['module']) {
            if (empty($embeddable_fields[$entity_name][$bundle_name][$type])) {
              $embeddable_fields[$entity_name][$bundle_name][$type] = array(
                'entity' => $entity,
                'bundle' => $bundle,
                'fields' => array(),
              );
            }
            $embeddable_fields[$entity_name][$bundle_name][$type]['fields'][$field_name] = array($field);
          }
        }
      }
    }
  }

  return $embeddable_fields;
}
