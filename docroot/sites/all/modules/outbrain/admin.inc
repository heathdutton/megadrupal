<?php

/**
 * @file
 * Contains the administrative functions of the outbrain integration module.
 *
 * This file is included by the core outbrain integration module, and includes the
 * settings form.
 */

/**
 * Implements hook_admin_settings_form().
 * Used to create the admin form to configure the outbrain blocks.
 */
function outbrain_integration_admin_settings_form() {
  $form = array();

  $form['outbrain_integration_enabled'] = array(
      '#type' => 'radios',
      '#title' => t('Outbrain Integration Enabled?'),
      '#options' => array(
          TRUE => t('Yes'),
          FALSE => t('No'),
      ),
      '#default_value' => variable_get('outbrain_integration_enabled', FALSE),
  );

  $form['outbrain_integration_internal_block_subject'] = array(
      '#type' => 'textfield',
      '#title' => t('Internal Ads Block Title'),
      '#default_value' => variable_get('outbrain_integration_internal_block_subject', ''),
      '#description' => 'The title you want to be displayed on the block. Use &lt;none&gt; if no title is desired.',
  );

  $form['outbrain_integration_internal_data_widget_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Internal Ads Data Widget ID'),
      '#default_value' => variable_get('outbrain_integration_internal_data_widget_id', ''),
      '#description' => 'Example: AR_1',
  );

  $form['outbrain_integration_internal_data_ob_template'] = array(
      '#type' => 'textfield',
      '#title' => t('Internal Ads Data OB Template'),
      '#default_value' => variable_get('outbrain_integration_internal_data_ob_template', ''),
      '#description' => 'Example: CUSTOM_TEMPLATE',
  );

  $form['outbrain_integration_external_block_subject'] = array(
      '#type' => 'textfield',
      '#title' => t('External Ads Block Title'),
      '#default_value' => variable_get('outbrain_integration_external_block_subject', ''),
      '#description' => 'The title you want to be displayed on the block. Use &lt;none&gt; if no title is desired.',
  );

  $form['outbrain_integration_external_data_widget_id'] = array(
      '#type' => 'textfield',
      '#title' => t('External Ads Data Widget ID'),
      '#default_value' => variable_get('outbrain_integration_external_data_widget_id', ''),
      '#description' => 'Example: AR_1',
  );

  $form['outbrain_integration_external_data_ob_template'] = array(
      '#type' => 'textfield',
      '#title' => t('External Ads Data OB Template'),
      '#default_value' => variable_get('outbrain_integration_external_data_ob_template', ''),
      '#description' => 'Example: CUSTOM_TEMPLATE',
  );

  return system_settings_form($form);
}