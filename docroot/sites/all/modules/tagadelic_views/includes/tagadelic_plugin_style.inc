<?php

/**
 * @file
 * Contains the tagadelic style plugin.
 */

/**
 * Style plugin to render a weighted set of taxonomy terms.
 *
 * @ingroup tagadelic_style_plugins
 */
class tagadelic_plugin_style extends views_plugin_style_list {

  private $cloud_tags = NULL;

  /**
   * Return the token replaced row class for the specified row.
   */
  function get_row_class($row_index) {
    $classes_string = parent::get_row_class($row_index);
    if($this->cloud_tags) {
      $weight = $this->cloud_tags[$row_index]->get_weight();
      $classes_string = implode(" ", array($classes_string, ' level'.$weight));
    }
    return $classes_string;
  }

  /**
   * Allow the style to do stuff before each row is rendered.
   *
   * @param $result
   *   The full array of results from the query.
   */
  function pre_render($result) {
    parent::pre_render($result);
    $this->build_cloud($result);
  }

  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['display_field'] = NULL;
    $options['count_field'] = NULL;
    // uses tagadelic ul.tag-cloud CSS class
    $options['class'] = array('default' => 'tag-cloud');
    return $options;
  }

  /**
   * Normalize a list of columns based upon the fields that are
   * available. This compares the fields stored in the style handler
   * to the list of fields actually in the view, removing fields that
   * have been removed and adding new fields in their own column.
   *
   * - Each field must be in a column.
   * - Each column must be based upon a field, and that field
   *   is somewhere in the column.
   * - Any fields not currently represented must be added.
   * - Columns must be re-ordered to match the fields.
   *
   * @param $columns
   *   An array of all fields; the key is the id of the field and the
   *   value is the id of the column the field should be in.
   * @param $fields
   *   The fields to use for the columns. If not provided, they will
   *   be requested from the current display. The running render should
   *   send the fields through, as they may be different than what the
   *   display has listed due to access control or other changes.
   *
   * @return array
   *    An array of all the sanitized columns.
   */
  function sanitize_columns($columns, $fields = NULL) {
    $sanitized = array();
    if ($fields === NULL) {
      $fields = $this->display->handler->get_option('fields');
    }
    // Preconfigure the sanitized array so that the order is retained.
    foreach ($fields as $field => $info) {
      // Set to itself so that if it isn't touched, it gets column
      // status automatically.
      $sanitized[$field] = $field;
    }

    foreach ($columns as $field => $column) {
      // first, make sure the field still exists.
      if (!isset($sanitized[$field])) {
        continue;
      }

      // If the field is the column, mark it so, or the column
      // it's set to is a column, that's ok
      if ($field == $column || $columns[$column] == $column && !empty($sanitized[$column])) {
        $sanitized[$field] = $column;
      }
      // Since we set the field to itself initially, ignoring
      // the condition is ok; the field will get its column
      // status back.
    }

    return $sanitized;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {

    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#markup' => '<div class="error messages">' . t('You need at least one field before you can configure your tagadelic settings') . '</div>',
      );
      return;
    }

    // Create an array of allowed columns from the data we know:
    $columns = $this->sanitize_columns($this->options['columns']);
    $field_names = $this->display->handler->get_field_labels();
    foreach($columns as $column_name => $column_label) {
      $columns[$column_name] = $field_names[$column_name];
    }

    $form['count_field'] = array(
      '#type' => 'select',
      '#title' => t('Count field'),
      '#default_value' => $this->options['count_field'],
      '#options' => $columns,
      '#description' => t('The field used to count the results for a row.'),
    );

    // standard settings for list style
    parent::options_form($form, $form_state);
  }

  function build_cloud($result) {

    drupal_add_css(drupal_get_path('module', 'tagadelic') . '/tagadelic.css');
    
    $count_field = $this->options['count_field'];
    $columns = $this->display->handler->get_option('fields');

    $count_field_alias = NULL;
    if(isset($this->view->field[$count_field])) {
      $count_field_alias = $this->view->field[$count_field]->field_alias;
    }

    if(!$count_field_alias ) {
      drupal_set_message("Please set which field you are using for counting results in the tagadelic display.", "error");
      return "";
    }

    $tags = array();
    foreach ($result as $index => $item) {
      $count =  floatval($item->{$count_field_alias});
      $tag = new TagadelicTag($index, "", $count);
      $tags[] = $tag;
    }

    $cloud = new TagadelicCloud($this->view->name, $tags);
    $this->cloud_tags = $cloud->get_tags();
  }

}
