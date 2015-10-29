<?php

/**
 * @file
 *   Views field handler for graph
 */

class statspro_views_handler_field_graph extends views_handler_field_numeric {
  var $_max_graph_value = 0;

  /**
   * Table name for max. calculation
   *
   * @var string
   */
  var $base_table = 'statspro_term';

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['display_mode'] = array(
      '#type' => 'select',
      '#title' => t('Display mode'),
      '#default_value' => $this->value,
      '#options' => array(
        'text' => t('String'),
        'graph' => t('Graph'),
      ),
      '#description' => t('Define the display mode for presentation.'),
    );

    $form['graph_size'] = array(
      '#type' => 'textfield',
      '#title' => t('Graph size'),
      '#default_value' => $this->options['graph_size'],
      '#description' => t('Define the maximum width of the graph. The larges value/image will have this size. Only used with display mode <em>graph</em>'),
      '#size' => 10,
      '#field_suffix' => t('px'),
    );
  }

  function render($values) {
    $estimation = $values->{$this->field_alias};
    $rc = '-';

    switch ($this->options['display_mode']) {
      case 'graph':
        if ($estimation) {
          $rc = $this->get_bar_graph($values);
        }
        break;

      default:
        if ($estimation) {
          $rc = $estimation;
        }
        break;
    }

    return $rc;
  }

  /**
   * Get HTML code for bar graph
   *
   * @param   object  $values
   * @return  string
   */
  function get_bar_graph($values) {
    $estimation = (int) $values->{$this->field_alias};
    $rc = '<div style="width: ' . (int) $this->options['graph_size'] . 'px">';
    $rc .= sprintf(
        '<img src="%s/images/bar_l.gif" height="10" alt="%s" title="%2$s" />',
        $this->get_base_path(),
        t('Left side of the bar representing the number !value', array('!value' => $estimation))
    );
    $rc .= sprintf(
        '<img src="%s/images/bar.gif" width="%s" height="10" alt="%s" title="%3$s" />',
        $this->get_base_path(),
        $this->get_current_graph_size($values),
        t('Middle of the bar representing the number !value', array('!value' => $estimation))
    );
    $rc .= sprintf(
        '<img src="%s/images/bar_r.gif" height="10" alt="%s" title="%2$s" />',
        $this->get_base_path(),
        t('Right side of the bar representing the number !value', array('!value' => $estimation))
    );
    $rc .= '</div>';

    return $rc;
  }

  /**
   * Get the maximum value of the graph
   */
  function get_max_graph_value() {
    // For now:
    return 200;
    
    $view_sql = $this->view->build_info['count_query'];
    $view_args = $this->view->build_info['query_args'];
    $pos = strpos($view_sql, ' FROM ');
    //TODO - clean this query.
    $max_query = 'SELECT MAX(' . $this->base_table . '.' . $this->field . ') AS max_value FROM ' . drupal_substr($view_sql, $pos + 6);

    // D6 version:
//     return db_query($max_query, $view_args)->fetchField();

    $result = db_query($max_query, $view_args);
    
    return $result->fetchField('max_value');
  }

  function get_current_graph_size($values) {
    if (!$this->_max_graph_value) {
      $this->_max_graph_value = $this->get_max_graph_value();
    }

    if (empty($this->options['graph_size'])) {
      $this->options['graph_size'] = 50;
    }

    return $values->{$this->field_alias} * ((((int) $this->options['graph_size']) - 6) / $this->_max_graph_value);
  }

  function get_base_path() {
    if (!$this->base_path) {
      $this->base_path = base_path() . drupal_get_path('module', 'statspro');
    }
    return $this->base_path;
  }
}
