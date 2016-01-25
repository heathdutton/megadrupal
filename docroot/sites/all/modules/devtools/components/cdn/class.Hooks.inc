<?php
namespace devtools\components\cdn;

class Hooks extends \devtools\Hooks {
  /**
   * Extends hook_menu().
   */
  public function hook_menu() {
    $items = array();

    $items['admin/config/development/cdn/touch'] = array(
      'title' => 'Touch',
      'access arguments' => array(CDN_PERM_ADMIN),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('devtools_cdn_admin_touch_form'),
      'type' => MENU_LOCAL_TASK,
      'file' => 'components/cdn/cdn.forms.touch.inc',
      'weight' => 10,
    );

    $items['admin/config/development/cdn/cloudfront'] = array(
      'title' => 'CloudFront',
      'access arguments' => array(CDN_PERM_ADMIN),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('devtools_cdn_admin_cloudfront_form'),
      'type' => MENU_LOCAL_TASK,
      'file' => 'components/cdn/cdn.forms.cloudfront.inc',
      'weight' => 12,
    );

    $items['admin/config/development/cdn/cloudfront/queue'] = array(
      'title' => 'CloudFront Invalidation Queue',
      'access arguments' => array(CDN_PERM_ADMIN),
      'page callback' => 'devtools_cdn_admin_cloudfront_queue',
      'type' => MENU_CALLBACK,
      'file' => 'components/cdn/cdn.callbacks.inc',
      'weight' => 12,
    );

    return $items;
  }
}