<?php
/**
 * @file
 * Class to represent an LDAP server schema.
 */

/**
 * Simple LDAP Schema class.
 */
class SimpleLdapSchema {

  protected $dn;
  protected $schema;
  protected $server;
  protected $attributes = array(
    'attributeTypes',
    'dITContentRules',
    'dITStructureRules',
    'matchingRules',
    'matchingRuleUse',
    'nameForms',
    'objectClasses',
    'ldapSyntaxes',
  );

  /**
   * Constructor.
   */
  public function __construct(SimpleLdapServer $server) {
    $this->server = $server;

    // Fallback for broken servers.
    $this->dn = 'cn=Subschema';

    // Load the subschema DN from the server.
    try {
      if (isset($this->server->rootdse['subschemasubentry'])) {
        $this->dn = $this->server->rootdse['subschemasubentry'][0];
      }
    } catch (SimpleLdapException $e) {}

  }

  /**
   * Magic __get function.
   *
   * @param string $name
   *   Name of the variable to get.
   *
   * @return mixed
   *   Returns the value of the requested variable, if allowed.
   */
  public function __get($name) {
    switch ($name) {
      case 'dn':
      case 'attributes':
        return $this->$name;

      case 'entry':
      case 'subentry':
        $this->load();
        return $this->schema;
    }
  }

  /**
   * Magic __set function.
   *
   * @param string $name
   *   The name of the attribute to set.
   * @param mixed $value
   *   The value to assigned to the given attribute.
   */
  public function __set($name, $value) {
    // The schema is read-only, just return.
    return;
  }

  /**
   * Returns whether the given item exists.
   *
   * @param string $attribute
   *   Name of the schema attribute type to check.
   * @param string $name
   *   Name or OID of a single entry to check. If NULL, then this function will
   *   return whether or not the given attribute type is empty.
   *
   * @return boolean
   *   TRUE if the item exists, FALSE otherwise.
   *
   * @throw SimpleLdapException
   */
  public function exists($attribute, $name = NULL) {
    // Make sure the schema for the requested type is loaded.
    $this->load($attribute);

    // Check to see if the requested schema entry exists.
    $attribute = drupal_strtolower($attribute);
    if (isset($this->schema[$attribute])) {
      if ($name === NULL) {
        return (count($this->schema[$attribute]) > 0);
      }
      else {
        if (isset($this->schema[$attribute][drupal_strtolower($name)])) {
          // An attribute of the given name exists.
          return TRUE;
        }
        else {
          // Search for an alias or OID.
          foreach ($this->schema[$attribute] as $attr) {
            foreach ($attr['aliases'] as $alias) {
              if (drupal_strtolower($alias) == drupal_strtolower($name)) {
                return TRUE;
              }
            }
            if ($attr['oid'] == $name) {
              return TRUE;
            }
          }
        }
      }
    }

    return FALSE;
  }

  /**
   * Fetches entries of the given type.
   *
   * @param string $attribute
   *   Name of the schema attribute type to return.
   * @param string $name
   *   If specified, a single entry with this name or OID is returned.
   *
   * @return array
   *   The requested attribute list or entry.
   *
   * @throw SimpleLdapException
   */
  public function get($attribute, $name = NULL) {
    if ($this->exists($attribute, $name)) {
      $attribute = drupal_strtolower($attribute);
      if ($name === NULL) {
        return $this->schema[$attribute];
      }
      else {
        $name = drupal_strtolower($name);
        if (isset($this->schema[$attribute][$name])) {
          // Return a named attribute.
          return $this->schema[$attribute][$name];
        }
        else {
          // Search for an alias or OID.
          foreach ($this->schema[$attribute] as $attr) {
            foreach ($attr['aliases'] as $alias) {
              if (drupal_strtolower($alias) == drupal_strtolower($name)) {
                return $attr;
              }
            }
            if ($attr['oid'] == $name) {
              return $attr;
            }
          }
        }
      }
    }

    throw new SimpleLdapException('The requested entry does not exist: ' . $attribute . ', ' . $name);
  }

  /**
   * Return a list of attributes defined for the objectclass.
   *
   * @param string $objectclass
   *   The objectclass to query for attributes.
   * @param boolean $recursive
   *   If TRUE, the attributes of the parent objectclasses will also be
   *   retrieved.
   *
   * @return array
   *   A list of the MAY/MUST attributes.
   *
   * @throw SimpleLdapException
   */
  public function attributes($objectclass, $recursive = FALSE) {
    $may = $this->may($objectclass, $recursive);
    if (!is_array($may)) {
      $may = array();
    }

    $must = $this->must($objectclass, $recursive);
    if (!is_array($must)) {
      $must = array();
    }

    $attributes = array_merge($may, $must);
    return $attributes;
  }

  /**
   * Return a list of attributes specified as MAY for the objectclass.
   *
   * @param string $objectclass
   *   The objectclass to query for attributes.
   * @param boolean $recursive
   *   If TRUE, the attributes of the parent objectclasses will also be
   *   retrieved.
   *
   * @return array
   *   A list of the MAY attributes.
   *
   * @throw SimpleLdapException
   */
  public function may($objectclass, $recursive = FALSE) {
    $oc = $this->get('objectclasses', $objectclass);
    $may = array();

    if (isset($oc['may'])) {
      $may = $oc['may'];
    }

    if ($recursive && isset($oc['sup'])) {
      foreach ($oc['sup'] as $sup) {
        $may = array_merge($may, $this->may($sup, TRUE));
      }
    }

    return $may;
  }

  /**
   * Return a list of attributes specified as MUST for the objectclass.
   *
   * @param string $objectclass
   *   The objectclass to query for attributes.
   * @param boolean $recursive
   *   If TRUE, the attributes of the parent objectclasses will also be
   *   retrieved.
   *
   * @return array
   *   A list of the MUST attributes.
   *
   * @throw SimpleLdapException
   */
  public function must($objectclass, $recursive = FALSE) {
    $oc = $this->get('objectclasses', $objectclass);
    $must = array();

    if (isset($oc['must'])) {
      $must = $oc['must'];
    }

    if ($recursive && isset($oc['sup'])) {
      foreach ($oc['sup'] as $sup) {
        $must = array_merge($must, $this->must($sup, TRUE));
      }
    }

    return $must;
  }

  /**
   * Returns the objectclass's superclass.
   */
  public function superclass($objectclass, $recursive = FALSE) {
    if ($oc = $this->get('objectclasses', $objectclass)) {
      $superclass = array();

      if (isset($oc['sup'])) {
        $superclass = $oc['sup'];
        if ($recursive) {
          foreach ($oc['sup'] as $sup) {
            $superclass = array_merge($superclass, $this->superclass($sup, TRUE));
          }
        }
      }

      return $superclass;
    }

    return FALSE;
  }

  /**
   * Load the schema.
   *
   * Schema parsing can be slow, so only the attributes that are specified, and
   * are not already cached, are loaded.
   *
   * @param array $attributes
   *   A list of attributes to load. If not specified, all attributes are
   *   loaded.
   *
   * @throw SimpleLdapException
   */
  protected function load($attributes = NULL) {
    // If no attributes are specified, default to all attributes.
    if ($attributes === NULL) {
      $attributes = $this->attributes;
    }

    // Make sure $attributes is an array.
    if (!is_array($attributes)) {
      $attributes = array($attributes);
    }

    // Determine which attributes need to be loaded.
    $load = array();
    foreach ($attributes as $attribute) {
      $attribute = drupal_strtolower($attribute);
      if (!isset($this->schema[$attribute])) {
        $load[] = $attribute;
      }
    }

    // Load the attributes.
    if (!empty($load)) {
      $result = SimpleLdap::clean($this->server->search($this->dn, 'objectclass=*', 'base', $load));

      // Parse the schema.
      foreach ($load as $attribute) {
        $attribute = drupal_strtolower($attribute);
        $this->schema[$attribute] = array();

        // Get the values for each attribute.
        if (isset($result[$this->dn][$attribute])) {
          foreach ($result[$this->dn][$attribute] as $value) {
            $parsed = $this->parse($value);
            $this->schema[$attribute][drupal_strtolower($parsed['name'])] = $parsed;
          }
        }
      }

    }

  }

  /**
   * Parse a schema value into a usable array.
   *
   * @link
   *   http://pear.php.net/package/Net_LDAP2/
   *
   * @license
   *   http://www.gnu.org/licenses/lgpl-3.0.txt LGPLv3
   */
  protected function parse($value) {
    // Tokens that have no associated value.
    $novalue = array(
      'single-value',
      'obsolete',
      'collective',
      'no-user-modification',
      'abstract',
      'structural',
      'auxiliary',
    );

    // Tokens that can have multiple values.
    $multivalue = array('must', 'may', 'sup');

    // Initialization.
    $schema_entry = array('aliases' => array());

    // Get an array of tokens.
    $tokens = $this->tokenize($value);

    // Remove left paren.
    if ($tokens[0] == '(') {
      array_shift($tokens);
    }

    // Remove right paren.
    if ($tokens[count($tokens) - 1] == ')') {
      array_pop($tokens);
    }

    // The first token is the OID.
    $schema_entry['oid'] = array_shift($tokens);

    // Loop through the tokens until there are none left.
    while (count($tokens) > 0) {
      $token = drupal_strtolower(array_shift($tokens));
      if (in_array($token, $novalue)) {
        // Single value token.
        $schema_entry[$token] = 1;
      }
      else {
        // This one follows a string or a list if it is multivalued.
        if (($schema_entry[$token] = array_shift($tokens)) == '(') {
          // This creates the list of values and cycles through the tokens until
          // the end of the list is reached ')'.
          $schema_entry[$token] = array();
          while ($tmp = array_shift($tokens)) {
            if ($tmp == ')') {
              break;
            }
            if ($tmp != '$') {
              array_push($schema_entry[$token], $tmp);
            }
          }
        }
        // Create an array if the value should be multivalued but was not.
        if (in_array($token, $multivalue) && !is_array($schema_entry[$token])) {
          $schema_entry[$token] = array($schema_entry[$token]);
        }
      }
    }

    // Get the max length from syntax.
    if (key_exists('syntax', $schema_entry)) {
      if (preg_match('/{(\d+)}/', $schema_entry['syntax'], $matches)) {
        $schema_entry['max_length'] = $matches[1];
      }
    }

    // Force a name.
    if (empty($schema_entry['name'])) {
      $schema_entry['name'] = $schema_entry['oid'];
    }

    // Make one name the default and put the others into aliases.
    if (is_array($schema_entry['name'])) {
      $aliases = $schema_entry['name'];
      $schema_entry['name'] = array_shift($aliases);
      $schema_entry['aliases'] = $aliases;
    }

    return $schema_entry;
  }

  /**
   * Tokenizes the given value into an array of tokens.
   *
   * @link
   *   http://pear.php.net/package/Net_LDAP2/
   *
   * @license
   *   http://www.gnu.org/licenses/lgpl-3.0.txt LGPLv3
   */
  protected function tokenize($value) {
    $tokens = array();
    $matches = array();

    // This one is taken from perl-lap, modified for php.
    $pattern = "/\s* (?:([()]) | ([^'\s()]+) | '((?:[^']+|'[^\s)])*)') \s*/x";

    // This one matches one big pattern wherin only one of the three subpatterns
    // matched. We are interested in the subpatterns that matched. If it matched
    // its value will be non-empty and so it is a token. Tokens may be round
    // brackets, a string, or a string enclosed by "'".
    preg_match_all($pattern, $value, $matches);

    // Loop through all tokens (full pattern match).
    for ($i = 0; $i < count($matches[0]); $i++) {
      // Loop through each sub-pattern.
      for ($j = 1; $j < 4; $j++) {
        // Pattern match in this sub-pattern.
        $token = trim($matches[$j][$i]);
        if (!empty($token)) {
          $tokens[$i] = $token;
        }
      }
    }

    return $tokens;
  }
}
