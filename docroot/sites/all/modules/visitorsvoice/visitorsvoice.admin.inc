<?php

/**
 * @file
 * Contains admin functions for the Visitors Voice module.
 */

/**
 * Page callback: Constructs a form for changing the Visitors Voice settings.
 */
function visitorsvoice_settings(array $form) {
  $access_key = variable_get('visitorsvoice_access_key', '');
  if ($access_key) {
    $options['attributes']['class'][] = 'button';
    $form['login'] = array(
      '#type' => 'fieldset',
      '#title' => t('Visitors Voice dashboard'),
      '#description' => t('Log in to the Visitors Voice dashboard to start customizing search results on your site.'),
      'login' => array(
        '#markup' => '<p>' . l(t('Log in'), 'http://service.visitorsvoice.com/Account/Login', $options) . '</p>',
      ),
    );
  }

  $form['visitorsvoice_access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Visitors Voice Access key'),
    '#description' => t('Please input your Visitors Voice Access key. This key will be used by Visitors Voice to authenticate its communication with your site. If you do not have such a key, you can sign up for free <a href="@url">here</a>.', array('@url' => url('http://www.visitorsvoice.com/sign-up-free.html'))),
    '#default_value' => $access_key,
    '#size' => 36,
  );

  $indexes = array();
  foreach (search_api_index_load_multiple(FALSE) as $index_id => $index) {
    if ($index->server() && $index->server()->class == 'search_api_solr_service') {
      $indexes[$index_id] = $index->label();
    }
  }
  if ($indexes) {
    $form['visitorsvoice_index'] = array(
      '#type' => 'select',
      '#title' => t('Search index to customize'),
      '#description' => t('Currently, Visitors Voice can only customize results for a single search index at a time. Please configure here which index to use. Support for customizing searches of multiple indexes will be added in a later version of Visitors Voice. (Note: Only indexes lying on Solr servers are eligible here.)'),
      '#options' => $indexes,
      '#disabled' => count($indexes) == 1,
      '#default_value' => variable_get('visitorsvoice_index', key($indexes)),
    );
  }
  else {
    $vars['@url'] = url('admin/config/search/search_api/add_index');
    $form['visitorsvoice_index'] = array(
      '#markup' => '<div>' . t('There are currently no Search API search indexes configured. Please <a href="@url">add a search index</a> before proceeding.', $vars) . '</div>',
    );
  }

  /*
  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('API access'),
    '#description' => t('Visitors Voice provides an API for displaying its statistical data on your local site. For that, an API key needs to be set. You will also need a module which uses this API for anything to be displayed to you.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['api']['visitorsvoice_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Visitors Voice API key'),
    '#description' => t('Please input your Visitors Voice API key. This key will be used to retrieve statistical data from Visitors Voice.'),
    '#default_value' => variable_get('visitorsvoice_api_key', ''),
    '#size' => 20,
  );

  $form['api']['visitorsvoice_cache_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache Response Data'),
    '#description' => t('Store Visitors Voice API responses in the cache.'),
    '#default_value' => variable_get('visitorsvoice_cache_data', FALSE),
  );

  $form['api']['visitorsvoice_cache_expire'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache Expiration'),
    '#description' => t('Set cache expiration time in minutes. This will only work if "Cache Response Data" is enabled.'),
    '#default_value' => variable_get('visitorsvoice_cache_expire', 0),
    '#size' => 5,
  );
  $form['api']['visitorsvoice_cache_expire']['#states']['visible'][':input[name="visitorsvoice_cache_data"]']['checked'] = TRUE;
  */

  return system_settings_form($form);
}
