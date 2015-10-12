<?php

/**
 * Form that provide exportable pretty guzzle json view.
 */
function services_guzzle_documentation_form($form, &$form_state, $endpoint) {
  $form = array();

  module_load_include('inc', 'services_guzzle');

  $return = services_guzzle_build_documentation_from_internals($endpoint);
  $return_json = json_encode($return);
  $return_json = str_replace('\/', '/', $return_json);

  $form['title'] = array(
    '#markup' => '<h2>' . t('Guzzle endpoint service description') . '</h2>'
  );

  $form['description'] = array(
    '#markup' => t('For more information about format see http://guzzlephp.org/guide/service/service_descriptions.html')
  );

  $form['guzzle_json'] = array(
    '#type' => 'textarea',
    '#title' => '',
    '#value' => _services_guzzle_json_pretty($return_json),
    '#rows' => 25
  );

  return $form;
}