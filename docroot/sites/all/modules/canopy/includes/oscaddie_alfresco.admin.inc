<?php

/**
 * @file
 * Provides the Admin UI to help manage the connection between
 * Drupal and Alfresco.
 */

/**
 * Callback to display the osCaddie Alfresco Admin Dashboard.
 */
function oscaddie_alfresco__admin__dashboard() {
  drupal_add_css(drupal_get_path('module', 'oscaddie_alfresco') . '/css/stylesheets/oscaddie_alfresco-admin.css');
  $sections = oscaddie_alfresco__admin__sections_available();
  $content = theme('oscaddie_alfresco_admin_content_dashboard', array('sections' => $sections));

  return theme('oscaddie_alfresco_admin', array('content' => $content));
}

/**
 * Callback to display the osCaddie Alfresco Portal Configuration.
 */
function oscaddie_alfresco__admin__portal() {
  drupal_add_css(drupal_get_path('module', 'oscaddie_alfresco') . '/css/stylesheets/oscaddie_alfresco-admin.css');
  drupal_add_js(drupal_get_path('module', 'oscaddie_alfresco') . '/js/oscaddie_alfresco.js', array('scope' => 'footer'));
  $sections = oscaddie_alfresco__admin__sections_available();

  // Get all existing Alfresco sites.
  $alfresco_settings = variable_get('oscaddie_alfresco_settings_alfresco', array());

  $alfresco_header = array(
    t("Host"),
    t("Port"),
    t("Service Endpoint"),
    t("Status"),
  );

  if (!empty($alfresco_settings)) {
    $alfresco_rows[] = array(
      'data' => array(
        array(
          'data' => l($alfresco_settings['address'], 'admin/oscaddie_alfresco/portal/alfresco/edit'),
        ),
        array(
          'data' => $alfresco_settings['port'],
        ),
        array(
          'data' => $alfresco_settings['endpoint'],
        ),
        array(
          'data' => '<span class="status"><font color="green">Online</font></span>',
        ),
      ),
    );
  }
  else {
    $alfresco_rows[] = array(
      'data' => array(
        'data' => array(
          'data' => t("You do not have any Alfresco server added."),
          'colspan' => 4,
        ),
      ),
    );
  }

  $alfresco_table = (object) array(
    'content' => theme('table', array('header' => $alfresco_header, 'rows' => $alfresco_rows)),
    'buttons' => array(
      'add' => empty($alfresco_settings) ? TRUE : FALSE,
    ),
  );

  // Get all Drupal settings.
  $drupal_header = array(t("Site Name"), t("Description"), t("Version"));
  $drupal_settings = variable_get('oscaddie_alfresco_settings_drupal', array());

  if (!empty($drupal_settings)) {
    $drupal_rows[] = array(
      'data' => array(
        l($drupal_settings['name'], 'admin/oscaddie_alfresco/portal/drupal/edit'),
        $drupal_settings['description'],
        VERSION,
      ),
    );
  }
  else {
    $drupal_rows[] = array(
      'data' => array(
        'data' => array(
          'data' => t("You do not have any Drupal site added."),
          'colspan' => 3,
        ),
      ),
    );
  }

  $drupal_table = (object) array(
    'content' => theme('table', array('header' => $drupal_header, 'rows' => $drupal_rows)),
    'buttons' => array('add' => empty($drupal_settings) && !empty($alfresco_settings) ? TRUE : FALSE),
  );

  $table = theme('oscaddie_alfresco_admin_content_portal', array('alfresco' => $alfresco_table, 'drupal' => $drupal_table));
  $sidebar = theme('oscaddie_alfresco_admin_sidebar', array('sections' => $sections));
  $content = theme('oscaddie_alfresco_admin_content', array('sidebar' => $sidebar, 'content' => $table));

  return theme('oscaddie_alfresco_admin', array('content' => $content));
}

/**
 * Callback to display the osCaddie Alfresco Portal Configuration Add/Edit form for Alfresco.
 */
function oscaddie_alfresco__admin__portal_afresco($server = NULL) {
  drupal_add_css(drupal_get_path('module', 'oscaddie_alfresco') . '/css/stylesheets/oscaddie_alfresco-admin.css');

  $sections = oscaddie_alfresco__admin__sections_available();
  $form = drupal_get_form('oscaddie_alfresco__admin__portal_alfresco_modify');
  $sidebar = theme('oscaddie_alfresco_admin_sidebar', array('sections' => $sections));
  $content = theme('oscaddie_alfresco_admin_content', array('sidebar' => $sidebar, 'content' => render($form)));

  return theme('oscaddie_alfresco_admin', array('content' => $content));
}

/**
 * Callback to display the osCaddie Alfresco Portal Configuration Add/Edit form for Drupal.
 */
function oscaddie_alfresco__admin__portal_drupal($site = NULL) {
  drupal_add_css(drupal_get_path('module', 'oscaddie_alfresco') . '/css/stylesheets/oscaddie_alfresco-admin.css');

  $sections = oscaddie_alfresco__admin__sections_available();
  $form = drupal_get_form('oscaddie_alfresco__admin__portal_drupal_modify');
  $sidebar = theme('oscaddie_alfresco_admin_sidebar', array('sections' => $sections));
  $content = theme('oscaddie_alfresco_admin_content', array('sidebar' => $sidebar, 'content' => render($form)));

  return theme('oscaddie_alfresco_admin', array('content' => $content));
}

/**
 * Drupal form to handle Add or Edit Alfresco server settings.
 */
function oscaddie_alfresco__admin__portal_alfresco_modify() {
  $settings = variable_get('oscaddie_alfresco_settings_alfresco', array());

  $form['#prefix'] = '<div id="portal-alfresco"><h3>' . t("Alfresco") . '</h3>';
  $form['#suffix'] = '</div>';

  // Server Address.
  $form['oscaddie_alfresco_settings_alfresco'] = array(
    '#tree' => TRUE,
  );

  // Server Port.
  $form['oscaddie_alfresco_settings_alfresco']['address'] = array(
    '#type' => 'textfield',
    '#title' => t("Server address"),
    '#description' => t("Address of the Alfresco server you are connecting to, e.g., localhost or IP address. Do not include http:// or leading slashes."),
    '#required' => TRUE,
    '#size' => 48,
    '#default_value' => !empty($settings['address']) ? $settings['address'] : '127.0.0.1',
  );

  $form['oscaddie_alfresco_settings_alfresco']['port'] = array(
    '#type' => 'textfield',
    '#title' => t("Server port"),
    '#description' => t("Server port of the Alfresco server, if any. Leave blank if none available."),
    '#size' => 48,
    '#default_value' => !empty($settings['port']) ? $settings['port'] : '8080',
  );

  // Server service endpoint.
  $form['oscaddie_alfresco_settings_alfresco']['endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t("Server service endpoint"),
    '#description' => t("Service endpoint that Alfresco will service any osCaddie Alfresco requests. E.g. 'alfresco/service'. Do not include leading or trailing slashes."),
    '#size' => 48,
    '#default_value' => !empty($settings['endpoint']) ? $settings['endpoint'] : 'alfresco/service',
    '#required' => TRUE,
  );

  $form['oscaddie_alfresco_settings_alfresco']['username'] = array(
    '#type' => 'textfield',
    '#title' => t("API username"),
    '#description' => t("API credential used to acquire a token for request any Alfresco services."),
    '#size' => 48,
    '#default_value' => !empty($settings['username']) ? $settings['username'] : '',
    '#required' => TRUE,
  );

  $form['oscaddie_alfresco_settings_alfresco']['password'] = array(
    '#type' => 'textfield',
    '#title' => t("API secret"),
    '#description' => t("API credential used to acquire a token for request any Alfresco services."),
    '#size' => 48,
    '#default_value' => !empty($settings['password']) ? $settings['password'] : '',
    '#required' => TRUE,
  );

  $form['buttons'] = array(
    '#tree' => TRUE,
    '#prefix' => '<div class="buttons">',
    '#suffix' => '</div>',
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('save'),
    '#attributes' => array('class' => array('button', 'ok')),
    '#weight' => 0,
  );

  $form['buttons']['cancel'] = array(
    '#type' => 'item',
    '#value' => l(t("Cancel"), 'admin/oscaddie_alfresco/portal', array('attributes' => array('class' => array('button', 'cancel')))),
    '#weight' => 1,
  );

  $form['buttons']['delete'] = array(
    '#type' => 'button',
    '#value' => t('delete'),
    '#executes_submit_callback' => TRUE,
    '#attributes' => array('class' => array('button', 'delete')),
    '#weight' => 2,
  );

  return $form;
}

/**
 * Submit handler for oscaddie_alfresco__admin__portal_alfresco_modify() form.
 */
function oscaddie_alfresco__admin__portal_alfresco_modify_submit($form, &$form_state) {
  form_state_values_clean($form_state);

  if ($form_state['input']['op'] == 'save') {
    variable_set('oscaddie_alfresco_settings_alfresco', $form_state['values']['oscaddie_alfresco_settings_alfresco']);

    // Attempt to login to get a Token to validate a connection establishes.
    $ticket = oscaddie_alfresco__alfresco_api__ticket_get(TRUE);

    if (!empty($ticket)) {
      drupal_set_message(t("Connection to Alfresco server established. Server settings saved."), 'status', FALSE);
    }
    else {
      drupal_set_message(t("Unable to contact Alfresco Server. Please contact your site administrator."), 'error', FALSE);
      watchdog('oscaddie_alfresco', 'Unable to contact Alfresco Server. Please contact your site administrator.', array(), WATCHDOG_ERROR);
    }
  }
  elseif ($form_state['input']['op'] == 'delete') {
    variable_del('oscaddie_alfresco_settings_alfresco');
    $_SESSION['oscaddie_alfresco_alfresco_api'] = NULL;

    drupal_set_message(t("Alfresco Server settings deleted."), 'status', FALSE);
  }

  $form_state['redirect'] = 'admin/oscaddie_alfresco/portal';
}

/**
 * Drupal form to handle Add or Edit Drupal site settings.
 */
function oscaddie_alfresco__admin__portal_drupal_modify() {
  $settings = variable_get('oscaddie_alfresco_settings_drupal', array());

  $form['#prefix'] = '<div id="portal-drupal"><h3>' . t("Drupal") . '</h3>';
  $form['#suffix'] = '</div>';

  $form['oscaddie_alfresco_settings_drupal'] = array(
    '#tree' => TRUE,
  );

  $form['oscaddie_alfresco_settings_drupal']['name'] = array(
    '#type' => 'textfield',
    '#title' => t("Site name"),
    '#description' => t("Name of this site that will be registered with Alfresco."),
    '#required' => TRUE,
    '#size' => 48,
    '#default_value' => !empty($settings['name']) ? $settings['name'] : '',
  );

  $form['oscaddie_alfresco_settings_drupal']['description'] = array(
    '#type' => 'textarea',
    '#title' => t("Site description"),
    '#description' => t("Additional information about this site."),
    '#default_value' => !empty($settings['description']) ? $settings['description'] : '',
  );

  $form['buttons'] = array(
    '#tree' => TRUE,
    '#prefix' => '<div class="buttons">',
    '#suffix' => '</div>',
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('save'),
    '#attributes' => array('class' => array('button', 'ok')),
    '#weight' => 0,
  );

  $form['buttons']['cancel'] = array(
    '#type' => 'item',
    '#value' => l(t("Cancel"), 'admin/oscaddie_alfresco/portal', array('attributes' => array('class' => array('button', 'cancel')))),
    '#weight' => 1,
  );

  $form['buttons']['delete'] = array(
    '#type' => 'button',
    '#value' => t('delete'),
    '#executes_submit_callback' => TRUE,
    '#attributes' => array('class' => array('button', 'delete')),
    '#weight' => 2,
  );

  $form['#redirect'] = 'admin/oscaddie_alfresco/portal';

  return $form;
}

/**
 * Submit handler for oscaddie_alfresco__admin__portal_alfresco_modify() form.
 */
function oscaddie_alfresco__admin__portal_drupal_modify_submit($form, &$form_state) {
  form_state_values_clean($form_state);

  if ($form_state['input']['op'] == 'save') {
    $alfresco_settings = variable_get('oscaddie_alfresco_settings_alfresco', array());

    if (empty($alfresco_settings)) {
      drupal_set_message(t("You do not have any Alfresco server added. Drupal cannot register a site with Alfresco if an Alfresco server does not exists."), 'error', FALSE);
      watchdog('oscaddie_alfresco', 'You do not have any Alfresco server added. Drupal cannot register a site with Alfresco if an Alfresco server does not exists.', array(), WATCHDOG_ERROR);
    }
    else {
      // Attempt to register the Drupal site before saving settings.
      $drupal_settings = $form_state['values']['oscaddie_alfresco_settings_drupal'];
      $registration = oscaddie_alfresco__alfresco_api__site_register($drupal_settings['name'], $drupal_settings['description']);

      if (isset($registration) && $registration == TRUE) {
        variable_set('oscaddie_alfresco_settings_drupal', $drupal_settings);
        drupal_set_message(t("Drupal site successfully added to Alfresco."), 'status', FALSE);
      }
      else {
        drupal_set_message(t("Unable to contact Alfresco Server. Please contact your site administrator."), 'error', FALSE);
        watchdog('oscaddie_alfresco', 'Unable to contact Alfresco Server. Please contact your site administrator.', array(), WATCHDOG_ERROR);
      }

      $form_state['redirect'] = 'admin/oscaddie_alfresco/portal';
    }
  }
  elseif ($form_state['input']['op'] == 'delete') {
    variable_del('oscaddie_alfresco_settings_drupal');
    drupal_set_message(t("Drupal settings deleted."), 'status', FALSE);

    $form_state['redirect'] = 'admin/oscaddie_alfresco/portal';
  }
}

/**
 * Get a list of all available osCaddie Alfresco sections.
 *
 * @return array
 *   An array of all sections with their URL, title, and if they are enabled or not.
 */
function oscaddie_alfresco__admin__sections_available() {
  $sections = array();
  $links = array();

  // Check if menu exists.
  $sql = "SELECT l.link_path FROM {menu_links} l WHERE l.link_path LIKE :path AND l.menu_name = :menu";
  $result = db_query($sql, array(':path' => db_like('admin/oscaddie_alfresco/') . '%', ':menu' => 'management'))->fetchAll();

  foreach ($result as $link) {
    $links[] = $link->link_path;
  }

  // Account administration.
  $sections[] = (object) array(
    'name' => 'account',
    'title' => t("Account Administration"),
    'path' => 'admin/oscaddie_alfresco/account',
    'enabled' => in_array('admin/oscaddie_alfresco/account', $links) ? TRUE : FALSE,
  );

  // Portal configuration.
  $sections[] = (object) array(
    'name' => 'portal',
    'title' => t("Portal Configuration"),
    'path' => 'admin/oscaddie_alfresco/portal',
    'enabled' => in_array('admin/oscaddie_alfresco/portal', $links) ? TRUE : FALSE,
  );

  // Content-Type mapping.
  $sections[] = (object) array(
    'name' => 'mapping',
    'title' => t("Content Type Mapping"),
    'path' => 'admin/oscaddie_alfresco/mapping',
    'enabled' => in_array('admin/oscaddie_alfresco/mapping', $links) ? TRUE : FALSE,
  );

  // File migration.
  $sections[] = (object) array(
    'name' => 'file',
    'title' => t("File Migration"),
    'path' => 'admin/oscaddie_alfresco/file',
    'enabled' => in_array('admin/oscaddie_alfresco/file', $links) ? TRUE : FALSE,
  );

  // Report manager.
  $sections[] = (object) array(
    'name' => 'report',
    'title' => t("Report Manager"),
    'path' => 'admin/oscaddie_alfresco/report',
    'enabled' => in_array('admin/oscaddie_alfresco/report', $links) ? TRUE : FALSE,
  );

  // SSO manager.
  $sections[] = (object) array(
    'name' => 'sso',
    'title' => t("SSO Manager"),
    'path' => 'admin/oscaddie_alfresco/sso',
    'enabled' => in_array('admin/oscaddie_alfresco/sso', $links) ? TRUE : FALSE,
  );

  // Workflow manager.
  $sections[] = (object) array(
    'name' => 'workflow',
    'title' => t("Workflow Manager"),
    'path' => 'admin/oscaddie_alfresco/workflow',
    'enabled' => in_array('admin/oscaddie_alfresco/workflow', $links) ? TRUE : FALSE,
  );

  return $sections;
}

/**
 * Generates the proper breadcrumb trail for osCaddie Alfresco.
 *
 * @return string
 *   An properly formatted breadcrumb trail.
 */
function oscaddie_alfresco__admin__breadcrumb() {
  $path = arg();
  $breadcrumb = '';

  if ($path[0] == 'admin' && $path[1] == 'oscaddie_alfresco') {
    for ($i = count($path); $i > 1; $i--) {
      $trail_path = implode('/', $path);
      $item = menu_get_item($trail_path);

      $trail[$item['path']] = array(
        'title' => $item['path'] == 'admin/oscaddie_alfresco' ? t("Dashboard") : $item['title'],
        'path' => $item['path'],
      );

      array_pop($path);
    }

    $trail = array_reverse($trail, TRUE);

    foreach ($trail as $data) {
      if ($_GET['q'] == $data['path']) {
        $breadcrumb .= $data['title'];
      }
      else {
        $breadcrumb .= sprintf("%s | ", l($data['title'], $data['path']));
      }
    }
  }

  return $breadcrumb;
}
