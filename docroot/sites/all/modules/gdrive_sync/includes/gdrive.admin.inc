<?php

function gdrive_admin_settings($form, &$form_state) {
  $form['Description'] = array(
    '#value' => t('Enter you registration OAuth2 parameters here')
  );
  $form['clientid'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('gdrive_clientid'),
    '#title' => t('Client Id'),
  );

  $form['secret'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('gdrive_secret'),
    '#title' => t('Secret Key')
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function gdrive_admin_settings_submit($form, &$form_state) {
  variable_set('gdrive_clientid', $form_state['values']['clientid']);
  variable_set('gdrive_secret', $form_state['values']['secret']);
}

function gdrive_gdrive_admin_settings_synhronize() {
  $ret = '';
  $path = drupal_get_path('module', 'gdrive');

  require_once ($path . '/includes/google-api-php-client/src/Google_Client.php');
  require_once ($path . '/includes/google-api-php-client/src/contrib/Google_DriveService.php');
  require_once ($path . '/model/gdrive.db.inc');
  $client = new Google_Client();
  $client->setClientId(variable_get('gdrive_clientid'));
  $client->setClientSecret(variable_get('gdrive_secret'));
  $client->setRedirectUri(url('admin/config/services/gdrive/synchro', array('absolute' => TRUE)));
  $client->setScopes(array('https://www.googleapis.com/auth/drive'));
  $service = new Google_DriveService($client);
  if (!isset($_REQUEST['code'])) {
    $authUrl = $client->createAuthUrl();
    drupal_goto($authUrl);
  }

  $accessToken = $client->authenticate($_REQUEST['code']);
  $client->setAccessToken($accessToken);
  // take maximum number of elements
  $list = $service->files->list(array('maxResults' => 1000));
  $num = 0;
  $new = 0;
  foreach ($list['items'] as $item) {
    $num++;
    if (insert_file($item)) {
      $new++;
    }
  }
  $ret = 'All files: <strong>{$num}</strong><br />';
  $ret .= 'New files: <strong>{$new}</strong>';
  return $ret;
}