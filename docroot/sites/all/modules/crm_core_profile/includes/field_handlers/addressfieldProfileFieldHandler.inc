<?php
/**
 * @file
 * Field handler for addressfield fields.
 */

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
class addressfieldProfileFieldHandler extends DefaultProfileFieldHandler {

  /**
   * This function is triggered when a field is being used in a profile.
   */
  public function renderField($field_name, $fields_config, &$form, &$form_state) {

    $entity_type = $form[$field_name][LANGUAGE_NONE][0]['#entity_type'];
    $bundle = $form[$field_name][LANGUAGE_NONE][0]['#bundle'];
    // Store the field prefix.
    $field_prefix = $entity_type . '_' . $bundle . '_';

    // Shows or hides main field.
    $profile_field = $field_prefix . $field_name;
    if (!isset($fields_config[$profile_field]['status'])) {
      $form[$field_name]['#access'] = FALSE;
      $form[$field_name]['#element_validate'] = array();

      return;
    }
    elseif ($fields_config[$profile_field]['status'] == 1) {
      return;
    }
    else {
      $form[$field_name]['#access'] = FALSE;
      $form[$field_name]['#element_validate'] = array();
    }

    // Subfields.
    $field = field_info_field($field_name);
    $field_instance = field_info_instance($form['#entity_type'], $field_name, $form['#bundle']);

    // Are all subfields hidden?
    $subfields_all_hidden = TRUE;

    foreach (array_keys($field['columns']) as $column) {
      $subfield_name = $field_prefix . $field_name . '_' . $column;
      $subfld_config = $fields_config[$subfield_name];
      if ((bool) $subfld_config['status']) {
        switch ($column) {
          case 'country':
            $form[$subfield_name] = array(
              '#type' => 'select',
              '#title' => $this->subfieldLabel($column) . ':',
              '#options' => _addressfield_country_options_list($field, $field_instance),
              '#weight' => $subfld_config['weight'],
            );
            break;

          // Rendering state/admin area for well known countries as a list.
          case 'administrative_area':
            $country_list = array('AU', 'BR', 'CA', 'IT', 'US');
            $address['country'] = $field_instance['default_value'][0]['country'];
            if (in_array($address['country'], $country_list)) {
              // Getting the selection of state based on default country value.
              // @todo, this might have to be changed later with additional configuration on the crm core profile field selection UI              
              $format = array();
              $context['mode'] = 'other';
              module_load_include('inc', 'addressfield', 'plugins/format/address');
              addressfield_format_address_generate($format, $address, $context);
              $form[$subfield_name] = array('#type' => 'select');
              $form[$subfield_name] += $format['locality_block']['administrative_area'];
              $form[$subfield_name]['#size'] = 1;
              $form[$subfield_name]['#weight'] = $subfld_config['weight'];
            }
            else {
              $form[$subfield_name] = array(
                '#type' => 'textfield',
                '#title' => $this->subfieldLabel($column) . ':',
                '#weight' => $subfld_config['weight'],
              );
            }
            break;

          default:
            $form[$subfield_name] = array(
              '#type' => 'textfield',
              '#title' => $this->subfieldLabel($column) . ':',
              '#weight' => $subfld_config['weight'],
            );
            break;
        }
      }

      // Hide "hidden" field.
      if ($subfld_config['hidden']) {
        // If using subfields, set it on the subfield.
        $form[$subfield_name]['#type'] = 'hidden';
      }
      else {
        // Displaying at least one component.
        $subfields_all_hidden = FALSE;
      }

      $crm_core_profile = $form_state['profile'];
      $prepopulate = isset($crm_core_profile->entities['crm_core_contact']['prepopulate']) ? $crm_core_profile->entities['crm_core_contact']['prepopulate'] : FALSE;
      if ($entity_type == 'crm_core_contact' && $prepopulate) {
        if (empty($profile[$profile_field]['status'])) {
          // If using subfields, set it on the subfield.
          $entity = $form[$field_name][LANGUAGE_NONE][0]['#entity'];
          if (isset($entity->$field_name)) {
            $real_field = $entity->$field_name;
            $form[$subfield_name]['#default_value'] = $real_field[LANGUAGE_NONE][0][$column];
          }
        }
      }
      else {
        // Apply default values for each component.
        $default_value = '';
        // Find default value from profile settings.
        if (array_key_exists($subfield_name, $fields_config) && !empty($subfld_config['default'])) {
          $default_value = $subfld_config['default'];
        }
        if (!empty($default_value)) {
          if (empty($fields_config[$profile_field]['status'])) {
            // If using subfields, set it on the subfield.
            $form[$subfield_name]['#default_value'] = $default_value;
          }
          else {
            // Otherwise, use the real field element #default_value.
            $form[$field_name][LANGUAGE_NONE][0][$column]['#default_value'] = $default_value;
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
   * This function is triggered by the CRM Core Profile form submit handler.
   */
  public function submitField($field_name, $fields_config, &$form, &$form_state) {
    $field = field_info_field($field_name);
    $entity_type = $form[$field_name][LANGUAGE_NONE][0]['#entity_type'];
    $bundle = $form[$field_name][LANGUAGE_NONE][0]['#bundle'];
    $field_prefix = $entity_type . '_' . $bundle;
    $field_separator = $field_prefix . '_' . $field_name;
    $field_config = isset($fields_config[$field_separator]) ? $fields_config[$field_separator] : FALSE;
    // If config exists for main field, but main field is not enabled, we must
    // check it subfields. If config not exists or main field enabled we should
    // do nothing here.
    if ($field_config && empty($field_config['status'])) {
      foreach (array_keys($field['columns']) as $column) {
        $subfield_name = $field_prefix . '_' . $field_name . '_' . $column;
        $subfld_config = isset($fields_config[$subfield_name]) ? $fields_config[$subfield_name] : FALSE;
        if (!empty($subfld_config['status']) && isset($form_state['values'][$subfield_name])) {
          $form_state['values'][$field_name][LANGUAGE_NONE][0][$column] = $form_state['values'][$subfield_name];
        }
      }
    }
  }

  public function renderFieldConfig(&$form, $entity_type, $entity_label, $bundle, $field_name) {
    $field_config = array();

    if (isset($form['#profile'])) {
      $profile = $form['#profile'];
    }

    $field_info = field_info_field($field_name);
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

    $main_field_weight = $weight - count($field_info['columns']) - 1;

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
    foreach (array_keys($field_info['columns']) as $item) {

      $subfield_label = $this->subfieldLabel($item);
      $label = $field_instance['label'] . ': ' . $subfield_label;

      $default_field_config = array(
        'entity_type' => $entity_type,
        'bundle' => $bundle,
        'field_name' => $field_name,
        'field_item' => $item,
        'field_label' => $label,
      );

      $field_separator = _crm_core_profile_field_separator($default_field_config);
      $field_config = isset($profile->fields[$field_separator]) ? $profile->fields[$field_separator] : array();
      $field_config += array('handler' => get_class($this));
      $field_config += $default_field_config;
      if (!isset($field_config['weight'])) {
        $field_config['weight'] = ++$main_field_weight;
      }
      _crm_core_profile_default_weight();
      _crm_core_profile_new_form_fields($form, $entity_label, $field_config, TRUE);
    }
  }

  /**
   * Triggered by the #after_build callback of a CRM Core Profile form.
   */
  public function afterBuild($field_name, $profile, &$form, &$form_state) {
    if (!empty($form[$field_name][LANGUAGE_NONE][0]['#element_validate'])) {
      array_unshift($form[$field_name][LANGUAGE_NONE][0]['#element_validate'], 'addressfieldProfileFieldHandlerElementValidate');
    }
    else {
      $form[$field_name][LANGUAGE_NONE][0]['#element_validate'][] = 'addressfieldProfileFieldHandlerElementValidate';
    }
  }

  /**
   * Helper function returning a human readable label given the column.
   *
   * I.E administrative_area = State
   */
  protected function subfieldLabel($field_column) {
    // @todo: Subfield labeling is difficult. This is because address component
    // label depends on country selection. For example: United States: Zip Code,
    // International: Postal Code. State/Province, etc etc.
    $subfield_label = '';
    switch ($field_column) {
      case 'country':
        $subfield_label = t('Country');
        break;

      case 'locality':
        $subfield_label = t('City');
        break;

      case 'administrative_area':
        $subfield_label = t('State');
        break;

      case 'postal_code':
        $subfield_label = t('Postal code');
        break;

      case 'thoroughfare':
        $subfield_label = t('Address 1');
        break;

      case 'premise':
        $subfield_label = t('Address 2');
        break;

      case 'sub_premise':
        $subfield_label = t('Address 3');
        break;

      default:
        $subfield_label = ucwords(str_replace('_', ' ', $field_column));
        break;
    }

    return $subfield_label;
  }
}

/**
 * Address field validation.
 *
 * Used when there is subfields.
 */
function addressfieldProfileFieldHandlerElementValidate(&$element, &$form_state, $form) {

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
      $input[$field_name][LANGUAGE_NONE][0][$subfield_name] = $input[$compound_name];
    }

    if (isset($values[$compound_name])) {
      $values[$field_name][LANGUAGE_NONE][0][$subfield_name] = $values[$compound_name];
    }

    if (isset($complete_form[$compound_name])) {
      $complete_form[$field_name][LANGUAGE_NONE][0]['#value'][$subfield_name] = $complete_form[$compound_name]['#value'];
      $complete_form[$field_name][LANGUAGE_NONE][0][$subfield_name]['#value'] = $complete_form[$compound_name]['#value'];
    }

    if (isset($form[$compound_name]['#value'])) {
      $element['#value'][$subfield_name] = $form[$compound_name]['#value'];
      $element[$subfield_name]['#value'] = $form[$compound_name]['#value'];
      $form[$field_name][LANGUAGE_NONE][0]['#value'][$subfield_name] = $form[$compound_name]['#value'];
      $form[$field_name][LANGUAGE_NONE][0][$subfield_name]['#value'] = $form[$compound_name]['#value'];
    }
  }
}
