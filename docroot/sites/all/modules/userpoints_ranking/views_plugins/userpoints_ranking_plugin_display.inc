<?php

/**
 * @file
 * PDF display plugin.
 */

/**
 * This class contains all the functionality of the PDF display.
 */
class userpoints_ranking_plugin_display extends views_plugin_display {

  /**
   * Define the display type
   */
  function get_style_type() { return 'userpoints_ranking_style'; }

  /**
   * Disable the breadcrumb
   */
  function uses_breadcrumb() { return FALSE; }

  function use_pager() { return FALSE; }

  function has_path() { return FALSE; }

  function execute() {
    return $this->view->render($this->display->id);
  }

  function render() {
    if (!empty($this->view->result) || !empty($this->view->style_plugin->definition['even empty'])) {
      return $this->view->style_plugin->render($this->view->result);
    }
    return '';
  }

  function uses_exposed() {
    return FALSE;
  }

  function query() {

  
    // Alter the query only if we are not in the preview mode.
    $period_id = $this->get_option('userpoints_ranking_period_id');
    
    $base_table = $this->view->query->ensure_table('users');
    
    $this->id_field_alias = $this->view->query->add_field($this->view->base_table, $this->view->base_field);
    
    if (empty($period_id)) {
      return;
    }
    
    
    
    $join = new views_join();
    $left_table = 'users';
    $left_field = 'uid';
    $right_table = 'userpoints_ranking';
    $right_field = 'uid';
    $type = 'LEFT';
    $extra = array(
      array(
        'field' => 'period_id',
        'value' => $period_id,
      )
    );
        
    $alias = $right_table . '_' . $left_table;
    
    $join->construct($right_table, $left_table, $left_field, $right_field, $extra, $type);
    
    $this->view->query->add_relationship($alias, $join, $right_table);  
    $this->view->query->add_where_expression(0, 'ranking_id IS NULL');
    
        
  }

  /**
   * The option definition.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['displays'] = array('default' => array());

    // Overrides for standard stuff:
    $options['style_plugin']['default'] = 'userpoints_ranking_style';
    $options['style_options']['default'] = array('mission_description' => FALSE, 'description' => '');
    $options['sitename_title']['default'] = FALSE;
    $options['defaults']['default']['style_plugin'] = FALSE;
    $options['defaults']['default']['style_options'] = FALSE;
    $options['defaults']['default']['row_plugin'] = FALSE;
    $options['defaults']['default']['row_options'] = FALSE;
    
    return $options;
  }

  
}
