<?php
/**
 * @file
 * Administrative page callbacks for the youtube_upload module.
 */

/**
 * General configuration form for youtube (account info + youtube settings).
 */
function media_youtube_upload_account_settings_form($form, $form_state) {
  include_once DRUPAL_ROOT . '/includes/locale.inc';

  // Test if we get the google API Library client.
  $ytapi = new MediaYoutubeUploadYtapi();
  if (!$ytapi->mtestGoogleLib()) {
    $form['lib_error']['#markup'] = t('Google Libraries not found.<br />Use drush to download them <em>drush myu-libraries</em>
        <br /> then go to the library folder google-api-php-client and run <em>composer install</em> 
        <br />so the path <em>libraries/google-api-php-client/src/Google/autoload.php</em> is available
        <br /> or check the !status for the manual procedure, section Google API and Google CORS.', array(
          '!status' => l(t('Status report'), 'admin/reports/status'),
        ));
    return $form;
  }

  $app_name = variable_get('media_youtube_upload_app_name', '');
  $client_id = variable_get('media_youtube_upload_client_id', '');
  $client_secret = variable_get('media_youtube_upload_client_secret', '');
  $countries = country_get_list();
  $app_country = variable_get('media_youtube_upload_app_country', '');
  $ca_certificate = variable_get('media_youtube_upload_root_ca_certificate', '');
  $access_application = user_access('administer media youtube upload google application');

  // Add an ID for some css styling if the application is authorized.
  $form['#id'] = 'media-youtube-upload-account-settings-form';

  $form['google_app'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Google Application'),
  );
  // Get the token if we have one.
  $token = variable_get('media_youtube_upload_token', array());
  // Display just the application information if we have a token present.
  if (!empty($token['refresh_token'])) {
    $form['google_app']['#description'] = t('Google application is succesfully authenticated.');
    if (!$access_application) {
      $form['google_app']['#description'] .= ' ' . ('But you do not have permission to access or alter the credentials.');
    }
    $form['google_app']['media_youtube_upload_app_name'] = array(
      '#type' => 'item',
      '#title' => t('Application name') . ': ',
      '#markup' => $access_application ? $app_name : str_repeat('&#959;', strlen($app_name)),
    );
    $form['google_app']['media_youtube_upload_client_id'] = array(
      '#type' => 'item',
      '#title' => t('Client ID') . ': ',
      '#markup' => $access_application ? $client_id : str_repeat('&#959;', strlen($client_id)),
    );
    $form['google_app']['media_youtube_upload_client_secret'] = array(
      '#type' => 'item',
      '#title' => t('Client secret') . ': ',
      '#markup' => $access_application ? $client_secret : str_repeat('&#959;', strlen($client_secret)),
    );
    $form['google_app']['media_youtube_upload_app_country'] = array(
      '#type' => 'item',
      '#title' => t('Country') . ': ',
      '#markup' => $countries[$app_country],
    );
    if (!empty($ca_certificate)) {
      $form['google_app']['media_youtube_upload_root_ca_certificate'] = array(
        '#type' => 'item',
        '#title' => t('Root CA Certificate') . ': ',
        '#markup' => $access_application ? $ca_certificate : str_repeat('&#959;', strlen($ca_certificate)),
      );
    }
    $form['#attributes']['class'] = array('successfully-authenticated');

    $form['#attached']['css'] = array(
      drupal_get_path('module', 'media_youtube_upload') . '/css/media_youtube_upload.css',
    );

    $form['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset application'),
      '#weight' => 2,
      '#access' => $access_application,
    );
  }
  // If no token is present, display the application form.
  else {
    global $base_url;
    $form['google_app']['#description'] = t('Get credentials keys on !here: <ul><li>Create a new project and give it a project name accordingly.</li><li>Under "API Manager" -> "Overview" -> enable the "YouTube Data API".</li><li>Next in "API Manager" -> "Credentials" -> Add credentials -> "OAuth 2.0 Client ID".</li><li>Then under "Application Type" select "Web Application" and fill the website URI\'s.</li><li>For the "Redirect URI" use <strong>!base_url/media_youtube_upload/oauth2callback</strong></li></ul>If these directions are not clear you can visit the !documentation_page with screenshots. <br />  * A much made mistake is when people get the error "Invalid Credentials" when uploading a video. This probably means you haven\'t set up a YouTube channel yet for your YouTube account.',
      array(
        '!here' => l(t('https://console.developers.google.com'), 'https://console.developers.google.com/project', array('attributes' => array('target' => '_blank'))),
        '!base_url' => $base_url,
        '!documentation_page' => l(t('documentation page'), 'https://www.drupal.org/node/2601984', array('attributes' => array('target' => '_blank'))),
      ));
    $form['google_app']['media_youtube_upload_app_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Application name'),
      '#default_value' => $app_name,
      '#required' => TRUE,
    );
    $form['google_app']['media_youtube_upload_client_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Client ID'),
      '#default_value' => $client_id,
      '#required' => TRUE,
    );
    $form['google_app']['media_youtube_upload_client_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Client secret'),
      '#default_value' => $client_secret,
      '#required' => TRUE,
    );
    $country = _media_youtube_upload_get_country();
    $form['google_app']['media_youtube_upload_app_country'] = array(
      '#type' => 'select',
      '#title' => t('Country'),
      '#description' => t('The country is required to get video category list. If you change this setting after you have data in the category field it is possible you will lose data.'),
      '#options' => $countries,
      '#default_value' => !empty($app_country) ? $app_country : $country,
      '#required' => TRUE,
      '#prefix' => l('', '', array('attributes' => array('name' => 'media-youtube-upload-app-country'))),
    );

    $form['google_app']['media_youtube_upload_root_ca_certificate'] = array(
      '#type' => 'textfield',
      '#title' => t('Root CA Certificate'),
      '#description' => t('If you are testing the Google Application on your localhost and get the following error: <em>GuzzleHttp\Exception\RequestException: cURL error 60: SSL certificate problem: unable to get local issuer certificate</em>. Then you need to provide the absolute path to the root CA certificate to authorize the SSL requests. Example path for WAMP: <em>C:\wamp\bin\php\php5.5.12\cacert.pem</em>. If no such error is presented you can leave this field empty.'),
      '#default_value' => variable_get('media_youtube_upload_root_ca_certificate', ''),
    );
  }

  $form['youtube_settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('YouTube settings'),
  );

  $form['youtube_settings']['media_youtube_upload_automatic_deletion'] = array(
    '#type' => 'checkbox',
    '#title' => '<strong>' . t('Automatic deletion') . '</strong>',
    '#description' => t('If this checkbox is checked all YouTube videos deleted on this site will also get deleted on YouTube. Be carefull with this option. <strong>If you wish to keep your videos online do not check this box!</strong> You can also use the action "Delete locally and remotely" if you wish to have more granular control.'),
    '#default_value' => variable_get('media_youtube_upload_automatic_deletion', FALSE),
  );

  $form['youtube_settings']['media_youtube_upload_replace_page_title'] = array(
    '#type' => 'checkbox',
    '#title' => '<strong>' . t('Replace page title') . '</strong>',
    '#description' => t('Check this box if you want to replace the page title <strong>"Filename"</strong> with the <strong>"YouTube video title"</strong>.'),
    '#default_value' => variable_get('media_youtube_upload_replace_page_title', FALSE),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 1,
  );

  return $form;
}

/**
 * Implements hook_form_submit().
 *
 * @see media_youtube_upload_account_settings_form()
 */
function media_youtube_upload_account_settings_form_submit($form, &$form_state) {

  $vals = $form_state['values'];
  $access_application = user_access('administer media youtube upload google application');
  if (strpos($form_state['triggering_element']['#id'], 'edit-submit') !== FALSE) {
    form_state_values_clean($form_state);
    // Populate variables.
    foreach ($form_state['values'] as $key => $val) {
      // Convert settings to variables.
      variable_set($key, trim($val));
    }
    $token = variable_get('media_youtube_upload_token', '');
    if (empty($token)) {
      $ytapi = new MediaYoutubeUploadYtapi();
      $redirect = $ytapi->getAuthUrl();

      // If overlay is enabled we need to close the dialog and then redirect.
      if (module_exists('overlay') && overlay_get_mode() == 'child') {
        unset($_GET['destination']);
        overlay_close_dialog($redirect);
        $form_state['redirect'] = FALSE;
      } 
      else {
        $form_state['redirect'] = $redirect;
      }
    }

  }
  elseif ($form_state['triggering_element']['#id'] === 'edit-reset' && $access_application) {
    variable_del('media_youtube_upload_token');
    drupal_set_message(t('The access token has been deleted. Please re-save the application with the right credentials.'), 'warning');
  }
}
