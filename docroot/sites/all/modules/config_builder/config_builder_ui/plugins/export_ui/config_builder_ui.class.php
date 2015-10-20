<?php
/**
 * @file
 */

class config_builder_ui extends ctools_export_ui {
  /**
   * Build a row based on the item.
   *
   * By default all of the rows are placed into a table by the render
   * method, so this is building up a row suitable for theme('table').
   * This doesn't have to be true if you override both.
   */
  function list_build_row($item, &$form_state, $operations) {
    // Set up sorting
    $name = $item->{$this->plugin['export']['key']};
    $schema = ctools_export_get_schema($this->plugin['schema']);

    // Note: $item->{$schema['export']['export type string']} should have already been set up by export.inc so
    // we can use it safely.
    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$name] = empty($item->disabled) . $name;
        break;
      case 'title':
        $this->sorts[$name] = $item->{$this->plugin['export']['admin_title']};
        break;
      case 'name':
        $this->sorts[$name] = $name;
        break;
      case 'storage':
        $this->sorts[$name] = $item->{$schema['export']['export type string']} . $name;
        break;
    }

    $this->rows[$name]['data'] = array();
    $this->rows[$name]['class'] = !empty($item->disabled) ? array('ctools-export-ui-disabled') : array('ctools-export-ui-enabled');

    // If we have an admin title, make it the first row.
    if (!empty($this->plugin['export']['admin_title'])) {
      $this->rows[$name]['data'][] = array('data' => check_plain($item->{$this->plugin['export']['admin_title']}), 'class' => array('ctools-export-ui-title'));
    }
    $this->rows[$name]['data'][] = array('data' => check_plain($name), 'class' => array('ctools-export-ui-name'));
    $this->rows[$name]['data'][] = array('data' => l(base_path() . $item->path, $item->path), 'class' => array('ctools-export-ui-path'));
    $this->rows[$name]['data'][] = array('data' => check_plain($item->{$schema['export']['export type string']}), 'class' => array('ctools-export-ui-storage'));

    $ops = theme('links__ctools_dropbutton', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));

    $this->rows[$name]['data'][] = array('data' => $ops, 'class' => array('ctools-export-ui-operations'));

    // Add an automatic mouseover of the description if one exists.
    if (!empty($this->plugin['export']['admin_description'])) {
      $this->rows[$name]['title'] = $item->{$this->plugin['export']['admin_description']};
    }
  }

  /**
   * Provide the table header.
   *
   * If you've added columns via list_build_row() but are still using a
   * table, override this method to set up the table header.
   */
  function list_table_header() {
    $header = array();
    if (!empty($this->plugin['export']['admin_title'])) {
      $header[] = array('data' => t('Title'), 'class' => array('ctools-export-ui-title'));
    }

    $header[] = array('data' => t('Name'), 'class' => array('ctools-export-ui-name'));
    $header[] = array('data' => t('Path'), 'class' => array('ctools-export-ui-path'));
    $header[] = array('data' => t('Storage'), 'class' => array('ctools-export-ui-storage'));
    $header[] = array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations'));

    return $header;
  }

  /**
   * Execute the standard form for editing.
   *
   * By default, export UI will provide a single form for editing an object.
   */
  function edit_execute_form_standard(&$form_state) {
    $output = drupal_build_form('ctools_export_ui_edit_item_form', $form_state);
    if (!empty($form_state['executed'])) {
      $this->edit_save_form($form_state);

      // Flush all drupal caches.
      drupal_flush_all_caches();
    }

    return $output;
  }

  /**
   * Provide the actual editing form.
   */
  function edit_form(&$form, &$form_state) {
    $export_key = $this->plugin['export']['key'];
    $item = $form_state['item'];
    $schema = ctools_export_get_schema($this->plugin['schema']);

    if (!empty($this->plugin['export']['admin_title'])) {
      $form['info'][$this->plugin['export']['admin_title']] = array(
        '#type' => 'textfield',
        '#title' => t('Administrative title'),
        '#description' => t('This will appear in the administrative interface to easily identify it.'),
        '#default_value' => $item->{$this->plugin['export']['admin_title']},
      );
    }

    $form['info'][$export_key] = array(
      '#title' => t($schema['export']['key name']),
      '#type' => 'textfield',
      '#default_value' => $item->{$export_key},
      '#description' => t('The unique ID for this @export.', array('@export' => $this->plugin['title singular'])),
      '#required' => TRUE,
      '#maxlength' => 255,
    );

    if (!empty($this->plugin['export']['admin_title'])) {
      $form['info'][$export_key]['#type'] = 'machine_name';
      $form['info'][$export_key]['#machine_name'] = array(
        'exists' => 'ctools_export_ui_edit_name_exists',
        'source' => array('info', $this->plugin['export']['admin_title']),
      );
    }

    if ($form_state['op'] === 'edit') {
      $form['info'][$export_key]['#disabled'] = TRUE;
      $form['info'][$export_key]['#value'] = $item->{$export_key};
    }

    if (!empty($this->plugin['export']['admin_description'])) {
      $form['info'][$this->plugin['export']['admin_description']] = array(
        '#type' => 'textarea',
        '#title' => t('Administrative description'),
        '#default_value' => $item->{$this->plugin['export']['admin_description']},
      );
    }

    // Add plugin's form definitions.
    if (!empty($this->plugin['form']['settings'])) {
      // Pass $form by reference.
      $this->plugin['form']['settings']($form, $form_state);
    }

    // Add the buttons if the wizard is not in use.
    if (empty($form_state['form_info'])) {
      // Make sure that whatever happens, the buttons go to the bottom.
      $form['buttons']['#weight'] = 100;

      // Add buttons.
      $form['buttons']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#limit_validation_errors' => array(
          array('access'),
          array('description'),
          array('form_builder_id'),
          array('form_builder_positions'),
          array('label'),
          array('name'),
          array('path'),
        ),
        '#submit' => array('ctools_export_ui_edit_item_form_submit'),
      );

      $form['buttons']['delete'] = array(
        '#type' => 'submit',
        '#value' => $item->export_type & EXPORT_IN_CODE ? t('Revert') : t('Delete'),
        '#access' => $form_state['op'] === 'edit' && $item->export_type & EXPORT_IN_DATABASE,
        '#submit' => array('ctools_export_ui_edit_item_form_delete'),
      );
    }
  }

  /**
   * Set an item's state to enabled or disabled and output to user.
   *
   * If javascript is in use, this will rebuild the list and send that back
   * as though the filter form had been executed.
   */
  function set_item_state($state, $js, $input, $item) {
    ctools_export_crud_set_status($this->plugin['schema'], $item, $state);

    // Rebuild index and flush caches.
    form_builder_crud_index_save();
    drupal_flush_all_caches();

    if (!$js) {
      drupal_goto(ctools_export_ui_plugin_base_path($this->plugin));
    }
    else {
      return $this->list_page($js, $input);
    }
  }

  /**
   * Page callback to display export information for an exportable item.
   */
  function export_page($js, $input, $item) {
    drupal_set_title($this->get_page_title('export', $item));
    return drupal_get_form('config_builder_export_ui_export_form', $item, t('Export'));
  }
}
