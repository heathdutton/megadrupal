<?php

/**
 * @file
 * Functions, forms and other stuff for Shared Content administration.
 *
 * @ingroup sharedcontent_admin
 */

/**
 * Enable/Disable Shared Content connections
 *
 * Menu callback to enable or disable Shared Content connections.
 */
function sharedcontent_client_configure_endpoints_connections($connection) {
  $active_connections = variable_get('sharedcontent_endpoint_connections', array());
  if (!empty($active_connections)) {
    $active_connections = array_combine($active_connections, $active_connections);
  }

  switch ($_GET['action']) {
    case SHAREDCONTENT_CLIENT_ACTION_CONNECTION_ENABLE:
      $active_connections[$connection->name] = $connection->name;
      break;
    case SHAREDCONTENT_CLIENT_ACTION_CONNECTION_DISABLE:
      unset($active_connections[$connection->name]);
      break;
  }

  variable_set('sharedcontent_endpoint_connections', array_values($active_connections));

  drupal_goto();
}

/**
 * Form to select the entities enabled to be linable to Shared Content.
 *
 * @see sharedcontent_configure_types_submit()
 */
function sharedcontent_client_configure_linkables($form, &$form_state) {
  if (!empty($form_state['linkables'])) {
    $description = t('If you confirm, a Shared Content reference field will be added to the selected entities. If there is such a field on unselected entities it will be deleted. This can lead to unwanted data loss!');
    $form = confirm_form($form, t('Do you really want to proceed?'), 'admin/config/services/sharedcontent/client/linkables', $description);
  }
  else {
    $field_name = sharedcontent_client_get_default_shared_content_field_name();
    foreach (entity_get_info() as $entity_type => $entity_values) {
      $form[$entity_type] = array(
        '#type' => 'fieldset',
        '#title' => $entity_values['label'],
        '#tree' => TRUE,
      );
      $types = array();
      $defaults = array();
      foreach ($entity_values['bundles'] as $bundle => $values) {
        $types[$bundle] = $values['label'];
        if (field_info_instance($entity_type, $field_name, $bundle)) {
          $defaults[] = $bundle;
        }
      }

      $form[$entity_type]['bundles'] = array(
        '#type' => 'checkboxes',
        '#options' => $types,
        '#default_value' => $defaults,
      );
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update linkable entities')
    );
  }
  return $form;
}

/**
 * Form callback to update the linkable entityes.
 *
 * Add or remove a sharedcontent reference field to or from the selected
 * entities.
 *
 * @see sharedcontent_configure_types()
 */
function sharedcontent_client_configure_linkables_submit($form, &$form_state) {

  if (empty($form_state['linkables'])) {
    $form_state['linkables'] = $form_state['values'];
    $form_state['rebuild'] = TRUE;
  }
  else {

    $field_name = sharedcontent_client_get_default_shared_content_field_name();

    foreach ($form_state['linkables'] as $entity_type => $bundles) {
      if (is_array($bundles) && $bundles = $bundles['bundles']) {
        foreach ($bundles as $bundle => $selection) {
          $instance = field_info_instance($entity_type, $field_name, $bundle);
          if (empty($instance) && $selection) {
            sharedcontent_client_attach_shared_content_field($field_name, $entity_type, $bundle);
          }
          // @todo: Really delete the instance?
          elseif (!empty($instance) && !$selection) {
            field_delete_instance($instance);
          }
        }
      }
    }
    unset($form_state['linkables']);
  }
}

/**
 * Attach a new Shared Content reference field to an entity type.
 *
 * @param $field_name
 *   Name string of the field.
 * @param $bundle
 *   Bundle string of the entity.
 * @param $entity_type
 *   Type string of the entity.
 * @return object
 *   Field instance object.
 */
function sharedcontent_client_attach_shared_content_field($field_name, $entity_type, $bundle) {
  $instance = array(
    'label' => t('Shared Content'),
    'field_name' => $field_name,
    'bundle' => $bundle,
    'entity_type' => $entity_type,
    'display' => array(
      'default' => array(
        'label' => 'above',
        'type' => 'sharedcontent',
        'settings' => array(
          'mode' => 'sharedcontent_teaser',
          'link' => 'origin',
        ),
      ),
      'teaser' => array(
        'label' => 'hidden',
        'type' => 'hidden',
      ),
      'sharedcontent_teaser' => array(
        'label' => 'hidden',
        'type' => 'hidden',
      ),
    ),
  );

  return field_create_instance($instance);
}

/**
 * Creates the Shared Content field.
 *
 * @param $field_name
 *   Name string of the field.
 * @return array
 *   Field object.
 */
function sharedcontent_client_create_shared_content_field($field_name) {
  $field = array(
    'field_name' => $field_name,
    'type' => 'entityreference',
    'cardinality' => FIELD_CARDINALITY_UNLIMITED,
    'translatable' => FALSE,
    'settings' => array(
      'target_type' => 'sharedcontent_index',
      'handler' => 'sharedcontent',
      'handler_settings' => array(
        'target_bundles' => array(
          SHAREDCONTENT_INDEX_BUNDLE_LOCAL => SHAREDCONTENT_INDEX_BUNDLE_LOCAL,
          SHAREDCONTENT_INDEX_BUNDLE_REMOTE => SHAREDCONTENT_INDEX_BUNDLE_REMOTE,
        ),
        'sort' => array(
          'type' => 'property',
          'property' => 'changed',
          'direction' => 'DESC',
        ),
      ),
    ),
  );
  return field_create_field($field);
}
