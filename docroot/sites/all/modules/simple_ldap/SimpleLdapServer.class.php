<?php
/**
 * @file
 * Class to handle LDAP server connections and related operations.
 */

/**
 * Simple LDAP server class.
 */
class SimpleLdapServer {

  // Singleton instance.
  protected static $instance;

  // LDAP connection parameters.
  protected $host;
  protected $port;
  protected $starttls = FALSE;

  // Require LDAPv3.
  protected $version = 3;

  // LDAP directory parameters.
  protected $binddn;
  protected $bindpw;
  protected $basedn;

  // LDAP server type (OpenLDAP, Active Directory, etc.).
  protected $type;

  // LDAP resource link.
  protected $resource = FALSE;
  protected $bound = FALSE;

  // Special LDAP entries.
  protected $rootdse;
  protected $schema;

  // Options.
  protected $pagesize = FALSE;
  protected $readonly;

  /**
   * Singleton constructor.
   *
   * This method should be used whenever a SimpleLdapServer object is needed. By
   * default, a new SimpleLdapServer object is returned, but this can be
   * overridden by setting conf['simple_ldap_server_class'] to an extended class
   * in settings.php.
   *
   * @param boolean $reset
   *   Forces a new object to be instantiated.
   *
   * @return object
   *   SimpleLdapServer object
   *
   * @throw SimpleLdapException
   */
  public static function singleton($reset = FALSE) {
    if ($reset || !isset(self::$instance)) {
      $server_class = variable_get('simple_ldap_server_class', 'SimpleLdapServer');
      self::$instance = new $server_class();
    }

    // Since custom classes are allowed, at least make sure it's a
    // SimpleLdapServer child.
    if (!is_a(self::$instance, 'SimpleLdapServer')) {
      throw new SimpleLdapException('Invalid controller class. Must be of type SimpleLdapServer.');
    }

    return self::$instance;
  }

  /**
   * Constructor.
   *
   * This constructor builds the object by pulling the configuration parameters
   * from the Drupal variable system.
   *
   * @param array $parameters
   *   Optional custom parameters for the server, to programmatically override
   *   any variables from the configuration form.
   */
  public function __construct(array $parameters = array()) {
    // Load up the default parameters.
    $default_parameters = array(
      'host' => variable_get('simple_ldap_host'),
      'port' => variable_get('simple_ldap_port', 389),
      'starttls' => variable_get('simple_ldap_starttls', FALSE),
      'binddn' => variable_get('simple_ldap_binddn'),
      'bindpw' => variable_get('simple_ldap_bindpw'),
      'readonly' => variable_get('simple_ldap_readonly', FALSE),
      'pagesize' => variable_get('simple_ldap_pagesize'),
    );

    // Populate the parameters array with the defaults.
    $parameters += $default_parameters;

    // Clean out anything that isn't one of the above properties, so callers
    // cannot set arbitrary elements on the object.
    $parameters = array_intersect_key($parameters, $default_parameters);

    // Only set the pagesize if paged queries are supported.
    if (!function_exists('ldap_control_paged_result_response') ||
        !function_exists('ldap_control_paged_result')) {
      unset($parameters['pagesize']);
    }

    // Set the object parameters.
    foreach ($parameters as $key => $value) {
      $this->{$key} = $value;
    }

    $this->bind();
  }

  /**
   * Destructor.
   */
  public function __destruct() {
    $this->unbind();
  }

  /**
   * Magic __get() function.
   *
   * @param string $name
   *   Name of the variable to get.
   *
   * @return mixed
   *   Returns the value of the requested variable, if allowed.
   */
  public function __get($name) {
    switch ($name) {
      case 'rootdse':
        // Load the rootDSE.
        $this->rootdse();
        break;

      case 'basedn':
        // Load the baseDN.
        $this->basedn();
        break;

      case 'type':
        // Determine the directory type.
        $this->type();
        break;

      case 'schema':
      case 'subschema':
        // Load the schema.
        $this->schema();
        return $this->schema;

      case 'error':
        return ldap_errno($this->resource);

      // Handle PHP ldap options.
      case 'LDAP_OPT_DEREF':
      case 'LDAP_OPT_SIZELIMIT':
      case 'LDAP_OPT_TIMELIMIT':
      case 'LDAP_OPT_NETWORK_TIMEOUT':
      case 'LDAP_OPT_PROTOCOL_VERSION':
      case 'LDAP_OPT_ERROR_NUMBER':
      case 'LDAP_OPT_REFERRALS':
      case 'LDAP_OPT_RESTART':
      case 'LDAP_OPT_HOST_NAME':
      case 'LDAP_OPT_ERROR_STRING':
      case 'LDAP_OPT_MATCHED_DN':
      case 'LDAP_OPT_SERVER_CONTROLS':
      case 'LDAP_OPT_CLIENT_CONTROLS':
        $this->connect();
        $result = SimpleLdap::ldap_get_option($this->resource, constant($name), $value);
        if ($result !== FALSE) {
          return $value;
        }
        return FALSE;
    }

    return $this->$name;
  }

  /**
   * Magic __set() function, handles changing server settings.
   *
   * @param string $name
   *   The name of the attribute to set.
   * @param mixed $value
   *   The value to assigned to the given attribute.
   */
  public function __set($name, $value) {
    switch ($name) {
      case 'host':
      case 'port':
      case 'starttls':
        $this->disconnect();
      case 'binddn':
      case 'bindpw':
        $this->unbind();
      case 'pagesize':
        $this->$name = $value;
        break;

      // Handle PHP LDAP options.
      case 'LDAP_OPT_DEREF':
      case 'LDAP_OPT_SIZELIMIT':
      case 'LDAP_OPT_TIMELIMIT':
      case 'LDAP_OPT_NETWORK_TIMEOUT':
      case 'LDAP_OPT_ERROR_NUMBER':
      case 'LDAP_OPT_REFERRALS':
      case 'LDAP_OPT_RESTART':
      case 'LDAP_OPT_HOST_NAME':
      case 'LDAP_OPT_ERROR_STRING':
      case 'LDAP_OPT_MATCHED_DN':
      case 'LDAP_OPT_SERVER_CONTROLS':
      case 'LDAP_OPT_CLIENT_CONTROLS':
        $this->connect();
        SimpleLdap::ldap_get_option($this->resource, constant($name), $old_value);
        $result = SimpleLdap::ldap_set_option($this->resource, constant($name), $value);
        if ($result && $old_value != $value) {
          $this->unbind();
        }
        break;

      // LDAPv3 is required, do not allow it to be changed.
      case 'LDAP_OPT_PROTOCOL_VERSION':
        return FALSE;

      default:
    }
  }

  /**
   * Connect and bind to the LDAP server.
   *
   * @param mixed $binddn
   *   Use the given DN while binding. Use NULL for an anonymous bind.
   * @param mixed $bindpw
   *   Use the given password while binding. Use NULL for an anonymous bind.
   * @param boolean $rebind
   *   Reset the object's bind credentials to those provided. Otherwise, just
   *   bind to verify that the credentials are valid.
   *
   * @return boolean
   *   TRUE on success, FALSE on failure.
   */
  public function bind($binddn = FALSE, $bindpw = FALSE, $rebind = FALSE) {
    // Connect first.
    try {
      $this->connect();
    } catch (SimpleLdapException $e) {
      return FALSE;
    }

    // Reset bind DN if provided, and reset is specified.
    if ($rebind && $binddn !== FALSE && $binddn != $this->binddn) {
      $this->binddn = $binddn;
      $this->bound = FALSE;
    }

    // Reset bind PW if provided, and reset is specified.
    if ($rebind && $bindpw !== FALSE && $bindpw != $this->bindpw) {
      $this->bindpw = $bindpw;
      $this->bound = FALSE;
    }

    // Attempt to bind if not already bound, or rebind is specified, or
    // credentials are given.
    if (!$this->bound || $rebind || $binddn !== FALSE && $bindpw !== FALSE) {

      // Bind to the LDAP server.
      if ($rebind || $binddn === FALSE || $bindpw === FALSE) {
        $this->bound = SimpleLdap::ldap_bind($this->resource, $this->binddn, $this->bindpw);
      }
      else {
        // Bind with the given credentials. This is a temporary bind to verify
        // the password, so $this->bound is reset to FALSE.
        $result = SimpleLdap::ldap_bind($this->resource, $binddn, $bindpw);
        $this->bound = FALSE;
        return $result;
      }

      // If paged queries are enabled, verify whether the server supports them.
      if ($this->bound && $this->pagesize) {
        // Load the rootDSE.
        $this->rootdse();

        // Look for the paged query OID supported control.
        if (!in_array('1.2.840.113556.1.4.319', $this->rootdse['supportedcontrol'])) {
          $this->pagesize = FALSE;
        }
      }

    }

    return $this->bound;
  }

  /**
   * Unbind from the LDAP server.
   *
   * @return boolean
   *   TRUE on success
   *
   * @throw SimpleLdapException
   */
  public function unbind() {
    if ($this->bound) {
      SimpleLdap::ldap_unbind($this->resource);
      $this->bound = FALSE;
    }
    return TRUE;
  }

  /**
   * Search the LDAP server.
   *
   * @param string $base_dn
   *   LDAP search base.
   * @param string $filter
   *   LDAP search filter.
   * @param string $scope
   *   LDAP search scope. Valid values are 'sub', 'one', and 'base'.
   * @param array $attributes
   *   Array of attributes to retrieve.
   * @param int $attrsonly
   *   Set to 1 in order to retrieve only the attribute names without the
   *   values. Set to 0 (default) to retrieve both the attribute names and
   *   values.
   * @param int $sizelimit
   *   Client-side size limit. Set this to 0 to indicate no limit. The server
   *   may impose stricter limits.
   * @param int $timelimit
   *   Client-side time limit. Set this to 0 to indicate no limit. The server
   *   may impose stricter limits.
   * @param int $deref
   *   Specifies how aliases should be handled during the search.
   *
   * @return array
   *   Search results.
   *
   * @throw SimpleLdapException
   */
  public function search($base_dn, $filter = 'objectclass=*', $scope = 'sub', $attributes = array(), $attrsonly = 0, $sizelimit = 0, $timelimit = 0, $deref = LDAP_DEREF_NEVER) {
    // Make sure there is a valid binding.
    $this->bind();

    try {
      // Use a post-test loop (do/while) because this will always be done once.
      // It will only loop if paged queries are supported/enabled, and more than
      // one page is available.
      $entries = array('count' => 0);
      $cookie = '';
      do {

        if ($this->pagesize) {
          // Set the paged query cookie.
          SimpleLdap::ldap_control_paged_result($this->resource, $this->pagesize, FALSE, $cookie);
        }

        // Perform the search based on the scope provided.
        switch ($scope) {
          case 'base':
            $result = SimpleLdap::ldap_read($this->resource, $base_dn, $filter, $attributes, $attrsonly, $sizelimit, $timelimit, $deref);
            break;

          case 'one':
            $result = SimpleLdap::ldap_list($this->resource, $base_dn, $filter, $attributes, $attrsonly, $sizelimit, $timelimit, $deref);
            break;

          case 'sub':
          default:
            $result = SimpleLdap::ldap_search($this->resource, $base_dn, $filter, $attributes, $attrsonly, $sizelimit, $timelimit, $deref);
            break;
        }

        if ($this->pagesize) {
          // Merge page into $entries.
          $e = SimpleLdap::ldap_get_entries($this->resource, $result);
          $entries['count'] += $e['count'];
          for ($i = 0; $i < $e['count']; $i++) {
            $entries[] = $e[$i];
          }

          // Get the paged query response cookie.
          SimpleLdap::ldap_control_paged_result_response($this->resource, $result, $cookie);
        }
        else {
          $entries = SimpleLdap::ldap_get_entries($this->resource, $result);
        }

        // Free the query result memory.
        SimpleLdap::ldap_free_result($result);

      } while ($cookie !== NULL && $cookie != '');

    } catch (SimpleLdapException $e) {
      // Error code 32 means there were no matching search results.
      if ($e->getCode() == 32) {
        $entries = array('count' => 0);
      }
      else {
        throw $e;
      }
    }

    // ldap_get_entries returns NULL if ldap_read does not find anything.
    // Reformat the result into something consistent with the other search
    // types.
    if ($entries === NULL) {
      $entries = array('count' => 0);
    }

    return $entries;
  }

  /**
   * Check whether the provided DN exists.
   *
   * @param string $dn
   *   LDAP DN to verify.
   *
   * @return boolean
   *   TRUE if the entry exists, FALSE otherwise.
   *
   * @throw SimpleLdapException
   */
  public function exists($dn) {
    $entry = $this->search($dn, '(objectclass=*)', 'base', array('dn'));
    return $entry['count'] > 0;
  }

  /**
   * Gets a single entry from the LDAP server.
   *
   * @param string $dn
   *   LDAP DN to retrieve.
   *
   * @return array
   *   The LDAP entry.
   *
   * @throw SimpleLdapException
   */
  public function entry($dn) {
    return $this->search($dn, '(objectclass=*)', 'base');
  }

  /**
   * Compare the given attribute value with what is in the LDAP server.
   *
   * @param string $dn
   *   The distinguished name of an LDAP entity.
   * @param string $attribute
   *   The attribute name.
   * @param string $value
   *   The compared value.
   *
   * @return boolen
   *   TRUE if value matches otherwise returns FALSE.
   *
   * @throw SimpleLdapException
   */
  public function compare($dn, $attribute, $value) {
    // Make sure there is a valid binding.
    $this->bind();

    // Do the comparison.
    return SimpleLdap::ldap_compare($this->resource, $dn, $attribute, $value);
  }

  /**
   * Add an entry to the LDAP directory.
   *
   * @param string $dn
   *   The distinguished name of an LDAP entry.
   * @param array $attributes
   *   An array of LDAP attributes and values.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  public function add($dn, $attributes) {
    // Make sure changes are allowed.
    if ($this->readonly) {
      throw new SimpleLdapException('The LDAP Server is configured as read-only');
    }

    // Make sure there is a valid binding.
    $this->bind();

    // Clean up the attributes array.
    $attributes = SimpleLdap::removeEmptyAttributes($attributes);

    // Add the entry.
    return SimpleLdap::ldap_add($this->resource, $dn, $attributes);
  }

  /**
   * Delete an entry from the directory.
   *
   * @param string $dn
   *   The distinguished name of an LDAP entry.
   * @param boolean $recursive
   *   If TRUE, all children of the given DN will be deleted before attempting
   *   to delete the DN.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  public function delete($dn, $recursive = FALSE) {
    // Make sure changes are allowed.
    if ($this->readonly) {
      throw new SimpleLdapException('The LDAP Server is configured as read-only');
    }

    // Make sure there is a valid binding.
    $this->bind();

    // Delete children.
    if ($recursive) {
      $subentries = SimpleLdap::clean($this->search($dn, '(objectclass=*)', 'one', array('dn')));
      foreach ($subentries as $subdn => $entry) {
        $this->delete($subdn, TRUE);
      }
    }

    // Delete the DN.
    return SimpleLdap::ldap_delete($this->resource, $dn);
  }

  /**
   * Modify an LDAP entry.
   *
   * @param string $dn
   *   The distinguished name of an LDAP entry.
   * @param array $attributes
   *   An array of attributes to modify
   * @param string $type
   *   The type of LDAP modification operation to use. Valid values are 'add',
   *   'del' or 'delete', and 'replace'. If unspecified, an object-level modify
   *   is performed.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  public function modify($dn, $attributes, $type = NULL) {
    // Make sure changes are allowed.
    if ($this->readonly) {
      throw new SimpleLdapException('The LDAP Server is configured as read-only');
    }

    // Make sure there is a valid binding.
    $this->bind();

    // Clean up the attributes array.
    $attributes = SimpleLdap::removeEmptyAttributes($attributes, FALSE);

    // Perform the LDAP modify.
    switch ($type) {
      case 'add':
        $result = SimpleLdap::ldap_mod_add($this->resource, $dn, $attributes);
        break;

      case 'del':
      case 'delete':
        $result = SimpleLdap::ldap_mod_del($this->resource, $dn, $attributes);
        break;

      case 'replace':
        $result = SimpleLdap::ldap_mod_replace($this->resource, $dn, $attributes);
        break;

      default:
        $result = SimpleLdap::ldap_modify($this->resource, $dn, $attributes);
    }

    return $result;
  }

  /**
   * Move an entry to a new DN.
   *
   * @param string $dn
   *   The distinguished name of an LDAP entry.
   * @param string $newdn
   *   The new distinguished name of the LDAP entry.
   * @param boolean $deleteoldrdn
   *   If TRUE the old RDN value(s) is removed, else the old RDN value(s) is
   *   retained as non-distinguished values of the entry.
   *
   * @return boolean
   *   TRUE on success
   *
   * @throw SimpleLdapException
   */
  public function move($dn, $newdn, $deleteoldrdn = TRUE) {
    // Make sure changes are allowed.
    if ($this->readonly) {
      throw new SimpleLdapException('The LDAP Server is configured as read-only');
    }

    // Make sure there is a valid binding.
    $this->bind();

    // Parse $newdn into a format that ldap_rename() can use.
    $parts = SimpleLdap::ldap_explode_dn($newdn, 0);
    $rdn = $parts[0];
    $parent = '';
    for ($i = 1; $i < $parts['count']; $i++) {
      $parent .= $parts[$i];
      if ($i < $parts['count'] - 1) {
        $parent .= ',';
      }
    }

    // Move the entry.
    return SimpleLdap::ldap_rename($this->resource, $dn, $rdn, $parent, $deleteoldrdn);
  }

  /**
   * Copy an entry to a new DN.
   *
   * @param string $dn
   *   The distinguished name of an LDAP entry.
   * @param string $newdn
   *   The distinguished name of the new LDAP entry.
   *
   * @return boolean
   *   TRUE on success
   *
   * @throw SimpleLdapException
   */
  public function copy($dn, $newdn) {
    // Get the LDAP entry.
    $entry = $this->search($dn, '(objectclass=*)', 'base');

    // Create the copy.
    $result = $this->add($newdn, $entry[$dn]);

    return $result;
  }

  /**
   * Connect to the LDAP server.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  protected function connect() {
    if ($this->resource === FALSE) {

      // Set up the connection.
      $this->resource = SimpleLdap::ldap_connect($this->host, $this->port);

      // Set the LDAP version.
      SimpleLdap::ldap_set_option($this->resource, LDAP_OPT_PROTOCOL_VERSION, $this->version);

      // Set the advanced LDAP options.
      $opt_referrals = variable_get('simple_ldap_opt_referrals', TRUE);
      SimpleLdap::ldap_set_option($this->resource, LDAP_OPT_REFERRALS, (int) $opt_referrals);

      // StartTLS.
      if ($this->starttls) {
        SimpleLdap::ldap_start_tls($this->resource);
      }

    }

    return TRUE;
  }

  /**
   * Unbind and disconnect from the LDAP server.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  protected function disconnect() {
    $this->unbind();
    $this->resource = FALSE;
    return TRUE;
  }

  /**
   * Loads the server's rootDSE.
   *
   * @throw SimpleLdapException
   */
  protected function rootdse() {
    if (!is_array($this->rootdse)) {
      $attributes = array(
        'vendorName',
        'vendorVersion',
        'namingContexts',
        'altServer',
        'supportedExtension',
        'supportedControl',
        'supportedSASLMechanisms',
        'supportedLDAPVersion',
        'subschemaSubentry',
        'objectClass',
        'rootDomainNamingContext',
      );

      $result = SimpleLdap::clean($this->search('', 'objectclass=*', 'base', $attributes));
      $this->rootdse = $result[''];
    }

  }

  /**
   * Loads the server's schema.
   */
  protected function schema() {
    if (!isset($this->schema)) {
      $this->schema = new SimpleLdapSchema($this);
    }
  }

  /**
   * Attempts to determine the server's baseDN.
   *
   * @return mixed
   *   Returns the LDAP server base DN, or FALSE on failure.
   */
  protected function basedn() {
    // If the baseDN has already been checked, just return it.
    if (isset($this->basedn)) {
      return $this->basedn;
    }

    // Check if the basedn is specified in the module configuration.
    $basedn = variable_get('simple_ldap_basedn');
    if (!empty($basedn)) {
      $this->basedn = $basedn;
      return $this->basedn;
    }

    // The basedn is not specified, so attempt to detect it from the rootDSE.
    try {
      $this->rootdse();
      if (isset($this->rootdse['namingcontexts'])) {
        $this->basedn = $this->rootdse['namingcontexts'][0];
        return $this->basedn;
      }
    } catch (SimpleLdapException $e) {}

    // Unable to determine the baseDN.
    return FALSE;
  }

  /**
   * Attempts to detect the directory type using the rootDSE.
   */
  protected function type() {
    // If the type has already been determined, return it.
    if (isset($this->type)) {
      return $this->type;
    }

    try {
      // Load the rootDSE.
      $this->rootdse();

      // Check for OpenLDAP.
      if (isset($this->rootdse['objectclass']) && is_array($this->rootdse['objectclass'])) {
        if (in_array('OpenLDAProotDSE', $this->rootdse['objectclass'])) {
          $this->type = 'OpenLDAP';
          return $this->type;
        }
      }

      // Check for Active Directory.
      if (isset($this->rootdse['rootdomainnamingcontext'])) {
        $this->type = 'Active Directory';
        return $this->type;
      }
    } catch (SimpleLdapException $e) {}

    // Default to generic LDAPv3.
    $this->type = 'LDAP';
    return $this->type;
  }

}
