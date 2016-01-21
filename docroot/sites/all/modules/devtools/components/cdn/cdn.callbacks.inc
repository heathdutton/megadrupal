<?php

/**
 * Callback for /admin/config/development/cdn/cloudfront/queue
 * Shows list of paths in the curent CloudFront invaliadtion queue
 */
function devtools_cdn_admin_cloudfront_queue() {
  $output = '';

  $cdn = devtools\components\CDN::instance();

  if ($cf = $cdn->get('cloudfront')) {
    if ($items = $cf->get_queue()) {
      $header = array(
        t('ID'), t('Date'), t('Status'),
      );

      $rows = array();

      foreach ($items as $item) {
        $item = (array) $item;

        $item['CreateTime'] = date('F j Y, H:ia', strtotime($item['CreateTime']));

        $rows[] = $item;
      }

      $output .= theme('table', array('header' => $header, 'rows' => $rows));
    }
  }

  return $output;
}