<?php

/**
 * Create a dummy class 'better_exposed_filters_exposed_form_plugin'
 * to allow our plugin to work both with and without the better_exposed_filters plugin
 */
if (!class_exists('better_exposed_filters_exposed_form_plugin')) {
  class better_exposed_filters_exposed_form_plugin extends views_plugin_exposed_form_basic { }
}

class views_expost_exposed_form_plugin extends better_exposed_filters_exposed_form_plugin {

  function init(&$view, &$display, $options = array()) {
    parent::init($view, $display, $options);
    $this->view->set_exposed_input($_POST + $_GET);
  }

  function summary_title() {
    return t('Settings');
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['form_method'] = array('default' => 'get');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['form_method'] = array(
      '#type' => 'select',
      '#title' => t('Form Method (Advanced)'),
      '#description' => t('If the URL produced by the exposed filters form might exceed the URL length limit, you can switch to use POST as the method for the form.'),
      '#default_value' => $this->options['form_method'],
      '#options' => array('get' => 'GET ' . t('(Recommended)'), 'post' => 'POST'),
    );
  }

  /*
   * Tweak the exposed filter form to show Better Exposed Filter options.
   */
  function exposed_form_alter(&$form, &$form_state) {
    parent::exposed_form_alter($form, $form_state);

    $form['#attributes']['method'] = $this->options['form_method'];

    // When using POST, we must handle the pager
    // otherwise, it will still attempt to use GET (with standard <a> links).
    // Instead, we will add an extra page number field (hidden)
    // and some JS to trigger the submission of the page
    // whenever a pager link is clicked
    if ($this->options['form_method'] == 'post') {
      // Check if the view itself is being rendered (not just the filters form)
      if (isset($this->view->dom_id)) {
        $data['viewsExPost']['exPostForms'][$form['#id']] = '.view-dom-id-' . $this->view->dom_id;

        $form['#attached']['js'][] = drupal_get_path('module', 'views_expost') . '/views_expost.js';
        $form['#attached']['js'][] = array(
          'data' => $data,
          'type' => 'setting'
        );
      }
    }
  }
}
