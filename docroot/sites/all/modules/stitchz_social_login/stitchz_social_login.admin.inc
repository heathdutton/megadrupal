<?php
/**
 * @file
 * General admin functions.
 */

/**
 * Create the list of identity providers for display.
 *
 * @param string $dnsalias
 *   App url as provided by Stitchz.
 * @param array $providers
 *   Json string of identity providers.
 * @param string $apiversion
 *   The version of the Stitchz api being used.
 * @param string $apikey
 *   Unique client id provided by Stitchz.
 * @param string $redirect_uri
 *   Callback url where Stitchz will send authenticated response.
 * @param string $scope
 *   Application scope as provided by Stitchz.
 * @param string $notes
 *   The custom message to add to the bottom of the div.
 * @param string $list_id
 *   The html ID name to give to the resulting div.
 *
 * @return array
 *   A render array of identity providers.
 */
function stitchz_social_login_admin_format_provider_list($dnsalias, array $providers, $apiversion, $apikey, $redirect_uri, $scope, $notes, $list_id = NULL) {

  $restricted_resource_url = rtrim($dnsalias, '/') . '/';

  $providers_array = array();
  if (!empty($providers) && isset($providers)) {
    foreach ($providers as $provider) {
      if ($provider['IsActive'] === TRUE) {
        $provider_name = check_plain($provider['Name']);

        // Check which api version to call.
        if ($apiversion == '1') {
          $authenticate_url = url($restricted_resource_url . str_replace(' ', '', $provider_name) . '/v1/Authenticate',
          array(
            'query' => array(
              'ApiKey' => urlencode($apikey),
              'ReturnUrl' => $redirect_uri,
            ),
            'external' => TRUE,
          ));
        }
        else {
          // Generate a random string.
          $state = drupal_substr(drupal_get_token(), 0, 10);
          $authenticate_url = url($restricted_resource_url . str_replace(' ', '', $provider_name) . '/v2/Authenticate',
          array(
            'query' => array(
              'client_id' => urlencode($apikey),
              'redirect_uri' => $redirect_uri,
              'scope' => urlencode($scope),
              'state' => $state,
              'response_type' => 'code',
            ),
            'external' => TRUE,
          ));
        }

        array_push($providers_array, array(
          '#theme' => 'stitchz_social_login_addin_provider_block',
          '#provider_name' => $provider_name,
          '#authentication_url' => $authenticate_url,
        ));
      }
    }
  }
  else {
    array_push($providers_array, array(
      '#markup' => t('You currently have no social login identities connected to your account.'),
      '#prefix' => '<li>',
      '#suffix' => '</li>',
    ));
  }

  $output = array(
    'content' => array(
      'header' => array(
        '#markup' => t('Sign In With:'),
        '#prefix' => '<div>',
        '#suffix' => '</div>',
      ),
      'container' => array(
        '#prefix' => '<ul class="stitchz_social_login_provider_list"' . (!empty($list_id) ? ' id="' . drupal_html_id($list_id) . '"' : '') . '>',
        '#suffix' => '</ul>',
        'child' => $providers_array,
      ),
      'notes' => array(
        '#prefix' => '<div class="stitchz_social_login_provider_list_notes">',
        '#suffix' => '</div>',
        '#markup' => check_markup($notes, 'filtered_html'),
      ),
      'footer' => array(
        '#prefix' => '<div class="icon_footer">',
        '#suffix' => '</div>',
        'child' => array(
          '#prefix' => '<div class="left">',
          '#suffix' => '</div>',
          'child' => array(
            'child1' => array(
              '#markup' => l(t('Powered by') . ' ', 'http://www.stitchz.net/',
              array(
                'attributes' => array(
                  'class' => array('poweredby'),
                ),
              )),
            ),
            'child2' => array(
              '#markup' => l(theme('image', array('path' => drupal_get_path('module', 'stitchz_social_login') . '/images/logo-55x40.png')),
              'http://www.stitchz.net/',
              array(
                'html' => TRUE,
                'attributes' => array(
                  'class' => array('poweredby'),
                  'target' => '_blank',
                ),
              )
              ),
            ),
          ),
        ),
      ),
      'clear' => array(
        '#markup' => '<div style="clear:both;"></div>',
      ),
    ),
  );

  return $output;
}

/**
 * Form validation for stitchz_social_login_config_settings_form_submit().
 *
 * Checks if the given App Url is a valid stitchz.net dns alias/subdomain.
 *
 * @see stitchz_social_login_config_settings_form_submit()
 */
function stitchz_social_login_config_settings_form_validate($form, &$form_state) {
  $domain = trim($form_state['values']['domain']);

  if (!empty($domain)) {
    $arr = parse_url($domain);
    if (isset($arr['host'])) {
      $host = $arr['host'];

      // Check that the API end point is valid.
      if (!preg_match('#^(?:[^.]+\.)*stitchz\.net$#i', $host, $arr)) {
        form_set_error('domain', t('The App Url is not a valid Stitchz subdomain.'));
      }
    }
    else {
      form_set_error('domain', t('App Url is invalid, enter a valid Url and try again.'));
    }
  }
  else {
    form_set_error('domain', t('App Url is required, enter a valid Url and try again.'));
  }
}

/**
 * Page callback: config page.
 *
 * @see stitchz_social_login_menu()
 */
function stitchz_social_login_config_settings_form($form, &$form_state) {

  // Add the necessary styles.
  $form['#attached']['css'][] = array(
    'data' => drupal_get_path('module', 'stitchz_social_login') . '/styles/stitchz_social_login.css',
    'type' => 'file',
  );
  $form['#attached']['css'][] = array(
    'data' => '//cdnjs.cloudflare.com/ajax/libs/font-awesome/4.1.0/css/font-awesome.min.css',
    'type' => 'external',
  );

  // Get configuration settings from the database.
  $config = stitchz_social_login_config_settings_get_settings();

  $form['stitchz_social_login_config_settings_form'] = array(
    '#id' => drupal_html_id('stitchz_social_login_config_settings_form_fieldset'),
    '#type' => 'fieldset',
    '#title' => t('Stitchz Login API Settings'),
  );

  $form['stitchz_social_login_config_settings_form']['instructions'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => drupal_html_id('stitchz_social_login_config_settings_form_instructions'),
    ),
  );

  $form['stitchz_social_login_config_settings_form']['instructions']['content'] = array(
    '#markup' => t('Connect your Drupal site with Stitchz Login by completing the fields below, sync your providers list, then click Save. The below information should be copied directly from your <a href="@url">Stitczh Login Application Settings</a>.', array('@url' => url('https://login.stitchz.net/', array('external' => TRUE)))),
  );

  if (!empty($form_state['values']['domain'])) {
    $default = $form_state['values']['domain'];
  }
  elseif (!empty($config['domain'])) {
    $default = $config['domain'];
  }
  else {
    $default = 'https://api.stitchz.net/';
  }

  $form['stitchz_social_login_config_settings_form']['domain'] = array(
    '#type' => 'textfield',
    '#title' => t('App Url'),
    '#description' => t('The App Url or subdomain of your Stitchz Login application'),
    '#maxlength' => 160,
    '#required' => TRUE,
    '#default_value' => $default,
  );

  if (!empty($form_state['values']['apikey'])) {
    $default = $form_state['values']['apikey'];
  }
  elseif (!empty($config['apikey'])) {
    $default = $config['apikey'];
  }
  else {
    $default = 'not found';
  }

  $form['stitchz_social_login_config_settings_form']['apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('ApiKey'),
    '#description' => t('Your Stitchz Login application apikey'),
    '#maxlength' => 50,
    '#required' => TRUE,
    '#default_value' => $default,
  );

  if (!empty($form_state['values']['appsecret'])) {
    $default = $form_state['values']['appsecret'];
  }
  elseif (!empty($config['appsecret'])) {
    $default = $config['appsecret'];
  }
  else {
    $default = 'not found';
  }

  $form['stitchz_social_login_config_settings_form']['appsecret'] = array(
    '#type' => 'textfield',
    '#title' => t('AppSecret'),
    '#description' => t('Your Stitchz Login application secret'),
    '#maxlength' => 150,
    '#required' => TRUE,
    '#default_value' => $default,
  );

  if (!empty($form_state['values']['redirecturl'])) {
    $default = $form_state['values']['redirecturl'];
  }
  elseif (!empty($config['redirecturl'])) {
    $default = $config['redirecturl'];
  }
  else {
    $default = url('/stitchz_social_login/auth',
      array(
        'absolute' => TRUE,
      )
    );
  }

  $form['stitchz_social_login_config_settings_form']['redirecturl'] = array(
    '#type' => 'textfield',
    '#title' => t('Return Url'),
    '#description' => t('This site&#39;s web address where Stitchz Login will send a response to. The URL is your Drupal website&#39;s full web address plus the Stitchz Drupal end point (&#39;/stitchz_social_login/auth&#39;), i.e. @example_url', array(
      '@example_url' => url('/stitchz_social_login/auth',
        array(
          'absolute' => TRUE,
        )
      ),
    )),
    '#maxlength' => 255,
    '#required' => TRUE,
    '#default_value' => $default,
  );

  if (!empty($form_state['values']['version'])) {
    $default = $form_state['values']['version'];
  }
  elseif (!empty($config['version'])) {
    $default = $config['version'];
  }
  else {
    $default = 'Standard Login';
  }

  $form['stitchz_social_login_config_settings_form']['version'] = array(
    '#type' => 'select',
    '#title' => t('API Version'),
    '#description' => t('The version of api call to authenticate the user'),
    '#required' => TRUE,
    '#options' => array(
      '1' => t('Standard Login'),
      '2' => t('OAuth 2 Login'),
    ),
    '#default_value' => $default,
  );

  if (!empty($form_state['values']['syncd_providers'])) {
    $default = $form_state['values']['syncd_providers'];
  }
  elseif (!empty($config['syncd_providers'])) {
    $default = $config['syncd_providers'];
  }
  else {
    $default = '';
  }

  $form['stitchz_social_login_config_settings_form']['syncd_providers'] = array(
    '#type' => 'hidden',
    '#title' => t('syncd_providers'),
    '#default_value' => $default,
    '#prefix' => '<div id="hidden_providers">',
    '#suffix' => '</div>',
  );

  if (!empty($form_state['values']['scope'])) {
    $default = $form_state['values']['scope'];
  }
  elseif (!empty($config['scope'])) {
    $default = $config['scope'];
  }
  else {
    $default = 'Basic';
  }

  $form['stitchz_social_login_config_settings_form']['scope'] = array(
    '#type' => 'hidden',
    '#title' => t('scope'),
    '#required' => TRUE,
    '#default_value' => $default,
    '#prefix' => '<div id="hidden_scope">',
    '#suffix' => '</div>',
  );

  /********************
   Begin configure Social Login providers.
   ********************/

  $form['stitchz_social_login_config_settings_provider_list'] = array(
    '#id' => drupal_html_id('stitchz_social_login_config_settings_provider_list'),
    '#type' => 'fieldset',
    '#title' => t('Stitchz Login Provider List'),
  );

  $form['stitchz_social_login_config_settings_provider_list']['instructions'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => drupal_html_id('stitchz_social_login_provider_list_instructions'),
    ),
  );

  $form['stitchz_social_login_config_settings_provider_list']['instructions']['content'] = array(
    '#markup' => t('After your configuration settings have been set, click the button below to test your settings and sync your identity providers from your Stitchz Login application here. Don&#39;t forget to click "Save" before leaving this page.'),
  );

  $form['stitchz_social_login_config_settings_provider_list']['providers'] = array(
    '#type' => 'button',
    '#value' => t('Sync Providers'),
    '#ajax' => array(
      'callback' => 'stitchz_social_login_config_settings_provider_list_api_call',
      'wrapper' => 'stitchz_social_login_provider_list_container',
    ),
  );

  if (!empty($config['syncd_providers'])) {
    $form['stitchz_social_login_config_settings_provider_list']['clear_providers'] = array(
      '#type' => 'button',
      '#value' => t('Clear Providers'),
      '#ajax' => array(
        'callback' => 'stitchz_social_login_config_settings_clear_providers',
        'wrapper' => 'stitchz_social_login_provider_list_container',
      ),
    );
  }

  $form['stitchz_social_login_config_settings_provider_list']['provider_list'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => drupal_html_id('stitchz_social_login_provider_list_container'),
    ),
  );

  $form['stitchz_social_login_config_settings_provider_list_empty_block'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => drupal_html_id('stitchz_social_login_provider_list_empty_block'),
      'style' => 'clear:both;',
    ),
  );

  if (!empty($config['syncd_providers'])) {
    $json_providers = drupal_json_decode(str_replace('\'', '"', $config['syncd_providers']));
    $providers = (isset($json_providers['Providers']) ? $json_providers['Providers'] : array());
    $scope = $json_providers['Scope'];
    $apikey = $config['apikey'];
    $version = $config['version'];
    $redirecturl = $config['redirecturl'];
    $notes = $config['notes'];
    $default = stitchz_social_login_admin_format_provider_list($config['domain'], $providers, $version, $apikey, $redirecturl, $scope, $notes);
  }
  else {
    $default = array(
      '#markup' => t('Your providers list is currently empty. Click "Sync Providers" to pull your list of configured identity providers from your Stitchz Login application.'),
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
  }

  $form['stitchz_social_login_config_settings_provider_list']['provider_list']['content'] = $default;

  $form['stitchz_social_login_config_settings_provider_list_empty_block'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => drupal_html_id('stitchz_social_login_config_settings_provider_list_empty_block'),
      'style' => 'clear:both;',
    ),
  );

  /********************
   Begin configure addin display settings.
   ********************/

  $form['stitchz_social_login_config_settings_display'] = array(
    '#id' => drupal_html_id('stitchz_social_login_config_settings_display'),
    '#type' => 'fieldset',
    '#title' => t('Stitchz Login Addin Settings'),
  );

  if (!empty($form_state['values']['theme_version'])) {
    $default = $form_state['values']['theme_version'];
  }
  elseif (!empty($config['theme_version'])) {
    $default = $config['theme_version'];
  }
  else {
    $default = 'Basic';
  }

  $form['stitchz_social_login_config_settings_display']['theme_version'] = array(
    '#type' => 'select',
    '#title' => t('Theme Version'),
    '#description' => t('The version to display the Social Login icons in'),
    '#required' => TRUE,
    '#options' => array(
      '1' => t('Basic'),
    ),
    '#default_value' => $default,
  );

  if (!empty($form_state['values']['enable_user_login_screen'])) {
    $default = $form_state['values']['enable_user_login_screen'];
  }
  elseif ($config['enable_user_login_screen'] !== NULL) {
    $default = $config['enable_user_login_screen'];
  }
  else {
    $default = 0;
  }

  $form['stitchz_social_login_config_settings_display']['enable_user_login_screen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable on User_Login Screen?'),
    '#description' => t('A boolean value indicating whether or not to show social login icons on the user_login form'),
    '#required' => FALSE,
    '#default_value' => $default,
  );

  if (!empty($form_state['values']['enable_user_login_block_screen'])) {
    $default = $form_state['values']['enable_user_login_block_screen'];
  }
  elseif ($config['enable_user_login_block_screen'] !== NULL) {
    $default = $config['enable_user_login_block_screen'];
  }
  else {
    $default = 0;
  }

  $form['stitchz_social_login_config_settings_display']['enable_user_login_block_screen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable on User_Login_Block Screen?'),
    '#description' => t('A boolean value indicating whether or not to show social login icons on the user_login_block form'),
    '#required' => FALSE,
    '#default_value' => $default,
  );

  if (!empty($form_state['values']['enable_user_register_form_screen'])) {
    $default = $form_state['values']['enable_user_register_form_screen'];
  }
  elseif ($config['enable_user_register_form_screen'] !== NULL) {
    $default = $config['enable_user_register_form_screen'];
  }
  else {
    $default = 0;
  }

  $form['stitchz_social_login_config_settings_display']['enable_user_register_form_screen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable on User_Registration Screen?'),
    '#description' => t('A boolean value indicating whether or not to show social login icons on the user_registration form'),
    '#required' => FALSE,
    '#default_value' => $default,
  );

  if (!empty($form_state['values']['enable_comment_form_screen'])) {
    $default = $form_state['values']['enable_comment_form_screen'];
  }
  elseif ($config['enable_comment_form_screen'] !== NULL) {
    $default = $config['enable_comment_form_screen'];
  }
  else {
    $default = 0;
  }

  $form['stitchz_social_login_config_settings_display']['enable_comment_form_screen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable on Comment_Form Screen?'),
    '#description' => t('A boolean value indicating whether or not to show social login icons on the comment_form form'),
    '#required' => FALSE,
    '#default_value' => $default,
  );

  if (!empty($form_state['values']['notes'])) {
    $default = $form_state['values']['notes'];
  }
  elseif (!empty($config['notes'])) {
    $default = $config['notes'];
  }
  else {
    $default = '';
  }

  $form['stitchz_social_login_config_settings_display']['notes'] = array(
    '#type' => 'textfield',
    '#title' => t('Social Login Notes'),
    '#description' => t('A short description or note displayed under the social login icons (255 characters or less)'),
    '#maxlength' => 255,
    '#required' => FALSE,
    '#default_value' => $default,
  );

  $form['stitchz_social_login_config_settings_display_empty_block'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => drupal_html_id('stitchz_social_login_config_settings_display_empty_block'),
      'style' => 'clear:both;',
    ),
  );

  /********************
   Begin form submit.
   ********************/

  $form['#submit'][] = 'stitchz_social_login_config_settings_form_submit';
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}
