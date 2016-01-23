<?php

/**
 * @file
 * Contains library_attach_plugin_display_extender_libraries.
 */

class library_attach_plugin_display_extender_libraries extends views_plugin_display_extender {

  function options_definition_alter(&$options) {
    $options['libraries'] = array('default' => array());
  }

  function options_summary(&$categories, &$options) {
    $libraries = $this->display->get_option('libraries');
    $options['libraries'] = array(
      'category' => 'other',
      'title' => t('Libraries'),
      'value' => !empty($libraries) ? $this->getLibrarySummary($libraries) : t('None'),
    );
  }

  /**
   * Given an array of libraries, convert them into simple 'summary' text.
   */
  protected function getLibrarySummary(array $libraries) {
    $library_names = array();
    foreach ($libraries as $key) {
      list($module, $name) = explode(':', $key, 2);
      if ($library = drupal_get_library($module, $name)) {
        $library_names[] = check_plain($library['title']);
      }
    }
    natsort($library_names);
    return implode(', ', $library_names);
  }

  function options_form(&$form, &$form_state) {
    if ($form_state['section'] == 'libraries') {
      $form['#title'] .= t('The libraries for this display');
      $libraries = $this->display->get_option('libraries');
      $form['libraries'] = array(
        '#type' => 'select',
        '#title' => t('Libraries to add to this display'),
        '#options' => library_attach_get_library_options(),
        '#multiple' => TRUE,
        '#default_value' => $libraries,
        '#size' => max(10, min(count($libraries), 10)),
      );
      if (module_exists('chosen')) {
        $form['libraries']['#prefix'] = '<div style="min-height: 300px;">';
        $form['libraries']['#suffix'] = '</div>';
        $form['libraries']['#attributes']['data-placeholder'] = t('Select a library');
        $form['libraries']['#attributes']['class'][] = 'chosen-widget';
        $form['libraries']['#attributes']['style'] = 'width: 100%';
      }
    }
  }

  function options_submit(&$form, &$form_state) {
    if ($form_state['section'] == 'libraries') {
      // Select boxes filter out empty values, so don't need to array_filter().
      $libraries = array_keys($form_state['values']['libraries']);
      $this->display->set_option('libraries', $libraries);
    }
  }
}
