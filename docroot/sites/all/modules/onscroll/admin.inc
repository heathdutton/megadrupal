<?php

/**
 * @file
 * Contains the administrative functions of the OnScroll module.
 */

/**
 * Implements hook_admin_settings_form().
 * Used to create the admin form to configure the Meetrics.
 */
function onscroll_admin_settings_form() {
  $form = array();

  $form['global'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global Settings'),
  );

  $form['global']['onscroll_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('OnScroll Enabled'),
    '#default_value' => _onscroll_variable_get('onscroll_enabled'),
  );

  $form['global']['onscroll_url'] = array(
    '#type' => 'textfield',
    '#title' => t('OnScroll URL'),
    '#default_value' => _onscroll_variable_get('onscroll_url'),
  );

  $dfp_description = '';
  $options =  array('custom' => t('Custom'));
  if (module_exists('dfp')) {
    $dfp_description = ' ' . t('Or you can choose one of the DFP slots.');
    $tags = dfp_tag_load_all();
    foreach ($tags as $tag) {
      $options[$tag->machinename] = $tag->slot;
    }
  }
  $form['global']['onscroll_ad_slot'] = array(
    '#type' => 'select',
    '#title' => t('Ad slot'),
    '#options' => $options,
    '#default_value' => _onscroll_variable_get('onscroll_ad_slot'),
    '#description' => t('You can paste the Ad code here.') . $dfp_description,
  );

  $form['global']['onscroll_code'] = array(
    '#type' => 'textarea',
    '#title' => t('OnScroll Code'),
    '#description' => t('Paste the Ad code here.'),
    '#default_value' => _onscroll_variable_get('onscroll_code'),
    '#states' => array(
      'visible' => array('select[name="onscroll_ad_slot"]' => array('value' => 'custom')),
    ),
  );

  $form['global']['onscroll_campaign_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Campaign Id'),
    '#size' => '25',
    '#default_value' => _onscroll_variable_get('onscroll_campaign_id'),
  );

  $form['global']['onscroll_creative_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Creative Id'),
    '#size' => '25',
    '#default_value' => _onscroll_variable_get('onscroll_creative_id'),
  );

  $form['global']['onscroll_account_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Account Id'),
    '#size' => '25',
    '#default_value' => _onscroll_variable_get('onscroll_account_id'),
  );

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('OnScroll Options'),
  );

  $form['options']['onscroll_mode'] = array(
    '#type' => 'select',
    '#title' => t('Mode'),
    '#options' => array(
      'basic' => t('Basic'),
      'sticky' => t('Sticky'),
      'sticky_reappearing' => t('Sticky Reappearing'),
      'sticky_dual' => t('Sticky Dual'),
    ),
    '#default_value' => _onscroll_variable_get('onscroll_mode'),
  );

  $form['options']['onscroll_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Interval'),
    '#maxlength' => 4,
    '#size' => 4,
    '#default_value' => _onscroll_variable_get('onscroll_interval'),
  );

  $form['options']['onscroll_reload'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reload'),
    '#default_value' => _onscroll_variable_get('onscroll_reload'),
  );

  $form['dimensions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dimensions'),
  );

  $form['dimensions']['onscroll_margin'] = array(
    '#type' => 'textfield',
    '#title' => t('Margin'),
    '#maxlength' => 4,
    '#size' => 4,
    '#default_value' => _onscroll_variable_get('onscroll_margin'),
  );

  $form['dimensions']['onscroll_margin_top'] = array(
    '#type' => 'textfield',
    '#title' => t('Top margin'),
    '#maxlength' => 4,
    '#size' => 4,
    '#default_value' => _onscroll_variable_get('onscroll_margin_top'),
  );

  $form['dimensions']['onscroll_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height (Pixels)'),
    '#maxlength' => 4,
    '#size' => 4,
    '#default_value' => _onscroll_variable_get('onscroll_height'),
  );

  $form['dimensions']['onscroll_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width (Pixels)'),
    '#maxlength' => 4,
    '#size' => 4,
    '#default_value' => _onscroll_variable_get('onscroll_width'),
  );

  return system_settings_form($form);
}
