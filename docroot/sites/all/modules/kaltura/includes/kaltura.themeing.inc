<?php
/**
 * @file
 * Defines all themeing functions for kaltura core module.
 */

/**
 * Theme the kaltura maintenance page.
 *
 * This page is displayed whenever a try to create the $kaltura_client object
 * fails.
 */
function theme_kaltura_maintenance_page($variables) {
  return $variables['text'] . '<br /><a href="#" onclick="window.top.kalturaCloseModalBox()">' . t('Close') . '</a>';
}
