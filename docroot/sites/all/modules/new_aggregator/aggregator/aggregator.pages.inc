<?php
// $Id: aggregator.pages.inc,v 1.6 2008/07/21 12:15:44 aronnovak Exp $

/**
 * @file
 *   User page callbacks for the aggregator module.
 */

/**
 * Menu callback; displays the most recent items gathered from any feed.
 *
 * @return
 *   The items HTML.
 */
function aggregator_page_last() {
  drupal_add_feed(url('aggregator/rss'), variable_get('site_name', 'Drupal') . ' ' . t('aggregator'));
  
  $items = aggregator_feed_items_load('SELECT i.*, n.title AS ftitle, f.link AS flink FROM {aggregator_item} i INNER JOIN {aggregator_feed} f ON i.nid = f.nid INNER JOIN {node} n ON f.nid = n.nid  ORDER BY i.timestamp DESC, i.iid DESC');

  return _aggregator_page_list($items, arg(1));
}

/**
 * Menu callback; displays all the items captured from a particular feed.
 *
 * If there are two arguments then this function is the categorize form.
 *
 * @param $arg1
 *   If there are two arguments then $arg1 is $form_state. Otherwise, $arg1 is $feed.
 * @param $arg2
 *   If there are two arguments then $arg2 is feed.
 * @return
 *   The item's HTML.
 */
function aggregator_page_source($arg1, $arg2 = NULL) {
  // If there are two arguments then this function is the categorize form, and
  // $arg1 is $form_state and $arg2 is $feed. Otherwise, $arg1 is $feed.
  $node = is_array($arg2) || is_object($arg2) ? $arg2 : $arg1;
  $node = (object) $node;
  drupal_set_title(check_markup($node->title, 4));
  $feed_source = theme('aggregator_feed_source', $node);

  // It is safe to include the fid in the query because it's loaded from the
  // database by aggregator_feed_load.
  $items = aggregator_feed_items_load('SELECT * FROM {aggregator_item} WHERE nid = ' . $node->nid . ' ORDER BY timestamp DESC, iid DESC');

  return _aggregator_page_list($items, arg(3), $feed_source);
}

/**
 * Load feed items by passing a SQL query.
 *
 * @param $sql
 *   The query to be executed.
 * @return
 *   An array of the feed items.
 */
function aggregator_feed_items_load($sql) {
  $items = array();
  if (isset($sql)) {
    $rest_of_args = array_slice(func_get_args(), 1);
    $result = pager_query($sql, 20, 0, NULL, $rest_of_args);
    while ($item = db_fetch_object($result)) {
      $result_terms = db_query('SELECT d.name, t.tid FROM {aggregator_item} i LEFT JOIN {term_node} t ON t.nid = i.nid LEFT JOIN {term_data} d ON d.tid = t.tid WHERE t.nid = %d', $item->nid);
      $result_type = db_query('SELECT n.type FROM {node} n WHERE nid = %d', $item->nid);
      $parent_type = db_fetch_array($result_type);
      if (is_array($parent_type)) {
        $item->parent_type = $parent_type['type'];
      }
      $item->terms = array();
      while ($item_terms = db_fetch_object($result_terms)) {
        $item->terms[] = $item_terms;
      }
      $items[$item->iid] = $item;
    }
  }

  return $items;
}

/**
 * Prints an aggregator page listing a number of feed items.
 *
 * Various menu callbacks use this function to print their feeds.
 *
 * @param $items
 *   The items to be listed.
 * @param $op
 *   Which form should be added to the items. Only 'categorize' is now recognized.
 * @param $feed_source
 *   The feed source URL.
 * @return
 *   The items HTML.
 */
function _aggregator_page_list($items, $op, $feed_source = '') {
  // Assemble themed output.
  $output = $feed_source;
  foreach ($items as $item) {
    $output .= theme('aggregator_item', $item);
  }
  $output = theme('aggregator_wrapper', $output);


  return $output;
}

/**
 * Process variables for aggregator-wrapper.tpl.php.
 *
 * @see aggregator-wrapper.tpl.php
 */
function template_preprocess_aggregator_wrapper(&$variables) {
  $variables['pager'] = theme('pager', NULL, 20, 0);
}

/**
 * Process variables for aggregator-item.tpl.php.
 *
 * @see aggregator-item.tpl.php
 */
function template_preprocess_aggregator_item(&$variables) {
  $item = $variables['item'];

  $variables['feed_url'] = check_url($item->link);
  $variables['feed_title'] = check_plain($item->title);
  $variables['content'] = check_markup($item->description, variable_get('aggregator_input_filter_' . $item->parent_type, FILTER_FORMAT_DEFAULT));

  $variables['source_url'] = '';
  $variables['source_title'] = '';
  if (isset($item->ftitle) && isset($item->fid)) {
    $variables['source_url'] = url("aggregator/sources/$item->fid");
    $variables['source_title'] = check_plain($item->ftitle);
  }
  if (date('Ymd', $item->timestamp) == date('Ymd')) {
    $variables['source_date'] = t('%ago ago', array('%ago' => format_interval(time() - $item->timestamp)));
  }
  else {
    $variables['source_date'] = format_date($item->timestamp, 'custom', variable_get('date_format_medium', 'D, m/d/Y - H:i'));
  }

  $variables['terms'] = array();
  foreach ($item->terms as $term) {
    $variables['terms'][$term->tid] = l($term->name, 'aggregator/terms/' . $term->tid);
  }
}

/**
 * Menu callback; displays all the feeds used by the aggregator.
 */
function aggregator_page_sources() {
  // Collect node types that light feeds can be.
  $light_types = array();
  $types = node_get_types();
  foreach ($types as $type) {
  	if (aggregator_is_enabled('aggregator', $type->type)) {
  	  $light_types[] = "'" . $type->type . "'";
  	}
  }
  $output = '';
  if ($light_types) {
    // Query all the feeds inside this content-type.
    $result = db_query('SELECT f.nid, n.title, n.type, MAX(i.timestamp) AS last FROM {aggregator_feed} f LEFT JOIN {aggregator_item} i ON f.nid = i.nid LEFT JOIN {node} n ON f.nid = n.nid WHERE n.type IN (' . implode(', ', $light_types) . ')GROUP BY f.nid, n.title ORDER BY last DESC, n.title');
    while ($feed = db_fetch_object($result)) {
      // Most recent items:
      $summary_items = array();
      if (variable_get('aggregator_summary_items_' . $feed->type, 3)) {
        $items = db_query_range('SELECT i.title, i.timestamp, i.link FROM {aggregator_item} i WHERE i.nid = %d ORDER BY i.timestamp DESC', $feed->nid, 0, variable_get('aggregator_summary_items_' . $feed->type, 3));
        while ($item = db_fetch_object($items)) {
          $summary_items[] = theme('aggregator_summary_item', $item);
        }
      }
      $feed->url = url('aggregator/sources/' . $feed->nid);
      $output .= theme('aggregator_summary_items', $summary_items, $feed);
    }
  }
  $output .= theme('feed_icon', url('aggregator/opml'), t('OPML feed'));
  
  return theme('aggregator_wrapper', $output);
}

/**
 * Menu callback; handle term-pages.
 */
function aggregator_page_terms($tid = NULL) {
  // List all the items for a given term
  if (is_numeric($tid)) {
    $term = taxonomy_get_term($tid);
    drupal_set_title(check_markup($term->name, 4));
    
    drupal_add_feed(url('aggregator/rss/' . $tid), variable_get('site_name', 'Drupal') . ' ' . t('aggregator - @title', array('@title' => $term->name)));
    
    $items = aggregator_feed_items_load('SELECT i.* FROM {aggregator_item} i LEFT JOIN {term_node} t ON i.nid = t.nid WHERE t.tid = %d ORDER BY i.timestamp DESC', $term->tid);
    return _aggregator_page_list($items, arg(3));
  }
  // List an overview of the terms
  else {
    $terms = _aggregator_collect_terms();
    $output = '';
    foreach ($terms as $tid => $name) {
      $summary_items = array();
      // @todo: now it lists the same as in the block. How to do?
      $items = db_query_range('SELECT i.* FROM {aggregator_item} i LEFT JOIN {term_node} t ON i.nid = t.nid WHERE t.tid = %d ORDER BY i.timestamp DESC', $tid, 0, variable_get('aggregator-' . $tid . '-block', 3));
      while ($item = db_fetch_object($items)) {
        $summary_items[] = theme('aggregator_summary_item', $item);
      }
      $term = new stdClass();
      $term->title = $name;
      $term->url = url('aggregator/terms/' . $tid);
      $output .= theme('aggregator_summary_items', $summary_items, $term);

    }
  }
  return theme('aggregator_wrapper', $output);
}

/**
 * Menu callback; generate an RSS 0.92 feed of aggregator items or categories.
 */
function aggregator_page_rss() {
  $result = NULL;
  // arg(2) is the passed cid, only select for that category.
  if (arg(2)) {
    $category = db_fetch_object(db_query('SELECT tid, name AS title FROM {term_data} WHERE tid = %d', arg(2)));
    $sql = 'SELECT i.*, n.title AS ftitle, f.link AS flink FROM {aggregator_item} i LEFT JOIN {node} n ON i.nid = n.nid LEFT JOIN {aggregator_feed} f ON f.nid = n.nid LEFT JOIN {term_node} t ON t.nid = n.nid WHERE t.tid = 4  ORDER BY timestamp DESC, i.iid DESC';
    $result = db_query_range($sql, $category->tid, 0, variable_get('feed_default_items', 10));
  }
  // Or, get the default aggregator items.
  else {
    $category = NULL;
    $sql = 'SELECT i.*, n.title AS ftitle, f.link AS flink FROM {aggregator_item} i INNER JOIN {node} n ON i.nid = n.nid LEFT JOIN {aggregator_feed} f ON n.nid = f.nid ORDER BY i.timestamp DESC, i.iid DESC';
    $result = db_query_range($sql, 0, variable_get('feed_default_items', 10));
  }

  $feeds = array();
  while ($item = db_fetch_object($result)) {
    $feeds[] = $item;
  }

  return theme('aggregator_page_rss', $feeds, $category);
}

/**
 * Theme the RSS output.
 *
 * @param $feeds
 *   An array of the feeds to theme.
 * @param $category
 *   A common category, if any, for all the feeds.
 * @ingroup themeable
 */
function theme_aggregator_page_rss($feeds, $category = NULL) {
  drupal_set_header('Content-Type: application/rss+xml; charset=utf-8');

  $items = '';
  $feed_length = variable_get('feed_item_length', 'teaser');
  foreach ($feeds as $feed) {
    switch ($feed_length) {
      case 'teaser':
        $teaser = node_teaser($feed->description);
        if ($teaser != $feed->description) {
          $teaser .= '<p><a href="' . check_url($feed->link) . '">' . t('read more') . "</a></p>\n";
        }
        $feed->description = $teaser;
        break;
      case 'title':
        $feed->description = '';
        break;
    }
    $items .= format_rss_item($feed->ftitle . ': ' . $feed->title, $feed->link, $feed->description, array('pubDate' => date('r', $feed->timestamp)));
  }

  $site_name = variable_get('site_name', 'Drupal');
  $url = url((isset($category) ? 'aggregator/categories/' . $category->tid : 'aggregator'), array('absolute' => TRUE));
  $description = isset($category) ? t('@site_name - aggregated feeds in category @title', array('@site_name' => $site_name, '@title' => $category->title)) : t('@site_name - aggregated feeds', array('@site_name' => $site_name));

  $output  = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<rss version=\"2.0\">\n";
  $output .= format_rss_channel(t('@site_name aggregator', array('@site_name' => $site_name)), $url, $description, $items);
  $output .= "</rss>\n";

  print $output;
}

/**
 * Menu callback; generates an OPML representation of all feeds.
 *
 * @param $tid
 *   If set, feeds are exported only from a feed with this term. Otherwise, all feeds are exported.
 * @return
 *   The output XML.
 */
function aggregator_page_opml($tid = NULL) {
  if ($tid) {
    $result = db_query('SELECT n.title, f.url FROM {aggregator_feed} f LEFT JOIN {node} n ON n.nid = f.nid LEFT JOIN {term_node} c on f.nid = c.nid WHERE c.tid = %d ORDER BY title', $tid);
  }
  else {
    $result = db_query('SELECT f.*, n.title FROM {aggregator_feed} f LEFT JOIN {node} n ON n.nid = f.nid  ORDER BY title');
  }

  $feeds = array();
  while ($item = db_fetch_object($result)) {
    $feeds[] = $item;
  }

  return theme('aggregator_page_opml', $feeds);
}

/**
 * Theme the OPML feed output.
 *
 * @param $feeds
 *   An array of the feeds to theme.
 * @ingroup themeable
 */
function theme_aggregator_page_opml($feeds) {
  drupal_set_header('Content-Type: text/xml; charset=utf-8');

  $output  = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<opml version=\"1.1\">\n";
  $output .= "<head>\n";
  $output .= '<title>' . check_plain(variable_get('site_name', 'Drupal')) . "</title>\n";
  $output .= '<dateModified>' . gmdate('r') . "</dateModified>\n";
  $output .= "</head>\n";
  $output .= "<body>\n";
  foreach ($feeds as $feed) {
    $output .= '<outline text="' . check_plain($feed->title) . '" xmlUrl="' . check_url($feed->url) . "\" />\n";
  }
  $output .= "</body>\n";
  $output .= "</opml>\n";

  print $output;
}

/**
 * Process variables for aggregator-summary-items.tpl.php.
 *
 * @see aggregator-summary-item.tpl.php
 */
function template_preprocess_aggregator_summary_items(&$variables) {
  $variables['title'] = check_plain($variables['source']->title);
  $variables['summary_list'] = theme('item_list', $variables['summary_items']);
  $variables['source_url'] = $variables['source']->url;
}

/**
 * Process variables for aggregator-summary-item.tpl.php.
 *
 * @see aggregator-summary-item.tpl.php
 */
function template_preprocess_aggregator_summary_item(&$variables) {
  $item = $variables['item'];

  $variables['feed_url'] = check_url($item->link);
  $variables['feed_title'] = check_plain($item->title);
  $variables['feed_age'] = t('%age old', array('%age' => format_interval(time() - $item->timestamp)));

  $variables['source_url'] = '';
  $variables['source_title'] = '';
  if (!empty($item->feed_link)) {
    $variables['source_url'] = check_url($item->feed_link);
    $variables['source_title'] = check_plain($item->feed_title);
  }
}

/**
 * Process variables for aggregator-feed-source.tpl.php.
 *
 * @see aggregator-feed-source.tpl.php
 */
function template_preprocess_aggregator_feed_source(&$variables) {
  $node = $variables['feed'];
  $variables['source_icon'] = theme('feed_icon', $node->feed->url, t('!title feed', array('!title' => $node->title)));
  
  $variables['source_image'] = !empty($node->feed->image) ? '<a href="'. check_url($node->feed->link) .'" class="feed-image"><img src="'. check_url($node->feed->image) .'" alt="'. check_plain($node->title) .'" /></a>' : '';
  $variables['source_description'] = check_markup($node->body, FILTER_FORMAT_DEFAULT);
  $variables['source_url'] = check_url(url($node->feed->link, array('absolute' => TRUE)));

  if ($node->feed->checked) {
    $variables['last_checked'] = t('@time ago', array('@time' => format_interval(time() - $node->feed->checked)));
  }
  else {
    $variables['last_checked'] = t('never');
  }

  if (user_access('administer news feeds')) {
    $variables['last_checked'] = l($variables['last_checked'], 'admin/content/aggregator');
  }
}
