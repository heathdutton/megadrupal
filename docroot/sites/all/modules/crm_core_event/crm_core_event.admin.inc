<?php
/**
 * @file
 * crm_core_event.admin.inc
 */

/**
 * Settings form builder.
 */
function crm_core_event_admin_settings_form($form, &$form_state) {
  $form['crm_core_event_auto_complete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Close event registration once an event has occurred'),
    '#description' => t('Check this box to automatically disable event registration once an event has occurred.'),
    '#default_value' => variable_get('crm_core_event_auto_complete', FALSE),
  );

  $form['crm_core_default_text_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Text'),
    '#collapsible' => TRUE,
  );

  $form['crm_core_default_text_fieldset']['crm_core_event_default_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Login to register message'),
    '#description' => t('Text to display to users when they must login to register for an event.'),
    '#default_value' => variable_get('crm_core_event_default_text', t('Please login to register for this event.')),
  );

  $form['crm_core_default_text_fieldset']['crm_core_event_max_registrants'] = array(
    '#type' => 'textarea',
    '#title' => t('Max registrants message'),
    '#description' => t("Text to display when registration for an event has reached it's maximum."),
    '#default_value' => variable_get('crm_core_event_max_registrants', t('This event is sold-out.')),
  );

  $form['crm_core_default_text_fieldset']['crm_core_event_registration_complete'] = array(
    '#type' => 'textarea',
    '#title' => t('Registration complete message'),
    '#description' => t("Text to display when registration has reached it's maximum."),
    '#default_value' => variable_get('crm_core_event_registration_complete', t('Registration is complete.')),
  );

  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email options'),
  );

  $form['email']['description_wrapper'] = array(
    '#type' => 'container',
  );
  $form['email']['description_wrapper']['description'] = array(
    '#markup' => t('Configure the default email to be sent when users register for an event. This setting can be overridden when setting up events.'),
  );

  $rule_set_tag = CMCEV_THANKS_EMAIL_TAG;
  $rules_components = _crm_core_event_load_rules_components_by_tag($rule_set_tag);
  $form['email']['crm_core_event_default_confirmation'] = array(
    '#type' => 'select',
    '#title' => t('Default Confirmation Message'),
    '#empty_option' => t('--Please Select--'),
    '#empty_value' => '',
    '#options' => $rules_components,
    '#default_value' => variable_get('crm_core_event_default_confirmation', ''),
  );

  return system_settings_form($form);
}

/**
 * Basic settings form builder.
 */
function crm_core_event_admin_general_settings_form($form, &$form_state) {

  return system_settings_form($form);
}

/**
 * Report settings form builder.
 */
function crm_core_event_admin_report_settings_form($form, &$form_state) {

  // Display settings.
  $form['display_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#weight' => 0,
    '#collapsible' => FALSE,
  );

  // Default height for charts in CRM Core Event.
  $form['display_settings']['crm_core_event_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Default height'),
    '#description' => t('Please enter the default height for charts used in CRM Core Event, as an integer.'),
    '#default_value' => variable_get('crm_core_event_height', '200'),
  );

  $form['display_settings']['crm_core_event_include_legend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display legends by default'),
    '#description' => t('Check this box to include a legend in charts by default.'),
    '#default_value' => variable_get('crm_core_event_include_legend', 0),
    '#return_value' => 1,
  );

  $form['display_settings']['crm_core_event_include_tooltips'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display tooltips when hovering over points in a chart'),
    '#description' => t('Check this box to include tooltips in flot charts.'),
    '#default_value' => variable_get('crm_core_event_include_tooltips', 0),
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
  $form['display_settings']['default_colors']['crm_core_event_line_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Line color'),
    '#description' => t('Used on line charts. Please enter the color to use for the line in valid #RRGGBB or rgba format. Leave blank to use the default color.'),
    '#default_value' => variable_get('crm_core_event_line_color', ''),
  );

  // Default chart color.
  $form['display_settings']['default_colors']['crm_core_event_chart_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Chart color'),
    '#description' => t('Used for most charts. Please enter the background color to use for the chart iteself in valid #RRGGBB or rgba format. Leave blank to use the default color.'),
    '#default_value' => variable_get('crm_core_event_chart_color', ''),
  );

  // Default series background color.
  $form['display_settings']['default_colors']['crm_core_event_background_color'] = array(
    '#type' => 'textarea',
    '#title' => t('Background color'),
    '#description' => t('Used for line charts. Please enter the color to use for the background of the series in valid #RRGGBB or rgba format. Leave blank to use the default color. You can create gradients by separating each value with a new line.'),
    '#default_value' => variable_get('crm_core_event_background_color', ''),
  );

  // Default series background color.
  $form['display_settings']['default_colors']['crm_core_event_series_colors'] = array(
    '#type' => 'textarea',
    '#title' => t('Series colors'),
    '#description' => t('Used for pie and bar charts. Please enter the default color series to use when displaying data, in valid #RRGGBB or rgba format. Please separate each value with a new line. Leave blank to use the default color scheme.'),
    '#default_value' => variable_get('crm_core_event_series_colors', ''),
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
  $form['display_settings']['bar_chart_settings']['crm_core_event_column_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Column width'),
    '#description' => t('Used on bar charts. Please enter the default width for columns as a decimal, i.e. 0.8. Leave blank to use the default width.'),
    '#default_value' => variable_get('crm_core_event_column_width', '0.8'),
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
  $form['display_settings']['line_chart_settings']['crm_core_event_xaxis_tickmarks'] = array(
    '#type' => 'textfield',
    '#title' => t('Tick marks'),
    '#description' => t('Please enter the default number of tickmarks to display on the xaxis. This value can be overridden on individual charts.'),
    '#default_value' => variable_get('crm_core_event_xaxis_tickmarks', '7'),
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
  $form['display_settings']['pie_chart_settings']['crm_core_event_display_labels'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display labels on regions'),
    '#description' => t('Check this box to display labels on regions by default.'),
    '#default_value' => variable_get('crm_core_event_display_labels', 0),
    '#return_value' => 1,
  );

  return system_settings_form($form);
}
