<?php
/**
 * @file
 * Documentation for the Doorman module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the Doorman URLs which are accessible to anonymous users.
 *
 * @param String $pages
 * The URLs of pages which should be accessible to anonymous users.
 * Takes the same format as used by core's drupal_match_path() function.
 * Enter one path per line. The '*' character is a wildcard. Example paths are
 * %blog for the blog page and %blog-wildcard for every personal blog. %front
 * is the front page.
 */
function hook_doorman_accessible_urls_alter(&$pages) {
  // Allow anonymous users to access the FAQ page.
  $pages .= "\nfaq";
  $pages .= "\nfaq/*";
}

/**
 * @} End of "addtogroup hooks".
 */
