<?php
/**
 * @file
 * Commerce profile2 field.
 */

/**
 * Implements hook_field_info().
 */
function commerce_profile2_field_info() {
  return array(
    'commerce_profile2' => array(
      'label' => t('Commerce profile2'),
      'description' => t('Attaches profiles to commerce products.'),
      'default_widget' => 'commerce_profile2_commerce_profile2_form',
      'default_formatter' => 'commerce_profile2_default',
    ),
  );
}

/**
 * Implements hook_field_widget_info().
 */
function commerce_profile2_field_widget_info() {
  return array(
    'commerce_profile2_commerce_profile2_form' => array(
      'label' => t('Commerce profile2 form'),
      'field types' => array('commerce_profile2'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        // Use FIELD_BEHAVIOR_NONE for no default value.
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_info().
 */
function commerce_profile2_field_formatter_info() {
  return array(
    'commerce_profile2_default' => array(
      'label' => t('Default'),
      'field types' => array('commerce_profile2'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function commerce_profile2_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {
    case 'commerce_profile2_default':
      foreach ($items as $delta => $item) {
        $element[$delta]['#markup'] = theme('commerce_profile2_formatter_default', $item);
      }
      break;
  }

  return $element;
}

/**
 * Implements hook_field_is_empty().
 */
function commerce_profile2_field_is_empty($item, $field) {
  if ($field['type'] == 'commerce_profile2') {
    if (empty($item['profile2_type']) && empty($item['user_role']) && empty($item['created_email'])) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Implements hook_field_settings_form().
 */
function commerce_profile2_field_settings_form($field, $instance, $has_data) {
  if ($field['type'] == 'commerce_profile2') {
    $settings = $field['settings'];
    $form = array();
    return $form;
  }
}

/**
 * Implements hook_field_validate().
 */
function commerce_profile2_field_validate($obj_type, $object, $field, $instance, $langcode, &$items, &$errors) {
  if ($field['type'] == 'commerce_profile2') {

  }
}


/**
 * Implements hook_field_widget_form().
 */
function commerce_profile2_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $element += array(
    '#type' => 'fieldset',
  );
  if ($instance['widget']['type'] == 'commerce_profile2_commerce_profile2_form') {
    $widget = $instance['widget'];
    $settings = $widget['settings'];

    $element['profile2_type'] = array(
      '#type' => 'select',
      '#title' => t('Profile2 type'),
      '#description' => t('The profile type form to fill out on checkout.'),
      '#options' => drupal_map_assoc(array_keys(profile2_get_types())),
      '#default_value' => isset($items[$delta]['profile2_type']) ? $items[$delta]['profile2_type'] : NULL,
    );
    $element['user_role'] = array(
      '#type' => 'select',
      '#title' => t('User role'),
      '#description' => t('The role to set for the user attached to this profile.'),
      '#options' => user_roles(),
      '#default_value' => isset($items[$delta]['user_role']) ? $items[$delta]['user_role'] : NULL,
    );
    $element['created_email'] = array(
      '#type' => 'select',
      '#title' => t('E-mail message'),
      '#description' => t('The email to send the user on checkout complete.'),
      '#options' => array(
        'register_admin_created' => 'Welcome message for user created by the admin',
        'register_no_approval_required' => 'Welcome message when user self-registers',
        'register_pending_approval' => 'Welcome message, user pending admin approval',
        'password_reset' => 'Password recovery request',
        'status_activated' => 'Account activated',
        'status_blocked' => 'Account blocked',
        'cancel_confirm' => 'Account cancellation request',
        'status_canceled' => 'Account canceled',
      ),
      '#default_value' => isset($items[$delta]['created_email']) ? $items[$delta]['created_email'] : 'register_admin_created',
    );

  }
  return $element;
}

/**
 * Implements hook_theme.
 */
function commerce_profile2_theme($existing, $type, $theme, $path) {
  return array(
    'commerce_profile2_formatter_default' => array(
      'variables' => array('item' => NULL),
    ),
  );
}

/**
 * Default formatter.
 */
function theme_commerce_profile2_formatter_default($item) {
  $output = '';
  $output .= '<div class="commerce_profile2-wrapper">';
  $output .= 'Profile2 type: ' . $item['profile2_type'] . "<br />";
  $output .= 'User role: ' . $item['user_role'] . "<br />";
  $output .= 'Created email: ' . $item['created_email'] . "<br />";
  $output .= '</div><br />';

  return $output;
}
