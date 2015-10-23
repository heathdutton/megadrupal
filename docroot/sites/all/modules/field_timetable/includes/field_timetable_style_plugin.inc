<?php
/**
 * @file
 * Display style plugin for Views that renders rows as a timetable.
 */

/**
 * Style plugin to display a timetable.
 */
class field_timetable_style_plugin extends views_plugin_style {

  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['editable'] = FALSE;
    $options['selectable'] = FALSE;
    $options['selectHelper'] = FALSE;
    $options['mintime'] = 9;
    $options['maxtime'] = 18;
    $options['slotminutes'] = 60;
    $options['color'] = '#000000';
    $options['textcolor'] = '#FFFFFF';

    return $options;
  }

  // Create the options form.
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $timetable_fields = array();
    $title_fields = array();
    $color_field = array();

    // Default color settings.
    $color_field['random'] = t('Random');
    $color_field['manual'] = t('Manual');

    // Default title settings.
    $title_fields['description'] = t("Description (use timetable field's description)");

    // Build a list of all fields.
    foreach ($this->display->handler->get_handlers('field') as $field => $handler) {

      $title_fields[$field] .= ($handler->label() != '') ? $field. ' (' . $handler->label() . ')' : $field;
      $color_field[$field] .= ($handler->label() != '') ? $field. ' (' . $handler->label() . ')' : $field;
      
      // search for timetable field.
      if ($handler->field_info['type'] == 'field_timetable_timetable') {
        $timetable_fields[$field] = $handler->label();
      }

    }

    if (empty($title_fields)) {
      $title_fields[''] = t('No available fields');
    }

    if (empty($timetable_fields)) {
      $options[''] = t('No timetable fields');
    }

    // Title field.
    $form['settings']['timetable_field'] = array(
      '#type' => 'select',
      '#title' => t('Timetable Field'),
      '#description' => t('Select one or more timetable fields.'),
      '#options' => $timetable_fields,
      '#default_value' => $this->options['timetable_field'],
      '#multiple' => TRUE,
      '#required' => TRUE,
       '#parents' => array(
         'style_options',
         'timetable_field',
      ),
    );

    // Title field.
    $form['settings']['title_field'] = array(
      '#type' => 'select',
      '#title' => t('Title Field'),
      '#description' => t('Choose the field(s) with the custom title.'),
      '#options' => $title_fields,
      '#default_value' => $this->options['title_field'],
      '#multiple' => TRUE,
      '#parents' => array(
        'style_options',
        'title_field',
      ),
    );

    // Color field.
    $form['settings']['color_field'] = array(
      '#type' => 'select',
      '#title' => t('Color Field'),
      '#options' => $color_field,
      '#default_value' => $this->options['color_field'],
      '#parents' => array(
        'style_options',
        'color_field',
      ),
    );

    // Manual Color field.
    $form['settings']['color_field_manual'] = array(
      '#type' => 'textfield',
      '#title' => t('Manual Color'),
      '#description' => t('Format #FFFFFF'),
      '#default_value' => $this->options['color_field_manual'],
      '#parents' => array(
        'style_options',
        'color_field_manual',
      ),
      '#states' => array(
        'visible' => array(
          ':input[id="edit-style-options-color-field"]' => array('value' => 'manual'),
        ),
      ),
    );

    $options = array();
    for ($i = 0; $i < 24; $i++){
      $options[$i] = $i . ':00';
    }
    $form['settings']['mintime'] = array(
      '#type' => 'select',
      '#title' => t('Min Time'),
      '#options' => $options,
      '#default_value' => $this->options['mintime'],
      '#parents' => array(
        'style_options',
        'mintime',
      ),
    );

    $options = array();
    for ($i = 1; $i <= 24; $i++){
      $options[$i] = $i . ':00';
    }
    $form['settings']['maxtime'] = array(
      '#type' => 'select',
      '#title' => t('Max Time'),
      '#options' => $options,
      '#default_value' => $this->options['maxtime'],
      '#parents' => array(
        'style_options',
        'maxtime',
      ),
    );

    $options = array(60 => t('60 min'), 30 => t('30 min'), 15 => t('15 min'), 5 => t('5 min'), 1 => t('1 min'));
    $form['settings']['slotminutes'] = array(
      '#type' => 'select',
      '#title' => t('Slot Minutes'),
      '#options' => $options,
      '#default_value' => $this->options['slotminutes'],
      '#parents' => array(
        'style_options',
        'slotminutes',
      ),
    );    

  }

  // Override the render functionality.
  function render() {

    $title_fields = $this->options['title_field'];
    $timetable_fields = $this->options['timetable_field'];
    $color_field = $this->options['color_field'];
    $color_field_manual = $this->options['color_field_manual'];

    if (empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }

    $rows = array();
    $random_color = field_timetable_random_color();

    // for each result
    foreach ($this->view->result as $row_index => $row) {

      // Build title.
      $titles = array();
      foreach ($title_fields as $title_field) {
        if ($title_field == 'title' || $title_field == 'title_1') {
          $title_field = 'node_title';
          $temp = $row->$title_field;
          $titles[] = $temp;
        } 
        elseif ($title_field == 'description') {
          // Nothing to do.
        } 
        else {
          $title_field = 'field_' . $title_field;
          $temp = $row->$title_field;
          $titles[] = $temp[0]['rendered']['#markup'];
        }
      }
      if (empty($titles)) {
        $title = '';
      } 
      else {
        $title = implode(', ',$titles);
      }

      // Build color.
      switch ($color_field) {
        case 'manual':
          $color = $color_field_manual;
          break;
        case 'random':
          $color = $random_color;
          break;
        default:
          $color = 'field_' . $color_field;
          $color = $row->$color;
          $color = $color[0]['rendered']['#markup'];
      }

      // for each timetable field.
      $node = $row->_field_data['nid']['entity'];
      foreach ($timetable_fields as $timetable_field) {
        $tmp = $node->$timetable_field;
        $row = array();
        $events = $tmp[LANGUAGE_NONE];
        $row['title'] = $title;
        $row['color'] = $color;
        $row['events'] = $events;
        $rows[] = $row;
      }

    }

    $output = theme('field_timetable_view',
      array(
        'view' => $this->view,
        'options' => $this->options,
        'rows' => $rows,
      )
    );

    unset($this->view->row_index);

    return $output;

  }

}
