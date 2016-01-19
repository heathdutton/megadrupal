<?php
/**
 * @file
 * restmini_service: Hooks provided by this module.
 *
 * See also restmini_service_test.api.php.
 */


/**
 * Register services and their endpoints and (HTTP) methods.
 *
 *  More than one module is allowed to contribute with:
 *  - endpoints of a specific service
 *  - methods of specific endpoint
 *
 * If more than one module declares a method (enabled) for a service + endpoint, the router will use the first method by module weight.
 *
 * Example of a hook_restmini_service() implementation.
 * 
 * @code
 * function some_restmini_service() {
 *   return array(
 *     // Base route names (machine name, max. length 32).
 *     'rest_service' => array(
 *       // Services (machine name, max. length 32).
 *       'some_service' => array(
 *         // Endpoints (machine name, max. length 32).
 *         'some_endpoint' => array(
 *           // Methods.
 *           'GET' => array(
 *             'callback' => 'some_function',
 *             'enabled' => TRUE,
 *             // permission defaults to 'access content'.
 *           ),
 *           'POST' => array(
 *             'callback' => 'SomeClass::staticMethod',
 *             'enabled' => TRUE,
 *             'permission' => 'create some thing',
 *             'file' => 'inc/SomeClass.inc',
 *           ),
 *           //'PUT' => array(...),
 *           //'DELETE' => array(...),
 *         ),
 *       ),
 *     ),
 *   );
 * }
 * @endcode
 *
 * @return array
 */
function hook_restmini_service() {
  return array(
    // Base route names (machine name, max. length 32).
    'rest_service' => array(
      // Services (machine name, max. length 32).
      'some_service' => array(
        // Endpoints (machine name, max. length 32).
        'some_endpoint' => array(
          // Methods.
          //'HEAD' => array(...)
          'GET' => array(
            'callback' => 'some_function',
            'enabled' => TRUE,
            // permission defaults to 'access content'.
          ),
          'POST' => array(
            'callback' => 'SomeClass::staticMethod',
            'enabled' => TRUE,
            // Optional, defaults to 'access content'.
            'permission' => 'create some thing',
            // Optional, only required if the file isnt included automatically.
            'file' => 'inc/SomeClass.inc',
            // Optional, but needed if responder uses RestMiniService::importArguments().
            'parameters' => array(
              // Optional.
              'path' => array(
                'some_path_arg' => array(
                  // See RestMini::validationPattern().
                ),
              ),
              // Optional.
              'get' => array(
                'some_get_arg' => array(
                  // See RestMini::validationPattern().
                ),

              ),
              // Optional.
              'post' => array(
                'some_post_arg' => array(
                  // See RestMini::validationPattern().
                ),
              ),
            ),
          ),
          //'PUT' => array(...),
          //'DELETE' => array(...),
        ),
      ),
    ),
  );
}

/**
 * Register services, and their endpoints and (HTTP) methods, on behalf of other modules.
 *
 * Delegated entries takes precedence over the entries delivered by service modules themselves,
 * because delegation is intended to be a way of controlling all (or most) endpoints from a single module.
 *
 * Example of a hook_restmini_service_delegate() implementation.
 * 
 * @code
 * function hook_restmini_service_delegate() {
 *   return array(
 *     // Modules.
 *     'some_service_module' => array(
 *       // Base route names (machine name, max. length 32).
 *       'rest_service' => array(
 *         // Services (machine name, max. length 32).
 *         'some_service' => array(
 *           // Endpoints (machine name, max. length 32).
 *           'some_endpoint' => array(
 *             // Methods.
 *             //'HEAD' => array(...),
 *             'GET' => array(
 *               'callback' => 'some_function',
 *               'enabled' => TRUE,
 *               // permission defaults to 'access content'.
 *             ),
 *             'POST' => array(
 *               'callback' => 'SomeClass::staticMethod',
 *               'enabled' => TRUE,
 *               // Optional, defaults to 'access content'.
 *               'permission' => 'create some thing',
 *               // Optional, only required if the file isnt included automatically.
 *               'file' => 'inc/SomeClass.inc',
 *             ),
 *             //'PUT' => array(...),
 *             //'DELETE' => array(...),
 *           ),
 *         ),
 *       ),
 *     ),
 *   );
 * }
 * @endcode
 *
 * @see hook_restmini_service()
 *
 * @return array
 */
function hook_restmini_service_delegate() {
  return array(
    // Modules.
    'some_service_module' => array(
      // Base route names (machine name, max. length 32).
      'rest_service' => array(
        // Services (machine name, max. length 32).
        'some_service' => array(
          // Endpoints (machine name, max. length 32).
          'some_endpoint' => array(
            // Methods.
            //'HEAD' => array(...),
            'GET' => array(
              'callback' => 'some_function',
              'enabled' => TRUE,
              // permission defaults to 'access content'.
            ),
            'POST' => array(
              'callback' => 'SomeClass::staticMethod',
              'enabled' => TRUE,
              // Optional, defaults to 'access content'.
              'permission' => 'create some thing',
              // Optional, only required if the file isnt included automatically.
              'file' => 'inc/SomeClass.inc',
            ),
            //'PUT' => array(...),
            //'DELETE' => array(...),
          ),
        ),
      ),
    ),
  );
}
