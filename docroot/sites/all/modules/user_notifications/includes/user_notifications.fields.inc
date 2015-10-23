<?php

/**
 * Creates a required, locked field base user_notifications_recipient_uid for
 * recipient_uid field.
 *
 * For now, this should not be called directly. It will be called when creating
 * the field instance.
 *
 * @return
 *  TRUE if success, FALSE if a failure creating the field base.
 */
function user_notifications_create_target_id_field_base() {
  $field = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'user_notifications_recipient_uid',
    'foreign keys' => array(
      'users' => array(
        'columns' => array(
          'target_id' => 'uid',
        ),
        'table' => 'users',
      ),
    ),
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 1,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'base',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'type' => 'none',
        ),
        'target_bundles' => array(),
      ),
      'target_type' => 'user',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  try {
    field_create_field($field);
    return TRUE;
  } catch (Exception $e) {
    drupal_set_message(t('Could not create recipient uid field base'), 'error');
    return FALSE;
  }
}

/**
 * Creates a required, locked instance of a user_notifications_recipient_uid
 * field on the specified bundle.
 *
 * @param $entity_type
 *   The type of entity the field instance will be attached to.
 * @param $bundle
 *   The bundle name of the entity the field instance will be attached to.
 * @param $label
 *   The label of the field instance.
 * @param $weight
 *   The default weight of the field instance widget and display.
 *
 * @return
 *  TRUE if success, FALSE if a failure creating the field instance.
 */
function user_notifications_create_target_id_field_instance($entity_type, $bundle, $label, $weight = 0) {
  $field_name = "user_notifications_recipient_uid";
  //TODO:Consider whether we need to activate the field. Could it be disabled
  //with the module being disabled?

  $field = field_info_field($field_name);
  $instance = field_info_instance($entity_type, $field_name, $bundle);

  //Check to make sure the base exists and if not, create it.
  if (empty($field)) {
    user_notifications_create_target_id_field_base();
  }

  if (empty($instance)) {
    $instance = array(
      'bundle' => $bundle,
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'entityreference',
          'settings' => array(
            'link' => FALSE,
          ),
          'type' => 'entityreference_label',
          'weight' => 1,
        ),
      ),
      'entity_type' => $entity_type,
      'field_name' => $field_name,
      'label' => $label,
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'entityreference',
        'settings' => array(
          'match_operator' => 'CONTAINS',
          'path' => '',
          'size' => 60,
        ),
        'type' => 'entityreference_autocomplete',
        'weight' => 2,
      ),
    );

    $entity_info = entity_get_info($entity_type);

    try {
      field_create_instance($instance);
      return TRUE;
    } catch (Exception $e) {
      drupal_set_message(t("Could not create @field instance", array("@field => $field_name"), 'error'));
      return FALSE;
    }
  }
}

/**
 * Implements hook_field_default_field_bases().
 */
function user_notifications_create_message_field_base() {
  $field = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'user_notifications_message',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );

  try {
    field_create_field($field);
    return TRUE;
  } catch (Exception $e) {
    drupal_set_message(t('Could not create message field base'), 'error');
    return FALSE;
  }
}

/**
 * Implements hook_field_default_field_instances().
 */
function user_notifications_field_default_field_instances() {
  $instance = array(
    'bundle' => 'basic_notification',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter a message that will be displayed to a user as a notification.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'user_notifications',
    'field_name' => 'user_notifications_message',
    'label' => 'New Message',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 3,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Enter a message that will be displayed to a user as a notification.');
  t('New Message');

  try {
    field_create_instance($instance);
    return TRUE;
  } catch (Exception $e) {
    drupal_set_message(t("Could not create @field instance", array("@field => $field_name"), 'error'));
    return FALSE;
  }
}
