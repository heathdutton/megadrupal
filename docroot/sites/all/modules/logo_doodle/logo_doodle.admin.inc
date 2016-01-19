<?php
/**
 * @file
 * Code for the Logo Doodle admin configuration form.
 */

/**
 * Admin configuration form to set the birthday doodle.
 */
function logo_doodle_configure_form() {
  $form = array();

  $form['birthdays'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Birthday Settings'),
    '#description' => 'The selected logo will be shown if the value of the selected field for the currently logged in user equals "today"',
  );
  if (module_exists('birthdays')) {
    if (count(_birthdays_field_instances()) > 0) {
      $form['birthdays']['birthday-logo'] = array(
        '#type' => 'managed_file',
        '#title' => t('Birthday Logo'),
        '#required' => TRUE,
        '#default_value' => variable_get('logo_doodle_birthday_filename'),
        '#upload_location' => 'public://',
      );
      $form['birthdays']['birthday-field'] = array(
        '#type' => 'select',
        '#title' => t('User Birthday field'),
        '#options' => logo_doodle_configure_select_helper(_birthdays_field_instances()),
        '#required' => TRUE,
        '#default_value' => variable_get('logo_doodle_birthday_field'),
      );
    }
    else {
      $form['birthdays']['#description'] = t('You have not created any Birthday fields for the user. Please do so at !link.', array('!link' => l(t('here'), 'admin/config/people/accounts/fields')));
    }
  }
  else {
    $form['birthdays']['#description'] = t('Enable the !link to configure this functionality.', array('!link' => l(t('Birthdays module'), 'http://drupal.org/project/birthdays')));
  }

  $form['logo-doodle-image-style'] = array(
    '#type' => 'select',
    '#title' => t('Image style'),
    '#options' => image_style_options(),
    '#required' => TRUE,
    '#default_value' => variable_get('logo_doodle_image_style'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['#submit'] = array('logo_doodle_configure_form_submit');
  return $form;
}

/**
 * Helper function to generate a list of options for the drop-down.
 *
 * @param array $list
 *   An array of 'Birthday' fields.
 *
 * @return array
 *   Drop-down options for use in the admin form.
 */
function logo_doodle_configure_select_helper($list) {
  $options = array();
  foreach ($list as $item) {
    $options[$item['field_name']] = $item['label'];
  }
  return $options;
}

/**
 * Submit handler for the admin configuration form.
 *
 * @param array $form
 *   The drupal form definition.
 * @param array $form_state
 *   The values of the submitted form.
 */
function logo_doodle_configure_form_submit($form, $form_state) {
  variable_set('logo_doodle_image_style', $form_state['values']['logo-doodle-image-style']);
  variable_set('logo_doodle_birthday_filename', $form_state['values']['birthday-logo']);
  variable_set('logo_doodle_birthday_field', $form_state['values']['birthday-field']);
}
