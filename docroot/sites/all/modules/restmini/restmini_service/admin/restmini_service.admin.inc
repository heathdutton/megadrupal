<?php
/**
 * @file
 * restmini_service: Admin layer.
 */


/**
 * RESTmini Service settings and registered service endpoint methods overview.
 *
 * @param array $form
 * @param array &$form_state
 *
 * @return array
*/
function restmini_service_admin_form($form, &$form_state) {
  drupal_add_css(
    drupal_get_path('module', 'restmini_service') . '/admin/restmini_service.admin.css',
    array('type' => 'file', 'group' => CSS_DEFAULT, 'preprocess' => FALSE, 'every_page' => FALSE)
  );

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    'restmini_service_base_routes' => array(
      '#type' => 'textarea',
      '#title' => t('Service base paths'),
      '#description' => t(
        'Newline-separated list of name|path pairs.!nl!emphName!_emph: Machine name !regex max. length !le.!nl!emphPath!_emph: Wildcard <b>%</b> is allowed, but will be ignored. Make sure the path doesn\'t collide with system paths, like \'admin/...\', \'ajax/...\', etc.!nlDefault: service_rest|service/rest',
        array(
          '!nl' => '<br/>',
          '!emph' => '<em>',
          '!_emph' => '</em>',
          '!le' => RestMiniService::NAME_MAXLENGTH,
          '!regex' => substr(RestMiniService::MACHINE_NAME_REGEX, 2, strlen(RestMiniService::MACHINE_NAME_REGEX) - 4),
        )
      ),
      '#required' => TRUE,
      '#default_value' => variable_get('restmini_service_base_routes', 'service_rest|service/rest'),
    ),
    'restmini_service_log_bad_response' => array(
      '#type' => 'checkbox',
      '#title' => t('Log warning to watchdog when a HTTP response status code is used inappropriately'),
      '#default_value' => variable_get('restmini_service_log_bad_response', 1),
    ),
  );

  $form['restmini_service_clear_cache_on_submit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clear caches of this module and the menu system'),
    '#default_value' => 1,
  );

  $form['#submit'][] = 'restmini_service_admin_form_submit';
  return system_settings_form($form);
}

/**
 * @param array $form
 * @param array &$form_state
 */
function restmini_service_admin_form_submit($form, &$form_state) {
  $values =& $form_state['values'];

  $clear_cache = $values['restmini_service_clear_cache_on_submit'];

  // The base routes variable (restmini_service_base_routes) is a newline-separated list of [name]|path.
  // When used, it will be unpacked to an array of paths by names.
  // Since the variable isn't used at runtime
  // - but only when the module builds it's service registry, and when the menu system invokes hook_menu() -
  // the simple text format is preferable (makes it easier to the var i Features modules etc.
  if (($base_routes = trim($values['restmini_service_base_routes'])) === '') {
    $base_routes = 'service_rest|service/rest';
  }
  else {
    $pairs = explode("\n", str_replace("\r", '', $base_routes));
    $base_routes = $path_by_name = array();
    $error = '';
    foreach ($pairs as $index => $value) {
      if (strpos($value, '|') === FALSE) {
        $error = t('Suggested base route !no \'@base_route\' misses | pipe separator.',
          array('!no' => $index + 1, '@base_route' => $value));
        break;
      }
      $name_path = explode('|', $value);
      $name = $name_path[0];
      $path = $name_path[1];

      if (strlen($name) > RestMiniService::NAME_MAXLENGTH) {
        $error = t('Suggested base route !no \'@base_route\' name is longer than !le characters.',
          array('!no' => $index + 1, '@base_route' => $value, '!le' => RestMiniService::NAME_MAXLENGTH));
        break;
      }
      if (!preg_match(RestMiniService::MACHINE_NAME_REGEX, $name)) {
        $error = t('Suggested base route !no \'@base_route\' name is not a valid machine name !regex',
          array('!no' => $index + 1, '@base_route' => $value,
            '!regex' => substr(RestMiniService::MACHINE_NAME_REGEX, 2, strlen(RestMiniService::MACHINE_NAME_REGEX) - 4)));
        break;
      }
      if (array_key_exists($name, $path_by_name)) {
        $error = t('Suggested base route !no \'@base_route\' name @name is a dupe.',
          array('!no' => $index + 1, '@base_route' => $value, '@name' => $name));
        break;
      }

      // Remove leading slash.
      if ($path{0} === '/') {
        $path = substr($path, 1);
      }
      // Remove traling slash.
      if ($path{strlen($path) - 1} === '/') {
        $path = substr($path, 0, strlen($path) - 1);
      }
      if ($path === '' || $path === '/') {
        $error = t('Suggested base route \'@base_route\' path cannot be empty, nor /',
          array('@base_route' => $value));
        break;
      }
      // Replace wildcard * by % (common error).
      if (strpos($path, '*') !== FALSE) {
        drupal_set_message(
          t('Suggested base route \'@base_route\' path contains wrong wildcard * - replaced it/them with %',
            array('@base_route' => $value))
        );
        $path = str_replace('*', '%', $path);
      }
      if (path_is_admin($path)) {
        $error = t('Suggested base route !no \'@base_route\' path @path matches an admin page path.',
          array('!no' => $index + 1, '@base_route' => $value, '@path' => $path));
        break;
      }
      if (in_array($path, $path_by_name, TRUE)) {
        $error = t('Suggested base route !no \'@base_route\' path @path is a dupe.',
          array('!no' => $index + 1, '@base_route' => $value, '@path' => $path));
        break;
      }
      $path_by_name[$name] = $path;
      $base_routes[] = $name . '|' . $path;
    }
    unset($pairs, $path_by_name);
    if ($error) {
      form_set_error(
        'restmini_service_base_routes',
        $error
      );
      // Don't save that value.
      unset($form_state['input']['restmini_service_base_routes'], $form_state['values']['restmini_service_base_routes']);
      return;
    }
    if ($base_routes) {
      $base_routes = join("\n", $base_routes);
    }
    else {
      $base_routes = 'service_rest|service/rest';
    }
  }

  // For later.
  $values['restmini_service_base_routes'] = $base_routes;
  // For now; RestMiniService::registry() uses the value.
  if ($clear_cache) {
    variable_set('restmini_service_base_routes', $base_routes);
  }

  // That clear cache field shan't be saved as a variable.
  unset($form_state['input']['restmini_service_clear_cache_on_submit'], $form_state['values']['restmini_service_clear_cache_on_submit']);

  if ($clear_cache) {
    // menu_rebuild() effectively also rebuilds the service registry cache, because restmini_service_menu() calls RestMiniService::registry(TRUE).
    menu_rebuild();
  }
}

/**
 * Registered service endpoint methods overview.
 *
 * @return string
 */
function restmini_service_admin_endpoints() {
  drupal_add_css(
    drupal_get_path('module', 'restmini') . '/admin/restmini.admin.css',
    array('type' => 'file', 'group' => CSS_DEFAULT, 'preprocess' => FALSE, 'every_page' => FALSE)
  );
  drupal_add_css(
    drupal_get_path('module', 'restmini_service') . '/admin/restmini_service.admin.css',
    array('type' => 'file', 'group' => CSS_DEFAULT, 'preprocess' => FALSE, 'every_page' => FALSE)
  );

  // Table view of registered service endpoint methods.
  $table_header = array(
    t('Base path name', array(), array('context' => 'module:restmini_service')),
    t('Service', array(), array('context' => 'module:restmini_service')),
    t('Endpoint', array(), array('context' => 'module:restmini_service')),
    t('Method', array(), array('context' => 'module:restmini_service')),
    t('Enabled', array(), array('context' => 'module:restmini_service')),
    t('Delegate *', array(), array('context' => 'module:restmini_service')),
    t('Module **', array(), array('context' => 'module:restmini_service')),
    t('Permission', array(), array('context' => 'module:restmini_service')),
    t('Callback', array(), array('context' => 'module:restmini_service')),
    t('File', array(), array('context' => 'module:restmini_service')),
  );
  $table_rows = array();

  $base_routes = restmini_service_base_routes();
  $registry = RestMiniService::registry();

  foreach ($registry as $base_route_name => $services) {

    $base_path = $base_routes[$base_route_name];

    foreach ($services as $service_name => $endpoints) {

      foreach ($endpoints as $endpoint_name => $methods) {
        $route = $base_path . '/' . $service_name . '/' . $endpoint_name;

        // base route + service + endpoint goes to rowspanned cells.
        $n_responders = 0;
        $i_insert = count($table_rows);

        foreach ($methods as $method_name => $enableds_disableds) {

          foreach ($enableds_disableds as $enabled_disabled => $responders) {

            foreach ($responders as $responder) {
              ++$n_responders;

              $table_rows[] = array(
                'data' => array(
                  // rowspan.
                  $method_name,
                  array(
                    'data' => $enabled_disabled == 'enabled' ? '&#x2714;' : '&nbsp;',
                    'class' => array('restmini-service-list-enabled'),
                  ),
                  !empty($responder['delegate']) ? $responder['delegate'] : '&nbsp;',
                  array(
                    'data' => $responder['module'],
                    'class' => array(($module_exists = module_exists($responder['module'])) ? '' : 'restmini-service-list-module-missing'),
                  ),
                  !empty($responder['permission']) ? $responder['permission'] : RestMiniService::PERMISSION_DEFAULT,
                  $responder['callback'],
                  !empty($responder['filename']) ? ($responder['filename'] . '.' . $responder['fileext']) : '&nbsp;',
                ),
                'title' => $route,
                'onclick' => 'alert(window.location.href.replace(/^(https?:\/\/[^\/]+\/).*$/, \'$1\') + \'' . $route . '\');',
              );
            }
          }
        }

        // Insert base route + service + endpoint rowspanned cells.
        if ($n_responders) {
          $data = $table_rows[$i_insert]['data'];
          array_unshift(
            $data,
            array(
              'data' => $base_route_name,
              'rowspan' => $n_responders,
            ),
            array(
              'data' => $service_name,
              'rowspan' => $n_responders,
            ),
            array(
              'data' => $endpoint_name,
              'rowspan' => $n_responders,
            )
          );
          $table_rows[$i_insert]['data'] = $data;
        }

      }
    }
  }

  return theme_table(
    array(
      'header' => $table_header,
      'rows' => $table_rows,
      'attributes' => array(
        'class' => array(
          'restmini',
          'restmini-service-list',
        ),
      ),
      'caption' => '',
      'colgroups' => array(),
      'sticky' => TRUE,
      'empty' => t(
        'No responders registered via hook_restmini_service_delegate() and hook_restmini_service() implementations.',
        array(),
        array('context' => 'module:restmini_service')
      ),
    )
  )
  . '<p class="restmini-service-list-notes">'
  . t('Click a row to copy endpoint URL.', array(), array('context' => 'module:restmini_service'))
  . '<br/>' . '* ' . t(
    'A delegate is a module that registers a method on behalf of another module - via hook_restmini_service_delegate().',
    array(),
    array('context' => 'module:restmini_service')
  )
  . '<br/>' . '** ' . t(
    'If a module is striked-through it doesn\'t exist or isn\'t enabled.',
    array(),
    array('context' => 'module:restmini_service')
  )
  . '</p>';
}
