<?php

/**
 * @file
 * A panel plugin to provide an RSS link to the blog.
 */

$plugin = array(
  'title' => t('RSS'),
  'description' => t('This is a link to an RSS feed of the blog.'),
  'content_types' => array('mvpcreator_blog_rss'),
  'category' => t('Blog'),
  'single' => TRUE,
);

/**
  * Implementation of hook_CONTENT_TYPE_content_type_render()
  */
function mvpcreator_blog_mvpcreator_blog_rss_content_type_render($subtype, $conf, $panel_args, $context) {
  $sitename = variable_get('site_name', NULL);
  $title = is_null($sitename) ? t('RSS feed') : t('Blog on !sitename', array('!sitename' => $sitename));

  $pane = new stdClass();
  $pane->content = theme_feed_icon(array('title' => $title, 'url' => 'blog/rss'));
  return $pane;
}

