<?php

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'analytics_service',
  //'access' => 'administer analytics services',
  'menu' => array(
    'menu prefix' => 'admin/config/services',
    'menu item' => 'analytics',
    'menu title' => 'Analytics services',
    'menu description' => 'Add, edit, and remove analytics services.',
  ),
  'title singular' => t('analytics service'),
  'title singular proper' => t('Analytics service'),
  'title plural' => t('analytics services'),
  'title plural proper' => t('Analytics services'),
  'form' => array(
    'settings' => 'analytics_service_edit_form',
    //'validate' => 'analytics_service_edit_form_validate',
    //'submit' => 'analytics_service_edit_form_submit',
  ),
  'export' => array(
    'admin_title' => 'label',
  ),
   'handler' => array(
     'class' => 'AnalyticsServiceExportUI',
     'parent' => 'ctools_export_ui',
   ),
);

function analytics_service_edit_form(&$form, &$form_state) {
  $service = $form_state['item'];

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $service->description,
    '#rows' => 2,
    '#access' => FALSE,
  );

  if (empty($service->service)) {
    $options = analytics_service_add_service_options();
    natsort($options);
    $form['service'] = array(
      '#type' => 'select',
      '#title' => t('Service'),
      '#options' => $options,
      '#required' => TRUE,
      '#ajax' => array(
        'callback' => 'analytics_service_edit_form_options_ajax',
        'wrapper' => 'analytics-service-options',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );
  }
  else {
    $info = analytics_get_service_info($service->service);
    $form['service'] = array(
      '#type' => 'item',
      '#title' => t('Service'),
      '#markup' => check_plain($info['label']),
    );
  }

  $form['options'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
    '#prefix' => '<div id="analytics-service-options">',
    '#suffix' => '</div>',
  );
  if (empty($service->service) && !empty($form_state['values']['service'])) {
    $service->service = $form_state['values']['service'];
    $service->options = array();
  }
  if (!empty($service->service)) {
    try {
      $instance = analytics_get_service_instance($service);
      $form['options'] += $instance->getFormOptions($form, $form_state);
    }
    catch (Exception $e) {
      form_set_error('options', $e->getMessage());
    }
  }
}

/**
 * Callback element needs only select the portion of the form to be updated.
 * Since #ajax['callback'] return can be HTML or a renderable array (or an
 * array of commands), we can just return a piece of the form.
 */
function analytics_service_edit_form_options_ajax($form, $form_state) {
  return $form['options'];
}
