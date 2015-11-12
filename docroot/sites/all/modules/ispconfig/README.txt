-- SUMMARY --

The ISPConfig module implements the ISPConfig 3.x Remote API. It allows other
modules to interact with ISPConfig from within Drupal.

You can create, edit and delete servers, hostingplans, resellers, websites,
clients, users, databases and any other entities known to ISPConfig.

This is a complete rewrite of the former - now unsupported and not maintained -
ispconfig modules for Drupal 5.x and Drupal 6.x and is NOT COMPATIBLE with its
predecessors. It's main purpose is to provide a flexible API to other modules.

The ispconfig_admin sub module is providing some basic back-end forms for
administering ISPConfig via its API. It raises no claim to completeness, but
shows a possible implementation of the provided ISPConfig module.

For a full description of the module, visit the project page:
  http://drupal.org/project/ispconfig

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/ispconfig


-- DISCLAIMER --

THIS MODULE AND ITS SUB MODULES ARE PROVIDED AS IS. IN NO EVENT SHALL ANY
CONTRIBUTOR OR MAINTAINER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS MODULE AND ITS SUB
MODULES, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


-- CREDITS --

The original ispconfig module was created by Jan van Diepen and sponsored by
Mountbatten Ltd. (http://www.mountbatten.net/)

The 7.x version of this module has been sponsored by shoreless Limited.
(https://www.shoreless.asia)


-- REQUIREMENTS --

In order to use the ISPConfig module, the PHP Soap Extension must be available.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/895232 for further information.


-- CONFIGURATION --

* Configure ISPConfig Remote API settings and credentials at
  Administration » Configuration » Web services » ISPConfig

* Enable the desired sub modules of ISPConfig to have their API functions
  available.


-- USING THE API FUNCTIONS --

* List available API functions

  ispconfig_api_functions_get();

* Simple API function call

  An example call to retrieve an array of existing clients and their ISPConfig
  fields (clients_get_all, requires the sub module ispconfig_clients to be
  enabled):

  $result = ispconfig_api('clients_get_all');

  // Process the results here ...

* API function call with parameters

  An example call to retrieve the fields of a the ISPConfig client no. 1
  (client_get, requires the sub module ispconfig_clients to be
  enabled):

  $parameters = array(
    'client_id' => 1
  );
  $result = ispconfig_api('client_get', $parameters);

  // Process the results here ...

* Implement API call specific permissions

  Each API function may or may not provide specific access arrays that can be
  used within modules implementing front-end or back-end forms for ISPConfig
  remote access.

  To make use of the provided access arrays, implement hook_permission within
  your custom module 'your_module':

  /**
   * Implements hook_permission().
   *
   * @see https://api.drupal.org/api/function/hook_permission/7
   */
  function your_module_permission() {
    // Initialize the permissions array
    $permissions = array();

    // Get the registered ISPConfig API functions
    $functions = ispconfig_api_functions_get();
    // Add the ISPConfig API functions' permissions to your module
    foreach ($functions as $fname => $finfo) {
      $permissions = array_merge($permissions, $finfo['access']);
    }

    // Add your own permissions

    return $permissions;
  }

  
-- CUSTOMIZATION --

* In case of the ISPConfig module or one of its sub modules isn't providing the
  desired API call, you have two alternatives:

  - Create a feature request at http://drupal.org/project/issues/ispconfig

  - Implement the function yourself: 

    Create a custom module and implement the hook
    hook_ispconfig_api_functions_register().

    It should return an associative array of implemented API functions
    as follows:

    array(
      'api_function_name' => array(
        'name' => 'api_function_name',
        'parameters' => array(
            'param1' => 0, // Names and default values of the parameters as
                           // mentioned within the ISPConfig API reference,
                           // WITHOUT the $session_id parameter.
        ),
        'access' => array(
          // Suggested permissions for this function to be implemented by your
          // and other modules
        ),
        'native' => true,
        'module' => 'your_module',
        'file' => 'your_module.module',
        'path' => 'path_to_the_above_file',
        'callback' => 'your_module_api_function_name',
      ),
    );

    Additionally create a callback function that implements the above API call
    via the ISPConfig module in your 'path_to_the_above_file/your_module.module'
    file:

    function your_module_api_function_name($param1[, ...], $session_id = '', $show_errors = true) {
      return ispconfig_api_execute('api_function_name', array(
          'param1' => $param1,
          // ...
        ), $session_id, $show_errors);
    }

    Note: Make sure the signature of your callback function corresponds to the
    signature of the ISPConfig API function (and the parameters definition
    within the hook module_ispconfig_api_functions_register(), whereas the
    $session_id FOLLOWS the other parameters. An additional boolean parameter
    $show_errors shows or suppresses error messages via drupal_set_Message().
    (For further reference, compare how it's done within the ISPconfig sub
    modules.)

* To OVERRIDE already implemented API function calls, use the hook
  module_ispconfig_api_functions_alter(&$functions). It allows
  altering the registered API functions array within your custom module.

  Make sure, you also implement an according API function callback.

* To ALTER PARAMETERS of an API function just before its execution, you may
  wish to implement the hook
  module_ispconfig_api_[API_function_name]_pre_execute(&$parameters)


-- TROUBLESHOOTING --

* The API call throws a "Bad Request" error:

  - Make sure you have set up the API protocol correctly within the ISPConfig
    settings.

  - Make sure the signature of the parameters used matches the parameters of
    the ISPConfig Remote API reference.


-- FAQ --

None yet.