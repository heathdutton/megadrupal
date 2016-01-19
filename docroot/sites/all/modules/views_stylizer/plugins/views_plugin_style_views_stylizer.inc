<?php

/**
 * Timeline style plugin that structures the results in the form of a single linear time line.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_views_stylizer extends views_plugin_style {

  function option_definition() {
    $options = parent::option_definition();

    $options['columns'] = array('default' => array());
    $options['default'] = array('default' => '');
    $options['info'] = array('default' => array());
    $options['override'] = array('default' => TRUE, 'bool' => TRUE);
    $options['sticky'] = array('default' => FALSE, 'bool' => TRUE);
    $options['order'] = array('default' => 'asc');
    $options['caption'] = array('default' => '', 'translatable' => TRUE);
    $options['summary'] = array('default' => '', 'translatable' => TRUE);
    $options['empty_table'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

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
   //   if ($field == $column || $columns[$column] == $column && !empty($sanitized[$column])) {
        $sanitized[$field] = $column;
   //   }
      // Since we set the field to itself initially, ignoring
      // the condition is ok; the field will get its column
      // status back.
    }

    return $sanitized;
  }

  /**
   * Create forms to hold these values allowing the user to change the values
   */
function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#markup' => '<div class="error messages">' . t('You need at least one field before you can configure your table settings') . '</div>',
      );
      return;
    }

    // Note: views UI registers this theme handler on our behalf. Your module
    // will have to register your theme handlers if you do stuff like this.
    $form['#theme'] = 'views_stylizer_style_plugin_table';

    $columns = $this->sanitize_columns($this->options['columns']);
   // $columns = $this->options['columns'];
    // Create an array of allowed columns from the data we know:
    $field_names = $this->display->handler->get_field_labels();

    if (isset($this->options['default'])) {
      $default = $this->options['default'];
      if (!isset($columns[$default])) {
        $default = -1;
      }
    }
    else {
      $default = -1;
    }
    
    /*
     * %node:schemaorg-images:value:?
     * %node:schemaorg-images-thumbnail:value:?
     * %node:schemaorg-titles:value:?
     */
    $schema_array = array(
        "none" => "<None>",
        "title" => "Title",
        "description" => "Description",
        "image" => "Image",
        "link" => "Link",
        "images-medium" => "Image(medium)",
        "images-thumbnail" => "Image(thumbnail)",
        "images-large" => "Image(large)",
    );

    foreach ($columns as $field => $column) {
      $safe = str_replace(array('][', '_', ' '), '-', $field);
      // the $id of the column for dependency checking.
      $id = 'edit-style-options-columns-' . $safe;
      $form['columns'][$field] = array(
        '#type' => 'select',
        '#options' => $schema_array,
        '#default_value' => $column,
      );



      // markup for the field name
      $form['info'][$field]['name'] = array(
        '#markup' => $field_names[$field],
      );
    }

    

    $form['description_markup'] = array(
      '#markup' => '<div class="description form-item">' . t('Place fields into columns; you may combine multiple fields into the same column. If you do, the separator in the column specified will be used to separate the fields. Check the sortable box to make that column click sortable, and check the default sort radio to determine which column will be sorted by default, if any. You may control column order and field labels in the fields section.') . '</div>',
    );
  }
}
