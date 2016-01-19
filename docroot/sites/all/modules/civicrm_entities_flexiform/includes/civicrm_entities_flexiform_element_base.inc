<?php

class CiviCrmEntitiesFlexiformElementBase extends FlexiformElement {

  /**
   * Return the form element for this FlexiformElement.
   */
  public function form($form, &$form_state, $entity, $language = LANGUAGE_NONE) {
    return parent::form($form, $form_state, $entity, $language);
  }

  /**
   * Validate the form element.
   */
  public function formValidate($form, &$form_state, $entity, $language = LANGUAGE_NONE) {
    $errors = array();
    $tmp_entity = clone $entity;
    // Merge the form_state values with the $tmp_entity so we can pass it off for validation.
    foreach($form_state['values'][$this->entity_namespace] as $prop => $val) {
      $tmp_entity->{$prop} = $val;
    }
    property_validation_field_attach_validate($this->entity_type, $tmp_entity, $errors);
  }

  private static function getEntityTypeValues($form_state, $entity_type) {

  }

  /**
   * Submit the form element.
   */
  public function formSubmit($form, &$form_state, $entity, $language = LANGUAGE_NONE) {
    $value = $this->formExtractValues($form, $form_state, $entity);
    $entity->{$this->element_info['name']} = $value;
  }

  /**
   * Extract the submitted values for this form element.
   */
  public function formExtractValues($form, &$form_state, $entity) {
    $parents = $form['#parents'];
    $parents[] = $this->getEntityNamespace();
    $parents[] = $this->element_info['name'];
    $value = drupal_array_get_nested_value($form_state['input'], $parents);
    return $value;
  }

  /**
   * {@inheritdoc}
   */
  public function configureForm($form, &$form_state, $flexiform) {
    $form = parent::configureForm($form, $form_state, $flexiform);

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function configureFormSubmit($form, &$form_state, $flexiform) {

    parent::configureFormSubmit($form, $form_state, $flexiform);
  }

  /**
   * Gets the fields for a CiviCRM entity type.
   *
   * @param $entity_type
   * @internal param string $entity
   * @return array
   */
  static function getEntityFields($entity_type) {
    // Remove the "civicrm_" if it's there.
    $entity_type = str_ireplace('civicrm_', '', $entity_type);
    // Build the actual civicrm entity type defined by the civicrm_entities module.
    $entity_name = 'civicrm_' . strtolower($entity_type);
    $elements = array();
    civicrm_initialize();
    $fields = civicrm_api(
      $entity_type,
      'getfields',
      array(
        'version' => 3
      )
    );
    // There are no bundles for the civicrm entities so just use the entity name.
    $entity_bundle = $entity_name;
    // Create a label.
    $entity_label = 'CivCRM ' . ucwords(str_ireplace($entity_type, '_', ''));
    // Loop over the fields for the entity and defined some details to pass off to the controller class.
    foreach($fields['values'] as $field_name => &$field) {
      // If there isn't at least a "type" to go off of, skip the property because we don't have enough info.
      if(isset($field['type'])) {
        self::processFkField($field);
        // Get the string type from the enum value.
        $field['type_name'] = CRM_Utils_Type::typeToString($field['type']);
        // Save the enum for later use by the controller class.
        $field['type_enum'] = $field['type'];
        $property = $field['name'];

        // Define some basic settings.
        $settings = array(
          'label' => empty($field['title']) ? $field['label'] : $field['title'],
          'class' => self::getClassFromFieldType($field),
          'type' => $property,
          'group' => $entity_label,
        );
        if(empty($settings['label'])) {
          $settings['label'] = $field['name'];
        }
        // Merge the
        $settings = array_merge($field, $settings);
        $elements[$entity_name][$entity_bundle][$entity_name . ':' . $property] = $settings;
      }
    }


    return $elements;
  }


  /**
   * Handles adding additional meta data to an entity's fields that are FK (foreign key) fields.
   *
   * @param array $field
   */
  private static function processFkField(&$field) {
    // If data is a FK, then we can assume it's a 1-to-1 relationship,
    // so we'd want to use something like a radio or select html element.

    // Loop over the entity and find applicable fields.
    // If the field name ends with "_id" and has a pseudoconstant element.
    $name = $field['name'];
    $field_id = str_ireplace('custom_', '', $name);
    //if(stripos($name, '_id') == (strlen($name) - 3)
    //  && isset($field['pseudoconstant'])) {
    if(isset($field['pseudoconstant'])) {
      // This is a FK field, get the additional details this field needs.
      // Essentially it needs the possible values for the field.
      $options = self::processFkFieldValues($field['pseudoconstant']);
      $field['is_multiple'] = true;

    }
    elseif(!empty($field['option_group_id'])) {
      $options = CRM_Core_BAO_CustomOption::valuesByID($field_id,
        $field['option_group_id']
      );
      $field['is_multiple'] = true;
    }
    if(!empty($options)) {
      $field['options'] = $options;
    }
  }

  /**
   * Gets the values for a FK field by the pseudoconstant details provided from civicrm_api('entity_name', 'getfields').
   *
   * @param array $pseudoconstant A field's pseudoconstant details from civicrm_api('entity_name', 'getfields').
   * @throws Exception Throws exception if a type isn't implemented yet.
   * @return array
   */
  private static function processFkFieldValues($pseudoconstant) {
    if(isset($pseudoconstant['table'])) {
      $query = db_select($pseudoconstant['table'], 't')
        ->fields('t', array($pseudoconstant['keyColumn'], $pseudoconstant['labelColumn']))
        ->execute();
      $results = $query->fetchAllKeyed();
      return $results;
    }
    elseif (isset($pseudoconstant['optionGroupName'])) {
      $search = array('name' => $pseudoconstant['optionGroupName']);
      CRM_Core_BAO_OptionGroup::retrieve($search, $search);
      if(isset($search['id'])) {
        $results = CRM_Core_BAO_OptionValue::getOptionValuesArray($search['id']);
        return $results;
      }
    }
    else {
      throw new Exception('Unknown FK option values type.');
    }

    return null;
  }

  /**
   * Gets the FlexiformElement class name to use for a CiviCRM entity property based on the field's details.
   *
   * @param array $field The property's details as returned from civicrm_api('entity_name', 'getfields').
   * @return string The class name that should handle the field for Flexiform.
   */
  private static function getClassFromFieldType($field) {
    $class = self::fieldToTypeMapping($field['name']);
    if($class) {
      return $class;
    }

    $class = 'CiviCrmFlexiformTextElement';
    // Special CiviCRM country select that triggers filtering for the state/province.
    if($field['name'] == 'country_id') {
      $class = 'CiviCrmFlexiformCountryElement';
    }
    elseif(isset($field['html_type'])) {
      $class = 'CiviCrmFlexiform' . $field['html_type'] . 'Element';
    }
    elseif($field['options']) {
      $class = 'CiviCrmFlexiformSelectElement';
    }
    // Checkbox elements
    elseif(empty($field['options']) && $field['type_name'] == 'Boolean') {
      $class = 'CiviCrmFlexiformCheckboxElement';
    }

    return $class;
  }

  private static function fieldToTypeMapping($field_name) {
    $mappings = array(
      'do_not_email' => 'CiviCrmFlexiformCheckboxElement',
      'country_id' => 'CiviCrmFlexiformCountryElement',
      'state_province_id' => 'CiviCrmFlexiformStateElement',
      'image_URL' => 'CiviCrmFlexiformImageElement',
    );
    if(array_key_exists($field_name, $mappings)) {
      return $mappings[$field_name];
    }
    return null;
  }


  /**
   * @param string
   * @return array
   */
  static function explodeMultiValueStr($str) {
    $sp = CRM_Core_DAO::VALUE_SEPARATOR;
    if (is_array($str)) {
      return $str;
    }
    return explode($sp, trim((string) $str, $sp));
  }

  /**
   * Get contact types and sub-types
   * Unlike pretty much every other option list CiviCRM wants "name" instead of "id"
   *
   * @return array
   */
  static function getContactTypes() {
    static $contact_types = array();
    static $sub_types = array();
    if (!$contact_types) {
      $data = self::crmApiValues('contact_type', 'get', array('is_active' => 1));
      foreach ($data as $type) {
        if (empty($type['parent_id'])) {
          $contact_types[strtolower($type['name'])] = $type['label'];
          continue;
        }
        $sub_types[strtolower($data[$type['parent_id']]['name'])][$type['name']] = $type['label'];
      }
    }
    return array($contact_types, $sub_types);
  }

  /**
   * Get the values from an api call
   *
   * @param string $entity
   *   API entity
   * @param string $operation
   *   API operation
   * @param array $params
   *   API params
   * @param string $value
   *   Reduce each result to this single value
   *
   * @return array
   *   Values from API call
   */
  static function crmApiValues($entity, $operation, $params = array(), $value = NULL) {
    $params += array('options' => array());
    // Work around the api's default limit of 25
    $params['options'] += array('limit' => 9999);
    $ret = self::crmAval(self::civiCrmApi($entity, $operation, $params), 'values', array());
    if ($value) {
      foreach ($ret as &$values) {
        $values = self::crmAval($values, $value);
      }
    }
    return $ret;
  }

  /**
   * Return a value from nested arrays or objects.
   *
   * @param $haystack
   *   The array to search
   * @param $keys
   *   Pass a single key, or multiple keys separated by : to get a nested value
   * @param $default
   *   Value to return if given array key does not exist
   * @param bool $strict
   *   Should we use empty or isset to determine if array key exists?
   *
   * @return array|null : found value or default
   */
  static function crmAval($haystack, $keys, $default = NULL, $strict = FALSE) {
    foreach (explode(':', $keys) as $key) {
      if (is_object($haystack)) {
        $haystack = (array) $haystack;
      }
      if (!is_array($haystack) || !isset($haystack[$key]) || (empty($haystack[$key]) && $default !== NULL && !$strict)) {
        return $default;
      }
      $haystack = $haystack[$key];
    }
    // $haystack has been reduced down to the item we want
    return $haystack;
  }

  /**
   * Wrapper for all CiviCRM API calls
   * For consistency, future-proofing, and error handling
   *
   * @param string $entity
   *   API entity
   * @param string $operation
   *   API operation
   * @param array $params
   *   API params
   *
   * @return array
   *   Result of API call
   */
  function civiCrmApi($entity, $operation, $params) {
    $params += array(
      'check_permissions' => FALSE,
      'version' => 3
    );
    $result = civicrm_api($entity, $operation, $params);
    if (!empty($result['is_error'])) {
      $bt = debug_backtrace();
      $file = explode('/', $bt[0]['file']);
      watchdog('webform_civicrm', 'The CiviCRM "%function" API function returned the error: "%msg" when called by line !line of !file with the following parameters: "!params"', array(
        '%function' => $entity . ' ' . $operation,
        '%msg' => $result['error_message'],
        '!line' => $bt[0]['line'],
        '!file' => array_pop($file),
        '!params' => print_r($params, TRUE)
      ), WATCHDOG_ERROR);
    }
    return $result;
  }

  /**
   * Adds the entity, bundle, and property info to a form element under #entity_info.
   *
   * @param array $element Drupal form element.
   * @return array The altered form element.
   */
  protected function addEntityInfo($element) {
    $element['#entity_info'] = array(
      'entity_type' => $this->entity_type,
      'bundle' => $this->bundle,
      'property' => $this->element_info['name'],
    );
    return $element;
  }

  /**
   * Add the default settings for a element such as validation and entity info.
   *
   * @param array $element The starting Drupal element to apply default settings to.
   * @param array $form The Drupal form build array.
   * @param object $entity The entity this element is based off of.
   * @return array Returns the altered Drupal form element.
   */
  protected function defaultElementInfo($element, $form, $entity) {
    $parents = $form['#parents'];
    $parents[] = $this->element_info['name'];
    // We need tht entity info to be applied to the element so other
    // functions can work off of that data.
    $element = self::addEntityInfo($element);
    $element['#default_value'] = $this->getDefaultValue($form, $entity);
    $element['#parents'] = $parents;
    $element['#title'] = $this->label();
    // Apply any configured entity property validation rules via the property_validation module.
    $element = self::addValidation($element);

    // Return the updated element because the $element argument was not passed by reference.
    return $element;
  }

  /**
   * Adds validation rules to the element based on the #entity_info values.
   * @param $element
   * @return array|false
   */
  protected static function addValidation($element) {
    $validation_api = new PropertyFapiValidation(
      $element['#entity_info']['entity_type'],
      $element['#entity_info']['bundle'],
      $element['#entity_info']['property']);

    $element = $validation_api->addPropertyValidation($element);
    return $element;
  }

  /**
   * Gets the potential default value for this element.
   *
   * @param array $form Drupal form array.
   * @param object $entity The entity that this element is used for.
   * @return string
   */
  protected function getDefaultValue($form, $entity) {

    // Work out the default value.
    $default = '';
    if (!empty($this->settings['default_value']['default_value'])) {
      $default = $this->settings['default_value']['default_value'];
    }
    if (!empty($this->settings['default_value']['use_tokens'])) {
      $default = $this->replaceCtoolsSubstitutions($default, $form['#flexiform_entities']);
    }

    // Should we load an initial value from an existing entity?
    if ($entity->id && isset($entity->{$this->element_info['name']})) {
      $default = $entity->{$this->element_info['name']};
    }

    return $default;
  }


}
