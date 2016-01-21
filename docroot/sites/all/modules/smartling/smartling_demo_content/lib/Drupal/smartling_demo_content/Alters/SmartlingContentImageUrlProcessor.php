<?php

/**
 * @file
 * Class SmartlingContentImageUrlProcessor.
 */

namespace Drupal\smartling_demo_content\Alters;

use Drupal\smartling\Alters\SmartlingContentProcessorInterface;

if (!function_exists('http_build_url')) {
  require_once dirname(__FILE__) . '/http_build_url_function.php';
}

/**
 * Demo url processor. No real value here for now.
 */
class SmartlingContentImageUrlProcessor implements SmartlingContentProcessorInterface {

  /**
   * Process.
   *
   * @param string $item
   *   Item.
   * @param array $context
   *   Context.
   * @param string $lang
   *   Locale in drupal format (ru, en).
   * @param string $field_name
   *   Field name.
   * @param object $entity
   *   Entity object.
   */
  public function process(&$item, $context, $lang, $field_name, $entity) {
    if (!$context['external'] || $item[1] != 'href') {
      return;
    }

    $url = $item[2];
    $url = parse_url($url);

    if ($url['host'] == 'www.site.com') {
      $url['host'] = 'site.com';
    }

    if ($url['host'] != 'site.com') {
      return;
    }

    $url['host'] = $lang . '.' . $url['host'];
    $url = http_build_url('', $url);
    $item[2] = $url;
  }

}
