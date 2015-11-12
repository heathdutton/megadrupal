<?php
/**
 * @file
 * easy_blog_social.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function easy_blog_social_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
