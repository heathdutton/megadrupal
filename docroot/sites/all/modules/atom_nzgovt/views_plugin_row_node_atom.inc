<?php

/**
 * @file
 * Contains the node ATOM row style plugin.
 */

/**
 * Plugin which performs a node_view on the resulting object
 * and formats it as an ATOM item.
 */
class views_plugin_row_node_atom extends views_plugin_row {
  // Basic properties that let the row style follow relationships.
  var $base_table = 'node';
  var $base_field = 'nid';

  // Stores the nodes loaded with pre_render.
  var $nodes = array();

  function option_definition() {
    $options = parent::option_definition();

    $options['item_length'] = array('default' => 'default');
    $options['links'] = array('default' => 0);
    $options['service_links'] = array('default' => 0);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['item_length'] = array(
      '#type' => 'select',
      '#title' => t('Display type'),
      '#options' => $this->options_form_summary_options(),
      '#default_value' => $this->options['item_length'],
    );
    $form['links'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display links'),
      '#default_value' => $this->options['links'],
    );
    if (module_exists('service_links')) {
      $form['service_links'] = array(
        '#type' => 'checkbox',
        '#title' => t('Display service links'),
        '#default_value' => $this->options['service_links'],
      );
    }
  }

  /**
   * Return the main options, which are shown in the summary title.
   */
  function options_form_summary_options() {
    return array(
      'default' => t('Full text'),
      'title' => t('Title only'),
      'teaser' => t('Title plus teaser'),
    );
  }

  function summary_title() {
    $options = $this->options_form_summary_options();
    return check_plain($options[$this->options['item_length']]);
  }

  function pre_render($values) {
    $nids = array();
    foreach ($values as $row) {
      $nids[] = $row->{$this->field_alias};
    }
    if (!empty($nids)) {
      $this->nodes = node_load_multiple($nids);
    }
  }

  function render($row) {
    // For the most part, this code is taken from node_feed() in node.module
    global $base_url;
    global $language;

    $nid = $row->{$this->field_alias};
    if (!is_numeric($nid)) {
      return;
    }

    // Load the specified node:
    $node = $this->nodes[$nid];
    if (empty($node)) {
      return;
    }

    $node->rss_namespaces = array(
      'xmlns:dc' => 'http://purl.org/dc/elements/1.1/',
      'xml:lang' => $language->language,
    );
    $node->rss_elements = array(
      array('key' => 'published', 'value' => date('c', $node->created)),
      array('key' => 'updated', 'value' => date('c', $node->changed)),
      array('key' => 'dc:creator', 'value' => $node->name),
      array('key' => 'id', 'value' => atom_nzgovt_generate_node_tag_uri($node)),
    );

    // The node gets built and modules add to or modify $node->rss_elements
    // and $node->rss_namespaces.
    node_build_content($node, 'rss');

    $this->view->style_plugin->namespaces = array_merge($this->view->style_plugin->namespaces, $node->rss_namespaces);

    // work out what the content should be based on the current row option
    $item_text = '';
    $item_length = $this->options['item_length'];
    if (!empty($node->content)) {
      hide($node->content['links']);
      switch ($item_length) {
        case 'default' :
          // all the content
          $item_text .= drupal_render($node->content);
          break;
        case 'teaser' :
          // just the teaser
          $item_text .= $node->body['und'][0]['safe_summary'];
          break;
        case 'title' :
        default :
          // blank
          break;
      }
      // add stuff to the end of the content
      if (!empty($this->options['links'])) {
        show($node->content['links']);
        $item_text .= drupal_render($node->content['links']);
      }
      if (!empty($this->options['service_links'])) {
        $item_text .= $node->service_links_rendered;
      }
    }

    $item = new stdClass;
    $item->content = $item_text;
    $item->title = $node->title;
    $item->link = url("node/$node->nid", array('absolute' => TRUE));
    $item->elements = $node->rss_elements;
    // comments is not valid for ATOM, only RSS, so remove it
    foreach ($item->elements as $index => $element) {
      if ($element['key'] == 'comments') {
        unset($item->elements[$index]);
      }
    }
    $item->nid = $node->nid;
    if (isset($node->readmore)) {
      $item->readmore = $node->readmore;
    }

    // check for the possibility that this is a content type that can use the NZ
    // Govt category extensions
    $item->category = array();
    foreach(atom_nzgovt_nz_govt_information_type_map() as $info_type) {
      if (preg_match('/^' . $node->type . '/i', $info_type)) {
        $item->category[] = $info_type;
      }
    }

    // add in the taxonomy terms
    $item->terms = array();
    $result = field_view_field('node', $node, 'field_tags', array('default'));
    if (!empty($result) && isset($result['#object'])) {
      $terms = $result['#object']->field_tags['und'];
      if (!empty($terms)) {
        foreach($terms as $term) {
          $item->terms[] = $term['taxonomy_term']->name;
        }
      }
    }

    // add in the attachments
    $item->attachments = array();
    if (!empty($node->field_file_attachments)) {
      foreach ($node->field_file_attachments['und'] as $index => $attachment) {
        $item->attachments[] = $attachment;
      }
    }

    return theme($this->theme_functions(),
      array(
        'view' => $this->view,
        'options' => $this->options,
        'row' => $item
      ));
  }
}
