<?php

class sheetnode_handler_field_editable extends views_handler_field {
  
  // TODO - Verify that the plugin style is a Sheetnode, otherwise complain.

  function render($values) {
    // no rendering
  }

  function query() {
    // no query
  }

  function views_form(&$form, &$form_state) {
    // Return a form with spreadsheet and a hidden element.
    $plugin = $this->view->style_plugin;
    $value = socialcalc_save($plugin->render_sheet());
    $form['sheetview'] = array();
    $id = 'edit-annotations';
    $plugin->render_inject($value, $id, $form['sheetview'], '#markup');
    $form['annotations'] = array(
      '#type' => 'hidden',
      '#attributes' => array('id' => $id),
      '#weight' => 99
    );
  }

  function views_form_submit($form, &$form_state) {
    // Save the annotated spreadsheet to database.
    $sheetnode_view = (object)array(
      'view_name' => $this->view->name,
      'display_id' => $this->view->display_handler->display->id,
      'value' => $form_state['values']['annotations'],
    );
    // Check for existing value.
    $id = db_query("SELECT id FROM {sheetnode_view} WHERE view_name = :name AND display_id = :id", array(
      ':name' => $sheetnode_view->view_name,
      ':id' => $sheetnode_view->display_id,
    ))->fetchField();
    if ($id) {
      $sheetnode_view->id = $id;
      drupal_write_record('sheetnode_view', $sheetnode_view, 'id');
    }
    else {
      // TODO - For some reason this fails to write the view_name.
      drupal_write_record('sheetnode_view', $sheetnode_view);
    }
  }
}

