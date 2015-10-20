<?php
/**
 * @file
 * Class defining base Simple LDAP functionallity.
 */

/**
 * Simple LDAP class.
 */
class SimpleLdap {

  /**
   * Cleans up an array returned by the ldap_* functions.
   *
   * @param array $entry
   *   An LDAP entry as returned by SimpleLdapServer::search()
   *
   * @return array
   *   A scrubbed array, with all of the "extra crud" removed.
   *
   * @throw SimpleLdapException
   */
  public static function clean($entry) {
    if (!is_array($entry)) {
      throw new SimpleLdapException('Can only clean an array.');
    }

    $clean = array();

    // Yes, this is ugly, but so are the ldap_*() results.
    for ($i = 0; $i < $entry['count']; $i++) {
      $clean[$entry[$i]['dn']] = array();
      for ($j = 0; $j < $entry[$i]['count']; $j++) {
        $clean[$entry[$i]['dn']][$entry[$i][$j]] = array();
        for ($k = 0; $k < $entry[$i][$entry[$i][$j]]['count']; $k++) {
          $clean[$entry[$i]['dn']][$entry[$i][$j]][] = $entry[$i][$entry[$i][$j]][$k];
        }
      }
    }

    return $clean;
  }

  /**
   * Cleans an attribute array, removing empty items.
   *
   * @param array $attributes
   *   Array of attributes that needs to be cleaned.
   * @param boolean $strip_empty_array
   *   Determines whether an attribute consisting of an empty array should be
   *   stripped or left intact. Defaults to TRUE.
   *
   * @return array
   *   A scrubbed array with no empty attributes.
   */
  public static function removeEmptyAttributes($attributes, $strip_empty_array = TRUE) {
    foreach ($attributes as $key => $value) {
      if (is_array($value)) {
        // Remove empty values.
        foreach ($value as $k => $v) {
          if (empty($v)) {
            unset($attributes[$key][$k]);
          }
        }

        // Remove the 'count' property.
        unset($value['count']);
        unset($attributes[$key]['count']);

        // Remove attributes with no values.
        if ($strip_empty_array && count($attributes[$key]) == 0) {
          unset($attributes[$key]);
        }
      }
    }

    return $attributes;
  }

  /**
   * UTF8-encode an attribute or array of attributes.
   */
  public static function utf8encode($attributes) {
    // $attributes is expected to be an associative array.
    if (!is_array($attributes) || array_key_exists(0, $attributes)) {
      return FALSE;
    }

    // Make sure the schema is loaded.
    $this->schema();

    // Loop through the given attributes.
    $utf8 = array();
    foreach ($attributes as $attribute => $value) {

      // Verify the schema entry for the current attribute is supposed to be
      // utf8 encoded. This is specified by a syntax OID of
      // 1.3.6.1.4.1.1466.115.121.1.15
      $attributetype = $this->schema->get('attributetypes', $attribute);
      if (isset($attributetype['syntax']) && $attributetype['syntax'] == '1.3.6.1.4.1.1466.115.121.1.15') {
        $utf8[$attribute] = utf8_encode($value);
      }
      else {
        $utf8[$attribute] = $value;
      }

    }

    return $utf8;
  }

  /**
   * UTF8-decode an attribute or array of attributes.
   */
  public static function utf8decode($attributes) {
    // $attributes is expected to be an associative array.
    if (!is_array($attributes) || array_key_exists(0, $attributes)) {
      return FALSE;
    }

    // Make sure the schema is loaded.
    $this->schema();

    // Loop through the given attributes.
    $utf8 = array();
    foreach ($attributes as $attribute => $value) {

      // Verify the schema entry for the current attribute is supposed to be
      // utf8 encoded. This is specified by a syntax OID of
      // 1.3.6.1.4.1.1466.115.121.1.15
      $attributetype = $this->schema->get('attributetypes', $attribute);
      if (isset($attributetype['syntax']) && $attributetype['syntax'] == '1.3.6.1.4.1.1466.115.121.1.15') {
        $utf8[$attribute] = utf8_decode($value);
      }
      else {
        $utf8[$attribute] = $value;
      }

    }

    // Return the utf8-decoded array.
    return $utf8;
  }

  /**
   * Generates a random salt of the given length.
   */
  public static function salt($length) {
    $possible = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ./';
    $str = '';

    mt_srand((double) microtime() * 1000000);
    while (strlen($str) < $length) {
      $str .= substr($possible, (rand() % strlen($possible)), 1);
    }

    return $str;
  }

  /**
   * Hash a string for use in an LDAP password field.
   */
  public static function hash($string, $algorithm = NULL) {
    switch ($algorithm) {
      case 'crypt':
        $hash = '{CRYPT}' . crypt($string, substr($string, 0, 2));
        break;

      case 'salted crypt':
        $hash = '{CRYPT}' . crypt($string, self::salt(2));
        break;

      case 'extended des':
        $hash = '{CRYPT}' . crypt($string, '_' . self::salt(8));
        break;

      case 'md5crypt':
        $hash = '{CRYPT}' . crypt($string, '$1$' . self::salt(9));
        break;

      case 'blowfish':
        $hash = '{CRYPT}' . crypt($string, '$2a$12$' . self::salt(13));
        break;

      case 'md5':
        $hash = '{MD5}' . base64_encode(pack('H*', md5($string)));
        break;

      case 'salted md5':
        mt_srand((double) microtime() * 1000000);
        $salt = mhash_keygen_s2k(MHASH_MD5, $string, substr(pack('h*', md5(mt_rand())), 0, 8), 4);
        $hash = '{SMD5}' . base64_encode(mhash(MHASH_MD5, $string . $salt) . $salt);
        break;

      case 'sha':
        $hash = '{SHA}' . base64_encode(pack('H*', sha1($string)));
        break;

      case 'salted sha':
        mt_srand((double) microtime() * 1000000);
        $salt = mhash_keygen_s2k(MHASH_SHA1, $string, substr(pack('h*', md5(mt_rand())), 0, 8), 4);
        $hash = '{SSHA}' . base64_encode(mhash(MHASH_SHA1, $string . $salt) . $salt);
        break;

      case 'unicode':
        $string = '"' . $string . '"';
        $length = drupal_strlen($string);
        $hash = NULL;
        for ($i = 0; $i < $length; $i++) {
          $hash .= "{$string{$i}}\000";
        }
        break;

      case 'none':
      default:
        $hash = $string;
    }

    return $hash;
  }

  /**
   * Returns an array of supported hash types.
   *
   * The keys of this array are also the values supported by SimpleLdap::hash().
   * The values are translated, human-readable values.
   */
  public static function hashes() {
    $types = array();

    // Crypt, and Salted Crypt.
    $types['crypt'] = t('Crypt');
    $types['salted crypt'] = t('Salted Crypt');

    // Extended DES.
    if (defined('CRYPT_EXT_DES') || CRYPT_EXT_DES == 1) {
      $types['extended des'] = t('Extended DES');
    }

    // MD5Crypt.
    if (defined('CRYPT_MD5') || CRYPT_MD5 == 1) {
      $types['md5crypt'] = t('MD5Crypt');
    }

    // Blowfish.
    if (defined('CRYPT_BLOWFISH') || CRYPT_BLOWFISH == 1) {
      $types['blowfish'] = t('Blowfish');
    }

    // MD5
    $types['md5'] = t('MD5');

    // SMD5.
    if (function_exists('mhash') && function_exists('mhash_keygen_s2k')) {
      $types['salted md5'] = t('Salted MD5');
    }

    // SHA.
    $types['sha'] = t('SHA');

    // SSHA.
    if (function_exists('mhash') && function_exists('mhash_keygen_s2k')) {
      $types['salted sha'] = t('Salted SHA');
    }

    // Unicode (used by Active Directory).
    $types['unicode'] = t('Unicode');

    return $types;
  }

  /**
   * Wrapper function for ldap_add().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   * @param string $dn
   *   The distinguished name of an LDAP entity.
   * @param array $entry
   *   An array that specifies the information about the entry. The values in
   *   the entries are indexed by individual attributes. In case of multiple
   *   values for an attribute, they are indexed using integers starting with 0.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  public static function ldap_add($link_identifier, $dn, $entry) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$dn' => $dn,
        '$entry' => $entry,
      ));
    }

    // Wrapped function call.
    $return = @ldap_add($link_identifier, $dn, $entry);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier, $dn = @dn, $entry = @entry) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@dn' => print_r($dn, TRUE),
        '@entry' => print_r($entry, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_bind().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   * @param string $bind_rdn
   *   The RDN to bind with. If not specified, and anonymous bind is attempted.
   * @param string $bind_password
   *   The password to use during the bind.
   *
   * @return boolean
   *   Returns TRUE on success or FALSE on failure.
   */
  public static function ldap_bind($link_identifier, $bind_rdn = NULL, $bind_password = NULL) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$bind_rdn' => $bind_rdn,
        '$bind_password' => $bind_password,
      ));
    }

    // Wrapped function call.
    $return = @ldap_bind($link_identifier, $bind_rdn, $bind_password);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier, $bind_rdn = @bind_rdn, $bind_password = @bind_password) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@bind_rdn' => print_r($bind_rdn, TRUE),
        '@bind_password' => print_r($bind_password, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_unbind().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   *
   * @return boolean
   *   TRUE on success
   *
   * @throw SimpleLdapException
   */
  public static function ldap_unbind($link_identifier) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
    }

    // Wrapped function call.
    $return = @ldap_unbind($link_identifier);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if (!$return) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_compare().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   * @param string $dn
   *   The distinguished name of an LDAP entity.
   * @param string $attribute
   *   The attribute name.
   * @param string $value
   *   The compared value.
   *
   * @return boolean
   *   Returns TRUE if value matches otherwise returns FALSE.
   *
   * @throw SimpleLdapException
   */
  public static function ldap_compare($link_identifier, $dn, $attribute, $value) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$dn' => $dn,
        '$attribute' => $attribute,
        '$value' => $value,
      ));
    }

    // Wrapped function call.
    $return = @ldap_compare($link_identifier, $dn, $attribute, $value);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier, $dn = @dn, $attribute = @attribute, $value = @value) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@dn' => print_r($dn, TRUE),
        '@attribute' => print_r($attribute, TRUE),
        '@value' => print_r($value, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return == -1) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_connect().
   *
   * @param string $hostname
   *   If you are using OpenLDAP 2.x.x you can specify a URL instead of the
   *   hostname. To use LDAP with SSL, compile OpenLDAP 2.x.x with SSL support,
   *   configure PHP with SSL, and set this parameter as ldaps://hostname/
   * @param int $port
   *   The port to connect to. Not used when using URLs.
   *
   * @return resource
   *   LDAP link identifier
   *
   * @throw SimpleLdapException
   */
  public static function ldap_connect($hostname = NULL, $port = 389) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$hostname' => $hostname,
        '$port' => $port,
      ));
    }

    // Wrapped function call.
    $return = @ldap_connect($hostname, $port);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($hostname = @hostname, $port = @port) returns @return';
      $variables = array(
        '@hostname' => print_r($hostname, TRUE),
        '@port' => print_r($port, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return == FALSE) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_control_paged_result().
   *
   * @param resource $link
   *   An LDAP link identifier.
   * @param int $pagesize
   *   The number of entries by page.
   * @param boolean $iscritical
   *   Indicates whether the pagination is critical of not. If true and if the
   *   server doesn't support pagination, the search will return no result.
   * @param string $cookie
   *   An opaque structure sent by the server.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   *
   * @todo Default values for $pagesize, $iscritical, $cookie.
   */
  public static function ldap_control_paged_result($link, $pagesize, $iscritical, $cookie) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$pagesize' => $pagesize,
        '$iscritical' => $iscritical,
        '$cookie' => $cookie,
      ));
    }

    // Wrapped function call.
    $return = @ldap_control_paged_result($link, $pagesize, $iscritical, $cookie);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link = @link, $pagesize = @pagesize, $iscritical = @iscritical, $cookie = @cookie) returns @return';
      $variables = array(
        '@link' => print_r($link, TRUE),
        '@pagesize' => print_r($pagesize, TRUE),
        '@iscritical' => print_r($iscritical, TRUE),
        '@cookie' => print_r($cookie, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException($link);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_control_paged_result_response().
   *
   * @param resource $link
   *   An LDAP link identifier.
   * @param resouce $result
   *   An LDAP search result identifier.
   * @param string $cookie
   *   An opaque structure sent by the server.
   * @param int $estimated
   *   The estimated number of entries to retrieve.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  public static function ldap_control_paged_result_response($link, $result, &$cookie = NULL, &$estimated = NULL) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$result' => $result,
        '$cookie' => $cookie,
        '$estimated' => $estimated,
      ));
    }

    // Wrapped function call.
    $return = @ldap_control_paged_result_response($link, $result, $cookie, $estimated);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link = @link, $result = @result, $cookie = @cookie, $estimated = @estimated) returns @return';
      $variables = array(
        '@link' => print_r($link, TRUE),
        '@result' => print_r($result, TRUE),
        '@cookie' => print_r($cookie, TRUE),
        '@estimated' => print_r($estimated, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_delete().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   * @param string $dn
   *   The distinguished name of an LDAP entity.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  public static function ldap_delete($link_identifier, $dn) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$dn' => $dn,
      ));
    }

    // Wrapped function call.
    $return = @ldap_delete($link_identifier, $dn);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier, $dn = @dn) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@dn' => print_r($dn, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_free_result().
   *
   * @param resource $result_identifier
   *   LDAP search result identifier.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  public static function ldap_free_result($result_identifier) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
    }

    // Wrapped function call.
    $return = @ldap_free_result($result_identifier);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($result_identifier = @result_identifier) returns @return';
      $variables = array(
        '@result_identifier' => print_r($result_identifier, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException($result_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_get_entries().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   * @param resource $result_identifier
   *   An LDAP search result identifier.
   *
   * @return array
   *   An array of LDAP entries.
   *
   * @throw SimpleLdapException
   */
  public static function ldap_get_entries($link_identifier, $result_identifier) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
    }

    // Wrapped function call.
    $return = @ldap_get_entries($link_identifier, $result_identifier);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier, $result_identifier = @result_identifier) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@result_identifier' => print_r($result_identifier, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_get_option().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   * @param int $option
   *   The parameter option.
   *   @see http://us2.php.net/manual/en/function.ldap-get-option.php
   * @param mixed $retval
   *   This will be set to the option value.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  public static function ldap_get_option($link_identifier, $option, &$retval) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$option' => $option,
        '$retval' => $retval,
      ));
    }

    // Wrapped function call.
    $return = @ldap_get_option($link_identifier, $option, $retval);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier, $option = @option, $retval = @retval) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@option' => print_r($option, TRUE),
        '@retval' => print_r($retval, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_list().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   * @param string $base_dn
   *   The base DN for the directory.
   * @param string $filter
   *   The LDAP filter to apply.
   * @param array $attributes
   *   An array of the required attributes.
   * @param int $attrsonly
   *   Should be set to 1 if only attribute types are wanted. If set to 0 both
   *   attributes types and attribute values are fetched which is the default
   *   behaviour.
   * @param int $sizelimit
   *   Enables you to limit the count of entries fetched. Setting this to 0
   *   means no limit.
   * @param int $timelimit
   *   Sets the number of seconds how long is spend on the search. Setting this
   *   to 0 means no limit.
   * @param int $deref
   *   Specifies how aliases should be handled during the search.
   *
   * @return resource
   *   LDAP search result identifier.
   *
   * @throw SimpleLdapException
   *
   * @todo debug $result
   */
  public static function ldap_list($link_identifier, $base_dn, $filter, $attributes = array(), $attrsonly = 0, $sizelimit = 0, $timelimit = 0, $deref = LDAP_DEREF_NEVER) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$base_dn' => $base_dn,
        '$filter' => $filter,
        '$attributes' => $attributes,
        '$attrsonly' => $attrsonly,
        '$sizelimit' => $sizelimit,
        '$timelimit' => $timelimit,
        '$deref' => $deref,
      ));
    }

    // Wrapped function call.
    $return = @ldap_list($link_identifier, $base_dn, $filter, $attributes, $attrsonly, $sizelimit, $timelimit, $deref);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier, $base_dn = @base_dn, $filter = @filter, $attributes = @attributes, $attrsonly = @attrsonly, $sizelimit = @sizelimit, $timelimit = @timelimit, $deref = @deref) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@base_dn' => print_r($base_dn, TRUE),
        '@filter' => print_r($filter, TRUE),
        '@attributes' => print_r($attributes, TRUE),
        '@attrsonly' => print_r($attrsonly, TRUE),
        '@sizelimit' => print_r($sizelimit, TRUE),
        '@timelimit' => print_r($timelimit, TRUE),
        '@deref' => print_r($deref, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_mod_add().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   * @param string $dn
   *   The distinguished name of an LDAP entity.
   * @param array $entry
   *   An array that specifies the information about the entry. The values in
   *   the entries are indexed by individual attributes. In case of multiple
   *   values for an attribute, they are indexed using integers starting with 0.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  public static function ldap_mod_add($link_identifier, $dn, $entry) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$dn' => $dn,
        '$entry' => $entry,
      ));
    }

    // Wrapped function call.
    $return = @ldap_mod_add($link_identifier, $dn, $entry);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier, $dn = @dn, $entry = @entry) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@dn' => print_r($dn, TRUE),
        '@entry' => print_r($entry, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_mod_del().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   * @param string $dn
   *   The distinguished name of an LDAP entity.
   * @param array $entry
   *   An array that specifies the information about the entry. The values in
   *   the entries are indexed by individual attributes. In case of multiple
   *   values for an attribute, they are indexed using integers starting with 0.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  public static function ldap_mod_del($link_identifier, $dn, $entry) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$dn' => $dn,
        '$entry' => $entry,
      ));
    }

    // Wrapped function call.
    $return = @ldap_mod_del($link_identifier, $dn, $entry);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier, $dn = @dn, $entry = @entry) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@dn' => print_r($dn, TRUE),
        '@entry' => print_r($entry, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_mod_replace().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   * @param string $dn
   *   The distinguished name of an LDAP entity.
   * @param array $entry
   *   An array that specifies the information about the entry. The values in
   *   the entries are indexed by individual attributes. In case of multiple
   *   values for an attribute, they are indexed using integers starting with 0.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  public static function ldap_mod_replace($link_identifier, $dn, $entry) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$dn' => $dn,
        '$entry' => $entry,
      ));
    }

    // Wrapped function call.
    $return = @ldap_mod_replace($link_identifier, $dn, $entry);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier, $dn = @dn, $entry = @entry) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@dn' => print_r($dn, TRUE),
        '@entry' => print_r($entry, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_modify().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   * @param string $dn
   *   The distinguished name of an LDAP entity.
   * @param array $entry
   *   An array that specifies the information about the entry. The values in
   *   the entries are indexed by individual attributes. In case of multiple
   *   values for an attribute, they are indexed using integers starting with 0.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  public static function ldap_modify($link_identifier, $dn, $entry) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$dn' => $dn,
        '$entry' => $entry,
      ));
    }

    // Wrapped function call.
    $return = @ldap_modify($link_identifier, $dn, $entry);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier, $dn = @dn, $entry = @entry) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@dn' => print_r($dn, TRUE),
        '@entry' => print_r($entry, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_read().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   * @param string $base_dn
   *   The base DN for the directory.
   * @param string $filter
   *   The LDAP filter to apply.
   * @param array $attributes
   *   An array of the required attributes.
   * @param int $attrsonly
   *   Should be set to 1 if only attribute types are wanted. If set to 0 both
   *   attributes types and attribute values are fetched which is the default
   *   behaviour.
   * @param int $sizelimit
   *   Enables you to limit the count of entries fetched. Setting this to 0
   *   means no limit.
   * @param int $timelimit
   *   Sets the number of seconds how long is spend on the search. Setting this
   *   to 0 means no limit.
   * @param int $deref
   *   Specifies how aliases should be handled during the search.
   *
   * @return resource
   *   LDAP search result identifier.
   *
   * @throw SimpleLdapException
   *
   * @todo debug $result
   */
  public static function ldap_read($link_identifier, $base_dn, $filter, $attributes = array(), $attrsonly = 0, $sizelimit = 0, $timelimit = 0, $deref = LDAP_DEREF_NEVER) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$base_dn' => $base_dn,
        '$filter' => $filter,
        '$attributes' => $attributes,
        '$attrsonly' => $attrsonly,
        '$sizelimit' => $sizelimit,
        '$timelimit' => $timelimit,
        '$deref' => $deref,
      ));
    }

    // Wrapped function call.
    $return = @ldap_read($link_identifier, $base_dn, $filter, $attributes, $attrsonly, $sizelimit, $timelimit, $deref);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier, $base_dn = @base_dn, $filter = @filter, $attributes = @attributes, $attrsonly = @attrsonly, $sizelimit = @sizelimit, $timelimit = @timelimit, $deref = @deref) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@base_dn' => print_r($base_dn, TRUE),
        '@filter' => print_r($filter, TRUE),
        '@attributes' => print_r($attributes, TRUE),
        '@attrsonly' => print_r($attrsonly, TRUE),
        '@sizelimit' => print_r($sizelimit, TRUE),
        '@timelimit' => print_r($timelimit, TRUE),
        '@deref' => print_r($deref, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_search().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   * @param string $base_dn
   *   The base DN for the directory.
   * @param string $filter
   *   The LDAP filter to apply.
   * @param array $attributes
   *   An array of the required attributes.
   * @param int $attrsonly
   *   Should be set to 1 if only attribute types are wanted. If set to 0 both
   *   attributes types and attribute values are fetched which is the default
   *   behaviour.
   * @param int $sizelimit
   *   Enables you to limit the count of entries fetched. Setting this to 0
   *   means no limit.
   * @param int $timelimit
   *   Sets the number of seconds how long is spend on the search. Setting this
   *   to 0 means no limit.
   * @param int $deref
   *   Specifies how aliases should be handled during the search.
   *
   * @return resource
   *   LDAP search result identifier.
   *
   * @throw SimpleLdapException
   *
   * @todo debug $result
   */
  public static function ldap_search($link_identifier, $base_dn, $filter, $attributes = array(), $attrsonly = 0, $sizelimit = 0, $timelimit = 0, $deref = LDAP_DEREF_NEVER) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$base_dn' => $base_dn,
        '$filter' => $filter,
        '$attributes' => $attributes,
        '$attrsonly' => $attrsonly,
        '$sizelimit' => $sizelimit,
        '$timelimit' => $timelimit,
        '$deref' => $deref,
      ));
    }

    // Wrapped function call.
    $return = @ldap_search($link_identifier, $base_dn, $filter, $attributes, $attrsonly, $sizelimit, $timelimit, $deref);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier, $base_dn = @base_dn, $filter = @filter, $attributes = @attributes, $attrsonly = @attrsonly, $sizelimit = @sizelimit, $timelimit = @timelimit, $deref = @deref) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@base_dn' => print_r($base_dn, TRUE),
        '@filter' => print_r($filter, TRUE),
        '@attributes' => print_r($attributes, TRUE),
        '@attrsonly' => print_r($attrsonly, TRUE),
        '@sizelimit' => print_r($sizelimit, TRUE),
        '@timelimit' => print_r($timelimit, TRUE),
        '@deref' => print_r($deref, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_set_option().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   * @param int $option
   *   The parameter option.
   *   @see http://us2.php.net/manual/en/function.ldap-set-option.php
   * @param mixed $newval
   *   The new value for the specified option.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  public static function ldap_set_option($link_identifier, $option, $newval) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$option' => $option,
        '$newval' => $newval,
      ));
    }

    // Wrapped function call.
    $return = @ldap_set_option($link_identifier, $option, $newval);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier, $option = @option, $newval = @newval) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@option' => print_r($option, TRUE),
        '@newval' => print_r($newval, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if (!$return) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_start_tls().
   *
   * @param resource $link
   *   An LDAP link identifier.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  public static function ldap_start_tls($link) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
    }

    // Wrapped function call.
    $return = @ldap_start_tls($link);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link = @link) returns @return';
      $variables = array(
        '@link' => print_r($link, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException($link);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_rename().
   *
   * @param resource $link_identifier
   *   An LDAP link identifier.
   * @param string $dn
   *   The distinguished name of an LDAP entity.
   * @param string $newrdn
   *   The new RDN.
   * @param string $newparent
   *   The new parent/superior entry.
   * @param boolean $deleteoldrdn
   *   If TRUE the old RDN value(s) is removed, else the old RDN value(s) is
   *   retained as non-distinguished values of the entry.
   *
   * @return boolean
   *   TRUE on success.
   *
   * @throw SimpleLdapException
   */
  public static function ldap_rename($link_identifier, $dn, $newrdn, $newparent, $deleteoldrdn) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$dn' => $dn,
        '$newrdn' => $newrdn,
        '$newparent' => $newparent,
        '$deleteoldrdn' => $deleteoldrdn,
      ));
    }

    // Wrapped function call.
    $return = @ldap_rename($link_identifier, $dn, $newrdn, $newparent, $deleteoldrdn);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($link_identifier = @link_identifier, $dn = @dn, $newrdn = @newrdn, $newparent = @newparent, $deleteoldrdn = @deleteoldrdn) returns @return';
      $variables = array(
        '@link_identifier' => print_r($link_identifier, TRUE),
        '@dn' => print_r($dn, TRUE),
        '@newrdn' => print_r($newrdn, TRUE),
        '@newparent' => print_r($newparent, TRUE),
        '@deleteoldrdn' => print_r($deleteoldrdn, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException($link_identifier);
    }

    return $return;
  }

  /**
   * Wrapper function for ldap_explode_dn().
   *
   * @param string $dn
   *   The distinguished name of an LDAP entity.
   * @param int $with_attrib
   *   Used to request if the RDNs are returned with only values or their
   *   attributes as well. To get RDNs with the attributes (i.e. in
   *   attribute=value format) set with_attrib to 0 and to get only values set
   *   it to 1.
   *
   * @return array
   *   Returns an array of all DN components. The first element in this array
   *   has count key and represents the number of returned values, next elements
   *   are numerically indexed DN components.
   *
   * @throw SimpleLdapException
   */
  public static function ldap_explode_dn($dn, $with_attrib = 0) {
    // Devel debugging.
    if (variable_get('simple_ldap_devel', FALSE)) {
      dpm(__FUNCTION__);
      dpm(array(
        '$dn' => $dn,
        '$with_attrib' => $with_attrib,
      ));
    }

    // Wrapped function call.
    $return = @ldap_explode_dn($dn, $with_attrib);

    // Debugging.
    if (variable_get('simple_ldap_debug', FALSE)) {
      $message = __FUNCTION__ . '($dn = @dn, $with_attrib = @with_attrib) returns @return';
      $variables = array(
        '@dn' => print_r($dn, TRUE),
        '@with_attrib' => print_r($with_attrib, TRUE),
        '@return' => print_r($return, TRUE),
      );
      watchdog('simple_ldap', $message, $variables, WATCHDOG_DEBUG);
    }

    // Error handling.
    if ($return === FALSE) {
      throw new SimpleLdapException('Invalid parameters were passed to ldap_explode_dn');
    }

    return $return;
  }

}
