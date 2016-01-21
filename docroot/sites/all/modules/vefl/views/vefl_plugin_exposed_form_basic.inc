<?php
/**
 * @file
 * Provides plugin to output Views exposed filters in layout for basic.
 */

/**
 * Exposed form plugin that provides a basic exposed form with layout options.
 *
 * @ingroup views_exposed_form_plugins
 */
class vefl_plugin_exposed_form_basic extends views_plugin_exposed_form_basic {
  function option_definition() {
    $options = parent::option_definition();

    $options['layout'] = array(
      'contains' => array(
        'layout_id' => array('default' => 'vefl_onecol'),
        'regions' => array('default' => array()),
      ),
    );
    return $options;
  }

  /**
   * Provide a form for setting options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $layouts = vefl_get_layouts();
    $layout_id = $this->options['layout']['layout_id'];

    // Prepare layout select options.
    $options = array();
    foreach ($layouts as $name => $layout) {
      $module = !empty($layout['module']) ? $layout['module'] : 'VEFL';
      $options[$module][$name] = $layout['title'];
    }

    // Remove module categories if only one category.
    if (count($options) < 2) {
      $options = reset($options);
    }

    // Outputs layout selectbox.
    $form['layout'] = array(
      '#type' => 'fieldset',
      '#title' => t('Layout settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['layout']['layout_id'] = array(
      '#prefix' => '<div class="container-inline">',
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('Layout'),
      '#default_value' => $layout_id,
    );
    $form['layout']['change'] = array(
      '#type' => 'submit',
      '#value' => t('Change'),
      '#submit' => array('_vefl_change_layout_button'),
      '#suffix' => '</div>',
    );

    // Outputs regions selectbox for each filter.
    $types = array(
      'filters' => $this->display->handler->get_handlers('filter'),
      'actions' => vefl_form_actions(),
    );
    $regions = $layouts[$layout_id]['regions'];
    foreach ($types as $type => $fields) {
      foreach ($fields as $id => $filter) {
        if ($type == 'filters') {
          if (!$filter->options['exposed']) {
            continue;
          }
          $filter = $filter->definition['title'];
        }

        $form['layout']['widget_region'][$id] = array(
          '#type' => 'select',
          '#title' => $filter,
          '#options' => $regions,
        );

        // Set default region for chosen layout.
        if (!empty($this->options['layout']['widget_region'][$id]) && !empty($regions[$this->options['layout']['widget_region'][$id]])) {
          $form['layout']['widget_region'][$id]['#default_value'] = $this->options['layout']['widget_region'][$id];
        }
      }
    }
    // Store regions in form_state to have it in options array.
    $form_state['layout_regions'] = $regions;
  }

  /**
   * Submit handler for the options to rebuild theme registry.
   */
  function options_submit(&$form, &$form_state) {
    parent::options_submit($form, $form_state);
    $form_state['values']['exposed_form_options']['layout']['regions'] = $form_state['layout_regions'];
    drupal_theme_rebuild();
  }

  /**
   * Views exposed form alter function.
   *
   * Changes form's theme function if needed and
   * adds '#layout' attribute with layout settings.
   */
  function exposed_form_alter(&$form, &$form_state) {
    parent::exposed_form_alter($form, $form_state);
    $view = &$form_state['view'];
    $display = &$form_state['display'];
    $layout_id = $this->options['layout']['layout_id'];

    // Fill regions.
    $region_widgets = array();
    $regions = array_keys(array_filter($this->options['layout']['regions']));
    foreach ($regions as $region) {
      $region_widgets[$region] = array();
    }

    // Shorthand for all filters in this view.
    $filters = $form_state['view']->display_handler->handlers['filter'];
    foreach ($this->display->handler->get_handlers('filter') as $label => $filter) {
      if (!$filter->options['exposed']) {
        continue;
      }
      $filter_region = !empty($this->options['layout']['widget_region'][$label]) ? $this->options['layout']['widget_region'][$label] : reset($regions);
      $filter_key = 'filter-' . ($filters[$label]->options['is_grouped'] ? $filters[$label]->options['group_info']['identifier'] : $label);

      $region_widgets[$filter_region][] = $filter_key;
    }

    // Fill action fields.
    $actions = array_keys(vefl_form_actions());
    foreach ($actions as $action_id) {
      $filter_region = $this->options['layout']['widget_region'][$action_id];
      $region_widgets[$filter_region][] = $action_id;
    }
    $form['#theme'] = _vefl_form_theme_functions($layout_id, $view, $display);
    $form['#layout'] = $this->options['layout'];
    $form['#layout']['region_widgets'] = $region_widgets;
  }

}
