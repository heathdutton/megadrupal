<?php
/**
 * @file
 *
 * Calendars table.
 */

/**
 * Menu loader callback. Load a Spider Calendar settings.
 */
function spider_calendar_calendars() {
  drupal_set_message(t('Themes are disabled for the non-commercial version.'), 'warning', FALSE);
  $form['fieldset_cal_buttons'] = array(
    '#prefix' => '<a href="http://web-dorado.com/files/fromSpiderCalendarDrupal.php" target="_blank" style="color:red; text-decoration:none; float:right;">
                    <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/header.png" border="0" alt="www.web-dorado.com" width="215"><br />
                  <div style="float:right;">' . t('Get the full version') . '&nbsp;&nbsp;&nbsp;&nbsp;</div>
                  </a><br />',
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('fieldset')),
  );
  $form['fieldset_cal_buttons']['publish_calendar'] = array(
    '#type' => 'submit',
    '#submit' => array('spider_calendar_publish_calendars'),
    '#value' => t('Publish'),
  );
  $form['fieldset_cal_buttons']['unpublish_calendar'] = array(
    '#type' => 'submit',
    '#submit' => array('spider_calendar_unpublish_calendars'),
    '#value' => t('Unpublish'),
  );
  $form['fieldset_cal_buttons']['delete_calendar'] = array(
    '#type' => 'submit',
    '#submit' => array('spider_calendar_delete_calendar'),
    '#value' => t('Delete'),
  );
  $form['fieldset_cal_buttons']['new_calendar'] = array(
    '#prefix' => '<a href="' . url('admin/settings/spider_calendar/Calendars/add', array('absolute' => TRUE)) . '">' . t('New') . '</a>',
  );
  $form['fieldset_block_number'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('fieldset')),
  );
  $form['fieldset_block_number']['block_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of calendar blocks'),
    '#default_value' => db_query("SELECT block_number FROM {spider_calendar_block_number_table} WHERE id=:id", array(':id' => 1))->fetchField(),
    '#size' => 25,
  );
  $form['fieldset_block_number']['save_block_number'] = array(
    '#type' => 'submit',
    '#submit' => array('spider_calendar_save_blocks_number'),
    '#value' => t('Save Blocks number'),
  );
  $form['fieldset_search_cal_title'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('fieldset')),
  );
  $form['fieldset_search_cal_title']['search_cal_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Search calendars by title'),
    '#size' => 25,
    '#default_value' => variable_get('search_cal_title', ''),
  );
  $form['fieldset_search_cal_title']['search_cal_button'] = array(
    '#type' => 'submit',
    '#submit' => array('spider_calendar_cal_seach'),
    '#value' => t('Go'),
  );
  $form['fieldset_search_cal_title']['reset_cal_button'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('spider_calendar_cal_reset'),
    '#suffix' => '<a href="http://web-dorado.com/drupal-event-calendar-guide-step-2.html" target="_blank" style="float:right;"><img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/user-manual.png" border="0" alt="' . t('User Manual') . '"></a>',
  );
  $form['fieldset_calendars'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('fieldset')),
    '#title' => t('Calendars'),
  );
  drupal_add_js(drupal_get_path('module', 'spider_calendar') . '/js/spider_calendar_check_all.js');
  $id_col = db_query("SELECT id FROM {spider_calendar_calendar_table}")->fetchCol();
  $id_col_string = implode($id_col, ',');
  $form['fieldset_calendars']['calendars_table'] = array(    
    '#prefix' => '<table>
                   <tr>
                    <th>#</th>
                    <th>',
    '#type' => 'checkbox',
    '#attributes' => array('onclick' => 'spider_calendar_check_all("edit-calendars-table", "' . $id_col_string . '", "edit-check")'),
    '#suffix' => '</th>
                    <th>' . l(t('ID') . ((isset($_GET['sort_by']) && ($_GET['sort_by'] == 'id'))? ((isset($_GET['asc_or_desc']) && ($_GET['asc_or_desc'] == 1)) ? ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/asc.png">' : ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/desc.png">') : ''), url('admin/settings/spider_calendar/Calendars', array('query' => array('sort_by' => 'id', 'asc_or_desc' => (isset($_GET['asc_or_desc']) ? !$_GET['asc_or_desc'] : 1)), 'absolute' => TRUE)), array('html' => TRUE)) . '</th>
                    <th width="500">' . l(t('Title') . ((isset($_GET['sort_by']) && ($_GET['sort_by'] == 'title'))? ((isset($_GET['asc_or_desc']) && ($_GET['asc_or_desc'] == 1)) ? ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/asc.png">' : ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/desc.png">') : ''), url('admin/settings/spider_calendar/Calendars', array('query' => array('sort_by' => 'title', 'asc_or_desc' => (isset($_GET['asc_or_desc']) ? !$_GET['asc_or_desc'] : 1)), 'absolute' => TRUE)), array('html' => TRUE)) . '</th>
                    <th>' . t('Manage Events') . '</th>
                    <th>' . t('Published') . '</th>
                    <th>' . t('Delete') . '</th>
                   </tr>',
  );
  $number = 1;
  // Sort by column.
  if (isset($_GET['sort_by'])) {
    // Ascending/Descending by title.
    if (check_plain($_GET['sort_by']) == 'title') {
      if (isset($_GET['asc_or_desc']) && (check_plain($_GET['asc_or_desc']) == 1)) {
        $asc_or_desc = 'ASC';
        $_GET['asc_or_desc'] = 0;
      }
      else {
        $asc_or_desc = 'DESC';
        $_GET['asc_or_desc'] = 1;
      }
      $id_cols = db_query("SELECT id FROM {spider_calendar_calendar_table} WHERE title like :title ORDER BY title " . $asc_or_desc, array(':title' => '%' . variable_get('search_cal_title', '') . '%'))->fetchCol();
    }
    // Ascending/Descending by ID.
    elseif (check_plain($_GET['sort_by']) == 'id') {
      if (isset($_GET['asc_or_desc']) && (check_plain($_GET['asc_or_desc']) == 1)) {
        $asc_or_desc = 'ASC';
        $_GET['asc_or_desc'] = 0;
      }
      else {
        $asc_or_desc = 'DESC';
        $_GET['asc_or_desc'] = 1;
      }
      $id_cols = db_query("SELECT id FROM {spider_calendar_calendar_table} WHERE title like :title ORDER BY id " . $asc_or_desc, array(':title' => '%' . variable_get('search_cal_title', '') . '%'))->fetchCol();
    }
  }
  else {
    $id_cols = db_query("SELECT id FROM {spider_calendar_calendar_table} WHERE title like :title", array(':title' => '%' . variable_get('search_cal_title', '') . '%'))->fetchCol();
  }
  //variable_set('search_cal_title', '');
  foreach ($id_cols as $id_col) {
    $row = db_query("SELECT * FROM {spider_calendar_calendar_table} WHERE id=:id", array(':id' => $id_col))->fetchObject();
    $id = $row->id;
    $title = $row->title;
    $published = $row->published;
    $form['fieldset_calendars']['check' . $id] = array(
      '#prefix' => '<tr  style="border-bottom:1px solid #cccccc;">
                     <td>' . $number++ . '</td>
                     <td>',
      '#type' => 'checkbox',
      '#suffix' => '</td>
                    <td>' . $id . '</td>
                    <td>
                      <a href="' . url('admin/settings/spider_calendar/Calendars/edit', array('query' => array('id' => $id), 'absolute' => TRUE)) . '"> ' . $title . '</a>
                    </td>
                    <td>
                      <a href="' . url('admin/settings/spider_calendar/Calendars/events', array('query' => array('calendar' => $id), 'absolute' => TRUE)) . '">' . t('Manage Events') . '</a></th>
                    </td>',
    );
    if ($published) {
      $form['fieldset_calendars']['publish' . $id] = array(
        '#prefix' => '<td>',
        '#type' => 'image_button',
        '#src' => drupal_get_path('module', 'spider_calendar') . '/images/publish.png',
        '#submit' => array('spider_calendar_unpublish_calendars'),
        '#suffix' => '</td>',
      );
    }
    else {
      $form['fieldset_calendars']['publish' . $id] = array(
        '#prefix' => '<td>',
        '#type' => 'image_button',
        '#src' => drupal_get_path('module', 'spider_calendar') . '/images/unpublish.png',
        '#submit' => array('spider_calendar_publish_calendars'),
        '#suffix' => '</td>',
      );
    }
    $form['fieldset_calendars']['delete' . $id] = array(
        '#prefix' => '<td>',
        '#type' => 'image_button',
        '#src' => drupal_get_path('module', 'spider_calendar') . '/images/delete_row.png',
        '#submit' => array('spider_calendar_delete_calendar'),
        '#attributes' => array('onclick' => 'if (!confirm(Drupal.t("Do you want to delete?"))) {return false;}'),
        '#suffix' => '</td>',
      );
    $form['fieldset_calendars']['row_end'] = array(
      '#prefix' => '</tr>',
    );
  }
  $form['fieldset_calendars']['table_end'] = array(
    '#prefix' => '</table>',
  );
  return $form;
}

/**
 * Search in calendars.
 */
function spider_calendar_cal_seach($form, &$form_state) {
  if ($form_state['values']['search_cal_title'] != '') {
    variable_set('search_cal_title', $form_state['values']['search_cal_title']);
  }
  else {
    variable_set('search_cal_title', '');
  }
  $form_state['redirect'] = url('admin/settings/spider_calendar', array('absolute' => TRUE));
}

/**
 * Reset calendars.
 */
function spider_calendar_cal_reset($form, &$form_state) {
  variable_set('search_cal_title', '');
  $form_state['redirect'] = url('admin/settings/spider_calendar', array('absolute' => TRUE));
}

/**
 * Publish selected calendars.
 */
function spider_calendar_publish_calendars($form, &$form_state) {
  $id_col = db_query("SELECT id FROM {spider_calendar_calendar_table}")->fetchCol();
  foreach ($id_col as $id) {
    if ((isset($form_state['values']['check' . $id]) && $form_state['values']['check' . $id]) || (isset($form_state['values']['publish' . $id]) && $form_state['values']['publish' . $id])) {
      db_query("UPDATE {spider_calendar_calendar_table} SET published=:published WHERE id=:id", array(':published' => 1, ':id' => $id));
    }
  }
}

/**
 * Unpublish selected calendars.
 */
function spider_calendar_unpublish_calendars($form, &$form_state) {
  $id_col = db_query("SELECT id FROM {spider_calendar_calendar_table}")->fetchCol();
  foreach ($id_col as $id) {
    if ((isset($form_state['values']['check' . $id]) && $form_state['values']['check' . $id]) || (isset($form_state['values']['publish' . $id]) && $form_state['values']['publish' . $id])) {
      db_query("UPDATE {spider_calendar_calendar_table} SET published=:published WHERE id=:id", array(':published' => 0, ':id' => $id));
    }
  }
}

/**
 * Delete selected calendars.
 */
function spider_calendar_delete_calendar($form, &$form_state) {
  $id_col = db_query("SELECT id FROM {spider_calendar_calendar_table}")->fetchCol();
  foreach ($id_col as $id) {
    if (isset($form_state['values']['check' . $id]) && $form_state['values']['check' . $id] || (isset($form_state['values']['delete' . $id]) && $form_state['values']['delete' . $id])) {
      db_query("DELETE FROM {spider_calendar_calendar_table} WHERE id = :id", array(':id' => $id));
      db_query("DELETE FROM {spider_calendar_events_table} WHERE calendar = :calendar", array(':calendar' => $id));
      drupal_set_message(t('Selected calendars successfully deleted.'), 'status', FALSE);
    }
  }
}

/**
 * Save blocks number.
 */
function spider_calendar_save_blocks_number($form, &$form_state) {
  if (!is_numeric($form_state['values']['block_number'])) {
    drupal_set_message(t('Blocks number must be numeric.'), 'error', FALSE);
  }
  else {
    db_query("UPDATE {spider_calendar_block_number_table} SET block_number=:block_number WHERE id=:id", array(':block_number' => $form_state['values']['block_number'], ':id' => 1));
    drupal_set_message(t('Blocks number successfully saved.'), 'status', FALSE);
  }
}

/**
 * Add a new calendar.
 */
function spider_calendar_add_calendar() {
  $form = array();
  $form['cal_title'] = array(
    '#prefix' => '<p style="font-size:20px; font-weight:bold;">' . t('Create a new calendar') . '</p>',
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#size' => 20,
  );
  $form['cal_published'] = array(
    '#type' => 'radios',
    '#title' => t('Published'),
    '#default_value' => 1,
    '#options' => array('1' => t('Yes'), '0' => t('No')),
  );
  $form['cal_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('spider_calendar_cal_save'),
  );
  $form['cal_apply'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('spider_calendar_cal_apply'),
  );
  $form['cal_cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#attributes' => array('onclick' => 'document.getElementById("edit-cal-title").setAttribute("style", "color:rgba(255, 0, 0, 0)");document.getElementById("edit-cal-title").setAttribute("value", "cancel");'),
    '#submit' => array('spider_calendar_cal_new_cancel'),
  );
  return $form;
}

/**
 * Save a new calendar.
 */
function spider_calendar_cal_save($form, &$form_state) {
  db_insert('spider_calendar_calendar_table')
    ->fields(array(
      'title' => $form_state['values']['cal_title'],
      'published' => $form_state['values']['cal_published'],
      ))
    ->execute();
  $form_state['redirect'] = url('admin/settings/spider_calendar', array('absolute' => TRUE));
  drupal_set_message(t('Your calendar successfully saved.'), 'status', FALSE);
}

/**
 * Apply a new calendar.
 */
function spider_calendar_cal_apply($form, &$form_state) {
  db_insert('spider_calendar_calendar_table')
    ->fields(array(
      'title' => $form_state['values']['cal_title'],
      'published' => $form_state['values']['cal_published'],
      ))
    ->execute();
  $id = db_query("SELECT MAX(id) FROM {spider_calendar_calendar_table}")->fetchField();
  $form_state['redirect'] = url('admin/settings/spider_calendar/Calendars/edit', array('query' => array('id' => $id), 'absolute' => TRUE));
  drupal_set_message(t('Your calendar successfully saved.'), 'status', FALSE);
}

/**
 * Cancel a new calendar save.
 */
function spider_calendar_cal_new_cancel($form, &$form_state) { 
  $form_state['redirect'] = url('admin/settings/spider_calendar', array('absolute' => TRUE));
}

/**
 * Edit calendar.
 */
function spider_calendar_edit_calendar() {
  $form = array();
  if (isset($_GET['id'])) {
    $id = check_plain($_GET['id']);
  }
  $cal = db_query("SELECT * FROM {spider_calendar_calendar_table} WHERE id=:id", array(':id' => $id))->fetchObject();
  $form['cal_title'] = array(
    '#prefix' => '<p style="font-size:20px; font-weight:bold;">' . t('Edit calendar') . ' ' . $cal->title . '</p>',
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#default_value' => $cal->title,
    '#size' => 20,
  );
  $form['cal_published'] = array(
    '#type' => 'radios',
    '#title' => t('Published'),
    '#default_value' => $cal->published,
    '#options' => array('1' => t('Yes'), '0' => t('No')),
  );
  $form['cal_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('spider_calendar_cal_update'),
  );
  $form['cal_apply'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('spider_calendar_cal_update_apply'),
  );
  $form['cal_cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('spider_calendar_cal_update_cancel'),
  );
  return $form;
}

/**
 * Update the calendar.
 */
function spider_calendar_cal_update($form, &$form_state) {
  if (isset($_GET['id'])) {
    $id = check_plain($_GET['id']);
  }
  db_query("UPDATE {spider_calendar_calendar_table} SET title=:title,published=:published WHERE id=:id", array(
    ':title' => $form_state['values']['cal_title'],
    ':published' => $form_state['values']['cal_published'],
    ':id' => $id,
    ));
  $form_state['redirect'] = url('admin/settings/spider_calendar', array('absolute' => TRUE));
  drupal_set_message(t('Your calendar successfully updated.'), 'status', FALSE);
}

/**
 * Apply updated calendar.
 */
function spider_calendar_cal_update_apply($form, &$form_state) {
  if (isset($_GET['id'])) {
    $id = check_plain($_GET['id']);
  }
  db_query("UPDATE {spider_calendar_calendar_table} SET title=:title,published=:published WHERE id=:id", array(
    ':title' => $form_state['values']['cal_title'],
    ':published' => $form_state['values']['cal_published'],
    ':id' => $id,
    ));
  $form_state['redirect'] = url('admin/settings/spider_calendar/Calendars/edit', array('query' => array('id' => $id), 'absolute' => TRUE));
  drupal_set_message(t('Your calendar successfully updated.'), 'status', FALSE);
}

/**
 * Cancel the calendar save.
 */
function spider_calendar_cal_update_cancel($form, &$form_state) {
  $form_state['redirect'] = url('admin/settings/spider_calendar', array('absolute' => TRUE));
}