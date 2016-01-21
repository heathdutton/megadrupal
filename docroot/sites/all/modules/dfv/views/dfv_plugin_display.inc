<?php

/**
 * @file
 * Handler for dfv_plugin_display.
 */

class dfv_plugin_display extends views_plugin_display {

  /**
   * Define options.
   */
  public function option_definition() {
    $options = parent::option_definition();

    // Force the style plugin to 'dfv_style' and the row plugin to 'fields'.
    $options['style_plugin']['default'] = 'dfv_style';
    $options['defaults']['default']['style_plugin'] = FALSE;
    $options['defaults']['default']['style_options'] = FALSE;
    $options['row_plugin']['default'] = 'dfv_fields';
    $options['defaults']['default']['row_plugin'] = FALSE;
    $options['defaults']['default']['row_options'] = FALSE;

    // Set the display title to an empty string (not used in this display type).
    $options['title']['default'] = '';
    $options['defaults']['default']['title'] = FALSE;

    return $options;
  }

  /**
   * Return display type name.
   */
  public function get_style_type() {
    return 'dfv';
  }

  /**
   * View gets run and returns data in the format required.
   */
  public function execute() {
    return $this->view->render($this->display->id);
  }

  /**
   * Render this display.
   */
  public function render() {
    if (!empty($this->view->result) || !empty($this->view->style_plugin->definition['even empty'])) {
      return $this->view->style_plugin->render($this->view->result);
    }
    return '';
  }

  /**
   * Determine if this display uses exposed filters.
   */
  public function uses_exposed() {
    return FALSE;
  }

  /**
   * Inject anything into the query that the display handler needs.
   */
  public function query() {
    $options = $this->get_option('dfv_options');

    // Play nice with View UI 'preview' : if the view is not executed
    // don't alter the query.
    if (empty($options)) {
      return;
    }

    // Make sure the id field is included in the results, and save its alias
    // so that dfv_plugin_style can retrieve it.
    $this->id_field_alias = $this->view->query->add_field($this->view->base_table, $this->view->base_field);

    // Restrict on the incoming string, or incoming ids.
    if ($options['string'] !== '') {
      switch ($options['match']) {
        case 'equals':
          $operator = '=';
          $value = $options['string'];
          break;

        case 'starts_with':
          $operator = 'LIKE';
          $value = db_like($options['string']) . '%';
          break;

        case 'contains':
        default:
          $operator = 'LIKE';
          $value = '%' . db_like($options['string']) . '%';
          break;
      }

      $table_alias = $this->view->query->ensure_table($this->view->base_table);
      $this->view->query->add_where(NULL, $table_alias . '.' . $options['title_field'], $value, $operator);
    }
    elseif ($options['ids']) {
      $table_alias = $this->view->query->ensure_table($this->view->base_table);
      $this->view->query->add_where(NULL, $table_alias . '.' . $this->view->base_field, $options['ids'], 'IN');
    }
  }
}
