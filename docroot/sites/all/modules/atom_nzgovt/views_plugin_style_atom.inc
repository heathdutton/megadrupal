<?php
/**
 * @file
 * Contains the ATOM style plugin.
 */

/**
 * Default style plugin to render an ATOM feed.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_atom extends views_plugin_style {
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
        'type' => 'application/atom+xml',
        'title' => $title,
        'href' => $url
      ));
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['subtitle'] = array('default' => '', 'translatable' => TRUE);
    $options['author']['name'] = array('default' => '', 'translatable' => FALSE);
    $options['author']['uri'] = array('default' => '', 'translatable' => FALSE);
    $options['license']['uri'] = array('default' => 'http://creativecommons.org/licenses/by-nc/3.0/nz/', 'translatable' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['subtitle'] = array(
      '#type' => 'textfield',
      '#title' => t('ATOM subtitle'),
      '#default_value' => $this->options['subtitle'],
      '#description' => t('This is metadata for the ATOM feed.'),
    );
    $form['author'] = array(
      '#type' => 'fieldset',
      '#title' => t('ATOM author'),
      '#description' => t('This is metadata for the ATOM feed.'),
    );
    $form['author']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('name'),
      '#default_value' => isset($this->options['author']['name']) ? $this->options['author']['name'] : variable_get('site_name', ''),
    );
    $form['author']['uri'] = array(
      '#type' => 'textfield',
      '#title' => t('uri'),
      '#default_value' => isset($this->options['author']['uri']) ? $this->options['author']['uri'] : url('', array('absolute' => TRUE)),
    );

    $form['license'] = array(
      '#type' => 'fieldset',
      '#title' => t('ATOM license'),
      '#description' => t('This is metadata for the ATOM feed.'),
    );
    $form['license']['uri'] = array(
      '#type' => 'textfield',
      '#title' => t('uri'),
      '#default_value' => isset($this->options['license']['uri']) ? $this->options['license']['uri'] : 'http://creativecommons.org/licenses/by-nc/3.0/nz/',
    );
  }

  /**
   * Return an array of additional XHTML elements to add to the channel.
   *
   * @return
   *   An array that can be passed to format_xml_elements().
   */
  function get_channel_elements() {
    return array();
  }

  function render() {
    if (empty($this->row_plugin)) {
      debug('views_plugin_style_default: Missing row plugin');
      return;
    }
    $rows = '';

    // This will be filled in by the row plugin and is used later on in the
    // theming output.
    $this->namespaces = array(
      'xmlns' => 'http://www.w3.org/2005/Atom',
      'xmlns:nz' => 'http://www.e.govt.nz/standards/nz/2009-03-01',
    );

    // Fetch any additional elements for the channel and merge in their
    // namespaces.
    $this->channel_elements = $this->get_channel_elements();
    foreach ($this->channel_elements as $element) {
      if (isset($element['namespace'])) {
        $this->namespaces = array_merge($this->namespaces, $element['namespace']);
      }
    }

    foreach ($this->view->result as $row_index => $row) {
      $this->view->row_index = $row_index;
      $rows .= $this->row_plugin->render($row);
    }

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
