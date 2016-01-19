<?php
/**
 * @file
 * Defines the administration pages for the webclient_proxy module.
 */

/*
 * Page callback functions.
 */

/**
 * Page callback for 'admin/config/webclient/proxy'.
 */
function webclient_proxy_admin_overview_page() {
  // Construct overview table header.
  $header = array(t('Name'), t('Operations'));
  // Construct the empty text.
  $empty_text = t('No proxies available. !add_link', array('!add_link' => l(t('Add new proxy'), 'admin/config/webclient/proxy/add')));
  // Initialize the rows array.
  $rows = array();
  // Iterate through the proxies.
  foreach (webclient_proxy_get_proxies() as $proxy) {
    // Generate the operation links.
    $operations = $proxy->machine_name === WEBCLIENT_PROXY_DRUPAL_PROXY ? '' : theme('links__ctools_dropbutton', array(
      'title' => t('Edit'),
      'links' => array(
        array('title' => t('Edit'), 'href' => "admin/config/webclient/proxy/edit/{$proxy->machine_name}"),
        array('title' => t('Delete'), 'href' => "admin/config/webclient/proxy/delete/{$proxy->machine_name}"),
      ),
    ));
    // Initialize the row variable.
    $rows[] = array(
      '<div class="name">' . check_plain($proxy->name) . '</div><div class="description">' . check_plain($proxy->description) . '</div>',
      $operations,
    );
  }
  // Get the path to the CSS folder.
  $css_path = drupal_get_path('module', 'webclient_proxy') . '/css';
  // Add the required CSS.
  drupal_add_css("{$css_path}/webclient_proxy.ctools.css");
  drupal_add_css("{$css_path}/webclient_proxy.overview.css");
  // Theme the overview table.
  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => $empty_text,
    'attributes' => array('class' => array('webclient-proxy-overview')),
  ));
}

/**
 * Page callback for 'admin/config/webclient/proxy/edit/%'.
 */
function webclient_proxy_admin_edit_proxy_page($proxy) {
  // Redirect to 404 page if argument is invalid.
  _webclient_proxy_redirect_to_page_not_found_on_invalid_proxy($proxy);
  // Argument valid, get and return the form.
  return drupal_get_form('webclient_proxy_admin_edit_proxy_form', $proxy);
}

/**
 * Page callback for 'admin/config/webclient/proxy/delete/%'.
 */
function webclient_proxy_admin_delete_proxy_page($proxy) {
  // Redirect to 404 page if argument is invalid.
  _webclient_proxy_redirect_to_page_not_found_on_invalid_proxy($proxy);
  // Argument valid, get and return the form.
  return drupal_get_form('webclient_proxy_admin_delete_proxy_form', $proxy);
}

/*
 * Form generator functions.
 */

/**
 * Form generator function for 'admin/config/webclient/proxy/edit/%'.
 */
function webclient_proxy_admin_edit_proxy_form($form, &$form_state, $proxy = NULL) {
  // Check if the proxy object is not set.
  if (!isset($proxy)) {
    // Initialize the proxy argument with the defaults.
    $proxy = (object) webclient_proxy_default_proxy();
  }
  // Check if the user is trying to edit the Drupal proxy.
  elseif ($proxy->machine_name == WEBCLIENT_PROXY_DRUPAL_PROXY) {
    // Notify the user that these settings need to be changed in the Drupal
    // settings file.
    drupal_set_message(t('Changes to the Drupal proxy need to be performed in the Drupal settings file.'), 'warning');
    // Redirect the user to the overview.
    drupal_goto('admin/config/webclient/proxy');
  }
  // Check if we are in edit mode.
  $edit_mode = !empty($proxy->machine_name);
  // Start constructing the form based on the proxy object.
  $form = array();
  // Add the current proxy object to the form.
  $form['old_proxy'] = array(
    '#type' => 'value',
    '#value' => $proxy,
  );
  // Add proxy form element container, this container will be used as form
  // representation of our proxy object.
  $form['new_proxy'] = array(
    '#tree' => TRUE,
  );
  // Will contain the human readable name.
  $form['new_proxy']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('The human readable name of the proxy.'),
    '#default_value' => $proxy->name,
    '#maxlength' => 64,
    '#required' => TRUE,
  );
  // Will contain the machine readable name.
  $form['new_proxy']['machine_name'] = array(
    '#type' => 'machine_name',
    '#maxlength' => 64,
    '#default_value' => $proxy->machine_name,
    '#disabled' => $edit_mode,
    '#machine_name' => array(
      'source' => array('new_proxy', 'name'),
      'exists' => 'webclient_proxy_machine_name_exists',
    ),
  );
  // Will contain the proxy description.
  $form['new_proxy']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('Description about the proxy which will be shown in the proxy overview list.'),
    '#default_value' => $proxy->description,
    '#maxlength' => 255,
  );
  // Create settings container which will hold all proxy settings.
  $form['new_proxy']['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Proxy settings'),
    '#collapsible' => TRUE,
    '#collapsed' => $edit_mode,
  );
  // Add the proxy URL setting.
  $form['new_proxy']['settings']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Proxy URL'),
    '#description' => t('The HTTP url of the proxy server, e.g. http://localhost:8080.'),
    '#default_value' => $proxy->settings['url'],
    '#required' => TRUE,
    '#element_validate' => array('webclient_proxy_element_validate_url'),
  );
  // Add the proxy user agent setting.
  $form['new_proxy']['settings']['user_agent'] = array(
    '#type' => 'textfield',
    '#title' => t('User agent'),
    '#description' => t('Enter the user agent that is required by the proxy. Leave empty if no user agent should be applied.'),
    '#default_value' => $proxy->settings['user_agent'],
  );
  // Add the proxy authentication setting.
  $form['new_proxy']['settings']['authentication'] = array(
    '#type' => 'select',
    '#title' => t('Authentication'),
    '#description' => t('Choose the authentication which is supported by the proxy server.'),
    '#options' => array(
      WEBCLIENT_PROXY_AUTHENTICATION_DISABLED => t('Disabled'),
      CURLAUTH_BASIC => t('Basic'),
      CURLAUTH_NTLM => t('NTLM'),
      WEBCLIENT_PROXY_AUTHENTICATION_AUTO => t('Automatic'),
    ),
    '#default_value' => $proxy->settings['authentication'],
  );
  // Add the username setting.
  $form['new_proxy']['settings']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => $proxy->settings['username'],
    '#dependency' => array(
      'edit-new-proxy-settings-authentication' => array(
        CURLAUTH_BASIC,
        CURLAUTH_NTLM,
        WEBCLIENT_PROXY_AUTHENTICATION_AUTO,
      ),
    ),
  );
  // Add the password setting.
  $form['new_proxy']['settings']['password'] = array(
    // TODO: Change to password and add support for dependency.
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => $proxy->settings['password'],
    '#dependency' => array(
      'edit-new-proxy-settings-authentication' => array(
        CURLAUTH_BASIC,
        CURLAUTH_NTLM,
        WEBCLIENT_PROXY_AUTHENTICATION_AUTO,
      ),
    ),
  );
  // Submit and cancel button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $edit_mode ? t('Save') : t('Add'),
  );
  $form['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/config/webclient/proxy'),
  );
  return $form;
}

/**
 * Form generator function for 'admin/config/webclient/proxy/delete/%'.
 */
function webclient_proxy_admin_delete_proxy_form($form, &$form_state, $proxy) {
  // Check if the user is trying to delete the Drupal proxy.
  if ($proxy->machine_name == WEBCLIENT_PROXY_DRUPAL_PROXY) {
    // Notify the user that these settings need to be removed in the Drupal
    // settings file.
    drupal_set_message(t('Remove the Drupal proxy from the Drupal settings file.'), 'warning');
    // Redirect the user to the overview.
    drupal_goto('admin/config/webclient/proxy');
  }
  $form = array();
  // Add the proxy object to the form.
  $form['proxy'] = array(
    '#type' => 'value',
    '#value' => $proxy,
  );
  // Create confirmation form and return result.
  return confirm_form(
      $form,
      t('Delete @proxy?', array('@proxy' => $proxy->name)),
      'admin/config/webclient/proxy',
      t('Deleting the proxy setting will remove all configuration related to this proxy. Do you want to continue?'),
      t('Delete')
  );
}

/**
 * Form generator function for 'admin/config/webclient/proxy/configure'.
 */
function webclient_proxy_admin_configure_form() {
  $form = array(); array('#tree' => TRUE);
  // Add the default proxy setting.
  $form['webclient_proxy_default_proxy'] = array(
    '#type' => 'select',
    '#title' => t('Default proxy'),
    '#description' => t('Choose the default proxy which is applied to all unsupported requests which use the WebClient API.'),
    '#options' => webclient_proxy_get_proxies_select(FALSE, TRUE),
    '#default_value' => webclient_proxy_get_default_proxy_setting(),
  );
  // Create reusable select list for all supported requests.
  $proxy = array(
    '#type' => 'select',
    '#title' => t('Proxy'),
    '#description' => t('Which proxy needs to be applied for this supported request.'),
    '#options' => webclient_proxy_get_proxies_select(TRUE, TRUE),
  );
  // Create exclude hosts container.
  $exceptions = array(
    '#type' => 'fieldset',
    '#title' => t('Exceptions'),
    '#description' => t('List of hostnames that can be accessed directly, not via proxy. Hostnames listed overrule the supported WebClient request settings.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'webclient_proxy_host_exceptions' => array(
      '#type' => 'textarea',
      '#description' => t('Enter one hostname per line. The \'*\' character is a wildcard. Example hostnames are !localhost for excluding local requests and !drupal_wildcard for excluding all requests to Drupal subdomains.', array('!localhost' => '<em class="placeholder">localhost</em>', '!drupal_wildcard' => '<em class="placeholder">*.drupal.org</em>')),
      '#default_value' => variable_get('webclient_proxy_host_exceptions', NULL),
    ),
  );
  // Get the proxy exceptions from the Drupal settings file.
  $drupal_proxy_exceptions = variable_get('proxy_exceptions', array());
  // Initialize the rows as an empty array.
  $rows = array();
  // Iterate through the drupal proxy exceptions.
  foreach ($drupal_proxy_exceptions as $hostname) {
    // Add the hostname to the rows.
    $rows[] = array($hostname);
  }
  // Build the Drupal settings exception list.
  $exceptions['drupal_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Drupal settings'),
    '#description' => t('List of hostnames which will be excluded from using a proxy as defined by the Drupal settings file. These hostnames do not support wildcards and will be evaluated if none of the above exceptions match.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'list' => array(
      '#type' => 'markup',
      '#markup' => theme('table', array('header' => array(t('Hostname')), 'rows' => $rows, 'empty' => t('No hostnames configured in the Drupal settings file.'))),
    ),
  );
  // Add the exceptions container.
  $form['exceptions'] = $exceptions;
  // Create supported requests container.
  $supported_requests = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Supported WebClient requests'),
    '#description' => t('A supported WebClient request uses a specific tag to identify itself. Configure which proxy needs to be used when a supported request is found.'),
  );
  // Iterate through supported requests.
  foreach (webclient_proxy_get_supported_requests() as $request_tag => $request_info) {
    // Get the setting for the current request tag.
    $setting = webclient_proxy_get_supported_request_setting($request_tag);
    // Create container element with the request tag name.
    $container = array(
      '#type' => 'fieldset',
      '#title' => check_plain($request_info['name']),
      '#description' => check_plain($request_info['description']),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'proxy' => $proxy + array('#default_value' => $setting),
    );
    // Add the container to the form.
    $supported_requests[$request_tag] = $container;
  }
  // Add supported requests container.
  $form['webclient_proxy_supported_requests'] = $supported_requests;
  // Create and return the settings form.
  return system_settings_form($form);
}

/*
 * Form validation handlers.
 */

/**
 * Form element validation for absolute URLs.
 */
function webclient_proxy_element_validate_url($element, &$form_state) {
  $url = $element['#value'];
  // Check if the URL is not empty and invalid.
  if (!empty($url) && !valid_url($url, TRUE)) {
    // Raise form error: Invalid absolute URL format.
    form_error($element, t('@field must contain an absolute URL.', array('@field' => $element['#title'])));
  }
}

/**
 * Form validation handler for 'webclient_proxy_admin_edit_proxy_form'.
 */
function webclient_proxy_admin_edit_proxy_form_validate($form, &$form_state) {
  // Evaluate the authentication method.
  switch ($form_state['values']['new_proxy']['settings']['authentication']) {
    case CURLAUTH_BASIC:
    case CURLAUTH_NTLM:
    case WEBCLIENT_PROXY_AUTHENTICATION_AUTO:
      // Check if the username is missing.
      if (empty($form_state['values']['new_proxy']['settings']['username'])) {
        // Get the username element.
        $username_element = $form['new_proxy']['settings']['username'];
        // Raise form error for the username field: username required.
        form_set_error('new_proxy][settings][username', t('@field is required because authentication is enabled.', array('@field' => $username_element['#title'])));
      }
      // Check if the password is missing.
      if (empty($form_state['values']['new_proxy']['settings']['password'])) {
        // Get the password element.
        $password_element = $form['new_proxy']['settings']['password'];
        // Raise form error for the password field: password required.
        form_set_error('new_proxy][settings][password', t('@field is required because authentication is enabled.', array('@field' => $password_element['#title'])));
      }
  }
}

/*
 * Form submit handlers.
 */

/**
 * Form submit handler for 'webclient_proxy_admin_edit_proxy_form'.
 */
function webclient_proxy_admin_edit_proxy_form_submit($form, &$form_state) {
  // Get the old proxy object.
  $old_proxy = $form_state['values']['old_proxy'];
  // Get the new proxy and convert to an object.
  $proxy = (object) $form_state['values']['new_proxy'];
  // Check if the old proxy object has a pid.
  if (isset($old_proxy->pid)) {
    // Set the pid property of the new proxy object.
    $proxy->pid = $old_proxy->pid;
  }
  // Try to save the proxy object.
  if (($save_state = webclient_proxy_save_proxy($proxy))) {
    // Evaluate the save state.
    switch ($save_state) {
      case SAVED_NEW:
        // Notify the user that the proxy was created.
        drupal_set_message(t('The proxy @name was created.', array('@name' => $proxy->name)), 'status', FALSE);
        break;

      case SAVED_UPDATED:
        // Notify the user that the proxy was saved.
        drupal_set_message(t('The proxy @name was saved.', array('@name' => $proxy->name)), 'status', FALSE);
        break;

    }
    // Redirect to overview page.
    $form_state['redirect'] = 'admin/config/webclient/proxy';
  }
  else {
    // Notify the user that the proxy was not created.
    drupal_set_message(t('Could not create proxy. Contact your administrator if the problem persists.'), 'error', FALSE);
    // Rebuild the form with current form state.
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Form submit handler for 'webclient_proxy_admin_delete_proxy_form'.
 */
function webclient_proxy_admin_delete_proxy_form_submit($form, &$form_state) {
  // Get the proxy object.
  $proxy = $form_state['values']['proxy'];
  // Try to delete the proxy object.
  if (webclient_proxy_delete_proxy($proxy)) {
    // Notify the user that the proxy was deleted.
    drupal_set_message(t('The proxy @name was deleted.', array('@name' => $proxy->name)), 'status', FALSE);
  }
  else {
    // Notify the user that the proxy could not be removed.
    drupal_set_message(t('Could not delete the proxy @name. Contact your administrator if this problem persists.', array('@name' => $proxy->name)), 'error', FALSE);
  }
  // Redirect to the WebClient proxy overview page.
  $form_state['redirect'] = 'admin/config/webclient/proxy';
}

/*
 * Private helper functions.
 */

/**
 * Redirect to 404 page if the proxy is invalid.
 *
 * @param object $proxy
 *   A proxy object.
 */
function _webclient_proxy_redirect_to_page_not_found_on_invalid_proxy($proxy) {
  // Check if the proxy argument is invalid.
  if (!isset($proxy) || !is_object($proxy)) {
    // Redirect to the 404 page.
    drupal_not_found();
  }
}
