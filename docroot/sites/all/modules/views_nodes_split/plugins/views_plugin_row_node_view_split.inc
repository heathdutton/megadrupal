<?php

/**
 * @file
 *   Contains the node row plugin.
 */

/**
 * Plugin which performs a node_view on the resulting object.
 *
 * The difference between this plugin and the core node row plugin is that it
 * can display the first nodes with a different view module.
 */
class views_plugin_row_node_view_split extends views_plugin_row_node_view {
  
  function option_definition() {
    $options = parent::option_definition();
    
    // Adds some new options.
    $options['first_settings']['view_mode'] = array('default' => 'teaser');
    $options['first_settings']['links'] = array('default' => TRUE);
    $options['first_settings']['comments'] = array('default' => FALSE);
    $options['first_settings']['first_x_value'] = array('default' => 0);
    return $options;
  }
  
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    
    $options = $this->options_form_summary_options();
    $form['first_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Split settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['first_settings']['first_x_value'] = array(
      '#type' => 'textfield',
      '#title' => t('Split limit'),
      '#description' => t('The results that are situated before or at this limit will have a different view mode.'),
      '#default_value' => $this->options['first_settings']['first_x_value'],
    );
    $form['first_settings']['view_mode'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('View mode'),
      '#default_value' => $this->options['first_settings']['view_mode'],
     );
    $form['first_settings']['links'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display links'),
      '#default_value' => $this->options['first_settings']['links'],
    );
    $form['first_settings']['comments'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display comments'),
      '#default_value' => $this->options['first_settings']['comments'],
    );
  }
  
  function summary_title() {
    $options = $this->options_form_summary_options();
    return check_plain($options[$this->options['view_mode']] . ', ' . $options[$this->options['first_settings']['view_mode']]);
  }
  
  function render($row) {
    // Some static variables that help us to identify at which result index
    // are we in the current result set.
    static $keys, $current_page, $items_per_page;
    // If the static variables are not yet set, time to set them now.
    if (!isset($keys)) {
      $keys = array_keys($this->nodes);
    }
    if (!isset($current_page)) {
      $current_page = $this->view->get_current_page();
    }
    if (!isset($items_per_page)) {
      $items_per_page = $this->view->get_items_per_page();
    }
    $node = $this->nodes[$row->{$this->field_alias}];
    $node->view = $this->view;
    // The position of the node depends on the page we currently are and
    // on the offset inside the current page.
    $absolute_position = $current_page*$items_per_page + array_search($node->nid, $keys) + 1;
    // Because we can have two view modes settings, we will store in the
    // the $options array the correct set of options for this row (links and
    // comments).
    $options = $this->options;
    // And now we check which view mode we should display.
    if ($absolute_position <= $this->options['first_settings']['first_x_value']) {
      // If we display the other view mode, also update the display settings.
      $options = $this->options['first_settings'];
      $build = node_view($node, $this->options['first_settings']['view_mode']);
    } else {
      $build = node_view($node, $this->options['view_mode']);
    }
    // Check if we have to unset the links or set/unset the comments.
    // Prevent the comment form from showing up if this is not a page display.
    if ($options['view_mode'] == 'full' && !$this->view->display_handler->has_path()) {
      $node->comment = FALSE;
    }
    if (!$options['links']) {
      unset($build['links']);
    }
    if (!empty($options['comments']) && user_access('access comments') && $node->comment) {
      $build['comments'] = comment_node_page_additions($node);
    }

    return drupal_render($build);
  }
}