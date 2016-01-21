<?php

/**
 * @file
 * Defines and handles the fields required by the GCC Domain module.
 */

// Fields type defines.
define('GCC_DOMAIN_FIELD_TYPE_DOMAIN', 'gcc_domain');

// Fields name defines.
define('GCC_DOMAIN_FIELD_DOMAIN', 'field_gcc_domain');

/* Declaration and Management of the fields required by GCC Domain */

/**
 * Implements hook_field_info().
 */
function gcc_domain_field_info() {

  return array(

    GCC_DOMAIN_FIELD_TYPE_DOMAIN => array(

      'label' => t('Choose a domain'),
      'description' => t('This field allow to choose a domain for the group.'),
      'default_widget' => 'gcc_domain_widget',
      'default_formatter' => 'gcc_domain_hidden_formatter',
      'no_ui' => TRUE,
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function gcc_domain_field_is_empty($item, $field) {

  switch ($field['type']) {

    case GCC_DOMAIN_FIELD_TYPE_DOMAIN:
      if (!empty($item['domain'])) {
        return FALSE;
      }
      break;
  }

  return TRUE;
}

/**
 * Implements hook_field_validate().
 */
function gcc_domain_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {

  switch ($field['type']) {

    case GCC_DOMAIN_FIELD_TYPE_DOMAIN:
      $count = 0;

      foreach ($items as $delta => $item) {
        if (!empty($item['domain'])) {

          if (!preg_match('#^https?:\/\/(?:[a-z0-9\-\.])+(?::[0-9]+)?$#', $item['domain'])) {

            $errors[$field['field_name']][$langcode][$delta][] = array(

              'error' => 'domain_invalide',
              'message' => t('%field : %domain is not a valid domain', array('%field' => $instance['label'], '%domain' => $item['domain'])),
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
function gcc_domain_field_widget_error($element, $error, $form, &$form_state) {

  $error_element = $element['domain'];

  form_error($error_element, $error['message']);
}

/* Widgets */

/**
 * Implements hook_field_widget_info().
 */
function gcc_domain_field_widget_info() {

  return array(

    'gcc_domain_widget' => array(

      'label' => t('Domain widget'),
      'field types' => array(GCC_DOMAIN_FIELD_TYPE_DOMAIN),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function gcc_domain_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  global $base_url;

  switch ($instance['widget']['type']) {

    case 'gcc_domain_widget':
      $element['domain'] = array(

        '#title' => $element['#title'],
        '#type' => 'textfield',
        '#maxlength' => GCC_DOMAIN_MAX_LENGTH,
        '#required' => $element['#required'],
        '#description' => 'You must enter a single well formatted domain with no trailing slash. (eg. ' . $base_url . ')',
        '#default_value' => isset($items[$delta]['domain']) ? $items[$delta]['domain'] : '',
      );
      break;
  }

  return $element;
}

/* Formatters */

/**
 * Implements hook_field_formatter_info().
 */
function gcc_domain_field_formatter_info() {

  return array(

    'gcc_domain_hidden_formatter' => array(

      'label' => t('Hidden'),
      'field types' => array(GCC_DOMAIN_FIELD_TYPE_DOMAIN),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function gcc_domain_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  $element = array();

  switch ($display['type']) {

    case 'gcc_domain_hidden_formatter':
      break;
  }

  return $element;
}
