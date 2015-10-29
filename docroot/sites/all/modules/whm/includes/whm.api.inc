<?php

/**
 * @file
 * Integration with cPanel's WebHost Manager (WHM) software API.
 * 
 * Note: Functions do not check permissions.
 */

/**
 * Constants defining error codes.
 */
class WHMConstant {

  // Successful return value.
  const WHM_SUCCESS = 1;
  // Generic error.
  const WHM_ERROR = -1;
  // Could not connect to WHM server.
  const WHM_ERROR_CONNECT = -2;
  // Authentication failed.
  const WHM_ERROR_AUTH = -3;
  // Curl not installed.
  const WHM_ERROR_CURL = -4;
  // Variables not specified.
  const WHM_ERROR_VARS = -5;
  // Username contains invalid characters.
  const WHM_USERNAME_INVALID = -10;
  // Username already taken.
  const WHM_USERNAME_TAKEN = -11;
  // Username is too long (max is 8 chars).
  const WHM_USERNAME_TOOLONG = -12;
  // Domain is malformed.
  const WHM_DOMAIN_INVALID = -20;
  // Domain is already taken.
  const WHM_DOMAIN_TAKEN = -21;
  // Email is invalid.
  const WHM_EMAIL_INVALID = -30;
  // Password is invalid.
  const WHM_PASSWORD_INVALID = -40;
  // Port to access WHM insecurely on.
  const WHM_STANDARD_PORT = 2086;
  // Port to access WHM securely on.
  const WHM_SECURE_PORT = 2087;
  // Maximum user name length.
  const WHM_MAX_USERNAME_LEN = 8;
  // Maximum password length.
  const WHM_MAX_PASSWORD_LEN = 6;
}

/**
 * This function displays the server's hostname.
 */
function whm_hostname() {
  $result = _whm_command('gethostname');
  return $result['hostname'];
}

/**
 * This function displays the version of cPanel/WHM running on the server.
 */
function whm_version() {
  $result = _whm_command('version');
  return $result['version'];
}

/**
 * This function will display your server's load average.
 */
function whm_average_load() {
  // TODO: depending on WHM version also call systemloadavg.
  $result = _whm_command('loadavg');
  return $result;
}

/**
 * This function retrieves a list of the languages available on your server.
 */
function whm_available_languages() {
  $result = _whm_command('getlanglist');
  return $result;
}

/**
 * This function retrieves a list of IP's configured on the server.
 */
function whm_main_ip() {
  $result = _whm_command('listips');
  return $result['result'][0]['ip'];
}

/**
 * Account functions
 */

/**
 * This function creates a hosting account and sets up associated information.
 * 
 * @param string $user
 *   Username for the account. Example: user 
 * @param string $domain
 *   Domain name. Example: domain.tld
 * @param array $options
 *   The following variables are optional: 
 *   <li>plan (string) — Package to use for account creation.</li> 
 *   <li>
 *     pkgname (string) — Name of a new package to be created. 
 *     Example: reseller_silver
 *   </li>
 *   <li>savepkg (boolean) — Save the settings used as a new package.
 *     <ul>
 *       <li>1 — yes.</li>
 *       <li>0 — no.</li>
 *     </ul>
 *   </li> 
 *   <li>
 *     featurelist (string) — Name of the feature list to be used.. 
 *     Example: no_ftp_100mb_gold
 *   </li>
 *   <li>quota (integer) — Disk space quota in Megabytes. Allowed values:
 *     <ul>
 *       <li>A number from 0 to 999999.</li>
 *       <li>0 is unlimited.</li>
 *     </ul>
 *   </li> 
 *   <li>password (string) — Password for accessing cPanel.</li>
 *   <li>ip (string) — Whether or not the domain has a dedicated IP address.
 *     <ul>
 *       <li>y — yes.</li>
 *       <li>n — no.</li>
 *     </ul>
 *   </li> 
 *   <li>cgi (boolean) — Whether or not the domain has CGI access.
 *     <ul>
 *       <li>1 — yes.</li>
 *       <li>0 — no.</li>
 *     </ul>
 *   </li> 
 *   <li>frontpage (boolean) — Whether or not the domain has FrontPage.
 *     <ul>
 *       <li>1 — yes.</li>
 *       <li>0 — no.</li>
 *     </ul>
 *   </li> 
 *   <li>hasshell (boolean) — Whether or not the domain has shell/SSH access.
 *     <ul>
 *       <li>1 — yes.</li>
 *       <li>0 — no.</li>
 *     </ul>
 *   </li> 
 *   <li>contactemail (string) — Contact email address for the account.</li>
 *   <li>cpmod (string) — cPanel theme name. Example: x3</li>
 *   <li>maxftp (string) — Maximum number of FTP accounts. Allowed values:
 *     <ul>
 *       <li>A number from 0 to 999999.</li>
 *       <li>0 is unlimited.</li>
 *       <li>unlimited</li>
 *       <li>null</li>
 *     </ul>
 *   </li> 
 *   <li>maxsql (string) — Maximum number of SQL databases. Allowed values:
 *     <ul>
 *       <li>A number from 0 to 999999.</li>
 *       <li>0 is unlimited.</li>
 *       <li>unlimited</li>
 *       <li>null</li>
 *     </ul>
 *   </li>
 *   <li>maxpop (string) — Maximum number of email accounts. Allowed values:
 *     <ul>
 *       <li>A number from 0 to 999999.</li>
 *       <li>0 is unlimited.</li>
 *       <li>unlimited</li>
 *       <li>null</li>
 *     </ul>
 *   </li> 
 *   <li>maxlst (string) — Maximum number of mailing lists. Allowed values:
 *     <ul>
 *       <li>A number from 0 to 999999.</li>
 *       <li>0 is unlimited.</li>
 *       <li>unlimited</li>
 *       <li>null</li>
 *     </ul>
 *   </li> 
 *   <li>maxsub (string) — Maximum number of subdomains. Allowed values:
 *     <ul>
 *       <li>A number from 0 to 999999.</li>
 *       <li>0 is unlimited.</li>
 *       <li>unlimited</li>
 *       <li>null</li>
 *     </ul>
 *   </li> 
 *   <li>maxpark (string) — Maximum number of parked domains. Allowed values:
 *     <ul>
 *       <li>A number from 0 to 999999.</li>
 *       <li>0 is unlimited.</li>
 *       <li>unlimited</li>
 *       <li>null</li>
 *     </ul>
 *   </li> 
 *   <li>maxaddon (string) — Maximum number of addon domains. Allowed values:
 *     <ul>
 *       <li>A number from 0 to 999999.</li>
 *       <li>0 is unlimited.</li>
 *       <li>unlimited</li>
 *       <li>null</li>
 *     </ul>
 *   </li> 
 *   <li>bwlimit (string) — Bandiwdth limit in Megabytes.
 *     <ul>
 *       <li>0 is unlimited.</li>
 *       <li>Must between 0 and 999999.</li>
 *     </ul> 
 *   </li>
 *   <li>customip (string) — Specific IP address for the site.</li>
 *   <li>language (string) — Language to use. Example: spanish-utf8</li>
 */
function whm_create_account($user, $domain, $options = array()) {
  $rv = whm_check_username($user);
  if ($rv !== WHMConstant::WHM_SUCCESS) {
    return $rv;
  }
  $rv = whm_check_domain($domain);
  if ($rv !== WHMConstant::WHM_SUCCESS) {
    return $rv;
  }
  if ($options['contactemail']) {
    $rv = whm_check_email($options['contactemail']);
    if ($rv !== WHMConstant::WHM_SUCCESS) {
      return $rv;
    }
  }
  if ($options['pass']) {
    $rv = whm_check_password($options['pass']);
    if ($rv !== WHMConstant::WHM_SUCCESS) {
      return $rv;
    }
  }
  if ($options['plan']) {
    $rv = whm_check_plan($options['plan']);
    if ($rv !== WHMConstant::WHM_SUCCESS) {
      return $rv;
    }
  }

  $options = array(
    'username' => $user,
    'domain' => $domain,
  ) + $options;

  $result = _whm_command('createacct', $options);
  if ($result['result'][0]['status'] == 1) {
    return WHMConstant::WHM_SUCCESS;
  }
  return $result['result'][0]['statusmsg'];
}

/**
 * This function changes the password of an account.
 * 
 * @param string $user
 *   Name of the user whose password should be changed.
 * @param string $pass
 *   New password for the user.
 */
function whm_change_password($user, $pass) {
  $rv = whm_check_password($pass);
  if ($rv === WHMConstant::WHM_SUCCESS) {
    $result = _whm_command('passwd', array('user' => $user, 'pass' => $pass));
    if ($result['passwd'][0]['status'] == 1) {
      return WHMConstant::WHM_SUCCESS;
    }
  }
  else {
    return $rv;
  }
  return WHMConstant::WHM_ERROR;
}

/**
 * This function modifies the bandwidth usage limit for a specific account.
 *  
 * @param string $user
 *   Name of the user for whom you wish to modify the bandwidth usage limit.
 * @param string $limit
 *   Bandwidth usage limit, in Megabytes.
 */
function whm_limit_bandwidth($user, $limit) {
  $result = _whm_command('limitbw', array('user' => $user, 'bwlimit' => $limit));
  if ($result['result'][0]['status'] == 1) {
    return WHMConstant::WHM_SUCCESS;
  }
  return WHMConstant::WHM_ERROR;
}

/**
 * This function lists all accounts on the server and also allows searching.
 * 
 * @param string $search_type
 *   Type of account search. Allowed values: domain, owner, user, ip, package
 * @param string $search
 *   Search criteria; this field takes regular expressions.
 */
function whm_list_accounts($search_type = NULL, $search = NULL) {
  $opts = array();
  if ($search_type != NULL) {
    $opts['searchtype'] = $search_type;
  }
  if ($search != NULL) {
    $opts['search'] = $search;
  }
  $result = _whm_command('listaccts', $opts);
  return $result['acct'];
}

/**
 * This function modifies settings for an account.
 *  
 * <b>Important:</b> Instead of using this function call, 
 * we recommend changing packages with the Upgrade/Downgrade Account feature.
 * When you modify an account using this API function,
 * the changes are in danger of being lost when the package is modified.
 * 
 * @param string $user
 *   User name of the account. Example: user
 * @param array $options
 *   The following variables are optional: 
 *   <li>domain (string) — Domain name. Example: domain.tld</li>
 *   <li>newuser (string) — Used when changing the username of an account.</li> 
 *   <li>owner (string) — Change the owner of the account.</li>
 *   <li>CPTHEME (string) — cPanel theme name. Example: x3</li>
 *   <li>HASCGI (boolean) — Whether or not the domain has CGI access.
 *     <ul>
 *       <li>1 — yes.</li>
 *       <li>0 — no.</li>
 *     </ul>
 *   </li>
 *   <li>LANG (string) — Language to use.</li> 
 *   <li>
 *     LOCALE (string) — Locale to use in the account's cPanel interface. 
 *     Note: Only compatible with WHM version 11.25 and later.
 *   </li> 
 *   <li>MAXFTP (string) — Maximum number of FTP accounts. Allowed values:
 *     <ul>
 *       <li>A number from 0 to 999999. 0 is unlimited.</li> 
 *       <li>unlimited</li> 
 *       <li>null</li>
 *     </ul>
 *   </li>
 *   <li>MAXSQL (string) — Maximum number of SQL databases. Allowed values: 
 *     <ul>
 *       <li>A number from 0 to 999999. 0 is unlimited.</li> 
 *       <li>unlimited</li> 
 *       <li>null</li>
 *     </ul>
 *   </li>
 *   <li>MAXPOP (string) — Maximum number of email accounts. Allowed values: 
 *     <ul>
 *       <li>A number from 0 to 999999. 0 is unlimited.</li> 
 *       <li>unlimited</li> 
 *       <li>null</li>
 *     </ul>
 *   </li>
 *   <li>MAXLST (string) — Maximum number of mailing lists. Allowed values: 
 *     <ul>
 *       <li>A number from 0 to 999999. 0 is unlimited.</li> 
 *       <li>unlimited</li> 
 *       <li>null</li>
 *     </ul>
 *   </li>
 *   <li>MAXSUB (string) — Maximum number of subdomains. Allowed values:
 *     <ul>
 *       <li>A number from 0 to 999999. 0 is unlimited.</li> 
 *       <li>unlimited</li> 
 *       <li>null</li>
 *     </ul>
 *   </li>
 *   <li>MAXPARK (string) — Maximum number of parked domains. Allowed values: 
 *     <ul>
 *       <li>A number from 0 to 999999. 0 is unlimited.</li> 
 *       <li>unlimited</li> 
 *       <li>null</li>
 *     </ul>
 *   </li>
 *   <li>MAXADDON (string) — Maximum number of addon domains. Allowed values: 
 *     <ul>
 *       <li>A number from 0 to 999999. 0 is unlimited.</li> 
 *       <li>unlimited</li> 
 *       <li>null</li>
 *     </ul>
 *   </li>
 *   <li>shell (boolean) — Whether or not the domain has shell/SSH access. 
 *     <ul>
 *       <li>1 — yes.</li>
 *       <li>0 — no.</li>
 *     </ul>
 *   </li>
 */
function whm_modify_account($user, $options = array()) {
  $rv = whm_check_username($options['newuser']);
  if ($rv !== WHMConstant::WHM_SUCCESS) {
    return $rv;
  }

  $options = array(
    'user' => $user,
  ) + $options;

  $result = _whm_command('modifyacct', $options);
  if ($result['result'][0]['status'] == 1) {
    return WHMConstant::WHM_SUCCESS;
  }
  return $result['result'][0]['statusmsg'];
}

/**
 * Function to retrieve server status information.
 */
function whm_get_services() {
  $result = _whm_command('servicestatus');
  if ($result['result']['status'] == 1) {
    return $result['service'];
  }
  return $result['result']['statusmsg'];
}

/**
 * Function to retrieve service status information.
 */
function whm_get_service($service) {
  $result = _whm_command('servicestatus', array('service' => $service));
  if ($result['result']['status'] == 1) {
    return $result['service'];
  }
  return $result['result']['statusmsg'];
}

/**
 * This function changes an account's disk space usage quota.
 * 
 * @param string $user
 *   User name of the account. Example: user
 * @param int $quota
 *   The user's new disk space quota, in Megabytes. Example: 500
 */
function whm_modify_quota($user, $quota) {
  $result = _whm_command('editquota', array('user' => $user, 'quota' => $quota));
  if ($result['result'][0]['status'] == 1) {
    return WHMConstant::WHM_SUCCESS;
  }
  return WHMConstant::WHM_ERROR;
}

/**
 * This function displays pertinent information about a specific account.
 * 
 * @param string $user
 *   Username associated with the acount you wish to display.
 */
function whm_account_info($user) {
  $result = _whm_command('accountsummary', array('user' => $user));
  if ($result['status'] == 1) {
    return $result['acct'];
  }
  return WHMConstant::WHM_ERROR;
}

/**
 * Suspend user from current reseller account.
 */
function whm_suspend_account($user) {
  if (empty($user)) {
    return WHMConstant::WHM_ERROR_VARS;
  }
  $user = urlencode($user);
  $result = _whm_command('suspendacct', array('user' => $user));
  if ($result['result'][0]['status'] == 1) {
    return WHMConstant::WHM_SUCCESS;
  }
  return WHMConstant::WHM_ERROR;
}

/**
 * Terminates a WHM account from current reseller account.
 */
function whm_terminate_account($user) {
  if (empty($user)) {
    return WHMConstant::WHM_ERROR_VARS;
  }
  $user = urlencode($user);
  $result = _whm_command('removeacct', array('user' => $user));
  if ($result['result'][0]['status'] == 1) {
    return WHMConstant::WHM_SUCCESS;
  }
  return WHMConstant::WHM_ERROR;
}

/**
 * Unsuspend user from current reseller account.
 */
function whm_unsuspend_account($user) {
  if (empty($user)) {
    return WHMConstant::WHM_ERROR_VARS;
  }
  $user = urlencode($user);
  $result = _whm_command('unsuspendacct', array('user' => $user));
  if ($result['result'][0]['status'] == 1) {
    return WHMConstant::WHM_SUCCESS;
  }
  return WHMConstant::WHM_ERROR;
}

/**
 * Check availability of username from listaccts (only for current reseller).
 */
function whm_check_username($user) {
  $user = drupal_strtolower(trim($user));

  // Check if username is too long.
  if (drupal_strlen($user) > WHMConstant::WHM_MAX_USERNAME_LEN) {
    return WHMConstant::WHM_USERNAME_TOOLONG;
  }

  // Check username for non-alphanumeric characters.
  $validchr = 'abcdefghijklmnopqrstuvwxyz0123456789';
  for ($i = 0; $i < drupal_strlen($user); $i++) {
    if (strpos($validchr, $user[$i]) === FALSE) {
      return WHMConstant::WHM_USERNAME_INVALID;
    }
  }

  // Check if username is already taken.
  $accts = whm_list_accounts();
  foreach ($accts as $acct) {
    if ($acct['user'] == $user) {
      return WHMConstant::WHM_USERNAME_TAKEN;
    }
  }
  return WHMConstant::WHM_SUCCESS;
}

/**
 * Check availability of domain from listaccts (only for current reseller).
 */
function whm_check_domain($domain) {
  $domain = drupal_strtolower(trim($domain));

  // Check domain for valid length (between 2 and 63).
  if ((drupal_strlen($domain) < 2) || (drupal_strlen($domain) > 63)) {
    return WHMConstant::WHM_DOMAIN_INVALID;
  }

  // Check domain for valid characters.
  $validchr = 'abcdefghijklmnopqrstuvwxyz0123456789-.';
  for ($i = 0; $i < drupal_strlen($domain); $i++) {
    if (strpos($validchr, $domain[$i]) === FALSE) {
      return WHMConstant::WHM_DOMAIN_INVALID;
    }
  }

  // Check that domain has at least one period.
  $dot = strpos($domain, '.');
  if ($dot === FALSE) {
    return WHMConstant::WHM_DOMAIN_INVALID;
  }

  // Check that domain does not begin or end with a hyphen or period.
  if (($domain[0] == '.') || ($domain[0] == '-') || ($domain[$dot - 1] == '.') || ($domain[$dot - 1] == '-')) {
    return WHMConstant::WHM_DOMAIN_INVALID;
  }

  // If domain begins with 'www.' strip subdomain.
  if (drupal_substr($domain, 0, 4) === "www.") {
    $domain = drupal_substr($domain, 4);
  }

  // Check if domain is already taken.
  $accts = whm_list_accounts();
  foreach ($accts as $acct) {
    if ($acct['domain'] == $domain) {
      return WHMConstant::WHM_DOMAIN_TAKEN;
    }
  }
  return WHMConstant::WHM_SUCCESS;
}

/**
 * Check e-mail address for proper format and characters.
 */
function whm_check_email($mail) {
  $mail = drupal_strtolower(trim($mail));
  $pos = strpos($mail, '@');
  if (($pos === FALSE) || ($pos === 0)) {
    return WHMConstant::WHM_EMAIL_INVALID;
  }

  $user = drupal_substr($mail, 0, $pos);
  $validchr = "abcdefghijklmnopqrstuvwxyz0123456789!#$%^&*-=_+?{}|~.";
  for ($i = 0; $i < drupal_strlen($user); $i++) {
    if (strpos($validchr, $user[$i]) === FALSE) {
      return WHMConstant::WHM_EMAIL_INVALID;
    }
  }
  if (($user[0] == '.') || ($user[drupal_strlen($user) - 1] == '.')) {
    return WHMConstant::WHM_EMAIL_INVALID;
  }

  $domain = drupal_substr($mail, $pos + 1);
  if (whm_check_domain($domain) !== WHMConstant::WHM_SUCCESS) {
    return WHMConstant::WHM_EMAIL_INVALID;
  }
  return WHMConstant::WHM_SUCCESS;
}

/**
 * Check password for allowed characters.
 */
function whm_check_password($pass) {
  if (drupal_strlen($pass) < WHMConstant::WHM_MAX_PASSWORD_LEN) {
    return WHMConstant::WHM_PASSWORD_INVALID;
  }
  return WHMConstant::WHM_SUCCESS;
}

/**
 * Check user-specified plan type with actual plan choices.
 */
function whm_check_plan($plan) {
  $plan = trim($plan);
  $plans = whm_list_plans();
  foreach ($plans as $plan_data) {
    if ($plan_data['name'] == $plan) {
      return WHMConstant::WHM_SUCCESS;
    }
  }
  return WHMConstant::WHM_ERROR;
}

/**
 * This function lists all hosting packages available for use.
 */
function whm_list_plans() {
  $data = _whm_command('listpkgs');
  return $data['package'];
}

/**
 * This function lists plans matching the given name.
 * 
 * @param string $plan
 *   Name of the plan.
 */
function whm_plan_info($plan) {
  $data = _whm_command('listpkgs');
  $results = $data['package'];
  foreach ($results as $result) {
    if ($result['name'] = $plan) {
      return $result;
    }
  }
  return WHMConstant::WHM_ERROR;
}

/**
 * This function creates a new plan.
 *
 * @param string $name
 *   Name of the plan.
 *
 * @param array $options
 *   Options/variables for plan settings.
 */
function whm_create_plan($name, $options = array()) {
  $options = array(
    'name' => $name,
  ) + $options;
  $result = _whm_command('addpkg', $options);
  if ($result['result'][0]['status'] == 1) {
    return WHMConstant::WHM_SUCCESS;
  }
  return $result['result'][0]['statusmsg'];
}

/**
 * This function modifies an existing plan.
 *
 * @param string $name
 *   Name of the plan.
 *
 * @param array $options
 *   Options/variables for plan settings.
 */
function whm_modify_plan($name, $options = array()) {
  $options = array(
    'name' => $name,
  ) + $options;
  $result = _whm_command('editpkg', $options);
  if ($result['result'][0]['status'] == 1) {
    return WHMConstant::WHM_SUCCESS;
  }
  return $result['result'][0]['statusmsg'];
}

/**
 * This function creates a new plan.
 *
 * @param string $plan_name
 *   Name of the plan.
 */
function whm_delete_plan($plan_name) {
  if (empty($plan_name)) {
    return WHMConstant::WHM_ERROR_VARS;
  }
  $result = _whm_command('killpkg', array('pkg' => urlencode($plan_name)));
  if ($result['result'][0]['status'] == 1) {
    return WHMConstant::WHM_SUCCESS;
  }
  return WHMConstant::WHM_ERROR;
}

/**
 * Get array of possible plan types from WHM.
 */
function whm_list_feature_lists() {
  $data = _whm_command('getfeaturelist');
  $plans = $data['package'];
  $plan_names = array();
  foreach ($plans as $plan) {
    $plan_names[] = $plan['name'];
  }
  return $plan_names;
}

/**
 * Execute WHM script as current reseller account.
 * 
 * Returns error code or response data.
 * 
 * @param string $command
 *   The name of the WHM command resource to resolve.
 * @param array $args
 *   Any optional parameters that maybe passed as args to the target WHM 
 *   command.
 */
function _whm_command($command, $args = array()) {
  $host = variable_get('whm_hostname', '');
  $whmuser = variable_get('whm_username', '');
  $whmauthkey = variable_get('whm_authkey', '');

  if (empty($host) || empty($whmuser) || empty($whmauthkey)) {
    return WHMConstant::WHM_ERROR_VARS;
  }

  $headers = _whm_build_command_headers();
  $url = _whm_build_command_url($command, $args);

  watchdog('whm', "Attempting to resolve $url.", NULL, WATCHDOG_DEBUG);
  watchdog('whm', 'Auth Header: ' . $headers['Authorization'], NULL, WATCHDOG_DEBUG);

  $response = drupal_http_request($url, $options = array('headers' => $headers));
  watchdog('whm', "Resolving URL $url returned status code $response->code.", NULL, WATCHDOG_DEBUG);
  if ($response->code == "401") {
    return WHMConstant::WHM_ERROR_AUTH;
  }
  else {
    $data = json_decode($response->data, TRUE);
  }

  return $data;
}

/**
 * Helper function to build command url.
 */
function _whm_build_command_url($command, $args = array()) {
  $host = variable_get('whm_hostname', '');
  $port = variable_get('whm_port', WHMConstant::WHM_STANDARD_PORT);
  $protocol = $port == WHMConstant::WHM_SECURE_PORT ? 'https' : 'http';
  $url = "$protocol://$host:$port/json-api/$command";
  if (count($args) > 0) {
    $url .= '?';
    foreach ($args as $key => $value) {
      $url .= $key . '=' . $value . '&';
    }
    $url = substr_replace($url, "", -1);
  }
  return $url;
}

/**
 * Helper function to build command headers.
 */
function _whm_build_command_headers() {
  $host = variable_get('whm_hostname', '');
  $whmuser = variable_get('whm_username', '');
  $whmpass = variable_get('whm_password', '');
  $whmauthkey = variable_get('whm_authkey', '');
  $auth_type = 'whm';
  $headers = array();
  if ($auth_type == 'whm') {
    $headers['Authorization'] = "WHM $whmuser:" . preg_replace("'(\r|\n)'", "", $whmauthkey);
  }
  else {
    $headers['Authorization'] = 'Basic ' . base64_encode($whmuser . ":" . $whmpass);
  }
  return $headers;
}

/**
 * Helper function to return plan name as themeable output.
 */
function _whm_theme_plan_name($name) {
  $whmuser = variable_get('whm_username', '');
  return str_replace($whmuser . '_', '', $name);
}
