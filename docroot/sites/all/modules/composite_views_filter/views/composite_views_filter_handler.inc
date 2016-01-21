<?php

class composite_views_filter_handler extends views_handler_filter {

  /**
   * Override query() to do nothing.
   */
  function query() {}


  /**
   * Options definition.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['composite_views_filter']['contains']['groups'] = array('default' => '');
    $options['composite_views_filter']['contains']['default_group'] = array('default' => '');
    $options['composite_views_filter']['contains']['all_label'] = array('default' => '');
    $options['composite_views_filter']['contains']['classification'] = array('default' => array());

    $options['exposed']['default'] = TRUE;

    return $options;
  }


  /**
   * Define options form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // No multiple!
    if (isset($form['expose']['multiple'])) {
      $form['expose']['multiple']['#access'] = FALSE;
      $form['expose']['multiple']['#default_value'] = 0;
    }

    // @see _composite_views_filter_groups()
    $form['expose']['label']['#required'] = TRUE;

    $form['composite_views_filter'] = array(
      '#type' => 'fieldset',
      '#title' => t('Groups'),
      '#tree' => TRUE,
      '#collapsed' => FALSE,
      '#collapsible' => FALSE,
    );
    $form['composite_views_filter']['groups'] = array(
      '#type' => 'textarea',
      '#title' => t('Groups'),
      '#default_value' => $this->options['composite_views_filter']['groups'],
      '#required' => TRUE,
      '#description' => t('Define groups like you would options in a List Field: <code>key|Readable label</code>. Labels will be translated.'),
    );
    $form['composite_views_filter']['default_group'] = array(
      '#type' => 'textfield',
      '#title' => t('Default selected composite filter'),
      '#default_value' => $this->options['composite_views_filter']['default_group'],
      '#description' => t('Machine name of one of the above groups. Leave this empty to disable all composite filters initially.'),
    );
    $form['composite_views_filter']['all_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Label for &quot;- All -&quot;'),
      '#default_value' => $this->options['composite_views_filter']['all_label'],
      '#description' => t('The dropdown label that represents all filter groups. If empty, &quot;- All -&quot; will be used. This string will be translated.'),
    );
  }

  /**
   * Here we define the custom label for the - All - option.
   */
  function exposed_translate(&$form, $type) {
    parent::exposed_translate($form, $type);

    $label = $this->options['composite_views_filter']['all_label'] ?: '- All -';
    $form['#options']['All'] = t($label);
    $form['#default_value'] = $this->options['composite_views_filter']['default_group'];
  }

  /**
   * Exposed form.
   */
  function value_form(&$form, &$form_state) {
    $form_id = @$form_state['build_info']['form_id'] ?: @$form_state['form_id'] ?: '';
    if ($form_id == 'views_exposed_form') {
      $label = @$this->options['expose']['label'] ?: t('Composite Views Filter Group');
      $form['value'] = array(
        '#type' => 'select',
        '#title' => $label,
        '#options' => $this->get_groups(TRUE),
      );
    }
  }

  /**
   * Admin summary (in View edit overview).
   *
   * This is truly nasty, but I need finer control than Views provides.
   * @see composite_views_filter_views_ui_display_tab_alter()
   */
  function admin_summary() {
    $filter_handlers = _composite_views_filter_view_has_composite_filters($this->view);
    $groups = $this->get_groups();

    return $this->options['expose']['label'] . ' (' . t('@num groups', array('@num' => count($groups)));
  }


  /**
   * Helper to parse and return groups.
   */
  function get_groups($translate = FALSE) {
    $options = list_extract_allowed_values($this->options['composite_views_filter']['groups'], 'list_text', FALSE);
    if ($translate) {
      $options = array_map('t', $options);
    }
    return $options;
  }


  /**
   * Helper to return saved classification.
   */
  function get_classification() {
    return $this->options['composite_views_filter']['classification'];
  }

}
