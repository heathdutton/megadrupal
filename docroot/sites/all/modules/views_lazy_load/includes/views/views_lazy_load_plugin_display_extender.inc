<?php

/**
 * @file
 * A display extender plugin for lazy loading a view.
 */
class views_lazy_load_plugin_display_extender extends views_plugin_display_extender {

  /**
   * {@inheritdoc}
   */
  public function pre_execute() {
    if ($this->isEnabled() && empty($this->view->live_preview)) {
      if (!$this->isExcludedUserAgent()) {
        // Lazy loading requires an AJAX view.
        $this->view->set_use_ajax(TRUE);

        // Marking the view as executed prevents the Search API querying.
        $this->view->executed = TRUE;

        // Add the loading text in the area plugin.
        $this->addLoadingArea();

        // Add our JavaScript to the page with an array of dom_ids that can be
        // used client side to retrieve the view instance.
        $path = drupal_get_path('module', 'views_lazy_load');
        $settings[] = $this->view->dom_id;
        drupal_add_js(array('views_lazy_load' => $settings), 'setting');
        drupal_add_js($path . '/js/views-lazy-load.js', array('scope' => 'footer', 'weight' => 10));
      }
      else {
        // Ensure that AJAX won't be used if a crawler is visiting the view
        $this->view->set_use_ajax(FALSE);
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  function options_definition_alter(&$options) {
    $options['views_lazy_load_enabled'] = array('default' => FALSE);
  }

  /**
   * Check the user agent string to see if it's one of our excluded agents.
   *
   * @return bool
   *   TRUE if the current user agent should be excluded otherwise FALSE.
   */
  function isExcludedUserAgent() {
    $user_agent = $_SERVER['HTTP_USER_AGENT'];
    $excluded_user_agents = views_lazy_load_get_excluded_user_agents();
    return (bool) preg_match("/$excluded_user_agents/i", $user_agent);
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    if ($form_state['section'] == 'views_lazy_load') {
      $form['#title'] .= t('Enable Views Lazy Load');

      $form['views_lazy_load_enabled'] = array(
        '#title' => t('Enabled'),
        '#description' => t('Enabling Views Lazy Load will cause the view to be loaded via AJAX after the initial page load. "Use AJAX" will be enabled for you.'),
        '#type' => 'checkbox',
        '#default_value' => $this->isEnabled(),
      );
    }
  }

  /**
   * {@inheritdoc}
   */
  function options_validate(&$form, &$form_state) {
    if ($form_state['section'] === 'use_ajax') {
      if (empty($form_state['values']['use_ajax']) && $this->isEnabled()) {
        form_set_error('use_ajax', 'You cannot disable AJAX when using Views Lazy Load');
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  function options_submit(&$form, &$form_state) {
    if ($form_state['section'] == 'views_lazy_load') {
      $views_lazy_enabled = $form_state['values']['views_lazy_load_enabled'];
      $this->display->set_option('views_lazy_load_enabled', $views_lazy_enabled);

      // We enable use AJAX as that is required for VLL.
      $this->display->set_option('use_ajax', TRUE);
    }
  }

  /**
   * {@inheritdoc}
   */
  function options_summary(&$categories, &$options) {
    $options['views_lazy_load'] = array(
      'category' => 'other',
      'title' => t('Views Lazy Loading'),
      'value' => $this->isEnabled() ? t('Enabled') : t('Disabled'),
    );
  }

  /**
   * Gets the enabled status of lazy loading.
   *
   * @return bool
   *   TRUE if the lazy load setting is enabled otherwise FALSE.
   */
  protected function isEnabled() {
    // We check this query param which was set client side so we can reuse the
    // views_ajax() function which actually does quite a lot for us.
    return $this->display->get_option('views_lazy_load_enabled') && !isset($_GET['views_lazy_load_disabled']);
  }

  /**
   * Add a loading div to the view while we're loading the results.
   */
  protected function addLoadingArea() {
    // Add a empty area plugin to this view display so it gets initialised.
    $display_id = $this->display->display->id;
    $handler_id = $this->view->add_item($display_id, 'empty', 'views', 'area', array(
      'empty' => TRUE,
      'content' => theme('views_lazy_load_throbber'),
    ));

    // Now get the area plugin info back and initialise an area plugin.
    $empty_info = $this->display->get_option('empty');
    $area = views_get_handler('views', 'area', 'area');
    $area->init($this->view, $empty_info[$handler_id]);

    // We must add the area directly to the display_handler to make sure it gets
    // rendered.
    $this->display->handlers['empty'][$handler_id] = $area;
  }

}
