<?php

/**
 * @file
 * Plugin that adds rss topic links for all topics.
 */

if (module_exists('ns_ch_rss')) {
  $plugin = array(
    'title' => t('RSS Topic add all feeds to page'),
    'single' => TRUE,
    'icon' => 'icon_term.png',
    'content_types' => array('ns_core_rss_topic_add_all_topics_content_type'),
    'description' => t('Add all RSS feeds to this panel page'),
  //  'required context' => new ctools_context_required(t('Term'), array('term', 'taxonomy_term')),
    'category' => t('RSS Feed'),
  );

  function ns_core_rss_topic_add_all_topics_content_type_render($subtype, $conf, $panel_args, $context) {
    $rss_feeds = ns_core_rss_topic_get_all_feeds();
    foreach($rss_feeds as $rss_feed) {
      drupal_add_html_head_link($rss_feed);
    }
    return NULL;
  }

  function ns_core_rss_topic_add_all_topics_content_type_admin_title($subtype, $conf, $context) {
    return t('All RSS Topics');
  }

  function ns_core_rss_topic_add_all_topics_content_type_edit_form(&$form, &$form_state) {
    // provide a blank form so we have a place to have context setting.
    return $form;
  }


  function ns_core_rss_topic_get_all_feeds() {
    $rss_links = array();

    $vocabulary = taxonomy_vocabulary_machine_name_load('ns_ch_rss_topic');
    $terms = taxonomy_get_tree($vocabulary->vid);

    foreach($terms as $term){
    // get all terms or just only thos who referres to actual term.
      $rss_links['ns_core_rss_feed_links_' . $term->tid] = array(
        'href' => base_path() . 'feed/' . $term->tid . '/rss.xml',
        'rel' => 'alternate',
        'type' => 'application/rss+xml',
        'title' => $term->name,
      );
    }

    return $rss_links;
  }
}
