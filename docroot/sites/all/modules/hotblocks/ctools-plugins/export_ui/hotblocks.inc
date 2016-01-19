<?php

$plugin = array(
  'access' => 'administer hotblocks',
  'schema' => 'hotblocks_settings',
  'menu' => array(
    'menu prefix' => 'admin/structure',
    'menu item' => 'hotblocks',
    'menu title' => 'Hotblocks',
    'menu description' => 'Hotblocks menu description.',
  ),
  'title singular' => t('hotblock'),
  'title singular proper' => t('Hotblock'),
  'title plural' => t('hotblocks'),
  'title plural proper' => t('Hotblocks'),
  //'form' => array(
  //  'settings' => 'hotblocks_edit_form',
  //  'submit' => 'hotblocks_edit_form_submit',
  //),
  'handler' => array(
    'class' => 'hotblocks_export_ui',
    'parent' => 'ctools_export_ui',
  ),
);

/**
 * CTools export UI extending class. Slightly customized for Context.
 */
class hotblocks_export_ui extends ctools_export_ui {
  /**
   * Provide the actual editing form.
   */
  function edit_form(&$form, &$form_state) {
    parent::edit_form($form, $form_state);

    // Unlike ctools default, we allow machine name to be changed. We set type to machine_name because it does not seem
    // to protect against duplicate names or restrict characters.
    $form['info']['machine_name']['#type'] = 'machine_name';
    $form['info']['machine_name']['#disabled'] = FALSE;
    $form['info']['machine_name']['#machine_name']  = array(
      // function that return 1 if the machine name is duplicated.
      'exists' => 'hotblocks_check_machine_name_if_exist',
      'standalone' => TRUE,
    );

    $hotblock = $form_state['item'];
    $delta = isset($hotblock->machine_name) ? $hotblock->machine_name : '';
    $label = isset($hotblock->label) ? $hotblock->label : '';

    // Block label
    $form['label'] = array(
      '#title'         => t('Label'),
      '#type'          => 'textfield',
      '#description'   => 'The human readable label of this hotblock. This label will appear to the administrator of the' .
        ' block within its content area as well as on the administer blocks page.',
      '#default_value' => $label,
      '#required' => TRUE,
    );

  }

  /**
   * Handle the submission of the edit form.
   *
   * At this point, submission is successful. Our only responsibility is
   * to copy anything out of values onto the item that we are able to edit.
   *
   * If the keys all match up to the schema, this method will not need to be
   * overridden.
   */
  function edit_form_submit(&$form, &$form_state) {
    parent::edit_form_submit($form, $form_state);

    $current_machine_name = $form_state['item']->machine_name;
    $new_machine_name  = $form_state['input']['machine_name'];

    // If the machine name is changing, update hotblocks, hotblocks_weight, and hotblocks_settings table
    if($current_machine_name != trim($new_machine_name)) {
      $replacements = array(':newmname' => $new_machine_name, ':currentmname' => $current_machine_name);
      db_query("UPDATE {hotblocks_settings} SET machine_name = :newmname WHERE machine_name = :currentmname", $replacements);
      db_query("UPDATE {hotblocks} SET delta = :newmname WHERE delta = :currentmname", $replacements);
      db_query("UPDATE {hotblocks_weight} SET delta = :newmname WHERE delta = :currentmname", $replacements);
    }

    // Label update is handled by ctools automagically it seems.
    //db_query("UPDATE {hotblocks_settings} SET label = :label WHERE machine_name = :newmname", array(':newmname' => $new_machine_name, ':label' => $form_state['values']['label']));

    drupal_set_message('Hotblock saved.');
  }

  /**
   * Deletes exportable items from the database.
   */
  function delete_form_submit(&$form_state) {
    parent::delete_form_submit($form_state);

    // Delete all remnants of this block from the database
    $delta = $form_state['item']->machine_name;
    db_query("DELETE FROM {hotblocks} WHERE delta = :delta", array(':delta' => $delta));
    db_query("DELETE FROM {hotblocks_weight} WHERE delta = :delta", array(':delta' => $delta));
    db_query("DELETE FROM {hotblocks_settings} WHERE machine_name = :delta", array(':delta' => $delta));
  }

}
