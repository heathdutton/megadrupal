<?php
class ffpc_plugin_row_podcast extends views_plugin_row_node_rss {

  function render($row) {
    // For the most part, this code is taken from node_feed() in node.module
    global $base_url;
    $output = '';

    $nid = $row->{$this->field_alias};
    if (!is_numeric($nid)) {
      return;
    }

    $item_length = $this->options['item_length'];
    if ($item_length == 'default') {
      $item_length = variable_get('feed_item_length', 'teaser');
    }

    if (empty($this->view->style_plugin->namespaces)  || !in_array('xmlns:itunes', $this->view->style_plugin->namespaces)) {
      $this->view->style_plugin->namespaces['xmlns:itunes'] = 'http://www.itunes.com/dtds/podcast-1.0.dtd';
    }

    // Load the specified node:
    $node = $this->nodes[$nid];
    if (empty($node)) {
      return;
    }
    
    $item_text = '';

    $uri = entity_uri('node', $node);

    $node->link = url($uri['path'], $uri['options'] + array('absolute' => TRUE));
    $node->rss_namespaces = array();
    $node->rss_elements = array(
      array(
        'key' => 'pubDate',
        'value' => gmdate('r', $node->created),
      ),
      array(
        'key' => 'dc:creator',
        'value' => $node->name,
      ),
    );

    // The node gets built and modules add to or modify $node->rss_elements
    // and $node->rss_namespaces.
    $build = node_view($node, 'rss');
    unset($build['#theme']);

    if (!empty($node->rss_namespaces)) {
      $this->view->style_plugin->namespaces = array_merge($this->view->style_plugin->namespaces, $node->rss_namespaces);
    }

    // Hide the links if desired.
    if (!$this->options['links']) {
      hide($build['links']);
    }

    if ($item_length != 'title') {
      // We render node contents and force links to be last.
      $build['links']['#weight'] = 1000;
      $item_text .= drupal_render($build);
    }

    $stripped_item_text = strip_tags($item_text);

    if (strlen($stripped_item_text) > 255) {
      $item_subtitle = substr($stripped_item_text, 0, 252) .'...';
    }
    else {
      $item_subtitle = $stripped_item_text;
    }

    if (!getid3_load(TRUE)) {
      return;
    }
    $getid3 = new getID3;

    foreach ( $this->view->field as $id => $field ) {          

      // @todo: make this handle more than just file fields. e.g., link fields
      if (isset($field->field_info['type']) && $field->field_info['type'] == 'file') {
        foreach ( field_get_items('node', $node, $field->field) as $file ) {
          $info = $getid3->analyze(drupal_realpath($file['uri']));

          // @todo Shouldn't this be hook based and allow other modules to plugin?
          $file_extra = array();
          $find = array('&amp;', 'nbsp;');
          $replace = array('&', ' ');
          $file_extra[] = array(
            'key' => 'enclosure',
            'attributes'  =>  array(
              'url' => file_create_url($file['uri']),
              'length' => $file['filesize'],
              'type' => $file['filemime'],
            ),
          );
          $file_extra[] = array(
            'key' => 'itunes:duration',
            'value' => $info['playtime_string'],
          );
          $file_extra[] = array(
            'key' => 'itunes:author',
            'value' => $info['tags']['id3v2']['artist'][0],
          );

          $file_extra[] = array(
            'key' => 'itunes:subtitle',
            'value' => str_replace($find, $replace, $item_subtitle),
          );
          $file_extra[] = array(
            'key' => 'itunes:summary',
            'value' => str_replace($find, $replace, $stripped_item_text),
          );
          $file_extra[] = array(
            'key' => 'guid',
            'value' => file_create_url($file['uri']),
            'attributes' => array('isPermaLink' => 'false'),
          );

          /*
           * The following function takes title, link, description and then
           * all additional XML elements.  For the title we'll use the node
           * title.  Link serves no real purpose in a podcast.  Description
           * is overridden by the extra "subtitle" tag but we'll keep it for
           * completeness with RSS and use the node teaser.
          */


          $item = new stdClass();
          $item->description = $item_text;
          $item->title = $node->title;
          $item->link = $node->link;
          $item->elements = array_merge($node->rss_elements, $file_extra);
          $item->nid = $node->nid;

          $output .= theme($this->theme_functions(), array(
            'view' => $this->view,
            'options' => $this->options,
            'row' => $item,
          ));
        }
      }
    }
    
    return $output;
  }
}