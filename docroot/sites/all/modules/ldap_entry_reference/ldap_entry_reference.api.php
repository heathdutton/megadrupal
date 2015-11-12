<?php

/**
 * @file
 * API documentation for LDAP Entry Reference.
 */

/**
 * Search plugin execute callback.
 *
 * @param $params
 *   The LDAP search parameters:
 *   - search_plugin: (Drupal) The search plugin id
 *   - entry_type: (Drupal) The Drupal type of entry being queried - user, og, role.
 *   - stored_attr: (Drupal) The attribute that is stored in the field type - always dn.
 *
 *   - base_dn: The base dn of the search. If empty
 *   - search_attr: The attribute being searched, ie 'cn'.
 *   - filter: The combined LDAP filter string for the search. This includes 'extra_filter'.
 *   - extra_filter: An additional LDAP filter string, ie "objectClass=groupOfNames".
 *   - attributes: The attributes to return.
 *   - attrsonly:
 *       0 (default): to retrieve both the attribute names and values
 *       1: To retrieve only the attribute names without the
 *   - scope: The scope of the search - 'base', 'one', 'sub'.
 *   - sizelimit: Client-side size limit, 0 indicates no limit. The server
 *     may impose stricter limits.
 *   - timelimit: Client-side time limit, 0 indicates no limit. The server
 *     may impose stricter limits.
 *   - deref: Specifies how aliases should be handled during the search.
 *
 * @return
 *  An array of LDAP search results, example:
 *  array(
 *    count => 1,
 *    0 => array(
 *      'cn' => array(
 *          'count' => 1,
 *          0 => 'group01-members',
 *      ),
 *      0 => 'cn',
 *      'count' => 1,
 *      'dn' => 'cn=group01-members,ou=website,dc=ldap,dc=example,dc=com'
 *   )
 * )
 *
 * @ingroup ldap_entry_reference_search
 */
function CALLBACK_ldap_entry_reference_execute_callback($params) {
  // No example.
}

/**
 * Search plugin status callback.
 *
 * @param $params
 *   The LDAP search parameters:
 *   - base_dn: The base dn of the search. If empty
 *   - entry_type: The Drupal type of entry being queried - user, og, role.
 *
 * @return
 *   TRUE if can perform search
 *
 * @ingroup ldap_entry_reference_search
 */
function CALLBACK_ldap_entry_reference_status_callback($params) {
  // No example.
}
