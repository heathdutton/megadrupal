<?php

/**
 * @file
 * Definition of views_tabs_field_views_handler_field.
 */

/**
 * A handler to provide tabbed output.
 */
class views_tabs_field_views_handler_field extends views_handler_field {

  /**
   * {inheritdoc}
   */
  function query() {
    // No query to run.
  }

  /**
   * {inheritdoc}
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['tabs'] = array('default' => array(), 'bool' => FALSE);
    $options['tabs_type'] = array('default' => 'vertical_tabs', 'bool' => FALSE);
    return $options;
  }

  /**
   * {inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Remove rewrite and link config.
    unset($form['alter']['alter_text']);
    unset($form['alter']['text']);
    unset($form['alter']['make_link']);
    unset($form['alter']['path']);
    unset($form['alter']['absolute']);
    unset($form['alter']['replace_spaces']);
    unset($form['alter']['external']);
    unset($form['alter']['path_case']);
    unset($form['alter']['link_class']);
    unset($form['alter']['alt']);
    unset($form['alter']['rel']);
    unset($form['alter']['prefix']);
    unset($form['alter']['suffix']);
    unset($form['alter']['target']);
    unset($form['alter']['trim']);
    unset($form['alter']['max_length']);
    unset($form['alter']['ellipsis']);
    unset($form['alter']['word_boundary']);
    unset($form['alter']['more_link']);
    unset($form['alter']['more_link_text']);
    unset($form['alter']['more_link_path']);

//    $form['tabs']['help'] = $form['alter']['help'];
//    unset($form['alter']['help']);

    $form['tabs_type'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#options' => array('vertical_tabs' => t('Vertical tabs')),
      '#default_value' => $this->options['tabs_type'],
    );

    if ($info = element_info('horizontal_tabs')) {
      $form['tabs_type']['#options']['horizontal_tabs'] = t('Horizontal tabs');
    }

    $form['tabs'] = array(
      '#type' => 'fieldset',
      '#title' => t('Tabs'),
    );

    // Add form elements for each configured tab.
    foreach ($this->options['tabs'] as $delta => $tab) {
      $tab['delta'] = $delta;
      $form['tabs'][$delta] = $this->add_tab($tab);
    }

    // Add a blank tab if no tabs exist.
    if (empty($this->options['tabs'])) {
      $form['tabs'][] = $this->add_tab();
    }

    $form['tabs']['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add tab'),
      '#op' => 'views_tabs_field_add',
      '#weight' => 9999,
      '#name' => drupal_html_id('views_tabs_field_add'),
    );
  }

  /**
   * {inheritdoc}
   */
  function options_submit(&$form, &$form_state) {
    parent::options_submit($form, $form_state);

    $trigger = $form_state['triggering_element'];

    if (isset($form_state['values']['options']['tabs']['add'])) {
      unset($form_state['values']['options']['tabs']['add']);
    }

    if (isset($trigger['#op'])) {
      switch ($trigger['#op']) {
        case 'views_tabs_field_add':
          $form_state['values']['options']['tabs'][] = array('title' => '', 'content' => '');
          $form_state['rebuild'] = $form_state['rerender'] = TRUE;
          break;
        case 'views_tabs_field_remove':
          $form_state['rebuild'] = $form_state['rerender'] = TRUE;
          unset($form_state['values']['options']['tabs'][$trigger['#delta']]);
          break;
      }
    }

    $form_state['values']['options']['tabs'] = array_values($form_state['values']['options']['tabs']);
  }

  /**
   * Provide form inputs for configuring a tab.
   *
   * @param array $tab
   *   A tab declaration, keys "title" and "content". If empty, a "blank" tab
   *   will be provided.
   *
   * @return array
   *   FAPI form elements for configuring a tab.
   */
  public static function add_tab($tab = NULL) {

    $form_element = array(
      '#type' => 'fieldset',
    );

    $form_element['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
    );

    $form_element['content'] = array(
      '#type' => 'textarea',
      '#title' => t('Content'),
    );

    // If we've got an "existing" tab, populate it.
    if ($tab) {
      $form_element['title']['#default_value'] = $tab['title'];
      $form_element['content']['#default_value'] = $tab['content'];

      // Add button to remove.
      $form_element['remove'] = array(
        '#type' => 'submit',
        '#value' => t('Remove'),
        '#op' => 'views_tabs_field_remove',
        '#delta' => $tab['delta'],
        '#name' => drupal_html_id('views_tabs_field_remove'),
      );
    }

    return $form_element;
  }

  /**
   * {inheritdoc}
   */
  function render($values) {

    switch ($this->options['tabs_type']) {
      case 'horizontal_tabs':
        $element = $this->render_horizontal_tabs();
        break;
      default:
        $element = $this->render_vertical_tabs();
    }

    return drupal_render($element);
  }

  /**
   * Render the field as vertical tabs.
   *
   * @return array
   *   Renderable array representing the tabs.
   */
  protected function render_vertical_tabs() {
    $element = array();

    $element['#type'] = 'vertical_tabs';
    $this->last_render = __FUNCTION__;

    foreach ($this->options['tabs'] as $delta => $config) {
      $tab = array();
      $tab['#type'] = 'fieldset';
      $tab['#title'] = $config['title'];

      // Rewrite.
      $alter['alter_text'] = TRUE;
      $alter['text'] = $config['content'];
      $alter['phase'] = VIEWS_HANDLER_RENDER_TEXT_PHASE_COMPLETELY;
      $tab['content']['#markup'] = $this->render_text($alter);

      $tab['#group'] = 'tabs';
      $element['tab_' . $delta] = $tab;
    }

    $form_state = array();
    drupal_process_form('views_tabs_field', $element, $form_state);
    return $element;
  }

  /**
   * Render the field as horizontal tabs.
   *
   * @return array
   *   Renderable array representing the tabs.
   */
  protected function render_horizontal_tabs() {
    $element = array();

    $element['#type'] = 'horizontal_tabs';
    $element['#attached']['library'] = array(array('field_group', 'horizontal-tabs'));

    $this->last_render = __FUNCTION__;

    foreach ($this->options['tabs'] as $delta => $config) {
      $tab = array();
      $tab['#type'] = 'fieldset';
      $tab['#title'] = $config['title'];

      // Rewrite.
      $alter['alter_text'] = TRUE;
      $alter['text'] = $config['content'];
      $alter['phase'] = VIEWS_HANDLER_RENDER_TEXT_PHASE_COMPLETELY;
      $tab['content']['#markup'] = $this->render_text($alter);

      $tab['#group'] = 'tabs';
      $element['tab_' . $delta] = $tab;
    }

    return $element;
  }

  /**
   * {inheritdoc}
   */
  public function is_value_empty() {
    // Always not empty.
    return FALSE;
  }

}
