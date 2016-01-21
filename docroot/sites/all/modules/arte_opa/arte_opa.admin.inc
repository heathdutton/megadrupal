<?php

/**
 * @file
 * Administration functions for arte_opa.module.
 */

/**
 * Form builder; Configure OPA settings for this site.
 */
function arte_opa_admin_settings() {
  $form['opa'] = array(
    '#type' => 'fieldset',
    '#title' => t('OPA connection parameters'),
    '#description' => t('You will need to request Open API access to ARTE. !info.', array('!info' => l(t('Find out how to get access'), 'https://github.com/ArteGEIE/API-OPA/blob/develop/app/Resources/doc/index.md#how-to-request-access-).'))),
  );
  $form['opa']['arte_opa_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#default_value' => variable_get('arte_opa_client_id', ''),
    '#size' => 150,
    '#required' => TRUE,
  );
  $form['opa']['arte_opa_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client Secret'),
    '#default_value' => variable_get('arte_opa_client_secret', ''),
    '#size' => 150,
    '#required' => TRUE,
  );
  $form['opa']['arte_opa_token_never_expire'] = array(
    '#type' => 'textfield',
    '#title' => t('Token (Never Expire)'),
    '#default_value' => variable_get('arte_opa_token_never_expire', ''),
    '#description' => 'A never expire token can be used to avoid a roundtrip to fetch a token before issueing the OPA request. <em>Warning: this is only available for ARTE partners.</em>',
    '#size' => 150,
  );

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('OPA settings'),
    '#description' => t('General OPA settings'),
  );
  $form['general']['arte_opa_languages'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Languages'),
    '#default_value' => variable_get('arte_opa_languages', array()),
    '#options' => drupal_map_assoc(arte_opa_supported_languages()),
    '#description' => t('Select the languages that will be used to query OPA. If your site is in two languages (e.g. French and German), you would select both of these languages.'),
  );

  return system_settings_form($form);
}

/**
 * OPA Query builder page.
 */
function arte_opa_query_page() {
  // Add CSS.
  drupal_add_css(drupal_get_path('module', 'arte_opa') . '/arte_opa.css');

  $elements = drupal_get_form('arte_opa_query_form');
  $output = drupal_render($elements);
  $output .= _arte_opa_query_table();
  return $output;
}

/**
 * Return form for OPA query filters.
 *
 * @ingroup forms
 */
function arte_opa_query_form() {
  $filters = arte_opa_query_filters();

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('OPA Parameters'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  foreach ($filters as $key => $filter) {
    // Special case.
    if ($key == 'emNumber' || $key == 'freeFilters') {
      $form['filters']['status'][$key] = array(
        '#type' => 'textfield',
        '#title' => $filter['title'],
        '#description' => $filter['description'],
      );
    }
    else {
      $form['filters']['status'][$key] = array(
        '#title' => $filter['title'],
        '#type' => 'select',
        '#empty_value' => isset($filter['empty_value']) ? $filter['empty_value'] : 'all',
        '#empty_option' => isset($filter['empty_option']) ? $filter['empty_option'] : $filter['options']['all'],
        '#size' => 0,
        '#options' => $filter['options'],
      );
    }
    if (!empty($_SESSION['arte_opa_query_filter'][$key])) {
      $form['filters']['status'][$key]['#default_value'] = $_SESSION['arte_opa_query_filter'][$key];
    }
  }

  $form['filters']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Run query'),
  );
  if (!empty($_SESSION['arte_opa_query_filter'])) {
    $form['filters']['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset')
    );
  }

  return $form;
}

/**
 * Validate result from OPA query builder form.
 */
function arte_opa_query_form_validate($form, &$form_state) {
}

/**
 * Process result from OPA query builder form.
 */
function arte_opa_query_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];

  $filters = arte_opa_query_filters();
  switch ($op) {
    case t('Run query'):
      foreach ($filters as $name => $filter) {
        if (isset($form_state['values'][$name]) && $form_state['values'][$name]) {
          $_SESSION['arte_opa_query_filter'][$name] = $form_state['values'][$name];
        }
      }
      break;
    case t('Reset'):
      $_SESSION['arte_opa_query_filter'] = array();
      break;
  }

  $form_state['redirect'] = 'admin/config/services/opa';
}

/**
 * List locale translation filters that can be applied.
 */
function arte_opa_query_filters() {
  $filters = array();

  $filters['emNumber'] = array(
    'title' => t('Emission number'),
    'description' => t('The complete emission number of the video.'),
  );

  require_once DRUPAL_ROOT . '/includes/iso.inc';
  $predefined = _locale_get_predefined_list();
  $options = array('all' => t('All languages'));
  $languages = arte_opa_languages();
  foreach ($languages as $language) {
    $options[$language] = reset($predefined[$language]);
  }

  $filters['language'] = array(
    'title' => t('Language'),
    'options' => $options,
  );

  $filters['kind'] = array(
    'title' => t('Kind'),
    'options' => array(
      'all' => 'All kinds',
      'SHOW' => 'Show',
      'BONUS' => 'Bonus',
    ),
  );

  $filters['limit'] = array(
    'title' => t('Limit'),
    'options' => array(
      10 => 10,
      25 => 25,
      50 => 50,
      100 => 100,
      'all' => t('No limit'),
    ),
    'empty_value' => 10,
    'empty_option' => 10,
  );

  $filters['platform'] = array(
    'title' => t('Platform'),
    'options' => array('all' => t('All platforms')) + arte_opa_platforms(),
  );

  $filters['freeFilters'] = array(
    'title' => t('Extra parameters'),
    'description' => t('Write as <em>parameter=value&amp;parameter2=value2</em>'),
  );

  return $filters;
}

/**
 * Perform an OPA search and display results in a table
 */
function _arte_opa_query_table() {
  $output = '';

  // We have at least one criterion to match
  if (!($query = _arte_opa_query())) {
    $query = array(
      'language' => arte_opa_languages(),
      'limit' => 10,
      'kind' => 'SHOW',
    );
  }
  else {
    // Do not pass the "all" option
    foreach ($query as $var => $val) {
      if ($val == 'all') {
        unset($query[$var]);
      }
    }
    if (isset($query['freeFilters'])) {
      $extra = explode('&', $query['freeFilters']);
      foreach ($extra as $var) {
        $split = explode('=', $var);
        if (count($split) == 2) {
          $query[$split[0]] = $split[1];
        }
      }
      unset($query['freeFilters']);
    }
  }

  $opa = arte_opa_get_api();
  $opa->setDebug();

  $reader = new OPAVideosReader($opa, $query);
  $headers = array('emNumber', 'title', 'language', 'kind', 'platform', 'creationDate');
  $data = array();

  foreach ($reader as $key => $value) {
    $row = array();
    foreach ($headers as $header) {
      if ($header == 'title' && isset($value['infolink'])) {
        $row[] = l($value[$header], $value['infolink']);
      }
      else {
        $row[] = $value[$header];
      }
    }
    $data[] = $row;
  }

  $output = theme_table(array(
    'header' => $headers,
    'rows' => $data,
    'attributes' => array('class' => array('opa')),
    'caption' => 'From OPA. Showing ' . count($data) . ' results.',
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => 'No results found',
  ));

  return $output;
}

/**
 * Build array out of search criteria specified in request variables
 */
function _arte_opa_query() {
  $query = &drupal_static(__FUNCTION__);
  if (!isset($query)) {
    $query = array();
    $fields = arte_opa_query_filters();
    foreach ($fields as $field => $value) {
      if (isset($_SESSION['arte_opa_query_filter'][$field])) {
        $query[$field] = $_SESSION['arte_opa_query_filter'][$field];
      }
    }
  }
  return $query;
}
