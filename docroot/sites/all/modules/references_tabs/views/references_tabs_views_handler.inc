<?php

/**
 * @file
 * 
 */

/**
 * Style plugin to render each item as a tab in a tabs group.
 *
 * @ingroup views_style_plugins
 */
class references_tabs_views_handler extends views_plugin_style {

  /**
   * Set default options
   */
  function options(&$options) {
    $options = parent::option_definition();

    $options['references_tabs_type'] = REFERENCES_TABS_TYPE_NONE;
    $options['references_tabs_title'] = REFERENCES_TABS_TITLE_NODETITLE;

    return $options;
  }

  /**
   * Build options form
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    
    $type_options = array();
    $type_options[REFERENCES_TABS_TYPE_HORIZONTAL] = t('Horizontal');
    $type_options[REFERENCES_TABS_TYPE_VERTICAL] = t('Vertical');

    $form['references_tabs_type'] = array(
      '#title'   => t('Tabs'),
      '#type'    => 'select',
      '#options' => $type_options,
      '#default_value' => $this->options['references_tabs_type'],
    );
    
    if ($this->uses_fields()) {
      $title_options = array();
      $title_options[REFERENCES_TABS_TITLE_NODETITLE] = t('Node: title');

      $handlers = $this->display->handler->get_handlers('field');
      foreach ($handlers as $id => $handler) {
        $title_options[$id] = $handler->ui_name();
      }
    
      $form['references_tabs_title'] = array(
        '#title'   => t('Tab title'),
        '#type'    => 'select',
        '#options' => $title_options,
        '#default_value' => $this->options['references_tabs_title'],
      );
    }
  }
  
  /**
   * Render the display in this style.
   *
   * This is overridden so that we can render our grouping specially.
   */
  function render() {
    if (empty($this->row_plugin)) {
      debug('references_tabs_views_handler: Missing row plugin');
      return;
    }
    
    $types_map = array(
      REFERENCES_TABS_TYPE_VERTICAL => 'vertical_tabs',
      REFERENCES_TABS_TYPE_HORIZONTAL => 'horizontal_tabs',
    );
    $type = $types_map[$this->options['references_tabs_type']];

    // Make sure tab type has been set.
    if (!$type) {
      debug('references_tabs_views_handler: Tab type not set');
      return;
    }

    // Create tab container
    $tabs = array( '#type' => $type );    

    // Horizontal tabs require the drupal.form library, or tabs will not
    // be built properly because of javascript errors (undefined
    // callback $.fn.drupalGetSummary). This should probably be fixed in the
    // module providing horizontal_tabs (i.e. field_group or elements).
    $tabs['#attached']['library'][] = array('system', 'drupal.form');
    
    $view_name = $this->view->name . '__' . $this->view->current_display;
    
    // Defaults for a tab fieldset
    $tabdefault = array(
      '#type' => 'fieldset',
      '#group' => $view_name,
      '#collapsed' => FALSE,
      '#collapsible' => TRUE,
    );
    
    // Make sure fields are rendered
    $this->render_fields($this->view->result);

    // Build fieldsets (=tabs) for each referenced node
    foreach ($this->view->result as $delta => $row) {
      $node = node_load($row->nid);
    
      // create fieldset
      $tab_id = 'group_nodereference_tabs_' . $view_name . '_tab' . $delta;
      $tab = array() + $tabdefault;
      
      if ($this->uses_fields()) {
        if ($this->options['references_tabs_title'] != REFERENCES_TABS_TITLE_NODETITLE) {
          $tab['#title'] = $this->get_field($delta, $this->options['references_tabs_title']);
        }
      }

      // use node title if tab title is not set yet
      if (empty($tab['#title'])) {
        $tab['#title'] = $node->title;
      }

      // attach (rendered) node as tab child
      $this->view->row_index = $delta;
      $tab[] = array('#markup' => $this->row_plugin->render($row));

      // attach tab to tabs container
      $tabs[] = $tab;
    }
    
    unset($this->view->row_index);
    return render($tabs);
  }
}
