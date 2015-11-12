<?php
namespace devtools\components;

class CDN extends \devtools\patterns\Singleton {
  protected static $_instance;

  /**
   * Returns an object for the specified CDN.
   * @param  [type] $type [description]
   * @return [type]       [description]
   */
  public function get($type) {
    switch ($type) {
      case 'cloudfront':
        $cf_key = variable_get('devtools_cdn_aws_access_key');
        $cf_sec = variable_get('devtools_cdn_aws_secret_key');
        $cf_did = variable_get('devtools_cdn_aws_distribution_id');

        if (!empty($cf_key) && !empty($cf_sec) && !empty($cf_did)) {
          return new \devtools\components\cdn\CloudFront($cf_key, $cf_sec, $cf_did);
        }

        break;
    }

    return FALSE;
  }

  /**
   * Touches specified path.
   * @param  [type] $path [description]
   * @return [type]       [description]
   */
  public function touch($path) {
    // suppress possible permission errors
    if (@touch($path)) {
      drupal_set_message(t('Successfully touched %path.', array('%path' => $path)));
    }
    else {
      drupal_set_message(t('Failed to touch %path. Do you have sufficient permissions?', array('%path' => $path)), 'error');
    }
  }
}