<?php
/**
 * Field handler to gather multiple fields into one.
 */
class views_compound_field_handler_field_gathering extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();

    $options['included_fields'] = array('default' => array());

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = array();
    $fields = $this->view->display_handler->get_handlers('field');
    // Build up the options from all the fields up to this one but no further.
    foreach ($fields as $field => $handler) {
      if ($field == $this->field) {
        break;
      }
      $label = empty($handler->options['label']) ? $field : $handler->options['label'];
      $options[$field] = $label;
    }

    $form['included_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Gathered fields'),
      '#options' => $options,
      '#default_value' => isset($this->options['included_fields']) ? $this->options['included_fields'] : '',
      '#description' => t('Fields to gather. Only fields that are ordered above this one may be included. You should set the fields selected here to exclude from display if you do not want them output twice.'),
    );
  }

  /**
   * Validate the form options to check we have two or more fields selected.
   */
  function options_validate($form, &$form_state) {
    parent::options_validate($form, $form_state);

    if (count(array_filter($form_state['values']['options']['included_fields'])) < 2) {
      form_error($form['included_fields'], t('You must select at least two fields.'));
    }
  }

  /**
   * Validate the whole view to check a gathering field is not on a table style
   * with which it is incompatible.
   */
  function validate() {
    $errors = parent::validate();

    foreach ($this->view->display as $display_id => $display) {
      if (isset($display->display_options['fields'])) {
        // Check each field in a display that has them for a gathering field;
        // this is because we can't tell which display we are on.
        foreach ($display->display_options['fields'] as $field_id => $field) {
          if ($field['field'] == 'views_gathering') {
            if ($display->display_options['style_plugin'] == 'table') {
              $errors[] = t('The Gathering field is not compatible with the table style.');
            }
          }
        }
      }
    }

    return $errors;
  }

  function query() {
    // do nothing -- to override the parent query.
  }

  function render($values) {
    // Clone the view so we can mess around with the fields.
    $view_inner = $this->view->clone_view();

    // Views' shallow cloning loses us a bunch of stuff, such as...
    $current_display_id = $this->view->current_display;
    // Add a clone of the style plugin so we have something to theme with.
    $view_inner->style_plugin = clone($this->view->style_plugin);

    $fields = array_intersect_key($this->view->display_handler->get_handlers('field'), array_filter($this->options['included_fields']));
    foreach ($fields as $field => $handler) {
      // Clone the field handler.
      $handler_clone = clone($handler);
      // Force its visibility, as we go through the same theme functions that
      // will skip it if 'exclude' is set.
      $handler_clone->options['exclude'] = FALSE;
      $view_inner->field[$field] = $handler_clone;
    }

    // We call the current row plugin for some data: the list of theme
    // functions, options, and the field alias (whatever that does).
    // @todo: make it work with Semantic Views.
    $row_plugin = $this->view->style_plugin->row_plugin;
    $output = theme($row_plugin->theme_functions(), array(
        'view' => $view_inner,
        'options' => $row_plugin->options,
        'row' => $values,
        'field_alias' => $row_plugin->field_alias,
      )
    );

    return $output;
  }
}
