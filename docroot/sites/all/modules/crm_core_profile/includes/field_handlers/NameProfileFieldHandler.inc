<?php

/**
 * Default class for rendering a form field in CRM Core Profile.
 *
 * @param array $field
 *   Field to render config for.
 * @param array $field_info
 *   Field info.
 * @param array $form
 *   Form to be modified
 */
class NameProfileFieldHandler extends DefaultProfileFieldHandler {

  /**
   * This function is triggered when a field is being used in a profile.
   */
  public function renderField($field_name, $profile, &$form, &$form_state) {

    $entity_type = $form[$field_name][LANGUAGE_NONE][0]['#entity_type'];
    $bundle = $form[$field_name][LANGUAGE_NONE][0]['#bundle'];
    // Store the field prefix.
    $field_prefix = $entity_type . '_' . $bundle . '_';
    $profile_field = $field_prefix . $field_name;

    if (!isset($profile[$profile_field]['status'])) {
      $form[$field_name]['#access'] = FALSE;
      $form[$field_name]['#element_validate'] = array();

      return;
    }
    elseif ($profile[$profile_field]['status'] == 1) {
      return;
    }
    else {
      $form[$field_name]['#access'] = FALSE;
      $form[$field_name]['#element_validate'] = array();
    }

    // Now we have to check for subfields.
    $sub_fields = array();
    $field = field_info_field($field_name);

    // Are all subfields hidden?
    $subfields_all_hidden = TRUE;

    foreach (array_keys($field['columns']) as $column) {
      // we are going to check if each sub-field is visible or not
      // if it is visible, we are going to render a dummy field to store the value
      // if it is not, we are not worried about it

      $subfield_name = $field_prefix . $field_name . '_' . $column;
      if (!empty($profile[$subfield_name]['status'])) {
        // these are the form fields that need to display
        // what we are going to do is add some dummy fields to the form to capture these values
        // in the appropriate format
        // i.e. we are adding some fields that will capture values and those values will be
        // injected into the proper form fields when the profile is saved.
        switch ($column) {
          case 'given':
          case 'middle':
          case 'family':
          case 'credentials':
            $form[$subfield_name] = array(
              '#type' => 'textfield',
              '#title' => t($field['settings']['labels'][$column]) . ':',
              '#weight' => $profile[$subfield_name]['weight'],
            );
            break;

          case 'title':
          case 'generational':
            $form[$subfield_name] = array(
              '#type' => 'select',
              '#title' => t($field['settings']['labels'][$column]) . ':',
              '#options' => name_field_get_options($field, $column),
              '#weight' => $profile[$subfield_name]['weight'],
            );
            break;
        }
      }

      // Hide "hidden" field.
      if (!empty($profile[$subfield_name]['hidden'])) {
        // If using subfields, set it on the subfield.
        $form[$subfield_name]['#type'] = 'hidden';
        // Workaround name field class attribute merge bug.
        $form[$field_name][LANGUAGE_NONE][0]['#components'][$column]['attributes']['style'] = 'display: none;';
        // Hide the component label which sits outside the field.
        $form[$field_name][LANGUAGE_NONE][0]['#components'][$column]['title_display'] = 'none';
      }
      else {
        // Displaying at least one component.
        $subfields_all_hidden = FALSE;
      }

      $crm_core_profile = $form_state['profile'];
      $prepopulate = isset($crm_core_profile->entities['crm_core_contact']['prepopulate'])
        ? $crm_core_profile->entities['crm_core_contact']['prepopulate'] : FALSE;
      if ($entity_type == 'crm_core_contact' && $prepopulate) {
        if (empty($profile[$profile_field]['status'])) {
          // If using subfields, set it on the subfield if there's a value.
          if (isset($form[$field_name][LANGUAGE_NONE][0]['#default_value'][$column])) {
            $form[$subfield_name]['#default_value'][$column] = $form[$field_name][LANGUAGE_NONE][0]['#default_value'][$column];
          }
        }
      }
      else {
        // Apply default values for each component.
        $default_value = '';
        // Find default value from profile settings.
        if (array_key_exists($subfield_name, $profile) && !empty($profile[$subfield_name]['default'])) {
          $default_value = $profile[$subfield_name]['default'];
        }
        if (!empty($default_value)) {
          if (empty($profile[$profile_field]['status'])) {
            // If using subfields, set it on the subfield.
            $form[$subfield_name]['#default_value'][$column] = $default_value;
          }
          else {
            // Otherwise, use the real name field element #default_value.
            $form[$field_name][LANGUAGE_NONE][0]['#default_value'][$column] = $default_value;
          }
        }
      }
    }

    // Hide the whole field if all subfields are hidden.
    if ($subfields_all_hidden) {
      $form[$field_name]['#attributes']['class'][] = 'element-hidden';
    }
  }

  /**
   * This function is triggered by the #after_build callback of a
   * CRM Core Profile form.
   */
  public function afterBuild($field_name, $profile, &$form, &$form_state) {
    array_unshift($form[$field_name][LANGUAGE_NONE][0]['#element_validate'], 'NameProfileFieldHandlerElementValidate');
  }

  /**
   * This function is triggered by the CRM Core Profile form submit handler.
   */
  public function submitField($field_name, $profile, &$form, &$form_state) {
  }

  public function renderFieldConfig(&$form, $entity_type, $entity_label, $bundle, $field_name) {
    $field_config = array();

    if (isset($form['#profile'])) {
    $profile = $form['#profile'];
    }

    $field = field_info_field($field_name);
    $field_instance = field_info_instance($entity_type, $field_name, $bundle);
    $default_field_config = array(
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'field_name' => $field_name,
      'field_item' => '',
      'field_label' => $field_instance['label'],
    );
    $field_separator = _crm_core_profile_field_separator($default_field_config);
    if (isset($profile->fields[$field_separator])) {
      $field_config = $profile->fields[$field_separator];
    }
    $field_config += array('handler' => get_class($this));
    $field_config += $default_field_config;

    $form['fields'][$field_separator] = array();
    $row =& $form['fields'][$field_separator];

    $weight = _crm_core_profile_default_weight();

    $main_field_weight = $weight - count($field['columns']) - 1;

    $row['#weight'] = isset($field_config['weight']) ? _crm_core_profile_weight_fix($field_config['weight']) : $main_field_weight;

    $row['field_name'] = array(
      '#type' => 'value',
      '#value' => $field_config['field_name'],
    );
    $row['field_item'] = array(
      '#type' => 'value',
      '#value' => $field_config['field_item'],
    );
    $row['handler'] = array(
      '#type' => 'value',
      '#value' => $field_config['handler'],
    );
    $row['entity_type'] = array(
      '#type' => 'value',
      '#value' => $field_config['entity_type'],
    );
    $row['bundle'] = array(
      '#type' => 'value',
      '#value' => $field_config['bundle'],
    );

    $row['status'] = array(
      '#type' => 'checkbox',
      '#default_value' => isset($field_config['status']) ? $field_config['status'] : FALSE,
    );

    $row['title_wrapper'] = array(
      '#type' => 'container',
    );
    $row['title_wrapper']['title'] = array(
      '#markup' => t('@field_label (%field_entity)', array(
          '@field_label' => $field_config['field_label'],
          '%field_entity' => $entity_label,
        )
      ),
    );

    $row['description_wrapper'] = array(
      '#type' => 'container',
    );
    $row['description_wrapper']['description'] = array(
      '#markup' => t('Check this box to display all %field fields as a single fieldset.', array(
        '%field' => $field_config['field_label'],
      )),
    );

    $row['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $field_config['field_label'])),
      '#title_display' => 'invisible',
      '#default_value' => isset($field_config['weight']) ? $field_config['weight'] : $main_field_weight,
      '#delta' => DEFAULT_WEIGHT_DELTA,
      '#attributes' => array('class' => array('parent-field')),
    );

    // Sub fields.
    foreach ($field['columns'] as $item => $info) {

      $sub_field_label = $field_instance['label'] . ': ' . $field['settings']['labels'][$item];
      $default_field_config = array(
        'entity_type' => $entity_type,
        'bundle' => $bundle,
        'field_name' => $field_name,
        'field_item' => $item,
        'field_label' => $sub_field_label,
      );
      $field_separator = _crm_core_profile_field_separator($default_field_config);
      $field_config = isset($profile->fields[$field_separator]) ? $profile->fields[$field_separator] : array();
      $field_config += array('handler' => get_class($this));
      $field_config += $default_field_config;

      if ($field_instance['settings']['inline_css'][$item] != 'display:none') {
        $sub_field = TRUE;
        $form['fields'][$field_separator] = array();
        $row =& $form['fields'][$field_separator];

        // Fixing subfields order.
        $weight = ++$main_field_weight;
        _crm_core_profile_default_weight();

        $row['#weight'] = isset($field_config['weight']) ? _crm_core_profile_weight_fix($field_config['weight']) : $weight;

        if ($sub_field) {
          $row['#attributes']['class'] = array('sub-field');
        }

        $row['field_name'] = array(
          '#type' => 'value',
          '#value' => $field_config['field_name'],
        );
        $row['field_item'] = array(
          '#type' => 'value',
          '#value' => $field_config['field_item'],
        );
        $row['handler'] = array(
          '#type' => 'value',
          '#value' => $field_config['handler'],
        );
        $row['entity_type'] = array(
          '#type' => 'value',
          '#value' => $field_config['entity_type'],
        );
        $row['bundle'] = array(
          '#type' => 'value',
          '#value' => $field_config['bundle'],
        );

        $row['status'] = array(
          '#type' => 'checkbox',
          '#default_value' => isset($field_config['status']) ? $field_config['status'] : FALSE,
        );

        $row['title_wrapper'] = array(
          '#type' => 'container',
        );
        $row['title_wrapper']['title'] = array(
          '#markup' => t('@field_label (%field_entity)', array(
              '@field_label' => $field_config['field_label'],
              '%field_entity' => $entity_label,
            )
          ),
        );

        $row['hidden'] = array(
          '#type' => 'checkbox',
          '#title' => t('Hidden'),
          '#title_display' => 'after',
          '#default_value' => isset($field_config['hidden']) ? $field_config['hidden'] : FALSE,
        );

        switch ($item) {
          case 'given':
          case 'middle':
          case 'family':
          case 'credentials':
            $row['default'] = array(
              '#type' => 'textfield',
              '#title' => t('Default Value'),
              '#default_value' => isset($field_config['default']) ? $field_config['default'] : '',
              '#size' => 10,
              '#maxlength' => 20,
            );
            break;

          case 'title':
          case 'generational':
            $row['default'] = array(
              '#type' => 'select',
              '#title' => t('Default Value'),
              '#default_value' => isset($field_config['default']) ? $field_config['default'] : '',
              '#options' => name_field_get_options($field, $item),
            );
            break;
        }

        $row['weight'] = array(
          '#type' => 'weight',
          '#title' => t('Weight for @title', array('@title' => $field_config['field_label'])),
          '#title_display' => 'invisible',
          '#default_value' => isset($field_config['weight']) ? $field_config['weight'] : $weight,
          '#delta' => DEFAULT_WEIGHT_DELTA,
        );
      }
    }
  }
}

function NameProfileFieldHandlerElementValidate(&$element, &$form_state, $form) {
  $field_name = $element['#field_name'];

  $subfields = _crm_core_profile_subfields($field_name);

  // Apply entity and bundle prefix to field component names.
  foreach ($subfields as $component => $subfield_name) {
    $subfields[$component] = $element['#entity_type'] . '_' . $element['#bundle'] . '_' . $subfield_name;
  }

  $input =& $form_state['input'];
  $values =& $form_state['values'];
  $complete_form =& $form_state['complete form'];

  foreach ($subfields as $subfield_name => $compound_name) {

    // @todo Use proper language.
    if (isset($input[$compound_name])) {
      $input['contact_name'][LANGUAGE_NONE][0][$subfield_name] = $input[$compound_name];
    }

    if (isset($values[$compound_name])) {
      $values['contact_name'][LANGUAGE_NONE][0][$subfield_name] = $values[$compound_name];
    }

    if (isset($complete_form[$compound_name]) && isset($complete_form[$compound_name]['#value'])) {
      $complete_form['contact_name'][LANGUAGE_NONE][0]['#value'][$subfield_name] = $complete_form[$compound_name]['#value'];
      $complete_form['contact_name'][LANGUAGE_NONE][0][$subfield_name]['#value'] = $complete_form[$compound_name]['#value'];
    }

    if (isset($form[$compound_name]['#value'])) {
      $element['#value'][$subfield_name] = $form[$compound_name]['#value'];
      $element[$subfield_name]['#value'] = $form[$compound_name]['#value'];
      $form['contact_name'][LANGUAGE_NONE][0]['#value'][$subfield_name] = $form[$compound_name]['#value'];
      $form['contact_name'][LANGUAGE_NONE][0][$subfield_name]['#value'] = $form[$compound_name]['#value'];
    }
  }
}
