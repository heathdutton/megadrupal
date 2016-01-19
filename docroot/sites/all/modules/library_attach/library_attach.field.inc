<?php

/**
 * Implements hook_field_info().
 */
function library_attach_field_info() {
  $info = array();
  $info['library_reference'] = array(
    'label' => t('Library reference'),
    'description' => t('This field stores a reference to a library from drupal_get_library().'),
    'default_widget' => 'options_select',
    'default_formatter' => 'library_attached',
  );

  return $info;
}

/**
 * Implements hook_field_load().
 */
function library_attach_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => &$item) {
      // Expand the field value into separate parts for the module and
      // library name for better use.
      list($item['module'], $item['library']) = explode(':', $item['value'], 2);
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function library_attach_field_is_empty($item, $field) {
  return empty($item['value']) || strpos($item['value'], ':') < 1;
}

/**
 * Implements hook_field_widget_info_alter().
 */
function library_attach_field_widget_info_alter(array &$info) {
  $info['options_select']['field types'][] = 'library_reference';
  $info['options_buttons']['field types'][] = 'library_reference';
}

/**
 * Implements hook_options_list().
 */
function library_attach_options_list($field, $instance, $entity_type, $entity) {
  if (empty($field['settings']['options_list_callback']) || !function_exists($field['settings']['options_list_callback'])) {
    return library_attach_get_library_options();
  }
}

/**
 * Implements hook_field_formatter_info().
 */
function library_attach_field_formatter_info() {
  $info = array();
  $info['library_attached'] = array(
    'label' => t('Attached'),
    'field types' => array('library_reference'),
  );
  $info['library_plain'] = array(
    'label' => t('Plain text'),
    'field types' => array('library_reference'),
  );

  return $info;
}

/**
 * Implements hook_field_prepare_view().
 */
function library_attach_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => &$item) {
      // Expand the field value into separate parts for the module and
      // library name for better use.
      //list($item['module'], $item['library']) = explode(':', $item['value'], 2);
    }
  }
}

/**
 * Implements hook_field_formatter_view().
 */
function library_attach_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $settings = $display['settings'];

  switch ($display['type']) {
    case 'library_attached':
      $libraries = array();
      foreach ($items as $delta => $item) {
        $libraries[] = $item['value'];
      }
      library_attach_add_libraries($libraries, $element);
      //foreach ($items as $delta => $item) {
      //  $element['#attached']['library'][] = array($item['module'], $item['library']);
      //}
      break;

    case 'library_plain':
      foreach ($items as $delta => $item) {
        if ($library = drupal_get_library($item['module'], $item['library'])) {
          $element[$delta]['#markup'] = filter_xss($library['title']);
        }
      }
      break;
  }

  return $element;
}

/**
 * Implements hook_field_attach_view_alter().
 */
function library_attach_field_attach_view_alter(array &$output, array $context) {
  if ($context['view_mode'] != '_custom' && isset($output['#bundle'])) {
    // Find the actual view mode being used.
    $view_mode = _library_attach_get_actual_view_mode_used($context['entity_type'], $output['#bundle'], $context['view_mode']);
    if ($libraries = library_attach_get_view_mode_libraries($context['entity_type'], $output['#bundle'], $view_mode)) {
      library_attach_add_libraries($libraries, $output);
    }
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function library_attach_form_field_ui_display_overview_form_alter(&$form, &$form_state) {
  $libraries = library_attach_get_view_mode_libraries($form['#entity_type'], $form['#bundle'], $form['#view_mode']);

  $form['libraries'] = array(
    '#type' => 'fieldset',
    '#title' => t('Libraries'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['libraries']['libraries'] = array(
    '#type' => 'select',
    '#title' => t('Libraries to add to this display'),
    '#options' => library_attach_get_library_options(),
    '#multiple' => TRUE,
    '#default_value' => $libraries,
    '#size' => max(10, min(count($libraries), 10)),
  );
  if (module_exists('chosen')) {
    //$form['libraries']['libraries']['#prefix'] = '<div style="min-height: 200px;">';
    //$form['libraries']['libraries']['#suffix'] = '</div>';
    $form['libraries']['libraries']['#attributes']['data-placeholder'] = t('Select a library');
    $form['libraries']['libraries']['#attributes']['class'][] = 'chosen-widget';
    $form['libraries']['libraries']['#attributes']['style'] = 'width: 100%';
  }

  $form['#submit'][] = 'library_attach_form_field_ui_display_overview_form_submit';
}

/**
 * Form submit handler for field_ui_display_overview_form().
 */
function library_attach_form_field_ui_display_overview_form_submit($form, &$form_state) {
  if (isset($form_state['values']['libraries'])) {
    // Select boxes filter out empty values, so don't need to array_filter().
    $libraries = array_keys($form_state['values']['libraries']);
    library_attach_set_view_mode_libraries($form['#entity_type'], $form['#bundle'], $form['#view_mode'], $libraries);
  }
}
