<?php

/**
 * @file
 * Plugin that adds a link to the topic that the current topic relates to,
 * if available.
 */

if (module_exists('ns_ch_rss')) {
  $plugin = array(
    'title' => t('RSS Topic add a feed to page'),
    'single' => TRUE,
    'icon' => 'icon_term.png',
    'content_types' => array('ns_core_rss_topic_add_topic_content_type'),
    'description' => t('Add all RSS feeds for a given topis to this panel page'),
    'required context' => new ctools_context_required(t('Term'), array('term', 'taxonomy_term')),
    'category' => t('RSS Feed'),
  );

  function ns_core_rss_topic_add_topic_content_type_render($subtype, $conf, $panel_args, $context) {
    $topic = $context->data;
    if (!$topic) {
      return NULL;
    }
    $rss_feeds = ns_core_rss_topic_get_all_feeds_for_topic($topic);

    foreach ($rss_feeds as $rss_feed) {
      drupal_add_html_head_link($rss_feed);
    }
    return NULL;
  }

  function ns_core_rss_topic_add_topic_content_type_admin_title($subtype, $conf, $context) {
    return t('"@s" rss topics for a given topic', array('@s' => $context->identifier));
  }

  function ns_core_rss_topic_add_topic_content_type_edit_form(&$form, &$form_state) {
    // provide a blank form so we have a place to have context setting.
    return $form;
  }

  function ns_core_rss_topic_get_all_feeds_for_topic($topic) {
    $rss_links = array();
    $query = new EntityFieldQuery();
    $result = $query->entityCondition('entity_type', 'taxonomy_term')
      ->fieldCondition('field_ns_ch_rss_web_topic', 'tid', $topic->tid)
      ->execute();
    if (isset($result['taxonomy_term'])) {
      $term_entities = taxonomy_term_load_multiple(array_keys($result['taxonomy_term']));
      foreach($term_entities as $term) {
      // get all terms or just only thos who referres to actual term.
        $rss_links['ns_core_rss_feed_links_' . $term->tid] = array(
          'href' => base_path() . 'feed/' . $term->tid . '/rss.xml',
          'rel' => 'alternate',
          'type' => 'application/rss+xml',
          'title' => $term->name,
        );
      }
    }

    return $rss_links;
  }
}