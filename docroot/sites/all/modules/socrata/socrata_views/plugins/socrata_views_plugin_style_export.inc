<?php
/**
 * @file
 * Plugin include file for export style plugin.
 */

/**
 * Generalized style plugin for export plugins.
 *
 * @ingroup views_style_plugins
 */
class socrata_views_plugin_style_export extends views_plugin_style {

  /**
   * Set options fields and default values.
   *
   * @return
   * An array of options information.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['attach_text'] = array(
      'default' => $this->definition['export feed text'],
      'translatable' => TRUE,
    );
    /*$options['filename'] = array(
      'default' => $this->definition['export feed file'],
      'translatable' => FALSE,
    );*/
    return $options;
  }

  /**
   * Options form mini callback.
   *
   * @param $form
   * Form array to add additional fields to.
   * @param $form_state
   * State of the form.
   * @return
   * None.
   */
  function options_form(&$form, &$form_state) {
    $form['attach_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Attach text'),
      '#default_value' => $this->options['attach_text'],
      '#description' => t('This text is used in building the feed link. By default it is the "alt" text for the feed image.'),
    );
    /*$form['filename'] = array(
      '#type' => 'textfield',
      '#title' => t('Filename'),
      '#default_value' => $this->options['filename'],
      '#description' => t('The filename that will be suggested to the browser for downloading purposes. You may include replacement patterns from the list below.'),
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array(
        'edit-style-options-provide-file' => array(TRUE),
      ),
    );

    // General token replacement.
    $output = t('<p>The following substitution patterns are available for this display. Use the pattern shown on the left to display the value indicated on the right.</p>');
    $items = array(
      '%view == ' . t('View name'),
      '%display == ' . t('Display name'),
    );

    $output .= theme('item_list', array('items' => $items));

    // Get a list of the available arguments for token replacement.
    $options = array();

    $count = 0; // This lets us prepare the key as we want it printed.
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
      $options[t('Arguments')]['%' . ++$count . '-title'] = t('@argument title', array('@argument' => $handler->ui_name()));
      $options[t('Arguments')]['%' . $count . '-value'] = t('@argument value', array('@argument' => $handler->ui_name()));
    }

    // Append the list with exposed filters stuff.
    $options[t('Exposed filters')]['%exposed'] = t('effective exposed filters, like <em>filter1_foo-filter2_bar</em>');

    // ...and datestamp.
    $time = REQUEST_TIME;
    $parts = array(
      'full' => 'Y-m-d\TH-i-s',
      'yy' => 'y',
      'yyyy' => 'Y',
      'mm' => 'm',
      'mmm' => 'M',
      'dd' => 'd',
      'ddd' => 'D',
      'hh' => 'H',
      'ii' => 'i',
      'ss' => 's',
    );
    foreach ($parts as $part => $format) {
      $options[t('Timestamp')]['%timestamp-' . $part] = format_date($time, 'custom', $format);
    }

    // We have some options, so make a list.
    if (!empty($options)) {
      foreach (array_keys($options) as $type) {
        if (!empty($options[$type])) {
          $items = array();
          foreach ($options[$type] as $key => $value) {
            $items[] = $key . ' == ' . $value;
          }
          $output .= theme('item_list', array('items' => $items, 'title' => $type));
        }
      }
    }
    $form['help'] = array(
      '#type' => 'fieldset',
      '#title' => t('Replacement patterns'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#value' => $output,
      '#dependency' => array(
        'edit-style-options-provide-file' => array(1),
      ),
    );*/
  }

  /**
   * Attach this view to another display as a feed.
   *
   * Provide basic functionality for all export style views like attaching a
   * feed image link.
   */
  function attach_to($display_id, $path, $title) {
    if ($this->display->handler->access()) {
      /*$type = $this->definition['export feed type'];
      $theme_pattern = array(
        'socrata_views_feed_icon__' . $this->view->name . '__' . $display_id . '__' . $type,
        'socrata_views_feed_icon__' . $this->view->name . '__' . $display_id,
        'socrata_views_feed_icon__' . $this->view->name . '__' . $type,
        'socrata_views_feed_icon__' . $display_id . '__' . $type,
        'socrata_views_feed_icon__' . $display_id,
        'socrata_views_feed_icon__' . $type,
        'socrata_views_feed_icon',
      );
      $query = $this->view->get_exposed_input();
      // Stash the display id we're coming form in the url so we can hijack it later.
      // if ($this->options['parent_sort']) {
        $query['attach'] = $display_id;
      // }
      if (!isset($this->view->feed_icon)) {
        $this->view->feed_icon = '';
      }
      $this->view->feed_icon .= theme($theme_pattern, array(
          'image_path' => $this->definition['export feed icon'],
          'url' => $this->view->get_url(NULL, $path),
          'query' => $query,
          'text' => $this->options['attach_text'],
        )
      );*/

      // Temporary link until I figure out theme call above
      $download_format = $this->definition['export feed type'];
      $socrata_select_query = $this->view->build_info['query'];
      $url = $socrata_select_query->getDownloadUrl($download_format);
      $this->view->feed_icon = l(
        $this->options['attach_text'],
        $url
      );
    }
  }

  /**
   * Generate the filename for the export.
   */
  /*function generate_filename() {
    $view = $this->view;
    $filename = '';

    if (isset($this->options['filename']) && !empty($this->options['provide_file'])) {
      // General tokens.
      $tokens = array(
        '%view' => check_plain($view->name),
        '%display' => check_plain($view->current_display),
      );
      // Argument tokens.
      $count = 0;
      foreach ($view->display_handler->get_handlers('argument') as $arg => $handler) {
        $token = '%' . ++$count;
        $tokens[$token . '-title'] = $handler->get_title();
        $tokens[$token . '-value'] = isset($view->args[$count - 1]) ? check_plain($view->args[$count - 1]) : '';
      }

      // Effective exposed filters token.
      $exposed = array();
      foreach ($view->display_handler->get_handlers('filter') as $arg => $handler) {
        if (!$handler->options['exposed']) {
          continue;
        }
        if (!empty($view->exposed_input[$handler->options['expose']['identifier']])) {
          $identifier = $handler->options['expose']['identifier'];
          $option = $view->exposed_input[$identifier];
          // The option may be a string or an array, depending on whether the
          // widget is a text box/area or a select box.
          if (is_array($option)) {
            $option = implode('--', $option);
          }
          $exposed[] = check_plain($identifier) . '_' . check_plain($option);
        }
      }
      if (!empty($exposed)) {
        $tokens['%exposed'] = implode('-', $exposed);
      }
      else {
        $tokens['%exposed'] = 'default' ;
      }

      // Timestamp token.
      $time = REQUEST_TIME;
      $parts = array(
        'full' => 'Y-m-d\TH-i-s',
        'yy' => 'y',
        'yyyy' => 'Y',
        'mm' => 'm',
        'mmm' => 'M',
        'dd' => 'd',
        'ddd' => 'D',
        'hh' => 'H',
        'ii' => 'i',
        'ss' => 's',
      );
      foreach ($parts as $part => $format) {
        $tokens['%timestamp-' . $part] = format_date($time, 'custom', $format);
      }

      $filename = strtr($this->options['filename'], $tokens);
    }

    return $filename;
  }*/
}
