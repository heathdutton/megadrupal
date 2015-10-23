<?php
/**
 * @file
 *
 * Calendars events managment.
 */

/**
 * Calendar event manage page.
 */
function spider_calendar_manage_events() {
  $form = array();
  if (isset($_GET['calendar'])) {
    $calendar_id = check_plain($_GET['calendar']);
  }
  $cal_name = db_query("SELECT title FROM {spider_calendar_calendar_table} WHERE id=:id", array(':id' => $calendar_id))->fetchField();
  $form['fieldset_event_buttons'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('fieldset')),
  );
  $form['fieldset_event_buttons']['publish_event'] = array(
    '#type' => 'submit',
    '#submit' => array('spider_calendar_publish_events'),
    '#value' => t('Publish'),
  );
  $form['fieldset_event_buttons']['unpublish_event'] = array(
    '#type' => 'submit',
    '#submit' => array('spider_calendar_unpublish_events'),
    '#value' => t('Unpublish'),
  );
  $form['fieldset_event_buttons']['delete_event'] = array(
    '#type' => 'submit',
    '#submit' => array('spider_calendar_delete_events'),
    '#value' => t('Delete'),
  );
  $form['fieldset_event_buttons']['event_table_cancel'] = array(
    '#type' => 'submit',
    '#submit' => array('spider_calendar_manage_events_cancel'),
    '#value' => t('Back to calendars'),
  );
  $form['fieldset_event_buttons']['new_event'] = array(
    '#prefix' => ' <a href="' . url('admin/settings/spider_calendar/Calendars/events/add', array('query' => array('calendar' => $calendar_id), 'absolute' => TRUE)) . '">' . t('New') . '</a>',
  );
  $form['fieldset_search_event_date'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('fieldset')),
  );
  $form['fieldset_search_event_date']['search_event_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Search events by title'),
    '#size' => 25,
    '#default_value' => variable_get('search_event_title', ''),
  );
  $form['fieldset_search_event_date']['search_event_date_button'] = array(
    '#type' => 'submit',
    '#submit' => array('spider_calendar_event_seach'),
    '#value' => t('Go'),
  );
  $form['fieldset_search_event_date']['reset_event_date_button'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('spider_calendar_event_reset'),
  );
  $form['fieldset_events'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('fieldset')),
    '#title' => $cal_name . ' ' . t('Calendar Events'),
  );
  drupal_add_js(drupal_get_path('module', 'spider_calendar') . '/js/spider_calendar_check_all.js');
  $id_col = db_query("SELECT id FROM {spider_calendar_events_table} WHERE calendar=:calendar", array(':calendar' => $calendar_id))->fetchCol();
  $id_col_string = implode($id_col, ',');
  $form['fieldset_events']['events_table'] = array(
    '#prefix' => '<table>
                   <tr>
                    <th>#</th>
                    <th>',
    '#type' => 'checkbox',
    '#attributes' => array('onclick' => 'spider_calendar_check_all("edit-events-table", "' . $id_col_string . '", "edit-check-event")'),
    '#suffix' => '  </th>
                    <th width="30">' . l(t('ID') . ((isset($_GET['sort_by']) && ($_GET['sort_by'] == 'id'))? ((isset($_GET['asc_or_desc']) && ($_GET['asc_or_desc'] == 1)) ? ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/asc.png">' : ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/desc.png">') : ''), url('admin/settings/spider_calendar/Calendars/events', array('query' => array('calendar' => $calendar_id, 'sort_by' => 'id', 'asc_or_desc' => (isset($_GET['asc_or_desc']) ? !$_GET['asc_or_desc'] : 1)), 'absolute' => TRUE)), array('html' => TRUE)) . '</th>
                    <th width="400">' . l(t('Title') . ((isset($_GET['sort_by']) && ($_GET['sort_by'] == 'title'))? ((isset($_GET['asc_or_desc']) && ($_GET['asc_or_desc'] == 1)) ? ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/asc.png">' : ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/desc.png">') : ''), url('admin/settings/spider_calendar/Calendars/events', array('query' => array('calendar' => $calendar_id, 'sort_by' => 'title', 'asc_or_desc' => (isset($_GET['asc_or_desc']) ? !$_GET['asc_or_desc'] : 1)), 'absolute' => TRUE)), array('html' => TRUE)) . '</th>
                    <th width="200">' . l(t('Date') . ((isset($_GET['sort_by']) && ($_GET['sort_by'] == 'date'))? ((isset($_GET['asc_or_desc']) && ($_GET['asc_or_desc'] == 1)) ? ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/asc.png">' : ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/desc.png">') : ''), url('admin/settings/spider_calendar/Calendars/events', array('query' => array('calendar' => $calendar_id, 'sort_by' => 'date', 'asc_or_desc' => (isset($_GET['asc_or_desc']) ? !$_GET['asc_or_desc'] : 1)), 'absolute' => TRUE)), array('html' => TRUE)) . '</th>
                    <th width="150">' . l(t('Time') . ((isset($_GET['sort_by']) && ($_GET['sort_by'] == 'time'))? ((isset($_GET['asc_or_desc']) && ($_GET['asc_or_desc'] == 1)) ? ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/asc.png">' : ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/desc.png">') : ''), url('admin/settings/spider_calendar/Calendars/events', array('query' => array('calendar' => $calendar_id, 'sort_by' => 'time', 'asc_or_desc' => (isset($_GET['asc_or_desc']) ? !$_GET['asc_or_desc'] : 1)), 'absolute' => TRUE)), array('html' => TRUE)) . '</th>
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
      $events_id_col = db_query("SELECT id FROM {spider_calendar_events_table} WHERE title like :title and calendar=:calendar ORDER BY title " . $asc_or_desc, array(':title' => '%' . variable_get('search_event_title', '') . '%', ':calendar' => $calendar_id))->fetchCol();
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
      $events_id_col = db_query("SELECT id FROM {spider_calendar_events_table} WHERE title like :title and calendar=:calendar ORDER BY id " . $asc_or_desc, array(':title' => '%' . variable_get('search_event_title', '') . '%', ':calendar' => $calendar_id))->fetchCol();
    }
    // Ascending/Descending by date.
    elseif (check_plain($_GET['sort_by']) == 'date') {
      if (isset($_GET['asc_or_desc']) && (check_plain($_GET['asc_or_desc']) == 1)) {
        $asc_or_desc = 'ASC';
        $_GET['asc_or_desc'] = 0;
      }
      else {
        $asc_or_desc = 'DESC';
        $_GET['asc_or_desc'] = 1;
      }
      $events_id_col = db_query("SELECT id FROM {spider_calendar_events_table} WHERE title like :title and calendar=:calendar ORDER BY event_date " . $asc_or_desc, array(':title' => '%' . variable_get('search_event_title', '') . '%', ':calendar' => $calendar_id))->fetchCol();
    }
    // Ascending/Descending by time.
    elseif (check_plain($_GET['sort_by']) == 'time') {
      if (isset($_GET['asc_or_desc']) && (check_plain($_GET['asc_or_desc']) == 1)) {
        $asc_or_desc = 'ASC';
        $_GET['asc_or_desc'] = 0;
      }
      else {
        $asc_or_desc = 'DESC';
        $_GET['asc_or_desc'] = 1;
      }
      $events_id_col = db_query("SELECT id FROM {spider_calendar_events_table} WHERE title like :title and calendar=:calendar ORDER BY event_time " . $asc_or_desc, array(':title' => '%' . variable_get('search_event_title', '') . '%', ':calendar' => $calendar_id))->fetchCol();
    }
  }
  else {
    $events_id_col = db_query("SELECT id FROM {spider_calendar_events_table} WHERE title like :title and calendar=:calendar", array(':title' => '%' . variable_get('search_event_title', '') . '%', ':calendar' => $calendar_id))->fetchCol();
  }
  foreach ($events_id_col as $event_id_col) {
    $event = db_query("SELECT * FROM {spider_calendar_events_table} WHERE id=:id", array(':id' => $event_id_col))->fetchObject();
    $event_id = $event->id;
    $event_title = $event->title;
    $event_published = $event->published;
    $event_date = $event->event_date;
    if ($event->event_date_end != '') {
      $event_date_end = ' - ' . $event->event_date_end;
    }
    else {
      $event_date_end = '';
    }
    $event_time = $event->event_time;
    $form['fieldset_events']['check_event' . $event_id] = array(
      '#prefix' => '<tr  style="border-bottom:1px solid #cccccc;">
                     <td>' . $number++ . '</td>
                     <td>',
      '#type' => 'checkbox',
      '#suffix' => '</td>
                    <td>' . $event_id . '</td>
                    <td>
                      <a href="' . url('admin/settings/spider_calendar/Calendars/events/add', array('query' => array('calendar' => $calendar_id, 'event_id' => $event_id), 'absolute' => TRUE)) . '"> ' . $event_title . '</a>
                    </td>
                    <td>' . $event_date . $event_date_end . '</td>
                    <td>' . $event_time . '</td>',
    );
    if ($event_published) {
      $form['fieldset_events']['publish_event' . $event_id] = array(
        '#prefix' => '<td>',
        '#type' => 'image_button',
        '#src' => drupal_get_path('module', 'spider_calendar') . '/images/publish.png',
        '#submit' => array('spider_calendar_unpublish_events'),
        '#suffix' => '</td>',
      );
    }
    else {
      $form['fieldset_events']['publish_event' . $event_id] = array(
        '#prefix' => '<td>',
        '#type' => 'image_button',
        '#src' => drupal_get_path('module', 'spider_calendar') . '/images/unpublish.png',
        '#submit' => array('spider_calendar_publish_events'),
        '#suffix' => '</td>',
      );
    }
    $form['fieldset_events']['delete_event' . $event_id] = array(
        '#prefix' => '<td>',
        '#type' => 'image_button',
        '#src' => drupal_get_path('module', 'spider_calendar') . '/images/delete_row.png',
        '#submit' => array('spider_calendar_delete_events'),
        '#attributes' => array('onclick' => 'if (!confirm(Drupal.t("Do you really want to delete?"))) {return false;}'),
        '#suffix' => '</td>',
      );
    $form['fieldset_events']['event_row_end'] = array(
      '#prefix' => '</tr>',
    );
  }
  $form['fieldset_events']['events_table_end'] = array(
    '#prefix' => '</table>',
  );
  return $form;
}

/**
 * Search in events.
 */
function spider_calendar_event_seach($form, &$form_state) {
  if ($form_state['values']['search_event_title'] != '') {
    variable_set('search_event_title', $form_state['values']['search_event_title']);
  }
  else {
    variable_set('search_event_title', '');
  }
  $form_state['redirect'] = url('admin/settings/spider_calendar/Calendars/events', array('query' => array('calendar' => $_GET['calendar']), 'absolute' => TRUE));
}

/**
 * Reset events.
 */
function spider_calendar_event_reset($form, &$form_state) {
  variable_set('search_event_title', '');
  $form_state['redirect'] = url('admin/settings/spider_calendar/Calendars/events', array('query' => array('calendar' => $_GET['calendar']), 'absolute' => TRUE));
}

/**
 * Publish selected events.
 */
function spider_calendar_publish_events($form, &$form_state) {
  $events_id_col = db_query("SELECT id FROM {spider_calendar_events_table}")->fetchCol();
  foreach ($events_id_col as $id) {
    if ((isset($form_state['values']['check_event' . $id]) && $form_state['values']['check_event' . $id]) || (isset($form_state['values']['publish_event' . $id]) && $form_state['values']['publish_event' . $id])) {
      db_query("UPDATE {spider_calendar_events_table} SET published=:published WHERE id=:id", array(':published' => 1, ':id' => $id));
    }
  }
  if (isset($_GET['calendar'])) {
    $calendar_id = check_plain($_GET['calendar']);
    $form_state['redirect'] = url('admin/settings/spider_calendar/Calendars/events', array('query' => array('calendar' => $calendar_id), 'absolute' => TRUE));
  }
}

/**
 * Unpublish selected events.
 */
function spider_calendar_unpublish_events($form, &$form_state) {
  $events_id_col = db_query("SELECT id FROM {spider_calendar_events_table}")->fetchCol();
  foreach ($events_id_col as $id) {
    if ((isset($form_state['values']['check_event' . $id]) && $form_state['values']['check_event' . $id]) || (isset($form_state['values']['publish_event' . $id]) && $form_state['values']['publish_event' . $id])) {
      db_query("UPDATE {spider_calendar_events_table} SET published=:published WHERE id=:id", array(':published' => 0, ':id' => $id));
    }
  }
  if (isset($_GET['calendar'])) {
    $calendar_id = check_plain($_GET['calendar']);
    $form_state['redirect'] = url('admin/settings/spider_calendar/Calendars/events', array('query' => array('calendar' => $calendar_id), 'absolute' => TRUE));
  }
}

/**
 * Delete selected events.
 */
function spider_calendar_delete_events($form, &$form_state) {
  $events_id_col = db_query("SELECT id FROM {spider_calendar_events_table}")->fetchCol();
  foreach ($events_id_col as $id) {
    if (isset($form_state['values']['check_event' . $id]) && $form_state['values']['check_event' . $id] || (isset($form_state['values']['delete_event' . $id]) && $form_state['values']['delete_event' . $id])) {
      db_query("DELETE FROM {spider_calendar_events_table} WHERE id=:id", array(':id' => $id));
    }
  }
  if (isset($_GET['calendar'])) {
    $calendar_id = check_plain($_GET['calendar']);
    $form_state['redirect'] = url('admin/settings/spider_calendar/Calendars/events', array('query' => array('calendar' => $calendar_id), 'absolute' => TRUE));
    drupal_set_message(t('Selected calendars successfully deleted.'), 'status', FALSE);
  }
}

/**
 * Cancel Manage events.
 */
function spider_calendar_manage_events_cancel($form, &$form_state) {
  $form_state['redirect'] = url('admin/settings/spider_calendar', array('absolute' => TRUE));
}

/**
 * Add a new event.
 */
function spider_calendar_add_events() {
  drupal_add_js(drupal_get_path('module', 'spider_calendar') . '/js/spider_calendar_check_hour.js');
  drupal_add_js(drupal_get_path('module', 'spider_calendar') . '/js/spider_calendar_event_repeat.js');
  drupal_add_js(drupal_get_path('module', 'spider_calendar') . '/js/spider_calendar_cal_function.js');
  drupal_add_js(drupal_get_path('module', 'spider_calendar') . '/js/spider_calendar_calendar.js');
  drupal_add_js(drupal_get_path('module', 'spider_calendar') . '/js/spider_calendar_calendar-setup.js');
  drupal_add_css(drupal_get_path('module', 'spider_calendar') . '/css/spider_calendar_calendar-jos.css');
  $cal_name = db_query("SELECT title FROM {spider_calendar_calendar_table} WHERE id=:id", array(':id' => check_plain($_GET['calendar'])))->fetchField();
  if (isset($_GET['event_id'])) {
    $event_id = check_plain($_GET['event_id']);
    $event_row = db_query("SELECT * FROM {spider_calendar_events_table} WHERE id=:id", array(':id' => $event_id))->fetchObject();
    $event_title = $event_row->title;
    $event_date = $event_row->event_date;
    $event_date_end = $event_row->event_date_end;
    $event_time = $event_row->event_time;
    $time_event1 = drupal_substr($event_time, 0, 2);
    $time_event2 = drupal_substr($event_time, 3, 2);
    $time_event3 = drupal_substr($event_time, 6, 2);
    $time_event4 = drupal_substr($event_time, 9, 2);
    $event_text = $event_row->text;
    $event_published = $event_row->published;
    $repeat_method = $event_row->repeat_method;
    $repeat = $event_row->event_repeat;
    $week = $event_row->week;
    $month = $event_row->month;
    $month_type = $event_row->month_type;
    $month_list = $event_row->month_list;
    $month_week = $event_row->month_week;
    $year_month = $event_row->years_month;
  }
  else {
    $event_title = '';
    $event_date = '';
    $event_date_end = '';
    $time_event1 = '';
    $time_event2 = '';
    $time_event3 = '';
    $time_event4 = '';
    $event_text = '';
    $event_published = 1;
    $repeat_method = 'no_repeat';
    $repeat = '1';
    $week = '';
    $month = '';
    $month_type = '1';
    $month_list = 'First';
    $month_week = 'Mon';
    $year_month = 'Jan';
  }
  $form = array();
  $form['event'] = array(
    '#type' => 'fieldset',
    '#title' => $cal_name . ' ' . t('Calendar Event'),
    '#attributes' => array('class' => array('spider_calendar_event_left_fielset')),
    '#prefix' => '<div>',
  );
  $form['event']['event_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#default_value' => $event_title,
    '#size' => 10,
  );
  $form['event']['calfield'] = array(
    '#type' => 'textfield',
    '#title' => t('Date'),
    '#required' => TRUE,
    '#default_value' => $event_date,
    '#size' => 10,
  );
  $form['event']['calbutton'] = array(
    '#type' => 'button',
    '#value' => t('...'),
    '#attributes' => array(
      'onclick' => 'return showCalendar("edit-calfield","%Y-%m-%d");',
      
    ),
  );
  $form['event']['event_time'] = array(
    '#prefix' => '<div style="font-weight:bold; margin-top:10px;">' . t('Time') . '</div>
                 <input type="text" class="form-text" name="time_event1" id="time_event1" size="2" value="' . $time_event1 . '" style="text-align:right" onkeypress="return spider_calendar_checkhour(\'time_event1\')" onblur="spider_calendar_add_0(\'time_event1\')"> : 
                 <input type="text" class="form-text" name="time_event2" id="time_event2" size="2" value="' . $time_event2 . '" style="text-align:right" onkeypress="return spider_calendar_checkminute(\'time_event2\')" onblur="spider_calendar_add_0(\'time_event2\')"> - 
                 <input type="text" class="form-text" name="time_event3" id="time_event3" size="2" value="' . $time_event3 . '" style="text-align:right" onkeypress="return spider_calendar_checkhour(\'time_event3\')" onblur="spider_calendar_add_0(\'time_event3\')"> : 
                 <input type="text" class="form-text" name="time_event4" id="time_event4" size="2" value="' . $time_event4 . '" style="text-align:right" onkeypress="return spider_calendar_checkminute(\'time_event4\')" onblur="spider_calendar_add_0(\'time_event4\')">',
  );
  if (file_exists("sites/all/libraries/tinymce/jscripts/tiny_mce/tiny_mce.js")) {
    drupal_add_js('sites/all/libraries/tinymce/jscripts/tiny_mce/tiny_mce.js');
    drupal_add_js('tinyMCE.init({
				// General options
        mode : "specific_textareas",
        editor_selector : "spider_calendar_editor",
        // mode : "textareas",
				theme : "advanced",
				plugins : "autolink,lists,spellchecker,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template",
				// Theme options
				theme_advanced_buttons1 : "save,newdocument,|,bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,styleselect,formatselect,fontselect,fontsizeselect",
				theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
				theme_advanced_buttons3 : "tablecontrols,|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen",
				theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,spellchecker,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,blockquote,pagebreak,|,insertfile,insertimage",
				theme_advanced_toolbar_location : "top",
				theme_advanced_toolbar_align : "left",
				theme_advanced_statusbar_location : "bottom",
				theme_advanced_resizing : true,

				// Skin options
				skin : "o2k7",
				skin_variant : "silver",

				// Example content CSS (should be your site CSS)
				//content_css : "css/example.css",
				
				// Drop lists for link/image/media/template dialogs
				template_external_list_url : "js/template_list.js",
				external_link_list_url : "js/link_list.js",
				external_image_list_url : "js/image_list.js",
				media_external_list_url : "js/media_list.js",

				// Replace values for the template plugin
				template_replace_values : {
					username : "Some User",
					staffid : "991234"
				}
			});', array('type' => 'inline', 'scope' => 'footer'));
  }
  else {
    drupal_add_css(drupal_get_path('module', 'spider_calendar') . '/css/spider_calendar_event_filedset.css');
    $form['event']['event_note_without_editor'] = array(
      '#prefix' => '<div style="width:100%; position:relative; top:30px; background-color:#FEF5F1; color: #8C2E0B; border-color:#ED541D; padding: 2px; margin-bottom:20px; border: 1px solid #DD7777;">
                    To show HTML editor download \'tinymce\' library from <a href=\'http://github.com/downloads/tinymce/tinymce/tinymce_3.5.7.zip\'>http://github.com/downloads/tinymce/tinymce/tinymce_3.5.7.zip</a> and extract it to sites/all/libraries/tinymce directory.
                   </div>',
    );
  }
  $form['event']['event_note'] = array(
    '#prefix' => '<div style="width:450px">',
    '#type' => 'textarea',
    '#title' => t('Note'),
    '#default_value' => $event_text,
    '#cols' => 60,
    '#resizable' => FALSE,
    '#rows' => 10,
    '#attributes' => array('class' => array('spider_calendar_editor')),
    '#suffix' => '</div>',
  );
  $form['event']['event_published'] = array(
    '#type' => 'radios',
    '#title' => t('Published'),
    '#default_value' => $event_published,
    '#options' => array('1' => t('Yes'), '0' => t('No')),
  );
  $form['repeat_event'] = array(
    '#type' => 'fieldset',
    '#title' => t('Repeat Event'),
    '#attributes' => array('class' => array('spider_calendar_event_right_fielset')),
  );
  drupal_add_js('window.onload=spider_calendar_radio_month();
                 window.onload=spider_calendar_change_type("' . $repeat_method . '");
                 window.onload=spider_calendar_week_value();', array('type' => 'inline', 'scope' => 'footer'));
  $form['repeat_event']['repeat_radios'] = array(
    '#prefix' => '<table style="border:none; width:490px">
                  <tbody>
                   <tr>
                    <td valign="top">
                     <input type="radio" onchange=spider_calendar_change_type("no_repeat") ' . (($repeat_method == 'no_repeat') ? 'checked="checked"' : '') . ' name="repeat_method" value="no_repeat">' . t("Don't repeat this event") . '</input><br />
                     <input type="radio" onchange=spider_calendar_change_type("daily") ' . (($repeat_method == 'daily') ? 'checked="checked"' : '') . ' name="repeat_method" value="daily">' . t("Repeat daily") . '</input><br />
                     <input type="radio" onchange=spider_calendar_change_type("weekly") ' . (($repeat_method == 'weekly') ? 'checked="checked"' : '') . ' name="repeat_method" value="weekly">' . t("Repeat weekly") . '</input><br />
                     <input type="radio" onchange=spider_calendar_change_type("monthly") ' . (($repeat_method == 'monthly') ? 'checked="checked"' : '') . ' name="repeat_method" value="monthly">' . t("Repeat monthly") . '</input><br />
                     <input type="radio" onchange=spider_calendar_change_type("yearly") ' . (($repeat_method == 'yearly') ? 'checked="checked"' : '') . ' name="repeat_method" value="yearly">' . t("Repeat yearly") . '</input><br />
                    </td>
                    <td valign="top" style="padding-left:10px; border:none;">
                      <div id="daily" style="display:none; margin:2px;">' . t('Repeat every') . '
                        <input id="repeat_input" type="text" class="form-text" value="' . $repeat . '" onkeypress="return spider_calendar_checknumber(repeat_input)" name="repeat" size="5"></input>
                        <label id="repeat" style="display:inline; margin:2px;">' . t('Day(s)') . '</label>
                        <label id="year_month" style="display:none;">
                          <select id="year_month" class="inputbox form-select" name="year_month" >
                            <option value="01" ' . (($year_month == '01') ? 'selected="selected"' : '') . '>' . t('January') . '</option>
                            <option value="02" ' . (($year_month == '02') ? 'selected="selected"' : '') . '>' . t('February') . '</option>
                            <option value="03" ' . (($year_month == '03') ? 'selected="selected"' : '') . '>' . t('March') . '</option>
                            <option value="04" ' . (($year_month == '04') ? 'selected="selected"' : '') . '>' . t('April') . '</option>
                            <option value="05" ' . (($year_month == '05') ? 'selected="selected"' : '') . '>' . t('May') . '</option>
                            <option value="06" ' . (($year_month == '06') ? 'selected="selected"' : '') . '>' . t('June') . '</option>
                            <option value="07" ' . (($year_month == '07') ? 'selected="selected"' : '') . '>' . t('July') . '</option>
                            <option value="08" ' . (($year_month == '08') ? 'selected="selected"' : '') . '>' . t('August') . '</option>
                            <option value="09" ' . (($year_month == '09') ? 'selected="selected"' : '') . '>' . t('September') . '</option>
                            <option value="10" ' . (($year_month == '10') ? 'selected="selected"' : '') . '>' . t('October') . '</option>
                            <option value="11" ' . (($year_month == '11') ? 'selected="selected"' : '') . '>' . t('November') . '</option>
                            <option value="12" ' . (($year_month == '12') ? 'selected="selected"' : '') . '>' . t('December') . '</option>
                          </select>
                        </label>
                        <input id="daily1" type="hidden" value="' . $repeat . '">
                        <input id="weekly1" type="hidden" value="' . $repeat . '">
                        <input id="monthly1" type="hidden" value="' . $repeat . '">
                        <input id="yearly1" type="hidden" value="' . $repeat . '">
                      </div>
                      <div id="weekly" class="key" style="display:none;">
                        <input id="week_1" type="checkbox" onchange="spider_calendar_week_value()" value="Mon" ' . ((!strstr($week, 'Mon')) ? '' : 'checked="checked"') . '>' . t('Mon') . '</input>
                        <input id="week_2" type="checkbox" onchange="spider_calendar_week_value()" value="Tue" ' . ((!strstr($week, 'Tue')) ? '' : 'checked="checked"') . '>' . t('Tue') . '</input>
                        <input id="week_3" type="checkbox" onchange="spider_calendar_week_value()" value="Wed" ' . ((!strstr($week, 'Wed')) ? '' : 'checked="checked"') . '>' . t('Wed') . '</input>
                        <input id="week_4" type="checkbox" onchange="spider_calendar_week_value()" value="Thu" ' . ((!strstr($week, 'Thu')) ? '' : 'checked="checked"') . '>' . t('Thu') . '</input>
                        <input id="week_5" type="checkbox" onchange="spider_calendar_week_value()" value="Fri" ' . ((!strstr($week, 'Fri')) ? '' : 'checked="checked"') . '>' . t('Fri') . '</input>
                        <input id="week_6" type="checkbox" onchange="spider_calendar_week_value()" value="Sat" ' . ((!strstr($week, 'Sat')) ? '' : 'checked="checked"') . '>' . t('Sat') . '</input>
                        <input id="week_7" type="checkbox" onchange="spider_calendar_week_value()" value="Sun" ' . ((!strstr($week, 'Sun')) ? '' : 'checked="checked"') . '>' . t('Sun') . '</input>
                        <input id="week" type="hidden" value="" name="week">
                      </div>
                      <div id="monthly" class="key" style="display:none">
                        <input id="radio1" type="radio" ' . (($month_type == '1') ? 'checked="checked"' : '') . ' value="1" onchange="spider_calendar_radio_month()" name="month_type">' . t('on the:') . '</input>
                        <input id="month" class="form-text" type="text" value="' . $month . '" onkeypress="return spider_calendar_checknumber(month)" size="3" name="month" style="margin:2px;"></input><br>
                        <input id="radio2" type="radio" ' . (($month_type == '2') ? 'checked="checked"' : '') . ' value="2" onchange="spider_calendar_radio_month()" name="month_type" style="margin:2px;">' . t('on the:') . '
                        <select id="monthly_list" class="inputbox form-select" name="monthly_list" disabled="">
                          <option value="1" ' . (($month_list == '1') ? 'selected="selected"' : '') . '>' . t('First') . '</option>
                          <option value="8" ' . (($month_list == '8') ? 'selected="selected"' : '') . '>' . t('Second') . '</option>
                          <option value="15" ' . (($month_list == '15') ? 'selected="selected"' : '') . '>' . t('Third') . '</option>
                          <option value="22" ' . (($month_list == '22') ? 'selected="selected"' : '') . '>' . t('Fourth') . '</option>
                          <option value="last" ' . (($month_list == 'last') ? 'selected="selected"' : '') . '>' . t('Last') . '</option>
                        </select>
                        <select id="month_week" class="inputbox form-select" name="month_week" disabled="" style="margin:2px;">
                          <option value="Mon" ' . (($month_week == 'Mon') ? 'selected="selected"' : '') . '>' . t('Monday') . '</option>
                          <option value="Tue" ' . (($month_week == 'Tue') ? 'selected="selected"' : '') . '>' . t('Tuesday') . '</option>
                          <option value="Wed" ' . (($month_week == 'Wed') ? 'selected="selected"' : '') . '>' . t('Wednesday') . '</option>
                          <option value="Thu" ' . (($month_week == 'Thu') ? 'selected="selected"' : '') . '>' . t('Thursday') . '</option>
                          <option value="Fri" ' . (($month_week == 'Fri') ? 'selected="selected"' : '') . '>' . t('Friday') . '</option>
                          <option value="Sat" ' . (($month_week == 'Sat') ? 'selected="selected"' : '') . '>' . t('Saturday') . '</option>
                          <option value="Sun" ' . (($month_week == 'Sun') ? 'selected="selected"' : '') . '>' . t('Sunday') . '</option>
                        </select>
                      </div>
                    </td>
                   </tr>
                   <tr id="repeat_until" style="display:none">
                      <td>' . t('Repeat until:') . ' </td>
                      <td style="border:none;">
                        <input id="date_end" class="inputbox form-text" type="text" value="' . $event_date_end . '" maxlength="10" size="10" name="date_end" style="width:90px">
                        <input class="form-submit" type="reset" onclick=\'return showCalendar("date_end","%Y-%m-%d");\' value="...">
                      </td>
                   </tr>
                 </tbody>
               </table>',
    '#suffix' => '</div>',
  );
  $form['event_save'] = array(
    '#type' => 'submit',
    '#prefix' => '<div style="float:left; bottom:0px; clear:both;">',
    '#value' => t('Save'),
    '#submit' => array('spider_calendar_event_save'),
  );
  $form['event_apply'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('spider_calendar_event_apply'),
  );
  $form['event_cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#attributes' => array('onclick' => 'document.getElementById("edit-event-title").setAttribute("style", "color:rgba(255, 0, 0, 0)");document.getElementById("edit-event-title").setAttribute("value", "cancel");'),
    '#submit' => array('spider_calendar_event_cancel'),
    '#suffix' => '</div>',
  );
  return $form;
}

/**
 * Save event.
 */
function spider_calendar_event_save($form, &$form_state) {
  if (isset($_GET['calendar'])) {
    $calendar_id = check_plain($_GET['calendar']);
  }
  $date_start_event = $form_state['values']['calfield'];
  if (isset($_POST['date_end'])) {
    $date_end_event = check_plain($_POST['date_end']);
  }
  else {
    $date_end_event = '';
  }
  if (isset($_POST['time_event1'])) {
    $time_event = check_plain($_POST['time_event1']);
  }
  else {
    $time_event = '';
  }
  $time_event .= ':';
  if (isset($_POST['time_event2'])) {
    $time_event .= check_plain($_POST['time_event2']);
  }
  else {
    $time_event .= '';
  }
  if (check_plain($_POST['time_event3']) != '' && check_plain($_POST['time_event4']) != '') {
    $time_event .= '-';
    $time_event .= check_plain($_POST['time_event3']);
    $time_event .= ':';
    $time_event .= check_plain($_POST['time_event4']);
  }
  if (drupal_substr($time_event, 0, 1) == ':') {
    $time_event = '';
  }
  if (isset($_POST['repeat_method'])) {
    $repeat_method = check_plain($_POST['repeat_method']);
  }
  else {
    $repeat_method = '';
  }
  if (isset($_POST['repeat'])) {
    $repeat = check_plain($_POST['repeat']);
  }
  else {
    $repeat = '';
  }
  if (isset($_POST['week'])) {
    $week = check_plain($_POST['week']);
  }
  else {
    $week = '';
  }
  if (isset($_POST['month'])) {
    $month = (int)check_plain($_POST['month']);
  }
  else {
    $month = '';
  }
  if (isset($_POST['month_type'])) {
    $month_type = check_plain($_POST['month_type']);
  }
  else {
    $month_type = '';
  }
  if (isset($_POST['monthly_list'])) {
    $month_list = check_plain($_POST['monthly_list']);
  }
  else {
    $month_list = '';
  }
  if (isset($_POST['month_week'])) {
    $month_week = check_plain($_POST['month_week']);
  }
  else {
    $month_week = '';
  }
  if (isset($_POST['year_month'])) {
    $year_month = check_plain($_POST['year_month']);
  }
  else {
    $year_month = '';
  }
  if (isset($_GET['event_id'])) {
    $event_id = check_plain($_GET['event_id']);
    db_query("UPDATE {spider_calendar_events_table} SET title=:title,event_date=:event_date,event_date_end=:event_date_end,event_time=:event_time,text=:text,published=:published,repeat_method=:repeat_method,event_repeat=:event_repeat,week=:week,month=:month,month_type=:month_type,month_list=:month_list,month_week=:month_week,years_month=:years_month WHERE id=:id", array(
      ':title' => $form_state['values']['event_title'],
      ':event_date' => $date_start_event,
      ':event_date_end' => $date_end_event,
      ':event_time' => $time_event,
      ':text' => $form_state['values']['event_note'],
      ':published' => $form_state['values']['event_published'],
      ':repeat_method' => $repeat_method,
      ':event_repeat' => $repeat,
      ':week' => $week,
      ':month' => $month,
      ':month_type' => $month_type,
      ':month_list' => $month_list,
      ':month_week' => $month_week,
      ':years_month' => $year_month,
      ':id' => $event_id));
    drupal_set_message(t('Your event successfully updated.'), 'status', FALSE);
  }
  else {
    db_insert('spider_calendar_events_table')
      ->fields(array(
        'calendar' => $calendar_id,
        'title' => $form_state['values']['event_title'],
        'event_date' => $date_start_event,
        'event_date_end' => $date_end_event,
        'event_time' => $time_event,
        'text' => $form_state['values']['event_note'],
        'published' => $form_state['values']['event_published'],
        'repeat_method' => $repeat_method,
        'event_repeat' => $repeat,
        'week' => $week,
        'month' => $month,
        'month_type' => $month_type,
        'month_list' => $month_list,
        'month_week' => $month_week,
        'years_month' => $year_month,
        ))
      ->execute();
    drupal_set_message(t('Your event successfully saved.'), 'status', FALSE);
  }
  $form_state['redirect'] = url('admin/settings/spider_calendar/Calendars/events', array('query' => array('calendar' => $calendar_id), 'absolute' => TRUE));
}

/**
 * Apply event.
 */
function spider_calendar_event_apply($form, &$form_state) {
  if (isset($_GET['calendar'])) {
    $calendar_id = check_plain($_GET['calendar']);
  }
  $date_start_event = $form_state['values']['calfield'];
  if (isset($_POST['date_end'])) {
    $date_end_event = check_plain($_POST['date_end']);
  }
  else {
    $date_end_event = '';
  }
  if (isset($_POST['time_event1'])) {
    $time_event = check_plain($_POST['time_event1']);
  }
  else {
    $time_event = '';
  }
  $time_event .= ':';
  if (isset($_POST['time_event2'])) {
    $time_event .= check_plain($_POST['time_event2']);
  }
  else {
    $time_event .= '';
  }
  if (check_plain($_POST['time_event3']) != '' && check_plain($_POST['time_event4']) != '') {
    $time_event .= '-';
    $time_event .= check_plain($_POST['time_event3']);
    $time_event .= ':';
    $time_event .= check_plain($_POST['time_event4']);
  }
  if (drupal_substr($time_event, 0, 1) == ':') {
    $time_event = '';
  }
  if (isset($_POST['repeat_method'])) {
    $repeat_method = check_plain($_POST['repeat_method']);
  }
  else {
    $repeat_method = '';
  }
  if (isset($_POST['repeat'])) {
    $repeat = check_plain($_POST['repeat']);
  }
  else {
    $repeat = '';
  }
  if (isset($_POST['week'])) {
    $week = check_plain($_POST['week']);
  }
  else {
    $week = '';
  }
  if (isset($_POST['month'])) {
    $month = (int)check_plain($_POST['month']);
  }
  else {
    $month = '';
  }
  if (isset($_POST['month_type'])) {
    $month_type = check_plain($_POST['month_type']);
  }
  else {
    $month_type = '';
  }
  if (isset($_POST['monthly_list'])) {
    $month_list = check_plain($_POST['monthly_list']);
  }
  else {
    $month_list = '';
  }
  if (isset($_POST['month_week'])) {
    $month_week = check_plain($_POST['month_week']);
  }
  else {
    $month_week = '';
  }
  if (isset($_POST['year_month'])) {
    $year_month = check_plain($_POST['year_month']);
  }
  else {
    $year_month = '';
  }
  if (isset($_GET['event_id'])) {
    $event_id = check_plain($_GET['event_id']);
    db_query("UPDATE {spider_calendar_events_table} SET title=:title,event_date=:event_date,event_date_end=:event_date_end,event_time=:event_time,text=:text,published=:published,repeat_method=:repeat_method,event_repeat=:event_repeat,week=:week,month=:month,month_type=:month_type,month_list=:month_list,month_week=:month_week,years_month=:years_month WHERE id=:id", array(
      ':title' => $form_state['values']['event_title'],
      ':event_date' => $date_start_event,
      ':event_date_end' => $date_end_event,
      ':event_time' => $time_event,
      ':text' => $form_state['values']['event_note'],
      ':published' => $form_state['values']['event_published'],
      ':repeat_method' => $repeat_method,
      ':event_repeat' => $repeat,
      ':week' => $week,
      ':month' => $month,
      ':month_type' => $month_type,
      ':month_list' => $month_list,
      ':month_week' => $month_week,
      ':years_month' => $year_month,
      ':id' => $event_id));
    drupal_set_message(t('Your event successfully updated.'), 'status', FALSE);
  }
  else {  
    db_insert('spider_calendar_events_table')
      ->fields(array(
        'calendar' => $calendar_id,
        'title' => $form_state['values']['event_title'],
        'event_date' => $date_start_event,
        'event_date_end' => $date_end_event,
        'event_time' => $time_event,
        'text' => $form_state['values']['event_note'],
        'published' => $form_state['values']['event_published'],
        'repeat_method' => $repeat_method,
        'event_repeat' => $repeat,
        'week' => $week,
        'month' => $month,
        'month_type' => $month_type,
        'month_list' => $month_list,
        'month_week' => $month_week,
        'years_month' => $year_month,
        ))
      ->execute();
    $event_id = db_query("SELECT MAX(id) FROM {spider_calendar_events_table}")->fetchField();
    drupal_set_message(t('Your event successfully saved.'), 'status', FALSE);
  }
  $form_state['redirect'] = url('admin/settings/spider_calendar/Calendars/events/add', array('query' => array('calendar' => $calendar_id, 'event_id' => $event_id), 'absolute' => TRUE));
}

/**
 * Apply event.
 */
function spider_calendar_event_cancel($form, &$form_state) {
  if (isset($_GET['calendar'])) {
    $calendar_id = check_plain($_GET['calendar']);
  }
  $form_state['redirect'] = url('admin/settings/spider_calendar/Calendars/events', array('query' => array('calendar' => $calendar_id), 'absolute' => TRUE));
}
