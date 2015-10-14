<?php
/**
 * Replace some batch functions.
 */

/**
 * Include sitemap batch library.
 */
require_once drupal_get_path('module', 'xmlsitemap') . '/xmlsitemap.generate.inc';

/**
 * Batch callback; generate all pages of a sitemap.
 */
function mobile_domain_sitemap_xmlsitemap_regenerate_batch_generate($smid, array &$context) {
  if (!isset($context['sandbox']['sitemap'])) {
    $context['sandbox']['sitemap'] = xmlsitemap_sitemap_load($smid);
    $context['sandbox']['sitemap']->chunks = 1;
    $context['sandbox']['sitemap']->links = 0;
    $context['sandbox']['max'] = XMLSITEMAP_MAX_SITEMAP_LINKS;

    // Clear the cache directory for this sitemap before generating any files.
    xmlsitemap_check_directory($context['sandbox']['sitemap']);
    xmlsitemap_clear_directory($context['sandbox']['sitemap']);
  }

  $sitemap = &$context['sandbox']['sitemap'];
  $links = mobile_domain_sitemap_xmlsitemap_generate_page($sitemap, $sitemap->chunks);
  $context['message'] = t('Now generating %sitemap-url.', array('%sitemap-url' => url('sitemap.xml', $sitemap->uri['options'] + array('query' => array('page' => $sitemap->chunks)))));

  if ($links) {
    $sitemap->links += $links;
    $sitemap->chunks++;
  }
  else {
    // Cleanup the 'extra' empty file.
    $file = xmlsitemap_sitemap_get_file($sitemap, $sitemap->chunks);
    if (file_exists($file) && $sitemap->chunks > 1) {
      file_unmanaged_delete($file);
    }
    $sitemap->chunks--;

    // Save the updated chunks and links values.
    $context['sandbox']['max'] = $sitemap->chunks;
    $sitemap->updated = REQUEST_TIME;
    xmlsitemap_sitemap_get_max_filesize($sitemap);
    xmlsitemap_sitemap_save($sitemap);
  }

  if ($sitemap->chunks != $context['sandbox']['max']) {
    $context['finished'] = $sitemap->chunks / $context['sandbox']['max'];
  }
}

/**
 * Generate one page (chunk) of the sitemap.
 *
 * @param $sitemap
 *   An unserialized data array for an XML sitemap.
 * @param $page
 *   An integer of the specific page of the sitemap to generate.
 */
function mobile_domain_sitemap_xmlsitemap_generate_page(stdClass $sitemap, $page) {
  try {
    $writer = new MobileDomainSitemapWriter($sitemap, $page);
    $writer->startDocument();
    $writer->generateXML();
    $writer->endDocument();
  }
  catch (Exception $e) {
    watchdog_exception('xmlsitemap', $e);
    throw $e;
    return FALSE;
  }

  return $writer->getSitemapElementCount();
}

/**
 * Batch callback; generate the index page of a sitemap.
 */
function mobile_domain_sitemap_xmlsitemap_regenerate_batch_generate_index($smid, array &$context) {
  $sitemap = xmlsitemap_sitemap_load($smid);
  if ($sitemap->chunks > 1) {
    mobile_domain_sitemap_xmlsitemap_generate_index($sitemap);
    $context['message'] = t('Now generating sitemap index %sitemap-url.', array('%sitemap-url' => url('sitemap.xml', $sitemap->uri['options'])));
  }
}

/**
 * Generate the index sitemap.
 *
 * @param $sitemap
 *   An unserialized data array for an XML sitemap.
 */
function mobile_domain_sitemap_xmlsitemap_generate_index(stdClass $sitemap) {
  try {
    $writer = new MobileDomainSitemapIndexWriter($sitemap);
    dpm($writer, 'my writer');
    $writer->startDocument();
    $writer->generateXML();
    $writer->endDocument();
  }
  catch (Exception $e) {
    watchdog_exception('xmlsitemap', $e);
    throw $e;
    return FALSE;
  }

  return $writer->getSitemapElementCount();
}
