<?php
/**
 * @file
 * Drupal RestMini Service Example Test module.
 *
 * NB: This drush script is really obsolete, because the restmini_service_test provides a similar feature.
 * However, we let it linger on for a while, since it provides some examples of how to request via the client module.
 *
 * @code
 * // Alternative, provided by the restmini_service_test module:
 * drush restmini-service-test 'https://ser.ver' 'restmini_service_example_test'
 * @endcode
 */

/**
 * @return array
 */
function restmini_service_example_test_drush_command() {
  return array(
    'restmini-service-example-test' => array(
      'description' => 'Test ' . str_replace('_test_drush_command', '', __FUNCTION__) . ' service endpoint methods.',
      'arguments' => array(
        'server' => 'protocol://domain.tld'
      ),
      'options' => array(
        'no_ssl_verify' => 'The server (host) may use a self-signed SSL certificate.',
        'restart' => 'Send \'delete all\' request before sending any other request.'
      ),
      'examples' => array(
        'drush restmini-service-example-test "https://ser.ver" --no_ssl_verify' => ' ',
        'drush restmini-service-example-test "https://ser.ver" --restart' => ' ',
        'drush restmini-service-example-test "https://ser.ver" --restart --no_ssl_verify' => ' ',
      ),
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    )
  );
}

/**
 * @param string $server
 *   Format: 'protocol://domain.tld'
 */
function drush_restmini_service_example_test($server) {
  // Module name is function - drush_.
  $module = substr(__FUNCTION__, 6);

  // Check that we are in CLI mode.
  if (!drupal_is_cli()) {
    drush_log('Function ' . __FUNCTION__ . ' is only callable from drush/cli.', 'error');
    return;
  }
  // Check that this module is enabled.
  if (!module_exists($module)) {
    drush_log('This service test module[' . $module . '] isnt enabled.', 'error');
    return;
  }
  // Check args.
  if (!$server) {
    drush_log('Argument server is empty.', 'error');
    return;
  }


  $endpoint = 'service/restmini-example/example/thingie';
  $endpoint_human = 'Example Thingie';


  $options = array();
  if (drush_get_option('no_ssl_verify', FALSE)) {
    $options['ssl_verify'] = FALSE;
  }


  // Create a client which retrieves ordinary result.
  $client = RestMiniClient::make(
    $server,
    $endpoint,
    $options
  );
  // Create a client which retrieves response headers and wraps result in response info wrapper.
  $client_verbose = RestMiniClient::make(
    $server,
    $endpoint,
    array_merge(
      $options,
      array(
        // Send X-Rest-Service-Response-Info-Wrapper header.
        'service_response_info_wrapper' => TRUE,
        'get_headers' => TRUE,
      )
    )
  );


  if (drush_get_option('restart', FALSE)) {
    // DELETE delete (all instance).
    $operation = 'DELETE delete (all instance)';
    $args = array(
      'path' => array(
        '*',
      ),
      'get' => array(
        'whatever' => 2
      ),
      'post' => array(
      ),
    );
    inspect(
      $client->delete($args['path'], $args['get'], $args['post'])->result(),
      array(
        'category' => str_replace('drush_', '', __FUNCTION__),
        'message' => $endpoint_human . ' ' . $operation,
      )
    );
  }

  // GET index.
  $operation = 'GET index';
  $args = array(
    'path' => array(
    ),
    'get' => array(
    ),
  );
  inspect(
    array(
      $operation => $client->get($args['path'], $args['get'])->result(),
      // Second arg of ->result(NULL, TRUE) tells client wrap response in client response array.
      // Since this (verbose) client also tells the service to wrap it's response payload in an info wrapper,
      // we get a little information overloaded here.
      $operation . ' verbose' => $client_verbose->get($args['path'], $args['get'])->result(NULL, TRUE),
    ),
    array(
      'category' => str_replace('drush_', '', __FUNCTION__),
      'message' => $endpoint_human . ' ' . $operation,
    )
  );

  // POST create.
  $operation = 'POST create';
  $args = array(
    'path' => array(
    ),
    'get' => array(
    ),
    'post' => array(
      'properties' => array(
        'name' => 'first',
        'color' => 'green',
      ),
    ),
  );
  inspect(
    array(
      $operation => $first_instance_id = $client->post($args['path'], $args['get'], $args['post'])->result(),
      'client info' => $client->info(),
    ),
    array(
      'category' => str_replace('drush_', '', __FUNCTION__),
      'message' => $endpoint_human . ' ' . $operation,
    )
  );

  // POST create, more instances at a time.
  $operation = 'POST create, more instances at a time';
  $args = array(
    'path' => array(
    ),
    'get' => array(
    ),
    'post' => array(
      'instances' => array(
        array(
          'name' => 'second',
          'color' => 'blue',
        ),
        array(
          'name' => 'third',
          'color' => 'red',
        ),
        array(
          'name' => 'fourth',
          'color' => 'yellow',
        ),
      ),
    ),
  );
  inspect(
    array(
      $operation => $instance_ids = $client->post($args['path'], $args['get'], $args['post'])->result(),
      'client info' => $client->info(),
    ),
    array(
      'category' => str_replace('drush_', '', __FUNCTION__),
      'message' => $endpoint_human . ' ' . $operation,
    )
  );

  if ($instance_ids) {
    $operation = 'GET retrieve (second instance)';
    $args = array(
      'path' => array(
        $instance_ids[0]
      ),
      'get' => array(
      ),
    );
    inspect(
      array(
        $operation => $client->get($args['path'], $args['get'])->result(),
        $operation . ' verbose' => $client_verbose->get($args['path'], $args['get'])->result(NULL, TRUE),
      ),
      array(
        'category' => str_replace('drush_', '', __FUNCTION__),
        'message' => $endpoint_human . ' ' . $operation,
      )
    );

    // PUT update, using POST vars.
    $operation = 'PUT update, using POST vars (second instance)';
    $args = array(
      'path' => array(
        $instance_ids[0]
      ),
      'get' => array(
      ),
      'post' => array(
        'properties' => array(
          'color' => 'orange (updated, using POST vars)',
        )
      ),
    );
    inspect(
      array(
        $operation . ' verbose' => $client_verbose->put($args['path'], $args['get'], $args['post'])->result(NULL, TRUE),
        'client info' => $client_info = $client_verbose->info(),
      ),
      array(
        'category' => str_replace('drush_', '', __FUNCTION__),
        'message' => $endpoint_human . ' ' . $operation . ' verbose',
      )
    );

    if ($client_info['status'] == 400) {
      // PUT update, using GET vars.
      $operation = 'PUT update, using GET vars (second instance)';
      $args = array(
        'path' => array(
          $instance_ids[0]
        ),
        'get' => array(
          'color' => 'black (updated, using GET vars)',
        ),
        'post' => array(
        ),
      );
      inspect(
        array(
          $operation => $client->put($args['path'], $args['get'], $args['post'])->result(),
          'client info' => $client_verbose->info(),
        ),
        array(
          'category' => str_replace('drush_', '', __FUNCTION__),
          'message' => $endpoint_human . ' ' . $operation . ' verbose',
        )
      );
    }
  }

  if ($first_instance_id) {
    // DELETE delete.
    $operation = 'DELETE delete (first instance)';
    $args = array(
      'path' => array(
        $first_instance_id
      ),
      'get' => array(
      ),
      'post' => array(
      ),
    );
    inspect(
      $client_verbose->delete($args['path'], $args['get'], $args['post'])->result(NULL, TRUE),
      array(
        'category' => str_replace('drush_', '', __FUNCTION__),
        'message' => $endpoint_human . ' ' . $operation . ' verbose',
      )
    );
  }

  // GET index.
  $operation = 'GET index (again)';
  inspect(
    $client->get()->result(),
    array(
      'category' => str_replace('drush_', '', __FUNCTION__),
      'message' => $endpoint_human . ' ' . $operation,
    )
  );

  // HEAD index.
  $operation = 'HEAD index';
  $args = array(
    'path' => array(
    ),
    'get' => array(
    ),
  );
  inspect(
    $client_verbose->head($args['path'], $args['get'])->result(NULL, TRUE),
    array(
      'category' => str_replace('drush_', '', __FUNCTION__),
      'message' => $endpoint_human . ' ' . $operation . ' verbose',
    )
  );

  drush_log('Sent REST requests, please check watchdog logs.', 'success');
  drush_print('Next attempt: use the --restart option, to send a \'delete all\' request before sending any other request.' . "\n");
}
