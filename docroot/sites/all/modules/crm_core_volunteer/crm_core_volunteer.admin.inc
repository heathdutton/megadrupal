<?php
/**
 * @file
 * Administrative pages for CRM Core Volunteer.
 */

/**
 * Basic settings form builder.
 */
function cmcv_admin_general_settings_form($form, &$form_state) {

  $form['cmcv_wait_list'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable wait list for volunteer opportunities?'),
    '#description' => t("Check this box to allow people to register for volunteer opportunities once the maximum number of volunteers has been reached. When this box is not checked, registration will be cut off when an opportunity meets it's target."),
    '#default_value' => variable_get('cmcv_wait_list', 1),
  );

  $form['cmcv_late_registration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow volunteers to register once an opportunity has passed?'),
    '#description' => t("Check this box to allow people to register for volunteer opportunities after they have already occurred. This will store the volunteer's information in your system for use in the future."),
    '#default_value' => variable_get('cmcv_late_registration', 1),
  );

  $form['cmcv_cutoff_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Registration cutoff message'),
    '#description' => t("Message to display to volunteers when registration is no longer available for an opportunity."),
    '#default_value' => variable_get('cmcv_cutoff_message', 'Registration closed!'),
  );

  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email options'),
  );

  $form['email']['description_wrapper'] = array(
    '#type' => 'container',
  );
  $form['email']['description_wrapper']['description'] = array(
    '#markup' => t('Configure the default email to be sent when users register for a volunteer opportunity or are added to the wait list. These settings can be overridden when setting up volunteer opportunities.'),
  );

  $rule_set_tag = CMCV_SIGN_EMAIL_TAG;
  $rules_components = _crm_core_volunteer_load_rules_components_by_tag($rule_set_tag);
  $form['email']['crm_core_volunteer_default_sign_email'] = array(
    '#type' => 'select',
    '#title' => t('Volunteer Sign Up Email sending rule'),
    '#empty_option' => t('--Please Select--'),
    '#empty_value' => '',
    '#options' => $rules_components,
    '#default_value' => variable_get('crm_core_volunteer_default_sign_email', ''),
  );

  $rule_set_tag = CMCV_WAIT_EMAIL_TAG;
  $rules_components = _crm_core_volunteer_load_rules_components_by_tag($rule_set_tag);
  $form['email']['crm_core_volunteer_default_wait_email'] = array(
    '#type' => 'select',
    '#title' => t('Volunteer Waiting List Email sending rule'),
    '#empty_option' => t('--Please Select--'),
    '#empty_value' => '',
    '#options' => $rules_components,
    '#default_value' => variable_get('crm_core_volunteer_default_wait_email', ''),
  );

  return system_settings_form($form);
}

/**
 * Report settings form builder.
 */
function cmcv_admin_report_settings_form($form, &$form_state) {

  // Display settings.
  $form['display_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#weight' => 0,
    '#collapsible' => FALSE,
  );

  // Default height for charts in CRM Core Volunteer.
  $form['display_settings']['crm_core_volunteer_chart_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Default height'),
    '#description' => t('Please enter the default height for charts used in CRM Core volunteer, as an integer.'),
    '#default_value' => variable_get('crm_core_volunteer_chart_height', CMCV_CHART_HEIGHT),
  );

  $form['display_settings']['crm_core_volunteer_include_legend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display legends by default'),
    '#description' => t('Check this box to include a legend in charts by default.'),
    '#default_value' => variable_get('crm_core_volunteer_include_legend', 0),
    '#return_value' => 1,
  );

  $form['display_settings']['crm_core_volunteer_include_tooltips'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display tooltips when hovering over points in a chart'),
    '#description' => t('Check this box to include tooltips in flot charts.'),
    '#default_value' => variable_get('crm_core_volunteer_include_tooltips', 0),
    '#return_value' => 1,
  );

  // Settings for line charts.
  $form['display_settings']['default_colors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Colors'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Default line color.
  $form['display_settings']['default_colors']['crm_core_volunteer_line_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Line color'),
    '#description' => t('Used on line charts. Please enter the color to use for the line in valid #RRGGBB or rgba format. Leave blank to use the default color.'),
    '#default_value' => variable_get('crm_core_volunteer_line_color', ''),
  );

  // Default chart color.
  $form['display_settings']['default_colors']['crm_core_volunteer_chart_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Chart color'),
    '#description' => t('Used for most charts. Please enter the background color to use for the chart iteself in valid #RRGGBB or rgba format. Leave blank to use the default color.'),
    '#default_value' => variable_get('crm_core_volunteer_chart_color', ''),
  );

  // Default series background color.
  $form['display_settings']['default_colors']['crm_core_volunteer_background_color'] = array(
    '#type' => 'textarea',
    '#title' => t('Background color'),
    '#description' => t('Used for line charts. Please enter the color to use for the background of the series in valid #RRGGBB or rgba format. Leave blank to use the default color. You can create gradients by separating each value with a new line.'),
    '#default_value' => variable_get('crm_core_volunteer_background_color', ''),
  );

  // Default series background color.
  $form['display_settings']['default_colors']['crm_core_volunteer_series_colors'] = array(
    '#type' => 'textarea',
    '#title' => t('Series colors'),
    '#description' => t('Used for pie and bar charts. Please enter the default color series to use when displaying data, in valid #RRGGBB or rgba format. Please separate each value with a new line. Leave blank to use the default color scheme.'),
    '#default_value' => variable_get('crm_core_volunteer_series_colors', ''),
  );

  // Settings for pie charts.
  $form['display_settings']['bar_chart_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bar charts'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Default column width.
  $form['display_settings']['bar_chart_settings']['crm_core_volunteer_column_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Column width'),
    '#description' => t('Used on bar charts. Please enter the default width for columns as a decimal, i.e. 0.8. Leave blank to use the default width.'),
    '#default_value' => variable_get('crm_core_volunteer_column_width', CMCV_COLUMN_WIDTH),
  );

  // Settings for line charts.
  $form['display_settings']['line_chart_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Line charts'),
    '#weight' => 6,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Default number of tickmarks on the xaxis.
  $form['display_settings']['line_chart_settings']['crm_core_volunteer_xaxis_tickmarks'] = array(
    '#type' => 'textfield',
    '#title' => t('Tick marks'),
    '#description' => t('Please enter the default number of tickmarks to display on the xaxis. This value can be overridden on individual charts.'),
    '#default_value' => variable_get('crm_core_volunteer_xaxis_tickmarks', CMCV_XAXIS_TICKS),
  );

  // Settings for pie charts.
  $form['display_settings']['pie_chart_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pie charts'),
    '#weight' => 7,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Display labels on pie regions.
  $form['display_settings']['pie_chart_settings']['crm_core_volunteer_display_labels'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display labels on regions'),
    '#description' => t('Check this box to display labels on regions by default.'),
    '#default_value' => variable_get('crm_core_volunteer_display_labels', 1),
    '#return_value' => 1,
  );

  return system_settings_form($form);
}
