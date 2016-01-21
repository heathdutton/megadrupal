<?php

/**
 * @file
 * Views definitions for yoast_seo.
 */

/**
 * Implements hook_views_data().
 */
function yoast_seo_views_data() {
  $data = array();
  $data['node']['yoast_seo_score'] = array(
    'title' => t('SEO score'),
    'help' => t('SEO score'),
    'field' => array(
      'handler' => 'yoast_seo_handler_score',
    ),
  );
  return $data;
}
