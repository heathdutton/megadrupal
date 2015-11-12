<?php
/**
 * @file
 * seotools.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function seo_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'xmlsitemap_minimum_lifetime';
  $strongarm->value = '86400';
  $export['xmlsitemap_minimum_lifetime'] = $strongarm;

  return $export;
}
