<?php

/**
 * @file
 * Defines the inline entity form controller for bookable items.
 */

class BookitItemInlineEntityFormController extends EntityInlineEntityFormController {
    public function __construct($entityType, array $settings) {

    $this->entityType = $entityType;
    $this->settings = $settings + $this->defaultSettings();
  }

  /**
   * Overrides EntityInlineEntityFormController::css().
   */
  public function css() {
    return array(
      'base' => drupal_get_path('module', 'bookit_item') . '/theme/bookit-item.css',
    );
  }

  /**
   * Overrides EntityInlineEntityFormController::tableFields().
   */
  public function tableFields($bundles) {
    $fields = array();
    $fields['title'] = array(
      'type' => 'property',
      'label' => t('Bookable Item'),
      'weight' => 1,
    );

    // If only one booking type is allowed, its fields can be used as columns.
    if (count($bundles) == 1) {
      $bundle = reset($bundles);

      foreach (field_info_instances('bookit_item', $bundle) as $field_name => $instance) {
        $field = field_info_field($field_name);

        // If the bookable item has an imagefield, show it.
        if ($field['type'] == 'image') {

          // Determine the correct image style to use.
          $image_style = 'thumbnail';
          if (!empty($instance['widget']['settings']['preview_image_style'])) {
            $image_style = $instance['widget']['settings']['preview_image_style'];
          }

          $fields[$field_name] = array(
            'type' => 'field',
            'label' => $instance['label'],
            'formatter' => 'image',
            'settings' => array('image_style' => $image_style),
            'delta' => 0,
            'weight' => -10,
          );
          break;
        }
      }
    }

    $fields['status'] = array(
      'type' => 'property',
      'label' => t('Status'),
      'weight' => 100,
    );

    return $fields;
  }

  /**
   * Overrides EntityInlineEntityFormController::defaultSettings().
   */
  public function defaultSettings() {
    $defaults = parent::defaultSettings();
    $defaults['autogenerate_title'] = FALSE;

    return $defaults;
  }

  /**
   * Overrides EntityInlineEntityFormController::settingsForm().
   */
  public function settingsForm($field, $instance) {
    $form = parent::settingsForm($field, $instance);

    $form['autogenerate_title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto generate the bookable item title'),
      '#description' => t('This will hide the title input field and generate the title by appending any available attributes to the @entity title.', array('@entity' => $instance['entity_type'])),
      '#default_value' => $this->settings['autogenerate_title'],
    );

    return $form;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($entity_form, &$form_state) {
    global $user;

    // Get the labels (item / variation).
    $labels = $this->labels();
    $bookit_item = $entity_form['#entity'];
    $extra_fields = field_info_extra_fields('bookit_item', $bookit_item->type, 'form');

    // Assign newly created bookable items to the current user.
    if (empty($bookit_item->item_id)) {
      $bookit_item->uid = $user->uid;
    }

    $entity_form['bookit_item_attributes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Attributes'),
      '#attributes' => array('class' => array('container-inline', 'ief-bookit-item-attributes', 'ief-entity-fieldset')),
    );
    $entity_form['bookit_item_details'] = array(
      '#type' => 'fieldset',
      '#title' => t('Details'),
      '#attributes' => array('class' => array('ief-bookit-item-details', 'ief-entity-fieldset')),
    );

    $entity_form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('@label title', array('@label' => drupal_ucfirst($labels['singular']))),
      '#default_value' => $bookit_item->title,
      '#maxlength' => 255,
      '#required' => TRUE,
      '#fieldset' => 'bookit_item_details',
      // The label might be missing if the Title module has replaced it.
      '#weight' => !empty($extra_fields['title']) ? $extra_fields['title']['weight'] : -9,
    );
    $entity_form['status'] = array(
      '#type' => 'radios',
      '#title' => t('Status'),
      '#default_value' => $bookit_item->status,
      '#options' => array(1 => t('Active'), 0 => t('Disabled')),
      '#required' => TRUE,
      '#fieldset' => 'bookit_item_details',
      '#weight' => $extra_fields['status']['weight'],
    );

    // Attach fields.
    $langcode = entity_language('bookit_item', $bookit_item);
    field_attach_form('bookit_item', $bookit_item, $entity_form, $form_state, $langcode);

    // Hide the title field if it is auto-generated.
    if ($this->settings['autogenerate_title']) {
      $entity_form['title']['#required'] = FALSE;
      $entity_form['title']['#access'] = FALSE;
      // Hide the replacement field added by the Title module as well.
      if (module_exists('title')) {
        $title_field = title_field_replacement_info('bookit_item', 'title');
        if ($title_field) {
          $title_field_name = $title_field['field']['field_name'];
          if (isset($entity_form[$title_field_name])) {
            $entity_form[$title_field_name]['#access'] = FALSE;
            $entity_form[$title_field_name]['#required'] = FALSE;
          }
        }
      }
    }

    // Arrange attributes.
    $attributes = $this->attributes($bookit_item->type);
    if (empty($attributes)) {
      // Hide the fieldset, it will be empty.
      $entity_form['bookit_item_attributes']['#access'] = FALSE;
    }
    else {
      foreach ($attributes as $field_name => $attribute) {
        $entity_form[$field_name]['#fieldset'] = 'bookit_item_attributes';
      }
    }

    // Arrange non-attribute fields.
    foreach (field_info_instances('bookit_item', $bookit_item->type) as $name => $instance) {
      $field_name = $instance['field_name'];
      $field = field_info_field($field_name);
      if (!isset($attributes[$field_name])) {
        $entity_form[$field_name]['#fieldset'] = 'bookit_item_details';
      }
    }

    return $entity_form;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityFormValidate().
   */
  public function entityFormValidate($entity_form, &$form_state) {
    $bookit_item = $entity_form['#entity'];

    $parents_path = implode('][', $entity_form['#parents']);
    $bookit_item_values = drupal_array_get_nested_value($form_state['values'], $entity_form['#parents']);

    field_attach_form_validate('bookit_item', $bookit_item, $entity_form, $form_state);
  }

  /**
   * Overrides EntityInlineEntityFormController::entityFormSubmit().
   */
  public function entityFormSubmit(&$entity_form, &$form_state) {
    parent::entityFormSubmit($entity_form, $form_state);

    $bookit_item = $entity_form['#entity'];
    if (empty($bookit_item->title) && $this->settings['autogenerate_title']) {
      $bookit_item->title = t('Will be auto-generated when the form is saved.');
    }
  }

  /**
   * Returns a list of field names that are used as attributes for the given
   * item type.
   */
  protected function attributes($type) {
    // Attributes are tied to the commerce_cart module.
    if (!module_exists('commerce_cart')) {
      return array();
    }

    $attributes = array();
    // Loop through all the field instances on that booking type.
    foreach (field_info_instances('bookit_item', $type) as $name => $instance) {
      // A field qualifies if it is single value, required and uses a widget
      // with a definite set of options. For the sake of simplicity, this is
      // currently restricted to fields defined by the options module.
      $field = field_info_field($instance['field_name']);

      // Get the array of Cart settings pertaining to this instance.
      $commerce_cart_settings = commerce_cart_field_instance_attribute_settings($instance);

      // If the instance is of a field type that is eligible to function as
      // a bookable item attribute field and if its attribute field settings
      // specify that this functionality is enabled...
      if (commerce_cart_field_attribute_eligible($field) && $commerce_cart_settings['attribute_field']) {
        $attributes[$name] = array(
          'field' => $field,
          'instance' => $instance,
          'weight' => $instance['widget']['weight'],
        );
      }
    }

    // Sort the fields by weight.
    uasort($attributes, 'drupal_sort_weight');

    return $attributes;
  }

  /**
   * Overrides EntityInlineEntityFormController::save().
   *
   * Autogenerates the bookable item title if specified, and then saves the bookable item.
   */
  public function save($entity, $context) {
    // Remove the temporary message added in entityFormSubmit() so that
    // Commerce AutoSKU can do its thing.
    if (!empty($entity->_remove_sku)) {
      $entity->sku = NULL;
      unset($entity->_remove_sku);
    }

    // Generate the bookable item title. Take the parent entity title as the base.
    if ($this->settings['autogenerate_title']) {
      $entity->title = entity_label($context['parent_entity_type'], $context['parent_entity']);
      $attributes = $this->attributes($entity->type);
      if (!empty($attributes)) {
        $wrapper = entity_metadata_wrapper('bookit_item', $entity);
        $attribute_values = array();
        foreach ($attributes as $field_name => $attribute) {
          $attribute_label = $wrapper->{$field_name}->label();
          if (!empty($attribute_label)) {
            $attribute_values[] = $attribute_label;
          }
        }

        if (!empty($attribute_values)) {
          $entity->title .= ' (' . implode(', ', $attribute_values) . ')';
        }
      }
    }

    entity_save('bookit_item', $entity);
  }

  /**
   * Overrides EntityInlineEntityFormController::delete().
   *
   * Disables bookable items that can't be deleted (because they are already
   * referenced from a line item, or for some other reason), deletes the rest.
   */
  public function delete($ids, $context) {
    $bookit_items = entity_load('bookit_item', $ids);

    foreach ((array) $bookit_items as $bookit_item_id => $bookit_item) {
      if (!bookit_item_can_delete($bookit_item)) {
        $bookit_item->status = FALSE;
        entity_save('bookit_item', $bookit_item);

        unset($bookit_items[$bookit_item_id]);
      }
    }

    entity_delete_multiple('bookit_item', array_keys($bookit_items));
  }
}
