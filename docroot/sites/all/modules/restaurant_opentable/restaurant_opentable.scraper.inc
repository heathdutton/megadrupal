<?php
/**
 * @file
 * Opentable has no api. So this. Scraper for opentable.com.
 */

/**
 * Scraps data from a restaurant page on opentable.com
 *
 * @param $url
 *  The url of the restaurant page on opentable.com.
 *
 * @return
 *  An array of pages with html data.
 */
function restaurant_opentable_scraper_get_restaurant_data($url) {
  $cid = 'restaurant_opentable_data_' . md5($url);

  if ($cache = cache_get($cid)) {
    return $cache->data;
  }
  else {
    // Load simple_html_dom library.
    require_once(restaurant_opentable_get_simple_html_dom_path());

    // Get content from restaurant page.
    $html = file_get_html($url);

    // Get urls to subpages.
    $pages = array();
    $pages['overview'] = array('url' => $html->find('a#SearchBox_TabItemLink_Overview', 0)->href);

    // Menu subpage.
    if ($html->find('a#SearchBox_TabItemLink_Menu', 0)) {
      $pages['menu'] = array('url' => $html->find('a#SearchBox_TabItemLink_Menu', 0)->href);
    }

    // Reviews subpage.
    if ($html->find('a#SearchBox_TabItemLink_Reviews', 0)) {
      $pages['reviews'] = array('url' => $html->find('a#SearchBox_TabItemLink_Reviews', 0)->href);
    }

    // Get content for each subpages.
    foreach ($pages as $name => $page) {
      $pages[$name]['html'] = file_get_contents($page['url']);
    }

    // Get meta tags.
    foreach ($html->find('meta[property]') as $meta) {
      $pages['meta'][$meta->property] = $meta->content;
    }

    $restaurant_id = $pages['meta']['opentableapps:ot_rest_id'];

    // Reviews from bazaarvoice.
    $bazaarvoice_reviews_url = 'http://opentable.ugc.bazaarvoice.com/0938v2-en_us/' . $restaurant_id . '/reviews.djs?format=embeddedhtml';
    $bazaarvoice_reviews_html = file_get_contents($bazaarvoice_reviews_url);
    $BVRRSecondaryRatingSummarySourceID_start = strpos($bazaarvoice_reviews_html, '"BVRRSecondaryRatingSummarySourceID"') + strlen('"BVRRSecondaryRatingSummarySourceID"');
    $BVRRSecondaryRatingSummarySourceID_finish = strpos($bazaarvoice_reviews_html, '"BVRRSourceID"') - $BVRRSecondaryRatingSummarySourceID_start;
    $rating_summary = substr($bazaarvoice_reviews_html, $BVRRSecondaryRatingSummarySourceID_start, $BVRRSecondaryRatingSummarySourceID_finish);
    $rating_summary = str_replace('\n', "", $rating_summary);
    $rating_html = str_replace("\\", "", $rating_summary);

    $pages['rating_html'] = array(
      'url' => $bazaarvoice_reviews_url,
      'html' => $rating_html,
    );

    // Save to cache.
    cache_set($cid, $pages);

    return $pages;
  }
}
