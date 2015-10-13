<?php
/**
 * @file
 * Add a new field to form.
 */

/**
 * Add a new field to form.
 */
function form_maker_add_field() {
  // drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_calendar.js');
  // drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_calendar-setup.js');
  // drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_cal_function.js');

  drupal_add_css(drupal_get_path('module', 'form_maker') . '/css/form_maker_edit.css');
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_if_gmap.js');
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  $row = db_query("SELECT * FROM {form_maker_table} WHERE vid=:vid", array(':vid' => $nodeid))->fetchObject();
  // Get all labels to do not delete from date base after removing fields from form.
  $labels = array();
  $label_id = array();
  $label_order_original = array();
  $label_type = array();
  $label_all = explode('#****#', $row->label_order);
  $label_all = array_slice($label_all, 0, count($label_all) - 1);
  foreach ($label_all as $key => $label_each) {
    $label_id_each = explode('#**id**#', $label_each);
    array_push($label_id, $label_id_each[0]);
    $label_oder_each = explode('#**label**#', $label_id_each[1]);
    array_push($label_order_original, $label_oder_each[0]);
    array_push($label_type, $label_oder_each[1]);
  }
  $labels["id"] = implode(",", $label_id);
  $labels["label"] = implode(",", $label_order_original);
  $labels["type"] = implode(",", $label_type);
  drupal_add_js(array(
    'form_maker' => array(
      'labels_id' => $labels["id"],
      'labels_label' => str_replace(" ", "", $labels['label']),
      'labels_type' => $labels["type"],
      'get_module_path' => base_path() . drupal_get_path('module', 'form_maker'),
      'get_module_path_desc' => drupal_get_path('module', 'form_maker'),
      'country_popup_url' => url('node/' . $nodeid . '/form_maker/select_country_preview', array('query' => array('example' => ''), 'absolute' => FALSE)),      'captcha_url' => url('form_maker/captcha', array('query' => array('digit' => ''), 'absolute' => FALSE)),
      ),
    ),
    'setting');
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_add_field.js');
  drupal_add_js('
    var gen=' . $row->counter . ';
    var j = 2;
    var c;
    var a = new Array();
    var form_view = 1;
    var form_view_max = 1;
    var form_view_count = 1;
    var formOldFunctionOnLoad = null;
    formLoadBody();',
    array('type' => 'inline', 'scope' => 'footer'));
  $frontend_edit = '
			<table style="border:6px #00aeef solid; background-color:#00aeef; min-width:600px" width="95%" cellpadding="0" cellspacing="0">
				<tbody>
					<tr style="height:27px;">
						<td align="left" valign="middle" rowspan="3" style="padding:10px; border:none;">
							<img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/formmaker.png">
            </td>
            <td></td>
            <td>
							<input type="hidden" id="title" name="title" style="background:none; width:150px; height:15px; border:none; font-size:10px;" value="' . $row->title . '">
						</td>
					</tr>
					<tr>
						<td width="300" align="right" valign="middle">
							<span style="font-size:16.76pt; font-family:BauhausItcTEEMed; color:#FFFFFF; vertical-align:middle;">' . t('Email to send submissions to:') . '&nbsp;&nbsp;</span>
						</td>
						<td width="175" align="center" valign="middle">
							<div style=\'background-image:url("' . base_path() . drupal_get_path('module', 'form_maker') . '/images/input.png"); height:19px; width:175px\'>
								<input id="mail" name="mail" style="background:none; width:151px; height:15px; border:none; font-size:11px" value="' . $row->mail . '">
							</div>
						</td>
					</tr>
					<tr>
            <td width="450" align="right" valign="middle">
							<span style="font-size:16.76pt; font-family:BauhausItcTEEMed; color:#FFFFFF; vertical-align:middle;">' . t('Theme:') . '&nbsp;&nbsp;</span>
						</td>
						<td width="175" align="center" valign="middle">
              <div style=\'background-image:url("' . base_path() . drupal_get_path('module', 'form_maker') . '/images/input.png"); height:19px; width:175px\'>
                <select id="select_theme" name="select_theme" onchange="form_maker_theme_change();" style="background-color:rgba(255, 255, 255, 0);">';
  drupal_add_js('form_maker_theme_change();', array('type' => 'inline', 'scope' => 'footer'));
  $themes_ids_col = db_query("SELECT id,title FROM {form_maker_themes}")->fetchAllKeyed();
  foreach ($themes_ids_col as $theme_id => $theme_title) {
    $frontend_edit .= '
                  <option value="' . $theme_id . '" ' . (($row->theme == $theme_id) ? 'SELECTED' : '') . '>' . $theme_title . '</option>';
  }
  $frontend_edit .= '
                </select>
              </div>
						</td>
					</tr>
					<tr>
						<td align="left" colspan="3">
							<img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/addanewfield.png" onclick="form_maker_enable(); form_maker_Enable();" style="cursor:pointer;margin:10px;">
						</td>
					</tr>
				</tbody>
			</table>
      <div id="formMakerDiv" onclick="form_maker_close_window()"></div>
      <div id="formMakerDiv1" align="center">
        <table class="form_maker_table" width="100%" cellpadding="0" cellspacing="0" height="100%" style="border:6px #00aeef solid; background-color:#FFF; margin:0;">
          <tr>
            <td style="padding:0px">
              <table class="form_maker_table" cellpadding="0" cellspacing="0" width="100%" height="100%" style="margin:0;">
                <tr valign="top">
                  <td width="15%" height="100%" style="border-right:dotted black 1px; padding:0;" id="field_types">
                    <div id="when_edit" style="display:none"></div>
                    <table class="form_maker_table" cellpadding="0" cellspacing="3" width="100%" style="border-collapse:separate;border-spacing:3px; margin:0;">
                      <tr>
                        <td align="center" onClick=form_maker_addRow("customHTML") class="field_buttons" id="table_editor">
                          <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/customHTML.png" style="margin:5px" id="img_customHTML"/>
                        </td>
                        <td align="center" onClick=form_maker_addRow("text") class="field_buttons" id="table_text">
                          <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/text.png" style="margin:5px" id="img_text"/>
                        </td>
                      </tr>
                      <tr>
                        <td align="center" onClick=form_maker_addRow("time_and_date") class="field_buttons" id="table_time_and_date">
                          <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/time_and_date.png" style="margin:5px" id="img_time_and_date"/>
                        </td>
                        <td align="center" onClick=form_maker_addRow("select") class="field_buttons" id="table_select">
                          <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/select.png" style="margin:5px" id="img_select"/>
                        </td>
                      </tr>
                      <tr>
                        <td align="center" onClick=form_maker_addRow("checkbox") class="field_buttons" id="table_checkbox">
                          <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/checkbox.png" style="margin:5px" id="img_checkbox"/>
                        </td>
                        <td align="center" onClick=form_maker_addRow("radio") class="field_buttons" id="table_radio">
                          <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/radio.png" style="margin:5px" id="img_radio"/>
                        </td>
                      </tr>
                      <tr>
                        <td align="center" onClick=form_maker_addRow("file_upload") class="field_buttons" id="table_file_upload">
                          <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/file_upload.png" style="margin:5px" id="img_file_upload"/>
                        </td>
                        <td align="center" onClick=form_maker_addRow("captcha") class="field_buttons" id="table_captcha">
                          <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/captcha.png" style="margin:5px" id="img_captcha"/>
                        </td>
                      </tr>
                      <tr>
                        <td align="center" onClick=form_maker_addRow("page_break") class="field_buttons" id="table_page_break">
                          <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/page_break.png" style="margin:5px" id="img_page_break"/>
                        </td>
                        <td align="center" onClick=form_maker_addRow("section_break") class="field_buttons" id="table_section_break">
                          <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/section_break.png" style="margin:5px" id="img_section_break"/>
                        </td>
                      </tr>
                      <tr>
                        <td align="center" onClick=form_maker_addRow("map") class="field_buttons" id="table_map">
                          <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/map.png" style="margin:5px" id="img_map"/>
                        </td>
                        <td align="center" onClick=form_maker_addRow("button") class="field_buttons" id="table_button">
                          <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/button.png" style="margin:5px" id="img_button"/>
                        </td>
                      </tr>
                    </table>
                  </td>
                  <td width="30%" height="100%" align="left" style="padding:0;">
                    <div id="edit_table" style="padding:0px; overflow-y:scroll; height:520px;" ></div>
                  </td>
                  <td align="center" valign="top" style=\'padding:0; background:url("' . base_path() . drupal_get_path('module', 'form_maker') . '/images/border2.png") repeat-y;\'>&nbsp;</td>
                  <td style="padding:0;">
                    <table class="form_maker_table" cellpadding="0" cellspacing="0" width="100%" height="100%">
                      <tr>
                        <td align="right" style="padding:0">
                          <input type="radio" value="end" name="el_pos" checked="checked" id="pos_end" onclick="Disable()"/>' . t('At The End') . '
                          <input type="radio" value="begin" name="el_pos" id="pos_begin" onclick="Disable()"/>' . t('At The Beginning') . '
                          <input type="radio" value="before" name="el_pos" id="pos_before" onclick="form_maker_Enable();"/>' . t('Before') . '
                          <select style="width:100px; margin-left:5px" id="sel_el_pos" disabled="disabled" class="form-select"></select>
                          <img alt="ADD" title="add" style="cursor:pointer; vertical-align:middle; margin:5px" src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/save.png" onClick="add(0)"/>
                          <img alt="CANCEL" title="cancel"  style=" cursor:pointer; vertical-align:middle; margin:5px" src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/cancel_but.png" onClick="form_maker_close_window()"/>
                          <hr style=" margin-bottom:10px" />
                        </td>
                      </tr>
                      <tr height="100%" valign="top">
                        <td id="show_table" style="vertical-align:top; padding:0"></td>
                      </tr>
                    </table>
                  </td>
                </tr>
              </table>
            </td>
          </tr>
        </table>
        <input type="hidden" id="old" />
        <input type="hidden" id="old_selected" />
        <input type="hidden" id="element_type" />
        <input type="hidden" id="editing_id" />
        <div id="main_editor" style="position:absolute; display:none; z-index:140; background-color:white; padding-left:0px;">
          <textarea id="textAreaContent" class="form_maker_editor"></textarea>
        </div>
      </div>

      <input type="hidden" name="option" value="com_formmaker" />
      <input type="hidden" name="id" value="' . $row->id . '" />
      <input type="hidden" name="cid[]" value="' . $row->id . '" />
      <input type="hidden" name="task" value="" />
      <input type="hidden" id="araqel" value="0" />';
  return $frontend_edit;
}
