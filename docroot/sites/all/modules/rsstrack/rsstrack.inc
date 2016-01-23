<?php

/**
 * @file
 * RSS Track module menu callbacks and form builders.
 */

/**
 * RSS Track settings form.
 */
function rsstrack_system_feeds_settings() {
  $query = rsstrack_get_node_query_params();

  $form['preview'] = array(
    '#type' => 'item',
    '#title' => t('RSS node URL preview:'),
    '#markup' => '<code>'. url(t('some-node-title'), array('absolute' => TRUE, 'query' => $query)) .'</code>',
  );

  $form['ga'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google Analytics RSS Tracking'),
    '#description' => t('If you want to use GA RSS tracking remember that <b>Campaign Source, Campaign Medium and Campaign Name are required by GA</b>.  For more details about these parameters, please go to <a href="@url">Google Analytics URL Builder</a> page.', array('@url' =>'http://www.google.com/support/googleanalytics/bin/answer.py?hl=en&answer=55578')),
  );
  $form['ga']['rsstrack_utm_source'] = array(
    '#type' => 'textfield',
    '#title' => t('Campaign Source'),
    '#description' => t('<b>Required for GA</b>. The Referrer. Use <b>utm_source</b> to identify a search engine, newsletter name, or other source. <em>Example: google</em>'),
    '#default_value' => variable_get('rsstrack_utm_source', ''),
  );
  $form['ga']['rsstrack_utm_medium'] = array(
    '#type' => 'textfield',
    '#title' => t('Campaign Medium'),
    '#description' => t('<b>Required for GA</b>. The marketing medium. Use <b>utm_medium</b> to identify a medium such as email or cost-per-click. <em>Example: cpc</em>'),
    '#default_value' => variable_get('rsstrack_utm_medium', ''),
  );
  $form['ga']['rsstrack_utm_term'] = array(
    '#type' => 'textfield',
    '#title' => t('Campaign Term'),
    '#description' => t('Used for paid search. Use <b>utm_term</b> to note the keywords for this ad. <em>Example: running+shoes</em>'),
    '#default_value' => variable_get('rsstrack_utm_term', ''),
  );
  $form['ga']['rsstrack_utm_content'] = array(
    '#type' => 'textfield',
    '#title' => t('Campaign Content'),
    '#description' => t('Use <b>utm_content</b> to differentiate ads or links that point to the same URL - for A/B testing and content-targeted ads. Examples: <em>logolink</em> or <em>textlink</em>'),
    '#default_value' => variable_get('rsstrack_utm_content', ''),
  );
  $form['ga']['rsstrack_utm_campaign'] = array(
    '#type' => 'textfield',
    '#title' => t('Campaign Name'),
    '#description' => t('<b>Required for GA</b>. Product, promo code, or slogan. Used for keyword analysis. Use <b>utm_campaign</b> to identify a specific product promotion or strategic campaign. Example: <em>spring_sale</em>'),
    '#default_value' => variable_get('rsstrack_utm_campaign', ''),
  );

  $form['custom'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom tracking code'),
    '#description' => t('You can use this to provide your custom tracking parameters to GA or to support other tracking tools.'),
  );
  $form['custom']['rsstrack_custom'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom query parameters'),
    '#description' => t('Write <b>not urlencoded</b> keys and values, one per line, without ampersands (&amp;). Values will be urlencoded and ampersands will be added automatically.<br>Example:<pre>my_value1=1<br>my_value2=my value<br>my_value3=[token]</pre>'),
    '#default_value' => variable_get('rsstrack_custom', ''),
  );

  $form['tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tokens'),
  );
  $form['tokens']['rsstrack_use_tokens'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable token processing'),
    '#description' => t('Check if you are using Drupal tokens in your RSS Track query strings. Left disabled for better perfomance.'),
    '#default_value' => variable_get('rsstrack_use_tokens', 0),
  );
  $form['tokens']['token_tree'] = array(
    '#type' => 'item',
    '#title' => t('Available tokens'),
    '#markup' => module_exists('token') ?
      theme('token_tree', array('token_types' => array('site', 'node'))) :
      t('Please install <a href="http://drupal.org/project/token">Token module</a> to get clickable list of available tokens.'),
  );

  return system_settings_form($form);
}

/**
 * A rsstrack_system_feeds_settings form validation handler
 *
 * @see rsstrack_system_feeds_settings()
 */
function rsstrack_system_feeds_settings_validate($form, &$form_state) {
  // process GA
  $required_ga = FALSE;
  foreach (array('source', 'medium', 'term', 'content', 'campaign') as $param) {
    if (!empty($form_state['values']["rsstrack_utm_$param"])) {
      $required_ga = TRUE;
      break;
    }
  }
  if ($required_ga) {
    foreach (array('source', 'medium', 'campaign') as $param) {
      if (empty($form_state['values']["rsstrack_utm_$param"])) {
        form_set_error("rsstrack_utm_$param", t('When some GA RSS tracking field is non-empty you need to fill all required GA fields (Campaign Source, Campaign Medium, Campaign Campaign).'));
      }
    }
  }
}


/**
 * Menu callback; generates and prints an RSS feed.
 *
 * Overrides node_feed().
 *
 * Generates an RSS feed from an array of node IDs, and prints it with an HTTP
 * header, with Content Type set to RSS/XML.
 *
 * @param $nids
 *   An array of node IDs (nid). Defaults to FALSE so empty feeds can be
 *   generated with passing an empty array, if no items are to be added
 *   to the feed.
 * @param $channel
 *   An associative array containing title, link, description and other keys,
 *   to be parsed by format_rss_channel() and format_xml_elements().
 *   A list of channel elements can be found at the @link http://cyber.law.harvard.edu/rss/rss.html RSS 2.0 Specification. @endlink
 *   The link should be an absolute URL.
 *
 * @see node_feed()
 */
function rsstrack_node_feed($nids = FALSE, $channel = array()) {
  global $base_url, $language_content;

  if ($nids === FALSE) {
    $nids = db_select('node', 'n')
      ->fields('n', array('nid', 'created'))
      ->condition('n.promote', 1)
      ->condition('n.status', 1)
      ->orderBy('n.created', 'DESC')
      ->range(0, variable_get('feed_default_items', 10))
      ->addTag('node_access')
      ->execute()
      ->fetchCol();
  }

  $item_length = variable_get('feed_item_length', 'fulltext');
  $namespaces = array('xmlns:dc' => 'http://purl.org/dc/elements/1.1/');
  $teaser = ($item_length == 'teaser');

  // Load all nodes to be rendered.
  $nodes = node_load_multiple($nids);
  $items = '';
  foreach ($nodes as $node) {
    $item_text = '';

    $node->link = url("node/$node->nid", array('absolute' => TRUE, 'query' => rsstrack_get_node_query_params($node)));
    $node->rss_namespaces = array();
    $node->rss_elements = array(
      array('key' => 'pubDate', 'value' => gmdate('r', $node->created)),
      array('key' => 'dc:creator', 'value' => $node->name),
      array('key' => 'guid', 'value' => $node->nid . ' at ' . $base_url, 'attributes' => array('isPermaLink' => 'false'))
    );

    // The node gets built and modules add to or modify $node->rss_elements
    // and $node->rss_namespaces.
    $build = node_view($node, 'rss');
    unset($build['#theme']);

    if (!empty($node->rss_namespaces)) {
      $namespaces = array_merge($namespaces, $node->rss_namespaces);
    }

    if ($item_length != 'title') {
      // We render node contents and force links to be last.
      $build['links']['#weight'] = 1000;
      $item_text .= drupal_render($build);
    }

    $items .= format_rss_item($node->title, $node->link, $item_text, $node->rss_elements);
  }

  $channel_defaults = array(
    'version'     => '2.0',
    'title'       => variable_get('site_name', 'Drupal'),
    'link'        => $base_url,
    'description' => variable_get('feed_description', ''),
    'language'    => $language_content->language
  );
  $channel_extras = array_diff_key($channel, $channel_defaults);
  $channel = array_merge($channel_defaults, $channel);

  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<rss version=\"" . $channel["version"] . "\" xml:base=\"" . $base_url . "\" " . drupal_attributes($namespaces) . ">\n";
  $output .= format_rss_channel($channel['title'], $channel['link'], $channel['description'], $items, $channel['language'], $channel_extras);
  $output .= "</rss>\n";

  drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
  print $output;
}

/**
 * Generate the content feed for a taxonomy term.
 *
 * Overrides taxonomy_term_feed().
 *
 * @param $term
 *   The taxonomy term.
 *
 * @see taxonomy_term_feed()
 */
function rsstrack_taxonomy_term_feed($term) {
  $channel['link'] = url('taxonomy/term/' . $term->tid, array('absolute' => TRUE));
  $channel['title'] = variable_get('site_name', 'Drupal') . ' - ' . $term->name;
  // Only display the description if we have a single term, to avoid clutter and confusion.
  // HTML will be removed from feed description.
  $channel['description'] = check_markup($term->description, $term->format, '', TRUE);
  $nids = taxonomy_select_nodes($term->tid, FALSE, variable_get('feed_default_items', 10));

  rsstrack_node_feed($nids, $channel);
}

/**
 * Menu callback; displays an RSS feed containing recent blog entries of a given user.
 *
 * Overrides blog_feed_user()
 *
 * @see blog_feed_user()
 */
function rsstrack_blog_feed_user($account) {

  $nids = db_select('node', 'n')
    ->fields('n', array('nid', 'created'))
    ->condition('type', 'blog')
    ->condition('uid', $account->uid)
    ->condition('status', 1)
    ->orderBy('created', 'DESC')
    ->range(0, variable_get('feed_default_items', 10))
    ->addTag('node_access')
    ->execute()
    ->fetchCol();

  $channel['title'] = t("!name's blog", array('!name' => format_username($account)));
  $channel['link'] = url('blog/' . $account->uid, array('absolute' => TRUE));

  rsstrack_node_feed($nids, $channel);
}

/**
 * Menu callback; displays an RSS feed containing recent blog entries of all users.
 *
 * Overrides blog_feed_last().
 *
 * @see blog_feed_last()
 */
function rsstrack_blog_feed_last() {
  $nids = db_select('node', 'n')
    ->fields('n', array('nid', 'created'))
    ->condition('type', 'blog')
    ->condition('status', 1)
    ->orderBy('created', 'DESC')
    ->range(0, variable_get('feed_default_items', 10))
    ->addTag('node_access')
    ->execute()
    ->fetchCol();

  $channel['title'] = t('!site_name blogs', array('!site_name' => variable_get('site_name', 'Drupal')));
  $channel['link'] = url('blog', array('absolute' => TRUE));

  rsstrack_node_feed($nids, $channel);
}
