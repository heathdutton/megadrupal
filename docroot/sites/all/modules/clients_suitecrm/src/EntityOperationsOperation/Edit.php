<?php
/**
 * @file
 * Generic entity form for suiteCRM entities.
 */

namespace Drupal\clients_suitecrm\EntityOperationsOperation;

/**
 * Op handler for editing a SuiteCRM entity.
 */
class Edit extends \EntityOperationsOperationForm {

  /**
   * Returns basic information about the operation.
   */
  public function operationInfo() {
    return array(
      'label' => 'Edit',
      'description' => 'Produces a generic form to edit a suiteCRM entity.',
    ) + parent::operationInfo();
  }

  /**
   * Returns strings for the operations.
   */
  public function operationStrings() {
    return array(
      'tab title' => 'Edit',
      'label' => 'Edit',
      'description' => 'Produces a form to edit the suiteCRM entity.',
      'page title' => t('Edit %label'),
      'button label' => t('Save'),
      // This appears to be sufficient to result in the markup form element
      // not being created.
      'confirm question' => NULL,
      'submit message' => t('The @entity-type %label has been saved.'),
      // These intentionally have no replacements for the placeholders; these
      // are replaced in getOperationString().
    );
  }

  /**
   * Properties for the menu item specific to this handler.
   */
  public function menu_item($operation_path, $operation_definition, $loader_position) {
    $form_id = $this->getFormID($this->entityType, $operation_path);
    return array(
      'page callback' => 'drupal_get_form',
      'page arguments' => array(
        $form_id,
        $this->entityType,
        get_class($this),
        $operation_path,
        $loader_position, // Provides the entity as a parameter.
      ),
      'access callback' => 'entity_operations_operation_access_callback',
      'access arguments' => array(
        $this->entityType,
        get_class($this),
        $operation_path,
        $loader_position,
      ),
    );
  }

  /**
   * Get the form ID for the operation form.
   *
   * @param string $entity_type
   *   The entity type.
   * @param string $operation_path
   *   The path component for the operation.
   *
   * @return string
   *   A form ID that will be recognized by entity_operations_forms().
   */
  public function getFormID($entity_type, $operation_path) {
    // Force the form ID, so that child classes get the same one. This means
    // both the add and edit generic operation forms get the same form ID, and
    // thus makes form alteration easier.
    return implode('_', array(
      'entity_operations_operation_form',
      $entity_type,
      'edit',
    ));
  }

  /**
   * Form builder for this operation.
   */
  public function form($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Kill the cancel link.
    unset($form_state['entity_operation_form_elements']['cancel link']);

    return $this->entityForm($form, $form_state, $entity_type, $entity, $operation_path, 'edit');
  }

  /**
   * Helper for form(), building just the entity form.
   *
   * Keeps the entity form separate from any wrapping logic and allows clearer
   * reuse by subclasses.
   *
   * Note that overriding this will cause inheritance problems because this is
   * also used by the EntityOperationsOperationAddGeneric handler. If you want
   * to customize the form for your entity, you may want to consider using
   * the EntityOperationsOperationEdit handler instead.
   *
   * @param ...
   *   The same parameters as form().
   * @param string $form_op
   *   The 'op' value to set on the entity form state. Either 'edit' or 'add'.
   */
  public function entityForm($form, &$form_state, $entity_type, $entity, $operation_path, $form_op) {
    $entity_info = entity_get_info($this->entityType);


    // Ripped from entity_ui_form_defaults().
    $defaults = array(
      'entity_type' => $this->entityType,
    );
    if (isset($entity)) {
      $defaults[$this->entityType] = $entity;
    }
    if (isset($form_op)) {
      $defaults['op'] = $form_op;
    }
    $form_state += $defaults;

    // The bundle needs to be set as a form value for building the pseudo-entity
    // in entity_form_field_validate().
    if (!empty($entity_info['entity keys']['bundle'])) {
      $bundle_key = $entity_info['entity keys']['bundle'];
      $form[$bundle_key] = array(
        '#type' => 'value',
        '#value' => isset($entity->{$bundle_key}) ? $entity->{$bundle_key} : $this->entityType,
      );
    }

    // Iterate over the properties and create simple imput elements for now.
    $wrapper = entity_metadata_wrapper($this->entityType, $entity);
    $properties = $wrapper->getPropertyInfo();
    // Diff with all the remote properties.
    $local_properties = array_diff_key($properties, $entity_info['property map']);

    form_load_include($form_state, 'inc', 'clients_suitecrm', 'includes/clients_suitecrm.form');

    $reserved_properties = drupal_map_assoc($entity_info['entity keys']) + $entity_info['remote entity keys'] + array(
      'remote_id' => 'remote_id',
      'entity_data' => 'entity_data',
      'remote_saved' => 'remote_saved',
      'created' => 'created',
      'changed' => 'changed',
      'type' => 'type',
      'expires' => 'expires',
    );
    foreach ($local_properties as $property_key => $property_info) {
      // Just add non-core properties and no fields.
      if (!isset($reserved_properties[$property_key]) && empty($property_info['field'])) {
        if ($wdiget = $this->getPropertyWidget($entity, $property_key, $property_info)) {
          $form[$property_key] = $wdiget;
        }
      }
    }

    if (!empty($entity_info['fieldable'])) {
      field_attach_form($this->entityType, $entity, $form, $form_state);
    }

    $form['actions']['save_remote'] = array(
      '#type' => 'submit',
      '#value' => t('Remote save now'),
      '#op' => 'remote_save',
      '#weight' => 1,
    );

    return $form;
  }

  /**
   * Creates the input widget for a given property.
   *
   * @TODO handle more complex data types.
   *
   * @param \Drupal\clients_suitecrm\Entity\SuiteCrm $entity
   *   The entity to handle.
   * @param string $property
   *   The property to handle.
   * @param array $property_info
   *   The information of the property.
   *
   * @return array|NULL
   *   The form element array or NULL if this property can't be displayed.
   */
  protected function getPropertyWidget($entity, $property, $property_info) {
    $clients_resource = $entity->clientsResource();
    $entity_info = $entity->entityInfo();

    $element = array(
      '#title' => $property_info['label'],
      '#description' => isset($property_info['description']) ? $property_info['description'] : '',
      '#default_value' => isset($entity->{$property}) ? $entity->{$property} : '',
    );

    // Get information about the field from the remote configuration as this
    // provides a more detailed insight.
    $data_type = $property_info['type'];
    $remote_property = substr($property, 4);
    if (isset($entity_info['property map']['s_' . $remote_property]) && isset($clients_resource->configuration['fields']['module_fields'][$remote_property])) {
      $remote_field_info = $clients_resource->configuration['fields']['module_fields'][$remote_property];

      // Reuse the label from the CRM.
      $element['#title'] = $remote_field_info['label'];

      // If this field has a length ensure the field has set a max length.
      if (!empty($remote_field_info['len'])) {
        $element['#maxlength'] = $remote_field_info['len'];
      }

      // Check if this is a required field.
      $element['#required'] = !empty($remote_field_info['required']);

      // Use the remote data type since this is more specific.
      $data_type = $remote_field_info['type'];
    }

    switch ($data_type) {
      case 'date':
//        $element['#process'] = array('clients_suitecrm_process_date_field');
//        if (module_exists('date')) {
//          $date = new \DateTime('@' . $entity->{$property});
//          $element['#default_value'] = $date->format(DATE_FORMAT_DATE);
//          $element['#date_format'] = DATE_FORMAT_DATE;
//          $element['#date_type'] = DATE_FORMAT_UNIX;
//          if (module_exists('date_popup')) {
//            $element['#type'] = 'date_popup';
//            break;
//          }
//          $element['#type'] = 'date_select';
//          break;
//        }
//        unset($element['#default_value']);
//        if (isset($entity->{$property})) {
//          $date = new \DateTime('@' . $entity->{$property});
//          $element['#default_value'] = array(
//            'year' => $date->format('Y'),
//            'month' => $date->format('m'),
//            'day' => $date->format('d'),
//          );
//        }
//        $element['#type'] = 'date';
//        break;

      case 'datetime':
//        $element['#process'] = array('clients_suitecrm_process_date_field');
//        if (module_exists('date')) {
//          $date = new \DateTime('@' . $entity->{$property});
//          $element['#default_value'] = $date->format(DATE_FORMAT_DATETIME);
//          $element['#date_format'] = DATE_FORMAT_DATETIME;
//          $element['#date_type'] = DATE_FORMAT_UNIX;
//          if (module_exists('date_popup')) {
//            $element['#type'] = 'date_popup';
//            break;
//          }
//          $element['#type'] = 'date_select';
//          break;
//        }
      case 'integer':
        $element['#element_validate'] = array('element_validate_integer');
        $element['#type'] = 'textfield';
        break;

      case 'float':
        $element['#element_validate'] = array('element_validate_number');
        $element['#type'] = 'textfield';
        break;

      case 'text':
        $element['#type'] = 'textarea';
        break;

      case 'url':
        $element['#element_validate'] = array('clients_suitecrm_element_validate_url');
        $element['#type'] = 'textfield';
        break;

      case 'email':
        $element['#element_validate'] = array('clients_suitecrm_element_validate_email');
        $element['#type'] = 'textfield';
        break;

      case 'phone':
        $element['#element_validate'] = array('clients_suitecrm_element_validate_phone');
        $element['#type'] = 'textfield';
        break;

      case 'name':
      case 'id':
      case 'varchar':
        $element['#type'] = 'textfield';
        break;

      case 'boolean':
        $element['#type'] = 'checkbox';
        break;

      default:
        // Unknown types shouldn't be displayed to avoid storage issues.
        return NULL;
    }
    return $element;
  }

  /**
   * Form validation handler for this operation.
   *
   * Receives the same parameters as the form builder as a convenience.
   */
  public function formValidate($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Notify field widgets to validate their data.
    entity_form_field_validate($this->entityType, $form, $form_state);
  }

  /**
   * Form submit handler for this operation.
   *
   * Receives the same parameters as the form builder as a convenience.
   */
  public function formSubmit($form, &$form_state, $entity_type, $entity, $operation_path) {
    $entity_info = entity_get_info($entity_type);

    // Add the bundle property to the entity if the entity type supports bundles
    // and the form provides a value for the bundle key. Especially new entities
    // need to have their bundle property pre-populated before we invoke
    // entity_form_submit_build_entity().
    // This duplicates what entity_ui_form_submit_build_entity() does, but all
    // that does is call the entity admin UI controller, and we don't want a
    // dependency on that for just one of its methods which is pretty basic at
    // that.
    // @see EntityDefaultUIController::entityFormSubmitBuildEntity()
    if (!empty($entity_info['entity keys']['bundle']) && isset($form_state['values'][$entity_info['entity keys']['bundle']])) {
      $bundle_key = $entity_info['entity keys']['bundle'];
      $form_state[$entity_type]->{$bundle_key} = $form_state['values'][$bundle_key];
    }

    entity_form_submit_build_entity($entity_type, $form_state[$entity_type], $form, $form_state);
    $entity = $form_state[$entity_type];

    // The save operation will fail if the entity has no properties to save to
    // its base table. This is caused by a bug in drupal_write_record().
    // @see http://drupal.org/node/970338.

    // Save the entity.
    if (isset($form_state['clicked_button']['#op']) && $form_state['clicked_button']['#op'] == 'remote_save') {
      $entity->remoteSave();
    }
    else {
      $entity->save();
    }


    $message = $this->getOperationString('submit message', $entity_type, $entity, $operation_path);
    if (!empty($message)) {
      drupal_set_message($message);
    }

    // Redirect to the entity.
    if (empty($form_state['redirect'])) {
      $form_state['redirect'] = $this->getFormSubmitRedirect($this->entityType, $entity);
    }
  }

}
