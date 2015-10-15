<?php

/**
 * @file
 * Contains the vCard display plugin.
 */

/**
 * The plugin that handles one or more vCards.
 */
class views_vcards_plugin_display_vcard extends views_plugin_display_page {
  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options);
  }

  function uses_breadcrumb() {
    return FALSE;
  }

  function get_style_type() {
    return 'feed';
  }

  /**
   * Override theming mechanism.
   *
   * VCards do not go through the normal page theming mechanism. Instead, they
   * go through their own little theme function and then return NULL so that
   * Drupal believes that the page has already rendered itself...which it has.
   */
  function execute() {
    $output = $this->view->render();
    if (empty($output)) {
      return drupal_not_found();
    }
    print $output;
  }

  function preview() {
    if (!empty($this->view->live_preview)) {
      return '<pre>' . check_plain($this->view->render()) . '</pre>';
    }
    return $this->view->render();
  }

  /**
   * Instruct Views theming mechanism.
   *
   * Instead of going through the standard views_view.tpl.php, delegate this
   * to the style handler.
   */
  function render() {
    return $this->view->style_plugin->render($this->view->result);
  }

  function defaultable_sections($section = NULL) {
    if (in_array($section, array(
      'style_options',
      'style_plugin',
      'row_options',
      'row_plugin',
    ))) {
      return FALSE;
    }

    $sections = parent::defaultable_sections($section);
    return $sections;
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['displays'] = array('default' => array());

    // Overrides for standard stuff:
    $options['style_plugin']['default'] = 'views_vcards_style';
    $options['style_options']['default'] = array('description' => '');
    $options['row_plugin']['default'] = 'views_vcards_row';
    $options['defaults']['default']['style_plugin'] = FALSE;
    $options['defaults']['default']['style_options'] = FALSE;
    $options['defaults']['default']['row_plugin'] = FALSE;
    $options['defaults']['default']['row_options'] = FALSE;

    return $options;
  }

  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    // Since we're childing off the 'page' type, we'll still *call* our
    // category 'page' but let's override it so it says feed settings.
    $categories['page'] = array(
      'title' => t('vCard settings'),
      'column' => 'second',
      'build' => array(
        '#weight' => -10,
      ),
    );

    // I don't think we want to give vCards menus directly.
    unset($options['menu']);

    $displays = array_filter($this->get_option('displays'));
    if (count($displays) > 1) {
      $attach_to = t('Multiple displays');
    }
    elseif (count($displays) == 1) {
      $display = array_shift($displays);
      if (!empty($this->view->display[$display])) {
        $attach_to = check_plain($this->view->display[$display]->display_title);
      }
    }


    // Add the attached to option to the category and have it display 'none' or
    // a view name.
    if (!isset($attach_to)) {
      $attach_to = t('None');
    }

    $options['displays'] = array(
      'category' => 'page',
      'title' => t('Attach to'),
      'value' => $attach_to,
    );
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here.
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'displays':
        $form['#title'] .= t('Attach to');
        $displays = array();
        foreach ($this->view->display as $display_id => $display) {
          if (!empty($display->handler) && $display->handler->accept_attachments()) {
            $displays[$display_id] = $display->display_title;
          }
        }
        $form['displays'] = array(
          '#type' => 'checkboxes',
          '#description' => t('The feed icon will be available only to the selected displays.'),
          '#options' => $displays,
          '#default_value' => $this->get_option('displays'),
        );
        break;

      case 'path':
        $form['path']['#description'] = t('This view will be displayed by visiting this path on your site. It is recommended that the path be something like "path/%/%/feed" or "path/%/%/rss.xml", putting one % in the path for each contextual filter you have defined in the view.');
    }
  }

  /**
   * Handle submission of options.
   *
   * Perform any necessary changes to the form values prior to storage. There
   * is no need for this function to actually store the data.
   */
  function options_submit(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'displays':
        $this->set_option($form_state['section'], $form_state['values'][$form_state['section']]);
        break;
    }
  }

  /**
   * Attach to another view.
   */
  function attach_to($display_id) {
    $displays = $this->get_option('displays');
    if (empty($displays[$display_id])) {
      return;
    }

    $plugin = $this->get_plugin();
    if ($plugin) {
      $clone = $this->view->clone_view();
      $clone->set_display($this->display->id);
      $clone->build_title();
      $plugin->attach_to($display_id, $this->get_path(), $clone->get_title());

      // Clean up.
      $clone->destroy();
      unset($clone);
    }
  }

  function uses_link_display() {
    return TRUE;
  }
}
