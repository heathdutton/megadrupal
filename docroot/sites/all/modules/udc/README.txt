
Summary

  The User Data Connector module allows you to perform user authentication
  and obtaining information about users from a Drupal-external PHP script
  using a simple and compact API. The module is especially useful if your
  script environment (session, error callback, exception callback, shutdown
  function, defined variables, defined functions) must not be changed, as it
  would happen if you bootstrap Drupal and use its functions and classes.

  The API you include in your external script has three classes, all located
  in the directory "<root>/sites/all/modules/udc/client/", where <root>
  is the Drupal root folder, which should be the $_SERVER['DOCUMENT_ROOT'].
  The class source code is php/javadoc documented, so that IDEs like Eclipse
  or NetBeans autocomplete the object methods and instance variables for you,
  and list method/property information in-situu.

    * The DrupalUserAuth class allows you to check if a login name/password
      pair matches. This will not affect the Drupal page login state of
      the user because only a password check using the corresponding Drupal
      core functions is performed. The return values are stored in the
      object's properties and include username (correct key case like in
      database), email, roles, fields (only allowed fields in the module
      config), an 'active' flag that indicates if the user is active or
      blocked/canceled, and the "valid" flag that indicates that the
      password was correct.
    * The DrupalUserInfo class enables you to get detailed information
      about a user without performing an authentification check. The object
      properties are equivalent to the properties of DrupalUserAuth.
    * The DrupalUserList class can be used to retrieve a list of users. The
      only instance variable is "list", which will contain an associative
      array with the users. Each entry implies name, email and active flag.
      If you invoke the request method with the corresponding arguments,
      each entry contains the roles of the user as well.

  To provide this functionality, the module consists of two communicating
  parts: The client classe API and the server script. The client classes,
  which you will use, send a JSON request to the server via HTTPS or HTTP on
  localhost. The server script bootstraps Drupal (only the necessary part to
  save time), processes the user requests and sends back the JSON results -
  or an error message. The client API class then parses the response and puts
  the results in its instance variables.

  Using a local network connection is quite often used (e.g. database
  connections), but of cause it can be dogy according to the security of the
  user data. For this reason, the module has some features to prevent getting
  in trouble, all of them implemented in the server script:

    * The server only responds to requests comming from the its own IP
      address
    * The server only responds if the module is enabled
    * You can configure that the server only accepts HTTPS connections
    * You must specify a token in the configuration used to identify the
      client
    * You can exclude single users by name from the response
    * You can specify which user roles are allowed to be sent to the client
    * You can restrict the profile fields which can be sent to the client
    * Neither user UID nor password hash are sent in the response

  The token is required because the server script can only check if the
  request comes from its own IP. If you are the one and only admin on the
  server, the system is safe without a token identification. However,
  normally more than one virtual host are on on web server, and all have the
  same IP address. Having no additional client identification would allow
  other webmasters on your server to get information about your users.
  Fortunately, what they do not have is access to your file system and
  database (if they do they can get your user data anyway). Hence, an
  identification token prevents external requests from the same server. In
  case that the request does not come form the server IP a 404 response will
  be sent immediately. If any of the scripts is included in normal Drupal
  context, an exception will be thrown.

Performance tests

  Some annotations about the performance: As realized during the
  implementation and testing, the bottleneck according to the performance is
  not mainly the additional localhost connection, but the full bootstrapping
  and hash calculation. The server script always bootstraps to the
  DRUPAL_BOOTSTRAP_VARIABLES state, which does not consume much time. In case
  of an authentification request, the necessary drupal core files are
  included and the user_check_password() function is invoked, which increases
  the required process time significantly. A second factor reducing the
  performance is the request of several types of additional user information.
  There are two information types implemented in the server script: Fields of
  the core profile module (which is unfortunately deprecated since DP7.2) and
  common fields (As configured in User>Manage Fields). The server has a rapid
  method of retrieving profile module fields from the database whith almost
  no performance losses. For common fields however, the server needs the
  user_load() function. This causes an in-place full Drupal bootstrap, which
  is time and memory consuming. Here some process times for comparison, where
  tp = server process time, R=process time ratio with respect to the ping
  process time, and tr the total request time including the SSL network
  connection and unserializing. The test ran on a standard notebook, the
  values may vary with the server, code caching etc. You can find the test
  script "performance.php" in the examples folder.

    * Ping:                        tr =  73ms,      tp =  17ms,     R =  1.0
    * List without roles:          tr =  81ms,      tp =  24ms,     R =  1.4
    * List with roles:             tr =  83ms,      tp =  26ms,     R =  1.5
    * Info without fields:         tr =  86ms,      tp =  26ms,     R =  1.5
    * Info with 1 profile fields:  tr =  86ms,      tp =  29ms,     R =  1.7
    * Auth without fields:         tr = 186ms,      tp = 129ms,     R =  7.5
    * Auth with 1 profile field:   tr = 191ms,      tp = 134ms,     R =  7.8
    * Info with 1 common fields:   tr = 258ms,      tp = 188ms,     R = 10.9
    * Auth with 1 common field:    tr = 365ms,      tp = 295ms,     R = 17.0

  Conclusion: In order to avoid latencies in your script responses, try not
  to authenticate user often or every time, but save the client results (or
  the whole client object) in your session variables.

  To learn how easy the API usage is, take a look at the examples shown below
  (and available in the examples folder in the module directory). You can get
  additional information about the module on the project page:

    http://drupal.org/project/udc

  Send/check bug reports and feature requests on

    http://drupal.org/project/issues/udc


Applications

  The main application range where this module can help generally includes
  scripts/systems which are incompatible with the Drupal context or existing
  implementations in which you want to integrate the Drupal user management.

    * Auth for standalone admin scripts: error log feeds
    * Auth for WebDAV compliant php implementation
    * CLI scripts which require user information
    * Cron/incron/event based group mail or push notifications

  Related Drupal functions/module recommendations:

    * If you intent to implement an interfacing script e.g. for a computer
      application take a look at Drupals XMLRPC capabilities first.
    * If you want to login using HTTP auth, checkout the "Secure Site"
      module.
    * For restricting certain site sections using HTTP auth, have a look at
      the "HTPasswdSync" module.

Requirements

  The implementation of the API and the server requires a PHP version >= 5.0,
  if you want to enable SSL/HTTPS for the client-server connections, you need
  PHP with the cURL library. The configuration form shows you if this feature
  can be enabled. There are no special Drupal module dependencies.

  As your web server configuration might disallow the execution of scripts that
  are not located in the document root, you should ensure that the server script
  (<DRUPAL ROOT>/sites/all/modules/udc/server/server.php) can be accessed.

Configuration

  The module configuration page contains detailed in-place information about
  how to setup the module. In fact, there is not much to do to get the module
  running.

Customization

  None planed.

Troubleshooting

FAQ

Maintainers

  * Stefan Wilhelm (7.x-xx): http://drupal.org/user/1344522
