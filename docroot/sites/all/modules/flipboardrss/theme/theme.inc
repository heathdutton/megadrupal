<?php

/**
 * @file
 * Preprocessors and helper functions to make theming easier.
 */

/**
 * Preprocess the channel elements of an RSS feed
 *
 */
function template_preprocess_flipboardrss_view_rss(&$vars) {
  global $language;

  $view     = &$vars['view'];
  $items    = &$vars['rows'];

  $style    = &$view->style_plugin;

  // SET CHANNEL <title> element.
  if ($view->display_handler->get_option('sitename_title')) {
    $title = variable_get('site_name', 'Drupal');
    if ($slogan = variable_get('site_slogan', '')) {
      $title .= ' - ' . $slogan;
    }
  }
  else {
    $title = $view->get_title();
  }
  $vars['title'] = check_plain($title);

  // SET CHANNEL <link> element.
  // Figure out which display which has a path we're using for this feed. If there isn't
  // one, use the global $base_url
  $link_display_id = $view->display_handler->get_link_display();
  if ($link_display_id && !empty($view->display[$link_display_id])) {
    $path = $view->display[$link_display_id]->handler->get_path();
  }

  if ($path) {
    $path = $view->get_url(NULL, $path);
    $url_options = array('absolute' => TRUE);
    if (!empty($view->exposed_raw_input)) {
      $url_options['query'] = $view->exposed_raw_input;
    }

    // Compare the link to the default home page; if it's the default home page, just use $base_url.
    if ($path == variable_get('site_frontpage', 'node')) {
      $path = '';
    }

    $vars['link'] = check_url(url($path, $url_options));
  }

  // SET CHANNEL <description> element.
  // The RSS 2.0 "spec" doesn't indicate HTML can be used in the description.
  // We strip all HTML tags, but need to prevent double encoding from properly
  // escaped source data (such as &amp becoming &amp;amp;).
  $vars['description'] = check_plain(strip_tags($style->get_description()));

  // SET CHANNEL <language> element.
  $vars['langcode'] = check_plain($language->language);

  // SET CHANNEL other elements.
  $vars['namespaces'] = drupal_attributes($style->namespaces);
  $vars['hub_url'] = variable_get('flipboardrss_hub_url');
  $vars['feed_url'] = variable_get('flipboardrss_feed_url');

  // SET <item> elements from results.
  $vars['items'] = $items;

  // During live preview we don't want to output the header since the contents
  // of the feed are being displayed inside a normal HTML page.
  if(empty($vars['view']->live_preview)) {
    drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
  }
}

/**
 * Preprocess the rows.
 *
 */
function template_preprocess_flipboardrss_view_rss_row(&$vars) {
  $item = &$vars['row'];
  $vars['title'] = check_plain($item->title);
  $vars['link'] = check_url($item->link);
  $vars['item_elements'] = empty($item->elements) ? '' : format_xml_elements($item->elements);

  // Escape description with a CDATA section wrapper.
  $vars['description'] =
    '<![CDATA[' .
    $item->description .
    ']]>';

  // Combine the image inside the top of the content.
  // Escape with a CDATA section wrapper.
  $vars['content_encoded'] =
    '<![CDATA[' .
    "<figure>\n" .
    "$item->image\n" .
    "</figure>\n" .
    $item->content_encoded .
    ']]>';
}
