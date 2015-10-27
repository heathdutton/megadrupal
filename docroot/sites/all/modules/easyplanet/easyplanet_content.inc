<?php

/**
 * @file
 * This file is used to control output to the user
 */

/**
 * Page Title
 */
function easyplanet_page_title()
{
  $page_title = variable_get('easyplanet_page_title', NULL);
  if ($page_title != NULL && $page_title != '') {
    $site_name = $page_title;
  }
  else {
    $site_name = variable_get('site_name', 'Drupal') . ' Planet';
  }
  return $site_name;
}

/**
 * Page Content
 */
function easyplanet_page_content() {
  drupal_set_title(easyplanet_page_title());
  drupal_add_css(drupal_get_path('module', 'easyplanet') . '/easyplanet-style.css');
  drupal_add_feed(url('planet/rss', array('absolute' => TRUE)), easyplanet_page_title());

  $feeds = easyplanet_get_items();

  $planet = '';

  foreach ($feeds as $feed) {
    if (isset($feed['content'])) {
      $planet .= easyplanet_content_heading($feed['time'], $lastday);
      $planet .= theme('easyplanet_page',
                       array('title' => $feed['title'],
                             'url' => $feed['url'],
                             'heading' => $feed['heading'],
                             'link' => $feed['link'],
                             'content' => $feed['content'],
                             'time' => $feed['time'],
                             'uid' => $feed['uid']));
    }
  }

  if ($planet == '') {
    $planet = 'No planet content available.';
  }

  return $planet;
}

/**
 * RSS Content
 */
function easyplanet_page_rss() {
  $feeds = easyplanet_get_items();

  foreach ($feeds as $feed) {
    $items .= easyplanet_format_rss_item($feed['heading'], $feed['link'], $feed['content'], $feed['title'], array('pubDate' => date('r', $feed['time'])));
  }

  $url = url('planet/rss', array('absolute' => TRUE));

  $site_name = easyplanet_page_title();

  $page_desc = variable_get('easyplanet_page_desc', NULL);
  if ($page_desc != NULL && $page_desc != '') {
    $description = $page_desc;
  }
  else {
    $description = t('Planet Feeds');
  }

  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?" . ">\n";
  $output .= "<rss version=\"2.0\">\n";
  $output .= format_rss_channel(t('@site_name', array('@site_name' => $site_name)), $url, $description, $items);
  $output .= "</rss>\n";

  drupal_add_http_header('Content-type', 'text/xml');

  print $output;
}

function easyplanet_format_rss_item($title, $link, $description, $author, $args = array()) {
  $output = "<item>\n";
  $output .= ' <title>'. check_plain($title) ."</title>\n";
  $output .= ' <link>'. check_url($link) ."</link>\n";
  $output .= ' <description>'. check_plain($description) ."</description>\n";
  $output .= ' <author>'. check_plain($author) ."</author>\n";
  $output .= format_xml_elements($args);
  $output .= "</item>\n";

  return $output;
}


/**
 * Function to pull the planet data from the database
 */
function easyplanet_get_items() {
  $items_limit = variable_get('easyplanet_feed_items', 40);

  $query = db_select('users', 'u');
  $query->join('easyplanet_feeds', 'uf', 'uf.uid=u.uid');
  $query->join('aggregator_feed', 'af', 'af.fid=uf.fid');
  $query->join('aggregator_item', 'ai', 'af.fid=ai.fid');
  $query->range(0, $items_limit);
  $items = $query->fields('u', array('uid'))
    ->fields('af', array('title', 'url'))
    ->fields('ai', array('title', 'link', 'description', 'timestamp'))
    ->condition('uf.disabled', FALSE)
    ->condition('uf.frozen', FALSE)
    ->isNotNull('ai.timestamp')
    ->orderBy('timestamp', 'DESC')
    ->execute();

  $feeds = array();

  foreach ($items as $item) {
    $feeds[] = array(
      'title' => $item->title,
      'url' => $item->url,
      'heading' => $item->ai_title,
      'link' => $item->link,
      'content' => $item->description,
      'time' => $item->timestamp,
      'uid' => $item->uid,
    );
  }

  return $feeds;
}

/**
 * Function to manage change in heading
 */
function easyplanet_content_heading($time, &$day_last) {
  $day_this = format_date($time, 'custom', 'F d, Y');
  if ($day_last != $day_this) {
    $day_last = $day_this;
    $output = theme('easyplanet_heading', array('date' => $day_this));
  }

  return $output;
}

/**
 * Function that returns block content
 *
 * This is somewhat cluttered and ugly but does what it's supposed to.
 */
function easyplanet_block_content() {
  drupal_add_css(drupal_get_path('module', 'easyplanet') . '/easyplanet-style.css');

  $query = db_select('aggregator_feed', 'af');
  $query->join('easyplanet_feeds', 'uf', 'af.fid=uf.fid');
  $feeds = $query->fields('af')
    ->fields('uf', array('uid'))
    ->orderBy('title')
    ->execute();

  $block_content = '<div id="udp-rightColumn">';

  if (variable_get('easyplanet_block_feeds', TRUE)) {
    $block_content .= theme('easyplanet_sidebar_feeds_open');
    foreach ($feeds as $feed) {
      $user = user_load($feed->uid);
      $picture = "";
      if (!empty($user->picture->fid)) {
        $picture = file_create_url(file_load($user->picture->fid)->uri);
      }
      # $picture = $user['picture'];
      $block_content .= theme('easyplanet_sidebar_feeds_list',
                              array(
                                    'picture' => $picture,
                                    'title' => $feed->title,
                                    'feed' => $feed->url,
                                    'link' => $feed->link));
    }
    $block_content .= theme('easyplanet_sidebar_feeds_close');
  }

  $block_content .= '</div>';

  return $block_content;
}
