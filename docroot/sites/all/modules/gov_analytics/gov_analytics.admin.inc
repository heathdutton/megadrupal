<?php

/**
 * Implements hook_form().
 * Admin form to configurable welcome message
 */
function gov_analytics_form($form, &$form_state) {
  $form['gov_analytics_agency'] = array(
  '#type' => 'textfield',
  '#title' => t('Agency'),
  '#required' => TRUE,
  '#default_value' => variable_get('gov_analytics_agency', 'unspecified:domain.com'),
    '#description' => t('Required. Agency Custom dimension value.'),
  );
  $form['gov_analytics_subagency'] = array(
  '#type' => 'textfield',
  '#title' => t('Subagency'),
  '#required' => FALSE,
  '#default_value' => variable_get('gov_analytics_subagency','unspecified:domain.com -domain.com'),
    '#description' => t('Optional, recommended. Sub Agency Custom dimension value.'),
  );
  $form['gov_analytics_ver'] = array(
  '#type' => 'checkbox',
  '#title' => t('Ver'),
  '#required' => FALSE,
  '#default_value' => variable_get('gov_analytics_ver',1),
    '#description' => t('Optional, default on. Version control; used for diagnostics and testing.'),
  );
  $form['gov_analytics_sp'] = array(
  '#type' => 'textfield',
  '#title' => t('Search Parameter'),
  '#required' => FALSE,
  '#default_value' => variable_get('gov_analytics_sp'),
    '#description' => t('Optional. Additional search parameters.'),
  );
  $form['gov_analytics_exts'] = array(
  '#type' => 'textfield',
  '#title' => t('Extensions'),
  '#required' => FALSE,
  '#default_value' => variable_get('gov_analytics_exts'),
    '#description' => t('Optional. Additional download extensions.'),
  );
  $form['gov_analytics_yt'] = array(
  '#type' => 'checkbox',
  '#title' => t('Youtube'),
  '#required' => FALSE,
  '#default_value' => variable_get('gov_analytics_yt',1),
    '#description' => t('Optional, default on. Enable/Disable YoutTube Tracker.'),
  );
  $form['gov_analytics_sdor'] = array(
  '#type' => 'checkbox',
  '#title' => t('Sdor'),
  '#required' => FALSE,
  '#default_value' => variable_get('gov_analytics_sdor',1),
    '#description' => t('Optional, default on. If true, cookie will be set as subdomain.domain.com.'),
  );
  $form['gov_analytics_dclink'] = array(
  '#type' => 'checkbox',
  '#title' => t('Dclink'),
  '#required' => FALSE,
  '#default_value' => variable_get('gov_analytics_dclink',0),
    '#description' => t('Optional, default off. Demographic Data (true/false).'),
  );
  $form['gov_analytics_pua'] = array(
  '#type' => 'textfield',
  '#title' => t('Parallel UA ID'),
  '#required' => FALSE,
  '#default_value' => variable_get('gov_analytics_pua'),
    '#description' => t('Optional. Google Analytics Parallel UA Tracking ID.'),
  );
  $form['gov_analytics_enhlink'] = array(
  '#type' => 'checkbox',
  '#title' => t('Enhlink'),
  '#required' => FALSE,
  '#default_value' => variable_get('gov_analytics_enhlink',0),
    '#description' => t('Optional, default off. Enhanced Link Attribution (true/false).'),
  );
  $form['gov_analytics_autotracker'] = array(
  '#type' => 'checkbox',
  '#title' => t('Autotracker'),
  '#required' => FALSE,
  '#default_value' => variable_get('gov_analytics_autotracker',1),
    '#description' => t('Optional, default on. Enable/Disable AutoTracker.'),
  );
  $form['gov_analytics_optout'] = array(
  '#type' => 'checkbox',
  '#title' => t('Optout'),
  '#required' => FALSE,
  '#default_value' => variable_get('gov_analytics_optout',0),
    '#description' => t('Optional, default off. Page Opt-Out (true/false).'),
  );
  $form['gov_analytics_maincd'] = array(
  '#type' => 'checkbox',
  '#title' => t('Maincd'),
  '#required' => FALSE,
  '#default_value' => variable_get('gov_analytics_maincd',1),
    '#description' => t('Optional, default on. Send Custom Dimensions - DAP Tracker (true/false)'),
  );
  $form['gov_analytics_cto'] = array(
  '#type' => 'textfield',
  '#title' => t('Cookie Timeout'),
  '#required' => FALSE,
  '#default_value' => variable_get('gov_analytics_cto','24'),
    '#description' => t('Optional, default 24 (months). Ga cookie expiration in months.'),
  );
  return system_settings_form($form);
}
