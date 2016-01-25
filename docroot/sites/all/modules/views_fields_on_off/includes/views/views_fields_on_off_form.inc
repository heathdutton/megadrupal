<?php

/**
 * @file
 * Definition of views_fields_on_off_form.
 */

/**
 * Provides a handler that adds the form for Fields On/Off.
 */
class views_fields_on_off_form extends views_handler_field {
  function can_expose() {
    return TRUE;
  }

  function is_exposed() {
    return TRUE;
  }

  function exposed_form(&$form, &$form_state) {
    $fields = $this->options['fields'];
    $options = array();
    $checked = array();

    $all_fields = $this->get_field_labels_no_relationship_names($this->view->display_handler);

    // Grab the fields_on_off values that have been submitted already.
    $params = drupal_get_query_parameters();
    $on_off_submitted = array_key_exists('fields_on_off_hidden_submitted', $params);
    $checked_fields = array_key_exists('fields_on_off', $params) ? $params['fields_on_off'] : array();

    // Now loop through the fields defined in the view.
    foreach ($fields as $field) {
      if ($field) {
        $id = $field;
        if (array_key_exists($id, $all_fields)) {
          $label = $all_fields[$id];
          $options[$id] = $label;

          // If the field is included on the querystring...
          $check_me = (!count($checked_fields) && !$on_off_submitted) || array_key_exists($id, $checked_fields);
          if ($check_me) {
            // Check it because it has already been selected
            $checked[$id] = TRUE;
          }
        }
      }
    }

    // Form API to build the checkboxes.
    $form['fields_on_off'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Show Fields'),
      '#description' => t('Select the fields you want to display'),
      '#options' => $options,
      '#value' => $options,
      // I don't know why we have to include #options and #value, but it
      // doesn't work if we don't...
    );

    $form["fields_on_off_hidden_submitted"] = array(
      '#type' => 'hidden',
      '#default_value' => 1,
    );

    // Have to use $form_state['input'] because setting the default values on
    // the form field itself doesn't work.
    // Because of how Views handles the exposed filters,
    // this is how we set our values in the form.
    $form_state['input']['fields_on_off'] = $checked;
  }


  function option_definition() {
    $options = parent::option_definition();

    $options['fields'] = array('default' => array());

    return $options;
  }

  /**
   * Retrieve a list of fields for the current display.
   *
   * @param $display_handler
   * @return array
   *   array of Views options
   */
  function get_field_labels_no_relationship_names($display_handler) {
    $groupable_only = FALSE;

    $options = array();
    foreach ($display_handler->get_handlers('field') as $id => $handler) {
      if ($groupable_only && !$handler->use_string_group_by()) {
        // Continue to next handler if it's not groupable.
        continue;
      }
      if ($label = $handler->label()) {
        $options[$id] = $label;
      }
      else {
        $options[$id] = $handler->ui_name();
      }
    }
    return $options;
  }


  function options_form(&$form, &$form_state) {
    $all_fields = $this->get_field_labels_no_relationship_names($this->view->display_handler);
    // Remove any field that have been excluded from the display from the list.
    foreach ($all_fields as $key => $field) {
      $exclude = $this->view->display_handler->handlers['field'][$key]->options['exclude'];
      if ($exclude) {
        unset($all_fields[$key]);
      }
    }

    // Offer to include only those fields that follow this one.
    $field_options = array_slice($all_fields, 0, array_search($this->options['id'], array_keys($all_fields)));
    $form['fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Fields'),
      '#description' => t('Fields to be turned on and off.'),
      '#options' => $field_options,
      '#default_value' => $this->options['fields'],
    );
  }

  function query() {
    // This is not a real field and it only affects the query by excluding
    // fields from the display. But Views won't render if the query()
    // method is not present. This doesn't do anything, but it has to be here.
    // This function is a void so it doesn't return anything.
  }
}
