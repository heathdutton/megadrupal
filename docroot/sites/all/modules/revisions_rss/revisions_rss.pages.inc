<?php

/**
 * @file
 * Generates the actual RSS page view.
 */

/**
 * Format and print a revision feed.
 * 
 * @param object $revision_node The node revisions are being pulled for. 
 * @see node_feed
 */
function _revisions_rss_format_feed($revision_node) {
  if(!revisions_rss_check_type($revision_node)) {
    drupal_not_found();
    return;
  }

  global $base_url, $language;  

  $channel = array(
    'version'     => '2.0',
    'title'       => t('@site - Updates for "@title"', array(
                       '@site' => variable_get('site_name', 'Drupal'),
                       '@title' => $revision_node->title)),
    'link'        => url("node/{$revision_node->nid}", array('absolute' => TRUE)),
    'description' => t('Updates for "@title"', array('@title' => $revision_node->title)),
    'language'    => $language->language,
  ); 
  $namespaces = array('xmlns:dc' => 'http://purl.org/dc/elements/1.1/'); 
  $revision_nodes = _revisions_rss_get_revisions($revision_node->nid);

  $output  = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<rss version=\"". $channel["version"] ."\" xml:base=\"". $base_url ."\" ". drupal_attributes($namespaces) .">\n";
  $output .= format_rss_channel($channel['title'], $channel['link'], $channel['description'], $revision_nodes, $channel['language']);
  $output .= "</rss>\n";

  drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
  print $output;
}

/**
 *  Takes a node ID and generates the rss for that nodes revisions.
 *
 * @param int $nid The node ID of the note to generate RSS for.
 * @return The generated RSS.
 */
function _revisions_rss_get_revisions($nid) {  
  global $base_url;

  $rss_items = '';
  $latest_vid;
  
  $query   = db_select('node_revision', 'nr');
  $u_alias = $query->join('users', 'u', 'u.uid = nr.uid');
  $query->fields('nr', array('nid', 'uid', 'title', 'vid', 'log', 'timestamp'))
        ->fields("{$u_alias}", array('name'))
        ->condition('nr.nid', $nid)
        ->addTag('node_access')
        ->orderBy('nr.timestamp', 'DESC')  
        ->addTag('node_access')         
        ->range(0, variable_get('feed_default_items', 10));
        
  $revisions_list = $query->execute();

  foreach($revisions_list as $revision) {
    $description_list = array();
    
    if(empty($latest_vid)) {
      $latest_vid = $revision->vid;
    }          

    $updated = format_date($revision->timestamp, 'large'); 
    
    if(user_access('access user profiles')) {
      $user_link = l($revision->name, "user/{$revision->uid}", array('absolute' => TRUE));
    }
    else {
      $user_link = $revision->name;
    }
    
    if(!empty($revision->log)) {
      $description_list[] = nl2br($revision->log);
    }          
             
    $description_list[] = "Updated $updated, by $user_link";
    _revisions_rss_module_integrations($description_list, $revision, $latest_vid);

    $revision->description = '';
    foreach($description_list as $description) {
      $revision->description .= "<p>$description</p>";
    }

    $extra = array(
      array(
        'key' => 'pubDate', 'value' => gmdate('r', $revision->timestamp)),
      array(
        'key' => 'dc:creator', 'value' => $revision->name),
      array(
        'key' => 'guid', 'value' => "Update {$revision->vid} of $base_url/node/{$revision->nid}",
        'attributes' => array('isPermaLink' => 'false'))
    );
    
    if(user_access('view revisions') && $revision->vid != $latest_vid) {
      $revision_link = url("node/{$revision->nid}/revisions/{$revision->vid}/view", array('absolute' => TRUE));  
    }
    else {
      $revision_link = url("node/{$revision->nid}", array('absolute' => TRUE));
    }
    
    $rss_items .= format_rss_item($revision->title, $revision_link, $revision->description, $extra);
  }
  
  return $rss_items;
}

/**
 * Allows extra features provided by other modules to be added to an RSS item.
 * 
 * @param array $description_list The current array of item descriptions to be appended to.
 * @param object $revision The revision object this information is for.
 * @param int $latest_vid The latest revision ID for this node.
 */
function _revisions_rss_module_integrations(&$description_list, $revision, $latest_vid) {
  if (module_exists('diff') && $revision->vid != $latest_vid && user_access('view revisions')) {
    $description_list[] = l("Diff vs. Newest", "node/{$revision->nid}/revisions/view/{$revision->vid}/$latest_vid");
  }
}
