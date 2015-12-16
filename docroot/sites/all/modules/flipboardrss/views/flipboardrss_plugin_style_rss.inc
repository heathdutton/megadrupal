<?php

/**
 * @file
 * Contains the RSS style plugin.
 */

/**
 * Flipboard style plugin to render an RSS feed.
 *
 * @ingroup views_style_plugins
 */
class flipboardrss_plugin_style_rss extends views_plugin_style {

  function attach_to($display_id, $path, $title) {
    $display = $this->view->display[$display_id]->handler;
    $url_options = array();
    $input = $this->view->get_exposed_input();
    if ($input) {
      $url_options['query'] = $input;
    }
    $url_options['absolute'] = TRUE;

    $url = url($this->view->get_url(NULL, $path), $url_options);
    if ($display->has_path()) {
      if (empty($this->preview)) {
        drupal_add_feed($url, $title);
      }
    }
    else {
      if (empty($this->view->feed_icon)) {
        $this->view->feed_icon = '';
      }

      $this->view->feed_icon .= theme('feed_icon', array('url' => $url, 'title' => $title));
      drupal_add_html_head_link(array(
        'rel' => 'alternate',
        'type' => 'application/rss+xml',
        'title' => $title,
        'href' => $url
      ));
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['description'] = array('default' => '', 'translatable' => TRUE);
    $options['content_encoded'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['description'] = array(
      '#type' => 'textfield',
      '#title' => t('Channel description for your RSS feed.'),
      '#default_value' => $this->options['description'],
      '#description' => t('This will appear in the RSS feed itself.'),
      '#type' => 'textarea',
      '#rows' => 3,
      '#maxlength' => 600,
    );
  }

  /**
   * Get RSS feed description.
   *
   * @return string
   *   The string containing the description with the tokens replaced.
   */
  function get_description() {
    $description = $this->options['description'];

    // Allow substitutions from the first row.
    $description = $this->tokenize_value($description, 0);

    return $description;
  }

  function render() {
    if (empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }
    $rows = '';

    // Populate channel name spaces to allow for various syntax.
    $this->namespaces = array(
      'xmlns:dc' => 'http://purl.org/dc/elements/1.1/',
      'xmlns:content' => 'http://purl.org/rss/1.0/modules/content/',
      'xmlns:atom' => 'http://www.w3.org/2005/Atom',
      // TODO: Add location GeoRSS
      //'xmlns:georss' => 'http://www.georss.org/georss'
    );

    // Populate rows.
    foreach ($this->view->result as $row_index => $row) {
      $this->view->row_index = $row_index;
      $rows .= $this->row_plugin->render($row);
    }

    // Render rows into output.
    $output = theme($this->theme_functions(),
      array(
        'view' => $this->view,
        'options' => $this->options,
        'rows' => $rows
      ));
    unset($this->view->row_index);
    return $output;
  }
}
