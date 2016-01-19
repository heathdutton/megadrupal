<?php
/**
 * @file
 *   Field API definitions for Xero API.
 */


/**
 * Implements hook_field_info().
 */
function xero_field_info() {
  return array(
    'xero_reference' => array(
      'label' => t('Xero reference'),
      'description' => t('A reference to a Xero data type such as a Contact, Account, BankTransaction, etc... stored as a globally-unique identifier.'),
      'default_widget' => 'xero_textfield',
      'default_formatter' => 'xero_reference',
      'property_type' => 'xero_reference',
      'property_callbacks' => array('xero_entity_metadata_property_info_alter'),
    ),
  );
}

/**
 * Implements hook_field_widget_info().
 */
function xero_field_widget_info() {
  return array(
    'xero_autocomplete' => array(
      'label' => t('Xero autocomplete'),
      'description' => t('Use an autocomplete textfield to search Xero.'),
      'field types' => array('xero_reference'),
      'settings' => array(
        'xero_type' => '',
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
    'xero_textfield' => array(
      'label' => t('Xero textfield'),
      'description' => t('Use a plain text field to enter reference to globally-unique identifier and labels.'),
      'field types' => array('xero_reference'),
      'settings' => array(
        'xero_type' => array(),
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_info().
 */
function xero_field_formatter_info() {
  return array(
    'xero_reference' => array(
      'label' => t('Xero reference'),
      'description' => t('Display as a link to xero.com'),
      'field types' => array('xero_reference'),
      'settings' => array(
        'display' => array('guid' => 'guid', 'label' => 'label', 'type' => 'type'),
      ),
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function xero_field_is_empty($item, $field) {
  if (empty($item) || empty($item['guid'])) {
    return TRUE;
  }
}

/**
 * Implements hook_field_validate().
 *
 * Validating the existance of a xero object would be WAY too expensive for
 * private applications against the daily API limit. This will not be done.
 */
function xero_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {

  $field_name = $field['field_name'];
  $guid_regex = '/^\{?[A-Fa-f0-9]{8}-(?:[A-Fa-f0-9]{4}-){3}[A-Fa-f0-9]{12}\}?$/';

  foreach ($items as $delta => $item) {
    if (!empty($item['guid'])) {

      if (!preg_match($guid_regex, $item['guid'])) {
        // Not a globally-unique identifier.
        $errors[$field_name][$langcode][$delta][] = array(
          'error' => 'guid_error',
          'message' => t('%name: %value is not a globally-unique identifier.', array('%name' => $instance['label'], '%value' => $item['guid'])),
        );
      }
    }
    elseif ($instance['widget']['type'] == 'xero_textfield' && empty($item['type'])) {
      // Xero type must be set for manual/variable type entry.
      $errors[$field_name][$langcode][$delta][] = array(
        'error' => 'xero_type',
        'message' => t('%name: must have a valid Xero type.', array('%name' => $instance['label'])),
      );
    }
    elseif ($instance['widget']['type'] == 'xero_textfield' && !in_array($item['type'], $instance['widget']['settings']['xero_type'])) {
      // Xero type must be configured.
      $errors[$field_name][$langcode][$delta][] = array(
        'error' => 'xero_type',
        'message' => t('%name: invalid Xero type for this field.', array('%name' => $instance['label'])),
      );
    }
  }
}

/**
 * Implements hook_field_presave().
 */
function xero_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {

  foreach (element_children($items) as $delta) {
    // Remove brackets from guid before save.
    $items[$delta] = preg_replace('/[\{\}]/', '', $items[$delta]);

    if ($instance['widget']['type'] == 'xero_autocomplete') {
      // Need to set the data type from the widget settings.
      $items[$delta]['type'] = $instance['widget']['settings']['xero_type'];
    }
  }

}

/**
 * Implements hook_field_widget_settings_form().
 */
function xero_field_widget_settings_form($field, $instance) {

  $xero_types = array();
  $types = xero_get_data_types();
  foreach ($types as $name => $type) {
    $xero_types[$name] = $type['title'];
  }

  $form['xero_type'] = array(
    '#type' => 'select',
    '#title' => t('Xero data type'),
    '#description' => t('Select the Xero data type to use for this field. Example: Contact, Account, etc...'),
    '#options' => $xero_types,
    '#required' => TRUE,
    '#default_value' => isset($instance['widget']['settings']['xero_type']) ? $instance['widget']['settings']['xero_type'] : '',
    '#weight' => -10,
  );

  if ($instance['widget']['type'] == 'xero_textfield') {
    // Alter form settings to provide multiple values.
    $form['xero_type']['#multiple'] = TRUE;
    $form['xero_type']['#required'] = FALSE;
    $form['xero_type']['#description'] = t('Select the Xero data types to use for this field. If none are selected, then all data types will be available.');
  }

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function xero_field_widget_form(&$form, $form_state, $field, $instance, $langcode, $items, $delta, $element) {

  if ($instance['widget']['type'] == 'xero_autocomplete') {
    $type_name = empty($instance['widget']['settings']['xero_type']) ? 'Contact' : $instance['widget']['settings']['xero_type'];
    $type = xero_get_data_types($type_name);
    $label = empty($type['label']) ? $type['guid'] : $type['label'];
    $guids = array();
    $label_key = empty($type['label']) ? 'guid' : 'label';

    foreach ($items as $delta => $item) {
      $guids[] = isset($item['guid']) ? $item[$label_key] : '';
    }

    $element += array(
      '#type' => 'textfield',
      '#description' => t('Type a Xero %type %field to search for an available record.', array('%type' => $type['name'], '%field' => $label)),
      '#autocomplete_path' => 'xero/autocomplete/' . $instance['widget']['settings']['xero_type'],
      '#element_validate' => array('xero_field_autocomplete_validate'),
      '#default_value' => !empty($guids) ? implode(', ', $guids) : '',
      '#maxlength' => 1024,
      '#size' => 60,
    );
  }
  else {
    // Assemble elements. 
    $options = xero_field_get_type_options($instance['widget']['settings']['xero_type'], null, null);

    $element += array(
      '#type' => 'container',
      '#tree' => TRUE,
      'type' => array(
        '#type' => 'select',
        '#title' => t('Xero type'),
        '#description' => t('Select the Xero data type to associate.'),
        '#options' => $options,
        '#default_value' => (!empty($items) && !empty($items[$delta])) ? $items[$delta]['type'] : '',
      ),
      'guid' => array(
        '#type' => 'textfield',
        '#title' => t('GUID'),
        '#description' => t('Provide the globally-unique identifier for the Xero item.'),
        '#default_value' => (!empty($items) && !empty($items[$delta])) ? $items[$delta]['guid'] : '',
        '#maxlength' => 38,
        '#size' => 60,
      ),
      'label' => array(
        '#type' => 'textfield',
        '#title' => t('Description'),
        '#description' => t('Describe the reference to the Xero item'),
        '#default_value' => (!empty($items) && !empty($items[$delta])) ? $items[$delta]['label'] : '',
        '#maxlength' => 255,
        '#size' => 60,
      ),
    );
  }

  return $element;
}

/**
 * Xero autocomplete field widget validate.
 *
 * The autocomplete data must be processed into form state array
 * with the correct keys, guid and label.
 */
function xero_field_autocomplete_validate($element, &$form_state) {
  if (!empty($element['#value']) && !is_array($element['#value'])) {
    // Autocomplete field widget must be expanded into an array.
    preg_match('/(\S+) \((.+)\)/', $element['#value'], $matches);
    $item = array(
      array(
        'guid' => $matches[1],
        'label' => $matches[2],
      ),
    );
    form_set_value($element, $item, $form_state);
  }
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function xero_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element = array();

  $element['display'] = array(
    '#type' => 'select',
    '#title' => t('Display fields'),
    '#options' => array('guid' => t('GUID'), 'label' => t('Label'), 'type' => t('Xero type')),
    '#multiple' => TRUE,
    '#default_value' => $settings['display'],
  );

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function xero_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $guid = in_array('guid', $settings['display']) ? 'Visible' : 'Hidden';
  $label = in_array('label', $settings['display']) ? 'Visible' : 'Hidden';
  $type = in_array('type', $settings['display']) ? 'Visible' : 'Hidden';

  return t('GUID @guid, Label @label, Xero type @type', array('@guid' => $guid, '@label' => $label, '@type' => $type));
}

/**
 * Implements hook_field_formatter_view().
 */
function xero_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $types = xero_get_data_types();

  switch ($display['type']) {
    case 'xero_reference':

      $element = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('field', 'field-' . str_replace('_', '-', $field['field_name']), 'xero-field'),
        ),
      );

      foreach ($items as $delta => $item) {
        $type = $types[$item['type']];

        $element[$delta] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('field-item', 'field-item-' . $delta, 'xero-field-item-' . $delta, 'xero-field-' . $type['name'] . '-item'),
            'id' => $item['guid'],
          ),
        );

        if (in_array('type', $display['settings']['display'])) {
          $element[$delta]['type'] = array(
            '#theme' => 'html_tag',
            '#tag' => 'span',
            '#value' => $type['title'],
            '#attributes' => array(
              'class' => array('xero-type-label'),
            ),
          );
        }

        if (in_array('label', $display['settings']['display']) && $type['label']) {
          // Only add label if the data type has a label.
          $element[$delta]['label'] = array(
            '#theme' => 'html_tag',
            '#tag' => 'span',
            '#value' => check_plain($item['label']),
            '#attributes' => array(
              'class' => array('xero-' . $type['name'] . '-label'),
            ),
          );
        }
        
        if (in_array('guid', $display['settings']['display'])) {
          if (isset($element[$delta]['label'])) {
            $element[$delta]['label']['#value'] .= ': ';
          }

          $element[$delta]['guid'] = array(
            '#theme' => 'html_tag',
            '#tag' => 'span',
            '#value' => $item['guid'],
            '#attributes' => array(
              'class' => array('xero-' . $type['name'] . '-guid'),
            ),
          );
        }
      }

      break;
  }
 
  return $element;
}

/**
 * Implements hook_entity_metadata_property_info_alter().
 *
 * It is currently not possible to accurately get a token from Entity API tokens
 * for multi-value fields.
 *
 * @param &$info
 * @param $entity_type
 * @param $field
 * @param $instance
 * @param $field_type 
 */
function xero_entity_metadata_property_info_alter(&$info, $entity_type, $field, $instance, $field_type) {
  $name = $field['field_name'];
  $property = &$info[$entity_type]['bundles'][$instance['bundle']]['properties'][$name];

  // $property['type'] = $field['cardinality'] == 1 ? 'struct' : 'list<struct>';
  $property['getter callback'] = 'entity_metadata_field_verbatim_get';
  $property['setter callback'] = 'entity_metadata_field_verbatim_set';
  unset($property['query callback']);
  // $property['cardinality'] = $field['cardinality'];
  $property['property info'] = array(
    'guid' => array(
      'type' => 'text',
      'label' => t('GUID'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'raw getter callback' => 'entity_property_verbatim_get',
    ),
    'label' => array(
      'type' => 'text',
      'label' => t('Label'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'raw getter callback' => 'entity_property_verbatim_get',
    ),
    'type' => array(
      'type' => 'text',
      'label' => t('Xero type'),
      'getter callback' => 'entity_property_verbatim_get',
      'options list' => 'xero_field_get_type_options',
      'setter callback' => 'entity_property_verbatim_set',
      'raw getter callback' => 'entity_property_verbatim_get',
    ),
  );

}

/**
 * Get xero data types as an option list.
 */
function xero_field_get_type_options($name, $info, $display) {
  $available = is_array($name) ? $name : array();
  $options = array();
  $types = xero_get_data_types();
  foreach ($types as $type_name => $type) {
    if (empty($available) || (!empty($available) && in_array($type_name, $available))) {
      // Add data type to options list.
      $options[$type_name] = $type['title'];
    }
  }
  return $options;
}

