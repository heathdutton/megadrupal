<?php

/**
 * Forms for administrative task, such as viewing list of entities and
 * filtering them base on a field or property similar to page admin/content.
 */

/**
 * Returns the form to filter entity lists.
 *
 * Original code in Redhen module.
 */
function entityspice_filter_form($f, &$fs, $etype) {
  drupal_add_library('system', 'ui.datepicker');
  drupal_add_js(drupal_get_path('module', 'entityspice') . '/static_asset/entityspice.forms.js');

  $property_info = entity_get_all_property_info($etype);
  if (!empty($property_info)) {
    $f['filter_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Filters'),
      '#collapsible' => TRUE,
      '#collapsed' => (empty($_GET['properties']) && empty($_GET['fields']) && empty($_GET['bundle'])),
    );

    $f['filter_fieldset']['filters'] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );

    $f['filter_fieldset']['filters']['properties'] = array(
      '#type' => 'container',
    );

    // Add a form field for each property.
    foreach ($property_info as $key => $prop) {
      if (isset($prop['filter']) && $prop['filter']) {
        $field_type = isset($prop['field_type']) ? $prop['field_type'] : $prop['type'];

        // Convert datetime defaults to date strings.
        $default_value = isset($_GET['properties'][$key]) ? $_GET['properties'][$key] : NULL;
        if ($prop['type'] == 'date' && !empty($default_value)) {
          $default_value['from'] = ($default_value['from'] == '') ? '' :
            format_date(strtotime($default_value['from']), 'medium', 'm/d/Y');
          $default_value['to'] = ($default_value['to'] == '') ? '' :
            format_date(strtotime($default_value['to']), 'medium', 'm/d/Y');
        }

        if ($prop['type'] == 'date') {
          $f['filter_fieldset']['filters']['properties'][$key]['from'] = array(
            '#type' => $field_type,
            '#title' => t('@label from', array('@label' => $prop['label'])),
            '#attributes' => array('class' => array("entityspice-field-type-{$prop['type']}")),
            '#default_value' => $default_value['from'],
          );
          $f['filter_fieldset']['filters']['properties'][$key]['to'] = array(
            '#type' => $field_type,
            '#title' => t('@label to', array('@label' => $prop['label'])),
            '#attributes' => array('class' => array("entityspice-field-type-{$prop['type']}")),
            '#default_value' => $default_value['to'],
          );
        }
        else {
          $f['filter_fieldset']['filters']['properties'][$key] = array(
            '#type' => $field_type,
            '#title' => check_plain($prop['label']),
            '#attributes' => array('class' => array("entityspice-field-type-{$prop['type']}")),
            '#default_value' => $default_value,
          );
        }

        if ($field_type == 'select' && isset($prop['options list'])) {
          $select_options = array('' => t('-- Any --')) + call_user_func($prop['options list']);
          $f['filter_fieldset']['filters']['properties'][$key]['#options'] = $select_options;
        }
      }
    }

    // Add bundles; will load additional field filters on change.
    $type_options = array('' => t('-- Any --'));
    $entity_info = entity_get_info($etype);
    foreach ($entity_info['bundles'] as $key => $bundle) {
      $type_options[$key] = $bundle['label'];
    }

    // Single bundle, just load fields.
    if (count($entity_info['bundles']) == 1) {
      $bundle = array_keys($entity_info['bundles']);
      $bundle = $bundle[0];
      _entityspice_filter_form_fields($etype, $f, $fs, $bundle);
    }
    else {
      $f['filter_fieldset']['filters']['bundle'] = array(
        '#type' => 'select',
        '#title' => t('@type types', array('@type' => $entity_info['label'])),
        '#options' => $type_options,
        '#default_value' => isset($_GET['bundle']) ? $_GET['bundle'] : NULL,
        '#ajax' => array(
          'callback' => '_entityspice_filter_fields_callback',
          'wrapper' => 'entityspice_filter_fields_container',
          'method' => 'replace',
          'effect' => 'fade',
          'progress' => array(
            'type' => 'throbber',
            'message' => t('Retrieving fields for this @entity_type type.',
              array('@entity_type' => $entity_info['label'])),
          ),
        ),
      );
      $f['filter_fieldset']['filters']['fields'] = array(
        '#prefix' => '<div id="entityspice_filter_fields_container">',
        '#suffix' => '</div>',
      );
    }

    $f['filter_fieldset']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Apply'),
    );

    $f['filter_fieldset']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
      '#limit_validation_errors' => array(),
      '#submit' => array('entityspice_filter_form_reset'),
    );
  }

  // Add the fields on an ajax type change event or if a filter is being passed.
  if (!empty($fs['values']['filters']['bundle']) || isset($_GET['bundle'])) {
    $bundle = !empty($fs['values']['filters']['bundle']) ?
      $fs['values']['filters']['bundle'] : $_GET['bundle'];

    _entityspice_filter_form_fields($etype, $f, $fs, $bundle);
  }

  return $f;
}

/**
 * AJAX callback to return fields for a given type.
 */
function _entityspice_filter_fields_callback($f, $fs) {
  return $f['filter_fieldset']['filters']['fields'];
}

/**
 * Validation callback for filter form.
 */
function entityspice_filter_form_validate($f, &$fs) {
  // @TODO: Date field validation.
}

/**
 * Submit handler for entityspice_sec_filter_form().
 */
function entityspice_filter_form_submit($f, &$fs) {
  $filters = $fs['values']['filters'];
  $fs['redirect'] = array($_GET['q'], array('query' => $filters));
}

/**
 * Submit handler to reset the filter form.
 */
function entityspice_filter_form_reset($f, &$fs) {
  $fs['redirect'] = array($_GET['q']);
}

/**
 * Populate a form array with field filters for given bundle.
 *
 * @param array $f
 *   Form to alter.
 * @param array $fs
 *   Form state to alter.
 * @param string $etype
 *   Entity type.
 * @param string $bundle
 *   Bundle.
 */
function _entityspice_filter_form_fields($etype, &$f, &$fs, $bundle) {
  $fields = field_info_instances($etype, $bundle);
  $f['filter_fieldset']['filters']['fields'] = array(
    '#id' => 'entityspice_filter_fields_container',
  );
  // If there are multiple bundle types, show which one is current.
  if (isset($f['filter_fieldset']['filters']['bundle'])) {
    $f['filter_fieldset']['filters']['fields']['#type'] = 'fieldset';
    $f['filter_fieldset']['filters']['fields']['#title'] = t('Filter by %bundle specific fields',
      array('%bundle' => $bundle)
    );
  }
  else {
    $f['filter_fieldset']['filters']['fields']['#type'] = 'container';
  }
  $filters_count = 0;
  if (!empty($fields)) {
    if (!isset($fs['field'])) {
      $fs['field'] = array();
    }

    // Loop through each field adding a filter.
    $filters_count = 0;
    foreach ($fields as $field => $instance) {
      $field_info = field_info_field($instance['field_name']);

      $filters_count++;

      // Insert the widget.
      // field_default_form() assumes the $f has a #parents property.
      $element = array(
        '#parents' => array(),
      );
      // Because of the redirect in entityspice_filter_form_submit(), $fs
      // has no values. Instead, read directly from $_GET.
      $items = isset($_GET['fields'][$field]) ? $_GET['fields'][$field] : array(LANGUAGE_NONE => array());
      $instance['required'] = FALSE;
      $instance['description'] = '';
      $field_info['cardinality'] = 1;

      // Set up specific field types.
      switch ($field_info['type']) {
        // Set up date fields.
        case 'date':
        case 'datestamp':
        case 'datetime':
          // Ensure we have a 'to' date.
          $field_info['settings']['todate'] = 'required';
          // In case date repeat is present, disable it.
          $field_info['settings']['repeat'] = FALSE;
          break;
      }

      // The last argument to field_default_form() fetches a single delta.
      $additions = field_default_form($etype, NULL, $field_info, $instance, LANGUAGE_NONE, $items[LANGUAGE_NONE], $element, $fs, 0);

      foreach ($additions as $field_name => $addition) {
        // TODO: Handle country ajax post back. It currently corrupts the form.
        if ($field_info['type'] == 'addressfield') {
          // This hack sets up the form with the default country.
          $addition[LANGUAGE_NONE][0]['country']['#access'] = FALSE;
        }

        // Append an explanation on the handling of boolean values.
        if ($field_info['type'] == 'list_boolean') {
          $addition[LANGUAGE_NONE]['#type'] = 'radios';
          $addition[LANGUAGE_NONE]['#options'] = array(
            0 => t('False'),
            1 => t('True'),
            'skip' => t('Skip'),
          );
          $addition[LANGUAGE_NONE]['#description'] = $addition[LANGUAGE_NONE]['#description'] .
            t('This boolean checkbox has been converted to radio buttons to allow filtering for false as well as skipping non-required fields. .');
        }

        $f['filter_fieldset']['filters']['fields'][$field_name] = $addition;
      }
    }
  }
  // Show a message if there is more than one bundle and this one has no
  // applicable fields.
  if (!$filters_count && isset($f['filter_fieldset']['filters']['bundle'])) {
    $f['filter_fieldset']['filters']['fields']['message'] = array(
      '#markup' => t('There are no additional fields for %bundle.', array('%bundle' => $bundle))
    );
  }
}

