<?php
/**
 * @file
 * Views dynamic fields filter handler.
 */

/**
 * Simple filter to pick and choose the fields to show in the view.
 */
class views_handler_filter_dynamic_fields extends views_handler_filter {

  // Array to store combined fields as an associative array (parent => child)
  // E.g. using table style plugin, one can set the column of one field as that
  // of another so they appear as one column in the output.
  public $combined_fields = array();

  // Array to store the fields to be used in the exposed filter.
  public $view_fields = array();

  /**
   * Enables handler options.
   */
  public function has_extra_options() {
    return TRUE;
  }

  /**
   * Defines handler options.
   */
  public function option_definition() {
    $options = parent::option_definition();

    // Fields to display in exposed filter.
    $options['filterable_fields'] = array('default' => array());

    // Fields already displayed when user does no filtering.
    // @see http://drupal.org/node/1039760
    $options['defaults_filterable_fields'] = array('default' => array());

    // Choose between checkboxes or select.
    // @see http://drupal.org/node/1039760
    $options['checkboxes'] = array('default' => TRUE);

    // Boolean to control display of Exclusion mode.
    $options['reverse'] = array('default' => 0);

    // Label for the Exclusion mode.
    $options['reverse_label'] = array('default' => '');

    // Custom description.
    // @see http://drupal.org/node/1958930
    $options['description'] = array('default' => '');
    return $options;
  }

  /**
   * Pre-set the filter fields and combined fields arrays.
   *
   * @see \views\handler\views_handler_filter::init()
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);
    $this->view_fields = $view->get_items('field');
    $this->remove_combined_fields($view);

    // Select all fields by default when setting up a new filter.
    $this->options['filterable_fields']
      = empty($this->options['filterable_fields'])
        ? array_keys($this->view_fields) : $this->options['filterable_fields'];
  }

  /**
   * Prune fields to be displayed in the filter and capture combined fields sep.
   */
  protected function remove_combined_fields($view) {
    $this->combined_fields = array();
    $style_columns
      = isset($view
              ->display_handler
              ->default_display
              ->options['style_options']['columns'])
                ? $view
                  ->display_handler
                  ->default_display
                  ->options['style_options']['columns'] : NULL;
    if (is_array($style_columns)) {
      foreach ($style_columns as $key => $name) {
        if (isset($this->view_fields[$key]) && $key != $name) {
          unset($this->view_fields[$key]);

          // The $key field will be acted upon if the $name (parent) field
          // is acted upon by the user.
          $this->combined_fields[$name] = $key;
        }
      }
    }
  }

  /**
   * Provide an 'exclusion mode' option.
   *
   * @see \views\handler\views_handler::extra_options_form()
   */
  public function extra_options_form(&$form, &$form_state) {

    // Get all non-excluded fields.
    $view_fields = $this->view_fields;
    foreach ($view_fields as $field_name => $field) {
      if (empty($field['label'])) {
        $field_label = ucfirst($field['id']);
      }
      else {
        $field_label = $field['label'];
      }

      // Do not allow pre-configured excluded fields in the filter.
      if (isset($field['exclude']) && $field['exclude']) {
        $fields_excluded[$field_name] = $field_label;
      }
      else {
        $fields[$field_name] = $field_label;
      }
    }

    $form['description'] = array(
      '#type' => 'textfield',
      '#default_value' => $this->options['description'],
      '#title' => t('Exposed filter description'),
      '#description'
      => t('Change the default description for the exposed dynamic fields filter.'),
      '#size' => 40,
    );

    // Switch multi select mode vs single select.
    // @see http://drupal.org/node/1299806
    $form['checkboxes'] = array(
      '#type' => 'checkbox',
      '#default_value' => $this->options['checkboxes'],
      '#title' => t('List filters in checkboxes'),
      '#description'
      => t('If selected the fields will be listed as checkboxes (multi select mode), otherwise they will be listed in a select box (single select).'),
    );

    $form['filterable_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Selectable Fields'),
      '#description'
      => t('Select the fields that you want to make available for filtering. Unchecked fields will not be filterable and will always show up in the view.'),
      '#options' => $fields,
      '#default_value' => $this->options['filterable_fields'],
    );

    $form['defaults_filterable_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Default displayed fields'),
      '#description'
      => t('Select the fields that you want displayed by default in the view when no filtering is applied. Any explicitly unchecked fields in the list above will also be included as permanent fields. Leave empty to choose all fields.'),
      '#options' => $fields,
      '#default_value' => $this->options['defaults_filterable_fields'],
    );

    // Fields may been excluded from the view in field settings.
    if ($fields_excluded) {
      $form['filterable_fields_excluded'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Excluded fields'),
        '#options' => $fields_excluded,
        '#description'
        => t('You have configured these fields to be excluded from the view, hence they will not appear as filters.'),
        '#disabled' => TRUE,
      );
    }

    $form['reverse'] = array(
      '#type' => 'checkbox',
      '#default_value' => $this->options['reverse'],
      '#title' => t('Provide exclusion mode'),
      '#description'
      => t('Provide a checkbox to reverse the selection from inclusion to exclusion.'),
    );

    $form['reverse_label'] = array(
      '#type' => 'textfield',
      '#default_value' => $this->options['reverse_label'],
      '#title' => t('Label for exclusion fields options'),
      '#description' => t('Change the default label for the exclusion mode.'),
      '#size' => 40,
    );
  }

  /**
   * Provide a list of all fields that are for display.
   */
  public function value_form(&$form, &$form_state) {

    // All the non-permanently excluded fields of the view that we will display
    // in the exposed filter.
    $all_fields = array();

    // Just the names of all the non-permanently excluded fields.
    $field_names = array();
    $view_fields = $this->view_fields;
    $combined_fields = $this->combined_fields;

    // The final fields that are part of the filter after going through a
    // pruning process.
    $displayed_field_names = array();

    $filterable_fields = array_filter($this->options['filterable_fields']);
    $defaults_filterable_fields
      = array_filter($this->options['defaults_filterable_fields']);

    // Create fields to be displayed as part of the filter.
    foreach ($view_fields as $field_name => $field) {

      // Apply filterable fields options removing any field not in the set of
      // filterable fields.
      if (!empty($filterable_fields) &&
          !isset($filterable_fields[$field_name])) {
        continue;
      }

      if (!isset($_SESSION[$this->view->name]['perm_exc']) &&
          isset($field['exclude']) && $field['exclude']) {

        // Fields already marked as 'exclude from display' in the original
        // view should be omitted.
        $_SESSION[$this->view->name]['perm_exc'][$field_name] = $field_name;
        continue;
      }

      if (!isset($_SESSION[$this->view->name]['perm_exc'][$field_name])) {

        // Use the field id as label if label is empty
        // @see http://drupal.org/node/1951742
        if (empty($field['label'])) {
          $all_fields[$field_name] = ucfirst($field['id']);
        }
        else {
          $all_fields[$field_name] = $field['label'];
        }

        $field_names[] = $field_name;

        // Get fields to display in the plain (unfiltered) view.
        if (isset($defaults_filterable_fields[$field_name]) &&
            $defaults_filterable_fields[$field_name] == $field_name) {
          $field_names_defaults[$field_name] = $field_name;
        }
      }
    }

    // Also add any combined fields to the default fields to display
    // as configured.
    foreach ($combined_fields as $c_parent => $c_child) {
      if ($defaults_filterable_fields[$c_parent] == $c_parent) {
        $field_names_defaults[$c_parent] = $c_parent;
      }
    }

    // Initialize the permanent exclusion array if not set.
    if (!isset($_SESSION[$this->view->name]['perm_exc'])) {
      $_SESSION[$this->view->name]['perm_exc'] = array();
    }

    // Display all the filter fields in the plain view if no explicit.
    // defaults are set in the view.
    $field_names_defaults
      = empty($field_names_defaults) ? $all_fields : $field_names_defaults;

    if ($form_state['exposed']) {

      // Get submitted filter data if any.
      $exposed_input = $this->view->get_exposed_input();

      if (isset($exposed_input[$this->options['expose']['identifier']])) {
        $exposed_input_fields
          = $exposed_input[$this->options['expose']['identifier']];
        $exposed_field_names
          = isset($exposed_input['field_names'])
          ? unserialize($exposed_input['field_names']) : $field_names;
      }
      $this->options['expose']['multiple'] = $this->options['checkboxes'];

      // If checkboxes has not been enabled, display the fields in as
      // select list / drop down.
      if (!$this->options['checkboxes']) {
        $form['value'] = array(
          '#type' => 'select',
          '#no_convert' => $this->options['checkboxes'],
          '#title' => $this->options['expose']['label']
          ? $this->options['expose']['label'] : t('Fields'),
          '#options' => $all_fields,
          '#description' => $this->options['description']
          ? check_plain($this->options['description'])
          : t('Select the field to display in the view'),
        );
        $displayed_field_names = $field_names;
      }
      else {

        // For a view that has already been filtered by the user.
        if (!empty($exposed_input_fields)) {

          $delta = count($exposed_input_fields);
          $key = 0;

          // Regenerate the sortable table.
          foreach ($exposed_input_fields as $oldid => $oldvalue) {
            $def_check = (isset($oldvalue['check']) && $oldvalue['check']) ||
                          empty($all_fields[$exposed_field_names[$oldid]]);
            $def_sort = $oldvalue['sort'] ? $oldvalue['sort'] : $key;
            $def_disabled = empty($all_fields[$exposed_field_names[$oldid]]);
            $def_title = '';
            if (isset($all_fields[$exposed_field_names[$oldid]])
              && !empty($all_fields[$exposed_field_names[$oldid]])) {
              $def_title = $all_fields[$exposed_field_names[$oldid]];
            }
            else {
              if (empty($view_fields[$exposed_field_names[$oldid]]['label'])) {
                $def_title = ucfirst($view_fields[$exposed_field_names[$oldid]]['id']);
              }
              else {
                $def_title = $view_fields[$exposed_field_names[$oldid]]['label'];
              }
            }
            $def_desc = empty($all_fields[$exposed_field_names[$oldid]])
            ? t('Always visible') : '';
            $form['value'][$key]['check'] = array(
              '#type' => 'checkbox',
              '#value' => $def_check,

              // Set field to readonly rather than disbaling them.
              // @see http://drupal.org/node/2291727
              '#attributes'
              => $def_disabled ? array('readonly' => 'readonly') : '',
              '#description' => check_plain($def_desc));
            $form['value'][$key]['title'] = array('#markup' => $def_title);
            $form['value'][$key]['sort'] = array(
              '#type' => 'weight',
              '#delta' => $delta,
              '#value' => $def_sort,
              '#default_value' => $def_sort,
            );
            $displayed_field_names[] = $exposed_field_names[$oldid];
            ++$key;
          }
        }

        // First time (or no exposed filter data).
        else {

          $delta = count($view_fields) - count($combined_fields);
          $key = 0;

          // Create the sortable table.
          foreach ($view_fields as $field_title => $field_info) {

            // Skip over permanently excluded fields.
            if (isset($field_info['exclude']) && $field_info['exclude']) {
              continue;
            }
            // Display only default filterable fields
            // if there is no exposed input.
            $def_check
              = in_array($field_title, array_keys($field_names_defaults)) ||
              empty($all_fields[$field_title]);
            $def_sort = $key;
            $def_desc
              = empty($all_fields[$field_title]) ? t('Always visible') : '';
            $def_disabled = empty($all_fields[$field_title]);
            $def_title = '';
            if (isset($all_fields[$field_title]) && !empty($all_fields[$field_title])) {
              $def_title = $all_fields[$field_title];
            }
            else {
              if (empty($view_fields[$field_title]['label'])) {
                $def_title = ucfirst($view_fields[$field_title]['id']);
              }
              else {
                $def_title = $view_fields[$field_title]['label'];
              }
            }
            $form['value'][$key]['check']
              = array(
                '#type' => 'checkbox',
                '#value' => $def_check,

                // Set field to readonly rather than disbaling them.
                // @see http://drupal.org/node/2291727
                '#attributes'
                => $def_disabled ? array('readonly' => 'readonly') : '',
                '#description' => check_plain($def_desc));
            $form['value'][$key]['title']
              = array('#markup' => $def_title);
            $form['value'][$key]['sort'] = array(
              '#type' => 'weight',
              '#delta' => $delta,
              '#default_value' => $def_sort,
            );
            $displayed_field_names[] = $field_title;
            ++$key;
          }
        }

        // Theme the draggable sort table.
        $form['value']['#theme'] = 'views_dynamic_fields_sort_filter_fields';
      }

      $form['value']['#tree'] = TRUE;
      $form['value']['#title'] = $this->options['expose']['label']
        ? $this->options['expose']['label'] : t('Fields');

      // Store the fields being displayed in the filter for use
      // as form data when processing filter submissions.
      $form['field_names'] = array(
        '#type' => 'hidden',
        '#value' => serialize($displayed_field_names),
      );

      $form['combined'] = array(
        '#type' => 'hidden',
        '#value' => serialize($combined_fields),
        '#weight' => 9,
      );
      if ($this->options['reverse']) {
        $form['reverse'] = array(
          '#type' => 'checkbox',
          '#default_value' => $this->options['reverse'],
          '#title' => $this->options['reverse_label']
          ? $this->options['reverse_label']
          : t('Selected fields will be excluded'),
          '#description'
          => t('Check this to exclude the selected fields from the view.'),
        );
      }
    }
    else {

      // Ensure there is something in the 'value'.
      $form['value'] = array(
        '#type' => 'value',
        '#value' => NULL,
      );
    }
  }

  /**
   * Exclude fields from the view before the query is run.
   *
   * @see \views\handler\views_handler::pre_query()
   */
  public function pre_query() {

    // Fields submitted by user in the exposed filter.
    $exposed_input = $this->view->get_exposed_input();

    if (isset($exposed_input[$this->options['expose']['identifier']])) {
      $exposed_input_fields
        = $exposed_input[$this->options['expose']['identifier']];
    }

    $defaults_filterable_fields
      = array_filter($this->options['defaults_filterable_fields']);
    $filterable_fields = array_filter($this->options['filterable_fields']);

    // Get all fields.
    foreach ($this->view->field as $field_name => $field) {
      $field_names[] = $field_name;

      // Default view when no filtering is applied.
      if (empty($exposed_input_fields)) {
        if (!in_array($field_name, array_keys($defaults_filterable_fields)) &&
            isset($filterable_fields[$field_name])) {
          $this->view->field[$field_name]->options['exclude'] = 1;
        }
      }
    }
    if (!empty($exposed_input_fields)) {

      // Exclude these fields.
      $combined_fields = isset($exposed_input['combined'])
        ? unserialize($exposed_input['combined']) : array();
      if (!$this->options['checkboxes']) {
        $this
          ->pre_query_single($exposed_input, $field_names, $combined_fields);
      }
      else {
        $this->pre_query_multiple($exposed_input, $field_names, $combined_fields);
      }
    }
  }

  /**
   * Handle pre_query processing when checkboxes are not enabled.
   */
  private function pre_query_single($exposed_input,
                                    $field_names,
                                    $combined_fields) {

    // If exclusion mode is selected.
    if (isset($exposed_input['reverse']) && $exposed_input['reverse']) {
      $fields_to_exclude
        = $exposed_input[$this->options['expose']['identifier']];
    }
    else {
      $selected_fields = $exposed_input[$this->options['expose']['identifier']];
      if (!is_array($selected_fields)) {
        $selected_fields = array($selected_fields);
      }
      if ($selected_fields[0] == 'All') {
        $fields_to_exclude = array();
      }
      else {
        $fields_to_exclude = array_diff($field_names, $selected_fields);
      }
    }
    $filterable_fields = array_filter($this->options['filterable_fields']);

    // If only one field is selected, this ensures no issues with array.
    // @see http://drupal.org/node/1831858
    if (!is_array($fields_to_exclude)) {
      $fields_to_exclude = array($fields_to_exclude);
    }

    // Also include/exclude the fields combined into any
    // of the $exposed_input fields.
    foreach ($fields_to_exclude as $key => $efield) {

      // Add the combined field to the exclusion array
      // if parent is filterable and excluded.
      if (isset($combined_fields[$efield]) &&
          $this->options['filterable_fields'][$efield] !== 0) {
        $fields_to_exclude[$combined_fields[$efield]] = $combined_fields[$efield];
      }

      // On the other hand, the combined field should be include.
      // in the view if the parent is.
      if (in_array($efield, $combined_fields)) {
        unset($fields_to_exclude[$key]);
      }
    }

    // Mark the fields to be excluded to the view object.
    foreach ($fields_to_exclude as $field_to_exclude) {

      // Exclude only the fields chosen as excludable in filter options.
      if (empty($filterable_fields) ||
          (isset($this->options['filterable_fields'][$field_to_exclude]) &&
            $this->options['filterable_fields'][$field_to_exclude] !== 0)) {
        $this->view->field[$field_to_exclude]->options['exclude'] = 1;
      }
    }
  }

  /**
   * Handle pre_query processing when checkboxes are enabled.
   */
  private function pre_query_multiple($exposed_input,
                                      $field_names,
                                      $combined_fields) {
    $exposed_input_fields
      = $exposed_input[$this->options['expose']['identifier']];

    // Fields displayed in the filter.
    $form_field_names = isset($exposed_input['field_names'])
      ? unserialize($exposed_input['field_names']) : $field_names;
    $orig_field_exposed = $order_includes = $order_excludes = array();
    foreach ($exposed_input_fields as $id => $info) {
      $orig_field_exposed[$info['sort']] = $form_field_names[$id];

      // Unset fields that haven't been selected.
      if (!isset($info['check']) || $info['check'] != 1) {
        unset($exposed_input_fields[$id]);
        $order_includes[$info['sort']] = $form_field_names[$id];
      }
      else {
        $order_excludes[$info['sort']] = $form_field_names[$id];
      }
    }

    // Sorted order of fields for display.
    ksort($orig_field_exposed);
    ksort($order_includes);
    ksort($order_excludes);

    // If exclusion mode is selected.
    if (isset($exposed_input['reverse']) && $exposed_input['reverse']) {
      $fields_to_exclude = $order_excludes;
    }
    else {
      $fields_to_exclude = $order_includes;
    }

    // If only one field is selected, this ensures no issues with array.
    // @see http://drupal.org/node/1831858
    if (!is_array($fields_to_exclude)) {
      $fields_to_exclude = array($fields_to_exclude);
    }

    // Also include/exclude the fields combined into any
    // of the $exposed_input fields.
    foreach ($fields_to_exclude as $key => $efield) {

      // Add the combined field to the exclusion array if parent
      // is filterable and excluded.
      if (isset($combined_fields[$efield]) &&
          $this->options['filterable_fields'][$efield] !== 0) {
        $fields_to_exclude[$combined_fields[$efield]]
          = $combined_fields[$efield];
      }

      // On the other hand, the combined field should be included
      // in the view if the parent is.
      if (in_array($efield, $combined_fields)) {
        unset($fields_to_exclude[$key]);
      }
    }

    // Mark the fields to be excluded to the view object.
    foreach ($fields_to_exclude as $field_to_exclude) {

      // Exclude only the fields chosen as excludable in filter options.
      if ($this->options['filterable_fields'][$field_to_exclude] !== 0) {
        $this->view->field[$field_to_exclude]->options['exclude'] = 1;
      }
    }

    // Reorder the view's fields.
    $view_fields = $this->view->field;
    $view_fields_ordered = array();
    foreach ($orig_field_exposed as $field_title) {
      $view_fields_ordered[$field_title] = $view_fields[$field_title];
      unset($view_fields[$field_title]);
    }

    // Check if any still remaning (hidden fields, already excluded fields).
    if (!empty($view_fields)) {
      foreach ($view_fields as $field_title => $field_info) {
        $view_fields_ordered[$field_title] = $field_info;
      }
    }
    $this->view->field = $view_fields_ordered;
  }


  /**
   * Do nothing in the query.
   *
   * This function needs to be defined and left to do nothing.
   *
   * @see \views\handler\views_handler_filter::query()
   */
  public function query() {}
}
