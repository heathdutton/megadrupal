<?php

/**
 * @file
 * Act-On administration forms.
 */

/**
 * Act-On administration page.
 */
function acton_admin_form($form, &$form_state) {
  // Act-On Configuration.
  $form['configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Act-On Configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configuration']['acton_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Act-On Post Url'),
    '#description' => t("The url of the Act-On software."),
    '#default_value' => variable_get('acton_url', ''),
    '#required' => TRUE,
    '#size' => 60,
  );

  $form['configuration']['acton_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable debug mode'),
    '#description' => t('Enabling debug mode will show which fields are being submitted to Act-On after a submit'),
    '#default_value' => variable_get('acton_debug', 0),
  );

  $form['configuration']['acton_capture_all_forms'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable all forms with Act-On'),
    '#description' => t('Enable Act-On for ALL forms on this site (it is best to only enable Act-On for the forms you need below).'),
    '#default_value' => variable_get('acton_capture_all_forms', 0),
  );

  //Act-On Enabled forms.
  $form['enabled_forms'] = array(
    '#type' => 'fieldset',
    '#title' => t('Act-On Enabled Forms'),
    '#description' => t("Check the boxes next to individual forms on which you'd like Act-On enabled."),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      // Hide this fieldset when all forms are captured.
      'invisible' => array(
        'input[name="acton_capture_all_forms"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Generic forms.
  $form['enabled_forms']['general_forms'] = array('#markup' => '<h5>' . t('General Forms') . '</h5>');
  // User register form.
  $form['enabled_forms']['acton_form_user_register_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('User Registration form'),
    '#default_value' => variable_get('acton_form_user_register_form', 0),
  );
  // User password form.
  $form['enabled_forms']['acton_form_user_pass'] = array(
    '#type' => 'checkbox',
    '#title' => t('User Password Reset form'),
    '#default_value' => variable_get('acton_form_user_pass', 0),
  );

  // If webform.module enabled, add webforms.
  if (module_exists('webform')) {
    $form['enabled_forms']['acton_form_webforms'] = array(
      '#type' => 'checkbox',
      '#title' => t('Webforms (all)'),
      '#default_value' => variable_get('acton_form_webforms', 0),
    );
  }

  // If contact.module enabled, add contact forms.
  if (module_exists('contact')) {
    $form['enabled_forms']['contact_forms'] = array('#markup' => '<h5>' . t('Contact Forms') . '</h5>');
    // Sitewide contact form.
    $form['enabled_forms']['acton_form_contact_site_form'] = array(
      '#type' => 'checkbox',
      '#title' => t('Sitewide Contact form'),
      '#default_value' => variable_get('acton_form_contact_site_form', 0),
    );
    // Sitewide personal form.
    $form['enabled_forms']['acton_form_contact_personal_form'] = array(
      '#type' => 'checkbox',
      '#title' => t('Personal Contact forms'),
      '#default_value' => variable_get('acton_form_contact_personal_form', 0),
    );
  }

  // If profile.module enabled, add profile forms.
  if (module_exists('profile')) {
    $form['enabled_forms']['profile_forms'] = array('#value' => '<h5>' . t('Profile Forms') . '</h5>');
    $form['enabled_forms']['acton_form_user_profile_form'] = array(
      '#type' => 'checkbox',
      '#title' => t('Profile forms (all)'),
      '#default_value' => variable_get('acton_form_user_profile_form', 0),
    );
  }

  // Get node types for node forms and node comment forms.
  $types = node_type_get_types();
  if (!empty($types)) {
    // Node forms.
    $form['enabled_forms']['node_forms'] = array('#markup' => '<h5>' . t('Node Forms') . '</h5>');
    foreach ($types as $type) {
      $id = 'acton_form_' . $type->type . '_node_form';
      $form['enabled_forms'][$id] = array(
        '#type' => 'checkbox',
        '#title' => t('@name node form', array('@name' => $type->name)),
        '#default_value' => variable_get($id, 0),
      );
    }

    // Comment forms.
    if (module_exists('comment')) {
      $form['enabled_forms']['comment_forms'] = array('#markup' => '<h5>' . t('Comment Forms') . '</h5>');
      foreach ($types as $type) {
        $id = 'acton_form_comment_node_' . $type->type . '_form';
        $form['enabled_forms'][$id] = array(
          '#type' => 'checkbox',
          '#title' => t('@name comment form', array('@name' => $type->name)),
          '#default_value' => variable_get($id, 0),
        );
      }
    }
  }
  

  return system_settings_form($form);
}
