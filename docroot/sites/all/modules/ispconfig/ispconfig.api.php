<?php

/**
 * @file
 * Describes hooks provided by the ispconfig module.
 */

/**
 * @mainpage ISPConfig Manual
 *
 * This help will primarily be aimed at documenting
 * function calls.
 *
 * Topics:
 * - @link ispconfig_intro What is ISPConfig and the ISPConfig module? @endlink
 * - @link ispconfig_api The ISPConfig 3.x SOAP API @endlink
 * - @link ispconfig_api_calls Using the ISPConfig module @endlink
 * - @link ispconfig_hooks ISPConfig hooks @endlink
 */

/**
 * @defgroup ispconfig_intro What is ISPConfig and the ISPConfig module?
 * @{
 * ISPConfig is an open source hosting control panel for Linux, licensed under
 * BSD license and developed by the company ISPConfig UG.
 *
 * The ISPConfig project was started in autumn 2005 by the German company
 * projektfarm GmbH.
 *
 * It allows administrators to manage websites, email addresses and DNS records
 * through a web-based interface. The software has 4 login levels: administrator,
 * reseller, client and email-user.
 *
 * The current version of ISPConfig is 3.0. For accessing it from third-party
 * applications, it provides a @link ispconfig_api SOAP API @endlink.
 *
 * The ISPConfig module for Drupal implements this API and provides a flexible
 * and extendable way of conducting ISPConfig 3.x API calls from within Drupal.
 * @}
 */

/**
 * @defgroup ispconfig_api The ISPConfig 3.x SOAP API
 * @{
 * This page explains the ISPConfig 3.x SOAP API.
 *
 * @todo.
 * @}
 */

/**
 * @defgroup ispconfig_api_calls Using the ISPConfig module
 * @{
 * This page explains the use of the ISPConfig provided functions.
 *
 * Simple API function call
 *
 * An example call to retrieve an array of existing clients and their
 * ISPConfig fields (clients_get_all, requires the sub module ispconfig_clients
 * to be enabled):
 *
 * @code
 * $result = ispconfig_api('clients_get_all');
 *
 * // Process the results here ...
 * @endcode
 *
 * API function call with parameters
 *
 * An example call to retrieve the fields of a the ISPConfig client
 * no. 1 (client_get, requires the sub module ispconfig_clients to be
 * enabled):
 *
 * @code
 * $parameters = array(
 *   'client_id' => 1
 * );
 *
 * $result = ispconfig_api('client_get', $parameters);
 *
 * // Process the results here ...
 * @endcode
 *
 * The function ispconfig_api() uses registered API functions only.
 * This ensures, that they can be overridden by other modules as
 * required.
 *
 * @todo Describe more enhanced usage of the API.
 *
 * @}
 */

/**
 * @defgroup ispconfig_hooks ISPConfig hooks
 * @{
 * Hooks that can be implemented by other modules in order to implement the
 * ISPConfig API.
 */

/**
 * This hook allows to add registered API call functions to the ISPConfig
 * module.
 *
 * @return array
 *   An array whose keys are the API function name and whose corresponding
 *   values are arrays containing the following key-value-pairs:
 *   - name: The name of the API function.
 *   - parameters: An array with key-value pairs of API function parameter
 *     names and their default value, WITHOUT the first parameter
 *     $session_id of every API call. The definition of this array may be
 *     taken out of the API Reference of ISPConfig. The order of the
 *     parameters MUST correspond to the order of appearance within the
 *     API call function header, as this array is used to determine the
 *     parameters order when executing the API call.
 *   - access: Array of suggested permissions definitins for this API
 *     function. The structure and used keys must match the permissions
 *     of hook_permission(). These access rights should be obeyed by forms
 *     using the API functions.
 *   - native: A boolean value indicating, if this API function is a
 *     native ISPConfig SOAP API function (that can be executed against
 *     the SOAP API), or an additional function provided by a third-party
 *     module. This is especially helpful to define custom functions that
 *     do several interactions with the ISPConfig API at once. (e.g.
 *     'clients_get_all' implemented with the function
 *     ispconfig_clients_clients_get_all() of the ispconfig_clients
 *     module).
 *   - module: The name of the module that implements this API call.
 *   - callback: The name of the function that implements this API call.
 *   - file: The name of the file that contains the above function.
 *   - path: The name of the path to the above file.
 *
 * @see hook_permission()
 */
function hook_ispconfig_api_functions_register() {
  $path = drupal_get_path('module', 'ispconfig_clients');
  $file = 'ispconfig_clients.module';

  return array(
    // API functions
    'client_get' => array(
      'name' => 'client_get',
      'parameters' => array(
        'client_id' => 0,
      ),
      'access' => array(
        'ispconfig_clients client_get' => array(
          'title' => t("Retrieve client information"),
          'description' => t('Allows reading ISPConfig client information.'),
        ),
      ),
      'native' => true,
      'module' => 'ispconfig_clients',
      'callback' => 'ispconfig_clients_client_get',
      'file' => $file,
      'path' => $path,
    ),
  );
}

/**
 * This hook allows to override the registered API call functions
 * information of the ISPConfig module after all modules registerd
 * their functions using hook_ispconfig_api_functions_register().
 *
 * @param array $functions
 *   An array whose keys are the API function name and whose corresponding
 *   values are arrays containing the following key-value-pairs:
 *   - name: The name of the API function.
 *   - parameters: An array with key-value pairs of API function parameter
 *     names and their default value, WITHOUT the first parameter
 *     $session_id of every API call. The definition of this array may be
 *     taken out of the API Reference of ISPConfig. The order of the
 *     parameters MUST correspond to the order of appearance within the
 *     API call function header, as this array is used to determine the
 *     parameters order when executing the API call.
 *   - access: Array of suggested permissions definitins for this API
 *     function. The structure and used keys must match the permissions
 *     of hook_permission(). These access rights should be obeyed by forms
 *     using the API functions.
 *   - native: A boolean value indicating, if this API function is a
 *     native ISPConfig SOAP API function (that can be executed against
 *     the SOAP API), or an additional function provided by a third-party
 *     module. This is especially helpful to define custom functions that
 *     do several interactions with the ISPConfig API at once. (e.g.
 *     'clients_get_all' implemented with the function
 *     ispconfig_clients_clients_get_all() of the ispconfig_clients
 *     module).
 *   - module: The name of the module that implements this API call.
 *   - callback: The name of the function that implements this API call.
 *   - file: The name of the file that contains the above function.
 *   - path: The name of the path to the above file.
 *
 * @see hook_ispconfig_api_functions_register()
 * @see hook_permission()
 */
function hook_ispconfig_api_functions_alter(&$functions) {
  // Replace the API function name, parameters and 'my_module' accordingly:
  $path = drupal_get_path('module', 'my_module');
  $file = 'my_module.module';

  $functions['client_get'] = array(
      'name' => 'client_get',
      'parameters' => array(
        'client_id' => 0,
      ),
      'access' => array(
        'my_module client_get' => array(
          'title' => t("Retrieve client information"),
          'description' => t('Allows reading ISPConfig client information.'),
        ),
      ),
      'native' => true,
      'module' => 'my_module',
      'callback' => 'my_module_client_get',
      'file' => $file,
      'path' => $path,
    );
}

/**
 * This hook allows altering the parameters of a native ISPConfig
 * API call executing the function 'API_FUNCTION_NAME', just before
 * it's executed.
 *
 * @param array $parameters
 *   The parameters of the API call.
 */
function hook_ispconfig_api_API_FUNCTION_NAME_pre_execute(&$parameters) {
  global $user;
  $new_client_id = my_module_get_client_by_uid($user->uid);

  $parameters['client_id'] = $new_client_id;
}

/**
 * @}
 */