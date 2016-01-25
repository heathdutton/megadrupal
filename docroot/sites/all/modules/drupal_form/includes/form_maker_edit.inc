<?php
/**
 * @file
 * Form maker edit page.
 */

/**
 * Edit form.
 */
function form_maker_edit() {
  header("X-XSS-Protection: 0");
  // drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_calendar.js');
  // drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_calendar-setup.js');
  // drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_cal_function.js');
  drupal_add_js('var already_submited = false;', array('type' => 'inline'));
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_preview_page_nav.js');
  $external_js = 'http://maps.google.com/maps/api/js?sensor=false';
  drupal_add_js('document.write(unescape("%3Cscript src=\'' . $external_js . '\' type=\'text/javascript\'%3E%3C/script%3E"));', array('type' => 'inline'));
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_if_gmap.js');
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_save_edit.js');
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_cal.js');
  drupal_add_css(drupal_get_path('module', 'form_maker') . '/css/form_maker_cal.css');
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  $row = db_query("SELECT * FROM {form_maker_table} WHERE vid=:vid", array(':vid' => $nodeid))->fetchObject();
  $form['fieldset_form_edit'] = array(
    '#prefix' => '<table width="95%" rules="none" style="border:none;">
                    <tr>	
                      <td style="width: 100%; text-align:right;font-size:16px; padding:20px; padding-right:50px; right:0; border:none;">
                        <a href="http://web-dorado.com/products/drupal-form-builder.html" target="_blank" style="color:red; text-decoration:none;">
                          <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/header.png" border="0" alt="www.web-dorado.com" width="215"><br>
                        Get the full version&nbsp;&nbsp;&nbsp;&nbsp;
                        </a>
                      </td>
                    </tr>
                  </table>
                  <div class="messages warning">' . t('Please save the form before leaving the tab.') . '</div>',
    '#type' => 'fieldset',
    '#title' => t('Form') . ' - ' . $row->title,
  );
  $form['fieldset_form_edit']['fieldset_form_edit_buttons'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('fieldset')),
  );
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_sliding_popup.js');
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_ajax.js');
  drupal_add_js(array(
    'form_maker' => array(
      'preview_url' => url('node/' . $nodeid . '/form_maker/preview', array('absolute' => TRUE)),
      ),
    ),
    'setting');
  $form['fieldset_form_edit']['fieldset_form_edit_buttons']['form_save'] = array(
    '#prefix' => '<div style="float:left; bottom:0px; clear:left;">                    
                    <input type="button" value="' . t('Preview') . '" class="form-submit form_maker_preview_button" onclick=\'if (isPopUpOpened) {return;} form_maker_createpopup("' . url('node/' . $nodeid . '/form_maker/preview', array('absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?580:jQuery(window).height() - 70, 1, "testpopup", 5);\'></input>',
    '#type' => 'submit',
    '#submit' => array('form_maker_form_save'),
    '#attributes' => array('onclick' => 'if (!already_submited) { form_maker_submitbutton("Save")} else {return false;}'),
    '#value' => t('Save'),
  );
  $form['fieldset_form_edit']['fieldset_form_edit_buttons']['form_apply'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_form_apply'),
    '#attributes' => array('onclick' => 'if (!already_submited) { form_maker_submitbutton("Save")} else {return false;}'),
    '#value' => t('Apply'),
  );
  $form['fieldset_form_edit']['fieldset_form_edit_buttons']['form_cancel'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_form_cancel'),
    '#value' => t('Cancel'),
    '#suffix' => '</div><a href="http://web-dorado.com/drupal-form-maker-guide-1.html" target="_blank" style="float:right;"><img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/user-manual.png" border="0" alt="' . t('User Manual') . '"></a>',
  );
  require_once "form_maker_add_field.inc";
  $form['fieldset_form_edit']['fieldset_form_edit_add_field'] = array(
    '#prefix' => form_maker_add_field(),
    '#suffix' => '
                <fieldset style="border-color:#00aeef; padding:0 15px 0 15px;">
                  <legend style="color:#00aeef;"><h2>' . t('Form') . '</h2></legend>
                  <style>
                    .wdform_table1 {
                      font-size:14px;
                      font-weight:normal;
                      color:#000000;
                      width:100%;
                    }
                    .wdform_tbody1 {
                      float:left;
                    }
                    .wdform_table2 {
                      padding-right:50px !important;
                      float:left;
                      border-spacing: 0px;
                      border-collapse:separate !important;
                      width:inherit;
                    }
                    .time_box {
                      border-width:1px;
                      margin: 0px;
                      padding: 0px;
                      text-align:right;
                      width:30px;
                      vertical-align:middle
                    }
                    .mini_label {
                      font-size:10px;
                      font-family: "Lucida Grande", Tahoma, Arial, Verdana, sans-serif;
                    }
                    .ch_rad_label {
                      display:inline;
                      margin-left:5px;
                      margin-right:15px;
                      float:none;
                    }
                    .label {
                      border:none;
                    }
                    .td_am_pm_select {
                      padding-left:5;
                    }
                    .am_pm_select {
                      height: 16px;
                      margin:0;
                      padding:0
                    }
                    .input_deactive {
                      color:#999999;
                      font-style:italic;
                      border-width:1px;
                      margin: 0px;
                      padding: 0px
                    }
                    .input_active {
                      color:#000000;
                      font-style:normal;
                      border-width:1px;
                      margin: 0px;
                      padding: 0px
                    }
                    .required {
                      border:none;
                      color:red
                    }
                    .captcha_img {
                      border-width:0px;
                      margin: 0px;
                      padding: 0px;
                      cursor:pointer;
                    }
                    .captcha_refresh {
                      width:30px;
                      height:30px;
                      border-width:0px;
                      margin: 0px;
                      padding: 0px;
                      vertical-align:middle;
                      cursor:pointer;
                      background-image: url("' . base_path() . drupal_get_path('module', 'form_maker') . '/images/refresh_black.png");
                    }
                    .captcha_input {
                      height:20px;
                      border-width:1px;
                      margin: 0px;
                      padding: 0px;
                      vertical-align:middle;
                    }
                    .file_upload {
                      border-width:1px;
                      margin: 0px;
                      padding: 0px
                    }
                    .page_deactive {
                      border:1px solid black;
                      padding:4px 7px 4px 7px;
                      margin:4px;
                      cursor:pointer;
                      background-color:#DBDBDB;
                    }
                    .page_active {
                      border:1px solid black;
                      padding:4px 7px 4px 7px;
                      margin:4px;
                      cursor:pointer;
                      background-color:#878787;
                    }
                    .page_percentage_active {
                      padding:0px;
                      margin:0px;
                      border-spacing: 0px;
                      height:30px;
                      line-height:30px;
                      background-color:yellow;
                      border-radius:30px;
                      font-size:15px;
                      float:left;
                      text-align: right !important;
                    }
                    .page_percentage_deactive {
                      height:30px;
                      line-height:30px;
                      padding:5px;
                      border:1px solid black;
                      width:100%;
                      background-color:white;
                      border-radius:30px;
                      text-align: left !important;
                    }
                    .page_numbers {
                      font-size:11px;
                    }
                    .phone_area_code {
                      width:50px;
                    }
                    .phone_number {
                      width:100px;
                    }
                  </style>
                  <table width="100%" style="margin:8px" class="form_maker_table">
                    <tr id="page_navigation">
                      <td align="center" width="90%" id="pages" show_title="' . $row->show_title . '" show_numbers="' . $row->show_numbers . '" type="' . $row->pagination . '">
                      <td align="left" id="edit_page_navigation"></td>
                    </tr>
                  </table>
                  <div id="take" class="main">
                  ' . (($row->form) ? $row->form : '<table cellpadding="4" cellspacing="0" class="wdform_table1 form_maker_table" style="border-top:0px solid black;">
                      <tbody id="form_id_tempform_view1" class="wdform_tbody1" page_title="Untitled page" next_title="Next" next_type="button" next_class="wdform_page_button" next_checkable="false" previous_title="Previous" previous_type="button" previous_class="wdform_page_button" previous_checkable="false">
                        <tr class="wdform_tr1">
                          <td class="wdform_td1">
                            <table class="wdform_table2 form_maker_table">
                              <tbody class="wdform_tbody2"></tbody>
                            </table>
                          </td>
                        </tr>
                        <tr class="wdform_footer">
                          <td colspan="100" valign="top">
                            <table width="100%" style="padding-right:170px" class="form_maker_table">
                              <tbody>
                                <tr id="form_id_temppage_nav1"></tr>
                              </tbody>
                            </table>
                          </td>
                        </tr>
                      </tbody>
                      <tbody id="form_id_tempform_view_img1" style="float:right; background-color: rgb(0, 0, 0, 0);">
                        <tr>
                          <td width="0%"></td>
                          <td align="right">
                            <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/minus.png" title="Show or hide the page" class="page_toolbar" onclick=\'show_or_hide("1")\' id="show_page_img_1" />
                          </td>
                          <td>
                            <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/page_delete.png" title="Delete the page"  class="page_toolbar" onclick=\'remove_page("1")\' />
                          </td>
                          <td>
                            <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/page_delete_all.png" title="Delete the page with fields"  class="page_toolbar" onclick=\'remove_page_all("1")\' />
                          </td>
                          <td>
                            <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/page_edit.png" title="Edit the page"  class="page_toolbar" onclick=\'edit_page_break("1")\' />
                          </td>
                        </tr>
                      </tbody>
                    </table>') . '
                  </div>
                  <input type="hidden" name="form_front" id="form_front" />
                  <input type="hidden" name="form" id="form" />
                  <input type="hidden" name="counter" id="counter" value="' . $row->counter . '">
                  <input type="hidden" name="pagination" id="pagination" />
                  <input type="hidden" name="show_title" id="show_title" />
                  <input type="hidden" name="show_numbers" id="show_numbers" />
                  <input type="hidden" name="public_key" id="public_key" value=""/>
                  <input type="hidden" name="private_key" id="private_key" value=""/>
                  <input type="hidden" name="recaptcha_theme" id="recaptcha_theme" />
                  <input type="hidden" name="javascript" id="javascript" value="' . $row->javascript . '" />
                  <input type="hidden" name="label_order" id="label_order" value="' . $row->label_order . '"/>
                  <input type="hidden" name="option" value="com_formmaker" />
                  <input type="hidden" name="task" value="" />
                  <input type="hidden" name="selected_theme_css" id="selected_theme_css" value="" />
                </fieldset>',
  );
  $form['#id'] = 'all_Form_Maker';
  $form['#attributes'] = array('name' => 'all_Form_Maker');
  return $form;
}

/**
 * Action after submission.
 */
function form_maker_action_after_submission($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  $row = db_query("SELECT * FROM {form_maker_table} WHERE vid=:vid", array(':vid' => $nodeid))->fetchObject();
  if ($row->redirect_url == '') {
    $redirect[0] = 'stay_on_form';
    $stay_on_form = '';
    $article = '';
    $custom_text = '';
    $url = '';
    drupal_add_js('form_maker_change_action_type("edit-action-after-submission-radios-stay-on-form");', array('type' => 'inline', 'scope' => 'footer'));
  }
  else {
    $redirect = explode('#****#', $row->redirect_url);
    switch ($redirect[0]) {
      case 'stay_on_form':
        $stay_on_form = '';
        $article = '';
        $custom_text = '';
        $url = '';
        drupal_add_js('form_maker_change_action_type("edit-action-after-submission-radios-stay-on-form");', array('type' => 'inline', 'scope' => 'footer'));
        break;

      case 'article':
        $stay_on_form = '';
        $article = $redirect[1];
        $custom_text = '';
        $url = '';
        drupal_add_js('form_maker_change_action_type("edit-action-after-submission-radios-article");', array('type' => 'inline', 'scope' => 'footer'));
        break;

      case 'custom_text':
        $stay_on_form = '';
        $article = '';
        $custom_text = $redirect[1];
        $url = '';
        drupal_add_js('form_maker_change_action_type("edit-action-after-submission-radios-custom-text");', array('type' => 'inline', 'scope' => 'footer'));
        break;

      case 'url':
        $stay_on_form = '';
        $article = '';
        $custom_text = '';
        $url = $redirect[1];
        drupal_add_js('form_maker_change_action_type("edit-action-after-submission-radios-url");', array('type' => 'inline', 'scope' => 'footer'));
        break;
    }
  }
  $form_state['redirect'] = url('node/' . $nodeid . '/form_maker/action_after_submission', array('absolute' => TRUE));
  $form['fieldset_form_action_after_submission'] = array(
    '#type' => 'fieldset',
  );
  $form['fieldset_form_action_after_submission']['form_action_after_submission_save'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_action_after_submission_save'),
    '#value' => t('Save'),
  );
  $form['fieldset_form_action_after_submission']['form_action_after_submission_apply'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_action_after_submission_apply'),
    '#value' => t('Apply'),
  );
  $form['fieldset_form_action_after_submission']['form_action_after_submission_cancel'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_javascript_cancel'),
    '#value' => t('Cancel'),
  );
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_change_action_type.js');
  $options = array(
    'stay_on_form' => t('Stay on form'),
    'article' => t('Article'),
    'custom_text' => t('Custom text'),
    'url' => t('URL'),
  );
  $form['fieldset_form_action_after_submission']['action_after_submission_radios'] = array(
    '#type' => 'radios',
    '#title' => t('Action type'),
    '#options' => $options,
    '#attributes' => array('onmouseup' => 'form_maker_change_action_type(this.id);'),
    '#default_value' => $redirect[0],
  );
  $form['fieldset_form_action_after_submission']['action_after_submission_article'] = array(
    '#type' => 'fieldset',
    '#title' => t('Article'),
    '#attributes' => array('id' => 'action_after_submission_article'),
  );
  $nodes_ids = db_query("SELECT nid FROM {node}")->fetchCol();
  foreach ($nodes_ids as $node_id) {
    $node_title = db_query("SELECT title FROM {node} WHERE nid=:nid", array(':nid' => $node_id))->fetchField();
    $select_options[$node_id] = $node_title;
  }
  $form['fieldset_form_action_after_submission']['action_after_submission_article']['select_article'] = array(
    '#type' => 'select',
    '#options' => $select_options,
    '#default_value' => $article,
  );
  $form['fieldset_form_action_after_submission']['action_after_submission_custom_text'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom text'),
    '#attributes' => array('id' => 'action_after_submission_custom_text'),
  );
  $editor_warning = '<div style="position:relative; background-color:#FEF5F1; color:#8C2E0B; border-color:#ED541D; padding:2px; border:1px solid #DD7777;">
                      ' . t('To show HTML editor download \'tinymce\' library from') . ' ' . l(t('http://github.com/downloads/tinymce/tinymce/tinymce_3.5.7.zip'), 'http://github.com/downloads/tinymce/tinymce/tinymce_3.5.7.zip') . ' ' . t('and extract it to sites/all/libraries/tinymce directory.') . '
                     </div>';
  $form['fieldset_form_action_after_submission']['action_after_submission_custom_text']['custom_text'] = array(
    '#prefix' => file_exists("sites/all/libraries/tinymce/jscripts/tiny_mce/tiny_mce.js") ? '' : $editor_warning,
    '#type' => 'textarea',
    '#title' => t('Text'),
    '#cols' => 100,
    '#attributes' => array('class' => array('form_maker_editor')),
    '#default_value' => $custom_text,
    '#resizable' => FALSE,
  );
  $form['fieldset_form_action_after_submission']['action_after_submission_url'] = array(
    '#type' => 'fieldset',
    '#title' => t('URL'),
    '#attributes' => array('id' => 'action_after_submission_url'),
  );
  $form['fieldset_form_action_after_submission']['action_after_submission_url']['url'] = array(
    '#prefix' => file_exists("sites/all/libraries/tinymce/jscripts/tiny_mce/tiny_mce.js") ? '' : $editor_warning,
    '#type' => 'textfield',
    '#default_value' => $url,
  );
  $form['fieldset_form_action_after_submission']['action_value'] = array(
    '#type' => 'hidden',
    '#attributes' => array('id' => 'form_maker_action_value'),
  );
  return $form;
}

/**
 * Save action type.
 */
function form_maker_action_after_submission_save($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  switch ($form_state['values']['action_value']) {
    case 'stay_on_form':
      $redirect_url = 'stay_on_form#****#';
      break;

    case 'article':
      $redirect_url = 'article#****#' . $form_state['values']['select_article'];
      break;

    case 'custom_text':
      $redirect_url = 'custom_text#****#' . $form_state['values']['custom_text'];
      break;

    case 'url':
      $redirect_url = 'url#****#' . $form_state['values']['url'];
      break;
  }
  db_query("UPDATE {form_maker_table} SET redirect_url=:redirect_url WHERE vid=:vid", array(
    ':redirect_url' => $redirect_url,
    ':vid' => $nodeid,
    ));
  $form_state['redirect'] = url('node/' . $nodeid . '/form_maker', array('absolute' => TRUE));
  drupal_set_message(t('Action type successfully updated.'), 'status', FALSE);
}

/**
 * Apply action type.
 */
function form_maker_action_after_submission_apply($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  switch ($form_state['values']['action_value']) {
    case 'stay_on_form':
      $redirect_url = 'stay_on_form#****#';
      break;

    case 'article':
      $redirect_url = 'article#****#' . $form_state['values']['select_article'];
      break;

    case 'custom_text':
      $redirect_url = 'custom_text#****#' . $form_state['values']['custom_text'];
      break;

    case 'url':
      $redirect_url = 'url#****#' . $form_state['values']['url'];
      break;
  }
  db_query("UPDATE {form_maker_table} SET redirect_url=:redirect_url WHERE vid=:vid", array(
    ':redirect_url' => $redirect_url,
    ':vid' => $nodeid,
    ));
  drupal_set_message(t('Action type successfully updated.'), 'status', FALSE);
}

/**
 * Edit JavaScript.
 */
function form_maker_edit_javascript() {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  $form_state['redirect'] = url('node/' . $nodeid . '/form_maker/edit_javascript', array('absolute' => TRUE));
  $row = db_query("SELECT * FROM {form_maker_table} WHERE vid=:vid", array(':vid' => $nodeid))->fetchObject();
  $form['fieldset_form_edit_javascript'] = array(
    '#type' => 'fieldset',
    '#title' => t('Edit JavaScript') . ' - ' . $row->title,
  );
  $form['fieldset_form_edit_javascript']['form_javascript_save'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_javascript_save'),
    '#value' => t('Save'),
  );
  $form['fieldset_form_edit_javascript']['form_javascript_apply'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_javascript_apply'),
    '#value' => t('Apply'),
  );
  $form['fieldset_form_edit_javascript']['form_javascript_cancel'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_javascript_cancel'),
    '#value' => t('Cancel'),
  );
  $form['fieldset_form_edit_javascript']['javascript_textarea'] = array(
    '#type' => 'textarea',
    '#title' => t('Javascript'),
    '#row' => 10,
    '#default_value' => $row->javascript,
  );
  return $form;
}

/**
 * Save JavaScript.
 */
function form_maker_javascript_save($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  db_query("UPDATE {form_maker_table} SET javascript=:javascript WHERE vid=:vid", array(
    ':javascript' => $form_state['values']['javascript_textarea'],
    ':vid' => $nodeid,
    ));
  $form_state['redirect'] = url('node/' . $nodeid . '/form_maker', array('absolute' => TRUE));
  drupal_set_message(t('JavaScript successfully updated.'), 'status', FALSE);
}

/**
 * Apply JavaScript.
 */
function form_maker_javascript_apply($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  db_query("UPDATE {form_maker_table} SET javascript=:javascript WHERE vid=:vid", array(
    ':javascript' => $form_state['values']['javascript_textarea'],
    ':vid' => $nodeid,
    ));
  drupal_set_message(t('JavaScript successfully updated.'), 'status', FALSE);
}

/**
 * Cancel JavaScript save.
 */
function form_maker_javascript_cancel($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  $form_state['redirect'] = url('node/' . $nodeid . '/form_maker', array('absolute' => TRUE));
}

/**
 * Edit Custom text in email for administrator.
 */
function form_maker_custom_text_in_email_admin() {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  $row = db_query("SELECT * FROM {form_maker_table} WHERE vid=:vid", array(':vid' => $nodeid))->fetchObject();
  $form['fieldset_form_custom_text_in_email_admin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom text in email for administrator') . ' - ' . $row->title,
  );
  $form['fieldset_form_custom_text_in_email_admin']['form_custom_text_in_email_save_admin'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_custom_text_in_email_save_admin'),
    '#value' => t('Save'),
  );
  $form['fieldset_form_custom_text_in_email_admin']['form_custom_text_in_email_apply_admin'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_custom_text_in_email_apply_admin'),
    '#value' => t('Apply'),
  );
  $form['fieldset_form_custom_text_in_email_admin']['form_custom_text_in_email_cancel_admin'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_javascript_cancel'),
    '#value' => t('Cancel'),
  );
  $message_for_without_editor = '<div style="width:100%; position:relative; top:30px; background-color:#FEF5F1; color:#8C2E0B; border-color:#ED541D; padding:2px; border: 1px solid #DD7777; margin:0 0 20px 0;">
                  ' . t('To show HTML editor download \'tinymce\' library from !url and extract it to sites/all/libraries/tinymce directory.', array('!url' => l(t('http://github.com/downloads/tinymce/tinymce/tinymce_3.5.7.zip'), 'http://github.com/downloads/tinymce/tinymce/tinymce_3.5.7.zip'))) . '
                   </div>';
  $form['fieldset_form_custom_text_in_email_admin']['custom_text_in_email_admin_textarea_before'] = array(
    '#prefix' => (file_exists("sites/all/libraries/tinymce/jscripts/tiny_mce/tiny_mce.js") ? '' : $message_for_without_editor),
    '#type' => 'textarea',
    '#title' => t('Text before Message'),
    '#default_value' => $row->admin_script1,
    '#attributes' => array('class' => array('form_maker_editor')),
    '#suffix' => '<hr /><br /><h2 style="text-align:center">' . t('MESSAGE') . '</h2><br /><hr />',
  );
  $form['fieldset_form_custom_text_in_email_admin']['custom_text_in_email_admin_textarea_after'] = array(    '#prefix' => (file_exists("sites/all/libraries/tinymce/jscripts/tiny_mce/tiny_mce.js") ? '' : $message_for_without_editor),
    '#type' => 'textarea',
    '#title' => t('Text after Message'),
    '#default_value' => $row->admin_script2,
    '#attributes' => array('class' => array('form_maker_editor')),
  );
  return $form;
}

/**
 * Save Custom text in email for administrator.
 */
function form_maker_custom_text_in_email_save_admin($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  db_query("UPDATE {form_maker_table} SET admin_script1=:admin_script1 WHERE vid=:vid", array(
    ':admin_script1' => $form_state['values']['custom_text_in_email_admin_textarea_before'],
    ':vid' => $nodeid,
    ));
  db_query("UPDATE {form_maker_table} SET admin_script2=:admin_script2 WHERE vid=:vid", array(
    ':admin_script2' => $form_state['values']['custom_text_in_email_admin_textarea_after'],
    ':vid' => $nodeid,
    ));
  $form_state['redirect'] = url('node/' . $nodeid . '/form_maker', array('absolute' => TRUE));
  drupal_set_message(t('Custom text in email for administrator successfully updated.'), 'status', FALSE);
}

/**
 * Apply Custom text in email for administrator.
 */
function form_maker_custom_text_in_email_apply_admin($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  db_query("UPDATE {form_maker_table} SET admin_script1=:admin_script1 WHERE vid=:vid", array(
    ':admin_script1' => $form_state['values']['custom_text_in_email_admin_textarea_before'],
    ':vid' => $nodeid,
    ));
  db_query("UPDATE {form_maker_table} SET admin_script2=:admin_script2 WHERE vid=:vid", array(
    ':admin_script2' => $form_state['values']['custom_text_in_email_admin_textarea_after'],
    ':vid' => $nodeid,
    ));
  drupal_set_message(t('Custom text in email for administrator successfully updated.'), 'status', FALSE);
}

/**
 * Edit Custom text in email for users.
 */
function form_maker_custom_text_in_email_user() {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  // $form_state['redirect'] = url('node/' . $nodeid . '/form_maker/custom_text_in_email', array('absolute' => TRUE));
  $row = db_query("SELECT * FROM {form_maker_table} WHERE vid=:vid", array(':vid' => $nodeid))->fetchObject();
  $form['fieldset_form_custom_text_in_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom text in email for user') . ' - ' . $row->title,
  );
  $form['fieldset_form_custom_text_in_email']['form_custom_text_in_email_save'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_custom_text_in_email_save'),
    '#value' => t('Save'),
  );
  $form['fieldset_form_custom_text_in_email']['form_custom_text_in_email_apply'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_custom_text_in_email_apply'),
    '#value' => t('Apply'),
  );
  $form['fieldset_form_custom_text_in_email']['form_custom_text_in_email_cancel'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_javascript_cancel'),
    '#value' => t('Cancel'),
  );
  $message_for_without_editor = '<div style="width:100%; position:relative; top:30px; background-color: #FEF5F1;color: #8C2E0B;border-color: #ED541D; padding: 2px;border: 1px solid #DD7777; margin:0 0 20px 0;">
                  ' . t('To show HTML editor download \'tinymce\' library from !url and extract it to sites/all/libraries/tinymce directory.', array('!url' => l(t('http://github.com/downloads/tinymce/tinymce/tinymce_3.5.7.zip'), 'http://github.com/downloads/tinymce/tinymce/tinymce_3.5.7.zip'))) . '
                   </div>';
  $form['fieldset_form_custom_text_in_email']['custom_text_in_email_textarea_before'] = array(
    '#prefix' => (file_exists("sites/all/libraries/tinymce/jscripts/tiny_mce/tiny_mce.js") ? '' : $message_for_without_editor),
    '#type' => 'textarea',
    '#title' => t('Text before Message'),
    '#default_value' => $row->script1,
    '#attributes' => array('class' => array('form_maker_editor')),
    '#suffix' => '<hr /><br /><h2 style="text-align:center">' . t('MESSAGE') . '</h2><br /><hr />',
  );
  $form['fieldset_form_custom_text_in_email']['custom_text_in_email_textarea_after'] = array(    '#prefix' => (file_exists("sites/all/libraries/tinymce/jscripts/tiny_mce/tiny_mce.js") ? '' : $message_for_without_editor),
    '#type' => 'textarea',
    '#title' => t('Text after Message'),
    '#default_value' => $row->script2,
    '#attributes' => array('class' => array('form_maker_editor')),
  );
  return $form;
}

/**
 * Save Custom text in email for users.
 */
function form_maker_custom_text_in_email_save($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  db_query("UPDATE {form_maker_table} SET script1=:script1 WHERE vid=:vid", array(
    ':script1' => $form_state['values']['custom_text_in_email_textarea_before'],
    ':vid' => $nodeid,
    ));
  db_query("UPDATE {form_maker_table} SET script2=:script2 WHERE vid=:vid", array(
    ':script2' => $form_state['values']['custom_text_in_email_textarea_after'],
    ':vid' => $nodeid,
    ));
  $form_state['redirect'] = url('node/' . $nodeid . '/form_maker', array('absolute' => TRUE));
  drupal_set_message(t('Custom text in email for user successfully updated.'), 'status', FALSE);
}

/**
 * Apply Custom text in email for users.
 */
function form_maker_custom_text_in_email_apply($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  db_query("UPDATE {form_maker_table} SET script1=:script1 WHERE vid=:vid", array(
    ':script1' => $form_state['values']['custom_text_in_email_textarea_before'],
    ':vid' => $nodeid,
    ));
  db_query("UPDATE {form_maker_table} SET script2=:script2 WHERE vid=:vid", array(
    ':script2' => $form_state['values']['custom_text_in_email_textarea_after'],
    ':vid' => $nodeid,
    ));
  drupal_set_message(t('Custom text in email for user successfully updated.'), 'status', FALSE);
}

/**
 * Save created form.
 */
function form_maker_form_save($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  if (isset($_POST["label_order"]) && isset($_POST["title"]) && isset($_POST["form"])) {
    $no_slash_form = stripslashes($_POST["form"]);
    $save_or_no = db_query("UPDATE {form_maker_table} SET 
      title=:title,
      mail=:mail,
      form=:form,
      form_front=:form_front,
      theme=:theme,
      counter=:counter,
      label_order=:label_order,
      pagination=:pagination,
      show_title=:show_title,
      show_numbers=:show_numbers,
      public_key=:public_key,
      private_key=:private_key,
      recaptcha_theme=:recaptcha_theme
      WHERE vid=:vid", array(
        ':title' => check_plain($_POST["title"]),
        ':mail' => filter_xss($_POST["mail"]),
        ':form' => $no_slash_form,
        ':form_front' => $_POST["form_front"],
        ':theme' => check_plain($_POST["select_theme"]),
        ':counter' => check_plain($_POST["counter"]),
        ':label_order' => check_plain($_POST["label_order"]),
        ':pagination' => check_plain($_POST["pagination"]),
        ':show_title' => check_plain($_POST["show_title"]),
        ':show_numbers' => check_plain($_POST["show_numbers"]),
        ':public_key' => check_plain($_POST["public_key"]),
        ':private_key' => check_plain($_POST["private_key"]),
        ':recaptcha_theme' => check_plain($_POST["recaptcha_theme"]),
        ':vid' => $nodeid,
      ));
  }
  $form_state['redirect'] = url('node/' . $nodeid, array('absolute' => TRUE));
  drupal_set_message(t('Form Saved'), 'status', FALSE);
}

/**
 * Apply created form.
 */
function form_maker_form_apply($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  if (isset($_POST["label_order"]) && isset($_POST["title"]) && isset($_POST["form"])) {
    $no_slash_form = stripslashes($_POST["form"]);
    $save_or_no = db_query("UPDATE {form_maker_table} SET 
      title=:title,
      mail=:mail,
      form=:form,
      form_front=:form_front,
      theme=:theme,
      counter=:counter,
      label_order=:label_order,
      pagination=:pagination,
      show_title=:show_title,
      show_numbers=:show_numbers,
      public_key=:public_key,
      private_key=:private_key,
      recaptcha_theme=:recaptcha_theme
      WHERE vid=:vid", array(
        ':title' => check_plain($_POST["title"]),
        ':mail' => filter_xss($_POST["mail"]),
        ':form' => $no_slash_form,
        ':form_front' => $_POST["form_front"],
        ':theme' => check_plain($_POST["select_theme"]),
        ':counter' => check_plain($_POST["counter"]),
        ':label_order' => check_plain($_POST["label_order"]),
        ':pagination' => check_plain($_POST["pagination"]),
        ':show_title' => check_plain($_POST["show_title"]),
        ':show_numbers' => check_plain($_POST["show_numbers"]),
        ':public_key' => check_plain($_POST["public_key"]),
        ':private_key' => check_plain($_POST["private_key"]),
        ':recaptcha_theme' => check_plain($_POST["recaptcha_theme"]),
        ':vid' => $nodeid,
      ));
  }
  drupal_set_message(t('Form Saved'), 'status', FALSE);
}

/**
 * Cancel form save.
 */
function form_maker_form_cancel($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  $form_state['redirect'] = url('node/' . $nodeid, array('absolute' => TRUE));
}
