<?php
/**
 * @file
 * Admin forms and page callbacks for trello api module.
 */

/**
 * General settings form.
 */
function trello_api_settings_form($form, &$form_state) {
  $form['intro'] = array(
    'key' => array(
      '#theme' => 'html_tag',
      '#tag' => 'p',
      '#value' => t('To get your API key, you can go to !trello_url and see it when logged in to trello.', array(
        '!trello_url' => '<a href="https://trello.com/app-key">https://trello.com/app-key</a>',
      )),
    ),
  );

  if (isset($_GET['token'])) {
    drupal_set_message(t('Token @new_token generated and saved replacing old token @old_token. If this is an error, you can manually save alter the token below.', array(
      '@new_token' => $_GET['token'],
      '@old_token' => variable_get('trello_api_token', ''),
    )), 'warning');
    variable_set('trello_api_token', $_GET['token']);
  }

  if (variable_get('trello_api_key', '')) {
    $trello_link_query = array(
      'key' => variable_get('trello_api_key', ''),
      'name' => 'Drupal Trello API: ' . variable_get('site_name'),
      'response_type' => 'token',
      'scope' => 'read',
      'return_url' => url('admin/config/services/trello', array('absolute' => TRUE)),
    );

    $form['intro']['token'] = array(
      'text' => array(
        '#theme' => 'html_tag',
        '#tag' => 'p',
        '#value' => t('To generate a token for your currently saved key, you can click on one of the links below.'),
      ),
      'links' => array(
        '#theme' => 'item_list',
        '#items' => array(
          l(t('Authorize for one day'), 'https://api.trello.com/1/authorize', array('query' => array_merge($trello_link_query, array(array('expiration' => '1day'))))),
          l(t('Authorize for 30 day'), 'https://api.trello.com/1/authorize', array('query' => array_merge($trello_link_query, array(array('expiration' => '30days'))))),
          l(t('Authorize forever'), 'https://api.trello.com/1/authorize', array('query' => array_merge($trello_link_query, array(array('expiration' => 'never'))))),
        )
      )
    );
  }

  $form['trello_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Trello API key'),
    '#description' => t('The key used for API integration.'),
    '#required' => TRUE,
    '#default_value' => variable_get('trello_api_key', ''),
  );

  $form['trello_api_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Trello API token'),
    '#description' => t('The token used for API integration.'),
    '#default_value' => variable_get('trello_api_token', ''),
  );

  $form['trello_api_auto_save'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatic save items fetched from Trello.'),
    '#default_value' => variable_get('trello_api_auto_save', FALSE),
  );

  $form['#attached']['js'][] = file_create_url(drupal_get_path('module', 'trello_api') . '/js/trello_api.admin.js');

  return system_settings_form($form);
}

