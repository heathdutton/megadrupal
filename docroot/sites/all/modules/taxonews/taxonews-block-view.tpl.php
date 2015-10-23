<?php
/**
 * @file
 * Block template for Taxonews blocks
 *
 * This default template won't link to the "archive" page if there are no
 * nodes in addition to the ones already listed.
 *
 * If a site-specific theme want to display the link anyway, invoke
 * Taxonews::archiveExists passing NULL instead of $items in order to check for
 * the page contents, instad of relying on $archive_exists.
 *
 * Note that in most normal situations, it will exist and contain at least the
 * nodes already listed in the block.
 *
 * @copyright (c) 2009-2010 Ouest Systèmes Informatiques (OSInet)
 * @author Frédéric G. MARAND <fgm@osinet.fr>
 * @license Licensed under the CeCILL 2.0 and General Public License version 2 and later
 * @link http://www.cecill.info/licences/Licence_CeCILL_V2-en.html @endlink
 * @link http://drupal.org/project/taxonews @endlink
 *
 * Available variables:
 * - $empty_message: The text to display if the block has no recent news to display
 * - $archive_exists: boolean. Does an archive page exist ?
 * - $show_archive: boolean. Show the archive link or not ?
 * - $term_path: The path to the archive (i.e. term) page
 * - $show_feed: boolean. Show the RSS feed icon for the block or not ?
 * - $feed_url: The URL for the RSS feed
 * - $feed_name: The name of the RSS feed. Used to build the feed title.
 */

$content = array();
if (empty($ar_items)) {
  if (!empty($empty_message)) {
    $content['items'] = array(
      '#markup'  => $empty_message,
    );
  }
}
else {
  $links = array();
  foreach ($ar_items as $k => $v) {
    $links[] = $v['link'];
  }
  $content['items'] = array(
    '#theme'   => 'item_list',
    '#items'   => $links,
  );
}

if (!empty($content) && $archive_exists && $show_archive) {
  $content['more'] = array(
    '#theme'   => 'taxonews_more_link',
    '#url'     => $term_path,
    '#title'   => t('Archive'),
    '#options' => array('absolute' => TRUE),
  );
}

if (!empty($content) && $show_feed) {
  $content['feed'] = array(
    '#theme'   => 'feed_icon',
    '#url'     => $feed_url,
    '#title'   => t('@name RSS feed', array('@name' => $feed_name)),
  );
  if ($head_feed) {
    drupal_add_feed($feed_url, $content['feed']['#title']);
  }
}

echo drupal_render($content);
