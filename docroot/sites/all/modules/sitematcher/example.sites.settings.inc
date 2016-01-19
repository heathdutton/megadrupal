<?php

/**
 * @file
 * Example sites settings include for SiteMatcher class.
 *
 * The sites.settings.inc include has the same structures
 * as any settings include, with two exceptions:
 *
 *   1. It is the only mandatory include
 *   Make sure you supply any config data that is not in the actual
 *   settings.php file and not in any other existing include.
 *
 *   2. Section ids must meet conventions.
 *   The array keys of $data must match your actual sites/* folders.
 *   If you site folder id is "example.com", the array key is just this.
 *
 * If you omit the 'regex' definition, the section will be ignored,
 * except for the '*' section.
 *
 * '*' section:
 *   Configuration in this array will be mapped if no other section
 *   id matches the sitename.
 *   This is useful e.g. if you have no explicit database for sites/default
 *   and you want to make sure no one accidentally meets your install.php.
 *
 * '+' section:
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
 *      'site' => 'sitefolder_name', // Mandatory in sites.settings.inc
 *                                   // Omit in any other section.
 *      'includes' => array('fallback'),
 *    ),
 * );
 *
 * // Global includes.
 * $data['+'] = array(
 *    // Any config,
 *    'internals' => array(
 *      'includes' => array('varnish'),
 *    ),
 * );
 *
 * // Standard section, must have regex condition.
 * $data['section1'] = array(
 *   // Data for section1 (settings, conf),
 *   'internals' => array(
 *     'regex' => '#^www\.example\.com$#i', // Mandatory, see readme.
 *      'includes' => array('section1'),
 *   ),
 * );
 */

/**
 * Multisite site example 1
 *
 * Hosts:
 *   Production host: www.mysite1.example.com
 *   Backend host: www.mysite1.example.com:8080
 *   Preprod hosts: www.mysite1-preprod.example.com(:8080)
 *   Stage host: www.mysite1-stage.example.com
 *   Stage varnish test host: www.mysite1-stage.example.com:6081
 */
$data['mysite1'] = array(
  'internals' => array(
    // regex can also be an indexed array of multiple patterns.
    'regex' => '#^((8080|6081)\.)?www\.mysite1(-(stage|preprod))?\.example\.com)$#i',
    'includes' => array('cookies'),
  ),
  'settings' => array(
    'databases' => array (
      'default' => array (
        'default' => array (
          'prefix' => '',
        ),
      ),
    ),
  ),
);


/**
 * Multisite site example 2
 *
 * Type: Main site and subsite, shared user databases.
 *
 * Hosts:
 *   Production host main: www.bigsite1.example.com
 *   Production host subsite: sub.bigsite1.example.com
 *   Preprod host main: www.bigsite1-preprod.example.com
 *   Preprod host subsite: sub.bigsite1-preprod.example.com
 *   Stage host main: www.bigsite1-stage.example.com
 *   Stage host subsite: sub.bigsite1-stage.example.com
 */
$data['bigsite_main'] = array(
  'settings' => array(
    'databases' => array (
      'default' => array (
        'default' => array (
          'prefix' => '',
        ),
      ),
    ),
  ),
  'internals' => array(
    // regex can also be a single string.
    'regex' => array(
      '#^www\.bigsite1\.example\.com$#i',
      '#^www\.bigsite1-(stage|preprod)\.example\.com$#i',
    ),
    'includes' => array('master_slave', 'proxy'),
  ),
),
$data['bigsite_sub'] = array(
  'settings' => array(
    'databases' => array (
      'default' => array (
        'default' => array (
          'prefix' => array(
            'default'   => 'bigsite_sub.',
            'users'     => 'bigsite_main.',
            'sessions'  => 'bigsite_main.',
            'role'      => 'bigsite_main.',
            'authmap'   => 'bigsite_main.',
          ),
        ),
      ),
    ),
  ),
  'internals' => array(
    'regex' => '#^www\.bigsite1(-(stage|preprod))?\.example\.com$#i',
    'includes' => array('master_slave', 'proxy'),
  ),
),
