<?php
/**
 * @file
 *
 * Block themes table.
 */

/**
 * Menu loader callback. Load a Spider Calendar Block themes.
 */
function spider_calendar_block_cal_theme() {
  drupal_set_message(t('Themes are disabled for the non-commercial version. You can create, edit and preview the calendar themes here, but to apply the themes to the calendars, you are required to purchase the full version.'), 'warning', FALSE);
  $form['fieldset_block_theme_buttons'] = array(
    '#prefix' => '<a href="http://web-dorado.com/files/fromSpiderCalendarDrupal.php" target="_blank" style="color:red; text-decoration:none; float:right;">
                    <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/header.png" border="0" alt="www.web-dorado.com" width="215"><br />
                  <div style="float:right;">' . t('Get the full version') . '&nbsp;&nbsp;&nbsp;&nbsp;</div>
                  </a><br /><br /><br />',
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('fieldset')),
  );
  $form['fieldset_block_theme_buttons']['delete_block_themes'] = array(
    '#type' => 'submit',
    '#submit' => array('spider_calendar_block_delete_theme'),
    '#value' => t('Delete'),
  );
  $form['fieldset_block_theme_buttons']['new_block_theme'] = array(
    '#prefix' => '<a href="' . url('admin/settings/spider_calendar/Block Calendar Themes/add', array('absolute' => TRUE)) . '">' . t('New') . '</a>',
    '#suffix' => '<a href="http://web-dorado.com/4-creatingediting-calendar-themes/drupal-event-calendar-guide-step-4-1.html" target="_blank" style="float:right;"><img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/user-manual.png" border="0" alt="' . t('User Manual') . '"></a>',
  );
  drupal_add_js(drupal_get_path('module', 'spider_calendar') . '/js/spider_calendar_check_all.js');
  $id_col = db_query("SELECT id FROM {spider_calendar_block_theme_table}")->fetchCol();
  $id_col_string = implode($id_col, ',');
  $form['fieldset_block_themes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block Calendar Themes'),
    '#attributes' => array('class' => array('fieldset')),
  );
  $form['fieldset_block_themes']['block_themes_table'] = array(
    '#prefix' => '<table>
                  <tr>
                    <th width="50px">' . l(t('ID') . ((isset($_GET['sort_by']) && ($_GET['sort_by'] == 'id'))? ((isset($_GET['asc_or_desc']) && ($_GET['asc_or_desc'] == 1)) ? ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/asc.png">' : ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/desc.png">') : ''), url('admin/settings/spider_calendar/Block Calendar Themes', array('query' => array('sort_by' => 'id', 'asc_or_desc' => (isset($_GET['asc_or_desc']) ? !$_GET['asc_or_desc'] : 1)), 'absolute' => TRUE)), array('html' => TRUE)) . '</th>
                    <th width="50px">',
    '#type' => 'checkbox',
    '#attributes' => array('onclick' => 'spider_calendar_check_all("edit-block-themes-table", "' . $id_col_string . '", "edit-block-theme-check")'),
    '#suffix' => '
                    </th>
                    <th>' . l(t('Title') . ((isset($_GET['sort_by']) && ($_GET['sort_by'] == 'title'))? ((isset($_GET['asc_or_desc']) && ($_GET['asc_or_desc'] == 1)) ? ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/asc.png">' : ' <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/desc.png">') : ''), url('admin/settings/spider_calendar/Block Calendar Themes', array('query' => array('sort_by' => 'title', 'asc_or_desc' => (isset($_GET['asc_or_desc']) ? !$_GET['asc_or_desc'] : 1)), 'absolute' => TRUE)), array('html' => TRUE)) . '</th>
                    <th width="50px">' . t('Delete') . '</th>
                  </tr>',
  );
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
      $block_themes_ids_col = db_query("SELECT id FROM {spider_calendar_block_theme_table} ORDER BY title " . $asc_or_desc)->fetchCol();
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
      $block_themes_ids_col = db_query("SELECT id FROM {spider_calendar_block_theme_table} ORDER BY id " . $asc_or_desc)->fetchCol();
    }
  }
  else {
    $block_themes_ids_col = db_query("SELECT id FROM {spider_calendar_block_theme_table}")->fetchCol();
  }
  foreach ($block_themes_ids_col as $row_id) {
    $row = db_query("SELECT * FROM {spider_calendar_block_theme_table} WHERE id=:id", array(':id' => $row_id))->fetchObject();
    $id = $row->id;
    if ($id != 12) {
      $title = $row->title;
      if ($row->default_theme == '1') {
        $disable_checkboxe = 'disabled';
      }
      else {
        $disable_checkboxe = 'enabled';
      }
      $form['fieldset_block_themes']['block_theme_check' . $id] = array(
        '#prefix' => '<tr  style="border-bottom:1px solid #cccccc;">
                       <td>' . $id . '</td>
                       <td>',
        '#type' => 'checkbox',
        '#attributes' => array($disable_checkboxe => $disable_checkboxe),
        '#suffix' => '</td>                    
                      <td>
                        <a href="' . url('admin/settings/spider_calendar/Block Calendar Themes/add', array('query' => array('block_theme_id' => $id), 'absolute' => TRUE)) . '"> ' . $title . '</a>
                      </td>',
      );
      if ($row->default_theme != '1') {
        $form['fieldset_block_themes']['block_delete_theme' . $id] = array(
            '#prefix' => '<td>',
            '#type' => 'image_button',
            '#src' => drupal_get_path('module', 'spider_calendar') . '/images/delete_row.png',
            '#submit' => array('spider_calendar_block_delete_theme'),
            '#attributes' => array('onclick' => 'if (!confirm(Drupal.t("Do you really want to delete?"))) {return false;}'),
            '#suffix' => '</td>',
          );
      }
      else {
        $form['fieldset_block_themes']['block_delete_theme' . $id] = array(
          '#prefix' => '<td></td>',
        );
      }
      $form['fieldset_block_themes']['block_theme_row_end'] = array(
        '#prefix' => '</tr>',
      );
    }
  }
  $form['fieldset_block_themes']['block_themes_table_end'] = array(
    '#prefix' => '</table>',
  );
  return $form;
}

/**
 * Delete selected block themes.
 */
function spider_calendar_block_delete_theme($form, &$form_state) {
  $block_themes_ids_col = db_query("SELECT id FROM {spider_calendar_block_theme_table}")->fetchCol();
  foreach ($block_themes_ids_col as $id) {
    if ($id != 12) {
      if ($form_state['values']['block_theme_check' . $id] || (isset($form_state['values']['block_delete_theme' . $id]) && $form_state['values']['block_delete_theme' . $id])) {
        db_query("DELETE FROM {spider_calendar_block_theme_table} WHERE id=:id", array(':id' => $id));
        drupal_set_message(t('Selected themes successfully deleted.'), 'status', FALSE);
      }
    }
  }
}

/**
 * Add a new block theme.
 */
function  spider_calendar_add_block_theme() {
  drupal_add_css(drupal_get_path('module', 'spider_calendar') . '/css/spider_calendar_event_filedset.css');
  drupal_add_js(drupal_get_path('module', 'spider_calendar') . '/js/spider_calendar_reset_block_theme.js');
  if (!file_exists("sites/all/libraries/jscolor/jscolor.js")) {
    drupal_set_message(t("Download 'jscolor' library from <a href='http://jscolor.com/release/jscolor-1.4.0.zip'>http://jscolor.com/release/jscolor-1.4.0.zip</a> and extract it to sites/all/libraries/jscolor directory.<br />"), 'warning', FALSE);
  }
  else {
    drupal_add_js('sites/all/libraries/jscolor/jscolor.js');
  }
  if (isset($_GET['block_theme_id'])) {
    $theme_id = check_plain($_GET['block_theme_id']);
    $theme_row = db_query("SELECT * FROM {spider_calendar_block_theme_table} WHERE id=:id", array(':id' => $theme_id))->fetchObject();
    $theme_title = $theme_row->title;
    $theme_width = $theme_row->theme_width;
    $week_start_day = $theme_row->week_start_day;
    $border_color = $theme_row->border_color;
    $border_radius = $theme_row->border_radius;
    $border_width = $theme_row->border_width;
    $show_titles_cloud = $theme_row->show_titles_cloud;
    $show_date_and_time_in_titlescloud = $theme_row->show_date_and_time_in_titlescloud;
    $titles_cloud_text_color = $theme_row->titles_cloud_text_color;
    $body_bgcolor = $theme_row->body_bgcolor;
    $cell_height = $theme_row->cell_height;
    $omd_fcolor = $theme_row->omd_fcolor;
    $omd_bgcolor = $theme_row->omd_bgcolor;
    $cell_text_color_wevents = $theme_row->cell_text_color_wevents;
    $cell_text_color_events = $theme_row->cell_text_color_events;
    $cell_text_color_selected_events = $theme_row->cell_text_color_selected_events;
    $event_title_color = $theme_row->event_title_color;
    $cell_bgcolor_events = $theme_row->cell_bgcolor_events;
    $cell_bgcolor_selected_events = $theme_row->cell_bgcolor_selected_events;
    $cur_day_cell_border_color = $theme_row->cur_day_cell_border_color;
    $selected_day_cell_border_color = $theme_row->selected_day_cell_border_color;
    $cell_border_color = $theme_row->cell_border_color;
    $all_days_border = $theme_row->all_days_border;
    $sundays_tcolor = $theme_row->sundays_tcolor;
    $sundays_cell_bgcolor = $theme_row->sundays_cell_bgcolor;
    $sundays_fcolor = $theme_row->sundays_fcolor;
    $days_fsize = $theme_row->days_fsize;
    $time_in_cell = $theme_row->time_in_cell;
    $header_heidht = $theme_row->header_heidht;
    $header_bgcolor = $theme_row->header_bgcolor;
    $year_font_size = $theme_row->year_font_size;
    $year_color = $theme_row->year_color;
    $year_arrow_color = $theme_row->year_arrow_color;
    $month_type = $theme_row->month_type;
    $cur_month_color = $theme_row->cur_month_color;
    $cur_month_fsize = $theme_row->cur_month_fsize;
    $month_arrow_color = $theme_row->month_arrow_color;
    $next_month_color = $theme_row->next_month_color;
    $next_month_fsize = $theme_row->next_month_fsize;
    $next_month_arrow_color = $theme_row->next_month_arrow_color;
    $prev_month_color = $theme_row->prev_month_color;
    $prev_month_fsize = $theme_row->prev_month_fsize;
    $prev_month_arrow_color = $theme_row->prev_month_arrow_color;
    $arrow_size = $theme_row->arrow_size;
    $weekdays_color = $theme_row->weekdays_color;
    $week_cell_height = $theme_row->week_cell_height;
    $weekdays_bgcolor = $theme_row->weekdays_bgcolor;
    $sunday_bgcolor = $theme_row->sunday_bgcolor;
    $weekdays_fsize = $theme_row->weekdays_fsize;
    $date_format_popup = '';
    $event_title_color_popup = $theme_row->event_title_color_popup;
    $event_title_fsize_popup = $theme_row->event_title_fsize_popup;
    $event_title_family_popup = $theme_row->event_title_family_popup;
    $event_title_fstyle_popup = $theme_row->event_title_fstyle_popup;
    $date_color_popup = $theme_row->date_color_popup;
    $date_fsize_popup = $theme_row->date_fsize_popup;
    $date_family_popup = $theme_row->date_family_popup;
    $date_fstyle_popup = $theme_row->date_fstyle_popup;
    $arrow_bgcolor_popup = '';
    $arrow_color_popup = '';
    $popup_bgcolor = '';
    $popup_width = '';
    $popup_height = '';
    $displaied_events = '';
    $repeat_rate = $theme_row->repeat_rate;
    $default_theme = $theme_row->default_theme;
  }
  else {
    $theme_row = db_query("SELECT * FROM {spider_calendar_block_theme_table} WHERE id=:id", array(':id' => 1))->fetchObject();
    $theme_title = 'new ' . $theme_row->title;
    $theme_width = $theme_row->theme_width;
    $week_start_day = $theme_row->week_start_day;
    $border_color = $theme_row->border_color;
    $border_radius = $theme_row->border_radius;
    $show_titles_cloud = $theme_row->show_titles_cloud;
    $show_date_and_time_in_titlescloud = $theme_row->show_date_and_time_in_titlescloud;
    $titles_cloud_text_color = $theme_row->titles_cloud_text_color;
    $border_width = $theme_row->border_width;
    $body_bgcolor = $theme_row->body_bgcolor;
    $cell_height = $theme_row->cell_height;
    $omd_fcolor = $theme_row->omd_fcolor;
    $omd_bgcolor = $theme_row->omd_bgcolor;
    $cell_text_color_wevents = $theme_row->cell_text_color_wevents;
    $cell_text_color_events = $theme_row->cell_text_color_events;
    $cell_text_color_selected_events = $theme_row->cell_text_color_selected_events;
    $event_title_color = $theme_row->event_title_color;
    $cell_bgcolor_events = $theme_row->cell_bgcolor_events;
    $cell_bgcolor_selected_events = $theme_row->cell_bgcolor_selected_events;
    $cur_day_cell_border_color = $theme_row->cur_day_cell_border_color;
    $selected_day_cell_border_color = $theme_row->selected_day_cell_border_color;
    $cell_border_color = $theme_row->cell_border_color;
    $all_days_border = $theme_row->all_days_border;
    $sundays_tcolor = $theme_row->sundays_tcolor;
    $sundays_cell_bgcolor = $theme_row->sundays_cell_bgcolor;
    $sundays_fcolor = $theme_row->sundays_fcolor;
    $days_fsize = $theme_row->days_fsize;
    $time_in_cell = $theme_row->time_in_cell;
    $header_heidht = $theme_row->header_heidht;
    $header_bgcolor = $theme_row->header_bgcolor;
    $year_font_size = $theme_row->year_font_size;
    $year_color = $theme_row->year_color;
    $year_arrow_color = $theme_row->year_arrow_color;
    $month_type = $theme_row->month_type;
    $cur_month_color = $theme_row->cur_month_color;
    $cur_month_fsize = $theme_row->cur_month_fsize;
    $month_arrow_color = $theme_row->month_arrow_color;
    $next_month_color = $theme_row->next_month_color;
    $next_month_fsize = $theme_row->next_month_fsize;
    $next_month_arrow_color = $theme_row->next_month_arrow_color;
    $prev_month_color = $theme_row->prev_month_color;
    $prev_month_fsize = $theme_row->prev_month_fsize;
    $prev_month_arrow_color = $theme_row->prev_month_arrow_color;
    $arrow_size = $theme_row->arrow_size;
    $weekdays_color = $theme_row->weekdays_color;
    $week_cell_height = $theme_row->week_cell_height;
    $weekdays_bgcolor = $theme_row->weekdays_bgcolor;
    $sunday_bgcolor = $theme_row->sunday_bgcolor;
    $weekdays_fsize = $theme_row->weekdays_fsize;
    $date_format_popup = '';
    $event_title_color_popup = $theme_row->event_title_color_popup;
    $event_title_fsize_popup = $theme_row->event_title_fsize_popup;
    $event_title_family_popup = $theme_row->event_title_family_popup;
    $event_title_fstyle_popup = $theme_row->event_title_fstyle_popup;
    $date_color_popup = $theme_row->date_color_popup;
    $date_fsize_popup = $theme_row->date_fsize_popup;
    $date_family_popup = $theme_row->date_family_popup;
    $date_fstyle_popup = $theme_row->date_fstyle_popup;
    $arrow_bgcolor_popup = '';
    $arrow_color_popup = '';
    $popup_bgcolor = '';
    $popup_width = '';
    $popup_height = '';
    $displaied_events = '';
    $repeat_rate = $theme_row->repeat_rate;
    $default_theme = '0';
  }
  drupal_set_message(t('Themes are disabled for the non-commercial version. You can create, edit and preview the calendar themes here, but to apply the themes to the calendars, you are required to purchase the full version.'), 'warning', FALSE);
  $form = array();
  if ($default_theme != '1') {
    $themes = db_query("SELECT id,title FROM {spider_calendar_block_theme_table} WHERE default_theme=:default_theme ORDER BY title", array(':default_theme' => 1))->fetchAllKeyed();
    $form['block_theme_select'] = array(
      '#type' => 'fieldset',
      '#attributes' => array(
        'class' => array('spider_calendar_theme_select_fieldset'),
        'id' => 'fieldset_theme_select',
      ),
    );
    $form['block_theme_select']['default_themes'] = array(
      '#type' => 'select',
      '#title' => t('Inherit from theme'),
      '#options' => $themes,
      '#attributes' => array('onchange' => 'spider_calendar_set_block_theme();'),
    );
  }
  $form['block_theme_general'] = array(
    '#prefix' => '<a href="http://web-dorado.com/files/fromSpiderCalendarDrupal.php" target="_blank" style="color:red; text-decoration:none; float:right;">
                    <img src="' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/header.png" border="0" alt="www.web-dorado.com" width="215"><br />
                  <div style="float:right;">' . t('Get the full version') . '&nbsp;&nbsp;&nbsp;&nbsp;</div>
                  </a>',
    '#type' => 'fieldset',
    '#title' => t('General Parameters'),
    '#attributes' => array(
      'class' => array('spider_calendar_theme_general_fielset'),
      'id' => 'theme_general_par',
    ),
  );
  $form['block_theme_general']['theme_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#default_value' => $theme_title,
    '#size' => 25,
  );
  $form['block_theme_general']['theme_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $theme_width,
    '#size' => 25,
  );
  $form['block_theme_general']['week_start_day'] = array(
    '#type' => 'select',
    '#title' => t('The first day of the week'),
    '#options' => array(
      'mo' => t('Monday'),
      'su' => t('Sunday'),
    ),
    '#default_value' => $week_start_day,
  );
  $form['block_theme_general']['border_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Main border color'),
    '#default_value' => $border_color,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_general']['border_radius'] = array(
    '#type' => 'textfield',
    '#title' => t('Main border radius'),
    '#default_value' => $border_radius,
    '#size' => 25,
  );
  $form['block_theme_general']['border_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Main border width'),
    '#default_value' => $border_width,
    '#size' => 25,
  );
  $form['block_theme_general']['show_titles_cloud'] = array(
    '#type' => 'radios',
    '#title' => t('Show Titles Cloud'),
    '#options' => array('1' => t('Yes'), '0' => t('No')),
    '#default_value' => $show_titles_cloud,
    '#size' => 25,
  );
  $form['block_theme_general']['show_date_and_time_in_titlescloud'] = array(
    '#type' => 'radios',
    '#title' => t('Show date and time in titlescloud'),
    '#default_value' => $show_date_and_time_in_titlescloud,
    '#options' => array('1' => t('Yes'), '0' => t('No')),
    '#size' => 25,
  );
  $form['block_theme_general']['titles_cloud_text_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Titles Cloud text color'),
    '#default_value' => $titles_cloud_text_color,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  if (isset($_GET['block_theme_id']) && $default_theme == '1') {
    $form['block_theme_general']['reset_button'] = array(
      '#prefix' => '<div id="reset_theme_id" onclick="spider_calendar_reset_block_theme_' . $theme_id . '();" style="width:170px; height:35px; color:#ffffff; font-size:15px; background-repeat:no-repeat; background-image:url(\'' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/reset_theme.png\')">
                    <p style="font-weight: bold; padding-left: 51px; padding-top: 7px; margin:0px;">' . t('Reset Theme') . '</p>
                  </div>',
    );
  }
  $form['block_theme_body'] = array(
    '#type' => 'fieldset',
    '#title' => t('Body Parameters'),
    '#attributes' => array(
      'class' => array('spider_calendar_theme_body_fielset'),
      'id' => 'theme_body_par',
    ),
  );
  $form['block_theme_body']['body_bgcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#default_value' => $body_bgcolor,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_body']['cell_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Cell height'),
    '#default_value' => $cell_height,
    '#size' => 25,
  );
  $form['block_theme_body']['omd_fcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Other months days text color'),
    '#default_value' => $omd_fcolor,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_body']['omd_bgcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Other months days background color'),
    '#default_value' => $omd_bgcolor,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_body']['cell_text_color_wevents'] = array(
    '#type' => 'textfield',
    '#title' => t('Cell text color without events'),
    '#default_value' => $cell_text_color_wevents,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_body']['cell_text_color_events'] = array(
    '#type' => 'textfield',
    '#title' => t('Cell text color with events'),
    '#default_value' => $cell_text_color_events,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_body']['cell_text_color_selected_events'] = array(
    '#type' => 'textfield',
    '#title' => t('Cell text color selected events'),
    '#default_value' => $cell_text_color_selected_events,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_body']['cell_bgcolor_events'] = array(
    '#type' => 'textfield',
    '#title' => t('Cell background color with events'),
    '#default_value' => $cell_bgcolor_events,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_body']['cell_bgcolor_selected_events'] = array(
    '#type' => 'textfield',
    '#title' => t('Cell background color selected events'),
    '#default_value' => $cell_bgcolor_selected_events,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_body']['event_title_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Event title color'),
    '#default_value' => $event_title_color,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_body']['cur_day_cell_border_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Current day cell border color'),
    '#default_value' => $cur_day_cell_border_color,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_body']['selected_day_cell_border_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Selected day cell border color'),
    '#default_value' => $selected_day_cell_border_color,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_body']['cell_border_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Cell border color'),
    '#default_value' => $cell_border_color,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_body']['all_days_border'] = array(
    '#type' => 'textfield',
    '#title' => t('All Days Border'),
    '#default_value' => $all_days_border,
    '#size' => 25,
  );
  $form['block_theme_body']['sundays_tcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Sundays text color'),
    '#default_value' => $sundays_tcolor,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_body']['sundays_cell_bgcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Sundays cell background color'),
    '#default_value' => $sundays_cell_bgcolor,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_body']['sundays_fcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Sundays font size'),
    '#default_value' => $sundays_fcolor,
    '#size' => 25,
  );
  $form['block_theme_body']['days_fsize'] = array(
    '#type' => 'textfield',
    '#title' => t('Days font size'),
    '#default_value' => $days_fsize,
    '#size' => 25,
  );
  $form['block_theme_body']['time_in_cell'] = array(
    '#type' => 'radios',
    '#title' => t('Show time in cell'),
    '#default_value' => $time_in_cell,
    '#options' => array('1' => t('Yes'), '0' => t('No')),
  );
  $form['block_theme_header'] = array(
    '#type' => 'fieldset',
    '#title' => t('Header Parameters'),
    '#attributes' => array(
      'class' => array('spider_calendar_theme_header_fielset'),
      'id' => 'theme_header_par',
    ),
  );
  $form['block_theme_header']['header_heidht'] = array(
    '#type' => 'textfield',
    '#title' => t('Header height'),
    '#default_value' => $header_heidht,
    '#size' => 25,
  );
  $form['block_theme_header']['header_bgcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Header background color'),
    '#default_value' => $header_bgcolor,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_header']['year_font_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Year font size'),
    '#default_value' => $year_font_size,
    '#size' => 25,
  );
  $form['block_theme_header']['year_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Year color'),
    '#default_value' => $year_color,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_header']['year_arrow_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Year arrow color'),
    '#default_value' => $year_arrow_color,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_header']['month_type'] = array(
    '#type' => 'select',
    '#title' => t('Month display format'),
    '#options' => array(
      '1' => t('Previous, Current, Next'),
      '2' => t('Current'),
    ),
    '#default_value' => $month_type,
  );
  $form['block_theme_header']['cur_month_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Current Month color'),
    '#default_value' => $cur_month_color,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_header']['cur_month_fsize'] = array(
    '#type' => 'textfield',
    '#title' => t('Current month font size'),
    '#default_value' => $cur_month_fsize,
    '#size' => 25,
  );
  $form['block_theme_header']['month_arrow_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Current Month arrow color'),
    '#default_value' => $month_arrow_color,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_header']['next_month_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Next month color'),
    '#default_value' => $next_month_color,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_header']['next_month_fsize'] = array(
    '#type' => 'textfield',
    '#title' => t('Next month font size'),
    '#default_value' => $next_month_fsize,
    '#size' => 25,
  );
  $form['block_theme_header']['next_month_arrow_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Next month arrow color'),
    '#default_value' => $next_month_arrow_color,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_header']['prev_month_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous month color'),
    '#default_value' => $prev_month_color,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_header']['prev_month_fsize'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous month font size'),
    '#default_value' => $prev_month_fsize,
    '#size' => 25,
  );
  $form['block_theme_header']['prev_month_arrow_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous month arrow color'),
    '#default_value' => $prev_month_arrow_color,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_header']['arrow_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Arrow size'),
    '#default_value' => $arrow_size,
    '#size' => 25,
  );
  $form['block_theme_header']['weekdays_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Weekdays color'),
    '#default_value' => $weekdays_color,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_header']['week_cell_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Weekdays cell height'),
    '#default_value' => $week_cell_height,
    '#size' => 25,
  );
  $form['block_theme_header']['weekdays_bgcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Weekdays background color'),
    '#default_value' => $weekdays_bgcolor,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_header']['sunday_bgcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Sunday background color'),
    '#default_value' => $sunday_bgcolor,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_header']['weekdays_fsize'] = array(
    '#type' => 'textfield',
    '#title' => t('Weekdays font size'),
    '#default_value' => $weekdays_fsize,
    '#size' => 25,
  );
  
  
  
  $form['block_theme_popup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Event page parameters'),
    '#attributes' => array(
      'class' => array('spider_calendar_theme_popup_fielset'),
      'id' => 'theme_popup_par',
    ),
  );
  // $form['block_theme_popup']['date_format_popup'] = array(
    // '#type' => 'textfield',
    // '#title' => t('Event date format (w/d/m/y)'),
    // '#default_value' => $date_format_popup,
    // '#size' => 25,
  // );
  $form['block_theme_popup']['event_title_color_popup'] = array(
    '#type' => 'textfield',
    '#title' => t('Event title color'),
    '#default_value' => $event_title_color_popup,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_popup']['event_title_fsize_popup'] = array(
    '#type' => 'textfield',
    '#title' => t('Event title font size'),
    '#default_value' => $event_title_fsize_popup,
    '#size' => 25,
  );
  $fonts_array = array(
    'Arial' => t('Arial'),
    'Courier New' => t('Courier New'),
    'Georgia' => t('Georgia'),
    'Tahoma' => t('Tahoma'),
    'Verdana' => t('Verdana'),
    'Impact' => t('Impact'),
  );
  $form['block_theme_popup']['event_title_family_popup'] = array(
    '#type' => 'select',
    '#title' => t('Event title font family'),
    '#options' => $fonts_array,
    '#default_value' => $event_title_family_popup,
  );
  $fonts_style_array = array(
    'Normal' => t('Normal'),
    'Bold' => t('Bold'),
    'Italic' => t('Italic'),
    'Bold and Italic' => t('Bold and Italic'),
  );
  $form['block_theme_popup']['event_title_fstyle_popup'] = array(
    '#type' => 'select',
    '#title' => t('Event title font style'),
    '#options' => $fonts_style_array,
    '#default_value' => $event_title_fstyle_popup,
  );
  $form['block_theme_popup']['date_color_popup'] = array(
    '#type' => 'textfield',
    '#title' => t('Date color'),
    '#default_value' => $date_color_popup,
    '#size' => 25,
    '#attributes' => array('class' => array('color')),
  );
  $form['block_theme_popup']['date_fsize_popup'] = array(
    '#type' => 'textfield',
    '#title' => t('Date font size'),
    '#default_value' => $date_fsize_popup,
    '#size' => 25,
  );
  $form['block_theme_popup']['date_family_popup'] = array(
    '#type' => 'select',
    '#title' => t('Date font family'),
    '#options' => $fonts_array,
    '#default_value' => $date_family_popup,
  );
  $form['block_theme_popup']['date_fstyle_popup'] = array(
    '#type' => 'select',
    '#title' => t('Date font style'),
    '#options' => $fonts_style_array,
    '#default_value' => $date_fstyle_popup,
  );
  // $form['block_theme_popup']['arrow_bgcolor_popup'] = array(
    // '#type' => 'textfield',
    // '#title' => t('Arrow background color in popup'),
    // '#default_value' => $arrow_bgcolor_popup,
    // '#size' => 25,
    // '#attributes' => array('class' => array('color')),
  // );
  // $form['block_theme_popup']['arrow_color_popup'] = array(
    // '#type' => 'textfield',
    // '#title' => t('Arrow color in popup'),
    // '#default_value' => $arrow_color_popup,
    // '#size' => 25,
    // '#attributes' => array('class' => array('color')),
  // );
  // $form['block_theme_popup']['popup_bgcolor'] = array(
    // '#type' => 'textfield',
    // '#title' => t('Popup background color'),
    // '#default_value' => $popup_bgcolor,
    // '#size' => 25,
    // '#attributes' => array('class' => array('color')),
  // );
  // $form['block_theme_popup']['popup_width'] = array(
    // '#type' => 'textfield',
    // '#title' => t('Popup width'),
    // '#default_value' => $popup_width,
    // '#size' => 25,
  // );
  // $form['block_theme_popup']['popup_height'] = array(
    // '#type' => 'textfield',
    // '#title' => t('Popup height'),
    // '#default_value' => $popup_height,
    // '#size' => 25,
  // );
  // $form['block_theme_popup']['displaied_events'] = array(
    // '#type' => 'textfield',
    // '#title' => t('Number of displayed events'),
    // '#default_value' => $displaied_events,
    // '#size' => 25,
  // );
  $form['block_theme_popup']['repeat_rate'] = array(
    '#type' => 'radios',
    '#title' => t('Show the repeat rate'),
    '#default_value' => $repeat_rate,
    '#options' => array('1' => t('Yes'), '0' => t('No')),
  );
  drupal_add_js(drupal_get_path('module', 'spider_calendar') . '/js/spider_calendar_sliding_popup.js');
  $form['block_theme_save'] = array(
    '#type' => 'submit',
    '#prefix' => '<div style="float:left; bottom:0px; clear:left;">                    
                    <input type="button" value="' . t('Preview') . '" class="form-submit" onclick=\'if (isPopUpOpened) {return;} spider_calendar_createpopup("' . url('admin/settings/spider_calendar/Block Calendar Themes/add/preview_popup', array('absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?580:jQuery(window).height() - 70, 1, "testpopup", 5);\'></input>',
    '#value' => t('Save'),
    '#submit' => array('spider_calendar_block_theme_save'),
  );
  $form['block_theme_apply'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('spider_calendar_block_theme_apply'),
  );
  $form['block_theme_cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('spider_calendar_block_theme_cancel'),
    '#suffix' => '</div>',
  );
  return $form;
}

/**
 * Save block theme.
 */
function spider_calendar_block_theme_save($form, &$form_state) {
  if (isset($_GET['block_theme_id'])) {
    $theme_id = check_plain($_GET['block_theme_id']);
    db_query("UPDATE {spider_calendar_block_theme_table} SET title=:title,theme_width=:theme_width,week_start_day=:week_start_day,border_color=:border_color,border_radius=:border_radius,border_width=:border_width,show_titles_cloud=:show_titles_cloud,show_date_and_time_in_titlescloud=:show_date_and_time_in_titlescloud,titles_cloud_text_color=:titles_cloud_text_color,body_bgcolor=:body_bgcolor,cell_height=:cell_height,omd_fcolor=:omd_fcolor,omd_bgcolor=:omd_bgcolor,cell_text_color_wevents=:cell_text_color_wevents,cell_text_color_events=:cell_text_color_events,cell_text_color_selected_events=:cell_text_color_selected_events,event_title_color=:event_title_color,cell_bgcolor_events=:cell_bgcolor_events,cell_bgcolor_selected_events=:cell_bgcolor_selected_events,cur_day_cell_border_color=:cur_day_cell_border_color,selected_day_cell_border_color=:selected_day_cell_border_color,cell_border_color=:cell_border_color,all_days_border=:all_days_border,sundays_tcolor=:sundays_tcolor,sundays_cell_bgcolor=:sundays_cell_bgcolor,sundays_fcolor=:sundays_fcolor,days_fsize=:days_fsize,time_in_cell=:time_in_cell,header_heidht=:header_heidht,header_bgcolor=:header_bgcolor,year_font_size=:year_font_size,year_color=:year_color,year_arrow_color=:year_arrow_color,month_type=:month_type,cur_month_color=:cur_month_color,cur_month_fsize=:cur_month_fsize,month_arrow_color=:month_arrow_color,next_month_color=:next_month_color,next_month_fsize=:next_month_fsize,next_month_arrow_color=:next_month_arrow_color,prev_month_color=:prev_month_color,prev_month_fsize=:prev_month_fsize,prev_month_arrow_color=:prev_month_arrow_color,arrow_size=:arrow_size,weekdays_color=:weekdays_color,week_cell_height=:week_cell_height,weekdays_bgcolor=:weekdays_bgcolor,sunday_bgcolor=:sunday_bgcolor,weekdays_fsize=:weekdays_fsize,date_format_popup=:date_format_popup,event_title_color_popup=:event_title_color_popup,event_title_fsize_popup=:event_title_fsize_popup,event_title_family_popup=:event_title_family_popup,event_title_fstyle_popup=:event_title_fstyle_popup,date_color_popup=:date_color_popup,date_fsize_popup=:date_fsize_popup,date_family_popup=:date_family_popup,date_fstyle_popup=:date_fstyle_popup,arrow_bgcolor_popup=:arrow_bgcolor_popup,arrow_color_popup=:arrow_color_popup,popup_bgcolor=:popup_bgcolor,popup_width=:popup_width,popup_height=:popup_height,displaied_events=:displaied_events,repeat_rate=:repeat_rate WHERE id=:id", array(
      ':title' => $form_state['values']['theme_title'],
      ':theme_width' => $form_state['values']['theme_width'],
      ':week_start_day' => $form_state['values']['week_start_day'],
      ':border_color' => $form_state['values']['border_color'],
      ':border_radius' => $form_state['values']['border_radius'],
      ':border_width' => $form_state['values']['border_width'],
      ':show_titles_cloud' => $form_state['values']['show_titles_cloud'],
      ':show_date_and_time_in_titlescloud' => $form_state['values']['show_date_and_time_in_titlescloud'],
      ':titles_cloud_text_color' => $form_state['values']['titles_cloud_text_color'],
      ':body_bgcolor' => $form_state['values']['body_bgcolor'],
      ':cell_height' => $form_state['values']['cell_height'],
      ':omd_fcolor' => $form_state['values']['omd_fcolor'],
      ':omd_bgcolor' => $form_state['values']['omd_bgcolor'],
      ':cell_text_color_wevents' => $form_state['values']['cell_text_color_wevents'],
      ':cell_text_color_events' => $form_state['values']['cell_text_color_events'],
      ':cell_text_color_selected_events' => $form_state['values']['cell_text_color_selected_events'],
      ':event_title_color' => $form_state['values']['event_title_color'],
      ':cell_bgcolor_events' => $form_state['values']['cell_bgcolor_events'],
      ':cell_bgcolor_selected_events' => $form_state['values']['cell_bgcolor_selected_events'],
      ':cur_day_cell_border_color' => $form_state['values']['cur_day_cell_border_color'],
      ':selected_day_cell_border_color' => $form_state['values']['selected_day_cell_border_color'],
      ':cell_border_color' => $form_state['values']['cell_border_color'],
      ':all_days_border' => $form_state['values']['all_days_border'],
      ':sundays_tcolor' => $form_state['values']['sundays_tcolor'],
      ':sundays_cell_bgcolor' => $form_state['values']['sundays_cell_bgcolor'],
      ':sundays_fcolor' => $form_state['values']['sundays_fcolor'],
      ':days_fsize' => $form_state['values']['days_fsize'],
      ':time_in_cell' => $form_state['values']['time_in_cell'],
      ':header_heidht' => $form_state['values']['header_heidht'],
      ':header_bgcolor' => $form_state['values']['header_bgcolor'],
      ':year_font_size' => $form_state['values']['year_font_size'],
      ':year_color' => $form_state['values']['year_color'],
      ':year_arrow_color' => $form_state['values']['year_arrow_color'],
      ':month_type' => $form_state['values']['month_type'],
      ':cur_month_color' => $form_state['values']['cur_month_color'],
      ':cur_month_fsize' => $form_state['values']['cur_month_fsize'],
      ':month_arrow_color' => $form_state['values']['month_arrow_color'],
      ':next_month_color' => $form_state['values']['next_month_color'],
      ':next_month_fsize' => $form_state['values']['next_month_fsize'],
      ':next_month_arrow_color' => $form_state['values']['next_month_arrow_color'],
      ':prev_month_color' => $form_state['values']['prev_month_color'],
      ':prev_month_fsize' => $form_state['values']['prev_month_fsize'],
      ':prev_month_arrow_color' => $form_state['values']['prev_month_arrow_color'],
      ':arrow_size' => $form_state['values']['arrow_size'],
      ':weekdays_color' => $form_state['values']['weekdays_color'],
      ':week_cell_height' => $form_state['values']['week_cell_height'],
      ':weekdays_bgcolor' => $form_state['values']['weekdays_bgcolor'],
      ':sunday_bgcolor' => $form_state['values']['sunday_bgcolor'],
      ':weekdays_fsize' => $form_state['values']['weekdays_fsize'],
      ':date_format_popup' => '',
      ':event_title_color_popup' => $form_state['values']['event_title_color_popup'],
      ':event_title_fsize_popup' => $form_state['values']['event_title_fsize_popup'],
      ':event_title_family_popup' => $form_state['values']['event_title_family_popup'],
      ':event_title_fstyle_popup' => $form_state['values']['event_title_fstyle_popup'],
      ':date_color_popup' => $form_state['values']['date_color_popup'],
      ':date_fsize_popup' => $form_state['values']['date_fsize_popup'],
      ':date_family_popup' => $form_state['values']['date_family_popup'],
      ':date_fstyle_popup' => $form_state['values']['date_fstyle_popup'],
      ':arrow_bgcolor_popup' => '',
      ':arrow_color_popup' => '',
      ':popup_bgcolor' => '',
      ':popup_width' => '',
      ':popup_height' => '',
      ':displaied_events' => '',
      ':repeat_rate' => $form_state['values']['repeat_rate'],
      ':id' => $theme_id));
    drupal_set_message(t('Your theme successfully updated.'), 'status', FALSE);
  }
  else {
    db_insert('spider_calendar_block_theme_table')
      ->fields(array(
      'title' => $form_state['values']['theme_title'],
      'theme_width' => $form_state['values']['theme_width'],
      'week_start_day' => $form_state['values']['week_start_day'],
      'border_color' => $form_state['values']['border_color'],
      'border_radius' => $form_state['values']['border_radius'],
      'border_width' => $form_state['values']['border_width'],
      'show_titles_cloud' => $form_state['values']['show_titles_cloud'],
      'show_date_and_time_in_titlescloud' => $form_state['values']['show_date_and_time_in_titlescloud'],
      'titles_cloud_text_color' => $form_state['values']['titles_cloud_text_color'],
      'body_bgcolor' => $form_state['values']['body_bgcolor'],
      'cell_height' => $form_state['values']['cell_height'],
      'omd_fcolor' => $form_state['values']['omd_fcolor'],
      'omd_bgcolor' => $form_state['values']['omd_bgcolor'],
      'cell_text_color_wevents' => $form_state['values']['cell_text_color_wevents'],
      'cell_text_color_events' => $form_state['values']['cell_text_color_events'],
      'cell_text_color_selected_events' => $form_state['values']['cell_text_color_selected_events'],
      'event_title_color' => $form_state['values']['event_title_color'],
      'cell_bgcolor_events' => $form_state['values']['cell_bgcolor_events'],
      'cell_bgcolor_selected_events' => $form_state['values']['cell_bgcolor_selected_events'],
      'cur_day_cell_border_color' => $form_state['values']['cur_day_cell_border_color'],
      'selected_day_cell_border_color' => $form_state['values']['selected_day_cell_border_color'],
      'cell_border_color' => $form_state['values']['cell_border_color'],
      'all_days_border' => $form_state['values']['all_days_border'],
      'sundays_tcolor' => $form_state['values']['sundays_tcolor'],
      'sundays_cell_bgcolor' => $form_state['values']['sundays_cell_bgcolor'],
      'sundays_fcolor' => $form_state['values']['sundays_fcolor'],
      'days_fsize' => $form_state['values']['days_fsize'],
      'time_in_cell' => $form_state['values']['time_in_cell'],
      'header_heidht' => $form_state['values']['header_heidht'],
      'header_bgcolor' => $form_state['values']['header_bgcolor'],
      'year_font_size' => $form_state['values']['year_font_size'],
      'year_color' => $form_state['values']['year_color'],
      'year_arrow_color' => $form_state['values']['year_arrow_color'],
      'month_type' => $form_state['values']['month_type'],
      'cur_month_color' => $form_state['values']['cur_month_color'],
      'cur_month_fsize' => $form_state['values']['cur_month_fsize'],
      'month_arrow_color' => $form_state['values']['month_arrow_color'],
      'next_month_color' => $form_state['values']['next_month_color'],
      'next_month_fsize' => $form_state['values']['next_month_fsize'],
      'next_month_arrow_color' => $form_state['values']['next_month_arrow_color'],
      'prev_month_color' => $form_state['values']['prev_month_color'],
      'prev_month_fsize' => $form_state['values']['prev_month_fsize'],
      'prev_month_arrow_color' => $form_state['values']['prev_month_arrow_color'],
      'arrow_size' => $form_state['values']['arrow_size'],
      'weekdays_color' => $form_state['values']['weekdays_color'],
      'week_cell_height' => $form_state['values']['week_cell_height'],
      'weekdays_bgcolor' => $form_state['values']['weekdays_bgcolor'],
      'sunday_bgcolor' => $form_state['values']['sunday_bgcolor'],
      'weekdays_fsize' => $form_state['values']['weekdays_fsize'],
      'date_format_popup' => '',
      'event_title_color_popup' => $form_state['values']['event_title_color_popup'],
      'event_title_fsize_popup' => $form_state['values']['event_title_fsize_popup'],
      'event_title_family_popup' => $form_state['values']['event_title_family_popup'],
      'event_title_fstyle_popup' => $form_state['values']['event_title_fstyle_popup'],
      'date_color_popup' => $form_state['values']['date_color_popup'],
      'date_fsize_popup' => $form_state['values']['date_fsize_popup'],
      'date_family_popup' => $form_state['values']['date_family_popup'],
      'date_fstyle_popup' => $form_state['values']['date_fstyle_popup'],
      'arrow_bgcolor_popup' => '',
      'arrow_color_popup' => '',
      'popup_bgcolor' => '',
      'popup_width' => '',
      'popup_height' => '',
      'displaied_events' => '',
      'repeat_rate' => $form_state['values']['repeat_rate'],
      'default_theme' => 0,
      ))
      ->execute();
    drupal_set_message(t('Your theme successfully saved.'), 'status', FALSE);
  }
  $form_state['redirect'] = url('admin/settings/spider_calendar/Block Calendar Themes', array('absolute' => TRUE));
}

/**
 * Apply block theme.
 */
function spider_calendar_block_theme_apply($form, &$form_state) {
  if (isset($_GET['block_theme_id'])) {
    $theme_id = check_plain($_GET['block_theme_id']);
    db_query("UPDATE {spider_calendar_block_theme_table} SET title=:title,theme_width=:theme_width,week_start_day=:week_start_day,border_color=:border_color,border_radius=:border_radius,border_width=:border_width,show_titles_cloud=:show_titles_cloud,show_date_and_time_in_titlescloud=:show_date_and_time_in_titlescloud,titles_cloud_text_color=:titles_cloud_text_color,body_bgcolor=:body_bgcolor,cell_height=:cell_height,omd_fcolor=:omd_fcolor,omd_bgcolor=:omd_bgcolor,cell_text_color_wevents=:cell_text_color_wevents,cell_text_color_events=:cell_text_color_events,cell_text_color_selected_events=:cell_text_color_selected_events,event_title_color=:event_title_color,cell_bgcolor_events=:cell_bgcolor_events,cell_bgcolor_selected_events=:cell_bgcolor_selected_events,cur_day_cell_border_color=:cur_day_cell_border_color,selected_day_cell_border_color=:selected_day_cell_border_color,cell_border_color=:cell_border_color,all_days_border=:all_days_border,sundays_tcolor=:sundays_tcolor,sundays_cell_bgcolor=:sundays_cell_bgcolor,sundays_fcolor=:sundays_fcolor,days_fsize=:days_fsize,time_in_cell=:time_in_cell,header_heidht=:header_heidht,header_bgcolor=:header_bgcolor,year_font_size=:year_font_size,year_color=:year_color,year_arrow_color=:year_arrow_color,month_type=:month_type,cur_month_color=:cur_month_color,cur_month_fsize=:cur_month_fsize,month_arrow_color=:month_arrow_color,next_month_color=:next_month_color,next_month_fsize=:next_month_fsize,next_month_arrow_color=:next_month_arrow_color,prev_month_color=:prev_month_color,prev_month_fsize=:prev_month_fsize,prev_month_arrow_color=:prev_month_arrow_color,arrow_size=:arrow_size,weekdays_color=:weekdays_color,week_cell_height=:week_cell_height,weekdays_bgcolor=:weekdays_bgcolor,sunday_bgcolor=:sunday_bgcolor,weekdays_fsize=:weekdays_fsize,date_format_popup=:date_format_popup,event_title_color_popup=:event_title_color_popup,event_title_fsize_popup=:event_title_fsize_popup,event_title_family_popup=:event_title_family_popup,event_title_fstyle_popup=:event_title_fstyle_popup,date_color_popup=:date_color_popup,date_fsize_popup=:date_fsize_popup,date_family_popup=:date_family_popup,date_fstyle_popup=:date_fstyle_popup,arrow_bgcolor_popup=:arrow_bgcolor_popup,arrow_color_popup=:arrow_color_popup,popup_bgcolor=:popup_bgcolor,popup_width=:popup_width,popup_height=:popup_height,displaied_events=:displaied_events,repeat_rate=:repeat_rate WHERE id=:id", array(
      ':title' => $form_state['values']['theme_title'],
      ':theme_width' => $form_state['values']['theme_width'],
      ':week_start_day' => $form_state['values']['week_start_day'],
      ':border_color' => $form_state['values']['border_color'],
      ':border_radius' => $form_state['values']['border_radius'],
      ':border_width' => $form_state['values']['border_width'],
      ':show_titles_cloud' => $form_state['values']['show_titles_cloud'],
      ':show_date_and_time_in_titlescloud' => $form_state['values']['show_date_and_time_in_titlescloud'],
      ':titles_cloud_text_color' => $form_state['values']['titles_cloud_text_color'],
      ':body_bgcolor' => $form_state['values']['body_bgcolor'],
      ':cell_height' => $form_state['values']['cell_height'],
      ':omd_fcolor' => $form_state['values']['omd_fcolor'],
      ':omd_bgcolor' => $form_state['values']['omd_bgcolor'],
      ':cell_text_color_wevents' => $form_state['values']['cell_text_color_wevents'],
      ':cell_text_color_events' => $form_state['values']['cell_text_color_events'],
      ':cell_text_color_selected_events' => $form_state['values']['cell_text_color_selected_events'],
      ':event_title_color' => $form_state['values']['event_title_color'],
      ':cell_bgcolor_events' => $form_state['values']['cell_bgcolor_events'],
      ':cell_bgcolor_selected_events' => $form_state['values']['cell_bgcolor_selected_events'],
      ':cur_day_cell_border_color' => $form_state['values']['cur_day_cell_border_color'],
      ':selected_day_cell_border_color' => $form_state['values']['selected_day_cell_border_color'],
      ':cell_border_color' => $form_state['values']['cell_border_color'],
      ':all_days_border' => $form_state['values']['all_days_border'],
      ':sundays_tcolor' => $form_state['values']['sundays_tcolor'],
      ':sundays_cell_bgcolor' => $form_state['values']['sundays_cell_bgcolor'],
      ':sundays_fcolor' => $form_state['values']['sundays_fcolor'],
      ':days_fsize' => $form_state['values']['days_fsize'],
      ':time_in_cell' => $form_state['values']['time_in_cell'],
      ':header_heidht' => $form_state['values']['header_heidht'],
      ':header_bgcolor' => $form_state['values']['header_bgcolor'],
      ':year_font_size' => $form_state['values']['year_font_size'],
      ':year_color' => $form_state['values']['year_color'],
      ':year_arrow_color' => $form_state['values']['year_arrow_color'],
      ':month_type' => $form_state['values']['month_type'],
      ':cur_month_color' => $form_state['values']['cur_month_color'],
      ':cur_month_fsize' => $form_state['values']['cur_month_fsize'],
      ':month_arrow_color' => $form_state['values']['month_arrow_color'],
      ':next_month_color' => $form_state['values']['next_month_color'],
      ':next_month_fsize' => $form_state['values']['next_month_fsize'],
      ':next_month_arrow_color' => $form_state['values']['next_month_arrow_color'],
      ':prev_month_color' => $form_state['values']['prev_month_color'],
      ':prev_month_fsize' => $form_state['values']['prev_month_fsize'],
      ':prev_month_arrow_color' => $form_state['values']['prev_month_arrow_color'],
      ':arrow_size' => $form_state['values']['arrow_size'],
      ':weekdays_color' => $form_state['values']['weekdays_color'],
      ':week_cell_height' => $form_state['values']['week_cell_height'],
      ':weekdays_bgcolor' => $form_state['values']['weekdays_bgcolor'],
      ':sunday_bgcolor' => $form_state['values']['sunday_bgcolor'],
      ':weekdays_fsize' => $form_state['values']['weekdays_fsize'],
      ':date_format_popup' => '',
      ':event_title_color_popup' => $form_state['values']['event_title_color_popup'],
      ':event_title_fsize_popup' => $form_state['values']['event_title_fsize_popup'],
      ':event_title_family_popup' => $form_state['values']['event_title_family_popup'],
      ':event_title_fstyle_popup' => $form_state['values']['event_title_fstyle_popup'],
      ':date_color_popup' => $form_state['values']['date_color_popup'],
      ':date_fsize_popup' => $form_state['values']['date_fsize_popup'],
      ':date_family_popup' => $form_state['values']['date_family_popup'],
      ':date_fstyle_popup' => $form_state['values']['date_fstyle_popup'],
      ':arrow_bgcolor_popup' => '',
      ':arrow_color_popup' => '',
      ':popup_bgcolor' => '',
      ':popup_width' => '',
      ':popup_height' => '',
      ':displaied_events' => '',
      ':repeat_rate' => $form_state['values']['repeat_rate'],
      ':id' => $theme_id));
    drupal_set_message(t('Your theme successfully updated.'), 'status', FALSE);
  }
  else {
    db_insert('spider_calendar_block_theme_table')
      ->fields(array(
      'title' => $form_state['values']['theme_title'],
      'theme_width' => $form_state['values']['theme_width'],
      'week_start_day' => $form_state['values']['week_start_day'],
      'border_color' => $form_state['values']['border_color'],
      'border_radius' => $form_state['values']['border_radius'],
      'border_width' => $form_state['values']['border_width'],
      'show_titles_cloud' => $form_state['values']['show_titles_cloud'],
      'show_date_and_time_in_titlescloud' => $form_state['values']['show_date_and_time_in_titlescloud'],
      'titles_cloud_text_color' => $form_state['values']['titles_cloud_text_color'],
      'body_bgcolor' => $form_state['values']['body_bgcolor'],
      'cell_height' => $form_state['values']['cell_height'],
      'omd_fcolor' => $form_state['values']['omd_fcolor'],
      'omd_bgcolor' => $form_state['values']['omd_bgcolor'],
      'cell_text_color_wevents' => $form_state['values']['cell_text_color_wevents'],
      'cell_text_color_events' => $form_state['values']['cell_text_color_events'],
      'cell_text_color_selected_events' => $form_state['values']['cell_text_color_selected_events'],
      'event_title_color' => $form_state['values']['event_title_color'],
      'cell_bgcolor_events' => $form_state['values']['cell_bgcolor_events'],
      'cell_bgcolor_selected_events' => $form_state['values']['cell_bgcolor_selected_events'],
      'cur_day_cell_border_color' => $form_state['values']['cur_day_cell_border_color'],
      'selected_day_cell_border_color' => $form_state['values']['selected_day_cell_border_color'],
      'cell_border_color' => $form_state['values']['cell_border_color'],
      'all_days_border' => $form_state['values']['all_days_border'],
      'sundays_tcolor' => $form_state['values']['sundays_tcolor'],
      'sundays_cell_bgcolor' => $form_state['values']['sundays_cell_bgcolor'],
      'sundays_fcolor' => $form_state['values']['sundays_fcolor'],
      'days_fsize' => $form_state['values']['days_fsize'],
      'time_in_cell' => $form_state['values']['time_in_cell'],
      'header_heidht' => $form_state['values']['header_heidht'],
      'header_bgcolor' => $form_state['values']['header_bgcolor'],
      'year_font_size' => $form_state['values']['year_font_size'],
      'year_color' => $form_state['values']['year_color'],
      'year_arrow_color' => $form_state['values']['year_arrow_color'],
      'month_type' => $form_state['values']['month_type'],
      'cur_month_color' => $form_state['values']['cur_month_color'],
      'cur_month_fsize' => $form_state['values']['cur_month_fsize'],
      'month_arrow_color' => $form_state['values']['month_arrow_color'],
      'next_month_color' => $form_state['values']['next_month_color'],
      'next_month_fsize' => $form_state['values']['next_month_fsize'],
      'next_month_arrow_color' => $form_state['values']['next_month_arrow_color'],
      'prev_month_color' => $form_state['values']['prev_month_color'],
      'prev_month_fsize' => $form_state['values']['prev_month_fsize'],
      'prev_month_arrow_color' => $form_state['values']['prev_month_arrow_color'],
      'arrow_size' => $form_state['values']['arrow_size'],
      'weekdays_color' => $form_state['values']['weekdays_color'],
      'week_cell_height' => $form_state['values']['week_cell_height'],
      'weekdays_bgcolor' => $form_state['values']['weekdays_bgcolor'],
      'sunday_bgcolor' => $form_state['values']['sunday_bgcolor'],
      'weekdays_fsize' => $form_state['values']['weekdays_fsize'],
      'date_format_popup' => '',
      'event_title_color_popup' => $form_state['values']['event_title_color_popup'],
      'event_title_fsize_popup' => $form_state['values']['event_title_fsize_popup'],
      'event_title_family_popup' => $form_state['values']['event_title_family_popup'],
      'event_title_fstyle_popup' => $form_state['values']['event_title_fstyle_popup'],
      'date_color_popup' => $form_state['values']['date_color_popup'],
      'date_fsize_popup' => $form_state['values']['date_fsize_popup'],
      'date_family_popup' => $form_state['values']['date_family_popup'],
      'date_fstyle_popup' => $form_state['values']['date_fstyle_popup'],
      'arrow_bgcolor_popup' => '',
      'arrow_color_popup' => '',
      'popup_bgcolor' => '',
      'popup_width' => '',
      'popup_height' => '',
      'displaied_events' => '',
      'repeat_rate' => $form_state['values']['repeat_rate'],
      'default_theme' => 0,
      ))
      ->execute();
    $theme_id = db_query("SELECT MAX(id) FROM {spider_calendar_block_theme_table}")->fetchField();
    drupal_set_message(t('Your theme successfully saved.'), 'status', FALSE);
  }
  $form_state['redirect'] = url('admin/settings/spider_calendar/Block Calendar Themes/add', array('query' => array('block_theme_id' => $theme_id), 'absolute' => TRUE));
}

/**
 * Cancel the block save.
 */
function spider_calendar_block_theme_cancel($form, &$form_state) {
  $form_state['redirect'] = url('admin/settings/spider_calendar/Block Calendar Themes', array('absolute' => TRUE));
}
