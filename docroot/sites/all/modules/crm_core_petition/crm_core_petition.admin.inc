<?php

/**
 * @file
 * Administration pages of CRM Core Petition module.
 */

/**
 * Settings page form builder.
 *
 * @see crm_core_petition_settings_form_validate()
 * @see crm_core_petition_settings_form_submit()
 */
function crm_core_petition_settings_form($form, &$form_state) {

  $form['crm_core_petition_disable_goal_is_met'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable petitions once goal is met?'),
    '#description' => t('Check this box to disable petitions once a goal is met. Disabling a petition keeps it visible in the system, but users will no longer be able to sign it.'),
    '#default_value' => variable_get('crm_core_petition_disable_goal_is_met', FALSE),
  );

  $form['crm_core_petition_disabled_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Disabled petition message'),
    '#description' => t('Enter text to display to users when a petition is disabled. This will appear in place of a form for signing the petition.'),
    '#default_value' => variable_get('crm_core_petition_disabled_message', t('We have met our goal!')),
    '#states' => array(
      'visible' => array(
        ':input[name="crm_core_petition_disable_goal_is_met"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email options'),
  );

  $description = 'Configure the default email to be sent when users sign a'
    . ' petition, or when someone is targeted by a petition. These settings can'
    . ' be overridden when setting up petition pages.';
  $form['email']['description_wrapper'] = array(
    '#type' => 'container',
  );
  $form['email']['description_wrapper']['description'] = array(
    '#markup' => t($description),
  );

  $rule_set_tag = CMCP_SIGN_EMAIL_TAG;
  $rules_components = _crm_core_petition_load_rules_components_by_tag($rule_set_tag);
  $form['email']['crm_core_petition_default_sign_email'] = array(
    '#type' => 'select',
    '#title' => t('Default Target Message'),
    '#empty_option' => t('--Please Select--'),
    '#empty_value' => '',
    '#options' => $rules_components,
    '#default_value' => variable_get('crm_core_petition_default_sign_email', ''),
  );

  $rule_set_tag = CMCP_THANKS_EMAIL_TAG;
  $rules_components = _crm_core_petition_load_rules_components_by_tag($rule_set_tag);
  $form['email']['crm_core_petition_default_thanks_email'] = array(
    '#type' => 'select',
    '#title' => t('Default Thank You Message'),
    '#empty_option' => t('--Please Select--'),
    '#empty_value' => '',
    '#options' => $rules_components,
    '#default_value' => variable_get('crm_core_petition_default_thanks_email', ''),
  );

  return system_settings_form($form);
}

/**
 * Basic settings form builder.
 */
function crm_core_petition_admin_general_settings_form($form, &$form_state) {

  return system_settings_form($form);
}

/**
 * Report settings form builder.
 */
function crm_core_petition_admin_report_settings_form($form, &$form_state) {

  // display settings
  $form['display_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#weight' => 0,
    '#collapsible' => FALSE,
  );

  // default height for charts in CRM Core Event
  $form['display_settings']['crm_core_petition_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Default height'),
    '#description' => t('Please enter the default height for charts used in CRM Core Petition, as an integer.'),
    '#default_value' => variable_get('crm_core_petition_height', CMCP_CHART_HEIGHT),
  );

  $form['display_settings']['crm_core_petition_include_legend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display legends by default'),
    '#description' => t('Check this box to include a legend in charts by default.'),
    '#default_value' => variable_get('crm_core_petition_include_legend', 0),
    '#return_value' => 1,
  );

  $form['display_settings']['crm_core_petition_include_tooltips'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display tooltips when hovering over points in a chart'),
    '#description' => t('Check this box to include tooltips in flot charts.'),
    '#default_value' => variable_get('crm_core_petition_include_tooltips', 0),
    '#return_value' => 1,
  );

  // settings for line charts
  $form['display_settings']['default_colors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Colors'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // default line color 
  $form['display_settings']['default_colors']['crm_core_petition_line_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Line color'),
    '#description' => t('Used on line charts. Please enter the color to use for the line in valid #RRGGBB or rgba format. Leave blank to use the default color.'),
    '#default_value' => variable_get('crm_core_petition_line_color', ''),
  );

  // default chart color 
  $form['display_settings']['default_colors']['crm_core_petition_chart_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Chart color'),
    '#description' => t('Used for most charts. Please enter the background color to use for the chart iteself in valid #RRGGBB or rgba format. Leave blank to use the default color.'),
    '#default_value' => variable_get('crm_core_petition_chart_color', ''),
  );

  // default series background color
  $form['display_settings']['default_colors']['crm_core_petition_background_color'] = array(
    '#type' => 'textarea',
    '#title' => t('Background color'),
    '#description' => t('Used for line charts. Please enter the color to use for the background of the series in valid #RRGGBB or rgba format. Leave blank to use the default color. You can create gradients by separating each value with a new line.'),
    '#default_value' => variable_get('crm_core_petition_background_color', ''),
  );

  // default series background color
  $form['display_settings']['default_colors']['crm_core_petition_series_colors'] = array(
    '#type' => 'textarea',
    '#title' => t('Series colors'),
    '#description' => t('Used for pie and bar charts. Please enter the default color series to use when displaying data, in valid #RRGGBB or rgba format. Please separate each value with a new line. Leave blank to use the default color scheme.'),
    '#default_value' => variable_get('crm_core_petition_series_colors', ''),
  );

  // settings for pie charts
  $form['display_settings']['bar_chart_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bar charts'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // default column width
  $form['display_settings']['bar_chart_settings']['crm_core_petition_column_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Column width'),
    '#description' => t('Used on bar charts. Please enter the default width for columns as a decimal, i.e. 0.8. Leave blank to use the default width.'),
    '#default_value' => variable_get('crm_core_petition_column_width', CMCP_COLUMN_WIDTH),
  );

  // settings for line charts
  $form['display_settings']['line_chart_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Line charts'),
    '#weight' => 6,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // default number of tickmarks on the xaxis
  $form['display_settings']['line_chart_settings']['crm_core_petition_xaxis_tickmarks'] = array(
    '#type' => 'textfield',
    '#title' => t('Tick marks'),
    '#description' => t('Please enter the default number of tickmarks to display on the xaxis. This value can be overridden on individual charts.'),
    '#default_value' => variable_get('crm_core_petition_xaxis_tickmarks', CMCP_XAXIS_TICKS),
  );

  // settings for pie charts
  $form['display_settings']['pie_chart_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pie charts'),
    '#weight' => 7,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // display labels on pie regions
  $form['display_settings']['pie_chart_settings']['crm_core_petition_display_labels'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display labels on regions'),
    '#description' => t('Check this box to display labels on regions by default.'),
    '#default_value' => variable_get('crm_core_petition_display_labels', 0),
    '#return_value' => 1,
  );

  return system_settings_form($form);
}
