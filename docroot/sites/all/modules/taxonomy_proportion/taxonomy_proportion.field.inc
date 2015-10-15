<?php

/**
 * @file
 * Field hooks to implement a taxonomy_proportion field.
 */

/**
 * Implements hook_field_info().
 */
function taxonomy_proportion_field_info() {
  return array(
    'taxonomy_proportion' => array(
      'label' => t('Taxonomy proportion'),
      'description' => t('Weight terms'),
      'default_widget' => 'taxonomy_proportion_checkboxes',
      'default_formatter' => 'taxonomy_proportion_format',
      'settings' => array(
        'field_location' => 'after',
        'allowed_values' => array(
          array(
            'vocabulary' => '',
            'parent' => '0'
            )
          )
        )
      )
  );
}

/**
 * Implements hook_field_widget_info().
 */
function taxonomy_proportion_field_widget_info() {
  return array(
    'taxonomy_proportion_checkboxes' => array(
      'label' => t('Check boxes/radio buttons'),
      'field types' => array('taxonomy_proportion'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM
      )
    ),
    'taxonomy_proportion_autocomplete' => array(
      'label' => t('Autocomplete term widget (tagging)'),
      'field types' => array('taxonomy_proportion'),
      'settings' => array(
        'size' => 30,
        'autocomplete_path' => 'taxonomy-proportion/autocomplete',
        'mode' => 'single',
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
      ),
    )
  );
}

/**
 * Implements hook_field_widget_formatter_info().
 */
function taxonomy_proportion_field_formatter_info() {
  return array(
    'taxonomy_proportion_format' => array(
      'label' => t('Taxonomy proportion'),
      'field types' => array(
        'taxonomy_proportion'
      ),
      'settings' => array(
        'field_location' => 'after',
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function taxonomy_proportion_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {

  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element['field_location'] = array(
    '#type' => 'radios',
    '#title' => t('Proportion location'),
    '#options' => drupal_map_assoc(array('before', 'after')),
    '#default_value' => isset($settings['field_location']) ? $settings['field_location'] : 'after',
    '#size' => 60,
    '#description' => t("Define the place where the proportion will be displayed: before or after the term")
  );
  return $element;
}


/**
 * Implements hook_field_formatter_settings_summary().
 */
function taxonomy_proportion_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = t('Location: %location', array('%location' => $settings['field_location']));

  return $summary;
}


/**
 * Implements hook_field_formatter_view().
 */
function taxonomy_proportion_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  if ($display['type'] == 'taxonomy_proportion_format') {
    foreach ($items as $delta => $item) {
      $term = taxonomy_term_load($item['tid']);
      $element[$delta] = array(
        '#theme' => 'taxonomy_proportion_termlink',
        '#tid' => $item['tid'],
        '#term_name' => $term->name,
        '#proportion' => $item['proportion'],
        '#field_location' => $display['settings']['field_location'],
        '#field_suffix' => !empty($instance['settings']['field_suffix']) ? $instance['settings']['field_suffix'] : '',
        '#field_prefix' => !empty($instance['settings']['field_prefix']) ? $instance['settings']['field_prefix'] : ''
      );
    }
   }
   else {
   foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#markup' => $item['tid'],
        );
      }
   }

  return $element;
}

/**
 * Implements hook_field_is_empty().
 */
function taxonomy_proportion_field_is_empty($item, $field) {
  return empty($item['tid']);
}

/**
 * Implements hook_field_validate().
 */
function taxonomy_proportion_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  // @TODO need validation for required checkboxes
}

/**
 * Implements hook_field_widget_form().
 */
function taxonomy_proportion_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $field_name = $field['field_name'];
  $parents = $form['#parents'];

  $element['#attached']['js'] = array(
    drupal_get_path('module', 'taxonomy_proportion') . '/taxonomy_proportion.js'
  );
  $element['#attached']['css'] = array(
    drupal_get_path('module', 'taxonomy_proportion') . '/taxonomy_proportion.css'
  );

  // Get terms for edit.
  $edit_item = array();
  foreach ($items as $item) {
    if (!empty($item['tid']) && is_numeric($item['tid']))
      $edit_item[$item['tid']] = $item;
  }

  // Checkoxes widget.
  if ($instance['widget']['type'] == 'taxonomy_proportion_checkboxes') {

    // Get terms for selected vocabulary.
    $vname = $field['settings']['allowed_values'][0]['vocabulary'];
    $vocabulary = taxonomy_vocabulary_machine_name_load($vname);
    $terms = taxonomy_get_tree($vocabulary->vid);

    // Checkboxes for multiple, radios for 1.
    $multiple = $field['cardinality'] > 1 || $field['cardinality'] == FIELD_CARDINALITY_UNLIMITED;

    $options = array();

    if ($multiple) {
      $element['#theme'] = 'taxonomy_proportion_multiple_checkboxes';
      // @TODO $element['#element_validate'] = array('taxonomy_proportion_multiple_checkboxes_validate'),
      foreach ($terms as $term) {
        $element[$term->tid]['#theme'] = 'taxonomy_proportion_form_field';
        $element[$term->tid]['tid'] = array(
          '#type' => 'checkbox',
          '#title' =>  check_plain($term->name),
          '#return_value' => $term->tid,
          '#default_value' => isset($edit_item[$term->tid]['tid']) ? $edit_item[$term->tid]['tid'] : ''
        );

        $element[$term->tid]['proportion'] = array(
          '#type' => 'textfield',
          '#size' => 4,
          '#field_prefix' => isset($instance['settings']['field_prefix']) ? $instance['settings']['field_prefix'] : '',
          '#field_suffix' => isset($instance['settings']['field_suffix']) ? $instance['settings']['field_suffix'] : '',
          '#fstate' => isset($edit_item[$term->tid]['tid']) ? '' : 'element-hidden',
          // '#weight' => -15,
          '#default_value' => !empty($edit_item[$term->tid]['proportion']) ? $edit_item[$term->tid]['proportion'] : '',
          '#attributes' => array(
            'class' => array(
              'proportion'
            )
          )
        );
      }
    }
    else {
      $options = $element['#required'] ? array() : array('_none' => t('N/A'));
      foreach ($terms as $term) {
        $options[$term->tid] = $term->name;
      }

      $element[0]['tid'] = array(
        '#type' => 'radios',
        '#title' =>  check_plain($element['#title']),
        '#options' => $options,
        '#required' => $element['#required'],
        '#default_value' => isset($items[0]['tid']) ? $items[0]['tid'] : ''
      );

      $element[0]['proportion'] = array(
        '#type' => 'textfield',
        '#size' => 4,
        '#field_prefix' => isset($instance['settings']['field_prefix']) ? $instance['settings']['field_prefix'] : '',
        '#field_suffix' => isset($instance['settings']['field_suffix']) ? $instance['settings']['field_suffix'] : '',
        '#default_value' => isset($items[0]['proportion']) ? $items[0]['proportion'] : '',
        '#attributes' => array(
          'class' => array(
            'proportion'
          )
        )
      );
    }
  }
  elseif ($instance['widget']['type'] == 'taxonomy_proportion_autocomplete') {

    $element['#theme'] = 'taxonomy_proportion_autocomplete';
    // Check form_state.
    if (!empty($form_state['values']) && !empty($form_state['values'][$field['field_name']])) {
      $items = $form_state['values'][$field['field_name']][LANGUAGE_NONE];
      unset($items['add_name']);
    }
    elseif (empty($items) || empty($items[0]['tid'])) {
      $items = array('');
    }

    $field_state = field_form_get_state($parents, $field_name, $langcode, $form_state);
    if (empty($field_state['items_count'])) {
      $field_state['items_count'] = 1;
      field_form_set_state($parents, $field_name, $langcode, $form_state, $field_state);
    }

    $max = $field_state['items_count'];

    for ($i = 0; $i < $max; $i++) {

      $lterm = isset($items[$i]) && is_array($items[$i]) && isset($items[$i]['tid']) ? taxonomy_term_load($items[$i]['tid']) : array();

      $element[$i] = array(
        '#field_name' => $field_name,
        '#language' => $langcode,
        '#field_parents' => $parents,
      );

      $element[$i]['tid'] = array(
        '#type' => 'textfield',
        '#default_value' => !empty($lterm->name) ? $lterm->name : '',
        '#autocomplete_path' => $instance['widget']['settings']['autocomplete_path'] . '/' . $field['field_name'],
        '#size' => $instance['widget']['settings']['size'],
        '#theme_wrappers' => array(),
        '#prefix' => '<div style="padding:5px 0;">'
      );
      $element[$i]['proportion'] = array(
        '#type' => 'textfield',
        '#default_value' => isset($items[$i]) && is_array($items[$i]) && !empty($items[$i]['proportion']) ? $items[$i]['proportion'] : '',
        '#size' => 4,
        '#theme' => 'taxonomy_proportion_formelement',
        '#field_prefix' => !empty($instance['settings']['field_prefix']) ? $instance['settings']['field_prefix'] : '',
        '#field_suffix' => !empty($instance['settings']['field_suffix']) ? $instance['settings']['field_suffix'] : '',
        '#theme_wrappers' => array(),
        '#suffix' => '</div>',
      );
    }

      $element['add_name'] = array(
        '#type' => 'submit',
        '#value' => t('Add one more'),
        '#name' => 'add_more' . $i,
        '#submit' => array('taxonomy_proportion_add_more_add_one'),
        '#limit_validation_errors' => array(array_merge($parents, array($field_name, $langcode))),
        '#ajax' => array(
          'callback' => 'taxonomy_proportion_add_more_callback_js',
          'wrapper' => 'names-fieldset-wrapper',
          'effect' => 'fade',
        ),
      );
    }


  // Make our node aware of the taxonomy_propotion field.
  $vname = $field['settings']['allowed_values'][0]['vocabulary'];
  $vocabulary = taxonomy_vocabulary_machine_name_load($vname);

  $form['taxonomy_proportion'] = array(
    '#type' => 'value',
    '#value' => array(
      'field_name' => $field['field_name'],
      'vid' => $vocabulary->vid
    )
  );

  return $element;
}

/**
 * Implements hook_field_presave().
 */
function taxonomy_proportion_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {

  foreach ($items as $key => $item) {
    if (empty($item['tid']) || !is_array($item) ) {
      unset($items[$key]);
    }
    elseif ($instance['widget']['type'] == 'taxonomy_proportion_autocomplete') {
      // Autocomplete must get tid from term and create if not exists.
      $vname = $field['settings']['allowed_values'][0]['vocabulary'];
      $lterm = taxonomy_get_term_by_name($item['tid'], $vname);
      if (count($lterm)) {
        $items[$key]['tid'] = key($lterm);
      }
      else {
        // Create new term
        $term = new stdClass();
        $vocabulary = taxonomy_vocabulary_machine_name_load($vname);
        $term->vid = $vocabulary->vid;
        $term->name = $item['tid'];
        taxonomy_term_save($term);
        $items[$key]['tid'] = $term->tid;
      }
    }
  }
}

/**
 * Implements hook_field_settings_form().
 */
function taxonomy_proportion_field_settings_form($field, $instance, $has_data) {
  return taxonomy_field_settings_form($field, $instance, $has_data);
}

/**
 * Implements hook_field_instance_settings_form().
 */
function taxonomy_proportion_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];

  $form['field_prefix'] = array(
      '#type' => 'textfield',
      '#title' => t('Prefix'),
      '#default_value' => isset($settings['field_prefix']) ? $settings['field_prefix'] : '',
      '#size' => 60,
      '#description' => t("Define a string that should be prefixed to the value, like '$ ' or '&euro; '. Leave blank for none.")
  );
  $form['field_suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Suffix'),
      '#default_value' => isset($settings['field_suffix']) ? $settings['field_suffix'] : '',
      '#size' => 60,
      '#description' => t("Define a string that should be suffixed to the value, like ' %'. Leave blank for none.")
  );

  return $form;
}
