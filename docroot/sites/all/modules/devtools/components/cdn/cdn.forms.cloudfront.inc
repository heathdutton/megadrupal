<?php

/**
 * Form callback for /admin/config/development/cdn/cloudfront;
 * Provides configuration settings for CloudFront CDN;
 * @return array Form API array.
 */
function devtools_cdn_admin_cloudfront_form($form, &$form_state) {
  $form = array();

  $form['#prefix'] = t('If you are using CloudFront as your CDN, enter the information below
                        to gain access to file invalidation functionality on the !touch page.',
                        array('!touch' => l(t('Touch'), 'admin/config/development/cdn/touch')));

  $form['devtools_cdn_aws_access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('AWS Access Key'),
    '#default_value' => variable_get('devtools_cdn_aws_access_key'),
  );

  $form['devtools_cdn_aws_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('AWS Secret Key'),
    '#default_value' => variable_get('devtools_cdn_aws_secret_key'),
  );

  $form['devtools_cdn_aws_distribution_id'] = array(
    '#type' => 'textfield',
    '#title' => t('AWS Distribution ID'),
    '#default_value' => variable_get('devtools_cdn_aws_distribution_id'),
  );

  return system_settings_form($form);
}