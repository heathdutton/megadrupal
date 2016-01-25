<?php

/**
 * @file
 * Module settings UI for the Contact Attach module.
 */

/**
 * Form constructor for the settings form.
 */
function contact_attach_admin_settings() {
  $form = array();
  $admin_rid = variable_get('user_admin_role', FALSE);
  $admin_role = $admin_rid ? user_role_load($admin_rid) : FALSE;

  $form['contact_attach_contact_forms'] = array(
    '#type' => 'value',
    '#value' => array('site', 'user'),
  );

  if (module_exists('file')) {
    $form['contact_attach_simple_field'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use simple file field.'),
      '#default_value' => variable_get('contact_attach_simple_field', 0),
      '#description' => t('If the file module is enabled a managed file field will be used for the file attachments. If a simple file field is wanted instead, check this checkbox.'),
    );
  }

  foreach ($form['contact_attach_contact_forms']['#value'] as $contact_form_short) {
    $contact_attach_numbers = variable_get('contact_attach_number_' . $contact_form_short, array());
    $contact_attach_extensions = variable_get('contact_attach_extensions_' . $contact_form_short, array());
    $contact_attach_uploadsizes = variable_get('contact_attach_uploadsize_' . $contact_form_short, array());

    // Make context sensitive translatable strings translatable.
    if ($contact_form_short === 'site') {
      $contact_form_title = t('Settings for the site-wide contact form');
      $contact_form_attach_number_description = t('The number of file attachments to allow on the site-wide contact form.');
      $contact_form_permission = 'attach files on site-wide contact form';
    }
    else {
      $contact_form_title = t('Settings for personal contact forms');
      $contact_form_attach_number_description = t('The number of file attachments to allow on personal contact forms.');
      $contact_form_permission = 'attach files on personal contact forms';
    }

    $form['settings_' . $contact_form_short . '_form'] = array(
      '#type' => 'fieldset',
      '#title' => $contact_form_title,
      '#collapsible' => FALSE,
    );

    // array_flip() so that the key is a string. This prevents array_merge()
    // from changing the key.
    $roles = array_flip(user_roles(FALSE, $contact_form_permission));
    // If the authenticated user role has the permission, add in all roles that
    // indirectly get the permission (all roles except the anonymous user role).
    $indirect_roles = in_array(DRUPAL_AUTHENTICATED_RID, $roles, TRUE) ? array_flip(user_roles(TRUE)) : array();
    // Also add administrator, who seldom has the permission explicitly set.
    $roles = array_merge($roles, $indirect_roles, $admin_role ? array($admin_role->name => $admin_role->rid) : array());

    $form['contact_attach_roles_' . $contact_form_short] = array(
      '#type' => 'value',
      '#value' => $roles,
    );

    foreach ($roles as $role => $rid) {
      $form['settings_' . $contact_form_short . '_form']['settings_role_' . $contact_form_short . '_' . $rid] = array(
        '#type' => 'fieldset',
        '#title' => t('Settings for the @role role', array('@role' => $role)),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['settings_' . $contact_form_short . '_form']['settings_role_' . $contact_form_short . '_' . $rid]['contact_attach_number_' . $contact_form_short] = array(
        '#tree' => TRUE,
      );
      $form['settings_' . $contact_form_short . '_form']['settings_role_' . $contact_form_short . '_' . $rid]['contact_attach_number_' . $contact_form_short][$rid] = array(
        '#type' => 'textfield',
        '#title' => t('Number of file attachments'),
        '#default_value' => !empty($contact_attach_numbers[$rid]) ? $contact_attach_numbers[$rid] : CONTACT_ATTACH_DEFAULT_NUMBER,
        '#size' => 4,
        '#description' => $contact_form_attach_number_description,
      );
      $form['settings_' . $contact_form_short . '_form']['settings_role_' . $contact_form_short . '_' . $rid]['contact_attach_extensions_' . $contact_form_short] = array(
        '#tree' => TRUE,
      );
      $form['settings_' . $contact_form_short . '_form']['settings_role_' . $contact_form_short . '_' . $rid]['contact_attach_extensions_' . $contact_form_short][$rid] = array(
        '#type' => 'textfield',
        '#title' => t('Permitted file extensions'),
        '#default_value' => !empty($contact_attach_extensions[$rid]) ? $contact_attach_extensions[$rid] : CONTACT_ATTACH_DEFAULT_EXTENSIONS,
        '#maxlength' => 255,
        '#description' => t('Extensions that users with this role can attach. Separate extensions with a space and do not include the leading dot.'),
      );
      $form['settings_' . $contact_form_short . '_form']['settings_role_' . $contact_form_short . '_' . $rid]['contact_attach_uploadsize_' . $contact_form_short] = array(
        '#tree' => TRUE,
      );
      $form['settings_' . $contact_form_short . '_form']['settings_role_' . $contact_form_short . '_' . $rid]['contact_attach_uploadsize_' . $contact_form_short][$rid] = array(
        '#type' => 'textfield',
        '#title' => t('Maximum file size per attachment'),
        '#default_value' => !empty($contact_attach_uploadsizes[$rid]) ? $contact_attach_uploadsizes[$rid] : CONTACT_ATTACH_DEFAULT_UPLOADSIZE,
        '#size' => 5,
        '#maxlength' => 12,
        '#description' => t('The maximum size of a file a user with this role can attach.'),
        '#field_suffix' => t('MB'),
      );
    }
  }

  $form['#validate'] = array('contact_attach_admin_settings_validate');

  return system_settings_form($form);
}

/**
 * Form validation handler for contact_attach_admin_settings().
 */
function contact_attach_admin_settings_validate($form, &$form_state) {
  $exceed_max_msg = t('Your PHP settings limit the maximum file size per upload to %size.', array('%size' => format_size(file_upload_max_size()))) . '<br/>';
  $more_info = t("Depending on your server environment, these settings may be changed in the system-wide php.ini file, a php.ini file in your Drupal root directory, in your Drupal site's settings.php file, or in the .htaccess file in your Drupal root directory.");

  foreach ($form_state['values']['contact_attach_contact_forms'] as $contact_form_short) {
    foreach ($form_state['values']['contact_attach_roles_' . $contact_form_short] as $role => $rid) {
      $uploadsize = $form_state['values']['contact_attach_uploadsize_' . $contact_form_short][$rid];

      if (!ctype_digit($form_state['values']['contact_attach_number_' . $contact_form_short][$rid])) {
        form_set_error('contact_attach_number_' . $contact_form_short . '][' . $rid, t('The number of file attachments for the %role role must be a positive integer.', array('%role' => $role)));
      }
      elseif ($form_state['values']['contact_attach_number_' . $contact_form_short][$rid] === '0') {
        form_set_error('contact_attach_number_' . $contact_form_short . '][' . $rid, t('The number of file attachments for the %role role cannot be 0. If you want to disable the ability of attaching files on contact forms for a role, revoke its permission on the <a href="@permissions">permissions</a> page.', array('%role' => $role, '@permissions' => url('admin/people/permissions'))));
      }

      if (!is_numeric($uploadsize) || ($uploadsize <= 0)) {
        form_set_error('contact_attach_uploadsize_' . $contact_form_short . '][' . $rid, t('The %role role file size limit must be a number and greater than zero.', array('%role' => $role)));
      }
      if ($uploadsize * 1024 * 1024 > file_upload_max_size()) {
        form_set_error('contact_attach_uploadsize_' . $contact_form_short . '][' . $rid, $exceed_max_msg . $more_info);
        $more_info = '';
      }
    }
    // Unset messenger value for roles, so that it is not saved as a variable.
    unset($form_state['values']['contact_attach_roles_' . $contact_form_short]);
  }
  // Unset contact form messenger value, so that it is not saved as a variable.
  unset($form_state['values']['contact_attach_contact_forms']);
}
