<?php

/**
 * @file
 *   Admin section file
 */

/**
 * Form builder for admin settings
 */
function views_s3_admin_form($form, $form_state) {
  $form = array();
  // $form['credentials'] = array(
  //     '#type' => 'fieldset',
  //     '#title' => t('S3 credentials'),
  //     '#collapsible' => FALSE,
  //     '#disabled'
  //   );
  //   $form['credentials']['views_s3_access_key'] = array(
  //     '#title' => t('Access Key ID'),
  //     '#description' => t('You can add keys to your account in the !link.', array('!link' => l(t('AWS credentials page'), 'https://portal.aws.amazon.com/gp/aws/securityCredentials'))),
  //     '#type' => 'textfield',
  //     '#default_value' => variable_get('aws_key', ''),
  //   );
  //   $description = '';
  //   $secret = variable_get('aws_secret', '');
  //   if (!empty($secret)) {
  //     $description = t('IMPORTANT: You aready have a secret key stored, if you submit this form with this field blank your key will be lost. ');
  //   }
  //   $form['credentials']['views_s3_secret_key'] = array(
  //     '#title' => t('Secret Access Key'),
  //     '#description' => $description . t('For your protection, you should never share your secret access keys with anyone. In addition, industry best practice recommends frequent key rotation.'),
  //     '#type' => 'password',
  //     '#default_value' => '',
  //   );
  $form['configure_warning'] = array(
    '#markup' => '<p>' . l(t('Refer to the AWS SDK configuration page to set your credentials'), 'admin/config/media/awssdk') .'</p>',
  );
  $form['views_s3_debug_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug mode'),
    '#description' => t('Check this if you want extra debug information.'),
    '#default_value' => variable_get('views_s3_debug_mode', S3APIRequest::VIEWS_S3_DEFAULT_DEBUG_MODE),
  );
  return system_settings_form($form);
}
