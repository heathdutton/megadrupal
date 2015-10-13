<?php

/**
 * @file
 * File: Domain_registration admin.
 *
 * Restricts registration to a particular email domain.
 */

/**
 * Implements hook_form().
 */
function email_domain_restriction_admin_form($form, &$form_state) {
  $form['email_domain_restriction_list'] = array(
    '#type' => 'textarea',
    '#required' => TRUE,
    '#title' => t('Email domains'),
    '#default_value' => variable_get('email_domain_restriction_list', 'example.com'),
    '#description' => t('Enter the domains you wish to restrict. One domain per line:') . '<br />' . t('example.com') .  '<br />' .  t('example2.com'),
  );
  $form['email_domain_restriction_message'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Error message'),
    '#default_value' => variable_get('email_domain_restriction_message', t('Email domain not valid')),
    '#description' => t('Enter the error message you want the user to see if the email address does not validate.'),
  );
  $form['email_domain_restriction_behaviour'] = array(
    '#type' => 'radios',
    '#required' => TRUE,
    '#title' => 'Select the action to perform on the domains listed above',
    '#description' => '<b>' . t('Caution:') . '</b> <br />' . t('If you deny the above list all other domains will be allowed.') . '</br />' . t('If you allow the list above, all other domains will be refused'),
    '#options' => array(
      0 => t('Deny'),
      1 => t('Allow'),
    ),
    '#default_value' => variable_get('email_domain_restriction_behaviour', 0),
  );
  $form['email_domain_restriction_current_field'] = array(
    '#type' => 'fieldset',
    '#title' => t('Apply validation to the existing emails'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Apply validation to existing email fields. At the moment it apply only on
     user account email.'),
  );
  $form['email_domain_restriction_current_field']['email_domain_restriction_apply_current_fields'] = array(
    '#type' => 'radios',
    '#required' => TRUE,
    '#title' => 'Apply',
    '#description' => '<b>' . t('Caution:') . '</b> <br />' . t('If you apply to the existing emails an user will not be able to update his profile if he/she has an invalid email.'),
    '#options' => array(
      0 => t('Do not apply to the existing emails'),
      1 => t('Apply to the current emails'),
    ),
    '#default_value' => variable_get('email_domain_restriction_apply_current_fields', 0),
  );
  $form['email_domain_restriction_current_field']['email_domain_restriction_message_to_change'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('Message to show to the user if the current user email domain is not valid'),
    '#default_value' => variable_get('email_domain_restriction_message_to_change', t('Please, change your email')),
    '#description' => t('Enter the error message you want the user to see if the current user email domain is not
       valid.'),
  );

  return system_settings_form($form);
}
