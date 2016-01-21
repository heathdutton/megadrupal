<?php

/**
 * @file
 * Example settings include for DrupalSiteMatcher class.
 *
 * The sites.settings.inc include has the same structures
 * as any settings include, with three exceptions:
 *
 * 1. It is the only mandatory include
 *   Make sure you supply any config data that is not in the actual
 *   settings.php file and not in any other existing include.
 *
 * 2. Section ids must meet conventions.
 *   The array keys of $data must match your actual sites/* folders.
 *   If you site folder id is "example.com", the array key is just this.
 *
 * 3. Internal sitename must be defined for fallback section.
 *   If you define a fallback section ('*'), you must define a 'site'
 *   value in the 'internals' sub-array (see example below).
 *
 * If you omit the 'regex' definition in the 'internals', sub-array,
 * the section will be ignored, except for the '*' and '+' section ids.
 *
 * '*' section id
 *   Configuration in this array will be mapped if no other section
 *   id matches the sitename.
 * '+' section id
 *   Configuration from here will be mapped in any case, even if
 *   No section matched at all. Use this id for settings shared across
 *   individual sites.
 *
 * Matching paradigma
 *   First section id with a regex matching 'sitename' (host name) wins,
 *   while '*' fallback is last rescue.
 *
 * Structural example:
 *
 * // Optional fallback section.
 * $data['*'] = array(
 *    // Any config,
 *    'internals' => array(
 *      'includes' => array('fallback'),
 *    ),
 * );
 *
 * // Global includes.
 * $data['+'] = array(
 *    // Any config,
 *    'internals' => array(
 *      'includes' => array('global'),
 *    ),
 * );
 *
 * // Standard section, must have regex condition.
 * $data['section1'] = array(
 *   // Data for section1 (settings, conf),
 *   'internals' => array(
 *     'regex' => '#^www\.example\.com$#i', // Mandatory here.
 *     'includes' => array('section1'),
 *   ),
 * );
 */

/**
 * Example 1
 *
 * Varnish backend machines, additional authcache etc. conf entries,
 * global environments configuration, shared cookies.
 */
$data['varnish_machines'] = array(
  'internals' => array(
    // regex can also be an indexed array of multiple patterns.
    'regex' => '#^varnish-#i',
    'includes' => array('authcache', 'expire'),
  ),
  'settings' => array(
    'cookie_domain' => '.example.com',
  ),
  'conf' => array(
    'cache_backends' => array('sites/all/modules/varnish/varnish.cache.inc'),
    'page_cache_invoke_hooks' => FALSE,
    'cache_class_external_varnish_page' => 'VarnishCache',
    'cache_class_cache_page' => 'VarnishCache',
  ),
);

/**
 * Example 2
 * Different cookie domain for any other host.
 */
$data['*'] = array(
  'settings' => array(
    'cookie_domain' => 'other.example.com',
  ),
);

/**
 * Example 3
 * Share environments across all hosts.
 */
$data['+'] = array(
  'internals' => array(
    'includes' => array('environments'),
  ),
);
