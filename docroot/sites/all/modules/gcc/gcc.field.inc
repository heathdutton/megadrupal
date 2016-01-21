<?php

/**
 * @file
 * Defines and handles the fields required by the Group module.
 */

// Fields type defines.
define('GCC_FIELD_TYPE_ENABLE', 'gcc_enable');
define('GCC_FIELD_TYPE_AUDIENCE', 'gcc_audience');

// Fields name defines.
define('GCC_FIELD_ENABLE', 'field_gcc_enable');
define('GCC_FIELD_AUDIENCE', 'field_gcc_audience');

/* Declaration and Management of the fields required by GCC Core */

/**
 * Implements hook_field_info().
 */
function gcc_field_info() {

  return array(

    GCC_FIELD_TYPE_ENABLE => array(

      'label' => t('Group Enabled'),
      'description' => t('This field allow to transform an entity into a group.'),
      'default_widget' => 'gcc_enable_widget',
      'default_formatter' => 'gcc_enable_formatter',
      'no_ui' => TRUE,
    ),
    GCC_FIELD_TYPE_AUDIENCE => array(

      'label' => t('Groups'),
      'description' => t('This field associate an entity to one or more groups.'),
      'default_widget' => 'gcc_audience_widget',
      'default_formatter' => 'gcc_audience_formatter',
      'no_ui' => TRUE,
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function gcc_field_is_empty($item, $field) {

  switch ($field['type']) {

    case GCC_FIELD_TYPE_ENABLE:
      if (isset($item['enable_group']['value']) && $item['enable_group']['value'] == 1 && isset($item['enable_group']['uid']) && trim($item['enable_group']['uid']) != '') {
        return FALSE;
      }
      break;

    case GCC_FIELD_TYPE_AUDIENCE:
      if (isset($item)) {

        list($entity_type, $entity_id) = explode(':', $item) + array(NULL, NULL);
        if (is_string($entity_type) && is_numeric($entity_id)) {
          return FALSE;
        }
      }
      break;
  }

  return TRUE;
}

/**
 * Implements hook_field_presave().
 */
function gcc_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {

  switch ($field['type']) {

    case GCC_FIELD_TYPE_ENABLE:
      foreach ($items as &$item) {

        if (isset($item['enable_group']['value'])) {

          $uid = 0;
          $value = $item['enable_group']['value'];

          if ($value) {

            $user = user_load_by_name($item['enable_group']['uid']);
            if ($user) {
              $uid = $user->uid;
            }
            else {
              $value = 0;
            }
          }

          $item = array(

            'value' => $value,
            'uid' => $uid,
          );
        }
      }
      break;

    case GCC_FIELD_TYPE_AUDIENCE:
      foreach ($items as $delta => $item) {
        if (!is_array($item)) {

          list($entity_type, $entity_id) = explode(':', $item) + array(NULL, NULL);

          $items[$delta] = array();
          $items[$delta]['entity_type'] = $entity_type;
          $items[$delta]['entity_id'] = $entity_id;
        }
      }
      break;
  }
}

/**
 * Implements hook_field_validate().
 */
function gcc_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {

  switch ($field['type']) {

    case GCC_FIELD_TYPE_ENABLE:
      $count = 0;

      foreach ($items as $delta => $item) {
        if (!empty($item['enable_group']['value'])) {

          if (trim($item['enable_group']['uid']) == '') {

            $errors[$field['field_name']][$langcode][$delta][] = array(

              'error' => 'user_invalide',
              'message' => t('%field : please specify a user name', array('%field' => $instance['label'])),
            );
          }
          elseif (!user_load_by_name($item['enable_group']['uid'])) {

            $errors[$field['field_name']][$langcode][$delta][] = array(

              'error' => 'user_invalide',
              'message' => t('%field : %name is not a valid user', array('%field' => $instance['label'], '%name' => $item['enable_group']['uid'])),
            );
          }
          else {
            ++$count;
          }
        }
      }
      if ($instance['required'] && $count == 0) {
        $errors[$field['field_name']][$langcode][0][] = array(

          'error' => 'field_required',
          'message' => t('%field : the field is required', array('%field' => $instance['label'])),
        );
      }
      break;
  }
}

/**
 * Implements hook_field_widget_error().
 */
function gcc_field_widget_error($element, $error, $form, &$form_state) {

  switch ($error['error']) {

    case 'user_invalide':
      $error_element = $element['enable_group'][$element['#columns'][1]];
      break;

    default:
      $error_element = $element;
  }

  form_error($error_element, $error['message']);
}

/**
 * Default value function for the GCC_FIELD_ENABLE.
 */
function gcc_field_gcc_enable_default_value($entity_type, $entity, $field, $instance, $langcode) {

  global $user;

  $uid = 0;
  switch ($entity_type) {

    case 'node':
      $uid = $entity->uid;
      break;

    default:
      $uid = $user->uid;
      break;
  }

  return array(
    array(
      'value' => $instance['required'] ? 1 : 0,
      'uid' => $uid,
    ),
  );
}

/**
 * Implements hook_field_instance_settings_form().
 */
function gcc_field_instance_settings_form($field, $instance) {

  $form = array();
  $settings = $instance['settings'];

  switch ($field['type']) {

    case GCC_FIELD_TYPE_AUDIENCE:
      $options = array();

      $bundles = gcc_features_get_enabled_bundles('group');
      foreach ($bundles as $entity_type => $list) {
        foreach ($list as $bundle => $label) {
          $options[$entity_type . ':' . $bundle] = $label;
        }
      }

      $form['bundles'] = array(

        '#type' => 'checkboxes',
        '#title' => t('Which bundles can be used as audience'),
        '#options' => $options,
        '#default_value' => isset($settings['bundles']) ? $settings['bundles'] : array(),
        '#description' => t('If no bundle is selected, they will all be available'),
      );
      break;
  }

  return $form;
}

/* Widgets */

/**
 * Implements hook_field_widget_info().
 */
function gcc_field_widget_info() {

  return array(

    'gcc_enable_widget' => array(

      'label' => t('Checkbox'),
      'field types' => array(GCC_FIELD_TYPE_ENABLE),
    ),
    'gcc_audience_widget' => array(

      'label' => t('Select List'),
      'field types' => array(GCC_FIELD_TYPE_AUDIENCE),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function gcc_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  global $user;

  switch ($instance['widget']['type']) {

    case 'gcc_enable_widget':
      $account = isset($items[$delta]['uid']) ? user_load($items[$delta]['uid']) : NULL;

      $element['enable_group'] = array(

        '#type' => 'fieldset',
        '#title' => check_plain($element['#title']),
      );

      $element['enable_group']['value'] = array(

        '#type' => 'checkbox',
        '#default_value' => isset($items[$delta]['value']) ? $items[$delta]['value'] : FALSE,
        '#title' => t('Enable'),
        '#required' => $instance['required'] && $delta == 0,
      );

      $element['enable_group']['uid'] = array(

        '#type' => 'textfield',
        '#title' => t('Group Owner'),
        '#maxlength' => 60,
        '#autocomplete_path' => 'user/autocomplete',
        '#default_value' => isset($account->name) ? $account->name : '',
        '#weight' => 3,
      );

      break;

    case 'gcc_audience_widget':
      if (user_access('administer all groups')) {
        $list = gcc_group_get_list();
      }
      else {
        $list = gcc_membership_get_user_memberships($user->uid);
      }

      $context = array(

        'field' => $field,
        'instance' => $instance,
        'langcode' => $langcode,
        'items' => $items,
        'delta' => $delta,
      );
      drupal_alter('gcc_audience_allowed_value', $list, $context);

      $bundles_restriction = array();
      if (isset($instance['settings']['bundles'])) {
        $bundles_restriction = array_filter($instance['settings']['bundles']);
      }

      $options = array();
      foreach ($list as $group) {

        $entity = entity_load($group->entity_type, array($group->entity_id));
        $entity = reset($entity);

        if ($entity) {

          list(,,$bundle) = entity_extract_ids($group->entity_type, $entity);

          // Add only group of the correct entity type and bundle.
          if (empty($bundles_restriction) || isset($bundles_restriction[$group->entity_type . ':' . $bundle])) {
            $options[$group->entity_type . ':' . $group->entity_id] = entity_label($group->entity_type, $entity);
          }
        }
      }

      $default_value = array();
      foreach ($items as $item) {
        if (isset($item['entity_type'], $item['entity_id'])) {
          $default_value[] = $item['entity_type'] . ':' . $item['entity_id'];
        }
      }

      // Allow to pass default value by URL.
      if (empty($default_value) && isset($_GET['gcc_audience'])) {
        $default_value = $_GET['gcc_audience'];
      }

      $element += array(

        '#type' => 'select',
        '#options' => $options,
        '#default_value' => $default_value,
        '#multiple' => TRUE,
        '#size' => min(15, max(count($list), 5)),
      );
      break;
  }

  return $element;
}

/* Formatters */

/**
 * Implements hook_field_formatter_info().
 */
function gcc_field_formatter_info() {

  return array(

    'gcc_enable_formatter' => array(

      'label' => t('Default'),
      'field types' => array(GCC_FIELD_TYPE_ENABLE),
    ),
    'gcc_audience_formatter' => array(

      'label' => t('Default'),
      'field types' => array(GCC_FIELD_TYPE_AUDIENCE),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function gcc_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  $element = array();

  switch ($display['type']) {

    case 'gcc_enable_formatter':
      break;

    case 'gcc_audience_formatter':
      break;
  }

  return $element;
}
