<?php
/**
 * @file
 * Describe hooks provided by the Simple LDAP User module.
 */

/**
 * SimpleLdap Fingerprint.
 *
 * Public static functions
 * -----------------------
 * hash($string, $algorithm = NULL)
 * hashes()
 * salt($length)
 * utf8encode($attributes)
 * utf8decode($attributes)
 * clean($entry)
 * removeEmptyAttributes($attributes)
 *
 * Public static functions (wrappers)
 * ----------------------------------
 * ldap_add($link_identifier, $dn, $entry)
 * ldap_bind($link_identifier, $bind_rdn = NULL, $bind_password = NULL)
 * ldap_compare($link_identifier, $dn, $attribute, $value)
 * ldap_connect($hostname = NULL, $port = 389)
 * ldap_control_paged_result($link, $pagesize, $iscritical, $cookie)
 * ldap_control_paged_result_response($link, $result, &$cookie, &$estimated)
 * ldap_delete($link_identifier, $dn);
 * ldap_free_result($result_identifier)
 * ldap_get_entries($link_identifier, $result_identifier)
 * ldap_get_option($link_identifier, $option, &$retval)
 * ldap_list($link_identifier, $base_dn, $filter, $attributes, $attrsonly,
 *     $sizelimit, $timelimit, $deref)
 * ldap_mod_add($link_identifier, $dn, $entry)
 * ldap_mod_del($link_identifier, $dn, $entry)
 * ldap_mod_replace($link_identifier, $dn, $entry)
 * ldap_modify($link_identifier, $dn, $entry)
 * ldap_read($link_identifier, $base_dn, $filter, $attributes, $attrsonly,
 *     $sizelimit, $timelimit, $deref)
 * ldap_search($link_identifier, $base_dn, $filter, $attributes, $attrsonly,
 *     $sizelimit, $timelimit, $deref)
 * ldap_set_option($link_identifier, $option, $newval)
 * ldap_start_tls($link)
 */

/**
 * SimpleLdapException Fingerprint.
 *
 * @extends Exception
 *
 * Magic functions
 * ---------------
 * __construct()
 */

/**
 * SimpleLdapServer Fingerprint.
 *
 * Variables exposed by __get() and __set()
 * ----------------------------------------
 * $host
 * $port
 * $starttls
 * $version
 * $binddn
 * $bindpw
 * $pagesize
 * $readonly
 *
 * Dynamically loaded in __get()
 * -----------------------------
 * $error
 * $rootdse
 * $[sub]schema
 *
 * Magic functions
 * ---------------
 * __construct()
 * __destruct()
 * __get($name)
 * __set($name, $value)
 *
 * Control functions
 * -----------------
 * bind($binddn = null, $bindpw = null)
 * unbind()
 *
 * Read functions
 * --------------
 * search($base_dn, $filter, $scope = 'sub', $attributes = array(),
 *        $attrsonly = 0, $sizelimit = 0, $timelimit = 0,
 *        $deref = LDAP_DEREF_NEVER)
 * exists($dn)
 * entry($dn)
 * compare($dn, $attribute, $value)
 *
 * Write functions
 * ---------------
 * add($dn, $attributes)
 * delete($dn, $recursive = false)
 * modify($dn, $attributes, $type = FALSE)
 * move($dn, $newdn)
 * copy($dn, $newdn)
 *
 * Private functions
 * -----------------
 * connect()
 * disconnect()
 * rootdse()
 * schema()
 *
 * Public static functions
 * -----------------------
 * singleton($reset = FALSE)
 */

/**
 * SimpleLdapSchema Fingerprint.
 *
 * Variables exposed by __get() and __set()
 * ----------------------------------------
 * $attributes
 * $dn
 *
 * Dynamically loaded in __get()
 * -----------------------------
 * $[sub]entry
 *
 * Magic functions
 * ---------------
 * __construct(SimpleLdapServer $server)
 * __get($name)
 * __set($name, $value)
 *
 * Query functions
 * ---------------
 * exists($attribute, $name = NULL)
 * get($attribute = NULL, $name = NULL)
 *
 * ObjectClass functions
 * ---------------------
 * attributes($objectclass, $recursive = FALSE)
 * may($objectclass, $recursive = FALSE)
 * must($objectclass, $recursive = FALSE)
 * superclass($objectclass, $recursive = FALSE)
 *
 * Potential candidates for future implementation
 * ----------------------------------------------
 * isBinary($attribute)
 * getAssignedOCL($attribute)
 * checkAttribute($attribute, $objectclasses)
 */
