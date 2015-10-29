<?php
// $Id: statspro_admin_settings.inc,v 1.1.2.12 2010/11/05 23:02:14 rsevero Exp $

/**
 * @file
 *   Admin settings page.
 *
 */

require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'statspro') . '/statspro_settings.inc';

/**
 * Main admin settings form.
 */
function statspro_main_admin_settings_form($form, &$form_state) {
  $form = statspro_settings_form(
    variable_get('statspro_period', 'today'),
    variable_get('statspro_custom_number_days', 30)
  );

  $charting_libraries = statspro_get_available_charts_graphs();
  if ($charting_libraries) {
    $form['#validate'][] = 'statspro_admin_settings_form_validate';
    $charting_library_options = array();
    foreach ($charting_libraries as $charting_library) {
      $charting_library_options[$charting_library->name] = $charting_library->nice_name;
    }
    ksort($charting_library_options);
    $form['statspro_charting_library'] = array(
      '#type' => 'select',
      '#title' => t('Charting library'),
      '#options' => $charting_library_options,
      '#default_value' => statspro_get_current_charting_option(),
      '#required' => TRUE,
      '#weight' => 5,
      '#multiple' => FALSE,
    );
  }

  $form['optional_requirements'] = array(
    '#type' => 'fieldset',
    '#title' => t('Optional requirements'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 100,
  );

  statspro_optional_dependency_statistics($form);

  statspro_optional_dependency_access_log($form);

  statspro_optional_dependency_charts_graphs($form);

  statspro_optional_dependency_views_charts($form);

  statspro_optional_dependency_advanced_help($form);

  statspro_include_link_to_advanced_help_pages($form);

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function statspro_admin_settings_form_validate($form, &$form_state) {
  $charting_libraries = statspro_get_available_charts_graphs();
  if (!isset($charting_libraries[$form_state['values']['statspro_charting_library']])) {
    form_set_error(
      'statspro_charting_library',
      t(
        'Invalid charting option %charting_option.',
        array('%charting_option' => $form_state['values']['statspro_charting_library'])
      )
    );
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function statspro_optional_dependency_statistics(&$form) {
  $dependency_ok = module_exists('statistics');
  $form['optional_requirements']['statistics_module_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Statistics module'),
    '#collapsible' => TRUE,
    '#collapsed' => $dependency_ok,
  );
  $message = "\n<p>" . t("The core %statistics_module module is required
    for the following optional reports:",
    array(
    '%statistics_module' => t('Statistics'),
  )
  ) . "</p>\n";
  $message .= sprintf(<<<LIST
<ul>
  <li>%s</li>
  <li>%s</li>
</ul>
LIST
,
    t('Path aggregated'),
    t('Page visualization')
  );
  if (!$dependency_ok) {
    $message .= "\n<p>" . t(
      "You should also enable the %access_log_setting setting in the %access_log
      page.",
      array(
      '%access_log_setting' => t('Enable access log'),
      '%access_log' => t('Access log settings'),
    )
    ) . "</p>\n";
  }
  $message .= "\n<p>" . t(
    "The core %statistics_module module is currently %status.",
    array(
    '%statistics_module' => t('Statistics'),
    '%status' => $dependency_ok ? t('Enabled') : t('Disabled'),
  )
  ) . "</p>\n";
  $form['optional_requirements']['statistics_module_wrapper']['statistics_module'] = array(
    '#value' => $message,
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function statspro_optional_dependency_access_log(&$form) {
  if (module_exists('statistics') && (variable_get('statistics_enable_access_log', 0) == 0)) {
    $form['optional_requirements']['access_log_setting_wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Access logging settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $message = "\n<p>" . t("The core %statistics_module module is enabled but the
    %access_log_setting option in the !access_log page isn't. The
    %access_log_setting option is required for the following optional reports:",
      array(
      '%statistics_module' => t('Statistics'),
      '%access_log_setting' => t('Enable access log'),
      '!access_log' => l(t('Access log settings'), 'admin/config/system/statistics'),
    )
    ) . "</p>\n";
    $message .= sprintf(<<<LIST
<ul>
  <li>%s</li>
  <li>%s</li>
</ul>
LIST
,
      t('Path aggregated'),
      t('Page visualization')
    );
    $form['optional_requirements']['access_log_setting_wrapper']['access_log_setting'] = array(
      '#value' => $message,
    );
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function statspro_optional_dependency_charts_graphs(&$form) {
  $dependency_ok = module_exists('charts_graphs');
  $form['optional_requirements']['charts_graphs_module_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Charts and Graphs module'),
    '#collapsible' => TRUE,
    '#collapsed' => $dependency_ok,
  );
  $message = "\n<p>" . t("The %charts_graphs_module module is required
    for producing graphs on several reports.",
    array(
    '%charts_graphs_module' => t('Charts and Graphs'),
  )
  ) . "</p>\n";
  $message .= "\n<p>" . t(
    "The %charts_graphs_module module is currently %status.",
    array(
    '%charts_graphs_module' => t('Charts and Graphs'),
    '%status' => $dependency_ok ? t('Enabled') : t('Disabled'),
  )
  ) . "</p>\n";
  $form['optional_requirements']['charts_graphs_module_wrapper']['charts_graphs_module'] = array(
    '#value' => $message,
  );
  if ($dependency_ok) {
    $available_charts_graphs = charts_graphs_apis();
    if (count($available_charts_graphs) == 0) {
      $message = "\n<p>" . t("There is no charting submodule enabled for
        %charts_graphs_module module. You need at least one charting submodule
        enabled.",
        array(
        '%charts_graphs_module' => t('Charts and Graphs'),
      )
      ) . "</p>\n";
      $form['optional_requirements']['charts_graphs_module_wrapper']['charts_graphs_submodule'] = array(
        '#value' => $message,
      );
    }
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function statspro_optional_dependency_views_charts(&$form) {
  $dependency_ok = module_exists('views_charts');
  $form['optional_requirements']['views_charts_module_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Views Charts module'),
    '#collapsible' => TRUE,
    '#collapsed' => $dependency_ok,
  );
  $message = "\n<p>" . t("The %views_charts_module module is required
    for producing graphs on comments, logs, nodes, page visualization and users
    reports.",
    array(
    '%views_charts_module' => t('Views Charts'),
  )
  ) . "</p>\n";
  $message .= "\n<p>" . t(
    "The %views_charts_module module is currently %status.",
    array(
    '%views_charts_module' => t('Views Charts'),
    '%status' => $dependency_ok ? t('Enabled') : t('Disabled'),
  )
  ) . "</p>\n";
  $form['optional_requirements']['views_charts_module_wrapper']['views_charts_module'] = array(
    '#value' => $message,
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function statspro_optional_dependency_advanced_help(&$form) {
  $dependency_ok = module_exists('advanced_help');
  $form['optional_requirements']['advanced_help_module_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Help module'),
    '#collapsible' => TRUE,
    '#collapsed' => $dependency_ok,
  );
  $message = "\n<p>" . t("The %advanced_help_module module is required
    for presenting help info about Statistics Pro.",
    array(
    '%advanced_help_module' => t('Advanced Help'),
  )
  ) . "</p>\n";
  $message .= "\n<p>" . t(
    "The %advanced_help_module module is currently %status.",
    array(
    '%advanced_help_module' => t('Advanced Help'),
    '%status' => $dependency_ok ? t('Enabled') : t('Disabled'),
  )
  ) . "</p>\n";
  $form['optional_requirements']['advanced_help_module_wrapper']['advanced_help_module'] = array(
    '#value' => $message,
  );
}

/**
 * Implements hook_modulename_submit() for main admin settings form().
 */
function statspro_main_admin_settings_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($op == t('Reset to defaults')) {
    variable_del('statspro_period');
    variable_del('statspro_custom_number_days');
    variable_del('statspro_charting_library');
    $keys = array_keys($_SESSION);
    foreach ($keys as $key) {
      if (strpos($key, 'statspro_') === 0) {
        unset($_SESSION[$key]);
      }
    }
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
  else {
    variable_set('statspro_period', $form_state['values']['statspro_period']);
    if ($form_state['values']['statspro_period'] == 'custom_days') {
      variable_set('statspro_custom_number_days', (int) $form_state['values']['statspro_custom_number_days']);
    }
    if (statspro_get_available_charts_graphs()) {
      variable_set('statspro_charting_library', $form_state['values']['statspro_charting_library']);
    }
    drupal_set_message(t('The configuration options have been saved.'));
  }
}
