<?php
/**
 * @file
 * Contains the list style plugin.
 */

/**
 * Style plugin to render each item of an ordered or unordered list.
 *
 * @ingroup views_style_plugins
 */
class views_jqm_plugin_style_basic_list extends views_plugin_style_list {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'ul');
    $options['searchable'] = array('default' => FALSE);
    $options['list_divided'] = array('default' => FALSE);
    $options['class'] = array('default' => 'ui-listview');
    $options['wrapper_class'] = array('default' => '');


    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('jQuery Mobile List type'),
      '#options' => array('ul' => t('Basic list'), 'ol' => t('Numbered list')),
      '#default_value' => $this->options['type'],
    );
    $form['searchable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add Search Filter for List'),
      '#default_value' => $this->options['searchable'],
    );
    $form['list_divided'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use Groupings as List Dividers'),
      '#default_value' => $this->options['list_divided'],
    );
    $form['wrapper_class'] = array(
      '#title' => t('Wrapper class'),
      '#description' => t('The class to provide on the wrapper, outside the list.'),
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['wrapper_class'],
    );
    $form['class'] = array(
      '#title' => t('List class'),
      '#description' => t('The class to provide on the list element itself.'),
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['class'],
    );
  }
  
  function render() {
    if (!$this->options['list_divided']) {
      if ($this->uses_row_plugin() && empty($this->row_plugin)) {
        debug('views_plugin_style_default: Missing row plugin');
        return;
      }
  
      // Group the rows according to the grouping field, if specified.
      $sets = $this->render_grouping($this->view->result, $this->options['grouping']);
  
      // Render each group separately and concatenate.  Plugins may override this
      // method if they wish some other way of handling grouping.
      $output = '';
      foreach ($sets as $title => $records) {
        if ($this->uses_row_plugin()) {
          $rows = array();
          foreach ($records as $row_index => $row) {
            $this->view->row_index = $row_index;
            $rows[$row_index] = $this->row_plugin->render($row);
          }
        }
        else {
          $rows = $records;
        }
  
        $output .= theme($this->theme_functions(),
          array(
            'view' => $this->view,
            'options' => $this->options,
            'rows' => $rows,
            'title' => $title)
        );
      }
      unset($this->view->row_index);
      return $output;
    }
    else {
        if ($this->uses_row_plugin() && empty($this->row_plugin)) {
          debug('views_plugin_style_default: Missing row plugin');
          return;
        }
    
        // Group the rows according to the grouping field, if specified.
        $sets = $this->render_grouping($this->view->result, $this->options['grouping']);
    
        // Render each group separately and concatenate.  Plugins may override this
        // method if they wish some other way of handling grouping.
        $output = '';
        $rows = array();
        foreach ($sets as $title => $records) {
          $rows[strtolower(str_replace(' ', '_',$title))] = $title;
          if ($this->uses_row_plugin()) {
            foreach ($records as $row_index => $row) {
              $this->view->row_index = $row_index;
              $rows[$row_index] = $this->row_plugin->render($row);
            }
          }
          else {
            $rows = $records;
          }
        }
        $output .= theme($this->theme_functions(),
            array(
              'view' => $this->view,
              'options' => $this->options,
              'rows' => $rows)
          );
        unset($this->view->row_index);
        return $output;
    }
  }

}
