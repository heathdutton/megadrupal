function formOnload() {
  // Enable maps.
  for (t = 0; t < gen; t++)
  if (document.getElementById(t + "_typeform_id_temp")) {
    if (document.getElementById(t + "_typeform_id_temp").value == "type_map" || document.getElementById(t + "_typeform_id_temp").value == "type_mark_map") {
      if_gmap_init(t);
      for (q = 0; q < 20; q++) {
        if (document.getElementById(t + "_elementform_id_temp").getAttribute("long" + q)) {
          w_long = parseFloat(document.getElementById(t + "_elementform_id_temp").getAttribute("long" + q));
          w_lat = parseFloat(document.getElementById(t + "_elementform_id_temp").getAttribute("lat" + q));
          w_info = parseFloat(document.getElementById(t + "_elementform_id_temp").getAttribute("info" + q));
          add_marker_on_map(t, q, w_long, w_lat, w_info, false);
        }
      }
    }
    else if (document.getElementById(t + "_typeform_id_temp").value == "type_date") {
      Calendar.setup({
        inputField:t + "_elementform_id_temp",
        ifFormat:document.getElementById(t + "_buttonform_id_temp").getAttribute('format'),
        button:t + "_buttonform_id_temp",
        align:"Tl",
        singleClick:true,
        firstDay:0
      });
    }
  }
  form_view = 1;
  form_view_count = 0;
  for (i = 1; i <= 30; i++) {
    if (document.getElementById('form_id_tempform_view' + i)) {
      form_view_count++;
      form_view_max = i;
    }
  }
  if (form_view_count > 1) {
    for (i = 1; i <= form_view_max; i++) {
      if (document.getElementById('form_id_tempform_view' + i)) {
        first_form_view = i;
        break;
      }
    }
    form_view = form_view_max;
    generate_page_nav(first_form_view);
    var img_EDIT = document.createElement("img");
    img_EDIT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/edit.png");
    img_EDIT.style.cssText = "margin-left:40px; cursor:pointer";
    img_EDIT.setAttribute("onclick", 'el_page_navigation()');
    var td_EDIT = document.getElementById("edit_page_navigation");
    td_EDIT.appendChild(img_EDIT);
    document.getElementById('page_navigation').appendChild(td_EDIT);
  }
  document.getElementById('form').value = document.getElementById('take').innerHTML;
  document.getElementById('araqel').value = 1;
}

function formAddToOnload() {
  if (formOldFunctionOnLoad) {
    formOldFunctionOnLoad();
  }
  formOnload();
}

function formLoadBody() {
  formOldFunctionOnLoad = window.onload;
  window.onload = formAddToOnload;
}

/**
 * Show add field div.
 */
function form_maker_enable()	{
  alltypes = Array('customHTML','text','checkbox','radio','time_and_date','select','file_upload','captcha','map','button','page_break','section_break');
  for (x = 0; x < 12; x++) {
    document.getElementById('img_'+alltypes[x]).src = Drupal.settings.form_maker.get_module_path + '/images/' + alltypes[x] + ".png";
  }
  document.getElementById('formMakerDiv').style.display = (document.getElementById('formMakerDiv').style.display == 'block' ? 'none' : 'block');
  document.getElementById('formMakerDiv1').style.display = (document.getElementById('formMakerDiv1').style.display == 'block' ? 'none' : 'block');
  if (document.getElementById('formMakerDiv').offsetWidth) {
    document.getElementById('formMakerDiv1').style.width = (document.getElementById('formMakerDiv').offsetWidth - 60) + 'px';
  }
  document.getElementById('when_edit').style.display = 'none';
}

function enable2() {
	alltypes = Array('customHTML','text','checkbox','radio','time_and_date','select','file_upload','captcha','map','button','page_break','section_break');
	for (x = 0; x < 12; x++) {
    document.getElementById('img_' + alltypes[x]).src = "" + Drupal.settings.form_maker.get_module_path + "/images/" + alltypes[x] + ".png";
	}
  document.getElementById('formMakerDiv').style.display	=(document.getElementById('formMakerDiv').style.display == 'block' ? 'none' : 'block');
  document.getElementById('formMakerDiv1').style.display	=(document.getElementById('formMakerDiv1').style.display == 'block' ? 'none' : 'block');
  if (document.getElementById('formMakerDiv').offsetWidth) {
    document.getElementById('formMakerDiv1').style.width = (document.getElementById('formMakerDiv').offsetWidth - 60) + 'px';
  }
  document.getElementById('when_edit').style.display = 'block';
  if (document.getElementById('field_types').offsetWidth) {
    document.getElementById('when_edit').style.width = document.getElementById('field_types').offsetWidth + 'px';
  }
  if (document.getElementById('field_types').offsetHeight) {
    document.getElementById('when_edit').style.height = document.getElementById('field_types').offsetHeight + 'px';
  }
}
  
/**
 * Enable.
 */
function form_maker_Enable() {
  var pos = document.getElementsByName("el_pos");
	pos[0].setAttribute("checked", "checked");
  select_ = document.getElementById('sel_el_pos');
  select_.innerHTML = "";
  for (k = 1; k <= form_view_max; k++) {
    if (document.getElementById('form_id_tempform_view' + k)) {
      form_view_element = document.getElementById('form_id_tempform_view' + k);
      form_maker_remove_spaces(form_view_element);
      n = form_view_element.childNodes.length - 2;
      for (z = 0; z <= n; z++) {
        if (!form_view_element.childNodes[z].id) {
          GLOBAL_tr = form_view_element.childNodes[z];
          for (x = 0; x < GLOBAL_tr.firstChild.childNodes.length; x++) {
            table = GLOBAL_tr.firstChild.childNodes[x];
            tbody = table.firstChild;
            for (y = 0; y < tbody.childNodes.length; y++) {
              tr = tbody.childNodes[y];
              var option = document.createElement('option');
              option.setAttribute("id", tr.id + "_sel_el_pos");
              option.setAttribute("value", tr.id);
              option.innerHTML=document.getElementById( tr.id + '_element_labelform_id_temp').innerHTML;
              select_.appendChild(option);
            }
          }
        }
			}
		}
  }
  select_.removeAttribute("disabled");
}

/**
 * Hide edit form page.
 */
function form_maker_close_window() {
  form_maker_enable();
  document.getElementById('edit_table').innerHTML = "";
  document.getElementById('show_table').innerHTML = "";
  document.getElementById('main_editor').style.display = "none";
  // if (document.getElementsByTagName("iframe")[0]) {
    // ifr_id = document.getElementsByTagName("iframe")[0].id;
    // ifr = getIFrameDocument(ifr_id);
    // ifr.body.innerHTML = "";
  // }
  // document.getElementById('editor').value = "";
  document.getElementById('editing_id').value = "";
  document.getElementById('element_type').value = "";
  alltypes = Array('customHTML','text','checkbox','radio','time_and_date','select','file_upload','captcha','map','button','page_break','section_break');
  for (x = 0; x < 12; x++) {
    document.getElementById('img_' + alltypes[x]).parentNode.style.backgroundColor = '';
  }
}

/**
 * Add one of fields.
 */
function form_maker_addRow(b) {
  if (document.getElementById('show_table').innerHTML) {
    document.getElementById('show_table').innerHTML = "";
    document.getElementById('edit_table').innerHTML = "";
  }
  alltypes = Array('customHTML', 'text', 'checkbox', 'radio', 'time_and_date', 'select', 'file_upload', 'captcha', 'map', 'button', 'page_break', 'section_break');
  for (x = 0; x < 12; x++) {
    document.getElementById('img_'+alltypes[x]).parentNode.style.backgroundColor='';
  }
  document.getElementById('img_' + b).parentNode.style.backgroundColor = '#FE6400';
  switch(b) {
    case 'customHTML': {
      form_maker_el_editor();
      break;

    }
    case 'text': {
      form_maker_el_text();
      break;

    }
		case 'checkbox': {
      form_maker_el_checkbox();
      break;

    }
		case 'radio': {
      form_maker_el_radio();
      break;

    }
		case 'time_and_date': {
      form_maker_el_time_and_date();
      break;

    }
		case 'select': {
      form_maker_el_select();
      break;

    }
		case 'file_upload': {
      form_maker_el_file_upload();
      break;

    }
		case 'captcha': {
      form_maker_el_captcha();
      break;

    }
		case 'map': {
      form_maker_el_map();
      break;

    }
    case 'button': {
      form_maker_el_button();
      break;

    }
    case 'page_break': {
      form_maker_el_page_break();
      break;

    }
    case 'section_break': {
      form_maker_el_section_break();
      break;

    }
	}
  var pos = document.getElementsByName("el_pos");
  pos[0].checked = "checked";
}

/**
 * HTML Editor field.
 */
function form_maker_el_editor() {
  if (document.getElementById("editing_id").value) {
    new_id = document.getElementById("editing_id").value;
  }
  else {
    new_id = gen;
  }
  var pos = document.getElementsByName("el_pos");
  pos[0].removeAttribute("disabled");
  pos[1].removeAttribute("disabled");
  pos[2].removeAttribute("disabled");
  var sel_el_pos = document.getElementById("sel_el_pos");
  sel_el_pos.removeAttribute("disabled", "disabled");
  type_editor(new_id,''); ////////////////////////////????????????????????????????????????????????????????
}

function type_editor(i, w_editor) {
  document.getElementById("element_type").value = "type_editor";
  delete_last_child();
  // Edit table.
  oElement = document.getElementById('table_editor');
  var iReturnTop = 0;
  var iReturnLeft = 0;
  while (oElement != null) {
    iReturnTop += oElement.offsetTop;
    iReturnLeft += oElement.offsetLeft;
    oElement = oElement.offsetParent;
  }
  document.getElementById('main_editor').style.display = "block";
  document.getElementById('main_editor').style.left = iReturnLeft + 195 + "px";
  document.getElementById('main_editor').style.top = iReturnTop + 70 + "px";
  if (document.getElementById('textAreaContent').style.display == "none") {
    // ifr.body.innerHTML = w_editor;
    if (document.getElementsByTagName("iframe")[0]) {
      ifr_id = document.getElementsByTagName("iframe")[0].id;
      ifr = getIFrameDocument(ifr_id);
      ifr.body.innerHTML = w_editor;
    }
  }
  else {
    // document.getElementById('editor').value = w_editor;
    document.getElementById('textAreaContent').value = w_editor;
    var table_div = document.getElementById('edit_table');
    var helparea = document.createElement("div");
    helparea.setAttribute("id", "help");
    helparea.setAttribute("style", "position:relative; top:30px; background-color: #FEF5F1;color: #8C2E0B;border-color: #ED541D; padding: 2px;border: 1px solid #DD7777;");
    helparea.innerHTML = "<h2>To show HTML editor download \'tinymce\' library from <a href=\'http://github.com/downloads/tinymce/tinymce/tinymce_3.5.7.zip\'>http://github.com/downloads/tinymce/tinymce/tinymce_3.5.7.zip</a> and extract it to sites/all/libraries/tinymce directory.</h2>";
    table_div.appendChild(helparea);
    var textAreaContent = document.getElementById('textAreaContent');
    textAreaContent.setAttribute("class", "form-textarea");
    textAreaContent.setAttribute("cols", "85");
    textAreaContent.setAttribute("rows", "10");
    textAreaContent.setAttribute("style", "visibility:visible; width:600px;");
  }
  element = 'div';
  var div = document.createElement('div');
  div.setAttribute("id", "main_div");
  var main_td = document.getElementById('show_table');
  main_td.appendChild(div);
  var div = document.createElement('div');
  div.style.width = "650px";
  document.getElementById('edit_table').appendChild(div);
}

/**
 * Text field.
 */
function form_maker_el_text() {
  if (document.getElementById("editing_id").value) {
    new_id = document.getElementById("editing_id").value;
  }
  else {
    new_id = gen;
  }
  // Edit table.
  var td = document.getElementById('edit_table');
  // Type select.
  var el_type_label = document.createElement('label');
  el_type_label.style.cssText = "color: #00aeef; font-weight: bold; font-size: 13px";
  el_type_label.innerHTML = "<br />&nbsp;&nbsp;Field type";
  td.appendChild(el_type_label);
  var el_type_radio_text = document.createElement('input');
  el_type_radio_text.setAttribute("id", "el_type_radio_text");
  el_type_radio_text.setAttribute("type", "radio");
  el_type_radio_text.style.cssText = "margin-left:15px";
  el_type_radio_text.setAttribute("value", "text");
  el_type_radio_text.setAttribute("name", "el_type");
  el_type_radio_text.setAttribute("onclick", "go_to_type_text('" + new_id + "')");
  el_type_radio_text.setAttribute("checked", "checked");
  Text = document.createTextNode("Simple text");
  var el_type_radio_password = document.createElement('input');
  el_type_radio_password.setAttribute("id", "el_type_radio_password");
  el_type_radio_password.setAttribute("type", "radio");
  el_type_radio_password.style.cssText = "margin-left:15px";
  el_type_radio_password.setAttribute("value", "password");
  el_type_radio_password.setAttribute("name", "el_type");
  el_type_radio_password.setAttribute("onclick", "go_to_type_password('" + new_id + "')");
  Password = document.createTextNode("Password");
  var el_type_radio_textarea = document.createElement('input');
  el_type_radio_textarea.setAttribute("id", "el_type_radio_textarea");
  el_type_radio_textarea.setAttribute("type", "radio");
  el_type_radio_textarea.style.cssText = "margin-left:15px";
  el_type_radio_textarea.setAttribute("value", "textarea");
  el_type_radio_textarea.setAttribute("name", "el_type");
  el_type_radio_textarea.setAttribute("onclick", "go_to_type_textarea('" + new_id + "')");
  Textarea = document.createTextNode("Text area");
  var el_type_radio_name = document.createElement('input');
  el_type_radio_name.setAttribute("id", "el_type_radio_name");
  el_type_radio_name.setAttribute("type", "radio");
  el_type_radio_name.style.cssText = "margin-left:15px";
  el_type_radio_name.setAttribute("value", "name");
  el_type_radio_name.setAttribute("name", "el_type");
  el_type_radio_name.setAttribute("onclick", "go_to_type_name('" + new_id + "')");
  Name = document.createTextNode("Name");

  var el_type_radio_submitter_mail = document.createElement('input');
  el_type_radio_submitter_mail.setAttribute("id", "el_type_radio_submitter_mail");
  el_type_radio_submitter_mail.setAttribute("type", "radio");
  el_type_radio_submitter_mail.style.cssText = "margin-left:15px";
  el_type_radio_submitter_mail.setAttribute("value", "submitter_mail");
  el_type_radio_submitter_mail.setAttribute("name", "el_type");
  el_type_radio_submitter_mail.setAttribute("onclick", "go_to_type_submitter_mail('" + new_id + "')");
  Submitter_mail = document.createTextNode("E-mail");

  var el_type_radio_number = document.createElement('input');
  el_type_radio_number.setAttribute("id", "el_type_radio_number");
  el_type_radio_number.setAttribute("type", "radio");
  el_type_radio_number.style.cssText = "margin-left:15px";
  el_type_radio_number.setAttribute("value", "number");
  el_type_radio_number.setAttribute("name", "el_type");
  el_type_radio_number.setAttribute("onclick", "go_to_type_number('" + new_id + "')");
  Number = document.createTextNode("Number");

  var el_type_radio_phone = document.createElement('input');
  el_type_radio_phone.setAttribute("id", "el_type_radio_phone");
  el_type_radio_phone.setAttribute("type", "radio");
  el_type_radio_phone.style.cssText = "margin-left:15px";
  el_type_radio_phone.setAttribute("value", "phone");
  el_type_radio_phone.setAttribute("name", "el_type");
  el_type_radio_phone.setAttribute("onclick", "go_to_type_phone('" + new_id + "')");
  Phone = document.createTextNode("Phone");

  var el_type_radio_hidden = document.createElement('input');
  el_type_radio_hidden.setAttribute("type", "radio");
  el_type_radio_hidden.style.cssText = "margin-left:15px";
  el_type_radio_hidden.setAttribute("name", "el_type");
  el_type_radio_hidden.setAttribute("onclick", "go_to_type_hidden('" + new_id + "')");
  Hidden = document.createTextNode("Hidden field");

  var el_type_radio_address = document.createElement('input');
  el_type_radio_address.setAttribute("id", "el_type_radio_address");
  el_type_radio_address.setAttribute("type", "radio");
  el_type_radio_address.style.cssText = "margin-left:15px";
  el_type_radio_address.setAttribute("value", "address");
  el_type_radio_address.setAttribute("name", "el_type");
  el_type_radio_address.setAttribute("onchange", "go_to_type_address('" + new_id + "')");
  Address = document.createTextNode("Address");

  var el_type_radio_mark_map = document.createElement('input');
  el_type_radio_mark_map.setAttribute("id", "el_type_radio_mark_map");
  el_type_radio_mark_map.setAttribute("type", "radio");
  el_type_radio_mark_map.style.cssText = "margin-left:15px";
  el_type_radio_mark_map.setAttribute("value", "mark_map");
  el_type_radio_mark_map.setAttribute("name", "el_type");
  el_type_radio_mark_map.setAttribute("onchange", "go_to_type_mark_map('" + new_id + "')");
  Mark_map = document.createTextNode("Address(mark on map)");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');
  var br7 = document.createElement('br');
  var br8 = document.createElement('br');
  var br9 = document.createElement('br');
  var br10 = document.createElement('br');

  td.appendChild(br1);
  td.appendChild(el_type_radio_text);
  td.appendChild(Text);
  td.appendChild(br2);
  td.appendChild(el_type_radio_password);
  td.appendChild(Password);
  td.appendChild(br3);
  td.appendChild(el_type_radio_textarea);
  td.appendChild(Textarea);
  td.appendChild(br4);
  td.appendChild(el_type_radio_name);
  td.appendChild(Name);
  td.appendChild(br5);
  td.appendChild(el_type_radio_address);
  td.appendChild(Address);
  td.appendChild(br10);
  td.appendChild(el_type_radio_mark_map);
  td.appendChild(Mark_map);
  td.appendChild(br9);
  td.appendChild(el_type_radio_submitter_mail);
  td.appendChild(Submitter_mail);
  td.appendChild(br6);
  td.appendChild(el_type_radio_number);
  td.appendChild(Number);
  td.appendChild(br7);
  td.appendChild(el_type_radio_phone);
  td.appendChild(Phone);
  td.appendChild(br8);
  td.appendChild(el_type_radio_hidden);
  td.appendChild(Hidden);
  var pos = document.getElementsByName("el_pos");
  pos[0].removeAttribute("disabled");
  pos[1].removeAttribute("disabled");
  pos[2].removeAttribute("disabled");
  var sel_el_pos = document.getElementById("sel_el_pos");
  sel_el_pos.removeAttribute("disabled", "disabled");
  go_to_type_text(new_id);
}

function go_to_type_text(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  type_text(new_id, 'Text:', 'left', '200', '', '', 'no', 'no', '', w_attr_name, w_attr_value);
}

function go_to_type_number(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  type_number(new_id, 'Number:', 'left', '200', '', '', 'no', 'no', '', w_attr_name, w_attr_value);
}

function go_to_type_password(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  type_password(new_id, 'Password:', 'left', '200', 'no', 'no', 'wdform_input', w_attr_name, w_attr_value);
}

function go_to_type_textarea(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  type_textarea(new_id, 'Textarea:', 'left', '200', '100', '', '', 'no', 'no', '', w_attr_name, w_attr_value)
}

function go_to_type_name(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  w_first_val = ['', ''];
  w_title = ['', ''];
  type_name(new_id, 'Name:', 'left', w_first_val, w_title, '100', 'normal', 'no', 'no', '', w_attr_name, w_attr_value)
}

function go_to_type_address(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  type_address(new_id, 'Address:', 'left', '300', 'no', 'wdform_address', w_attr_name, w_attr_value)
}

function go_to_type_phone(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  w_first_val = ['', ''];
  w_title = ['', ''];
  type_phone(new_id, 'Phone:', 'left', '135', w_first_val, w_title, 'no', 'no', '', w_attr_name, w_attr_value)
}

function go_to_type_submitter_mail(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  type_submitter_mail(new_id, 'E-mail:', 'left', '200', '', '', 'no', 'no', '', w_attr_name, w_attr_value);
}

function go_to_type_time(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  type_time(new_id, 'Time:', 'left', '24', '0', '1', '', '', '', 'no', '', w_attr_name, w_attr_value);
}

function go_to_type_date(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  type_date(new_id, 'Date:', 'left', '', 'no', '', '%Y-%m-%d', '...', w_attr_name, w_attr_value);
}

function go_to_type_date_fields(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  var current_date = new Date();
  type_date_fields(new_id, 'Date:', 'left', '', '', '', 'SELECT', 'SELECT', 'SELECT', 'day', 'month', 'year', '55', '110', '75', 'no', 'wdform_date_fields', '1901', current_date.getFullYear(), '&nbsp;/&nbsp;', w_attr_name, w_attr_value);
}

function go_to_type_button(new_id) {
  w_title = [ "Button"];
  w_func = [""];
  w_attr_name = [];
  w_attr_value = [];
  type_button(new_id, w_title, w_func, 'wdform_button', w_attr_name, w_attr_value);
}

/**
 * Checkbox field.
 */
function form_maker_el_checkbox() {
  if (document.getElementById("editing_id").value) {
    new_id = document.getElementById("editing_id").value;
  }
  else {
    new_id = gen;
  }
  var pos = document.getElementsByName("el_pos");
  pos[0].removeAttribute("disabled");
  pos[1].removeAttribute("disabled");
  pos[2].removeAttribute("disabled");
  var sel_el_pos = document.getElementById("sel_el_pos");
  sel_el_pos.removeAttribute("disabled", "disabled");
  w_choices = [ "option 1", "option 2"];
  w_choices_checked = ["0", "0"];
  w_attr_name = [];
  w_attr_value = [];
  type_checkbox(new_id, 'Checkbox:', 'left', 'ver', w_choices, w_choices_checked, 'no', 'no', '0', '', w_attr_name, w_attr_value);
}

/**
 * Radio field.
 */
function form_maker_el_radio() {
  if (document.getElementById("editing_id").value) {
    new_id = document.getElementById("editing_id").value;
  }
  else {
    new_id = gen;
  }
  var pos = document.getElementsByName("el_pos");
  pos[0].removeAttribute("disabled");
  pos[1].removeAttribute("disabled");
  pos[2].removeAttribute("disabled");
  var sel_el_pos = document.getElementById("sel_el_pos");
  sel_el_pos.removeAttribute("disabled", "disabled");
  w_choices = [ "option 1", "option 2"];
  w_choices_checked = ["0", "0"];
  w_attr_name = [];
  w_attr_value = [];
  type_radio(new_id, 'Radio:', 'left', 'ver', w_choices, w_choices_checked, 'no', 'no', 'no', '0', '', w_attr_name, w_attr_value);
}

/**
 * Time and date field.
 */
function form_maker_el_time_and_date() {
  if (document.getElementById("editing_id").value) {
    new_id = document.getElementById("editing_id").value;
  }
  else {
    new_id = gen;
  }
  // Edit table.
  var el_type_label = document.createElement('label');
  el_type_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_type_label.innerHTML = "<br />&nbsp;&nbsp;Field type";

  var el_type_radio_time = document.createElement('input');
  el_type_radio_time.setAttribute("id", "el_type_radio_time");
  el_type_radio_time.setAttribute("type", "radio");
  el_type_radio_time.setAttribute("value", "time");
  el_type_radio_time.style.cssText = "margin-left:15px";
  el_type_radio_time.setAttribute("name", "el_type_radio_time");
  el_type_radio_time.setAttribute("onclick", "go_to_type_time('" + new_id + "')");
  Time_ = document.createTextNode("Time");

  var el_type_radio_date = document.createElement('input');
  el_type_radio_date.setAttribute("id", "el_type_radio_date");
  el_type_radio_date.setAttribute("type", "radio");
  el_type_radio_date.setAttribute("value", "date");
  el_type_radio_date.style.cssText = "margin-left:15px";
  el_type_radio_date.setAttribute("name", "el_type_radio_time");
  el_type_radio_date.setAttribute("onclick", "go_to_type_date('" + new_id + "')");
  el_type_radio_date.setAttribute("checked", "checked");

  Date_ = document.createTextNode("Date (Single fileld with a picker)");

  var el_type_radio_date_fields = document.createElement('input');
  el_type_radio_date_fields.setAttribute("id", "el_type_radio_date_fields");
  el_type_radio_date_fields.setAttribute("type", "radio");
  el_type_radio_date_fields.setAttribute("value", "date_fields");
  el_type_radio_date_fields.style.cssText = "margin-left:15px";
  el_type_radio_date_fields.setAttribute("name", "el_type_radio_time");
  el_type_radio_date_fields.setAttribute("onclick", "go_to_type_date_fields('" + new_id + "')");

  Date_fields_ = document.createTextNode("Date (3 separate fields)");

  var td = document.getElementById('edit_table');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  td.appendChild(el_type_label);
  td.appendChild(br1);
  td.appendChild(el_type_radio_date);
  td.appendChild(Date_);
  td.appendChild(br2);
  td.appendChild(el_type_radio_date_fields);
  td.appendChild(Date_fields_);
  td.appendChild(br3);
  td.appendChild(el_type_radio_time);
  td.appendChild(Time_);
  var pos = document.getElementsByName("el_pos");
  pos[0].removeAttribute("disabled");
  pos[1].removeAttribute("disabled");
  pos[2].removeAttribute("disabled");
  var sel_el_pos = document.getElementById("sel_el_pos");
  sel_el_pos.removeAttribute("disabled", "disabled");
  go_to_type_date(new_id);
}

/**
 * Select field.
 */
function form_maker_el_select() {
  // Edit table.
  if (document.getElementById("editing_id").value) {
    new_id = document.getElementById("editing_id").value;
  }
  else {
    new_id = gen;
  }
  // Type select.
  var el_type_label = document.createElement('label');
  el_type_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_type_label.innerHTML = "<br />&nbsp;&nbsp;Field type";

  var el_type_radio_own_select = document.createElement('input');
  el_type_radio_own_select.setAttribute("id", "el_type_radio_own_select");
  el_type_radio_own_select.setAttribute("type", "radio");
  el_type_radio_own_select.setAttribute("value", "own_select");
  el_type_radio_own_select.style.cssText = "margin-left:15px";
  el_type_radio_own_select.setAttribute("name", "el_type_radio_select");
  el_type_radio_own_select.setAttribute("onclick", "go_to_type_own_select('" + new_id + "')");
  el_type_radio_own_select.setAttribute("checked", "checked");
  Own_select = document.createTextNode("Custom Select");

  var el_type_radio_country = document.createElement('input');
  el_type_radio_country.setAttribute("id", "el_type_radio_country");
  el_type_radio_country.setAttribute("type", "radio");
  el_type_radio_country.setAttribute("value", "country");
  el_type_radio_country.style.cssText = "margin-left:15px";
  el_type_radio_country.setAttribute("name", "el_type_radio_select");
  el_type_radio_country.setAttribute("onclick", "go_to_type_country('" + new_id + "')");
  Country = document.createTextNode("Country List");

  var td = document.getElementById('edit_table');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  td.appendChild(el_type_label);
  td.appendChild(br1);
  td.appendChild(el_type_radio_own_select);
  td.appendChild(Own_select);
  td.appendChild(br2);
  td.appendChild(el_type_radio_country);
  td.appendChild(Country);
  var pos = document.getElementsByName("el_pos");
  pos[0].removeAttribute("disabled");
  pos[1].removeAttribute("disabled");
  pos[2].removeAttribute("disabled");
  var sel_el_pos = document.getElementById("sel_el_pos");
  sel_el_pos.removeAttribute("disabled", "disabled");
  go_to_type_own_select(new_id);
}

function go_to_type_own_select(new_id) {
  w_choices = [ "Select value", "option 1", "option 2"];
  w_choices_checked = ["1", "0", "0"];
  w_choices_disabled = [true, false, false];
  w_attr_name = [];
  w_attr_value = [];
  type_own_select(new_id, 'Select:', 'left', '200', w_choices, w_choices_checked, 'no', 'wdform_select', w_attr_name, w_attr_value, w_choices_disabled);
}

function go_to_type_country(new_id) {
  w_countries = ["", "Afghanistan", "Albania", "Algeria", "Andorra", "Angola", "Antigua and Barbuda", "Argentina", "Armenia", "Australia", "Austria", "Azerbaijan", "Bahamas", "Bahrain", "Bangladesh", "Barbados", "Belarus", "Belgium", "Belize", "Benin", "Bhutan", "Bolivia", "Bosnia and Herzegovina", "Botswana", "Brazil", "Brunei", "Bulgaria", "Burkina Faso", "Burundi", "Cambodia", "Cameroon", "Canada", "Cape Verde", "Central African Republic", "Chad", "Chile", "China", "Colombi", "Comoros", "Congo (Brazzaville)", "Congo", "Costa Rica", "Cote d'Ivoire", "Croatia", "Cuba", "Cyprus", "Czech Republic", "Denmark", "Djibouti", "Dominica", "Dominican Republic", "East Timor (Timor Timur)", "Ecuador", "Egypt", "El Salvador", "Equatorial Guinea", "Eritrea", "Estonia", "Ethiopia", "Fiji", "Finland", "France", "Gabon", "Gambia, The", "Georgia", "Germany", "Ghana", "Greece", "Grenada", "Guatemala", "Guinea", "Guinea-Bissau", "Guyana", "Haiti", "Honduras", "Hungary", "Iceland", "India", "Indonesia", "Iran", "Iraq", "Ireland", "Israel", "Italy", "Jamaica", "Japan", "Jordan", "Kazakhstan", "Kenya", "Kiribati", "Korea, North", "Korea, South", "Kuwait", "Kyrgyzstan", "Laos", "Latvia", "Lebanon", "Lesotho", "Liberia", "Libya", "Liechtenstein", "Lithuania", "Luxembourg", "Macedonia", "Madagascar", "Malawi", "Malaysia", "Maldives", "Mali", "Malta", "Marshall Islands", "Mauritania", "Mauritius", "Mexico", "Micronesia", "Moldova", "Monaco", "Mongolia", "Morocco", "Mozambique", "Myanmar", "Namibia", "Nauru", "Nepa", "Netherlands", "New Zealand", "Nicaragua", "Niger", "Nigeria", "Norway", "Oman", "Pakistan", "Palau", "Panama", "Papua New Guinea", "Paraguay", "Peru", "Philippines", "Poland", "Portugal", "Qatar", "Romania", "Russia", "Rwanda", "Saint Kitts and Nevis", "Saint Lucia", "Saint Vincent", "Samoa", "San Marino", "Sao Tome and Principe", "Saudi Arabia", "Senegal", "Serbia and Montenegro", "Seychelles", "Sierra Leone", "Singapore", "Slovakia", "Slovenia", "Solomon Islands", "Somalia", "South Africa", "Spain", "Sri Lanka", "Sudan", "Suriname", "Swaziland", "Sweden", "Switzerland", "Syria", "Taiwan", "Tajikistan", "Tanzania", "Thailand", "Togo", "Tonga", "Trinidad and Tobago", "Tunisia", "Turkey", "Turkmenistan", "Tuvalu", "Uganda", "Ukraine", "United Arab Emirates", "United Kingdom", "United States", "Uruguay", "Uzbekistan", "Vanuatu", "Vatican City", "Venezuela", "Vietnam", "Yemen", "Zambia", "Zimbabwe"];
  w_attr_name = [];
  w_attr_value = [];
  type_country(new_id, 'Country:', w_countries, 'left', '200', 'no', 'wdform_select', w_attr_name, w_attr_value);
}

/**
 * File upload field.
 */
function form_maker_el_file_upload() {
  alert(Drupal.t("This field type is disabled in free version. If you need this functionality, you need to buy the commercial version."));
  return;
}

/**
 * Captcha field.
 */
function form_maker_el_captcha() {
  // Edit table.
  if (document.getElementById("editing_id").value) {
    new_id = document.getElementById("editing_id").value;
  }
  else {
    new_id = gen;
  }
  if (document.getElementById('_wd_captchaform_id_temp')) {
    alert(Drupal.t("The captcha already has been created."));
    return;
  }
  if (document.getElementById('wd_recaptchaform_id_temp')) {
    alert(Drupal.t("The captcha already has been created."));
    return;
  }
  var el_type_label = document.createElement('label');
  el_type_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_type_label.innerHTML = "<br />&nbsp;&nbsp;Field type";

  var el_type_radio_captcha = document.createElement('input');
  el_type_radio_captcha.setAttribute("id", "el_type_captcha");
  el_type_radio_captcha.setAttribute("type", "radio");
  el_type_radio_captcha.setAttribute("value", "captcha");
  el_type_radio_captcha.style.cssText = "margin-left:15px";
  el_type_radio_captcha.setAttribute("name", "el_type_captcha");
  el_type_radio_captcha.setAttribute("onclick", "go_to_type_captcha('" + new_id + "')");
  el_type_radio_captcha.setAttribute("checked", "checked");
  Captcha = document.createTextNode("Simple Captcha");

  var el_type_radio_recaptcha = document.createElement('input');
  el_type_radio_recaptcha.setAttribute("id", "el_type_radio_recaptcha");
  el_type_radio_recaptcha.setAttribute("type", "radio");
  el_type_radio_recaptcha.setAttribute("value", "recaptcha");
  el_type_radio_recaptcha.style.cssText = "margin-left:15px";
  el_type_radio_recaptcha.setAttribute("name", "el_type_captcha");
  el_type_radio_recaptcha.setAttribute("onclick", "go_to_type_recaptcha('" + new_id + "')");
  Recaptcha_text = document.createTextNode("Recaptcha");

  var td = document.getElementById('edit_table');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');

  td.appendChild(el_type_label);
  td.appendChild(br1);
  td.appendChild(el_type_radio_captcha);
  td.appendChild(Captcha);
  td.appendChild(br2);
  td.appendChild(el_type_radio_recaptcha);
  td.appendChild(Recaptcha_text);


  var pos = document.getElementsByName("el_pos");
  pos[0].removeAttribute("disabled");
  pos[1].removeAttribute("disabled");
  pos[2].removeAttribute("disabled");

  w_attr_name = [];
  w_attr_value = [];
  go_to_type_captcha(new_id);
}

function go_to_type_captcha(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  type_captcha(new_id, 'Word Verification:', 'left', '6', '', w_attr_name, w_attr_value);
}

function go_to_type_recaptcha(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  type_recaptcha(new_id, 'Recaptcha Word Verification:', 'left', '', '', 'red', '', w_attr_name, w_attr_value);
}

/**
 * Page break field.
 */
function form_maker_el_page_break() {
  for (var t = form_view_max; t > 0; t--) {
    if (document.getElementById('form_id_tempform_view' + t)) {
      last_view = t;
      break;
    }
  }
  if (document.getElementById('form_id_tempform_view'+t).getAttribute('page_title')) {
		w_page_title = document.getElementById('form_id_tempform_view'+t).getAttribute('page_title');
  }
	else {
		w_page_title = 'Untitled Page';
  }
  w_title	= [ "Next","Previous"];
 	w_type	= ["button","button"];
 	w_class	= ["wdform_page_button","wdform_page_button"];
 	w_check	= ['false', 'false'];
  w_attr_name = [];
 	w_attr_value = [];
  type_page_break("0",w_page_title , w_title, w_type, w_class, w_check, w_attr_name, w_attr_value);
}

function type_page_break(i, w_page_title, w_title, w_type, w_class, w_check, w_attr_name, w_attr_value) {
  var pos = document.getElementsByName("el_pos");
  pos[0].setAttribute("disabled", "disabled");
  pos[1].setAttribute("disabled", "disabled");
  pos[2].setAttribute("disabled", "disabled");

  var sel_el_pos = document.getElementById("sel_el_pos");
  sel_el_pos.setAttribute("disabled", "disabled");

  document.getElementById("element_type").value = "type_page_break";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border:0px;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";

  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";
  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var el_page_title_label = document.createElement('label');
  el_page_title_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_page_title_label.innerHTML = "Page Title";

  var el_page_title_input = document.createElement('input');
  el_page_title_input.setAttribute("id", "el_page_title_input");
  el_page_title_input.setAttribute("type", "text");
  el_page_title_input.setAttribute("name", "el_page_title_input");
  el_page_title_input.setAttribute("value", w_page_title);
  el_page_title_input.style.cssText = "padding:0; border-width: 1px";
  el_page_title_input.setAttribute("onKeyup", "pagebreak_title_change(this.value,'" + i + "')");


  var el_type_next_label = document.createElement('label');
  el_type_next_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px;padding-top:10px;";
  el_type_next_label.innerHTML = "Next Type";

  var el_type_next_button = document.createElement('input');
  el_type_next_button.setAttribute("id", "el_type_next_button");
  el_type_next_button.setAttribute("type", "radio");
  el_type_next_button.setAttribute("name", "el_type_next");
  el_type_next_button.setAttribute("value", "button");
  el_type_next_button.style.cssText = "padding:0; border-width: 1px";
  el_type_next_button.setAttribute("onclick", "pagebreak_type_change('next','button')");
  Button_next = document.createTextNode("Button");

  var el_type_next_text = document.createElement('input');
  el_type_next_text.setAttribute("id", "el_type_next_text");
  el_type_next_text.setAttribute("type", "radio");
  el_type_next_text.setAttribute("name", "el_type_next");
  el_type_next_text.setAttribute("value", "text");
  el_type_next_text.style.cssText = " padding:0; border-width: 1px";
  el_type_next_text.setAttribute("onclick", "pagebreak_type_change('next','text')");
  Text_next = document.createTextNode("Text");

  var el_type_next_img = document.createElement('input');
  el_type_next_img.setAttribute("id", "el_type_next_img");
  el_type_next_img.setAttribute("type", "radio");
  el_type_next_img.setAttribute("name", "el_type_next");
  el_type_next_img.setAttribute("value", "img");
  el_type_next_img.style.cssText = "padding:0; border-width: 1px";
  el_type_next_img.setAttribute("onclick", "pagebreak_type_change('next','img')");
  Image_next = document.createTextNode("Image");

  if (w_type[0] == 'button') {
    el_type_next_button.setAttribute("checked", "checked");
  }
  else if (w_type[0] == 'text') {
    el_type_next_text.setAttribute("checked", "checked");
  }
  else {
    el_type_next_img.setAttribute("checked", "checked");
  }

  var el_title_next_label = document.createElement('label');
  el_title_next_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_title_next_label.setAttribute("id", "next_label");
  el_title_next_label.innerHTML = "Next name";

  var el_title_next = document.createElement('input');
  el_title_next.setAttribute("id", "el_title_next");
  el_title_next.setAttribute("type", "text");
  el_title_next.setAttribute("value", w_title[0]);
  el_title_next.style.cssText = "width:150px;   padding:0; border-width: 1px";
  el_title_next.setAttribute("onKeyUp", "change_pagebreak_label( this.value, 'next');");
  el_title_next.setAttribute("onChange", "change_pagebreak_label( this.value, 'next');");

  var el_style_next_label = document.createElement('label');
  el_style_next_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_next_label.innerHTML = "Next Class name";

  var el_style_next_textarea = document.createElement('input');
  el_style_next_textarea.setAttribute("id", "next_element_style");
  el_style_next_textarea.setAttribute("type", "text");
  el_style_next_textarea.setAttribute("value", w_class[0]);
  el_style_next_textarea.style.cssText = "width:100px; ";
  el_style_next_textarea.setAttribute("onChange", "change_pagebreak_class(this.value, 'next')");

  var el_check_next_label = document.createElement('label');
  el_check_next_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_check_next_label.innerHTML = "Check the required fields";

  var el_check_next_input = document.createElement('input');
  el_check_next_input.setAttribute("id", "el_check_next_input");
  el_check_next_input.setAttribute("type", "checkbox");
  el_check_next_input.setAttribute("onClick", "set_checkable('next');");

  if (w_check[0] == "true") {
    el_check_next_input.setAttribute("checked", "checked");
  }
  var el_type_previous_label = document.createElement('label');
  el_type_previous_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px;padding-top:10px; width:120px;";
  el_type_previous_label.innerHTML = "Previous Type";

  var el_type_previous_button = document.createElement('input');
  el_type_previous_button.setAttribute("id", "el_type_previous_button");
  el_type_previous_button.setAttribute("type", "radio");
  el_type_previous_button.setAttribute("name", "el_type_previous");
  el_type_previous_button.setAttribute("value", "button");
  el_type_previous_button.style.cssText = " padding:0; border-width: 1px";
  el_type_previous_button.setAttribute("onclick", "pagebreak_type_change('previous','button')");
  Button_previous = document.createTextNode("Button");

  var el_type_previous_text = document.createElement('input');
  el_type_previous_text.setAttribute("id", "el_type_previous_text");
  el_type_previous_text.setAttribute("type", "radio");
  el_type_previous_text.setAttribute("name", "el_type_previous");
  el_type_previous_text.setAttribute("value", "text");
  el_type_previous_text.style.cssText = " padding:0; border-width: 1px";
  el_type_previous_text.setAttribute("onclick", "pagebreak_type_change('previous','text')");
  Text_previous = document.createTextNode("Text");

  var el_type_previous_img = document.createElement('input');
  el_type_previous_img.setAttribute("id", "el_type_previous_img");
  el_type_previous_img.setAttribute("type", "radio");
  el_type_previous_img.setAttribute("name", "el_type_previous");
  el_type_previous_img.setAttribute("value", "img");
  el_type_previous_img.style.cssText = "  padding:0; border-width: 1px";
  el_type_previous_img.setAttribute("onclick", "pagebreak_type_change('previous','img')");
  Image_previous = document.createTextNode("Image");
  if (w_type[1] == 'button') {
    el_type_previous_button.setAttribute("checked", "checked");
  }
  else if (w_type[1] == 'text') {
    el_type_previous_text.setAttribute("checked", "checked");
  }
  else {
    el_type_previous_img.setAttribute("checked", "checked");
  }
  var el_title_previous_label = document.createElement('label');
  el_title_previous_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_title_previous_label.setAttribute("id", "previous_label");
  el_title_previous_label.innerHTML = "Previous name";
  var el_title_previous = document.createElement('input');
  el_title_previous.setAttribute("id", "el_title_previous");
  el_title_previous.setAttribute("type", "text");
  el_title_previous.setAttribute("value", w_title[1]);
  el_title_previous.style.cssText = "width:150px; padding:0; border-width: 1px";
  el_title_previous.setAttribute("onKeyUp", "change_pagebreak_label( this.value, 'previous');");
  el_title_previous.setAttribute("onChange", "change_pagebreak_label( this.value, 'previous');");
  var el_style_previous_label = document.createElement('label');
  el_style_previous_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_previous_label.innerHTML = "Previous Class name";
  var el_style_previous_textarea = document.createElement('input');
  el_style_previous_textarea.setAttribute("id", "previous_element_style");
  el_style_previous_textarea.setAttribute("type", "text");
  el_style_previous_textarea.setAttribute("value", w_class[1]);
  el_style_previous_textarea.style.cssText = "width:100px;";
  el_style_previous_textarea.setAttribute("onChange", "change_pagebreak_class(this.value, 'previous')");
  var el_check_previous_label = document.createElement('label');
  el_check_previous_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_check_previous_label.innerHTML = "Check the required fields";
  var el_check_previous_input = document.createElement('input');
  el_check_previous_input.setAttribute("id", "el_check_previous_input");
  el_check_previous_input.setAttribute("type", "checkbox");
  el_check_previous_input.setAttribute("onClick", "set_checkable('previous');");
  if (w_check[1] == "true") {
    el_check_previous_input.setAttribute("checked", "checked");
  }
  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr( 'type_checkbox')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";
  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";
  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);
  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';
    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');
    el_attr_name.setAttribute("type", "text");
    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_checkbox')");
    var el_attr_value = document.createElement('input');
    el_attr_value.setAttribute("type", "text");
    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_checkbox')");
    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_checkbox')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var t = document.getElementById('edit_table');
  var hr = document.createElement('hr');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');
  var br7 = document.createElement('br');
  var br8 = document.createElement('br');
  var br9 = document.createElement('br');
  var br10 = document.createElement('br');
  var br11 = document.createElement('br');
  var br12 = document.createElement('br');
  var br13 = document.createElement('br');
  var br14 = document.createElement('br');
  var br20 = document.createElement('br');
  var br21 = document.createElement('br');
  var br22 = document.createElement('br');
  edit_main_td1.appendChild(el_page_title_label);
  edit_main_td1_1.appendChild(el_page_title_input);
  edit_main_td3.appendChild(el_type_next_label);
  // edit_main_td3.appendChild(br10);
  edit_main_td3.appendChild(el_title_next_label);
  // edit_main_td3.appendChild(br11);
  edit_main_td3.appendChild(el_style_next_label);
  // edit_main_td3.appendChild(br12);
  edit_main_td3.appendChild(el_check_next_label);
  edit_main_td3_1.appendChild(el_type_next_button);
  edit_main_td3_1.appendChild(Button_next);
  edit_main_td3_1.appendChild(el_type_next_text);
  edit_main_td3_1.appendChild(Text_next);
  edit_main_td3_1.appendChild(el_type_next_img);
  edit_main_td3_1.appendChild(Image_next);
  edit_main_td3_1.appendChild(br);
  edit_main_td3_1.appendChild(el_title_next);
  edit_main_td3_1.appendChild(br4);
  edit_main_td3_1.appendChild(el_style_next_textarea);
  edit_main_td3_1.appendChild(br7);
  edit_main_td3_1.appendChild(el_check_next_input);
  //edit_main_td5.appendChild(hr);
  edit_main_td4.appendChild(el_type_previous_label);
  // edit_main_td4.appendChild(br20);
  edit_main_td4.appendChild(el_title_previous_label);
  // edit_main_td4.appendChild(br21);
  edit_main_td4.appendChild(el_style_previous_label);
  // edit_main_td4.appendChild(br22);
  edit_main_td4.appendChild(el_check_previous_label);
  edit_main_td4_1.appendChild(el_type_previous_button);
  edit_main_td4_1.appendChild(Button_previous);
  edit_main_td4_1.appendChild(el_type_previous_text);
  edit_main_td4_1.appendChild(Text_previous);
  edit_main_td4_1.appendChild(el_type_previous_img);
  edit_main_td4_1.appendChild(Image_previous);
  edit_main_td4_1.appendChild(el_title_previous);
  edit_main_td4_1.appendChild(br5);
  edit_main_td4_1.appendChild(el_style_previous_textarea);
  edit_main_td4_1.appendChild(br8);
  edit_main_td4_1.appendChild(el_check_previous_input);
  edit_main_td2.appendChild(el_attr_label);
  edit_main_td2.appendChild(el_attr_add);
  edit_main_td2.appendChild(br3);
  edit_main_td2.appendChild(el_attr_table);
  edit_main_td2.setAttribute("colspan", "2");
  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  //	edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr6.appendChild(edit_main_td6);
  //	edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr5.appendChild(edit_main_td5);
  //	edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr2);
  edit_div.appendChild(edit_main_table);
  t.appendChild(edit_div);
  //show table
  element = 'button';
  type = 'button';
  var div = document.createElement('div');
  div.setAttribute("id", "main_div");
  // tbody sarqac
  var table = document.createElement('table');
  table.setAttribute("id", "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');
  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'top');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", "_element_sectionform_id_temp");
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  //	table_little -@ sarqaca tbody table_little darela table_little_t
  var adding_next = document.createElement('div');
  adding_next.setAttribute("align", "right");
  adding_next.setAttribute("id", "_element_section_next");
  var adding_next_button = make_pagebreak_button('next', w_title[0], w_type[0], w_class[0], 0);
  adding_next.appendChild(adding_next_button);
  var adding_previous = document.createElement('div');
  adding_previous.setAttribute("align", "left");
  adding_previous.setAttribute("id", "_element_section_previous");
  var adding_previous_button = make_pagebreak_button('previous', w_title[1], w_type[1], w_class[1], 0);
  adding_previous.appendChild(adding_previous_button);
  var div_fields = document.createElement('div');
  div_fields.setAttribute("align", "center");
  div_fields.setAttribute("style", "border:2px solid blue;padding:20px; margin:20px");
  div_fields.innerHTML = 'FIELDS';
  var div_page_title = document.createElement('div');
  div_page_title.innerHTML = w_page_title + '<br/><br/>';
  div_page_title.setAttribute("id", "div_page_title");
  div_page_title.setAttribute("align", "center");
  var div_between = document.createElement('div');
  div_between.setAttribute("page_title", w_page_title);
  div_between.setAttribute("next_type", w_type[0]);
  div_between.setAttribute("next_title", w_title[0]);
  div_between.setAttribute("next_class", w_class[0]);
  div_between.setAttribute("next_checkable", w_check[0]);
  div_between.setAttribute("previous_type", w_type[1]);
  div_between.setAttribute("previous_title", w_title[1]);
  div_between.setAttribute("previous_class", w_class[1]);
  div_between.setAttribute("previous_checkable", w_check[1]);
  div_between.setAttribute("align", "center");
  div_between.setAttribute("id", "_div_between");
  div_between.innerHTML = "--------------------------------------<br />P A G E B R E A K<br />--------------------------------------"
  td2.appendChild(div_page_title);
  td2.appendChild(div_fields);
  td2.appendChild(adding_next);
  td2.appendChild(div_between);
  td2.appendChild(adding_previous);
  var main_td = document.getElementById('show_table');
  tr.appendChild(td2);
  table.appendChild(tr);
  div.appendChild(table);
  div.appendChild(br1);
  main_td.appendChild(div);
  refresh_attr(i, 'type_page_break');
}

function pagebreak_type_change(pagebreak_type, button_type) {
  document.getElementById("_div_between").setAttribute(pagebreak_type + '_type', button_type);
  switch (button_type) {
    case 'button': {
      document.getElementById("_div_between").setAttribute(pagebreak_type + '_title', pagebreak_type);
      var el_title_label = document.createElement('label');
      el_title_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
      el_title_label.setAttribute('id', pagebreak_type + "_label");
      el_title_label.setAttribute('type', "button");
      el_title_label.innerHTML = pagebreak_type + " " + button_type + " name";
      document.getElementById(pagebreak_type + "_label").parentNode.replaceChild(el_title_label, document.getElementById(pagebreak_type + "_label"));
      document.getElementById("el_title_" + pagebreak_type).value = pagebreak_type;
      var element = document.createElement('button');
      element.setAttribute('id', "page_" + pagebreak_type + '_0');
      element.setAttribute('class', document.getElementById("_div_between").getAttribute(pagebreak_type + '_class'));
      element.style.cursor = "pointer";
      element.innerHTML = pagebreak_type;
      document.getElementById("_element_section_" + pagebreak_type).replaceChild(element, document.getElementById("page_" + pagebreak_type + '_0'));
      break;

    }
    case 'text': {
      document.getElementById("_div_between").setAttribute(pagebreak_type + '_title', pagebreak_type);
      var el_title_label = document.createElement('label');
      el_title_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
      el_title_label.setAttribute('id', pagebreak_type + "_label");
      el_title_label.innerHTML = pagebreak_type + " " + button_type + " name";
      document.getElementById(pagebreak_type + "_label").parentNode.replaceChild(el_title_label, document.getElementById(pagebreak_type + "_label"));
      document.getElementById("el_title_" + pagebreak_type).value = pagebreak_type;
      var element = document.createElement('span');
      element.setAttribute('id', "page_" + pagebreak_type + '_0');
      element.setAttribute('class', document.getElementById("_div_between").getAttribute(pagebreak_type + '_class'));
      element.style.cursor = "pointer";
      element.innerHTML = pagebreak_type;
      document.getElementById("_element_section_" + pagebreak_type).replaceChild(element, document.getElementById("page_" + pagebreak_type + '_0'));
      break;

    }
    case 'img': {
      document.getElementById("_div_between").setAttribute(pagebreak_type + '_title', '' + Drupal.settings.form_maker.get_module_path + '/images/' + pagebreak_type + '.png');
      var el_title_label = document.createElement('label');
      el_title_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
      el_title_label.setAttribute('id', pagebreak_type + "_label");
      el_title_label.innerHTML = pagebreak_type + " " + button_type + " src";
      document.getElementById(pagebreak_type + "_label").parentNode.replaceChild(el_title_label, document.getElementById(pagebreak_type + "_label"));
      document.getElementById("el_title_" + pagebreak_type).value = '' + Drupal.settings.form_maker.get_module_path + '/images/' + pagebreak_type + '.png';
      var element = document.createElement('img');
      element.setAttribute('id', "page_" + pagebreak_type + '_0');
      element.setAttribute('class', document.getElementById("_div_between").getAttribute(pagebreak_type + '_class'));      element.style.cursor = "pointer";
      element.src = '' + Drupal.settings.form_maker.get_module_path + '/images/' + pagebreak_type + '.png';
      document.getElementById("_element_section_" + pagebreak_type).replaceChild(element, document.getElementById("page_" + pagebreak_type + '_0'));
      break;

    }
  }
}

function change_pagebreak_label(val, type) {
  button_type = document.getElementById("_div_between").getAttribute(type + '_type');
  if (button_type != "img") {
    document.getElementById("page_" + type + '_0').value = val;
    document.getElementById("page_" + type + '_0').innerHTML = val;
  }
  else {
    document.getElementById("page_" + type + '_0').src = val;
  }
  document.getElementById("_div_between").setAttribute(type + '_title',val);
}

function change_pagebreak_class(val, type) {
  document.getElementById("page_" + type + '_0').setAttribute('class', val);
  document.getElementById("_div_between").setAttribute(type + '_class',val);
}

function set_checkable(type) {
  document.getElementById("_div_between").setAttribute(type + '_checkable',document.getElementById("el_check_" + type + "_input").checked);
}

function change_attribute_value(id, x, type) {
  if (!document.getElementById("attr_name" + x).value) {
    alert(Drupal.t('The name of the attribute is required.'));
    return
  }
  if (document.getElementById("attr_name" + x).value.toLowerCase() == "style") {
    alert(Drupal.t('Sorry, you cannot add a style attribute here. Use "Class name" instead.'));
    return
  }
  refresh_attr(id, type);
}

function change_attribute_name(id, x, type) {
  value = x.value;
  if (!value) {
    alert(Drupal.t('The name of the attribute is required.'));
    return;
  }
  if (value.toLowerCase() == "style") {
    alert(Drupal.t('Sorry, you cannot add a style attribute here. Use "Class name" instead.'));
    return;
  }
  if (value == parseInt(value)) {
    alert(Drupal.t('The name of the attribute cannot be a number.'));
    return;
  }
  if (value.indexOf(" ") != -1) {
    var regExp = /\s+/g;
    value = value.replace(regExp, '');
    x.value = value;
    alert(Drupal.t("The name of the attribute cannot contain a space."));
    refresh_attr(id, type);
    return;
  }
  refresh_attr(id, type);
}

function remove_attr(id, el_id, type) {
  tr = document.getElementById("attr_row_" + id);
  tr.parentNode.removeChild(tr);
  refresh_attr(el_id, type);
}

function refresh_attr(x, type) {
  switch (type) {
    case "type_text": {
      id_array = Array();
      id_array[0] = x + '_elementform_id_temp';
      break;

    }
    case "type_name": {
      id_array = Array();
      id_array[0] = x + '_element_firstform_id_temp';
      id_array[1] = x + '_element_lastform_id_temp';
      id_array[2] = x + '_element_titleform_id_temp';
      id_array[3] = x + '_element_middleform_id_temp';
      break;

    }
    case "type_address": {
      id_array = Array();
      id_array[0] = x + '_street1form_id_temp';
      id_array[1] = x + '_street2form_id_temp';
      id_array[2] = x + '_cityform_id_temp';
      id_array[3] = x + '_stateform_id_temp';
      id_array[4] = x + '_postalform_id_temp';
      id_array[5] = x + '_countryform_id_temp';
      break;

    }
    case "type_checkbox": {
      id_array = Array();
      for (z = 0; z < 50; z++) {
        id_array[z] = x + '_elementform_id_temp' + z;
      }
      break;

    }
    case "type_time": {
      id_array = Array();
      id_array[0] = x + '_hhform_id_temp';
      id_array[1] = x + '_mmform_id_temp';
      id_array[2] = x + '_ssform_id_temp';
      id_array[3] = x + '_am_pmform_id_temp';
      break;

    }
    case "type_date": {
      id_array = Array();
      id_array[0] = x + '_elementform_id_temp';
      id_array[1] = x + '_buttonform_id_temp';
      break;

    }
    case "type_date_fields": {
      id_array = Array();
      id_array[0] = x + '_dayform_id_temp';
      id_array[1] = x + '_monthform_id_temp';
      id_array[2] = x + '_yearform_id_temp';
      break;

    }
    case "type_captcha":  {
      id_array = Array();
      id_array[0] = '_wd_captchaform_id_temp';
      id_array[1] = '_wd_captcha_inputform_id_temp';
      id_array[2] = '_element_refreshform_id_temp';
      break;

    }
    case "type_recaptcha": {
      id_array = Array();
      id_array[0] = 'wd_recaptchaform_id_temp';
      break;

    }
    case "type_submit_reset": {
      id_array = Array();
      id_array[0] = x + '_element_submitform_id_temp';
      id_array[1] = x + '_element_resetform_id_temp';
      break;

    }
    case "type_page_break": {
      id_array = Array();
      id_array[0] = '_div_between';
      break;
    }
  }
  for (q = 0; q < id_array.length; q++) {
    id = id_array[q];
    var input = document.getElementById(id);
    if (input) {
      atr = input.attributes;
      for (i = 0; i < 30; i++) {
        if (atr[i]) {
          if (atr[i].name.indexOf("add_") == 0) {
            input.removeAttribute(atr[i].name);
            i--;
          }
        }
      }
      for (i = 0; i < 10; i++) {
        if (document.getElementById("attr_name" + i)) {
          try {
            input.setAttribute("add_" + document.getElementById("attr_name" + i).value, document.getElementById("attr_value" + i).value)
          }
          catch (err) {
            alert(Drupal.t('Only letters, numbers, hyphens and underscores are allowed.'));
          }
        }
      }
    }
  }
}

function make_pagebreak_button(next_or_previous, title, type, class_, id) {
  switch (type) {
    case 'button': {
      var element = document.createElement('button');
      element.setAttribute('id', "page_" + next_or_previous + "_" + id);
      element.setAttribute('type', "button");
      element.setAttribute('class', class_);
      element.style.cursor = "pointer";
      element.innerHTML = title;
      return element;
      break;

    }
    case 'text': {
      var element = document.createElement('span');
      element.setAttribute('id', "page_" + next_or_previous + "_" + id);
      element.setAttribute('class', class_);
      element.style.cursor = "pointer";
      element.innerHTML = title;
      return element;
      break;

    }
    case 'img': {
      var element = document.createElement('img');
      element.setAttribute('id', "page_" + next_or_previous + "_" + id);
      element.setAttribute('class', class_);
      element.style.cursor = "pointer";
      element.src = title;
      return element;
      break;
    }
  }
}

function type_text(i, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_required, w_unique, w_class, w_attr_name, w_attr_value) {
  element_ids = [ 'option1', 'option2'];
  document.getElementById("element_type").value = "type_text";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_tr8 = document.createElement('tr');
  edit_main_tr8.setAttribute("valing", "top");

  var edit_main_tr9 = document.createElement('tr');
  edit_main_tr9.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";

  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var edit_main_td8 = document.createElement('td');
  edit_main_td8.style.cssText = "padding-top:10px";
  var edit_main_td8_1 = document.createElement('td');
  edit_main_td8_1.style.cssText = "padding-top:10px";

  var edit_main_td9 = document.createElement('td');
  edit_main_td9.style.cssText = "padding-top:10px";
  var edit_main_td9_1 = document.createElement('td');
  edit_main_td9_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');

  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");
  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }

  var el_size_label = document.createElement('label');
  el_size_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_size_label.innerHTML = "Field size(px) ";
  var el_size = document.createElement('input');
  el_size.setAttribute("id", "edit_for_input_size");
  el_size.setAttribute("type", "text");
  el_size.setAttribute("value", w_size);

  el_size.setAttribute("name", "edit_for_size");
  el_size.setAttribute("onKeyPress", "return check_isnum(event)");
  el_size.setAttribute("onKeyUp", "change_w_style('" + i + "_elementform_id_temp', this.value)");

  var el_first_value_label = document.createElement('label');
  el_first_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_first_value_label.innerHTML = "Value if empty ";

  var el_first_value_input = document.createElement('input');
  el_first_value_input.setAttribute("id", "el_first_value_input");
  el_first_value_input.setAttribute("type", "text");
  el_first_value_input.setAttribute("value", w_title);
  el_first_value_input.style.cssText = "width:200px;";
  el_first_value_input.setAttribute("onKeyUp", "change_input_value(this.value,'" + i + "_elementform_id_temp')");

  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";

  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes") {
    el_required.setAttribute("checked", "checked");
  }

  var el_unique_label = document.createElement('label');
  el_unique_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_unique_label.innerHTML = "Allow only unique values";

  var el_unique = document.createElement('input');
  el_unique.setAttribute("id", "el_send");
  el_unique.setAttribute("type", "checkbox");
  el_unique.setAttribute("value", "yes");
  el_unique.setAttribute("onclick", "set_unique('" + i + "_uniqueform_id_temp')");
  if (w_unique == "yes") {
    el_unique.setAttribute("checked", "checked");
  }

  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Deactive Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", "input_deactive");
  el_style_textarea.setAttribute("disabled", "disabled");
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_style_label2 = document.createElement('label');
  el_style_label2.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label2.innerHTML = "Active Class name";

  var el_style_textarea2 = document.createElement('input');
  el_style_textarea2.setAttribute("id", "element_style");
  el_style_textarea2.setAttribute("type", "text");
  el_style_textarea2.setAttribute("value", "input_active");
  el_style_textarea2.setAttribute("disabled", "disabled");
  el_style_textarea2.style.cssText = "width:200px;";
  el_style_textarea2.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";

  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_text')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");
    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_text')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");
    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_text')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_text')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var t = document.getElementById('edit_table');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');
  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);
  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2.appendChild(br1);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);
  edit_main_td3.appendChild(el_size_label);
  edit_main_td3_1.appendChild(el_size);
  edit_main_td4.appendChild(el_first_value_label);
  edit_main_td4_1.appendChild(el_first_value_input);
  edit_main_td5.appendChild(el_style_label);
  edit_main_td5_1.appendChild(el_style_textarea);
  edit_main_td9.appendChild(el_style_label2);
  edit_main_td9_1.appendChild(el_style_textarea2);
  edit_main_td6.appendChild(el_required_label);
  edit_main_td6_1.appendChild(el_required);
  edit_main_td8.appendChild(el_unique_label);
  edit_main_td8_1.appendChild(el_unique);
  edit_main_td7.appendChild(el_attr_label);
  edit_main_td7.appendChild(el_attr_add);
  edit_main_td7.appendChild(br6);
  edit_main_td7.appendChild(el_attr_table);
  edit_main_td7.setAttribute("colspan", "2");
  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr8.appendChild(edit_main_td8);
  edit_main_tr8.appendChild(edit_main_td8_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_tr9.appendChild(edit_main_td9);
  edit_main_tr9.appendChild(edit_main_td9_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr9);
  edit_main_table.appendChild(edit_main_tr6);
  edit_main_table.appendChild(edit_main_tr8);
  edit_main_table.appendChild(edit_main_tr7);
  edit_div.appendChild(edit_main_table);
  t.appendChild(edit_div);
  add_id_and_name(i, 'type_text');

  // Show table.
  element = 'input';
  type = 'text';
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_text");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");

  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");
  adding_required.setAttribute("id", i + "_requiredform_id_temp");

  var adding_unique = document.createElement("input");
  adding_unique.setAttribute("type", "hidden");
  adding_unique.setAttribute("value", w_unique);
  adding_unique.setAttribute("name", i + "_uniqueform_id_temp");
  adding_unique.setAttribute("id", i + "_uniqueform_id_temp");

  var adding = document.createElement(element);
  adding.setAttribute("type", type);

  if (w_title == w_first_val) {
    adding.style.cssText = "width:" + w_size + "px;";
    adding.setAttribute("class", "input_deactive");
  }
  else {
    adding.style.cssText = "width:" + w_size + "px;";
    adding.setAttribute("class", "input_active");
  }
  adding.setAttribute("id", i + "_elementform_id_temp");
  adding.setAttribute("name", i + "_elementform_id_temp");
  adding.setAttribute("value", w_first_val);
  adding.setAttribute("title", w_title);
  adding.setAttribute("onFocus", 'delete_value("' + i + '_elementform_id_temp")');
  adding.setAttribute("onBlur", 'return_value("' + i + '_elementform_id_temp")');
  adding.setAttribute("onChange", 'change_value("' + i + '_elementform_id_temp")');
  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');

  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'middle');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'middle');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');


  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");

  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes") {
    required.innerHTML = " *";
  }
  var main_td = document.getElementById('show_table');
  td1.appendChild(label);
  td1.appendChild(required);
  td2.appendChild(adding_type);
  td2.appendChild(adding_required);
  td2.appendChild(adding_unique);
  td2.appendChild(adding);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);
  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  if (w_field_label_pos == "top") {
    label_top(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_text');
}

function type_number(i, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_required, w_unique, w_class, w_attr_name, w_attr_value) {
  element_ids = [ 'option1', 'option2'];
  document.getElementById("element_type").value = "type_number";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_tr8 = document.createElement('tr');
  edit_main_tr8.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var edit_main_td8 = document.createElement('td');
  edit_main_td8.style.cssText = "padding-top:10px";
  var edit_main_td8_1 = document.createElement('td');
  edit_main_td8_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }
  var el_size_label = document.createElement('label');
  el_size_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_size_label.innerHTML = "Field size(px) ";
  var el_size = document.createElement('input');
  el_size.setAttribute("id", "edit_for_input_size");
  el_size.setAttribute("type", "text");
  el_size.setAttribute("value", w_size);

  el_size.setAttribute("name", "edit_for_size");
  el_size.setAttribute("onKeyPress", "return check_isnum(event)");
  el_size.setAttribute("onKeyUp", "change_w_style('" + i + "_elementform_id_temp', this.value)");

  var el_first_value_label = document.createElement('label');
  el_first_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_first_value_label.innerHTML = "Value if empty ";

  var el_first_value_input = document.createElement('input');
  el_first_value_input.setAttribute("id", "el_first_value_input");
  el_first_value_input.setAttribute("type", "text");
  el_first_value_input.setAttribute("value", w_title);
  el_first_value_input.style.cssText = "width:200px;";
  el_first_value_input.setAttribute("onKeyUp", "change_input_value(this.value,'" + i + "_elementform_id_temp')");
  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";

  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes") {
    el_required.setAttribute("checked", "checked");
  }
  var el_unique_label = document.createElement('label');
  el_unique_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_unique_label.innerHTML = "Allow only unique values";

  var el_unique = document.createElement('input');
  el_unique.setAttribute("id", "el_send");
  el_unique.setAttribute("type", "checkbox");
  el_unique.setAttribute("value", "yes");
  el_unique.setAttribute("onclick", "set_unique('" + i + "_uniqueform_id_temp')");
  if (w_unique == "yes") {
    el_unique.setAttribute("checked", "checked");
  }
  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";

  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_text')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");
    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_text')");
    var el_attr_value = document.createElement('input');
    el_attr_value.setAttribute("type", "text");
    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_text')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_text')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var t = document.getElementById('edit_table');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');
  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);
  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);
  edit_main_td3.appendChild(el_size_label);
  edit_main_td3_1.appendChild(el_size);
  edit_main_td4.appendChild(el_first_value_label);
  edit_main_td4_1.appendChild(el_first_value_input);
  edit_main_td5.appendChild(el_style_label);
  edit_main_td5_1.appendChild(el_style_textarea);
  edit_main_td6.appendChild(el_required_label);
  edit_main_td6_1.appendChild(el_required);
  edit_main_td8.appendChild(el_unique_label);
  edit_main_td8_1.appendChild(el_unique);
  edit_main_td7.appendChild(el_attr_label);
  edit_main_td7.appendChild(el_attr_add);
  edit_main_td7.appendChild(br6);
  edit_main_td7.appendChild(el_attr_table);
  edit_main_td7.setAttribute("colspan", "2");
  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr8.appendChild(edit_main_td8);
  edit_main_tr8.appendChild(edit_main_td8_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr6);
  edit_main_table.appendChild(edit_main_tr8);
  edit_main_table.appendChild(edit_main_tr7);
  edit_div.appendChild(edit_main_table);
  t.appendChild(edit_div);
  add_id_and_name(i, 'type_text');
  // Show table.
  element = 'input';
  type = 'text';
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_number");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");

  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");
  adding_required.setAttribute("id", i + "_requiredform_id_temp");

  var adding_unique = document.createElement("input");
  adding_unique.setAttribute("type", "hidden");
  adding_unique.setAttribute("value", w_unique);
  adding_unique.setAttribute("name", i + "_uniqueform_id_temp");
  adding_unique.setAttribute("id", i + "_uniqueform_id_temp");


  var adding = document.createElement(element);
  adding.setAttribute("type", type);

  if (w_title == w_first_val) {
    adding.style.cssText = "width:" + w_size + "px;";
    adding.setAttribute("class", "input_deactive");
  }
  else {
    adding.style.cssText = "width:" + w_size + "px;";
    adding.setAttribute("class", "input_active");
  }
  adding.setAttribute("id", i + "_elementform_id_temp");
  adding.setAttribute("name", i + "_elementform_id_temp");
  adding.setAttribute("value", w_first_val);
  adding.setAttribute("title", w_title);
  adding.setAttribute("onKeyPress", "return check_isnum(event)");
  adding.setAttribute("onFocus", 'delete_value("' + i + '_elementform_id_temp")');
  adding.setAttribute("onBlur", 'return_value("' + i + '_elementform_id_temp")');
  adding.setAttribute("onChange", 'change_value("' + i + '_elementform_id_temp")');

  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');

  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'middle');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'middle');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");

  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes") {
    required.innerHTML = " *";
  }
  var main_td = document.getElementById('show_table');
  td1.appendChild(label);
  td1.appendChild(required);
  td2.appendChild(adding_type);
  td2.appendChild(adding_required);
  td2.appendChild(adding_unique);
  td2.appendChild(adding);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);
  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  if (w_field_label_pos == "top") {
    label_top(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_text');
}

function type_password(i, w_field_label, w_field_label_pos, w_size, w_required, w_unique, w_class, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_password";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";

  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px;";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }
  var el_size_label = document.createElement('label');
  el_size_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_size_label.innerHTML = "Field size(px) ";

  var el_size = document.createElement('input');
  el_size.setAttribute("id", "edit_for_input_size");
  el_size.setAttribute("type", "text");
  el_size.setAttribute("value", w_size);

  el_size.setAttribute("name", "edit_for_size");
  el_size.setAttribute("onKeyPress", "return check_isnum(event)");
  el_size.setAttribute("onKeyUp", "change_w_style('" + i + "_elementform_id_temp', this.value)");

  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";

  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes") {
    el_required.setAttribute("checked", "checked");
  }
  var el_unique_label = document.createElement('label');
  el_unique_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_unique_label.innerHTML = "Allow only unique values";

  var el_unique = document.createElement('input');
  el_unique.setAttribute("id", "el_send");
  el_unique.setAttribute("type", "checkbox");
  el_unique.setAttribute("value", "yes");
  el_unique.setAttribute("onclick", "set_unique('" + i + "_uniqueform_id_temp')");
  if (w_unique == "yes") {
    el_unique.setAttribute("checked", "checked");
  }
  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");
  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_text')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");
    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_text')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");
    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_text')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_text')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }

  var t = document.getElementById('edit_table');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');

  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);
  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);
  edit_main_td3.appendChild(el_size_label);
  edit_main_td3_1.appendChild(el_size);

  edit_main_td4.appendChild(el_style_label);
  edit_main_td4_1.appendChild(el_style_textarea);

  edit_main_td5.appendChild(el_required_label);
  edit_main_td5_1.appendChild(el_required);

  edit_main_td7.appendChild(el_unique_label);
  edit_main_td7_1.appendChild(el_unique);

  edit_main_td6.appendChild(el_attr_label);
  edit_main_td6.appendChild(el_attr_add);
  edit_main_td6.appendChild(br3);
  edit_main_td6.appendChild(el_attr_table);
  edit_main_td6.setAttribute("colspan", "2");


  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_tr7.appendChild(edit_main_td7_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr7);
  edit_main_table.appendChild(edit_main_tr6);
  edit_div.appendChild(edit_main_table);

  t.appendChild(edit_div);
  add_id_and_name(i, 'type_text');

  // Show table.

  element = 'input';
  type = 'password';
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_password");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");
  adding_required.setAttribute("id", i + "_requiredform_id_temp");

  var adding_unique = document.createElement("input");
  adding_unique.setAttribute("type", "hidden");
  adding_unique.setAttribute("value", w_unique);
  adding_unique.setAttribute("name", i + "_uniqueform_id_temp");
  adding_unique.setAttribute("id", i + "_uniqueform_id_temp");

  var adding = document.createElement(element);
  adding.setAttribute("type", type);
  adding.setAttribute("id", i + "_elementform_id_temp");
  adding.setAttribute("name", i + "_elementform_id_temp");
  adding.style.cssText = "width:" + w_size + "px;";

  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');

  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'middle');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'middle');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");

  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes") {
    required.innerHTML = " *";
  }
  var main_td = document.getElementById('show_table');

  td1.appendChild(label);
  td1.appendChild(required);
  td2.appendChild(adding_type);
  td2.appendChild(adding_required);
  td2.appendChild(adding_unique);
  td2.appendChild(adding);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);

  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  if (w_field_label_pos == "top") {
    label_top(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_text');
}

function type_textarea(i, w_field_label, w_field_label_pos, w_size_w, w_size_h, w_first_val, w_title, w_required, w_unique, w_class, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_textarea";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");
  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_tr8 = document.createElement('tr');
  edit_main_tr8.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var edit_main_td8 = document.createElement('td');
  edit_main_td8.style.cssText = "padding-top:10px";
  var edit_main_td8_1 = document.createElement('td');
  edit_main_td8_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px;";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }
  var el_size_label = document.createElement('label');
  el_size_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_size_label.innerHTML = "Field size(px) ";

  var el_size_w = document.createElement('input');
  el_size_w.setAttribute("id", "edit_for_input_size");
  el_size_w.setAttribute("type", "text");
  el_size_w.setAttribute("value", w_size_w);
  el_size_w.style.cssText = "margin-right:2px; width: 60px";
  el_size_w.setAttribute("name", "edit_for_size");
  el_size_w.setAttribute("onKeyPress", "return check_isnum(event)");
  el_size_w.setAttribute("onKeyUp", "change_w_style('" + i + "_elementform_id_temp', this.value)");

  X = document.createTextNode("x");

  var el_size_h = document.createElement('input');
  el_size_h.setAttribute("id", "edit_for_input_size");
  el_size_h.setAttribute("type", "text");
  el_size_h.setAttribute("value", w_size_h);
  el_size_h.style.cssText = "margin-left:2px;  width:60px";
  el_size_h.setAttribute("name", "edit_for_size");
  el_size_h.setAttribute("onKeyPress", "return check_isnum(event)");
  el_size_h.setAttribute("onKeyUp", "change_h_style('" + i + "_elementform_id_temp', this.value)");
  var el_first_value_label = document.createElement('label');
  el_first_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_first_value_label.innerHTML = "Value if empty";

  var el_first_value_input = document.createElement('input');
  el_first_value_input.setAttribute("id", "el_first_value_input");
  el_first_value_input.setAttribute("type", "text");
  el_first_value_input.setAttribute("value", w_title);
  el_first_value_input.style.cssText = "width:200px;";
  el_first_value_input.setAttribute("onKeyUp", "change_input_value(this.value,'" + i + "_elementform_id_temp')");
  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";

  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes") {
    el_required.setAttribute("checked", "checked");
  }
  var el_unique_label = document.createElement('label');
  el_unique_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_unique_label.innerHTML = "Allow only unique values";

  var el_unique = document.createElement('input');
  el_unique.setAttribute("id", "el_send");
  el_unique.setAttribute("type", "checkbox");
  el_unique.setAttribute("value", "yes");
  el_unique.setAttribute("onclick", "set_unique('" + i + "_uniqueform_id_temp')");
  if (w_unique == "yes") {
    el_unique.setAttribute("checked", "checked");
  }
  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";

  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_text')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");
    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_text')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");
    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_text')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_text')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }

  var t = document.getElementById('edit_table');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');

  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);

  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);

  edit_main_td3.appendChild(el_size_label);
  edit_main_td3_1.appendChild(el_size_w);
  edit_main_td3_1.appendChild(X);
  edit_main_td3_1.appendChild(el_size_h);

  edit_main_td4.appendChild(el_first_value_label);
  edit_main_td4_1.appendChild(el_first_value_input);

  edit_main_td5.appendChild(el_style_label);
  edit_main_td5_1.appendChild(el_style_textarea);

  edit_main_td6.appendChild(el_required_label);
  edit_main_td6_1.appendChild(el_required);

  edit_main_td8.appendChild(el_unique_label);
  edit_main_td8_1.appendChild(el_unique);

  edit_main_td7.appendChild(el_attr_label);
  edit_main_td7.appendChild(el_attr_add);
  edit_main_td7.appendChild(br6);
  edit_main_td7.appendChild(el_attr_table);
  edit_main_td7.setAttribute("colspan", "2");

  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_tr7.appendChild(edit_main_td7_1);
  edit_main_tr8.appendChild(edit_main_td8);
  edit_main_tr8.appendChild(edit_main_td8_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr6);
  edit_main_table.appendChild(edit_main_tr8);
  edit_main_table.appendChild(edit_main_tr7);
  edit_div.appendChild(edit_main_table);

  t.appendChild(edit_div);
  add_id_and_name(i, 'type_text');

  // Show table.
  element = 'textarea';
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_textarea");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");
  adding_required.setAttribute("id", i + "_requiredform_id_temp");

  var adding_unique = document.createElement("input");
  adding_unique.setAttribute("type", "hidden");
  adding_unique.setAttribute("value", w_unique);
  adding_unique.setAttribute("name", i + "_uniqueform_id_temp");
  adding_unique.setAttribute("id", i + "_uniqueform_id_temp");

  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');
  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'top');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'top');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");

  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes") {
    required.innerHTML = " *";
  }
  var adding = document.createElement(element);
  if (w_title == w_first_val) {
    adding.style.cssText = "width:" + w_size_w + "px; height:" + w_size_h + "px;";
    adding.setAttribute("class", "input_deactive");
  }
  else {
    adding.style.cssText = "width:" + w_size_w + "px; height:" + w_size_h + "px;";
    adding.setAttribute("class", "input_active");
  }
  adding.setAttribute("id", i + "_elementform_id_temp");
  adding.setAttribute("name", i + "_elementform_id_temp");
  adding.setAttribute("title", w_title);
  adding.setAttribute("value", w_first_val);
  adding.setAttribute("onFocus", "delete_value('" + i + "_elementform_id_temp')");
  adding.setAttribute("onBlur", "return_value('" + i + "_elementform_id_temp')");
  adding.setAttribute("onChange", "change_value('" + i + "_elementform_id_temp')");
  adding.innerHTML = w_first_val;
  var main_td = document.getElementById('show_table');
  td1.appendChild(label);
  td1.appendChild(required);
  td2.appendChild(adding_type);

  td2.appendChild(adding_required);
  td2.appendChild(adding_unique);
  td2.appendChild(adding);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);

  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  if (w_field_label_pos == "top") {
    label_top(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_text');
}

function type_phone(i, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_required, w_unique, w_class, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_phone";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_tr8 = document.createElement('tr');
  edit_main_tr8.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var edit_main_td8 = document.createElement('td');
  edit_main_td8.style.cssText = "padding-top:10px";
  var edit_main_td8_1 = document.createElement('td');
  edit_main_td8_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px;";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }
  var el_size_label = document.createElement('label');
  el_size_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_size_label.innerHTML = "Field size(px) ";
  var el_size = document.createElement('input');
  el_size.setAttribute("id", "edit_for_input_size");
  el_size.setAttribute("type", "text");
  el_size.setAttribute("value", w_size);
  el_size.setAttribute("name", "edit_for_size");
  el_size.setAttribute("onKeyPress", "return check_isnum(event)");
  el_size.setAttribute("onKeyUp", "change_w_style('" + i + "_element_lastform_id_temp', this.value);");

  var gic = document.createTextNode("-");

  var el_first_value_label = document.createElement('label');
  el_first_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_first_value_label.innerHTML = "Value if empty ";

  var el_first_value_area = document.createElement('input');
  el_first_value_area.setAttribute("id", "el_first_value_area");
  el_first_value_area.setAttribute("type", "text");
  el_first_value_area.setAttribute("value", w_title[0]);
  el_first_value_area.style.cssText = "width:50px; margin-right:4px";
  el_first_value_area.setAttribute("onKeyUp", "change_input_value(this.value,'" + i + "_element_firstform_id_temp')");

  var el_first_value_phone = document.createElement('input');
  el_first_value_phone.setAttribute("id", "el_first_value_phone");
  el_first_value_phone.setAttribute("type", "text");
  el_first_value_phone.setAttribute("value", w_title[1]);
  el_first_value_phone.style.cssText = "width:100px; margin-left:4px";
  el_first_value_phone.setAttribute("onKeyUp", "change_input_value(this.value,'" + i + "_element_lastform_id_temp')");


  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";

  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes")
    el_required.setAttribute("checked", "checked");

  var el_unique_label = document.createElement('label');
  el_unique_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_unique_label.innerHTML = "Allow only unique values";

  var el_unique = document.createElement('input');
  el_unique.setAttribute("id", "el_send");
  el_unique.setAttribute("type", "checkbox");
  el_unique.setAttribute("value", "yes");
  el_unique.setAttribute("onclick", "set_unique('" + i + "_uniqueform_id_temp')");
  if (w_unique == "yes") {
    el_unique.setAttribute("checked", "checked");
  }

  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";

  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_name')");

  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_name')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");

    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_name')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_name')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }

  var t = document.getElementById('edit_table');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');

  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);

  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);

  edit_main_td3.appendChild(el_first_value_label);
  edit_main_td3_1.appendChild(el_first_value_area);
  edit_main_td3_1.appendChild(gic);
  edit_main_td3_1.appendChild(el_first_value_phone);


  edit_main_td7.appendChild(el_size_label);
  edit_main_td7_1.appendChild(el_size);

  edit_main_td4.appendChild(el_style_label);
  edit_main_td4_1.appendChild(el_style_textarea);

  edit_main_td5.appendChild(el_required_label);
  edit_main_td5_1.appendChild(el_required);

  edit_main_td8.appendChild(el_unique_label);
  edit_main_td8_1.appendChild(el_unique);

  edit_main_td6.appendChild(el_attr_label);
  edit_main_td6.appendChild(el_attr_add);
  edit_main_td6.appendChild(br3);
  edit_main_td6.appendChild(el_attr_table);
  edit_main_td6.setAttribute("colspan", "2");

  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr8.appendChild(edit_main_td8);
  edit_main_tr8.appendChild(edit_main_td8_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_tr7.appendChild(edit_main_td7_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr7);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr8);
  edit_main_table.appendChild(edit_main_tr6);
  edit_div.appendChild(edit_main_table);

  t.appendChild(edit_div);
  add_id_and_name(i, 'type_name');

  // Show table.
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_phone");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");
  adding_required.setAttribute("id", i + "_requiredform_id_temp");

  var adding_unique = document.createElement("input");
  adding_unique.setAttribute("type", "hidden");
  adding_unique.setAttribute("value", w_unique);
  adding_unique.setAttribute("name", i + "_uniqueform_id_temp");
  adding_unique.setAttribute("id", i + "_uniqueform_id_temp");

  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');

  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'top');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'top');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var table_name = document.createElement('table');
  table_name.setAttribute("id", i + "_table_name");
  table_name.setAttribute("cellpadding", '0');
  table_name.setAttribute("cellspacing", '0');

  var tr_name1 = document.createElement('tr');
  tr_name1.setAttribute("id", i + "_tr_name1");

  var tr_name2 = document.createElement('tr');
  tr_name2.setAttribute("id", i + "_tr_name2");

  var td_name_input1 = document.createElement('td');
  td_name_input1.setAttribute("id", i + "_td_name_input_first");

  var td_name_input2 = document.createElement('td');
  td_name_input2.setAttribute("id", i + "_td_name_input_last");

  var td_name_label1 = document.createElement('td');
  td_name_label1.setAttribute("id", i + "_td_name_label_first");
  td_name_label1.setAttribute("align", "left");

  var td_name_label2 = document.createElement('td');
  td_name_label2.setAttribute("id", i + "_td_name_label_last");
  td_name_label2.setAttribute("align", "left");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");

  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes") {
    required.innerHTML = " *";
  }
  var first = document.createElement('input');
  first.setAttribute("type", 'text');

  if (w_title[0] == w_first_val[0]) {
    first.setAttribute("class", "input_deactive");
  }
  else {
    first.setAttribute("class", "input_active");
  }
  first.style.cssText = "width:50px";
  first.setAttribute("id", i + "_element_firstform_id_temp");
  first.setAttribute("name", i + "_element_firstform_id_temp");
  first.setAttribute("value", w_first_val[0]);
  first.setAttribute("title", w_title[0]);
  first.setAttribute("onFocus", 'delete_value("' + i + '_element_firstform_id_temp")');
  first.setAttribute("onBlur", 'return_value("' + i + '_element_firstform_id_temp")');
  first.setAttribute("onChange", "change_value('" + i + "_element_firstform_id_temp')");
  first.setAttribute("onKeyPress", "return check_isnum(event)");

  var gic = document.createElement('span');
  gic.setAttribute("class", "wdform_line");
  gic.style.cssText = "margin: 0px 4px 0px 4px; padding: 0px;";
  gic.innerHTML = "-";

  var first_label = document.createElement('label');
  first_label.setAttribute("class", "mini_label");
  first_label.innerHTML = "<!--repstart-->Area Code<!--repend-->";

  var last = document.createElement('input');
  last.setAttribute("type", 'text');

  if (w_title[1] == w_first_val[1]) {
    last.setAttribute("class", "input_deactive");
  }
  else {
    last.setAttribute("class", "input_active");
  }
  last.style.cssText = "width:" + w_size + "px";
  last.setAttribute("id", i + "_element_lastform_id_temp");
  last.setAttribute("name", i + "_element_lastform_id_temp");
  last.setAttribute("value", w_first_val[1]);
  last.setAttribute("title", w_title[1]);
  last.setAttribute("onFocus", 'delete_value("' + i + '_element_lastform_id_temp")');
  last.setAttribute("onBlur", 'return_value("' + i + '_element_lastform_id_temp")');
  last.setAttribute("onChange", "change_value('" + i + "_element_lastform_id_temp')");
  last.setAttribute("onKeyPress", "return check_isnum(event)");

  var last_label = document.createElement('label');
  last_label.setAttribute("class", "mini_label");
  last_label.innerHTML = "<!--repstart-->Phone Number<!--repend-->";

  var main_td = document.getElementById('show_table');

  td1.appendChild(label);
  td1.appendChild(required);

  td_name_input1.appendChild(first);
  td_name_input1.appendChild(gic);
  td_name_input2.appendChild(last);
  tr_name1.appendChild(td_name_input1);
  tr_name1.appendChild(td_name_input2);

  td_name_label1.appendChild(first_label);
  td_name_label2.appendChild(last_label);
  tr_name2.appendChild(td_name_label1);
  tr_name2.appendChild(td_name_label2);
  table_name.appendChild(tr_name1);
  table_name.appendChild(tr_name2);

  td2.appendChild(adding_type);
  td2.appendChild(adding_required);
  td2.appendChild(adding_unique);
  td2.appendChild(table_name);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);

  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);

  if (w_field_label_pos == "top") {
    label_top(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_name');
}

function type_name(i, w_field_label, w_field_label_pos, w_first_val, w_title, w_size, w_name_format, w_required, w_unique, w_class, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_name";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_tr8 = document.createElement('tr');
  edit_main_tr8.setAttribute("valing", "top");

  var edit_main_tr9 = document.createElement('tr');
  edit_main_tr9.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var edit_main_td8 = document.createElement('td');
  edit_main_td8.style.cssText = "padding-top:10px";
  var edit_main_td8_1 = document.createElement('td');
  edit_main_td8_1.style.cssText = "padding-top:10px";

  var edit_main_td9 = document.createElement('td');
  edit_main_td9.style.cssText = "padding-top:10px";
  var edit_main_td9_1 = document.createElement('td');
  edit_main_td9_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px;";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px;";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }
  var gic = document.createTextNode("-");
  var el_first_value_label = document.createElement('label');
  el_first_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_first_value_label.innerHTML = "Value if empty ";

  var el_first_value_first = document.createElement('input');
  el_first_value_first.setAttribute("id", "el_first_value_first");
  el_first_value_first.setAttribute("type", "text");
  el_first_value_first.setAttribute("value", w_title[0]);
  el_first_value_first.style.cssText = "width:80px; margin-left:4px; margin-right:4px";
  el_first_value_first.setAttribute("onKeyUp", "change_input_value(this.value,'" + i + "_element_firstform_id_temp')");

  var el_first_value_last = document.createElement('input');
  el_first_value_last.setAttribute("id", "el_first_value_last");
  el_first_value_last.setAttribute("type", "text");
  el_first_value_last.setAttribute("value", w_title[1]);
  el_first_value_last.style.cssText = "width:80px; margin-left:4px; margin-right:4px";
  el_first_value_last.setAttribute("onKeyUp", "change_input_value(this.value,'" + i + "_element_lastform_id_temp')");

  var el_size_label = document.createElement('label');
  el_size_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_size_label.innerHTML = "Field size(px) ";
  var el_size = document.createElement('input');
  el_size.setAttribute("id", "edit_for_input_size");
  el_size.setAttribute("type", "text");
  el_size.setAttribute("value", w_size);

  el_size.setAttribute("name", "edit_for_size");
  el_size.setAttribute("onKeyPress", "return check_isnum(event)");
  el_size.setAttribute("onKeyUp", "change_w_style('" + i + "_element_firstform_id_temp', this.value); change_w_style('" + i + "_element_lastform_id_temp', this.value); change_w_style('" + i + "_element_middleform_id_temp', this.value)");

  var el_format_label = document.createElement('label');
  el_format_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_format_label.innerHTML = "Name Format";

  var el_format_normal = document.createElement('input');
  el_format_normal.setAttribute("id", "el_format_normal");
  el_format_normal.setAttribute("type", "radio");
  el_format_normal.setAttribute("value", "normal");
  el_format_normal.setAttribute("name", "edit_for_name_format");
  el_format_normal.setAttribute("onchange", "format_normal(" + i + ")");
  el_format_normal.setAttribute("checked", "checked");
  Normal = document.createTextNode("Normal");

  var el_format_extended = document.createElement('input');
  el_format_extended.setAttribute("id", "el_format_extended");
  el_format_extended.setAttribute("type", "radio");
  el_format_extended.setAttribute("value", "extended");
  el_format_extended.setAttribute("name", "edit_for_name_format");
  el_format_extended.setAttribute("onchange", "format_extended(" + i + ")");
  Extended = document.createTextNode("Extended");

  if (w_name_format == "normal") {
    el_format_normal.setAttribute("checked", "checked");
  }
  else {
    el_format_extended.setAttribute("checked", "checked");
  }
  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";

  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes") {
    el_required.setAttribute("checked", "checked");
  }
  var el_unique_label = document.createElement('label');
  el_unique_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_unique_label.innerHTML = "Allow only unique values";

  var el_unique = document.createElement('input');
  el_unique.setAttribute("id", "el_send");
  el_unique.setAttribute("type", "checkbox");
  el_unique.setAttribute("value", "yes");
  el_unique.setAttribute("onclick", "set_unique('" + i + "_uniqueform_id_temp')");
  if (w_unique == "yes") {
    el_unique.setAttribute("checked", "checked");
  }
  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_name')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");
    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_name')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");

    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_name')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_name')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }


  var t = document.getElementById('edit_table');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');

  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);

  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);

  edit_main_td9.appendChild(el_first_value_label);
  edit_main_td9_1.appendChild(el_first_value_first);
  edit_main_td9_1.appendChild(gic);
  edit_main_td9_1.appendChild(el_first_value_last);

  edit_main_td7.appendChild(el_size_label);
  edit_main_td7_1.appendChild(el_size);

  edit_main_td3.appendChild(el_format_label);

  edit_main_td3_1.appendChild(el_format_normal);
  edit_main_td3_1.appendChild(Normal);
  edit_main_td3_1.appendChild(br6);
  edit_main_td3_1.appendChild(el_format_extended);
  edit_main_td3_1.appendChild(Extended);

  edit_main_td4.appendChild(el_style_label);
  edit_main_td4_1.appendChild(el_style_textarea);

  edit_main_td5.appendChild(el_required_label);
  edit_main_td5_1.appendChild(el_required);

  edit_main_td8.appendChild(el_unique_label);
  edit_main_td8_1.appendChild(el_unique);

  edit_main_td6.appendChild(el_attr_label);
  edit_main_td6.appendChild(el_attr_add);
  edit_main_td6.appendChild(br3);
  edit_main_td6.appendChild(el_attr_table);
  edit_main_td6.setAttribute("colspan", "2");

  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_tr7.appendChild(edit_main_td7_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr8.appendChild(edit_main_td8);
  edit_main_tr8.appendChild(edit_main_td8_1);
  edit_main_tr9.appendChild(edit_main_td9);
  edit_main_tr9.appendChild(edit_main_td9_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr9);
  edit_main_table.appendChild(edit_main_tr7);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr8);
  edit_main_table.appendChild(edit_main_tr6);
  edit_div.appendChild(edit_main_table);

  t.appendChild(edit_div);
  add_id_and_name(i, 'type_name');

  // Show table.
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_name");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");
  adding_required.setAttribute("id", i + "_requiredform_id_temp");

  var adding_unique = document.createElement("input");
  adding_unique.setAttribute("type", "hidden");
  adding_unique.setAttribute("value", w_unique);
  adding_unique.setAttribute("name", i + "_uniqueform_id_temp");
  adding_unique.setAttribute("id", i + "_uniqueform_id_temp");

  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');
  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'top');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'top');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var table_name = document.createElement('table');
  table_name.setAttribute("id", i + "_table_name");
  table_name.setAttribute("cellpadding", '0');
  table_name.setAttribute("cellspacing", '0');

  var tr_name1 = document.createElement('tr');
  tr_name1.setAttribute("id", i + "_tr_name1");

  var tr_name2 = document.createElement('tr');
  tr_name2.setAttribute("id", i + "_tr_name2");

  var td_name_input1 = document.createElement('td');
  td_name_input1.setAttribute("id", i + "_td_name_input_first");

  var td_name_input2 = document.createElement('td');
  td_name_input2.setAttribute("id", i + "_td_name_input_last");

  var td_name_label1 = document.createElement('td');
  td_name_label1.setAttribute("id", i + "_td_name_label_first");
  td_name_label1.setAttribute("align", "left");

  var td_name_label2 = document.createElement('td');
  td_name_label2.setAttribute("id", i + "_td_name_label_last");
  td_name_label2.setAttribute("align", "left");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");

  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes") {
    required.innerHTML = " *";
  }
  var first = document.createElement('input');
  first.setAttribute("type", 'text');
  if (w_title[0] == w_first_val[0]) {
    first.setAttribute("class", "input_deactive");
  }
  else {
    first.setAttribute("class", "input_active");
  }
  first.style.cssText = "margin-right: 10px; width:" + w_size + "px";
  first.setAttribute("id", i + "_element_firstform_id_temp");
  first.setAttribute("name", i + "_element_firstform_id_temp");
  first.setAttribute("value", w_first_val[0]);
  first.setAttribute("title", w_title[0]);
  first.setAttribute("onFocus", 'delete_value("' + i + '_element_firstform_id_temp")');
  first.setAttribute("onBlur", 'return_value("' + i + '_element_firstform_id_temp")');
  first.setAttribute("onChange", "change_value('" + i + "_element_firstform_id_temp')");

  var first_label = document.createElement('label');
  first_label.setAttribute("class", "mini_label");
  first_label.innerHTML = "<!--repstart-->First<!--repend-->";

  var last = document.createElement('input');
  last.setAttribute("type", 'text');

  if (w_title[1] == w_first_val[1]) {
    last.setAttribute("class", "input_deactive");
  }
  else {
    last.setAttribute("class", "input_active");
  }
  last.style.cssText = "margin-right: 10px; width:" + w_size + "px";
  last.setAttribute("id", i + "_element_lastform_id_temp");
  last.setAttribute("name", i + "_element_lastform_id_temp");
  last.setAttribute("value", w_first_val[1]);
  last.setAttribute("title", w_title[1]);
  last.setAttribute("onFocus", 'delete_value("' + i + '_element_lastform_id_temp")');
  last.setAttribute("onBlur", 'return_value("' + i + '_element_lastform_id_temp")');
  last.setAttribute("onChange", "change_value('" + i + "_element_lastform_id_temp')");

  var last_label = document.createElement('label');
  last_label.setAttribute("class", "mini_label");
  last_label.innerHTML = "<!--repstart-->Last<!--repend-->";

  var main_td = document.getElementById('show_table');

  td1.appendChild(label);
  td1.appendChild(required);

  td_name_input1.appendChild(first);
  td_name_input2.appendChild(last);
  tr_name1.appendChild(td_name_input1);
  tr_name1.appendChild(td_name_input2);

  td_name_label1.appendChild(first_label);
  td_name_label2.appendChild(last_label);
  tr_name2.appendChild(td_name_label1);
  tr_name2.appendChild(td_name_label2);
  table_name.appendChild(tr_name1);
  table_name.appendChild(tr_name2);

  td2.appendChild(adding_type);
  td2.appendChild(adding_required);
  td2.appendChild(adding_unique);
  td2.appendChild(table_name);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);

  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);

  if (w_field_label_pos == "top") {
    label_top(i);
  }
  if (w_name_format == "extended") {
    format_extended(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_name');
}

function type_address(i, w_field_label, w_field_label_pos, w_size, w_required, w_class, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_address";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_tr8 = document.createElement('tr');
  edit_main_tr8.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var edit_main_td8 = document.createElement('td');
  edit_main_td8.style.cssText = "padding-top:10px";
  var edit_main_td8_1 = document.createElement('td');
  edit_main_td8_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px;";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }
  var el_size_label = document.createElement('label');
  el_size_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_size_label.innerHTML = "Overall size(px) ";
  var el_size = document.createElement('input');
  el_size.setAttribute("id", "edit_for_input_size");
  el_size.setAttribute("type", "text");
  el_size.setAttribute("value", w_size);

  el_size.setAttribute("name", "edit_for_size");
  el_size.setAttribute("onKeyPress", "return check_isnum(event)");
  el_size.setAttribute("onKeyUp", "change_w_style('" + i + "_div_address', this.value);");

  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";

  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes") {
    el_required.setAttribute("checked", "checked");
  }
  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_address')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_address')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");

    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_address')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_address')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var t = document.getElementById('edit_table');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');

  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);

  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);

  edit_main_td7.appendChild(el_size_label);
  edit_main_td7_1.appendChild(el_size);

  /*edit_main_td3.appendChild(el_format_label);
   edit_main_td3.appendChild(br5);
   edit_main_td3.appendChild(el_format_normal);
   edit_main_td3.appendChild(Normal);
   edit_main_td3.appendChild(br6);
   edit_main_td3.appendChild(el_format_extended);
   edit_main_td3.appendChild(Extended);*/

  edit_main_td4.appendChild(el_style_label);
  edit_main_td4_1.appendChild(el_style_textarea);

  edit_main_td5.appendChild(el_required_label);
  edit_main_td5_1.appendChild(el_required);

  /*	edit_main_td8.appendChild(el_unique_label);
   edit_main_td8.appendChild(el_unique);*/

  edit_main_td6.appendChild(el_attr_label);
  edit_main_td6.appendChild(el_attr_add);
  edit_main_td6.appendChild(br3);
  edit_main_td6.appendChild(el_attr_table);
  edit_main_td6.setAttribute("colspan", "2");

  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_tr7.appendChild(edit_main_td7_1);
  //edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  //edit_main_tr8.appendChild(edit_main_td8);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr7);
  //edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr5);
  //edit_main_table.appendChild(edit_main_tr8);
  edit_main_table.appendChild(edit_main_tr6);
  edit_div.appendChild(edit_main_table);

  t.appendChild(edit_div);
  add_id_and_name(i, 'type_address');

  // Show table.
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_address");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");
  adding_required.setAttribute("id", i + "_requiredform_id_temp");

  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');

  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'top');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'top');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var div_address = document.createElement('div');
  div_address.setAttribute("id", i + "_div_address");
  div_address.style.cssText = "width:" + w_size + "px";

  var span_addres1 = document.createElement('span');
  span_addres1.style.cssText = "float:left; width:100%;  padding-bottom: 8px; display:block";

  var span_addres2 = document.createElement('span');
  span_addres2.style.cssText = "float:left; width:100%;  padding-bottom: 8px; display:block";

  var span_addres3_1 = document.createElement('span');
  span_addres3_1.style.cssText = "float:left; width:48%; padding-bottom: 8px;";

  var span_addres3_2 = document.createElement('span');
  span_addres3_2.style.cssText = "float:right; width:48%; padding-bottom: 8px;";

  var span_addres4_1 = document.createElement('span');
  span_addres4_1.style.cssText = "float:left; width:48%; padding-bottom: 8px;";

  var span_addres4_2 = document.createElement('span');
  span_addres4_2.style.cssText = "float:right; width:48%; padding-bottom: 8px;";

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");

  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes") {
    required.innerHTML = " *";
  }
  var street1 = document.createElement('input');
  street1.setAttribute("type", 'text');
  street1.style.cssText = "width:100%";
  street1.setAttribute("id", i + "_street1form_id_temp");
  street1.setAttribute("name", i + "_street1form_id_temp");
  street1.setAttribute("onChange", "change_value('" + i + "_street1form_id_temp')");

  var street1_label = document.createElement('label');
  street1_label.setAttribute("class", "mini_label");
  street1_label.style.cssText = "display:block;";
  street1_label.innerHTML = "<!--repstart-->Street Address<!--repend-->";

  var street2 = document.createElement('input');
  street2.setAttribute("type", 'text');
  street2.style.cssText = "width:100%";
  street2.setAttribute("id", i + "_street2form_id_temp");
  street2.setAttribute("name", (parseInt(i) + 1) + "_street2form_id_temp");
  street2.setAttribute("onChange", "change_value('" + i + "_street2form_id_temp')");

  var street2_label = document.createElement('label');
  street2_label.setAttribute("class", "mini_label");
  street2_label.style.cssText = "display:block;";
  street2_label.innerHTML = "<!--repstart-->Street Address Line 2<!--repend-->";

  var city = document.createElement('input');
  city.setAttribute("type", 'text');
  city.style.cssText = "width:100%";
  city.setAttribute("id", i + "_cityform_id_temp");
  city.setAttribute("name", (parseInt(i) + 2) + "_cityform_id_temp");
  city.setAttribute("onChange", "change_value('" + i + "_cityform_id_temp')");

  var city_label = document.createElement('label');
  city_label.setAttribute("class", "mini_label");
  city_label.style.cssText = "display:block;";
  city_label.innerHTML = "<!--repstart-->City<!--repend-->";

  var state = document.createElement('input');
  state.setAttribute("type", 'text');
  state.style.cssText = "width:100%";
  state.setAttribute("id", i + "_stateform_id_temp");
  state.setAttribute("name", (parseInt(i) + 3) + "_stateform_id_temp");
  state.setAttribute("onChange", "change_value('" + i + "_stateform_id_temp')");

  var state_label = document.createElement('label');
  state_label.setAttribute("class", "mini_label");
  state_label.style.cssText = "display:block;";
  state_label.innerHTML = "<!--repstart-->State / Province / Region<!--repend-->";

  var postal = document.createElement('input');
  postal.setAttribute("type", 'text');
  postal.style.cssText = "width:100%";
  postal.setAttribute("id", i + "_postalform_id_temp");
  postal.setAttribute("name", (parseInt(i) + 4) + "_postalform_id_temp");
  postal.setAttribute("onChange", "change_value('" + i + "_postalform_id_temp')");

  var postal_label = document.createElement('label');
  postal_label.setAttribute("class", "mini_label");
  postal_label.style.cssText = "display:block;";
  postal_label.innerHTML = "<!--repstart-->Postal / Zip Code<!--repend-->";

  var country = document.createElement('select');
  country.setAttribute("type", 'text');
  country.style.cssText = "width:100%";
  country.setAttribute("id", i + "_countryform_id_temp");
  country.setAttribute("name", (parseInt(i) + 5) + "_countryform_id_temp");
  country.setAttribute("class", "form-select");
  country.setAttribute("onChange", "change_value('" + i + "_countryform_id_temp')");

  var country_label = document.createElement('label');
  country_label.setAttribute("class", "mini_label");
  country_label.style.cssText = "display:block;";
  country_label.innerHTML = "<!--repstart-->Country<!--repend-->";

  var option_ = document.createElement('option');
  option_.setAttribute("value", "");
  option_.innerHTML = "";
  country.appendChild(option_);

  coutries = ["Afghanistan", "Albania", "Algeria", "Andorra", "Angola", "Antigua and Barbuda", "Argentina", "Armenia", "Australia", "Austria", "Azerbaijan", "Bahamas", "Bahrain", "Bangladesh", "Barbados", "Belarus", "Belgium", "Belize", "Benin", "Bhutan", "Bolivia", "Bosnia and Herzegovina", "Botswana", "Brazil", "Brunei", "Bulgaria", "Burkina Faso", "Burundi", "Cambodia", "Cameroon", "Canada", "Cape Verde", "Central African Republic", "Chad", "Chile", "China", "Colombi", "Comoros", "Congo (Brazzaville)", "Congo", "Costa Rica", "Cote d'Ivoire", "Croatia", "Cuba", "Cyprus", "Czech Republic", "Denmark", "Djibouti", "Dominica", "Dominican Republic", "East Timor (Timor Timur)", "Ecuador", "Egypt", "El Salvador", "Equatorial Guinea", "Eritrea", "Estonia", "Ethiopia", "Fiji", "Finland", "France", "Gabon", "Gambia, The", "Georgia", "Germany", "Ghana", "Greece", "Grenada", "Guatemala", "Guinea", "Guinea-Bissau", "Guyana", "Haiti", "Honduras", "Hungary", "Iceland", "India", "Indonesia", "Iran", "Iraq", "Ireland", "Israel", "Italy", "Jamaica", "Japan", "Jordan", "Kazakhstan", "Kenya", "Kiribati", "Korea, North", "Korea, South", "Kuwait", "Kyrgyzstan", "Laos", "Latvia", "Lebanon", "Lesotho", "Liberia", "Libya", "Liechtenstein", "Lithuania", "Luxembourg", "Macedonia", "Madagascar", "Malawi", "Malaysia", "Maldives", "Mali", "Malta", "Marshall Islands", "Mauritania", "Mauritius", "Mexico", "Micronesia", "Moldova", "Monaco", "Mongolia", "Morocco", "Mozambique", "Myanmar", "Namibia", "Nauru", "Nepa", "Netherlands", "New Zealand", "Nicaragua", "Niger", "Nigeria", "Norway", "Oman", "Pakistan", "Palau", "Panama", "Papua New Guinea", "Paraguay", "Peru", "Philippines", "Poland", "Portugal", "Qatar", "Romania", "Russia", "Rwanda", "Saint Kitts and Nevis", "Saint Lucia", "Saint Vincent", "Samoa", "San Marino", "Sao Tome and Principe", "Saudi Arabia", "Senegal", "Serbia and Montenegro", "Seychelles", "Sierra Leone", "Singapore", "Slovakia", "Slovenia", "Solomon Islands", "Somalia", "South Africa", "Spain", "Sri Lanka", "Sudan", "Suriname", "Swaziland", "Sweden", "Switzerland", "Syria", "Taiwan", "Tajikistan", "Tanzania", "Thailand", "Togo", "Tonga", "Trinidad and Tobago", "Tunisia", "Turkey", "Turkmenistan", "Tuvalu", "Uganda", "Ukraine", "United Arab Emirates", "United Kingdom", "United States", "Uruguay", "Uzbekistan", "Vanuatu", "Vatican City", "Venezuela", "Vietnam", "Yemen", "Zambia", "Zimbabwe"];
  for (r = 0; r < coutries.length; r++) {
    var option_ = document.createElement('option');
    option_.setAttribute("value", coutries[r]);
    option_.innerHTML = coutries[r];
    country.appendChild(option_);
  }

  var main_td = document.getElementById('show_table');
  td1.appendChild(label);
  td1.appendChild(required);

  span_addres1.appendChild(street1);
  span_addres1.appendChild(street1_label);

  span_addres2.appendChild(street2);
  span_addres2.appendChild(street2_label);

  span_addres3_1.appendChild(city);
  span_addres3_1.appendChild(city_label);
  span_addres3_2.appendChild(state);
  span_addres3_2.appendChild(state_label);

  span_addres4_1.appendChild(postal);
  span_addres4_1.appendChild(postal_label);
  span_addres4_2.appendChild(country);
  span_addres4_2.appendChild(country_label);

  div_address.appendChild(span_addres1);
  div_address.appendChild(span_addres2);
  div_address.appendChild(span_addres3_1);
  div_address.appendChild(span_addres3_2);
  div_address.appendChild(span_addres4_1);
  div_address.appendChild(span_addres4_2);

  td2.appendChild(adding_type);
  td2.appendChild(adding_required);
  td2.appendChild(div_address);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);

  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);

  if (w_field_label_pos == "top") {
    label_top(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_address');
}

function type_submitter_mail(i, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_send, w_required, w_unique, w_class, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_submitter_mail";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");
  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_tr8 = document.createElement('tr');
  edit_main_tr8.setAttribute("valing", "top");

  var edit_main_tr9 = document.createElement('tr');
  edit_main_tr9.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";
  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";
  var edit_main_td8 = document.createElement('td');
  edit_main_td8.style.cssText = "padding-top:10px";
  var edit_main_td8_1 = document.createElement('td');
  edit_main_td8_1.style.cssText = "padding-top:10px";

  var edit_main_td9 = document.createElement('td');
  edit_main_td9.style.cssText = "padding-top:10px";
  var edit_main_td9_1 = document.createElement('td');
  edit_main_td9_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }

  var el_size_label = document.createElement('label');
  el_size_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_size_label.innerHTML = "Field size(px) ";
  var el_size = document.createElement('input');
  el_size.setAttribute("id", "edit_for_input_size");
  el_size.setAttribute("type", "text");
  el_size.setAttribute("value", w_size);

  el_size.setAttribute("name", "edit_for_size");
  el_size.setAttribute("onKeyPress", "return check_isnum(event)");
  el_size.setAttribute("onKeyUp", "change_w_style('" + i + "_elementform_id_temp', this.value)");

  var el_first_value_label = document.createElement('label');
  el_first_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_first_value_label.innerHTML = "Value if empty";

  var el_first_value_input = document.createElement('input');
  el_first_value_input.setAttribute("id", "el_first_value_input");
  el_first_value_input.setAttribute("type", "text");
  el_first_value_input.setAttribute("value", w_title);
  el_first_value_input.style.cssText = "width:150px;";
  el_first_value_input.setAttribute("onKeyUp", "change_input_value(this.value,'" + i + "_elementform_id_temp')");

  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_send_label = document.createElement('label');
  el_send_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_send_label.innerHTML = "Send mail to submitter ";

  var el_send = document.createElement('input');
  el_send.setAttribute("id", "el_send");
  el_send.setAttribute("type", "checkbox");
  el_send.setAttribute("value", "yes");
  el_send.setAttribute("onclick", "set_send('" + i + "_sendform_id_temp')");
  if (w_send == "yes") {
    el_send.setAttribute("checked", "checked");
  }
  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";

  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes") {
    el_required.setAttribute("checked", "checked");
  }
  var el_unique_label = document.createElement('label');
  el_unique_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_unique_label.innerHTML = "Allow only unique values";

  var el_unique = document.createElement('input');
  el_unique.setAttribute("id", "el_send");
  el_unique.setAttribute("type", "checkbox");
  el_unique.setAttribute("value", "yes");
  el_unique.setAttribute("onclick", "set_unique('" + i + "_uniqueform_id_temp')");
  if (w_unique == "yes") {
    el_unique.setAttribute("checked", "checked");
  }

  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_text')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_text')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");
    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_text')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_text')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }

  var t = document.getElementById('edit_table');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');

  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);

  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);

  edit_main_td3.appendChild(el_size_label);
  edit_main_td3_1.appendChild(el_size);

  edit_main_td4.appendChild(el_first_value_label);
  edit_main_td4_1.appendChild(el_first_value_input);

  edit_main_td5.appendChild(el_style_label);
  edit_main_td5_1.appendChild(el_style_textarea);

  edit_main_td6.appendChild(el_send_label);
  edit_main_td6_1.appendChild(el_send);

  edit_main_td7.appendChild(el_required_label);
  edit_main_td7_1.appendChild(el_required);

  edit_main_td9.appendChild(el_unique_label);
  edit_main_td9_1.appendChild(el_unique);

  edit_main_td8.appendChild(el_attr_label);
  edit_main_td8.appendChild(el_attr_add);
  edit_main_td8.appendChild(br4);
  edit_main_td8.appendChild(el_attr_table);
  edit_main_td8.setAttribute("colspan", "2");

  /*	edit_main_td5.appendChild(el_guidelines_label);
   edit_main_td5.appendChild(br4);
   edit_main_td5.appendChild(el_guidelines_textarea);
   */
  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);

  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_tr7.appendChild(edit_main_td7_1);
  edit_main_tr9.appendChild(edit_main_td9);
  edit_main_tr9.appendChild(edit_main_td9_1);
  edit_main_tr8.appendChild(edit_main_td8);
  edit_main_tr8.appendChild(edit_main_td8_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr5);

  edit_main_table.appendChild(edit_main_tr6);
  edit_main_table.appendChild(edit_main_tr7);
  edit_main_table.appendChild(edit_main_tr9);
  edit_main_table.appendChild(edit_main_tr8);
  edit_div.appendChild(edit_main_table);

  t.appendChild(edit_div);
  add_id_and_name(i, 'type_text');

  // Show table.
  element = 'input';
  type = 'text';
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_submitter_mail");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");

  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");
  adding_required.setAttribute("id", i + "_requiredform_id_temp");

  var adding_send = document.createElement("input");
  adding_send.setAttribute("type", "hidden");
  adding_send.setAttribute("value", w_send);
  adding_send.setAttribute("name", i + "_sendform_id_temp");
  adding_send.setAttribute("id", i + "_sendform_id_temp");

  var adding_unique = document.createElement("input");
  adding_unique.setAttribute("type", "hidden");
  adding_unique.setAttribute("value", w_unique);
  adding_unique.setAttribute("name", i + "_uniqueform_id_temp");
  adding_unique.setAttribute("id", i + "_uniqueform_id_temp");

  var adding = document.createElement(element);
  adding.setAttribute("type", type);
  if (w_title == w_first_val) {
    adding.style.cssText = "width:" + w_size + "px;";
    adding.setAttribute("class", "input_deactive");
  }
  else {
    adding.style.cssText = "width:" + w_size + "px;";
    adding.setAttribute("class", "input_active");
  }
  adding.setAttribute("id", i + "_elementform_id_temp");
  adding.setAttribute("name", i + "_elementform_id_temp");
  adding.setAttribute("value", w_first_val);
  adding.setAttribute("title", w_title);

  adding.setAttribute("onFocus", "delete_value('" + i + "_elementform_id_temp')");
  adding.setAttribute("onBlur", "return_value('" + i + "_elementform_id_temp')");
  adding.setAttribute("onChange", "change_value('" + i + "_elementform_id_temp')");
  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');

  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'middle');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'middle');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");

  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes")
    required.innerHTML = " *";
  var main_td = document.getElementById('show_table');

  td1.appendChild(label);
  td1.appendChild(required);
  td2.appendChild(adding_type);
  td2.appendChild(adding_required);
  td2.appendChild(adding_send);
  td2.appendChild(adding_unique);
  td2.appendChild(adding);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);

  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  if (w_field_label_pos == "top") {
    label_top(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_text');
}

function type_checkbox(i, w_field_label, w_field_label_pos, w_flow, w_choices, w_choices_checked, w_required, w_randomize, w_allow_other, w_allow_other_num, w_class, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_checkbox";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");
  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_tr8 = document.createElement('tr');
  edit_main_tr8.setAttribute("valing", "top");

  var edit_main_tr9 = document.createElement('tr');
  edit_main_tr9.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";

  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px; vertical-align:top";
  edit_main_td4.setAttribute("id", "choices");
  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var edit_main_td8 = document.createElement('td');
  edit_main_td8.style.cssText = "padding-top:10px";
  var edit_main_td8_1 = document.createElement('td');
  edit_main_td8_1.style.cssText = "padding-top:10px";

  var edit_main_td9 = document.createElement('td');
  edit_main_td9.style.cssText = "padding-top:10px";
  var edit_main_td9_1 = document.createElement('td');
  edit_main_td9_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px;";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }
  var el_label_flow = document.createElement('label');
  el_label_flow.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_flow.innerHTML = "Relative Position";

  var el_flow_vertical = document.createElement('input');
  el_flow_vertical.setAttribute("id", "edit_for_flow_vertical");
  el_flow_vertical.setAttribute("type", "radio");
  el_flow_vertical.setAttribute("value", "ver");
  el_flow_vertical.setAttribute("name", "edit_for_flow");
  el_flow_vertical.setAttribute("onchange", "flow_ver(" + i + ")");
  Vertical = document.createTextNode("Vertical");

  var el_flow_horizontal = document.createElement('input');
  el_flow_horizontal.setAttribute("id", "edit_for_flow_horizontal");
  el_flow_horizontal.setAttribute("type", "radio");
  el_flow_horizontal.setAttribute("value", "hor");
  el_flow_horizontal.setAttribute("name", "edit_for_flow");
  el_flow_horizontal.setAttribute("onchange", "flow_hor(" + i + ")");
  Horizontal = document.createTextNode("Horizontal");

  if (w_flow == "hor") {
    el_flow_horizontal.setAttribute("checked", "checked");
  }
  else {
    el_flow_vertical.setAttribute("checked", "checked");
  }
  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";

  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes") {
    el_required.setAttribute("checked", "checked");
  }

  var el_randomize_label = document.createElement('label');
  el_randomize_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_randomize_label.innerHTML = "Randomize in frontend";

  var el_randomize = document.createElement('input');
  el_randomize.setAttribute("id", "el_randomize");
  el_randomize.setAttribute("type", "checkbox");
  el_randomize.setAttribute("value", "yes");
  el_randomize.setAttribute("onclick", "set_randomize('" + i + "_randomizeform_id_temp')");
  if (w_randomize == "yes") {
    el_randomize.setAttribute("checked", "checked");
  }
  var el_allow_other_label = document.createElement('label');
  el_allow_other_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_allow_other_label.innerHTML = "Allow other";

  var el_allow_other = document.createElement('input');
  el_allow_other.setAttribute("id", "el_allow_other");
  el_allow_other.setAttribute("type", "checkbox");
  el_allow_other.setAttribute("value", "yes");
  el_allow_other.setAttribute("onclick", "set_allow_other('" + i + "','checkbox')");
  if (w_allow_other == "yes") {
    el_allow_other.setAttribute("checked", "checked");
  }
  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_checkbox')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_checkbox')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");
    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_checkbox')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_checkbox')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }

  var el_choices_label = document.createElement('label');
  el_choices_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_choices_label.innerHTML = "Options ";
  var el_choices_add = document.createElement('img');
  el_choices_add.setAttribute("id", "el_choices_add");
  el_choices_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_choices_add.style.cssText = 'cursor:pointer;';
  el_choices_add.setAttribute("title", 'add');
  el_choices_add.setAttribute("onClick", "add_choise('checkbox'," + i + ")");

  var t = document.getElementById('edit_table');

  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');

  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);

  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);

  edit_main_td3.appendChild(el_label_flow);
  edit_main_td3_1.appendChild(el_flow_vertical);
  edit_main_td3_1.appendChild(Vertical);
  edit_main_td3_1.appendChild(br4);
  edit_main_td3_1.appendChild(el_flow_horizontal);
  edit_main_td3_1.appendChild(Horizontal);

  edit_main_td5.appendChild(el_required_label);
  edit_main_td5_1.appendChild(el_required);

  edit_main_td8.appendChild(el_randomize_label);
  edit_main_td8_1.appendChild(el_randomize);

  edit_main_td9.appendChild(el_allow_other_label);
  edit_main_td9_1.appendChild(el_allow_other);

  edit_main_td6.appendChild(el_style_label);
  edit_main_td6_1.appendChild(el_style_textarea);

  edit_main_td7.appendChild(el_attr_label);
  edit_main_td7.appendChild(el_attr_add);
  edit_main_td7.appendChild(br6);
  edit_main_td7.appendChild(el_attr_table);
  edit_main_td7.setAttribute("colspan", "2");

  edit_main_td4.appendChild(el_choices_label);
  edit_main_td4_1.appendChild(el_choices_add);

  n = w_choices.length;
  for (j = 0; j < n; j++) {
    var br = document.createElement('br');
    br.setAttribute("id", "br" + j);

    var el_choices = document.createElement('input');
    el_choices.setAttribute("id", "el_choices" + j);
    el_choices.setAttribute("type", "text");
    el_choices.setAttribute("value", w_choices[j]);
    el_choices.style.cssText = "width:100px; margin:0; padding:0; border-width: 1px";
    el_choices.setAttribute("onKeyUp", "change_label('" + i + "_label_element" + j + "', this.value); change_in_value('" + i + "_elementform_id_temp" + j + "', this.value)");

    var el_choices_remove = document.createElement('img');
    el_choices_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_choices_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    if (w_allow_other == "yes" && j == w_allow_other_num) {
      el_choices_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px; display:none';
    }
    else {
      el_choices_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    }
    el_choices_remove.setAttribute("align", 'top');
    el_choices_remove.setAttribute("onClick", "remove_choise(" + j + "," + i + ")");

    edit_main_td4.appendChild(br);
    edit_main_td4.appendChild(el_choices);
    edit_main_td4.appendChild(el_choices_remove);
  }
  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr8.appendChild(edit_main_td8);
  edit_main_tr8.appendChild(edit_main_td8_1);
  edit_main_tr9.appendChild(edit_main_td9);
  edit_main_tr9.appendChild(edit_main_td9_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr6);
  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr8);
  edit_main_table.appendChild(edit_main_tr9);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr7);
  edit_div.appendChild(edit_main_table);
  t.appendChild(edit_div);
  // Show table.
  element = 'input';
  type = 'checkbox';
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_checkbox");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");
  adding_required.setAttribute("id", i + "_requiredform_id_temp");

  var adding_randomize = document.createElement("input");
  adding_randomize.setAttribute("type", "hidden");
  adding_randomize.setAttribute("value", w_randomize);
  adding_randomize.setAttribute("name", i + "_randomizeform_id_temp");
  adding_randomize.setAttribute("id", i + "_randomizeform_id_temp");

  var adding_allow_other = document.createElement("input");
  adding_allow_other.setAttribute("type", "hidden");
  adding_allow_other.setAttribute("value", w_allow_other);
  adding_allow_other.setAttribute("name", i + "_allow_otherform_id_temp");
  adding_allow_other.setAttribute("id", i + "_allow_otherform_id_temp");

  var adding_allow_other_id = document.createElement("input");
  adding_allow_other_id.setAttribute("type", "hidden");
  adding_allow_other_id.setAttribute("value", w_allow_other_num);
  adding_allow_other_id.setAttribute("name", i + "_allow_other_numform_id_temp");
  adding_allow_other_id.setAttribute("id", i + "_allow_other_numform_id_temp");

  var div = document.createElement('div');
  div.setAttribute("id", "main_div");
  // tbody sarqac.
  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');

  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'top');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'top');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  //	table_little -@ sarqaca tbody table_little darela table_little_t
  var table_little_t = document.createElement('table');
  table_little_t.setAttribute("style", "border:none;");

  var table_little = document.createElement('tbody');
  table_little.setAttribute("style", "border:none;");
  table_little.setAttribute("id", i + "_table_little");
  table_little_t.appendChild(table_little);

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");


  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes") {
    required.innerHTML = " *";
  }
  n = w_choices.length;
  aaa = false;
  for (j = 0; j < n; j++) {
    var tr_little = document.createElement('tr');
    tr_little.setAttribute("id", i + "_element_tr" + j);

    var td_little = document.createElement('td');
    td_little.setAttribute("valign", 'top');
    td_little.setAttribute("id", i + "_td_little" + j);
    td_little.setAttribute("idi", j);

    var adding = document.createElement(element);
    adding.setAttribute("type", type);
    adding.setAttribute("id", i + "_elementform_id_temp" + j);
    adding.setAttribute("name", i + "_elementform_id_temp" + j);
    adding.setAttribute("value", w_choices[j]);
    if (w_allow_other == "yes" && j == w_allow_other_num) {
      adding.setAttribute("other", "1");
      adding.setAttribute("onclick", "if(set_checked('" + i + "','" + j + "','form_id_temp')) show_other_input('" + i + "','form_id_temp');");
    }
    else {
      adding.setAttribute("onclick", "set_checked('" + i + "','" + j + "','form_id_temp')");
    }
    if (w_choices_checked[j] == '1') {
      adding.setAttribute("checked", "checked");
    }
    var label_adding = document.createElement('label');
    label_adding.setAttribute("id", i + "_label_element" + j);
    label_adding.setAttribute("class", "ch_rad_label");
    label_adding.setAttribute("for", i + "_elementform_id_temp" + j);
    label_adding.innerHTML = w_choices[j];

    td_little.appendChild(adding);
    td_little.appendChild(label_adding);
    tr_little.appendChild(td_little);
    table_little.appendChild(tr_little);

    if (w_choices_checked[j] == '1') {
      if (w_allow_other == "yes" && j == w_allow_other_num) {
        aaa = true;
      }
    }
  }
  var main_td = document.getElementById('show_table');
  td1.appendChild(label);
  td1.appendChild(required);
  td2.appendChild(adding_type);
  td2.appendChild(adding_required);
  td2.appendChild(adding_randomize);
  td2.appendChild(adding_allow_other);
  td2.appendChild(adding_allow_other_id);
  td2.appendChild(table_little_t);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);

  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  add_id_and_name(i, 'type_checkbox');

  if (w_field_label_pos == "top") {
    label_top(i);
  }
  if (w_flow == "hor") {
    flow_hor(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_checkbox');
  if (aaa) {
    show_other_input(i);
  }
}

function type_radio(i, w_field_label, w_field_label_pos, w_flow, w_choices, w_choices_checked, w_required, w_randomize, w_allow_other, w_allow_other_num, w_class, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_radio";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_tr8 = document.createElement('tr');
  edit_main_tr8.setAttribute("valing", "top");

  var edit_main_tr9 = document.createElement('tr');
  edit_main_tr9.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";

  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px; vertical-align:top";

  edit_main_td4.setAttribute("id", "choices");

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var edit_main_td8 = document.createElement('td');
  edit_main_td8.style.cssText = "padding-top:10px";
  var edit_main_td8_1 = document.createElement('td');
  edit_main_td8_1.style.cssText = "padding-top:10px";

  var edit_main_td9 = document.createElement('td');
  edit_main_td9.style.cssText = "padding-top:10px";
  var edit_main_td9_1 = document.createElement('td');
  edit_main_td9_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px;";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  el_label_position1.setAttribute("checked", "checked");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");

  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }
  var el_label_flow = document.createElement('label');
  el_label_flow.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_flow.innerHTML = "Relative Position";

  var el_flow_vertical = document.createElement('input');
  el_flow_vertical.setAttribute("id", "edit_for_flow_vertical");
  el_flow_vertical.setAttribute("type", "radio");
  el_flow_vertical.setAttribute("value", "ver");
  el_flow_vertical.setAttribute("name", "edit_for_flow");
  el_flow_vertical.setAttribute("onchange", "flow_ver(" + i + ")");
  Vertical = document.createTextNode("Vertical");

  var el_flow_horizontal = document.createElement('input');
  el_flow_horizontal.setAttribute("id", "edit_for_flow_horizontal");
  el_flow_horizontal.setAttribute("type", "radio");
  el_flow_horizontal.setAttribute("value", "hor");
  el_flow_horizontal.setAttribute("name", "edit_for_flow");
  el_flow_horizontal.setAttribute("onchange", "flow_hor(" + i + ")");
  Horizontal = document.createTextNode("Horizontal");

  if (w_flow == "hor") {
    el_flow_horizontal.setAttribute("checked", "checked");
  }
  else {
    el_flow_vertical.setAttribute("checked", "checked");
  }
  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";

  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes") {
    el_required.setAttribute("checked", "checked");
  }
  var el_randomize_label = document.createElement('label');
  el_randomize_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_randomize_label.innerHTML = "Randomize in frontend";

  var el_randomize = document.createElement('input');
  el_randomize.setAttribute("id", "el_randomize");
  el_randomize.setAttribute("type", "checkbox");
  el_randomize.setAttribute("value", "yes");
  el_randomize.setAttribute("onclick", "set_randomize('" + i + "_randomizeform_id_temp')");
  if (w_randomize == "yes") {
    el_randomize.setAttribute("checked", "checked");
  }
  var el_allow_other_label = document.createElement('label');
  el_allow_other_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_allow_other_label.innerHTML = "Allow other";

  var el_allow_other = document.createElement('input');
  el_allow_other.setAttribute("id", "el_allow_other");
  el_allow_other.setAttribute("type", "checkbox");
  el_allow_other.setAttribute("value", "yes");
  el_allow_other.setAttribute("onclick", "set_allow_other('" + i + "','radio')");
  if (w_allow_other == "yes") {
    el_allow_other.setAttribute("checked", "checked");
  }
  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_checkbox')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_checkbox')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");
    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_checkbox')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_checkbox')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var el_choices_label = document.createElement('label');
  el_choices_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_choices_label.innerHTML = "Options ";
  var el_choices_add = document.createElement('img');
  el_choices_add.setAttribute("id", "el_choices_add");
  el_choices_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_choices_add.style.cssText = 'cursor:pointer;';
  el_choices_add.setAttribute("title", 'add');
  el_choices_add.setAttribute("onClick", "add_choise('radio'," + i + ")");

  var t = document.getElementById('edit_table');

  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');

  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);

  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);

  edit_main_td3.appendChild(el_label_flow);
  edit_main_td3_1.appendChild(el_flow_vertical);
  edit_main_td3_1.appendChild(Vertical);
  edit_main_td3_1.appendChild(br4);
  edit_main_td3_1.appendChild(el_flow_horizontal);
  edit_main_td3_1.appendChild(Horizontal);

  edit_main_td6.appendChild(el_style_label);
  edit_main_td6_1.appendChild(el_style_textarea);

  edit_main_td5.appendChild(el_required_label);
  edit_main_td5_1.appendChild(el_required);

  edit_main_td8.appendChild(el_randomize_label);
  edit_main_td8_1.appendChild(el_randomize);

  edit_main_td9.appendChild(el_allow_other_label);
  edit_main_td9_1.appendChild(el_allow_other);

  edit_main_td7.appendChild(el_attr_label);
  edit_main_td7.appendChild(el_attr_add);
  edit_main_td7.appendChild(br6);
  edit_main_td7.appendChild(el_attr_table);
  edit_main_td7.setAttribute("colspan", "2");

  edit_main_td4.appendChild(el_choices_label);
  edit_main_td4_1.appendChild(el_choices_add);

  n = w_choices.length;
  for (j = 0; j < n; j++) {
    var br = document.createElement('br');
    br.setAttribute("id", "br" + j);

    var el_choices = document.createElement('input');
    el_choices.setAttribute("id", "el_choices" + j);
    el_choices.setAttribute("type", "text");
    el_choices.setAttribute("value", w_choices[j]);
    el_choices.style.cssText = "width:100px; margin:0; padding:0; border-width: 1px";
    el_choices.setAttribute("onKeyUp", "change_label('" + i + "_label_element" + j + "', this.value); change_in_value('" + i + "_elementform_id_temp" + j + "', this.value)");

    var el_choices_remove = document.createElement('img');
    el_choices_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_choices_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    if (w_allow_other == "yes" && j == w_allow_other_num)
      el_choices_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px; display:none';
    else
      el_choices_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_choices_remove.setAttribute("align", 'top');
    el_choices_remove.setAttribute("onClick", "remove_choise(" + j + "," + i + ")");

    edit_main_td4.appendChild(br);
    edit_main_td4.appendChild(el_choices);
    edit_main_td4.appendChild(el_choices_remove);
  }
  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);

  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr8.appendChild(edit_main_td8);
  edit_main_tr8.appendChild(edit_main_td8_1);
  edit_main_tr9.appendChild(edit_main_td9);
  edit_main_tr9.appendChild(edit_main_td9_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr6);
  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr8);
  edit_main_table.appendChild(edit_main_tr9);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr7);
  edit_div.appendChild(edit_main_table);

  t.appendChild(edit_div);

  // Show table.
  element = 'input';
  type = 'radio';
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_radio");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");
  adding_required.setAttribute("id", i + "_requiredform_id_temp");

  var adding_randomize = document.createElement("input");
  adding_randomize.setAttribute("type", "hidden");
  adding_randomize.setAttribute("value", w_randomize);
  adding_randomize.setAttribute("name", i + "_randomizeform_id_temp");
  adding_randomize.setAttribute("id", i + "_randomizeform_id_temp");

  var adding_allow_other = document.createElement("input");
  adding_allow_other.setAttribute("type", "hidden");
  adding_allow_other.setAttribute("value", w_allow_other);
  adding_allow_other.setAttribute("name", i + "_allow_otherform_id_temp");
  adding_allow_other.setAttribute("id", i + "_allow_otherform_id_temp");

  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');
  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'top');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'top');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  //tbody sarqac
  var table_little_t = document.createElement('table');
  var table_little = document.createElement('tbody');
  table_little.setAttribute("style", "border:none;");
  table_little.setAttribute("id", i + "_table_little");
  table_little_t.appendChild(table_little);
  var tr_little1 = document.createElement('tr');
  tr_little1.setAttribute("id", i + "_element_tr1");

  var tr_little2 = document.createElement('tr');
  tr_little2.setAttribute("id", i + "_element_tr2");

  var td_little1 = document.createElement('td');
  td_little1.setAttribute("valign", 'top');
  td_little1.setAttribute("id", i + "_td_little1");

  var td_little2 = document.createElement('td');
  td_little2.setAttribute("valign", 'top');
  td_little2.setAttribute("id", i + "_td_little2");

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");

  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes") {
    required.innerHTML = " *";
  }
  n = w_choices.length;
  aaa = false;
  for (j = 0; j < n; j++) {
    var tr_little = document.createElement('tr');
    tr_little.setAttribute("id", i + "_element_tr" + j);

    var td_little = document.createElement('td');
    td_little.setAttribute("valign", 'top');
    td_little.setAttribute("id", i + "_td_little" + j);
    td_little.setAttribute("idi", j);

    var adding = document.createElement(element);
    adding.setAttribute("type", type);
    adding.setAttribute("id", i + "_elementform_id_temp" + j);
    adding.setAttribute("name", i + "_elementform_id_temp");
    adding.setAttribute("value", w_choices[j]);
    if (w_allow_other == "yes" && j == w_allow_other_num) {
      adding.setAttribute("other", "1");
      adding.setAttribute("onclick", "set_default('" + i + "','" + j + "','form_id_temp'); show_other_input('" + i + "','form_id_temp');");
    }
    else {
      adding.setAttribute("onclick", "set_default('" + i + "','" + j + "','form_id_temp')");
    }
    if (w_choices_checked[j] == '1') {
      adding.setAttribute("checked", "checked");
    }
    var label_adding = document.createElement('label');
    label_adding.setAttribute("id", i + "_label_element" + j);
    label_adding.setAttribute("class", "ch_rad_label");
    label_adding.setAttribute("for", i + "_elementform_id_temp" + j);
    label_adding.innerHTML = w_choices[j];

    td_little.appendChild(adding);
    td_little.appendChild(label_adding);
    tr_little.appendChild(td_little);
    table_little.appendChild(tr_little);

    if (w_choices_checked[j] == '1') {
      if (w_allow_other == "yes" && j == w_allow_other_num) {
        aaa = true;
      }
    }
  }
  var main_td = document.getElementById('show_table');
  td1.appendChild(label);
  td1.appendChild(required);
  td2.appendChild(adding_type);

  td2.appendChild(adding_required);
  td2.appendChild(adding_randomize);
  td2.appendChild(adding_allow_other);
  td2.appendChild(table_little_t);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);

  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  add_id_and_name(i, 'type_radio');

  if (w_field_label_pos == "top") {
    label_top(i);
  }
  if (w_flow == "hor") {
    flow_hor(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_checkbox');
  if (aaa) {
    show_other_input(i);
  }
}

function type_time(i, w_field_label, w_field_label_pos, w_time_type, w_am_pm, w_sec, w_hh, w_mm, w_ss, w_required, w_class, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_time";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");
  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";
  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px;width:110px;";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px;";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  el_label_position1.setAttribute("checked", "checked");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }
  var el_label_time_type_label = document.createElement('label');
  el_label_time_type_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_time_type_label.innerHTML = "Time Format";

  var el_label_time_type1 = document.createElement('input');
  el_label_time_type1.setAttribute("id", "el_label_time_type1");
  el_label_time_type1.setAttribute("type", "radio");
  el_label_time_type1.setAttribute("value", "format_24");
  el_label_time_type1.setAttribute("name", "edit_for_time_type");
  el_label_time_type1.setAttribute("onchange", "format_24(" + i + ")");
  el_label_time_type1.setAttribute("checked", "checked");
  hour_24 = document.createTextNode("24 hour");

  var el_label_time_type2 = document.createElement('input');
  el_label_time_type2.setAttribute("id", "el_label_time_type2");
  el_label_time_type2.setAttribute("type", "radio");
  el_label_time_type2.setAttribute("value", "format_12");
  el_label_time_type2.setAttribute("name", "edit_for_time_type");
  el_label_time_type2.setAttribute("onchange", "format_12(" + i + ", 'am','', '','')");
  am_pm = document.createTextNode("12 hour");
  if (w_time_type == "24") {
    el_label_time_type1.setAttribute("checked", "checked");
  }
  else {
    el_label_time_type2.setAttribute("checked", "checked");
  }
  var el_label_second_label = document.createElement('label');
  el_label_second_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_second_label.innerHTML = "Display Seconds";

  var el_second_yes = document.createElement('input');
  el_second_yes.setAttribute("id", "el_second_yes");
  el_second_yes.setAttribute("type", "radio");
  el_second_yes.setAttribute("value", "yes");
  el_second_yes.setAttribute("name", "edit_for_time_second");
  el_second_yes.setAttribute("onchange", "second_yes(" + i + ",'" + w_ss + "')");
  el_second_yes.setAttribute("checked", "checked");
  display_seconds = document.createTextNode("Yes");

  var el_second_no = document.createElement('input');
  el_second_no.setAttribute("id", "el_second_no");
  el_second_no.setAttribute("type", "radio");
  el_second_no.setAttribute("value", "no");
  el_second_no.setAttribute("name", "edit_for_time_second");
  el_second_no.setAttribute("onchange", "second_no(" + i + ")");
  dont_display_seconds = document.createTextNode("No");

  if (w_sec == "1") {
    el_second_yes.setAttribute("checked", "checked");
  }
  else {
    el_second_no.setAttribute("checked", "checked");
  }
  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";

  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes") {
    el_required.setAttribute("checked", "checked");
  }
  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";

  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_time')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_time')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");

    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_time')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_time')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }

  var t = document.getElementById('edit_table');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');

  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);

  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);

  edit_main_td3.appendChild(el_label_time_type_label);
  edit_main_td3_1.appendChild(el_label_time_type1);
  edit_main_td3_1.appendChild(hour_24);
  edit_main_td3_1.appendChild(br6);
  edit_main_td3_1.appendChild(el_label_time_type2);
  edit_main_td3_1.appendChild(am_pm);

  edit_main_td4.appendChild(el_label_second_label);
  edit_main_td4_1.appendChild(el_second_yes);
  edit_main_td4_1.appendChild(display_seconds);
  edit_main_td4_1.appendChild(br4);
  edit_main_td4_1.appendChild(el_second_no);
  edit_main_td4_1.appendChild(dont_display_seconds);

  edit_main_td5.appendChild(el_style_label);
  edit_main_td5_1.appendChild(el_style_textarea);

  edit_main_td6.appendChild(el_required_label);
  edit_main_td6_1.appendChild(el_required);

  edit_main_td7.appendChild(el_attr_label);
  edit_main_td7.appendChild(el_attr_add);
  edit_main_td7.appendChild(br6);
  edit_main_td7.appendChild(el_attr_table);
  edit_main_td7.setAttribute("colspan", "2");
  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr6);
  edit_main_table.appendChild(edit_main_tr7);
  edit_div.appendChild(edit_main_table);
  t.appendChild(edit_div);
  add_id_and_name(i, 'type_time');

  // Show table.
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_time");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");

  adding_required.setAttribute("id", i + "_requiredform_id_temp");
  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');

  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'top');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'middle');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var table_time = document.createElement('table');
  table_time.setAttribute("id", i + "_table_time");
  table_time.setAttribute("cellpadding", '0');
  table_time.setAttribute("cellspacing", '0');
  var tr_time1 = document.createElement('tr');
  tr_time1.setAttribute("id", i + "_tr_time1");

  var tr_time2 = document.createElement('tr');
  tr_time2.setAttribute("id", i + "_tr_time2");

  var td_time_input1 = document.createElement('td');
  td_time_input1.setAttribute("id", i + "_td_time_input1");
  td_time_input1.style.cssText = "width:32px";

  var td_time_input1_ket = document.createElement('td');
  td_time_input1_ket.setAttribute("align", "center");


  var td_time_input2 = document.createElement('td');
  td_time_input2.setAttribute("id", i + "_td_time_input2");
  td_time_input2.style.cssText = "width:32px";
  var td_time_input2_ket = document.createElement('td');
  td_time_input2_ket.setAttribute("align", "center");

  var td_time_input3 = document.createElement('td');
  td_time_input3.setAttribute("id", i + "_td_time_input3");
  td_time_input3.style.cssText = "width:32px";

  var td_time_label1 = document.createElement('td');
  td_time_label1.setAttribute("id", i + "_td_time_label1");
  var td_time_label1_ket = document.createElement('td');
  var td_time_label2 = document.createElement('td');
  td_time_label2.setAttribute("id", i + "_td_time_label2");
  var td_time_label2_ket = document.createElement('td');
  var td_time_label3 = document.createElement('td');
  td_time_label3.setAttribute("id", i + "_td_time_label3");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");

  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes") {
    required.innerHTML = " *";
  }
  var hh = document.createElement('input');
  hh.setAttribute("type", 'text');
  hh.setAttribute("value", w_hh);
  hh.setAttribute("class", "time_box");
  hh.setAttribute("id", i + "_hhform_id_temp");
  hh.setAttribute("name", i + "_hhform_id_temp");
  hh.setAttribute("onKeyPress", "return check_hour(event, '" + i + "_hhform_id_temp', '23')");
  hh.setAttribute("onKeyUp", "change_hour(event, '" + i + "_hhform_id_temp','23')");
  hh.setAttribute("onBlur", "add_0('" + i + "_hhform_id_temp')");

  var hh_label = document.createElement('label');
  hh_label.setAttribute("class", "mini_label");
  hh_label.innerHTML = "HH";

  var hh_ = document.createElement('span');
  hh_.setAttribute("class", 'wdform_colon');
  hh_.style.cssText = "font-style:bold; vertical-align:middle";
  hh_.innerHTML = "&nbsp;:&nbsp;";

  var mm = document.createElement('input');
  mm.setAttribute("type", 'text');
  mm.setAttribute("value", w_mm);
  mm.setAttribute("class", "time_box");

  mm.setAttribute("id", i + "_mmform_id_temp");
  mm.setAttribute("name", i + "_mmform_id_temp");
  mm.setAttribute("onKeyPress", "return check_minute(event, '" + i + "_mmform_id_temp')");
  mm.setAttribute("onKeyUp", "change_minute(event, '" + i + "_mmform_id_temp')");
  mm.setAttribute("onBlur", "add_0('" + i + "_mmform_id_temp')");

  var mm_label = document.createElement('label');
  mm_label.setAttribute("class", "mini_label");
  mm_label.innerHTML = "MM";

  var mm_ = document.createElement('span');
  mm_.style.cssText = "font-style:bold; vertical-align:middle";
  mm_.innerHTML = "&nbsp;:&nbsp;";
  mm_.setAttribute("class", 'wdform_colon');

  var ss = document.createElement('input');
  ss.setAttribute("type", 'text');
  ss.setAttribute("value", w_ss);
  ss.setAttribute("class", "time_box");

  ss.setAttribute("id", i + "_ssform_id_temp");
  ss.setAttribute("name", i + "_ssform_id_temp");
  ss.setAttribute("onKeyPress", "return check_second(event, '" + i + "_ssform_id_temp')");
  ss.setAttribute("onKeyUp", "change_second('" + i + "_ssform_id_temp')");
  ss.setAttribute("onBlur", "add_0('" + i + "_ssform_id_temp')");

  var ss_label = document.createElement('label');
  ss_label.setAttribute("class", "mini_label");
  ss_label.innerHTML = "SS";
  var main_td = document.getElementById('show_table');

  td1.appendChild(label);
  td1.appendChild(required);

  td_time_input1.appendChild(hh);
  td_time_input1_ket.appendChild(hh_);
  td_time_input2.appendChild(mm);
  td_time_input2_ket.appendChild(mm_);
  td_time_input3.appendChild(ss);
  tr_time1.appendChild(td_time_input1);
  tr_time1.appendChild(td_time_input1_ket);
  tr_time1.appendChild(td_time_input2);
  tr_time1.appendChild(td_time_input2_ket);
  tr_time1.appendChild(td_time_input3);

  td_time_label1.appendChild(hh_label);
  td_time_label2.appendChild(mm_label);
  td_time_label3.appendChild(ss_label);
  tr_time2.appendChild(td_time_label1);
  tr_time2.appendChild(td_time_label1_ket);
  tr_time2.appendChild(td_time_label2);
  tr_time2.appendChild(td_time_label2_ket);
  tr_time2.appendChild(td_time_label3);
  table_time.appendChild(tr_time1);
  table_time.appendChild(tr_time2);

  td2.appendChild(adding_type);

  td2.appendChild(adding_required);
  td2.appendChild(table_time);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);

  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);

  if (w_field_label_pos == "top") {
    label_top(i);
  }
  if (w_time_type == "12") {
    format_12(i, w_am_pm, w_hh, w_mm, w_ss);
  }
  if (w_sec == "0") {
    second_no(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_time');
}

function type_date(i, w_field_label, w_field_label_pos, w_date, w_required, w_class, w_format, w_but_val, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_date";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";
  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px;";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  el_label_position1.setAttribute("checked", "checked");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }
  var el_format_label = document.createElement('label');
  el_format_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_format_label.innerHTML = "Date format";

  var el_format_textarea = document.createElement('input');
  el_format_textarea.setAttribute("id", "date_format");
  el_format_textarea.setAttribute("type", "text");
  el_format_textarea.setAttribute("value", w_format);
  el_format_textarea.style.cssText = "width:200px;";
  el_format_textarea.setAttribute("onChange", "change_date_format(this.value,'" + i + "')");

  var el_button_value_label = document.createElement('label');
  el_button_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_button_value_label.innerHTML = "Date Picker label";

  var el_button_value_textarea = document.createElement('input');
  el_button_value_textarea.setAttribute("id", "button_value");
  el_button_value_textarea.setAttribute("type", "text");
  el_button_value_textarea.setAttribute("value", w_but_val);
  el_button_value_textarea.style.cssText = "width:150px;";
  el_button_value_textarea.setAttribute("onKeyUp", "change_file_value(this.value,'" + i + "_buttonform_id_temp')");

  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";

  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes") {
    el_required.setAttribute("checked", "checked");
  }
  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_date')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_date')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");

    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_date')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_date')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var t = document.getElementById('edit_table');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');

  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);

  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);

  edit_main_td3.appendChild(el_format_label);
  edit_main_td3_1.appendChild(el_format_textarea);

  edit_main_td4.appendChild(el_button_value_label);
  edit_main_td4_1.appendChild(el_button_value_textarea);

  edit_main_td5.appendChild(el_style_label);
  edit_main_td5_1.appendChild(el_style_textarea);

  edit_main_td6.appendChild(el_required_label);
  edit_main_td6_1.appendChild(el_required);

  edit_main_td7.appendChild(el_attr_label);
  edit_main_td7.appendChild(el_attr_add);
  edit_main_td7.appendChild(br3);
  edit_main_td7.appendChild(el_attr_table);
  edit_main_td7.setAttribute("colspan", "2");

  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_tr7.appendChild(edit_main_td7_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr6);
  edit_main_table.appendChild(edit_main_tr7);
  edit_div.appendChild(edit_main_table);

  t.appendChild(edit_div);
  add_id_and_name(i, 'type_text');

  // Show table.
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_date");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");
  adding_required.setAttribute("id", i + "_requiredform_id_temp");

  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');
  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'middle');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'middle');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var table_date = document.createElement('table');
  table_date.setAttribute("id", i + "_table_date");
  table_date.setAttribute("cellpadding", '0');
  table_date.setAttribute("cellspacing", '0');

  var tr_date1 = document.createElement('tr');
  tr_date1.setAttribute("id", i + "_tr_date1");

  var tr_date2 = document.createElement('tr');
  tr_date2.setAttribute("id", i + "_tr_date2");

  var td_date_input1 = document.createElement('td');
  td_date_input1.setAttribute("id", i + "_td_date_input1");

  var td_date_input2 = document.createElement('td');
  td_date_input2.setAttribute("id", i + "_td_date_input2");

  var td_date_input3 = document.createElement('td');
  td_date_input3.setAttribute("id", i + "_td_date_input3");

  var td_date_label1 = document.createElement('td');
  td_date_label1.setAttribute("id", i + "_td_date_label1");

  var td_date_label2 = document.createElement('td');
  td_date_label2.setAttribute("id", i + "_td_date_label2");

  var td_date_label3 = document.createElement('td');
  td_date_label3.setAttribute("id", i + "_td_date_label3");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");
  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes") {
    required.innerHTML = " *";
  }
  var adding = document.createElement('input');
  adding.setAttribute("type", 'text');
  adding.setAttribute("value", w_date);
  adding.setAttribute("class", 'wdform_date');
  adding.setAttribute("id", i + "_elementform_id_temp");
  adding.setAttribute("name", i + "_elementform_id_temp");
  adding.setAttribute("maxlength", "10");
  adding.setAttribute("size", "10");
  adding.setAttribute("onChange", "change_value('" + i + "_elementform_id_temp')");

  var adding_button = document.createElement('input');
  adding_button.setAttribute("id", i + "_buttonform_id_temp");
  adding_button.setAttribute("class", "button form-submit");
  adding_button.setAttribute("type", 'reset');
  adding_button.setAttribute("value", w_but_val);
  adding_button.setAttribute("format", w_format);
  adding_button.setAttribute("onclick", "return showCalendar('" + i + "_elementform_id_temp' ,'" + w_format + "')");
  // adding_button.setAttribute("src", "templates/bluestork/images/system/calendar.png");
  // adding_button.setAttribute("alt", 'calendario');

  var main_td = document.getElementById('show_table');
  td1.appendChild(label);
  td1.appendChild(required);
  td2.appendChild(adding_type);
  td2.appendChild(adding_required);
  td2.appendChild(adding);
  td2.appendChild(adding_button);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);
  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  if (w_field_label_pos == "top") {
    label_top(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_date');
  Calendar.setup({
    inputField:i + '_elementform_id_temp',
    ifFormat:w_format,
    button:i + '_buttonform_id_temp',
    align:'Tl',
    singleClick:true,
    firstDay:0
  });
}

function add_id_and_name(i, type) {
  switch (type) {
    case 'type_text': {
      var edit_main_table = document.getElementById("edit_main_table");
      var edit_main_tr0 = document.createElement('tr');
      edit_main_tr0.setAttribute("valing", "top");

      var edit_main_td0 = document.createElement('td');
      edit_main_td0.style.cssText = "padding-top:10px";

      var edit_main_td0_1 = document.createElement('td');
      edit_main_td0_1.style.cssText = "padding-top:10px";

      var br = document.createElement('br');
      var br1 = document.createElement('br');

      var field_id = document.createElement('label');
      field_id.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px;  margin-right:27px; width:150px;";
      field_id.innerHTML = "Field id ";

      var field_id_text = document.createElement('input');
      field_id_text.setAttribute("size", "50");
      field_id_text.setAttribute("type", "text");
      field_id_text.setAttribute("disabled", "disabled");
      field_id_text.setAttribute("value", i + "_elementform_id_temp");

      var field_name = document.createElement('label');
      field_name.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px; margin-right:3px";
      field_name.innerHTML = "Field name ";

      var field_name_text = document.createElement('input');
      field_name_text.setAttribute("size", "50");
      field_name_text.setAttribute("type", "text");
      field_name_text.setAttribute("disabled", "disabled");
      field_name_text.setAttribute("value", i + "_elementform_id_temp");

      edit_main_td0.appendChild(field_id);
      // edit_main_td0.appendChild(br);
      edit_main_td0.appendChild(field_name);

      edit_main_td0_1.appendChild(field_id_text);
      edit_main_td0_1.appendChild(br1);
      edit_main_td0_1.appendChild(field_name_text);
      edit_main_tr0.appendChild(edit_main_td0);
      edit_main_tr0.appendChild(edit_main_td0_1);
      edit_main_table.insertBefore(edit_main_tr0, edit_main_table.childNodes[0]);
      break;

    }
    case 'type_address': {
      var edit_main_table = document.getElementById("edit_main_table");

      var edit_main_tr0 = document.createElement('tr');
      edit_main_tr0.setAttribute("valing", "top");

      var edit_main_td0 = document.createElement('td');
      edit_main_td0.style.cssText = "padding-top:10px";
      edit_main_td0.setAttribute("colspan", "2");

      var br = document.createElement('br');

      var field_id = document.createElement('label');
      field_id.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px;  margin-right:27px";
      field_id.innerHTML = "Fields id ";

      var field_id_text = document.createElement('input');
      field_id_text.setAttribute("type", "text");
      field_id_text.setAttribute("id", "field_id");
      field_id_text.setAttribute("disabled", "disabled");
      field_id_text.setAttribute("style", "width:350px");
      field_id_text.setAttribute("value", i + "_street1form_id_temp, " + i + "_street2form_id_temp, " + i + "_cityform_id_temp, " + i + "_stateform_id_temp, " + i + "_postalform_id_temp, " + i + "_countryform_id_temp");

      var field_name = document.createElement('label');
      field_name.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px; margin-right:3px";
      field_name.innerHTML = "Fields name ";

      var field_name_text = document.createElement('input');
      field_name_text.setAttribute("type", "text");
      field_name_text.setAttribute("id", "field_name");
      field_name_text.setAttribute("disabled", "disabled");
      field_name_text.setAttribute("style", "width:350px");
      field_name_text.setAttribute("value", i + "_street1form_id_temp, " + i + "_street2form_id_temp, " + i + "_cityform_id_temp, " + i + "_stateform_id_temp, " + i + "_postalform_id_temp, " + i + "_countryform_id_temp");

      edit_main_td0.appendChild(field_id);
      edit_main_td0.appendChild(field_id_text);
      edit_main_td0.appendChild(br);
      edit_main_td0.appendChild(field_name);
      edit_main_td0.appendChild(field_name_text);
      edit_main_tr0.appendChild(edit_main_td0);
      edit_main_table.insertBefore(edit_main_tr0, edit_main_table.childNodes[0]);
      break;

    }
    case 'type_name': {
      var edit_main_table = document.getElementById("edit_main_table");

      var edit_main_tr0 = document.createElement('tr');
      edit_main_tr0.setAttribute("valing", "top");

      var edit_main_td0 = document.createElement('td');
      edit_main_td0.style.cssText = "padding-top:10px";
      edit_main_td0.setAttribute("colspan", "2");

      var br = document.createElement('br');

      var field_id = document.createElement('label');
      field_id.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px;  margin-right:27px";
      field_id.innerHTML = "Fields id ";

      var field_id_text = document.createElement('input');
      field_id_text.setAttribute("type", "text");
      field_id_text.setAttribute("id", "field_id");
      field_id_text.setAttribute("disabled", "disabled");
      field_id_text.setAttribute("style", "width:350px");
      field_id_text.setAttribute("value", i + "_element_firstform_id_temp, " + i + "_element_lastform_id_temp");

      var field_name = document.createElement('label');
      field_name.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px; margin-right:3px";
      field_name.innerHTML = "Fields name ";

      var field_name_text = document.createElement('input');
      field_name_text.setAttribute("type", "text");
      field_name_text.setAttribute("id", "field_name");
      field_name_text.setAttribute("disabled", "disabled");
      field_name_text.setAttribute("style", "width:350px");
      field_name_text.setAttribute("value", i + "_element_firstform_id_temp, " + i + "_element_lastform_id_temp");

      edit_main_td0.appendChild(field_id);
      edit_main_td0.appendChild(field_id_text);
      edit_main_td0.appendChild(br);
      edit_main_td0.appendChild(field_name);
      edit_main_td0.appendChild(field_name_text);
      edit_main_tr0.appendChild(edit_main_td0);
      edit_main_table.insertBefore(edit_main_tr0, edit_main_table.childNodes[0]);
      break;

    }
    case 'type_radio': {
      var edit_main_table = document.getElementById("edit_main_table");

      var edit_main_tr0 = document.createElement('tr');
      edit_main_tr0.setAttribute("valing", "top");

      var edit_main_td0 = document.createElement('td');
      edit_main_td0.style.cssText = "padding-top:10px";
      edit_main_td0.setAttribute("colspan", "2");

      var br = document.createElement('br');

      var field_id = document.createElement('label');
      field_id.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px;  margin-right:27px";
      field_id.innerHTML = "Fields id ";

      var field_id_text = document.createElement('input');
      field_id_text.setAttribute("type", "text");
      field_id_text.setAttribute("id", "field_id");
      field_id_text.setAttribute("disabled", "disabled");
      field_id_text.setAttribute("style", "width:350px");
      field_id_text.setAttribute("value", '');

      var field_name = document.createElement('label');
      field_name.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px; margin-right:3px";
      field_name.innerHTML = "Fields name ";

      var field_name_text = document.createElement('input');
      field_name_text.setAttribute("type", "text");
      field_name_text.setAttribute("id", "field_name");
      field_name_text.setAttribute("disabled", "disabled");
      field_name_text.setAttribute("style", "width:350px");
      field_name_text.setAttribute("value", '');

      edit_main_td0.appendChild(field_id);
      edit_main_td0.appendChild(field_id_text);
      edit_main_td0.appendChild(br);
      edit_main_td0.appendChild(field_name);
      edit_main_td0.appendChild(field_name_text);
      edit_main_tr0.appendChild(edit_main_td0);
      edit_main_table.insertBefore(edit_main_tr0, edit_main_table.childNodes[0]);
      refresh_id_name(i, type);
      break;

    }
    case 'type_checkbox': {
      var edit_main_table = document.getElementById("edit_main_table");

      var edit_main_tr0 = document.createElement('tr');
      edit_main_tr0.setAttribute("valing", "top");

      var edit_main_td0 = document.createElement('td');
      edit_main_td0.style.cssText = "padding-top:10px";
      edit_main_td0.setAttribute("colspan", "2");

      var br = document.createElement('br');

      var field_id = document.createElement('label');
      field_id.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px;  margin-right:27px";
      field_id.innerHTML = "Fields id ";

      var field_id_text = document.createElement('input');
      field_id_text.setAttribute("type", "text");
      field_id_text.setAttribute("id", "field_id");
      field_id_text.setAttribute("disabled", "disabled");
      field_id_text.setAttribute("style", "width:350px");
      field_id_text.setAttribute("value", '');

      var field_name = document.createElement('label');
      field_name.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px; margin-right:3px";
      field_name.innerHTML = "Fields name ";

      var field_name_text = document.createElement('input');
      field_name_text.setAttribute("type", "text");
      field_name_text.setAttribute("id", "field_name");
      field_name_text.setAttribute("disabled", "disabled");
      field_name_text.setAttribute("style", "width:350px");
      field_name_text.setAttribute("value", '');

      edit_main_td0.appendChild(field_id);
      edit_main_td0.appendChild(field_id_text);
      edit_main_td0.appendChild(br);
      edit_main_td0.appendChild(field_name);
      edit_main_td0.appendChild(field_name_text);
      edit_main_tr0.appendChild(edit_main_td0);
      edit_main_table.insertBefore(edit_main_tr0, edit_main_table.childNodes[0]);
      refresh_id_name(i, type);
      break;

    }
    case 'type_time': {
      var edit_main_table = document.getElementById("edit_main_table");

      var edit_main_tr0 = document.createElement('tr');
      edit_main_tr0.setAttribute("valing", "top");

      var edit_main_td0 = document.createElement('td');
      edit_main_td0.style.cssText = "padding-top:10px";
      var edit_main_td0_1 = document.createElement('td');
      edit_main_td0_1.style.cssText = "padding-top:10px";

      var br = document.createElement('br');
      var br1 = document.createElement('br');

      var field_id = document.createElement('label');
      field_id.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px;  margin-right:27px";
      field_id.innerHTML = "Fields id ";

      var field_id_text = document.createElement('input');
      field_id_text.setAttribute("size", "50");
      field_id_text.setAttribute("type", "text");
      field_id_text.setAttribute("id", "field_id");
      field_id_text.setAttribute("disabled", "disabled");
      field_id_text.setAttribute("value", i + "_hhform_id_temp, " + i + "_mmform_id_temp, " + i + "_ssform_id_temp");

      var field_name = document.createElement('label');
      field_name.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px; margin-right:3px";
      field_name.innerHTML = "Fields name ";

      var field_name_text = document.createElement('input');
      field_name_text.setAttribute("size", "50");
      field_name_text.setAttribute("type", "text");
      field_name_text.setAttribute("id", "field_name");
      field_name_text.setAttribute("disabled", "disabled");
      field_name_text.setAttribute("value", i + "_hhform_id_temp, " + i + "_mmform_id_temp, " + i + "_ssform_id_temp");

      edit_main_td0.appendChild(field_id);
      // edit_main_td0.appendChild(br1);
      edit_main_td0.appendChild(field_name);
      edit_main_td0_1.appendChild(field_id_text);
      edit_main_td0_1.appendChild(br);
      edit_main_td0_1.appendChild(field_name_text);

      edit_main_tr0.appendChild(edit_main_td0);
      edit_main_tr0.appendChild(edit_main_td0_1);
      edit_main_table.insertBefore(edit_main_tr0, edit_main_table.childNodes[0]);
      break;

    }
    case 'type_date_fields': {
      var edit_main_table = document.getElementById("edit_main_table");

      var edit_main_tr0 = document.createElement('tr');
      edit_main_tr0.setAttribute("valing", "top");

      var edit_main_td0 = document.createElement('td');
      edit_main_td0.style.cssText = "padding-top:10px";
      edit_main_td0.setAttribute("colspan", "2");

      var br = document.createElement('br');

      var field_id = document.createElement('label');
      field_id.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px;  margin-right:27px";
      field_id.innerHTML = "Fields id ";

      var field_id_text = document.createElement('input');
      field_id_text.setAttribute("size", "50");
      field_id_text.setAttribute("type", "text");
      field_id_text.setAttribute("id", "field_id");
      field_id_text.setAttribute("disabled", "disabled");
      field_id_text.setAttribute("value", i + "_dayform_id_temp, " + i + "_monthform_id_temp, " + i + "_yearform_id_temp");

      var field_name = document.createElement('label');
      field_name.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px; margin-right:3px";
      field_name.innerHTML = "Fields name ";

      var field_name_text = document.createElement('input');
      field_name_text.setAttribute("size", "50");
      field_name_text.setAttribute("type", "text");
      field_name_text.setAttribute("id", "field_name");
      field_name_text.setAttribute("disabled", "disabled");
      field_name_text.setAttribute("value", i + "_dayform_id_temp, " + i + "_monthform_id_temp, " + i + "_yearform_id_temp");

      edit_main_td0.appendChild(field_id);
      edit_main_td0.appendChild(field_id_text);
      edit_main_td0.appendChild(br);
      edit_main_td0.appendChild(field_name);
      edit_main_td0.appendChild(field_name_text);
      edit_main_tr0.appendChild(edit_main_td0);
      edit_main_table.insertBefore(edit_main_tr0, edit_main_table.childNodes[0]);
      break;

    }
    case 'type_captcha': {
      var edit_main_table = document.getElementById("edit_main_table");

      var edit_main_tr0 = document.createElement('tr');
      edit_main_tr0.setAttribute("valing", "top");

      var edit_main_td0 = document.createElement('td');
      edit_main_td0.style.cssText = "padding-top:10px";

      var edit_main_td0_1 = document.createElement('td');
      edit_main_td0_1.style.cssText = "padding-top:10px";

      var br = document.createElement('br');
      var br1 = document.createElement('br');

      var field_id = document.createElement('label');
      field_id.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px;  margin-right:27px";
      field_id.innerHTML = "Fields id ";

      var field_id_text = document.createElement('input');
      field_id_text.setAttribute("size", "50");
      field_id_text.setAttribute("type", "text");
      field_id_text.setAttribute("id", "field_id");
      field_id_text.setAttribute("disabled", "disabled");
      field_id_text.setAttribute("value", "wd_captcha_inputform_id_temp");

      var field_name = document.createElement('label');
      field_name.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px; margin-right:3px";
      field_name.innerHTML = "Fields name ";

      var field_name_text = document.createElement('input');
      field_name_text.setAttribute("size", "50");
      field_name_text.setAttribute("type", "text");
      field_name_text.setAttribute("id", "field_name");
      field_name_text.setAttribute("disabled", "disabled");
      field_name_text.setAttribute("value", "captcha_inputform_id_temp");

      edit_main_td0.appendChild(field_id);
      // edit_main_td0.appendChild(br1);
      edit_main_td0.appendChild(field_name);
      edit_main_td0_1.appendChild(field_id_text);
      edit_main_td0_1.appendChild(br);
      edit_main_td0_1.appendChild(field_name_text);
      edit_main_tr0.appendChild(edit_main_td0);
      edit_main_tr0.appendChild(edit_main_td0_1);
      edit_main_table.insertBefore(edit_main_tr0, edit_main_table.childNodes[0]);
      break;
    }
  }
}

function refresh_id_name(i, type) {
  switch (type) {
    case 'type_radio': {
      document.getElementById('field_id').value = '';
      for (k = 0; k < 50; k++) {
        if (document.getElementById(i + '_elementform_id_temp' + k)) {
          document.getElementById('field_id').value += i + '_elementform_id_temp' + k + ', ';
        }
      }
      a = document.getElementById('field_id').value.slice(0, -2);
      document.getElementById('field_id').value = a;
      document.getElementById('field_name').value = i + '_element';
      break;

    }
    case 'type_checkbox': {
      document.getElementById('field_id').value = '';
      for (k = 0; k < 50; k++) {
        if (document.getElementById(i + '_elementform_id_temp' + k)) {
          document.getElementById('field_id').value += i + '_elementform_id_temp' + k + ', ';
        }
      }
      a = document.getElementById('field_id').value.slice(0, -2);
      document.getElementById('field_id').value = a;
      document.getElementById('field_name').value = a;
      break;

    }
  }
}

function add_attr(i, type) {
  var el_attr_table = document.getElementById('attributes');
  j = parseInt(el_attr_table.lastChild.getAttribute('idi')) + 1;
  w_attr_name[j] = "attribute";
  w_attr_value[j] = "value";
  var el_attr_tr = document.createElement('tr');
  el_attr_tr.setAttribute("id", "attr_row_" + j);
  el_attr_tr.setAttribute("idi", j);
  var el_attr_td_name = document.createElement('td');
  el_attr_td_name.style.cssText = 'width:100px';
  var el_attr_td_value = document.createElement('td');
  el_attr_td_value.style.cssText = 'width:100px';
  var el_attr_td_X = document.createElement('td');
  var el_attr_name = document.createElement('input');
  el_attr_name.setAttribute("type", "text");
  el_attr_name.style.cssText = "width:100px";
  el_attr_name.setAttribute("value", w_attr_name[j]);
  el_attr_name.setAttribute("id", "attr_name" + j);
  el_attr_name.setAttribute("onChange", "change_attribute_name('" + i + "', this, '" + type + "')");
  var el_attr_value = document.createElement('input');
  el_attr_value.setAttribute("type", "text");
  el_attr_value.style.cssText = "width:100px";
  el_attr_value.setAttribute("value", w_attr_value[j]);
  el_attr_value.setAttribute("id", "attr_value" + j);
  el_attr_value.setAttribute("onChange", "change_attribute_value('" + i + "', " + j + ", '" + type + "')");
  var el_attr_remove = document.createElement('img');
  el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
  el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
  el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
  el_attr_remove.setAttribute("align", 'top');
  el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", '" + type + "')");
  el_attr_table.appendChild(el_attr_tr);
  el_attr_tr.appendChild(el_attr_td_name);
  el_attr_tr.appendChild(el_attr_td_value);
  el_attr_tr.appendChild(el_attr_td_X);
  el_attr_td_name.appendChild(el_attr_name);
  el_attr_td_value.appendChild(el_attr_value);
  el_attr_td_X.appendChild(el_attr_remove);
  refresh_attr(i, type);
}

function change_date_format(value, id) {
  input_p = document.getElementById(id + '_buttonform_id_temp');
  input_p.setAttribute("onclick", "return showCalendar('" + id + "_elementform_id_temp' , '" + value + "')");
  input_p.setAttribute("format", value);
}

function set_send(id) {
  if (document.getElementById(id).value == "yes") {
    document.getElementById(id).setAttribute("value", "no");
  }
  else {
    document.getElementById(id).setAttribute("value", "yes");
  }
}

function change_class(x, id) {
  if (document.getElementById(id + '_label_sectionform_id_temp')) {
    document.getElementById(id + '_label_sectionform_id_temp').setAttribute("class", x);
  }
  if (document.getElementById(id + '_element_sectionform_id_temp')) {
    document.getElementById(id + '_element_sectionform_id_temp').setAttribute("class", x);
  }
}

function set_required(id) {
  if (document.getElementById(id + "form_id_temp").value == "yes") {
    document.getElementById(id + "form_id_temp").setAttribute("value", "no");
    document.getElementById(id + "_elementform_id_temp").innerHTML = "";
  }
  else {
    document.getElementById(id + "form_id_temp").setAttribute("value", "yes")
    document.getElementById(id + "_elementform_id_temp").innerHTML = " *";
  }
}

function set_unique(id) {
  if (document.getElementById(id).value == "yes") {
    document.getElementById(id).setAttribute("value", "no");
  }
  else {
    document.getElementById(id).setAttribute("value", "yes")
  }
}

function set_randomize(id) {
  if (document.getElementById(id).value == "yes") {
    document.getElementById(id).setAttribute("value", "no");
  }
  else {
    document.getElementById(id).setAttribute("value", "yes")
  }
}

function show_other_input(num) {
  for (k = 0; k < 50; k++) {
    if (document.getElementById(num + "_elementform_id_temp" + k)) {
      if (document.getElementById(num + "_elementform_id_temp" + k).getAttribute('other')) {
        if (document.getElementById(num + "_elementform_id_temp" + k).getAttribute('other') == 1) {
          element_other = document.getElementById(num + "_elementform_id_temp" + k);
          break;
        }
      }
    }
  }
  parent = element_other.parentNode;
  var br = document.createElement('br');
  br.setAttribute("id", num + "_other_brform_id_temp");
  var el_other = document.createElement('input');
  el_other.setAttribute("id", num + "_other_inputform_id_temp");
  el_other.setAttribute("name", num + "_other_inputform_id_temp");
  el_other.setAttribute("type", "text");
  el_other.setAttribute("class", "other_input");
  parent.appendChild(br);
  parent.appendChild(el_other);
}

function set_allow_other(num, type) {
  if (document.getElementById(num + '_allow_otherform_id_temp').value == "yes") {
    document.getElementById(num + '_allow_otherform_id_temp').setAttribute("value", "no");
    for (k = 0; k < 50; k++) {
      if (document.getElementById(num + "_elementform_id_temp" + k)) {
        if (document.getElementById(num + "_elementform_id_temp" + k).getAttribute('other')) {
          if (document.getElementById(num + "_elementform_id_temp" + k).getAttribute('other') == 1) {
            remove_choise(k, num);
            break;
          }
        }
      }
    }
  }
  else {
    document.getElementById(num + '_allow_otherform_id_temp').setAttribute("value", "yes");
    var q = 0;
    if (document.getElementById(num + '_hor')) {
      q = 1;
      flow_ver(num);
    }
    j++;
    element = 'input';
    var table = document.getElementById(num + '_table_little');
    var tr = document.createElement('tr');
    tr.setAttribute("id", num + "_element_tr" + j);
    var td = document.createElement('td');
    td.setAttribute("valign", "top");
    td.setAttribute("id", num + "_td_little" + j);
    td.setAttribute("idi", j);
    var adding = document.createElement(element);
    adding.setAttribute("type", type);
    adding.setAttribute("value", "other");
    adding.setAttribute("other", "1");
    adding.setAttribute("id", num + "_elementform_id_temp" + j);
    if (type == "checkbox") {
      adding.setAttribute("onClick", "if(set_checked('" + num + "','" + j + "','form_id_temp')) show_other_input('" + num + "','form_id_temp');");
      adding.setAttribute("name", num + "_elementform_id_temp" + j);
    }
    else {
      adding.setAttribute("onClick", "set_default('" + num + "','" + j + "','form_id_temp'); show_other_input('" + num + "','form_id_temp');");
      adding.setAttribute("name", num + "_elementform_id_temp");
    }
    var label_adding = document.createElement('label');
    label_adding.setAttribute("id", num + "_label_element" + j);
    label_adding.setAttribute("class", "ch_rad_label");
    label_adding.setAttribute("for", num + "_elementform_id_temp" + j);
    label_adding.innerHTML = "other";
    td.appendChild(adding);
    td.appendChild(label_adding);
    tr.appendChild(td);
    table.appendChild(tr);
    var choices_td = document.getElementById('choices');
    var br = document.createElement('br');
    br.setAttribute("id", "br" + j);
    var el_choices = document.createElement('input');
    el_choices.setAttribute("id", "el_choices" + j);
    el_choices.setAttribute("type", "text");
    el_choices.setAttribute("value", "other");
    el_choices.style.cssText = "width:100px; margin:0; padding:0; border-width: 1px";
    el_choices.setAttribute("onKeyUp", "change_label('" + num + "_label_element" + j + "', this.value); change_in_value('" + num + "_elementform_id_temp" + j + "', this.value)");
    var el_choices_remove = document.createElement('img');
    el_choices_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_choices_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_choices_remove.style.cssText = 'cursor:pointer;vertical-align:middle; margin:3px; display:none';
    el_choices_remove.setAttribute("align", 'top');
    el_choices_remove.setAttribute("onClick", "remove_choise('" + j + "','" + num + "')");
    choices_td.appendChild(br);
    choices_td.appendChild(el_choices);
    choices_td.appendChild(el_choices_remove);
    refresh_attr(num, 'type_checkbox');
    if (q == 1) {
      flow_hor(num);
    }
  }
}

function flow_hor(id) {
  tbody = document.getElementById(id + '_table_little');
  td_array = new Array();
  n = tbody.childNodes.length;
  for (k = 0; k < n; k++) {
    td_array[k] = tbody.childNodes[k].childNodes[0];
  }
  for (k = 0; k < n; k++) {
    tbody.removeChild(tbody.childNodes[0]);
  }
  var tr = document.createElement('tr');
  tr.setAttribute("id", id + "_hor");
  tbody.appendChild(tr);
  for (k = 0; k < n; k++) {
    tr.appendChild(td_array[k]);
  }
}

function flow_ver(id) {
  tbody = document.getElementById(id + '_table_little');
  tr = document.getElementById(id + '_hor');
  td_array = new Array();
  n = tr.childNodes.length;
  for (k = 0; k < n; k++) {
    td_array[k] = tr.childNodes[k];
  }
  tbody.removeChild(tr);
  for (k = 0; k < n; k++) {
    var tr_little = document.createElement('tr');
    tr_little.setAttribute("id", id + "_element_tr" + td_array[k].getAttribute("idi"));
    tr_little.appendChild(td_array[k]);
    tbody.appendChild(tr_little);
  }
}

function second_no(id) {
  time_box = document.getElementById(id + '_tr_time1');
  text_box = document.getElementById(id + '_tr_time2');
  second_box = document.getElementById(id + '_td_time_input3');
  second_text = document.getElementById(id + '_td_time_label3');
  document.getElementById(id + '_td_time_input2').parentNode.removeChild(document.getElementById(id + '_td_time_input2').nextSibling);
  time_box.removeChild(second_box);
  text_box.removeChild(second_text.previousSibling);
  text_box.removeChild(second_text);

}

function second_yes(id, w_ss) {
  time_box = document.getElementById(id + '_tr_time1');
  text_box = document.getElementById(id + '_tr_time2');
  var td_time_input2_ket = document.createElement('td');
  td_time_input2_ket.setAttribute("align", "center");
  var td_time_input3 = document.createElement('td');
  td_time_input3.setAttribute("id", id + "_td_time_input3");
  var td_time_label2_ket = document.createElement('td');
  var td_time_label3 = document.createElement('td');
  td_time_label3.setAttribute("id", id + "_td_time_label3");
  var mm_ = document.createElement('span');
  mm_.style.cssText = "font-style:bold; vertical-align:middle";
  mm_.innerHTML = "&nbsp;:&nbsp;";
  td_time_input2_ket.appendChild(mm_);
  var ss = document.createElement('input');
  ss.setAttribute("type", 'text');
  ss.setAttribute("value", w_ss);
  ss.setAttribute("class", "time_box");
  ss.setAttribute("id", id + "_ssform_id_temp");
  ss.setAttribute("name", id + "_ssform_id_temp");
  ss.setAttribute("onKeyPress", "return check_second(event, '" + id + "_ssform_id_temp')");
  ss.setAttribute("onKeyUp", "change_second('" + id + "_ssform_id_temp')");
  ss.setAttribute("onBlur", "add_0('" + id + "_ssform_id_temp')");
  var ss_label = document.createElement('label');
  ss_label.setAttribute("class", "mini_label");
  ss_label.innerHTML = "SS";
  td_time_input3.appendChild(ss);
  td_time_label3.appendChild(ss_label);
  if (document.getElementById(id + '_am_pm_select')) {
    select_ = document.getElementById(id + "_am_pm_select");
    select_text = document.getElementById(id + "_am_pm_label");
    time_box.insertBefore(td_time_input3, select_);
    time_box.insertBefore(td_time_input2_ket, td_time_input3);
    text_box.insertBefore(td_time_label3, select_text);
    text_box.insertBefore(td_time_label2_ket, td_time_label3);
  }
  else {
    time_box.appendChild(td_time_input2_ket);
    time_box.appendChild(td_time_input3);
    text_box.appendChild(td_time_label2_ket);
    text_box.appendChild(td_time_label3);
  }
  refresh_attr(id, 'type_time');
}

function check_isnum(e) {
  var chCode1 = e.which || e.keyCode;
  if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57)) {
    return false;
  }
  return true;
}

function change_w_style(id, w) {
  if (document.getElementById(id)) {
    document.getElementById(id).style.width = w + "px";
  }
}

function change_w_label(id, w) {
  if (document.getElementById(id)) {
    document.getElementById(id).innerHTML = w;
  }
}

function change_h_style(id, h) {
  document.getElementById(id).style.height = h + "px";
}

function set_checked(id, j) {
  checking = document.getElementById(id + "_elementform_id_temp" + j);
  if (checking.checked) {
    checking.setAttribute("checked", "checked");
  }
  if (!checking.checked) {
    checking.removeAttribute("checked");
    if (checking.getAttribute('other')) {
      if (checking.getAttribute('other') == 1) {
        if (document.getElementById(id + "_other_inputform_id_temp")) {
          document.getElementById(id + "_other_inputform_id_temp").parentNode.removeChild(document.getElementById(id + "_other_brform_id_temp"));
          document.getElementById(id + "_other_inputform_id_temp").parentNode.removeChild(document.getElementById(id + "_other_inputform_id_temp"));
        }
        return false;
      }
    }
  }
  return true;
}

function set_default(id, j) {
  for (k = 0; k < 100; k++) {
    if (document.getElementById(id + "_elementform_id_temp" + k)) {
      if (!document.getElementById(id + "_elementform_id_temp" + k).checked) {
        document.getElementById(id + "_elementform_id_temp" + k).removeAttribute("checked");
      }
      else {
        document.getElementById(id + "_elementform_id_temp" + j).setAttribute("checked", "checked");
      }
    }
  }
  if (document.getElementById(id + "_other_inputform_id_temp")) {
    document.getElementById(id + "_other_inputform_id_temp").parentNode.removeChild(document.getElementById(id + "_other_brform_id_temp"));
    document.getElementById(id + "_other_inputform_id_temp").parentNode.removeChild(document.getElementById(id + "_other_inputform_id_temp"));
  }
}

function add_0(id) {
  input = document.getElementById(id);
  if (input.value.length == 1) {
    input.value = '0' + input.value;
    input.setAttribute("value", input.value);
  }
}

function change_hour(ev, id, hour_interval) {
  if (check_hour(ev, id, hour_interval)) {
    input = document.getElementById(id);
    input.setAttribute("value", input.value);
  }
}

function change_minute(ev, id) {
  if (check_minute(ev, id)) {
    input = document.getElementById(id);
    input.setAttribute("value", input.value);
  }
}

function change_second(ev, id) {
  if (check_second(ev, id)) {
    input = document.getElementById(id);
    input.setAttribute("value", input.value);
  }
}

function check_hour(e, id, hour_interval) {
  var chCode1 = e.which || e.keyCode;
  if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57)) {
    return false;
  }
  hour = "" + document.getElementById(id).value + String.fromCharCode(chCode1);
  hour = parseFloat(hour);
  if ((hour < 0) || (hour > hour_interval)) {
    return false;
  }
  return true;
}

function check_minute(e, id) {
  var chCode1 = e.which || e.keyCode;
  if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57)) {
    return false;
  }
  minute = "" + document.getElementById(id).value + String.fromCharCode(chCode1);
  minute = parseFloat(minute);
  if ((minute < 0) || (minute > 59)) {
    return false;
  }
  return true;
}

function check_second(e, id) {
  var chCode1 = e.which || e.keyCode;
  if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57)) {
    return false;
  }
  second = "" + document.getElementById(id).value + String.fromCharCode(chCode1);
  second = parseFloat(second);
  if ((second < 0) || (second > 59)) {
    return false;
  }
  return true;
}

function label_top(num) {
  table = document.getElementById(num + '_elemet_tableform_id_temp');
  td1 = document.getElementById(num + '_label_sectionform_id_temp');
  td2 = document.getElementById(num + '_element_sectionform_id_temp');
  var table_t = document.createElement('tbody');
  var new_td1 = document.createElement('td');
  new_td1 = td1;
  var new_td2 = document.createElement('td');
  new_td2 = td2;
  var tr1 = document.createElement('tr');
  var tr2 = document.createElement('tr');
  while (table.firstChild) {
    table.removeChild(table.firstChild);
  }
  tr1.appendChild(new_td1);
  tr2.appendChild(new_td2);
  table_t.appendChild(tr1);
  table_t.appendChild(tr2);
  table.appendChild(table_t);
}

function label_left(num) {
  table = document.getElementById(num + '_elemet_tableform_id_temp');
  td1 = document.getElementById(num + '_label_sectionform_id_temp');
  td2 = document.getElementById(num + '_element_sectionform_id_temp');
  var table_t = document.createElement('tbody');
  var new_td1 = document.createElement('td');
  new_td1 = td1;
  var new_td2 = document.createElement('td');
  new_td2 = td2;
  var tr = document.createElement('tr');
  while (table.firstChild) {
    table.removeChild(table.firstChild);
  }
  tr.appendChild(new_td1);
  tr.appendChild(new_td2);
  table_t.appendChild(tr);
  table.appendChild(table_t);
}

function delete_value(id) {
  ofontStyle = document.getElementById(id).className;
  if (ofontStyle == "input_deactive") {
    document.getElementById(id).value = "";
    destroyChildren(document.getElementById(id));
    document.getElementById(id).setAttribute("class", "input_active");
    document.getElementById(id).className = 'input_active';
  }
}

function return_value(id) {
  input = document.getElementById(id);
  if (input.value == "") {
    input.value = input.title;
    input.className = 'input_deactive';
    input.setAttribute("class", 'input_deactive');
  }
}

function change_value(id) {
  input = document.getElementById(id);
  tag = input.tagName;
  if (tag == "TEXTAREA") {
    input.innerHTML = input.value;
  }
  else {
    input.setAttribute("value", input.value);
  }
}

function change_input_value(first_value, id) {
  input = document.getElementById(id);
  input.title = first_value;
  if (window.getComputedStyle) {
    ofontStyle = window.getComputedStyle(input, null).fontStyle;
  }
  else if (input.currentStyle) {
    ofontStyle = input.currentStyle.fontStyle;
  }
  if (ofontStyle == "italic") {
    input.value = first_value;
    input.setAttribute("value", first_value);
  }
}

function change_file_value(destination, id, prefix, postfix) {
  if (typeof(prefix) == 'undefined') {
    prefix = '';
    postfix = ''
  }
  input = document.getElementById(id);
  input.value = prefix + destination + postfix;
  input.setAttribute("value", prefix + destination + postfix);
}

function change_label(id, label) {
  document.getElementById(id).innerHTML = label;
  document.getElementById(id).value = label;
}

function change_in_value(id, label) {
  document.getElementById(id).setAttribute("value", label);
}

function add_choise(type, num) {
  var q = 0;
  if (document.getElementById(num + '_hor')) {
    q = 1;
    flow_ver(num);
  }
  j++;
  if (type == 'radio' || type == 'checkbox') {
    element = 'input';
    var table = document.getElementById(num + '_table_little');
    var tr = document.createElement('tr');
    tr.setAttribute("id", num + "_element_tr" + j);
    var td = document.createElement('td');
    td.setAttribute("valign", "top");
    td.setAttribute("id", num + "_td_little" + j);
    td.setAttribute("idi", j);
    var adding = document.createElement(element);
    adding.setAttribute("type", type);
    adding.setAttribute("value", "");
    adding.setAttribute("id", num + "_elementform_id_temp" + j);
    if (type == 'checkbox') {
      adding.setAttribute("onClick", "set_checked('" + num + "','" + j + "','form_id_temp')");
      adding.setAttribute("name", num + "_elementform_id_temp" + j);
    }
    if (type == 'radio') {
      adding.setAttribute("onClick", "set_default('" + num + "','" + j + "','form_id_temp')");
      adding.setAttribute("name", num + "_elementform_id_temp");
    }
    var label_adding = document.createElement('label');
    label_adding.setAttribute("id", num + "_label_element" + j);
    label_adding.setAttribute("class", "ch_rad_label");
    label_adding.setAttribute("for", num + "_elementform_id_temp" + j);
    td.appendChild(adding);
    td.appendChild(label_adding);
    tr.appendChild(td);
    table.appendChild(tr);
    var choices_td = document.getElementById('choices');
    var br = document.createElement('br');
    br.setAttribute("id", "br" + j);
    var el_choices = document.createElement('input');
    el_choices.setAttribute("id", "el_choices" + j);
    el_choices.setAttribute("type", "text");
    el_choices.setAttribute("value", "");
    el_choices.style.cssText = "width:100px; margin:0; padding:0; border-width: 1px";
    el_choices.setAttribute("onKeyUp", "change_label('" + num + "_label_element" + j + "', this.value); change_in_value('" + num + "_elementform_id_temp" + j + "', this.value)");
    var el_choices_remove = document.createElement('img');
    el_choices_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_choices_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_choices_remove.style.cssText = 'cursor:pointer;vertical-align:middle; margin:3px';
    el_choices_remove.setAttribute("align", 'top');
    el_choices_remove.setAttribute("onClick", "remove_choise('" + j + "','" + num + "')");
    choices_td.appendChild(br);
    choices_td.appendChild(el_choices);
    choices_td.appendChild(el_choices_remove);
    if (type == 'checkbox') {
      refresh_id_name(num, 'type_checkbox');
    }
    if (type == 'radio') {
      refresh_id_name(num, 'type_radio');
    }
    refresh_attr(num, 'type_checkbox');
  }
  if (type == 'select') {
    var select_ = document.getElementById(num + '_elementform_id_temp');
    var option = document.createElement('option');
    option.setAttribute("id", num + "_option" + j);
    select_.appendChild(option);
    var choices_td = document.getElementById('choices');
    var br = document.createElement('br');
    br.setAttribute("id", "br" + j);
    var el_choices = document.createElement('input');
    el_choices.setAttribute("id", "el_option" + j);
    el_choices.setAttribute("type", "text");
    el_choices.setAttribute("value", "");
    el_choices.style.cssText = "width:100px; margin:0; padding:0; border-width: 1px";
    el_choices.setAttribute("onKeyUp", "change_label('" + num + "_option" + j + "', this.value)");
    var el_choices_remove = document.createElement('img');
    el_choices_remove.setAttribute("id", "el_option" + j + "_remove");
    el_choices_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_choices_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_choices_remove.setAttribute("align", 'top');
    el_choices_remove.setAttribute("onClick", "remove_option('" + j + "','" + num + "')");
    var el_choices_dis = document.createElement('input');
    el_choices_dis.setAttribute("type", 'checkbox');
    el_choices_dis.setAttribute("id", "el_option" + j + "_dis");
    el_choices_dis.setAttribute("onClick", "dis_option('" + num + "_option" + j + "', this.checked)");
    choices_td.appendChild(br);
    choices_td.appendChild(el_choices);
    choices_td.appendChild(el_choices_dis);
    choices_td.appendChild(el_choices_remove);
  }
  if (q == 1) {
    flow_hor(num);
  }
}

function remove_choise(id, num) {
  var q = 0;
  if (document.getElementById(num + '_hor')) {
    q = 1;
    flow_ver(num);
  }
  j++;
  var table = document.getElementById(num + '_table_little');
  var tr = document.getElementById(num + '_element_tr' + id);
  table.removeChild(tr);
  var choices_td = document.getElementById('choices');
  var el_choices = document.getElementById('el_choices' + id);
  var el_choices_remove = document.getElementById('el_choices' + id + '_remove');
  var br = document.getElementById('br' + id);
  choices_td.removeChild(el_choices);
  choices_td.removeChild(el_choices_remove);
  choices_td.removeChild(br);
  if (q == 1) {
    flow_hor(num);
  }
  refresh_id_name(num, document.getElementById(num + '_typeform_id_temp').value);
}

function remove_option(id, num) {
  var select_ = document.getElementById(num + '_elementform_id_temp');
  var option = document.getElementById(num + '_option' + id);
  select_.removeChild(option);
  var choices_td = document.getElementById('choices');
  var el_choices = document.getElementById('el_option' + id);
  var el_choices_dis = document.getElementById('el_option' + id + '_dis');
  var el_choices_remove = document.getElementById('el_option' + id + '_remove');
  var br = document.getElementById('br' + id);
  choices_td.removeChild(el_choices);
  choices_td.removeChild(el_choices_dis);
  choices_td.removeChild(el_choices_remove);
  choices_td.removeChild(br);
}

function getIFrameDocument(aID) {
  var rv = null;
  // if contentDocument exists, W3C compliant (Mozilla) 
  if (document.getElementById(aID).contentDocument) {
    rv = document.getElementById(aID).contentDocument;
  } else {
  // IE 
    rv = document.frames[aID].document;
  }
  return rv;
}

function delete_last_child() {  
  // if (document.getElementsByTagName("iframe")[1]) {
    // ifr_id = document.getElementsByTagName("iframe")[1].id;
    // ifr = getIFrameDocument(ifr_id);
    // ifr.body.innerHTML = "";
  // }
  document.getElementById('main_editor').style.display = "none";
  // document.getElementById('editor').value = "";
  if (document.getElementById('show_table').lastChild) {
    var del1 = document.getElementById('show_table').lastChild;
    var del2 = document.getElementById('edit_table').lastChild;
    var main1 = document.getElementById('show_table');
    var main2 = document.getElementById('edit_table');
    main1.removeChild(del1);
    main2.removeChild(del2);
  }
}

function format_12(num, am_or_pm, w_hh, w_mm, w_ss) {
  tr_time1 = document.getElementById(num + '_tr_time1')
  tr_time2 = document.getElementById(num + '_tr_time2')
  var td1 = document.createElement('td');
  td1.setAttribute("id", num + "_am_pm_select");
  td1.setAttribute("class", "td_am_pm_select");
  var td2 = document.createElement('td');
  td2.setAttribute("id", num + "_am_pm_label");
  td2.setAttribute("class", "td_am_pm_select");
  var am_pm_select = document.createElement('select');
  am_pm_select.setAttribute("class", "am_pm_select form-select");
  am_pm_select.style.height = "25px";
  am_pm_select.setAttribute("name", num + "_am_pmform_id_temp");
  am_pm_select.setAttribute("id", num + "_am_pmform_id_temp");
  am_pm_select.setAttribute("onchange", "set_sel_am_pm(this)");
  var am_option = document.createElement('option');
  am_option.setAttribute("value", "am");
  am_option.innerHTML = "AM";
  var pm_option = document.createElement('option');
  pm_option.setAttribute("value", "pm");
  pm_option.innerHTML = "PM";
  if (am_or_pm == "pm") {
    pm_option.setAttribute("selected", "selected");
  }
  else {
    am_option.setAttribute("selected", "selected");
  }
  var am_pm_label = document.createElement('label');
  am_pm_label.setAttribute("class", "mini_label");
  am_pm_label.innerHTML = "AM/PM";
  am_pm_select.appendChild(am_option);
  am_pm_select.appendChild(pm_option);
  td1.appendChild(am_pm_select);
  td2.appendChild(am_pm_label);
  tr_time1.appendChild(td1);
  tr_time2.appendChild(td2);
  document.getElementById(num + '_hhform_id_temp').setAttribute("onKeyPress", "return check_hour(event, '" + num + "_hhform_id_temp'," + "'12'" + ")");
  document.getElementById(num + '_hhform_id_temp').value = w_hh;
  document.getElementById(num + '_mmform_id_temp').value = w_mm;
  if (document.getElementById(num + '_ssform_id_temp')) {
    document.getElementById(num + '_ssform_id_temp').value = w_ss;
  }
  refresh_attr(num, 'type_time');
}

function format_24(num) {
  tr_time1 = document.getElementById(num + '_tr_time1')
  td1 = document.getElementById(num + '_am_pm_select')
  tr_time2 = document.getElementById(num + '_tr_time2')
  td2 = document.getElementById(num + '_am_pm_label')
  tr_time1.removeChild(td1);
  tr_time2.removeChild(td2);
  document.getElementById(num + '_hhform_id_temp').setAttribute("onKeyPress", "return check_hour(event, '" + num + "_hhform_id_temp', '23')");
  document.getElementById(num + '_hhform_id_temp').value = "";
  document.getElementById(num + '_mmform_id_temp').value = "";
  if (document.getElementById(num + '_ssform_id_temp')) {
    document.getElementById(num + '_ssform_id_temp').value = "";
  }
}

function format_extended(num) {
  w_size = document.getElementById(num + '_element_firstform_id_temp').style.width;
  tr_name1 = document.getElementById(num + '_tr_name1');
  tr_name2 = document.getElementById(num + '_tr_name2');
  var td_name_input1 = document.createElement('td');
  td_name_input1.setAttribute("id", num + "_td_name_input_title");
  var td_name_input4 = document.createElement('td');
  td_name_input4.setAttribute("id", num + "_td_name_input_middle");
  var td_name_label1 = document.createElement('td');
  td_name_label1.setAttribute("id", num + "_td_name_label_title");
  td_name_label1.setAttribute("align", "left");
  var td_name_label4 = document.createElement('td');
  td_name_label4.setAttribute("id", num + "_td_name_label_middle");
  td_name_label4.setAttribute("align", "left");
  var title = document.createElement('input');
  title.setAttribute("type", 'text');
  title.setAttribute("class", "input_deactive");
  title.style.cssText = "border-width:1px; margin: 0px 10px 0px 0px; padding: 0px; width:40px";
  title.setAttribute("id", num + "_element_titleform_id_temp");
  title.setAttribute("name", num + "_element_titleform_id_temp");
  title.setAttribute("value", '');
  title.setAttribute("title", '');
  title.setAttribute("onFocus", 'delete_value("' + num + '_element_titleform_id_temp")');
  title.setAttribute("onBlur", 'return_value("' + num + '_element_titleform_id_temp")');
  title.setAttribute("onChange", "change_value('" + num + "_element_titleform_id_temp')");
  var title_label = document.createElement('label');
  title_label.setAttribute("class", "mini_label");
  title_label.innerHTML = "<!--repstart-->Title<!--repend-->";
  var middle = document.createElement('input');
  middle.setAttribute("type", 'text');
  middle.setAttribute("class", "input_deactive");
  middle.style.cssText = "border-width:1px; padding: 0px; width:" + w_size;
  middle.setAttribute("id", num + "_element_middleform_id_temp");
  middle.setAttribute("name", num + "_element_middleform_id_temp");
  middle.setAttribute("value", '');
  middle.setAttribute("title", '');
  middle.setAttribute("onFocus", 'delete_value("' + num + '_element_middleform_id_temp")');
  middle.setAttribute("onBlur", 'return_value("' + num + '_element_middleform_id_temp")');
  middle.setAttribute("onChange", "change_value('" + num + "_element_middleform_id_temp')");
  var middle_label = document.createElement('label');
  middle_label.setAttribute("class", "mini_label");
  middle_label.innerHTML = "<!--repstart-->Middle<!--repend-->";
  first_input = document.getElementById(num + '_td_name_input_first');
  last_input = document.getElementById(num + '_td_name_input_last');
  first_label = document.getElementById(num + '_td_name_label_first');
  last_label = document.getElementById(num + '_td_name_label_last');
  td_name_input1.appendChild(title);
  td_name_input4.appendChild(middle);
  tr_name1.insertBefore(td_name_input1, first_input);
  tr_name1.insertBefore(td_name_input4, null);
  td_name_label1.appendChild(title_label);
  td_name_label4.appendChild(middle_label);
  tr_name2.insertBefore(td_name_label1, first_label);
  tr_name2.insertBefore(td_name_label4, null);
  var gic1 = document.createTextNode("-");
  var gic2 = document.createTextNode("-");
  var el_first_value_title = document.createElement('input');
  el_first_value_title.setAttribute("id", "el_first_value_title");
  el_first_value_title.setAttribute("type", "text");
  el_first_value_title.setAttribute("value", '');
  el_first_value_title.style.cssText = "width:50px; margin-left:4px; margin-right:4px";
  el_first_value_title.setAttribute("onKeyUp", "change_input_value(this.value,'" + num + "_element_titleform_id_temp')");
  var el_first_value_middle = document.createElement('input');
  el_first_value_middle.setAttribute("id", "el_first_value_middle");
  el_first_value_middle.setAttribute("type", "text");
  el_first_value_middle.setAttribute("value", '');
  el_first_value_middle.style.cssText = "width:100px; margin-left:4px";
  el_first_value_middle.setAttribute("onKeyUp", "change_input_value(this.value,'" + num + "_element_middleform_id_temp')");
  el_first_value_first = document.getElementById('el_first_value_first');
  parent = el_first_value_first.parentNode;
  parent.insertBefore(gic1, el_first_value_first);
  parent.insertBefore(el_first_value_title, gic1);
  parent.appendChild(gic2);
  parent.appendChild(el_first_value_middle);
  refresh_attr(num, 'type_name');
}

function format_normal(num) {
  tr_name1 = document.getElementById(num + '_tr_name1');
  tr_name2 = document.getElementById(num + '_tr_name2');
  td_name_input1 = document.getElementById(num + '_td_name_input_title');
  td_name_input4 = document.getElementById(num + '_td_name_input_middle');
  td_name_label1 = document.getElementById(num + '_td_name_label_title');
  td_name_label4 = document.getElementById(num + '_td_name_label_middle');
  tr_name1.removeChild(td_name_input1);
  tr_name1.removeChild(td_name_input4);
  tr_name2.removeChild(td_name_label1);
  tr_name2.removeChild(td_name_label4);
  el_first_value_first = document.getElementById('el_first_value_first');
  parent = el_first_value_first.parentNode;
  parent.removeChild(document.getElementById('el_first_value_title').nextSibling);
  parent.removeChild(document.getElementById('el_first_value_title'));
  parent.removeChild(document.getElementById('el_first_value_middle').previousSibling);
  parent.removeChild(document.getElementById('el_first_value_middle'));
}

function type_button(i, w_title, w_func, w_class, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_button";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");
  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");
  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");
  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");
  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");
  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");
  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");
  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";
  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.setAttribute("id", "buttons");
  edit_main_td4.setAttribute("colspan", "2");
  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";
  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";
  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";
  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");
  var el_choices_add_label = document.createElement('label');
  el_choices_add_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_choices_add_label.innerHTML = "Add a new button&nbsp;";
  var el_choices_add = document.createElement('img');
  el_choices_add.setAttribute("id", "el_choices_add");
  el_choices_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_choices_add.style.cssText = 'cursor:pointer;';
  el_choices_add.setAttribute("title", 'add');
  el_choices_add.setAttribute("onClick", "add_button(" + i + ")");
  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_checkbox')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";
  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";
  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);
  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';
    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');
    el_attr_name.setAttribute("type", "text");
    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_checkbox')");
    var el_attr_value = document.createElement('input');
    el_attr_value.setAttribute("type", "text");
    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_checkbox')");
    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_checkbox')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var t = document.getElementById('edit_table');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');
  edit_main_td1.appendChild(el_style_label);
  edit_main_td1_1.appendChild(el_style_textarea);
  edit_main_td2.appendChild(el_attr_label);
  edit_main_td2.appendChild(el_attr_add);
  edit_main_td2.appendChild(br3);
  edit_main_td2.appendChild(el_attr_table);
  edit_main_td2.setAttribute("colspan", "2");
  edit_main_td3.appendChild(el_choices_add_label);
  edit_main_td3_1.appendChild(el_choices_add);
  n = w_title.length;
  for (j = 0; j < n; j++) {
    var table_button = document.createElement('table');
    table_button.setAttribute("width", "100%");
    table_button.setAttribute("border", "0");
    table_button.setAttribute("id", "button_opt" + j);
    table_button.setAttribute("idi", j + 1);
    var tr_button = document.createElement('tr');
    var tr_hr = document.createElement('tr');
    var td_button = document.createElement('td');
    var td_X = document.createElement('td');
    var td_hr = document.createElement('td');
    td_hr.setAttribute("colspan", "3");
    tr_hr.appendChild(td_hr);
    tr_button.appendChild(td_button);
    tr_button.appendChild(td_X);
    table_button.appendChild(tr_hr);
    table_button.appendChild(tr_button);
    var br1 = document.createElement('br');
    var hr = document.createElement('hr');
    hr.setAttribute("id", "br" + j);
    var el_title_label = document.createElement('label');
    el_title_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
    el_title_label.innerHTML = "Button name";
    var el_title = document.createElement('input');
    el_title.setAttribute("id", "el_title" + j);
    el_title.setAttribute("type", "text");
    el_title.setAttribute("value", w_title[j]);
    el_title.style.cssText = "width:100px;  margin-left:43px;  padding:0; border-width: 1px";
    el_title.setAttribute("onKeyUp", "change_label('" + i + "_elementform_id_temp" + j + "', this.value);");
    var el_func_label = document.createElement('label');
    el_func_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
    el_func_label.innerHTML = "OnClick function";
    var el_func = document.createElement('input');
    el_func.setAttribute("id", "el_func" + j);
    el_func.setAttribute("type", "text");
    el_func.setAttribute("value", w_func[j]);
    el_func.style.cssText = "width:100px;  margin-left:20px;  padding:0; border-width: 1px";
    el_func.setAttribute("onKeyUp", "change_func('" + i + "_elementform_id_temp" + j + "', this.value);");
    var el_choices_remove = document.createElement('img');
    el_choices_remove.setAttribute("id", "el_button" + j + "_remove");
    el_choices_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_choices_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_choices_remove.setAttribute("align", 'top');
    el_choices_remove.setAttribute("onClick", "remove_button(" + j + "," + i + ")");
    td_hr.appendChild(hr);
    td_button.appendChild(el_title_label);
    td_button.appendChild(el_title);
    td_button.appendChild(br1);
    td_button.appendChild(el_func_label);
    td_button.appendChild(el_func);
    td_X.appendChild(el_choices_remove);
    edit_main_td4.appendChild(table_button);
  }
  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr2);
  //	edit_main_table.appendChild(edit_main_tr5);
  //	edit_main_table.appendChild(edit_main_tr6);
  edit_div.appendChild(edit_main_table);
  t.appendChild(edit_div);
  // Show table.
  element = 'button';
  type = 'button';
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_button");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var div = document.createElement('div');
  div.setAttribute("id", "main_div");
  //tbody sarqac
  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');
  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'middle');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");
  td1.style.cssText = 'display:none';
  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'top');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  //	table_little -@ sarqaca tbody table_little darela table_little_t
  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = "button_" + i;
  label.style.cssText = 'display:none';
  n = w_title.length;
  for (j = 0; j < n; j++) {
    var adding = document.createElement(element);
    adding.setAttribute("type", type);
    adding.setAttribute("id", i + "_elementform_id_temp" + j);
    adding.setAttribute("name", i + "_elementform_id_temp" + j);
    adding.setAttribute("value", w_title[j]);
    adding.innerHTML = w_title[j];
    adding.setAttribute("onclick", w_func[j]);
    td2.appendChild(adding);
  }
  var main_td = document.getElementById('show_table');
  td1.appendChild(label);
  td2.appendChild(adding_type);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);
  div.appendChild(table);
  div.appendChild(br1);
  main_td.appendChild(div);
  change_class(w_class, i);
  refresh_attr(i, 'type_checkbox');
}

function type_date_fields(i, w_field_label, w_field_label_pos, w_day, w_month, w_year, w_day_type, w_month_type, w_year_type, w_day_label, w_month_label, w_year_label, w_day_size, w_month_size, w_year_size, w_required, w_class, w_from, w_to, w_divider, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_date_fields";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr8 = document.createElement('tr');
  edit_main_tr8.setAttribute("valing", "top");

  var edit_main_tr9 = document.createElement('tr');
  edit_main_tr9.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";

  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td8 = document.createElement('td');
  edit_main_td8.style.cssText = "padding-top:10px";
  var edit_main_td8_1 = document.createElement('td');
  edit_main_td8_1.style.cssText = "padding-top:10px";

  var edit_main_td9 = document.createElement('td');
  edit_main_td9.style.cssText = "padding-top:10px";
  var edit_main_td9_1 = document.createElement('td');
  edit_main_td9_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px;";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  el_label_position1.setAttribute("checked", "checked");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }
  var el_fields_divider_label = document.createElement('label');
  el_fields_divider_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_fields_divider_label.innerHTML = "Fields separator";

  var el_fields_divider = document.createElement('input');
  el_fields_divider.setAttribute("id", "edit_for_fields_divider");
  el_fields_divider.setAttribute("type", "text");
  el_fields_divider.setAttribute("value", w_divider);
  el_fields_divider.style.cssText = "margin-left:15px; width:80px";
  el_fields_divider.setAttribute("onKeyUp", "set_divider('" + i + "', this.value)");

  // Day.
  var el_day_field_type_label = document.createElement('label');
  el_day_field_type_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_day_field_type_label.innerHTML = "Day field type";

  var el_day_field_type_input1 = document.createElement('input');
  el_day_field_type_input1.setAttribute("id", "el_day_field_type_text");
  el_day_field_type_input1.setAttribute("type", "radio");
  el_day_field_type_input1.setAttribute("value", "text");
  el_day_field_type_input1.setAttribute("name", "edit_for_day_field_type");
  el_day_field_type_input1.setAttribute("onchange", "field_to_text(" + i + ", 'day')");
  Text_1 = document.createTextNode("Input");

  var el_day_field_type_input2 = document.createElement('input');
  el_day_field_type_input2.setAttribute("id", "el_day_field_type_select");
  el_day_field_type_input2.setAttribute("type", "radio");
  el_day_field_type_input2.setAttribute("value", "select");
  el_day_field_type_input2.setAttribute("name", "edit_for_day_field_type");
  el_day_field_type_input2.setAttribute("onchange", "field_to_select(" + i + ", 'day')");
  Select_1 = document.createTextNode("Select");

  if (w_day_type == "SELECT") {
    el_day_field_type_input2.setAttribute("checked", "checked");
  }
  else {
    el_day_field_type_input1.setAttribute("checked", "checked");
  }
  var el_day_field_size_label = document.createElement('label');
  el_day_field_size_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_day_field_size_label.innerHTML = "Day field size(px)";

  var el_day_field_size_input = document.createElement('input');
  el_day_field_size_input.setAttribute("id", "edit_for_day_size");
  el_day_field_size_input.setAttribute("type", "text");
  el_day_field_size_input.setAttribute("value", w_day_size);
  el_day_field_size_input.setAttribute("onKeyPress", "return check_isnum(event)");
  el_day_field_size_input.setAttribute("onKeyUp", "change_w_style('" + i + "_dayform_id_temp', this.value)");
  var el_day_field_text_label = document.createElement('label');
  el_day_field_text_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_day_field_text_label.innerHTML = "Day label";

  var el_day_field_text_input = document.createElement('input');
  el_day_field_text_input.setAttribute("id", "edit_for_day_text");
  el_day_field_text_input.setAttribute("type", "text");
  el_day_field_text_input.setAttribute("value", w_day_label);
  el_day_field_text_input.setAttribute("onKeyUp", "change_w_label('" + i + "_day_label', this.value)");

  // Month.

  var el_month_field_type_label = document.createElement('label');
  el_month_field_type_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_month_field_type_label.innerHTML = "Month field type";

  var el_month_field_type_input1 = document.createElement('input');
  el_month_field_type_input1.setAttribute("id", "el_month_field_type_text");
  el_month_field_type_input1.setAttribute("type", "radio");
  el_month_field_type_input1.setAttribute("value", "text");
  el_month_field_type_input1.setAttribute("name", "edit_for_month_field_type");
  el_month_field_type_input1.setAttribute("onchange", "field_to_text(" + i + ", 'month')");
  Text_2 = document.createTextNode("Input");

  var el_month_field_type_input2 = document.createElement('input');
  el_month_field_type_input2.setAttribute("id", "el_month_field_type_select");
  el_month_field_type_input2.setAttribute("type", "radio");
  el_month_field_type_input2.setAttribute("value", "select");
  el_month_field_type_input2.setAttribute("name", "edit_for_month_field_type");
  el_month_field_type_input2.setAttribute("onchange", "field_to_select(" + i + ", 'month')");
  Select_2 = document.createTextNode("Select");

  if (w_month_type == "SELECT") {
    el_month_field_type_input2.setAttribute("checked", "checked");
  }
  else {
    el_month_field_type_input1.setAttribute("checked", "checked");
  }
  var el_month_field_size_label = document.createElement('label');
  el_month_field_size_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_month_field_size_label.innerHTML = "Month field size(px) ";

  var el_month_field_size_input = document.createElement('input');
  el_month_field_size_input.setAttribute("id", "edit_for_month_size");
  el_month_field_size_input.setAttribute("type", "text");
  el_month_field_size_input.setAttribute("value", w_month_size);
  el_month_field_size_input.setAttribute("onKeyPress", "return check_isnum(event)");
  el_month_field_size_input.setAttribute("onKeyUp", "change_w_style('" + i + "_monthform_id_temp', this.value)");

  var el_month_field_text_label = document.createElement('label');
  el_month_field_text_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_month_field_text_label.innerHTML = "Month label";

  var el_month_field_text_input = document.createElement('input');
  el_month_field_text_input.setAttribute("id", "edit_for_month_text");
  el_month_field_text_input.setAttribute("type", "text");
  el_month_field_text_input.setAttribute("value", w_month_label);
  el_month_field_text_input.setAttribute("onKeyUp", "change_w_label('" + i + "_month_label', this.value)");

  // Year.
  var el_year_field_type_label = document.createElement('label');
  el_year_field_type_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_year_field_type_label.innerHTML = "Year field type";

  var el_year_field_type_input1 = document.createElement('input');
  el_year_field_type_input1.setAttribute("id", "el_year_field_type_text");
  el_year_field_type_input1.setAttribute("type", "radio");
  el_year_field_type_input1.setAttribute("value", "text");
  el_year_field_type_input1.setAttribute("name", "edit_for_year_field_type");
  el_year_field_type_input1.setAttribute("onchange", "field_to_text(" + i + ", 'year')");
  Text_3 = document.createTextNode("Input");

  var el_year_field_type_input2 = document.createElement('input');
  el_year_field_type_input2.setAttribute("id", "el_year_field_type_select");
  el_year_field_type_input2.setAttribute("type", "radio");
  el_year_field_type_input2.setAttribute("value", "select");
  el_year_field_type_input2.setAttribute("name", "edit_for_year_field_type");
  el_year_field_type_input2.setAttribute("onchange", "field_to_select(" + i + ", 'year')");
  Select_3 = document.createTextNode("Select");

  if (w_year_type == "SELECT") {
    el_year_field_type_input2.setAttribute("checked", "checked");
  }
  else {
    el_year_field_type_input1.setAttribute("checked", "checked");
  }
  var el_year_field_interval_label = document.createElement('label');
  el_year_field_interval_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_year_field_interval_label.innerHTML = "Year interval";

  var el_year_field_interval_from_input = document.createElement('input');
  el_year_field_interval_from_input.setAttribute("id", "edit_for_year_interval_from");
  el_year_field_interval_from_input.setAttribute("type", "text");
  el_year_field_interval_from_input.setAttribute("value", w_from);
  el_year_field_interval_from_input.style.cssText = "width:40px";
  el_year_field_interval_from_input.setAttribute("onKeyPress", "return check_isnum(event)");
  el_year_field_interval_from_input.setAttribute("onChange", "year_interval(" + i + ")");

  Line = document.createTextNode(" - ");

  var el_year_field_interval_to_input = document.createElement('input');
  el_year_field_interval_to_input.setAttribute("id", "edit_for_year_interval_to");
  el_year_field_interval_to_input.setAttribute("type", "text");
  el_year_field_interval_to_input.setAttribute("value", w_to);
  el_year_field_interval_to_input.style.cssText = "width:40px";
  el_year_field_interval_to_input.setAttribute("onKeyPress", "return check_isnum(event)");
  el_year_field_interval_to_input.setAttribute("onChange", "year_interval(" + i + ")");

  var el_year_field_size_label = document.createElement('label');
  el_year_field_size_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_year_field_size_label.innerHTML = "Year field size(px)";

  var el_year_field_size_input = document.createElement('input');
  el_year_field_size_input.setAttribute("id", "edit_for_year_size");
  el_year_field_size_input.setAttribute("type", "text");
  el_year_field_size_input.setAttribute("value", w_year_size);
  el_year_field_size_input.setAttribute("onKeyPress", "return check_isnum(event)");
  el_year_field_size_input.setAttribute("onKeyUp", "change_w_style('" + i + "_yearform_id_temp', this.value)");

  var el_year_field_text_label = document.createElement('label');
  el_year_field_text_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_year_field_text_label.innerHTML = "Year label";

  var el_year_field_text_input = document.createElement('input');
  el_year_field_text_input.setAttribute("id", "edit_for_year_text");
  el_year_field_text_input.setAttribute("type", "text");
  el_year_field_text_input.setAttribute("value", w_year_label);
  el_year_field_text_input.setAttribute("onKeyUp", "change_w_label('" + i + "_year_label', this.value)");

  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";

  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes") {
    el_required.setAttribute("checked", "checked");
  }
  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_date_fields')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_date_fields')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");

    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_date_fields')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_date_fields')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }

  var t = document.getElementById('edit_table');

  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');
  var br7 = document.createElement('br');
  var br8 = document.createElement('br');
  var br9 = document.createElement('br');
  var br10 = document.createElement('br');
  var br11 = document.createElement('br');
  var br12 = document.createElement('br');
  var br13 = document.createElement('br');
  var br14 = document.createElement('br');
  var br15 = document.createElement('br');
  var br16 = document.createElement('br');
  var br17 = document.createElement('br');
  var br18 = document.createElement('br');

  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);

  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);

  edit_main_td3.appendChild(el_fields_divider_label);
  edit_main_td3_1.appendChild(el_fields_divider);

  edit_main_td4.appendChild(el_day_field_type_label);
  // edit_main_td4.appendChild(br);
  edit_main_td4.appendChild(el_day_field_size_label);
  // edit_main_td4.appendChild(br1);
  edit_main_td4.appendChild(el_day_field_text_label);

  edit_main_td4_1.appendChild(el_day_field_type_input1);
  edit_main_td4_1.appendChild(Text_1);
  edit_main_td4_1.appendChild(el_day_field_type_input2);
  edit_main_td4_1.appendChild(Select_1);
  edit_main_td4_1.appendChild(br4);
  edit_main_td4_1.appendChild(el_day_field_size_input);
  edit_main_td4_1.appendChild(br5);
  edit_main_td4_1.appendChild(el_day_field_text_input);

  edit_main_td8.appendChild(el_month_field_type_label);
  // edit_main_td8.appendChild(br11);
  edit_main_td8.appendChild(el_month_field_size_label);
  // edit_main_td8.appendChild(br12);
  edit_main_td8.appendChild(el_month_field_text_label);
  edit_main_td8_1.appendChild(el_month_field_type_input1);
  edit_main_td8_1.appendChild(Text_2);
  edit_main_td8_1.appendChild(el_month_field_type_input2);
  edit_main_td8_1.appendChild(Select_2);
  edit_main_td8_1.appendChild(br6);
  edit_main_td8_1.appendChild(el_month_field_size_input);
  edit_main_td8_1.appendChild(br7);
  edit_main_td8_1.appendChild(el_month_field_text_input);

  edit_main_td9.appendChild(el_year_field_type_label);
  // edit_main_td9.appendChild(br13);
  edit_main_td9.appendChild(el_year_field_interval_label);
  // edit_main_td9.appendChild(br14);
  edit_main_td9.appendChild(el_year_field_size_label);
  // edit_main_td9.appendChild(br15);
  edit_main_td9.appendChild(el_year_field_text_label);
  edit_main_td9_1.appendChild(el_year_field_type_input1);
  edit_main_td9_1.appendChild(Text_3);
  edit_main_td9_1.appendChild(el_year_field_type_input2);
  edit_main_td9_1.appendChild(Select_3);
  edit_main_td9_1.appendChild(br8);
  edit_main_td9_1.appendChild(el_year_field_interval_from_input);
  edit_main_td9_1.appendChild(Line);
  edit_main_td9_1.appendChild(el_year_field_interval_to_input);
  edit_main_td9_1.appendChild(br9);
  edit_main_td9_1.appendChild(el_year_field_size_input);
  edit_main_td9_1.appendChild(br10);
  edit_main_td9_1.appendChild(el_year_field_text_input);

  edit_main_td5.appendChild(el_style_label);
  edit_main_td5_1.appendChild(el_style_textarea);

  edit_main_td6.appendChild(el_required_label);
  edit_main_td6_1.appendChild(el_required);

  edit_main_td7.appendChild(el_attr_label);
  edit_main_td7.appendChild(el_attr_add);
  edit_main_td7.appendChild(br3);
  edit_main_td7.appendChild(el_attr_table);
  edit_main_td7.setAttribute("colspan", "2");

  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr8.appendChild(edit_main_td8);
  edit_main_tr8.appendChild(edit_main_td8_1);
  edit_main_tr9.appendChild(edit_main_td9);
  edit_main_tr9.appendChild(edit_main_td9_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_tr7.appendChild(edit_main_td7_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr8);
  edit_main_table.appendChild(edit_main_tr9);
  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr6);
  edit_main_table.appendChild(edit_main_tr7);
  edit_div.appendChild(edit_main_table);

  t.appendChild(edit_div);
  add_id_and_name(i, 'type_date_fields');
  // Show table.
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_date_fields");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");
  adding_required.setAttribute("id", i + "_requiredform_id_temp");

  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');

  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'top');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'middle');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var table_date = document.createElement('table');
  table_date.setAttribute("id", i + "_table_date");
  table_date.setAttribute("cellpadding", '0');
  table_date.setAttribute("cellspacing", '0');

  var tr_date1 = document.createElement('tr');
  tr_date1.setAttribute("id", i + "_tr_date1");

  var tr_date2 = document.createElement('tr');
  tr_date2.setAttribute("id", i + "_tr_date2");

  var td_date_input1 = document.createElement('td');
  td_date_input1.setAttribute("id", i + "_td_date_input1");

  var td_date_separator1 = document.createElement('td');
  td_date_separator1.setAttribute("id", i + "_td_date_separator1");

  var td_date_input2 = document.createElement('td');
  td_date_input2.setAttribute("id", i + "_td_date_input2");

  var td_date_separator2 = document.createElement('td');
  td_date_separator2.setAttribute("id", i + "_td_date_separator2");

  var td_date_input3 = document.createElement('td');
  td_date_input3.setAttribute("id", i + "_td_date_input3");

  var td_date_label1 = document.createElement('td');
  td_date_label1.setAttribute("id", i + "_td_date_label1");
  var td_date_label_empty1 = document.createElement('td');

  var td_date_label2 = document.createElement('td');
  td_date_label2.setAttribute("id", i + "_td_date_label2");
  var td_date_label_empty2 = document.createElement('td');

  var td_date_label3 = document.createElement('td');
  td_date_label3.setAttribute("id", i + "_td_date_label3");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");
  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes") {
    required.innerHTML = " *";
  }
  var day = document.createElement('input');
  day.setAttribute("type", 'text');
  day.setAttribute("value", w_day);
  day.setAttribute("id", i + "_dayform_id_temp");
  day.setAttribute("name", i + "_dayform_id_temp");
  day.setAttribute("onChange", "change_value('" + i + "_dayform_id_temp')");
  day.setAttribute("onKeyPress", "return check_day(event, '" + i + "_dayform_id_temp')");
  day.setAttribute("onBlur", "if (this.value=='0') this.value=''; else add_0('" + i + "_dayform_id_temp')");
  day.style.width = w_day_size + 'px';
  var day_label = document.createElement('label');
  day_label.setAttribute("class", "mini_label");
  day_label.setAttribute("id", i + "_day_label");
  day_label.innerHTML = w_day_label;

  var day_ = document.createElement('span');
  day_.setAttribute("id", i + "_separator1");
  day_.setAttribute("class", "wdform_separator");
  day_.innerHTML = w_divider;

  var month = document.createElement('input');
  month.setAttribute("type", 'text');
  month.setAttribute("value", w_month);
  month.setAttribute("id", i + "_monthform_id_temp");
  month.setAttribute("name", i + "_monthform_id_temp");
  month.style.width = w_month_size + 'px';
  month.setAttribute("onKeyPress", "return check_month(event, '" + i + "_monthform_id_temp')");
  month.setAttribute("onChange", "change_value('" + i + "_monthform_id_temp')");
  month.setAttribute("onBlur", "if (this.value=='0') this.value=''; else add_0('" + i + "_monthform_id_temp')");
  var month_label = document.createElement('label');
  month_label.setAttribute("class", "mini_label");
  month_label.setAttribute("class", "mini_label");
  month_label.setAttribute("id", i + "_month_label");
  month_label.innerHTML = w_month_label;

  var month_ = document.createElement('span');
  month_.setAttribute("id", i + "_separator2");
  month_.setAttribute("class", "wdform_separator");
  month_.innerHTML = w_divider;

  var year = document.createElement('input');
  year.setAttribute("type", 'text');
  year.setAttribute("from", w_from);
  year.setAttribute("to", w_to);
  year.setAttribute("value", w_year);
  year.setAttribute("id", i + "_yearform_id_temp");
  year.setAttribute("name", i + "_yearform_id_temp");
  year.style.width = w_year_size + 'px';
  year.setAttribute("onChange", "change_year('" + i + "_yearform_id_temp')");
  year.setAttribute("onKeyPress", "return check_year1(event, '" + i + "_yearform_id_temp')");
  year.setAttribute("onBlur", "check_year2('" + i + "_yearform_id_temp')");
  var year_label = document.createElement('label');
  year_label.setAttribute("class", "mini_label");
  year_label.setAttribute("id", i + "_year_label");
  year_label.innerHTML = w_year_label;
  var main_td = document.getElementById('show_table');
  td1.appendChild(label);
  td1.appendChild(required);
  td_date_input1.appendChild(day);
  td_date_separator1.appendChild(day_);
  td_date_input2.appendChild(month);
  td_date_separator2.appendChild(month_);
  td_date_input3.appendChild(year);
  tr_date1.appendChild(td_date_input1);
  tr_date1.appendChild(td_date_separator1);
  tr_date1.appendChild(td_date_input2);
  tr_date1.appendChild(td_date_separator2);
  tr_date1.appendChild(td_date_input3);
  td_date_label1.appendChild(day_label);
  td_date_label2.appendChild(month_label);
  td_date_label3.appendChild(year_label);
  tr_date2.appendChild(td_date_label1);
  tr_date2.appendChild(td_date_label_empty1);
  tr_date2.appendChild(td_date_label2);
  tr_date2.appendChild(td_date_label_empty2);
  tr_date2.appendChild(td_date_label3);
  table_date.appendChild(tr_date1);
  table_date.appendChild(tr_date2);
  td2.appendChild(adding_type);
  td2.appendChild(adding_required);
  td2.appendChild(table_date);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);
  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  if (w_field_label_pos == "top") {
    label_top(i);
  }
  if (w_day_type == "SELECT") {
    field_to_select(i, 'day');
  }
  if (w_month_type == "SELECT") {
    field_to_select(i, 'month');
  }
  if (w_year_type == "SELECT") {
    field_to_select(i, 'year');
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_date_fields');
}

function type_own_select(i, w_field_label, w_field_label_pos, w_size, w_choices, w_choices_checked, w_required, w_class, w_attr_name, w_attr_value, w_choices_disabled) {
  document.getElementById("element_type").value = "type_own_select";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");
  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px; vertical-align:top";
  edit_main_td3.setAttribute("id", "choices");

  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";
  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px;";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  el_label_position1.setAttribute("checked", "checked");
  Left = document.createTextNode("Left");
  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");
  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");
  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }
  var el_size_label = document.createElement('label');
  el_size_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_size_label.innerHTML = "Field size(px) ";
  var el_size = document.createElement('input');
  el_size.setAttribute("id", "edit_for_input_size");
  el_size.setAttribute("type", "text");
  el_size.setAttribute("value", w_size);

  el_size.setAttribute("name", "edit_for_size");
  el_size.setAttribute("onKeyPress", "return check_isnum(event)");
  el_size.setAttribute("onKeyUp", "change_w_style('" + i + "_elementform_id_temp', this.value)");

  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";

  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes") {
    el_required.setAttribute("checked", "checked");
  }
  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_text')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_text')");

    var el_attr_value = document.createElement('input');
    el_attr_value.setAttribute("type", "text");
    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_text')");
    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_text')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var el_choices_label = document.createElement('label');
  el_choices_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_choices_label.innerHTML = "Options";
  var el_choices_add = document.createElement('img');
  el_choices_add.setAttribute("id", "el_choices_add");
  el_choices_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_choices_add.style.cssText = 'cursor:pointer;';
  el_choices_add.setAttribute("title", 'add');
  el_choices_add.setAttribute("onClick", "add_choise('select'," + i + ")");
  var t = document.getElementById('edit_table');

  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  br3.setAttribute("id", "br1");
  var br4 = document.createElement('br');
  br4.setAttribute("id", "br2");
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');

  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);

  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);

  edit_main_td6.appendChild(el_style_label);
  edit_main_td6_1.appendChild(el_style_textarea);

  edit_main_td7.appendChild(el_attr_label);
  edit_main_td7.appendChild(el_attr_add);
  edit_main_td7.appendChild(br3);
  edit_main_td7.appendChild(el_attr_table);
  edit_main_td7.setAttribute("colspan", "2");
  edit_main_td4.appendChild(el_required_label);
  edit_main_td4_1.appendChild(el_required);

  edit_main_td5.appendChild(el_size_label);
  edit_main_td5_1.appendChild(el_size);

  edit_main_td3.appendChild(el_choices_label);
  edit_main_td3_1.appendChild(el_choices_add);

  n = w_choices.length;
  for (j = 0; j < n; j++) {
    var br = document.createElement('br');
    br.setAttribute("id", "br" + j);
    var el_choices = document.createElement('input');
    el_choices.setAttribute("id", "el_option" + j);
    el_choices.setAttribute("type", "text");
    el_choices.setAttribute("value", w_choices[j]);
    el_choices.style.cssText = "width:100px; margin:0; padding:0; border-width: 1px";
    el_choices.setAttribute("onKeyUp", "change_label('" + i + "_option" + j + "', this.value)");

    var el_choices_remove = document.createElement('img');
    el_choices_remove.setAttribute("id", "el_option" + j + "_remove");
    el_choices_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_choices_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_choices_remove.setAttribute("align", 'top');
    el_choices_remove.setAttribute("onClick", "remove_option(" + j + "," + i + ")");

    var el_choices_dis = document.createElement('input');
    el_choices_dis.setAttribute("type", 'checkbox');
    el_choices_dis.setAttribute("title", 'Empty value');
    el_choices_dis.setAttribute("id", "el_option" + j + "_dis");
    el_choices_dis.setAttribute("onClick", "dis_option('" + i + "_option" + j + "', this.checked)");
    el_choices_dis.style.cssText = "vertical-align: middle";
    if (w_choices_disabled[j]) {
      el_choices_dis.setAttribute("checked", "checked");
    }
    edit_main_td3.appendChild(br);
    edit_main_td3.appendChild(el_choices);
    edit_main_td3.appendChild(el_choices_dis);
    edit_main_td3.appendChild(el_choices_remove);
  }
  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);

  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_tr7.appendChild(edit_main_td7_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr6);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr7);
  edit_div.appendChild(edit_main_table);
  t.appendChild(edit_div);
  add_id_and_name(i, 'type_text');
  // Show table.
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_own_select");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");
  adding_required.setAttribute("id", i + "_requiredform_id_temp");
  var div = document.createElement('div');
  div.setAttribute("id", "main_div");
  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');
  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'middle');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");
  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'middle');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var table_little = document.createElement('table');
  table_little.setAttribute("style", "border:none;");
  table_little.setAttribute("id", i + "_table_little");
  var tr_little1 = document.createElement('tr');
  tr_little1.setAttribute("id", i + "_element_tr1");
  var tr_little2 = document.createElement('tr');
  tr_little2.setAttribute("id", i + "_element_tr2");
  var td_little1 = document.createElement('td');
  td_little1.setAttribute("valign", 'top');
  td_little1.setAttribute("id", i + "_td_little1");
  var td_little2 = document.createElement('td');
  td_little2.setAttribute("valign", 'top');
  td_little2.setAttribute("id", i + "_td_little2");
  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");
  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes") {
    required.innerHTML = " *";
  }
  var select_ = document.createElement('select');
  select_.setAttribute("id", i + "_elementform_id_temp");
  select_.setAttribute("name", i + "_elementform_id_temp");
  select_.setAttribute("class", "form-select");
  select_.style.cssText = "width:" + w_size + "px";
  select_.setAttribute("onchange", "set_select(this)");
  select_.setAttribute("class", "form-select");
  for (j = 0; j < n; j++) {
    var option = document.createElement('option');
    option.setAttribute("id", i + "_option" + j);
    if (w_choices_disabled[j]) {
      option.value = "";
    }
    else {
      option.setAttribute("value", w_choices[j]);
    }
    option.setAttribute("onselect", "set_select('" + i + "_option" + j + "')");
    option.innerHTML = w_choices[j];
    if (w_choices_checked[j] == 1)  {
      option.setAttribute("selected", "selected");
    }
    select_.appendChild(option);
  }

  var main_td = document.getElementById('show_table');
  td1.appendChild(label);
  td1.appendChild(required);
  td2.appendChild(adding_type);
  td2.appendChild(adding_required);
  td2.appendChild(select_);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);
  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  if (w_field_label_pos == "top") {
    label_top(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_text');
}

function dis_option(id, value) {
  if (value) {
    document.getElementById(id).value = '';
  }
  else {
    document.getElementById(id).value = document.getElementById(id).innerHTML;
  }
}

function type_country(i, w_field_label, w_countries, w_field_label_pos, w_size, w_required, w_class, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_country";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");
  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");
  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");
  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");
  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");
  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");
  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");
  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");
  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");
  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";
  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";
  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";
  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";
  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";
  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_label.innerHTML = "Field label";
  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px;";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;
  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";
  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");
  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  el_label_position1.setAttribute("checked", "checked");
  Left = document.createTextNode("Left");
  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");
  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");
  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }
  var el_size_label = document.createElement('label');
  el_size_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_size_label.innerHTML = "Field size(px) ";
  var el_size = document.createElement('input');
  el_size.setAttribute("id", "edit_for_input_size");
  el_size.setAttribute("type", "text");
  el_size.setAttribute("value", w_size);
  el_size.setAttribute("name", "edit_for_size");
  el_size.setAttribute("onKeyPress", "return check_isnum(event)");
  el_size.setAttribute("onKeyUp", "change_w_style('" + i + "_elementform_id_temp', this.value)");
  var el_edit_list = document.createElement('a');
  el_edit_list.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px; font-style:italic; cursor:pointer";
  el_edit_list.innerHTML = "Edit country list";
  // el_edit_list.setAttribute("rel", "{handler: 'iframe', size: {x: 650, y: 375}}");
  // el_edit_list.setAttribute("href", "index.php?option=com_formmaker&task=country_list&field_id=" + i + "&tmpl=component");
  el_edit_list.setAttribute("href", "javascript:form_maker_createpopup('" + Drupal.settings.form_maker.country_popup_url + "&field_id=" + i + "', navigator.userAgent.indexOf('Opera') > -1?580:jQuery(window).height() - 70, 1, 'testpopup', 5);");
  // el_edit_list.setAttribute("class", "modal");
  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";
  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");
  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";
  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes") {
    el_required.setAttribute("checked", "checked");
  }
  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_text')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";
  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";
  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);
  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';
    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');
    el_attr_name.setAttribute("type", "text");
    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_text')");
    var el_attr_value = document.createElement('input');
    el_attr_value.setAttribute("type", "text");
    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_text')");
    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_text')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var t = document.getElementById('edit_table');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  br3.setAttribute("id", "br1");
  var br4 = document.createElement('br');
  br4.setAttribute("id", "br2");
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');
  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);
  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);
  edit_main_td3.appendChild(el_size_label);
  edit_main_td3_1.appendChild(el_size);
  edit_main_td4.appendChild(el_style_label);
  edit_main_td4_1.appendChild(el_style_textarea);
  edit_main_td5.appendChild(el_required_label);
  edit_main_td5_1.appendChild(el_required);
  edit_main_td6.appendChild(el_attr_label);
  edit_main_td6.appendChild(el_attr_add);
  edit_main_td6.appendChild(br3);
  edit_main_td6.appendChild(el_attr_table);
  edit_main_td6.setAttribute("colspan", "2");
  edit_main_td7.appendChild(el_edit_list);
  edit_main_td7.setAttribute("colspan", "2");
  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr7);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr6);
  edit_div.appendChild(edit_main_table);
  t.appendChild(edit_div);
  add_id_and_name(i, 'type_text');
  // Show table.
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_country");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");
  adding_required.setAttribute("id", i + "_requiredform_id_temp");
  var div = document.createElement('div');
  div.setAttribute("id", "main_div");
  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');
  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'middle');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");
  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'middle');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var table_little = document.createElement('table');
  table_little.setAttribute("style", "border:none;");
  table_little.setAttribute("id", i + "_table_little");
  var tr_little1 = document.createElement('tr');
  tr_little1.setAttribute("id", i + "_element_tr1");
  var tr_little2 = document.createElement('tr');
  tr_little2.setAttribute("id", i + "_element_tr2");
  var td_little1 = document.createElement('td');
  td_little1.setAttribute("valign", 'top');
  td_little1.setAttribute("id", i + "_td_little1");
  var td_little2 = document.createElement('td');
  td_little2.setAttribute("valign", 'top');
  td_little2.setAttribute("id", i + "_td_little2");
  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");
  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes") {
    required.innerHTML = " *";
  }
  var select_ = document.createElement('select');
  select_.setAttribute("id", i + "_elementform_id_temp");
  select_.setAttribute("name", i + "_elementform_id_temp");
  select_.setAttribute("class", "form-select");
  select_.style.cssText = "width:" + w_size + "px";
  for (r = 0; r < w_countries.length; r++) {
    var option_ = document.createElement('option');
    option_.setAttribute("value", w_countries[r]);
    option_.innerHTML = w_countries[r];
    select_.appendChild(option_);
  }
  var main_td = document.getElementById('show_table');
  td1.appendChild(label);
  td1.appendChild(required);
  td2.appendChild(adding_type);
  td2.appendChild(adding_required);
  td2.appendChild(select_);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);
  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  if (w_field_label_pos == "top") {
    label_top(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_text');
  // enable_modals();
}

function type_file_upload(i, w_field_label, w_field_label_pos, w_destination, w_extension, w_max_size, w_required, w_class, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_file_upload";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");
  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");
  var edit_main_tr8 = document.createElement('tr');
  edit_main_tr8.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";
  var edit_main_td8 = document.createElement('td');
  edit_main_td8.style.cssText = "padding-top:10px";
  var edit_main_td8_1 = document.createElement('td');
  edit_main_td8_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }
  var el_max_size_label = document.createElement('label');
  el_max_size_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_max_size_label.innerHTML = "Maximum size(KB) ";

  var el_max_size_input = document.createElement('input');

  el_max_size_input.setAttribute("type", "text");

  el_max_size_input.setAttribute("id", "edit_for_max_size");

  el_max_size_input.setAttribute("value", w_max_size);
  el_max_size_input.style.cssText = "width:100px;";

  el_max_size_input.setAttribute("onKeyPress", "return check_isnum(event)");

  el_max_size_input.setAttribute("onChange", "change_file_value(this.value,'" + i + "_max_size', '***max_sizeskizb" + i + "***', '***max_sizeverj" + i + "***')");

  var el_destination_label = document.createElement('label');
  el_destination_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_destination_label.innerHTML = "Destination";

  var el_destination_input = document.createElement('input');
  el_destination_input.setAttribute("id", "el_destination_input");
  el_destination_input.setAttribute("type", "text");
  el_destination_input.setAttribute("value", w_destination);
  el_destination_input.style.cssText = "width:200px;";
  el_destination_input.setAttribute("onChange", "change_file_value(this.value,'" + i + "_destination', '***destinationskizb" + i + "***', '***destinationverj" + i + "***')");

  var el_extension_label = document.createElement('label');
  el_extension_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_extension_label.innerHTML = "Allowed file extensions&nbsp;";

  var el_extension_textarea = document.createElement('textarea');
  el_extension_textarea.setAttribute("id", "edit_for_extension");
  el_extension_textarea.setAttribute("rows", "4");
  el_extension_textarea.style.cssText = "width:200px;";

  el_extension_textarea.setAttribute("onChange", "change_file_value(this.value,'" + i + "_extension', '***extensionskizb" + i + "***', '***extensionverj" + i + "***')");

  el_extension_textarea.innerHTML = w_extension;

  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_required_label = document.createElement('label');
  el_required_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_required_label.innerHTML = "Required";

  var el_required = document.createElement('input');
  el_required.setAttribute("id", "el_send");
  el_required.setAttribute("type", "checkbox");
  el_required.setAttribute("value", "yes");
  el_required.setAttribute("onclick", "set_required('" + i + "_required')");
  if (w_required == "yes") {
    el_required.setAttribute("checked", "checked");
  }
  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_text')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);
  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';
    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');
    el_attr_name.setAttribute("type", "text");
    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_text')");
    var el_attr_value = document.createElement('input');
    el_attr_value.setAttribute("type", "text");
    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_text')");
    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_text')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var t = document.getElementById('edit_table');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');
  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);
  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);
  edit_main_td3.appendChild(el_max_size_label);
  edit_main_td3_1.appendChild(el_max_size_input);
  edit_main_td4.appendChild(el_destination_label);
  edit_main_td4_1.appendChild(el_destination_input);
  edit_main_td5.appendChild(el_extension_label);
  edit_main_td5_1.appendChild(el_extension_textarea);
  edit_main_td6.appendChild(el_style_label);
  edit_main_td6_1.appendChild(el_style_textarea);
  edit_main_td7.appendChild(el_required_label);
  edit_main_td7_1.appendChild(el_required);
  edit_main_td8.appendChild(el_attr_label);
  edit_main_td8.appendChild(el_attr_add);
  edit_main_td8.appendChild(br3);
  edit_main_td8.appendChild(el_attr_table);
  edit_main_td8.setAttribute("colspan", "2");
  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_tr7.appendChild(edit_main_td7_1);
  edit_main_tr8.appendChild(edit_main_td8);
  edit_main_tr8.appendChild(edit_main_td8_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr6);
  edit_main_table.appendChild(edit_main_tr7);
  edit_main_table.appendChild(edit_main_tr8);
  edit_div.appendChild(edit_main_table);
  t.appendChild(edit_div);
  add_id_and_name(i, 'type_text');
  // Show table.
  element = 'input';
  type = 'file';
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_file_upload");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var adding_required = document.createElement("input");
  adding_required.setAttribute("type", "hidden");
  adding_required.setAttribute("value", w_required);
  adding_required.setAttribute("name", i + "_requiredform_id_temp");
  adding_required.setAttribute("id", i + "_requiredform_id_temp");
  var adding = document.createElement(element);
  adding.setAttribute("type", type);
  adding.setAttribute("class", "file_upload");
  adding.setAttribute("id", i + "_elementform_id_temp");
  adding.setAttribute("name", i + "_fileform_id_temp");
  var adding_max_size = document.createElement("input");
  adding_max_size.setAttribute("type", "hidden");
  adding_max_size.setAttribute("value", '***max_sizeskizb' + i + '***' + w_max_size + '***max_sizeverj' + i + '***');
  adding_max_size.setAttribute("id", i + "_max_size");
  adding_max_size.setAttribute("name", i + "_max_size");
  var adding_destination = document.createElement("input");
  adding_destination.setAttribute("type", "hidden");
  adding_destination.setAttribute("value", '***destinationskizb' + i + '***' + w_destination + '***destinationverj' + i + '***');
  adding_destination.setAttribute("id", i + "_destination");
  adding_destination.setAttribute("name", i + "_destination");
  var adding_extension = document.createElement("input");
  adding_extension.setAttribute("type", "hidden");
  adding_extension.setAttribute("value", '***extensionskizb' + i + '***' + w_extension + '***extensionverj' + i + '***');
  adding_extension.setAttribute("id", i + "_extension");
  adding_extension.setAttribute("name", i + "_extension");
  var div = document.createElement('div');
  div.setAttribute("id", "main_div");
  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');
  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'middle');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");
  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'top');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");
  var required = document.createElement('span');
  required.setAttribute("id", i + "_required_elementform_id_temp");
  required.innerHTML = "";
  required.setAttribute("class", "required");
  if (w_required == "yes") {
    required.innerHTML = " *";
  }
  var main_td = document.getElementById('show_table');
  td1.appendChild(label);
  td1.appendChild(required);
  td2.appendChild(adding_type);
  td2.appendChild(adding_required);
  td2.appendChild(adding_max_size);
  td2.appendChild(adding_destination);
  td2.appendChild(adding_extension);
  td2.appendChild(adding);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);
  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  if (w_field_label_pos == "top") {
    label_top(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_text');
}

function type_recaptcha(i, w_field_label, w_field_label_pos, w_public, w_private, w_theme, w_class, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_recaptcha";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_tr8 = document.createElement('tr');
  edit_main_tr8.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var edit_main_td8 = document.createElement('td');
  edit_main_td8.style.cssText = "padding-top:10px";
  var edit_main_td8_1 = document.createElement('td');
  edit_main_td8_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top")
    el_label_position2.setAttribute("checked", "checked");
  else
    el_label_position1.setAttribute("checked", "checked");

  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_public_label = document.createElement('label');
  el_public_label.style.cssText = "color:#BA0D0D; font-weight:bold; font-size: 13px;text-decoration:underline";
  el_public_label.innerHTML = "Public key";

  var el_public_textarea = document.createElement('input');
  el_public_textarea.setAttribute("id", "public_key");
  el_public_textarea.setAttribute("type", "text");
  el_public_textarea.setAttribute("value", w_public);
  el_public_textarea.style.cssText = "width:200px;";
  el_public_textarea.setAttribute("onChange", "change_key(this.value, 'public_key')");

  var el_private_label = document.createElement('label');
  el_private_label.style.cssText = "color:#BA0D0D; font-weight:bold; font-size: 13px; text-decoration:underline";
  el_private_label.innerHTML = "Private key";

  var el_private_textarea = document.createElement('input');
  el_private_textarea.setAttribute("id", "private_key");
  el_private_textarea.setAttribute("type", "text");
  el_private_textarea.setAttribute("value", w_private);
  el_private_textarea.style.cssText = "width:200px;";
  el_private_textarea.setAttribute("onChange", "change_key(this.value, 'private_key')");

  var el_theme_label = document.createElement('label');
  el_theme_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_theme_label.innerHTML = "Recaptcha Theme";

  var el_theme_select = document.createElement('select');
  el_theme_select.style.cssText = "width:100px;";
  el_theme_select.setAttribute("onChange", "change_key(this.value, 'theme')");
  el_theme_select.setAttribute("class", "form-select");

  var el_theme_option1 = document.createElement('option');
  el_theme_option1.value = "red";
  el_theme_option1.innerHTML = "Red";

  var el_theme_option2 = document.createElement('option');
  el_theme_option2.value = "white";
  el_theme_option2.innerHTML = "White";

  var el_theme_option3 = document.createElement('option');
  el_theme_option3.value = "blackglass";
  el_theme_option3.innerHTML = "Blackglass";

  var el_theme_option4 = document.createElement('option');
  el_theme_option4.value = "clean";
  el_theme_option4.innerHTML = "Clean";

  el_theme_select.appendChild(el_theme_option1);
  el_theme_select.appendChild(el_theme_option2);
  el_theme_select.appendChild(el_theme_option3);
  el_theme_select.appendChild(el_theme_option4);

  el_theme_select.value = w_theme;

  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_recaptcha')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_recaptcha')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");

    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_recaptcha')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_recaptcha')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var t = document.getElementById('edit_table');

  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');

  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);

  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);

  edit_main_td4.appendChild(el_style_label);
  edit_main_td4_1.appendChild(el_style_textarea);

  edit_main_td6.appendChild(el_public_label);
  edit_main_td6_1.appendChild(el_public_textarea);

  edit_main_td7.appendChild(el_private_label);
  edit_main_td7_1.appendChild(el_private_textarea);

  edit_main_td8.appendChild(el_theme_label);
  edit_main_td8_1.appendChild(el_theme_select);

  edit_main_td5.appendChild(el_attr_label);
  edit_main_td5.appendChild(el_attr_add);
  edit_main_td5.appendChild(br3);
  edit_main_td5.appendChild(el_attr_table);
  edit_main_td5.setAttribute("colspan", "2");
  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_tr7.appendChild(edit_main_td7_1);
  edit_main_tr8.appendChild(edit_main_td8);
  edit_main_tr8.appendChild(edit_main_td8_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr6);
  edit_main_table.appendChild(edit_main_tr7);
  edit_main_table.appendChild(edit_main_tr8);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr5);
  edit_div.appendChild(edit_main_table);
  t.appendChild(edit_div);
  add_id_and_name(i, 'type_text');
  // Show table.
  element = 'img';
  type = 'captcha';
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_recaptcha");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var adding = document.createElement('div');
  adding.setAttribute("id", "wd_recaptchaform_id_temp");
  adding.setAttribute("public_key", w_public);
  adding.setAttribute("private_key", w_private);
  adding.setAttribute("theme", w_theme);
  var adding_text = document.createElement('span');
  adding_text.style.color = "red";
  adding_text.style.fontStyle = "italic";
  adding_text.innerHTML = "Recaptcha don't display in back end";
  adding.appendChild(adding_text);
  var div = document.createElement('div');
  div.setAttribute("id", "main_div");
  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');
  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'top');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");
  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'top');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");
  var main_td = document.getElementById('show_table');
  td1.appendChild(label);
  td2.appendChild(adding_type);
  td2.appendChild(adding);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);
  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  if (w_field_label_pos == "top") {
    label_top(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_recaptcha');
}

function type_captcha(i, w_field_label, w_field_label_pos, w_digit, w_class, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_captcha";
  delete_last_child();
  // Edit table.
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px; width:110px;";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");

  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top")
    el_label_position2.setAttribute("checked", "checked");
  else
    el_label_position1.setAttribute("checked", "checked");

  var el_size_label = document.createElement('label');
  el_size_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_size_label.innerHTML = "Captcha size";

  var el_captcha_digit = document.createElement('input');
  el_captcha_digit.setAttribute("id", "captcha_digit");
  el_captcha_digit.setAttribute("type", "text");
  el_captcha_digit.setAttribute("value", w_digit);
  el_captcha_digit.setAttribute("name", "captcha_digit");
  el_captcha_digit.setAttribute("onKeyPress", "return check_isnum_3_10(event)");
  el_captcha_digit.setAttribute("onKeyUp", "change_captcha_digit(this.value, '" + i + "')");
  el_captcha_digit.style.cssText = "margin-right:18px";

  Digits = document.createTextNode("Digits (3 - 9)");

  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";

  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_captcha')");

  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';

  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');

  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';

  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';

  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';

  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_captcha')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");

    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_captcha')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_captcha')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var t = document.getElementById('edit_table');

  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');

  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);

  edit_main_td2.appendChild(el_label_position_label);
  edit_main_td2_1.appendChild(el_label_position1);
  edit_main_td2_1.appendChild(Left);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_label_position2);
  edit_main_td2_1.appendChild(Top);

  edit_main_td3.appendChild(el_size_label);
  edit_main_td3_1.appendChild(el_captcha_digit);
  edit_main_td3_1.appendChild(Digits);

  edit_main_td4.appendChild(el_style_label);
  edit_main_td4_1.appendChild(el_style_textarea);

  edit_main_td5.appendChild(el_attr_label);
  edit_main_td5.appendChild(el_attr_add);
  edit_main_td5.appendChild(br3);
  edit_main_td5.appendChild(el_attr_table);
  edit_main_td5.setAttribute("colspan", "2");
  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr5);
  edit_div.appendChild(edit_main_table);

  t.appendChild(edit_div);
  add_id_and_name(i, 'type_captcha');

  // Show table.
  element = 'img';
  type = 'captcha';
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_captcha");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");
  var adding = document.createElement(element);
  adding.setAttribute("type", type);
  adding.setAttribute("digit", w_digit);
  adding.setAttribute("src", "" + Drupal.settings.form_maker.captcha_url + w_digit + "&i=form_id_temp");
  adding.setAttribute("id", "_wd_captchaform_id_temp");
  adding.setAttribute("class", "captcha_img");
  adding.setAttribute("onClick", "captcha_refresh('_wd_captcha','form_id_temp')");
  var refresh_captcha = document.createElement("div");
  refresh_captcha.setAttribute("class", "captcha_refresh");
  refresh_captcha.setAttribute("id", "_element_refreshform_id_temp");
  refresh_captcha.setAttribute("onClick", "captcha_refresh('_wd_captcha','form_id_temp')");

  var input_captcha = document.createElement("input");
  input_captcha.setAttribute("type", "text");
  input_captcha.style.cssText = "width:" + (w_digit * 10 + 15) + "px;";
  input_captcha.setAttribute("class", "captcha_input");
  input_captcha.setAttribute("id", "_wd_captcha_inputform_id_temp");
  input_captcha.setAttribute("name", "captcha_input");

  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');

  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'top');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'top');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var captcha_table = document.createElement('table');

  var captcha_tr1 = document.createElement('tr');
  var captcha_tr2 = document.createElement('tr');

  var captcha_td1 = document.createElement('td');
  captcha_td1.setAttribute("valign", 'middle');

  var captcha_td2 = document.createElement('td');
  captcha_td2.setAttribute("valign", 'middle');
  var captcha_td3 = document.createElement('td');

  captcha_table.appendChild(captcha_tr1);
  captcha_table.appendChild(captcha_tr2);
  captcha_tr1.appendChild(captcha_td1);
  captcha_tr1.appendChild(captcha_td2);
  captcha_tr2.appendChild(captcha_td3);

  captcha_td1.appendChild(adding);
  captcha_td2.appendChild(refresh_captcha);
  captcha_td3.appendChild(input_captcha);


  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");
  var main_td = document.getElementById('show_table');
  td1.appendChild(label);
  td2.appendChild(adding_type);
  td2.appendChild(captcha_table);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);
  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  if (w_field_label_pos == "top") {
    label_top(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_captcha');
}

function pagebreak_title_change(val) {
  document.getElementById("_div_between").setAttribute('page_title', val);
  document.getElementById("div_page_title").innerHTML = val + '<br/><br/>';
}

function form_maker_el_section_break() {
  if (document.getElementById("editing_id").value) {
    new_id = document.getElementById("editing_id").value;
  }
  else {
    new_id = gen;
  }
  var pos = document.getElementsByName("el_pos");
  pos[0].removeAttribute("disabled");
  pos[1].removeAttribute("disabled");
  pos[2].removeAttribute("disabled");
  var sel_el_pos = document.getElementById("sel_el_pos");
  sel_el_pos.removeAttribute("disabled", "disabled");
  type_section_break(new_id, '<hr/>');
}

function form_maker_el_button() {
  if (document.getElementById("editing_id").value) {
    new_id = document.getElementById("editing_id").value;
  }
  else {
    new_id = gen;
  }
  // Edit table.
  var td = document.getElementById('edit_table');
  // Type selec.
  var el_type_label = document.createElement('label');
  el_type_label.style.cssText = "color: #00aeef; font-weight: bold; font-size: 13px";
  el_type_label.innerHTML = "<br />&nbsp;&nbsp;Field type";
  td.appendChild(el_type_label);
  var el_type_radio_submit_reset = document.createElement('input');
  el_type_radio_submit_reset.setAttribute("type", "radio");
  el_type_radio_submit_reset.style.cssText = "margin-left:15px";
  el_type_radio_submit_reset.setAttribute("name", "el_type");
  el_type_radio_submit_reset.setAttribute("onclick", "go_to_type_submit_reset('" + new_id + "')");
  el_type_radio_submit_reset.setAttribute("checked", "checked");
  Submit_and_Reset = document.createTextNode("Submit and Reset");
  var el_type_radio_custom = document.createElement('input');
  el_type_radio_custom.setAttribute("type", "radio");
  el_type_radio_custom.style.cssText = "margin-left:15px";
  el_type_radio_custom.setAttribute("name", "el_type");
  el_type_radio_custom.setAttribute("onclick", "go_to_type_button('" + new_id + "')");
  Custom = document.createTextNode("Custom");
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  td.appendChild(br1);
  td.appendChild(el_type_radio_submit_reset);
  td.appendChild(Submit_and_Reset);
  td.appendChild(br2);
  td.appendChild(el_type_radio_custom);
  td.appendChild(Custom);
  var pos = document.getElementsByName("el_pos");
  pos[0].removeAttribute("disabled");
  pos[1].removeAttribute("disabled");
  pos[2].removeAttribute("disabled");
  var sel_el_pos = document.getElementById("sel_el_pos");
  sel_el_pos.removeAttribute("disabled", "disabled");
  go_to_type_submit_reset(new_id);
}

function go_to_type_hidden(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  type_hidden(new_id, '', '', '', w_attr_name, w_attr_value);
}

function go_to_type_submit_reset(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  type_submit_reset(new_id, 'Submit', 'Reset', '', true, w_attr_name, w_attr_value);
}

function go_to_type_mark_map(new_id) {
  w_attr_name = [];
  w_attr_value = [];
  type_mark_map(new_id, 'Mark your place on map:', 'left', '2.294254', '48.858334', "2.294254", "48.858334", "13", "400", "300", 'wdform_map', '', w_attr_name, w_attr_value);
}

function form_maker_el_map() {
  alert(Drupal.t("This field type is disabled in free version. If you need this functionality, you need to buy the commercial version."));
  return;
}

function destroyChildren(node) {
  while (node.firstChild) {
    node.removeChild(node.firstChild);
  }
}

// function cont_elements() {
  // cp2 = 1;
  // for (t = 0; t < 100; t++) {
    // if (document.getElementById(t)) {
      // ++cp2;
    // }
  // }
  // return cp2;
// }

function cont_elements() {
  cp2 = 0;
  var zzzxxx = document.getElementsByClassName('wdform_table1').length;
  for (t = 1; t < 100; t++) {
    if (document.getElementById(t)) {
      cp2++;
    }
  }
  return cp2 + zzzxxx;
}

function add(key) {
  // if (document.getElementById('editing_id').value == "") {
    // if (key == 0) {
      // kz6 = cont_elements();
      // if (kz6 > 7) {
        // alert(Drupal.t("The free version is limited up to 7 fields to add. If you need this functionality, you need to buy the commercial version."));
        // return;
      // }
    // }
  // }
  if (document.getElementById('editing_id').value == "") {
    if (key == 0) {
    var wen_limit_is_page_break = document.getElementsByClassName('wdform_table1').length;		
      kz6 = cont_elements();
      if (document.getElementById("element_type").value == "type_page_navigation") {
        page_navigation = 1;
      }
      else {
        page_navigation = 0;
      }
      if (kz6 > (7 + page_navigation)) {
        alert(Drupal.t("The free version is limited up to 7 fields to add. If you need this functionality, you need to buy the commercial version."));
        return;
      }
    }
  }
  if (document.getElementById("element_type").value == "type_section_break") {
    form_view = 0;
    for (t = form_view_max; t > 0; t--) {
      if (document.getElementById('form_id_tempform_view' + t))
        if (jQuery("#form_id_tempform_view" + t).is(":visible")) {
          form_view = t;
          form_maker_remove_spaces(document.getElementById('form_id_tempform_view' + form_view));
          break;
        }
    }
    if (form_view == 0) {
      alert(Drupal.t("The pages are closed."));
      return;
    }
    if (document.getElementById('editing_id').value) {
      i = document.getElementById('editing_id').value;
      document.getElementById('editing_id').value = "";
      tr = document.getElementById(i);
      destroyChildren(tr);
      var img_X = document.createElement("img");
      img_X.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/delete_el.png");
      img_X.setAttribute("height", "17");
      img_X.setAttribute("title", "Remove the field");
      img_X.style.cssText = "cursor:pointer; margin:2px";
      img_X.setAttribute("onclick", 'remove_section_break("' + i + '")');

      var td_X = document.createElement("td");
      td_X.setAttribute("id", "X_" + i);
      td_X.setAttribute("valign", "middle");
      td_X.setAttribute("align", "right");
      td_X.setAttribute("class", "element_toolbar");
      td_X.appendChild(img_X);

      var img_UP = document.createElement("img");
      img_UP.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/up.png");
      img_UP.setAttribute("title", "Move the field up");

      img_UP.style.cssText = "cursor:pointer";
      img_UP.setAttribute("onclick", 'up_row("' + i + '")');

      var td_UP = document.createElement("td");
      td_UP.setAttribute("id", "up_" + i);
      td_UP.setAttribute("valign", "middle");
      td_UP.setAttribute("class", "element_toolbar");
      td_UP.appendChild(img_UP);

      var img_DOWN = document.createElement("img");
      img_DOWN.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/down.png");
      img_DOWN.setAttribute("title", "Move the field down");

      img_DOWN.style.cssText = "margin:2px;cursor:pointer";
      img_DOWN.setAttribute("onclick", 'down_row("' + i + '")');

      var td_DOWN = document.createElement("td");
      td_DOWN.setAttribute("id", "down_" + i);
      td_DOWN.setAttribute("valign", "middle");
      td_DOWN.setAttribute("class", "element_toolbar");
      td_DOWN.appendChild(img_DOWN);

      var img_EDIT = document.createElement("img");
      img_EDIT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/edit.png");
      img_EDIT.setAttribute("title", "Edit the field");

      img_EDIT.style.cssText = "margin:2px;cursor:pointer";
      img_EDIT.setAttribute("onclick", 'edit("' + i + '")');

      var td_EDIT = document.createElement("td");
      td_EDIT.setAttribute("id", "edit_" + i);
      td_EDIT.setAttribute("valign", "middle");
      td_EDIT.setAttribute("class", "element_toolbar");
      td_EDIT.appendChild(img_EDIT);

      var img_DUBLICATE = document.createElement("img");
      img_DUBLICATE.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/dublicate.png");
      img_DUBLICATE.setAttribute("title", "Dublicate the field");
      img_DUBLICATE.style.cssText = "margin:2px;cursor:pointer";
      img_DUBLICATE.setAttribute("onclick", 'dublicate("' + i + '")');

      var td_DUBLICATE = document.createElement("td");
      td_DUBLICATE.setAttribute("id", "dublicate_" + i);
      td_DUBLICATE.setAttribute("valign", "middle");
      td_DUBLICATE.setAttribute("class", "element_toolbar");
      td_DUBLICATE.appendChild(img_DUBLICATE);

      var img_PAGEUP = document.createElement("img");
      img_PAGEUP.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/page_up.png");
      img_PAGEUP.setAttribute("title", "Move the field to the upper page");
      img_PAGEUP.style.cssText = "margin:2px;cursor:pointer";
      img_PAGEUP.setAttribute("onclick", 'page_up("' + i + '")');

      var td_PAGEDOWN = document.createElement("td");
      td_PAGEDOWN.setAttribute("id", "page_up_" + i);
      td_PAGEDOWN.setAttribute("valign", "middle");
      td_PAGEDOWN.setAttribute("class", "element_toolbar");
      td_PAGEDOWN.appendChild(img_PAGEUP);

      var img_PAGEDOWN = document.createElement("img");
      img_PAGEDOWN.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/page_down.png");
      img_PAGEDOWN.setAttribute("title", "Move the field to the lower page");
      img_PAGEDOWN.style.cssText = "margin:2px;cursor:pointer";
      img_PAGEDOWN.setAttribute("onclick", 'page_down("' + i + '")');

      var td_PAGEUP = document.createElement("td");
      td_PAGEUP.setAttribute("id", "dublicate_" + i);
      td_PAGEUP.setAttribute("valign", "middle");
      td_PAGEUP.setAttribute("class", "element_toolbar");
      td_PAGEUP.appendChild(img_PAGEDOWN);

      var in_editor = document.createElement("td");
      in_editor.setAttribute("id", i + "_element_sectionform_id_temp");
      in_editor.setAttribute("align", 'left');
      in_editor.setAttribute("valign", "top");
      in_editor.setAttribute("colspan", "100");
      in_editor.setAttribute('class', 'toolbar_padding');
      if (document.getElementById('textAreaContent').style.display == "none") {
        if (document.getElementsByTagName("iframe")[0]) {
            ifr_id = document.getElementsByTagName("iframe")[0].id;
            ifr = getIFrameDocument(ifr_id);
            in_editor.innerHTML = ifr.body.innerHTML;
        }
      }
      else {
        in_editor.innerHTML = document.getElementById('textAreaContent').value;
      }
      var label = document.createElement('span');
      label.setAttribute("id", i + "_element_labelform_id_temp");
      label.innerHTML = "custom_" + i;
      label.style.cssText = 'display:none';
      td_EDIT.appendChild(label);
      tr.appendChild(in_editor);
      tr.appendChild(td_X);
      tr.appendChild(td_EDIT);
      tr.appendChild(td_DUBLICATE);
      j = 2;
    }
    else {
      i = gen;
      gen++;
      var tr = document.createElement('tr');
      tr.setAttribute("id", i);
      tr.setAttribute("class", "wdform_tr_section_break");
      tr.setAttribute("type", "type_section_break");

      var select_ = document.getElementById('sel_el_pos');
      var option = document.createElement('option');
      option.setAttribute("id", i + "_sel_el_pos");
      option.setAttribute("value", i);
      option.innerHTML = "custom_" + i;

      table = document.getElementById('form_id_tempform_view' + form_view);

      var img_X = document.createElement("img");
      img_X.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/delete_el.png");
      img_X.setAttribute("height", "17");
      img_X.setAttribute("title", "Remove the field");
      img_X.style.cssText = "cursor:pointer; margin:2px";
      img_X.setAttribute("onclick", 'remove_section_break("' + i + '")');

      var td_X = document.createElement("td");
      td_X.setAttribute("id", "X_" + i);
      td_X.setAttribute("valign", "middle");
      td_X.setAttribute("align", "right");
      td_X.setAttribute("class", "element_toolbar");
      td_X.appendChild(img_X);

      var img_UP = document.createElement("img");
      img_UP.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/up.png");
      img_UP.setAttribute("title", "Move the field up");

      img_UP.style.cssText = "cursor:pointer";
      img_UP.setAttribute("onclick", 'up_row("' + i + '")');

      var td_UP = document.createElement("td");
      td_UP.setAttribute("id", "up_" + i);
      td_UP.setAttribute("valign", "middle");
      td_UP.setAttribute("class", "element_toolbar");
      td_UP.appendChild(img_UP);

      var img_DOWN = document.createElement("img");
      img_DOWN.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/down.png");
      img_DOWN.setAttribute("title", "Move the field down");

      img_DOWN.style.cssText = "margin:2px;cursor:pointer";
      img_DOWN.setAttribute("onclick", 'down_row("' + i + '")');

      var td_DOWN = document.createElement("td");
      td_DOWN.setAttribute("id", "down_" + i);
      td_DOWN.setAttribute("valign", "middle");
      td_DOWN.setAttribute("class", "element_toolbar");
      td_DOWN.appendChild(img_DOWN);


      var img_EDIT = document.createElement("img");
      img_EDIT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/edit.png");
      img_EDIT.setAttribute("title", "Edit the field");
      img_EDIT.style.cssText = "margin:2px;cursor:pointer";
      img_EDIT.setAttribute("onclick", 'edit("' + i + '")');

      var td_EDIT = document.createElement("td");
      td_EDIT.setAttribute("id", "edit_" + i);
      td_EDIT.setAttribute("valign", "middle");
      td_EDIT.setAttribute("class", "element_toolbar");
      td_EDIT.appendChild(img_EDIT);

      var img_DUBLICATE = document.createElement("img");
      img_DUBLICATE.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/dublicate.png");
      img_DUBLICATE.setAttribute("title", "Dublicate the field");
      img_DUBLICATE.style.cssText = "margin:2px;cursor:pointer";
      img_DUBLICATE.setAttribute("onclick", 'dublicate("' + i + '")');

      var td_DUBLICATE = document.createElement("td");
      td_DUBLICATE.setAttribute("id", "dublicate_" + i);
      td_DUBLICATE.setAttribute("valign", "middle");
      td_DUBLICATE.setAttribute("class", "element_toolbar");
      td_DUBLICATE.appendChild(img_DUBLICATE);

      var img_PAGEUP = document.createElement("img");
      img_PAGEUP.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/page_up.png");
      img_PAGEUP.setAttribute("title", "Move the field to the upper page");
      img_PAGEUP.style.cssText = "margin:2px;cursor:pointer";
      img_PAGEUP.setAttribute("onclick", 'page_up("' + i + '")');

      var td_PAGEDOWN = document.createElement("td");
      td_PAGEDOWN.setAttribute("id", "page_up_" + i);
      td_PAGEDOWN.setAttribute("valign", "middle");
      td_PAGEDOWN.setAttribute("class", "element_toolbar");
      td_PAGEDOWN.appendChild(img_PAGEUP);

      var img_PAGEDOWN = document.createElement("img");
      img_PAGEDOWN.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/page_down.png");
      img_PAGEDOWN.setAttribute("title", "Move the field to the lower page");
      img_PAGEDOWN.style.cssText = "margin:2px;cursor:pointer";
      img_PAGEDOWN.setAttribute("onclick", 'page_down("' + i + '")');

      var td_PAGEUP = document.createElement("td");
      td_PAGEUP.setAttribute("id", "dublicate_" + i);
      td_PAGEUP.setAttribute("valign", "middle");
      td_PAGEUP.setAttribute("class", "element_toolbar");
      td_PAGEUP.appendChild(img_PAGEDOWN);

      var in_editor = document.createElement("td");
      in_editor.setAttribute("id", i + "_element_sectionform_id_temp");
      in_editor.setAttribute("align", 'left');
      in_editor.setAttribute("valign", "top");
      in_editor.setAttribute("colspan", "100");
      in_editor.setAttribute('class', 'toolbar_padding');

      // ifr_id = document.getElementsByTagName("iframe")[0].id;
      // ifr = getIFrameDocument(ifr_id)

      if (document.getElementById('textAreaContent').style.display == "none") {
        if (document.getElementsByTagName("iframe")[0]) {
          ifr_id = document.getElementsByTagName("iframe")[0].id;
          ifr = getIFrameDocument(ifr_id);
          in_editor.innerHTML = ifr.body.innerHTML;
        }
        // in_editor.innerHTML = ifr.body.innerHTML;
      }
      else {
        in_editor.innerHTML = document.getElementById('textAreaContent').value;
        // in_editor.innerHTML = document.getElementById('editor').value;
      }
      var label = document.createElement('span');
      label.setAttribute("id", i + "_element_labelform_id_temp");
      label.innerHTML = "custom_" + i;
      label.style.cssText = 'display:none';

      td_EDIT.appendChild(label);
      tr.appendChild(in_editor);

      tr.appendChild(td_X);
      tr.appendChild(td_EDIT);
      tr.appendChild(td_DUBLICATE);

      beforeTr = table.lastChild;
      table.insertBefore(tr, beforeTr);

      tr = document.createElement('tr');
      tr.setAttribute('class', 'wdform_tr1');
      tr.setAttribute('style', 'background-color:rgba(0,0,0,0)');
      td = document.createElement('td');
      td.setAttribute('class', 'wdform_td1 form_maker_table');
      table_min = document.createElement('table');
      table_min.setAttribute('class', 'wdform_table2 form_maker_table');
      tbody_min = document.createElement('tbody');
      tbody_min.setAttribute('class', 'wdform_tbody2 form_maker_table');
      table_min.appendChild(tbody_min);
      td.appendChild(table_min);
      tr.appendChild(td);
      beforeTr = table.lastChild;
      table.insertBefore(tr, beforeTr);
      j = 2;
    }
    form_maker_close_window();
    return;
  }
  if (document.getElementById("element_type").value == "type_page_navigation") {
    document.getElementById("pages").setAttribute('show_title', document.getElementById("el_show_title_input").checked);
    document.getElementById("pages").setAttribute('show_numbers', document.getElementById("el_show_numbers_input").checked);
    if (document.getElementById("el_pagination_steps").checked) {
      document.getElementById("pages").setAttribute('type', 'steps');
      make_page_steps_front();
    }
    else if (document.getElementById("el_pagination_percentage").checked) {
      document.getElementById("pages").setAttribute('type', 'percentage');
      make_page_percentage_front();
    }
    else {
      document.getElementById("pages").setAttribute('type', 'none');
      make_page_none_front();
    }
    refresh_page_numbers();
    form_maker_close_window();
    return;
  }
  if (document.getElementById("element_type").value == "type_page_break") {
    if (document.getElementById("editing_id").value) {
      i = document.getElementById("editing_id").value;
      form_view_element = document.getElementById('form_id_tempform_view' + i);
      page_title = document.getElementById('_div_between').getAttribute('page_title');
      next_title = document.getElementById('_div_between').getAttribute('next_title');
      next_type = document.getElementById('_div_between').getAttribute('next_type');
      next_class = document.getElementById('_div_between').getAttribute('next_class');
      next_checkable = document.getElementById('_div_between').getAttribute('next_checkable');
      previous_title = document.getElementById('_div_between').getAttribute('previous_title');
      previous_type = document.getElementById('_div_between').getAttribute('previous_type');
      previous_class = document.getElementById('_div_between').getAttribute('previous_class');
      previous_checkable = document.getElementById('_div_between').getAttribute('previous_checkable');
      form_view_element.setAttribute('next_title', next_title);
      form_view_element.setAttribute('next_type', next_type);
      form_view_element.setAttribute('next_class', next_class);
      form_view_element.setAttribute('next_checkable', next_checkable);
      form_view_element.setAttribute('previous_title', previous_title);
      form_view_element.setAttribute('previous_type', previous_type);
      form_view_element.setAttribute('previous_class', previous_class);
      form_view_element.setAttribute('previous_checkable', previous_checkable);
      form_view_element.setAttribute('page_title', page_title);
      var input = document.getElementById('_div_between');
      atr = input.attributes;
      for (v = 0; v < 30; v++) {
        if (atr[v]) {
          if (atr[v].name.indexOf("add_") == 0) {
            form_view_element.setAttribute(atr[v].name, atr[v].value);
          }
        }
      }
      if (form_view_count != 1) {
        generate_page_nav(form_view);
      }
      form_maker_close_window();
      return;
    }
    else {
      for (t = form_view_max; t > 0; t--) {
        if (document.getElementById('form_id_tempform_view' + t)) {
          form_view = t;
          break;
        }
      }
      if (form_view_count == 1) {
        var img_EDIT = document.createElement("img");
        img_EDIT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/edit.png");
        img_EDIT.setAttribute("title", "Edit the pagination options");
        img_EDIT.style.cssText = "margin-left:40px; cursor:pointer";
        img_EDIT.setAttribute("onclick", 'el_page_navigation()');
        var td_EDIT = document.getElementById("edit_page_navigation");
        td_EDIT.appendChild(img_EDIT);
        document.getElementById('page_navigation').appendChild(td_EDIT);
      }
      old_to_gen = form_view;

      form_view_max++;
      form_view_count++;
      form_view = form_view_max;

      table = document.createElement('table');
      table.setAttribute('cellpadding', '4');
      table.setAttribute('cellspacing', '0');
      table.setAttribute('class', 'wdform_table1 form_maker_table');
      table.style.cssText = "border-top:1px solid black";

      tbody = document.createElement('tbody');
      tbody.setAttribute('id', 'form_id_tempform_view' + form_view);
      tbody.setAttribute('page_title', 'Untitled Page');
      tbody.setAttribute('class', 'wdform_tbody1 form_maker_table');

      tbody_img = document.createElement('tbody');
      tbody_img.setAttribute('id', 'form_id_tempform_view_img' + form_view);
      tbody_img.style.cssText = "float:right";

      tr_img = document.createElement('tr');
      tr_img.setAttribute('valign', 'middle');
      td_title = document.createElement('td');
      td_title.setAttribute('width', '0%');
      td_img = document.createElement('td');
      td_img.setAttribute('align', 'right');

      var img = document.createElement('img');
      img.setAttribute('src', '' + Drupal.settings.form_maker.get_module_path + '/images/minus.png');
      img.setAttribute('title', 'Show or hide the page');
      img.setAttribute("class", "page_toolbar");
      img.setAttribute('id', 'show_page_img_' + form_view);
      img.setAttribute('onClick', 'show_or_hide("' + form_view + '")');

      var img_X = document.createElement("img");
      img_X.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/page_delete.png");
      img_X.setAttribute('title', 'Delete the page');
      img_X.setAttribute("class", "page_toolbar");
      img_X.setAttribute("onclick", 'remove_page("' + form_view + '")');

      var td_X = document.createElement("td");
      td_X.appendChild(img_X);

      var img_X_all = document.createElement("img");
      img_X_all.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/page_delete_all.png");
      img_X_all.setAttribute('title', 'Delete the page with fields');
      img_X_all.setAttribute("class", "page_toolbar");
      img_X_all.setAttribute("onclick", 'remove_page_all("' + form_view + '")');

      var td_X_all = document.createElement("td");
      td_X_all.appendChild(img_X_all);

      var img_EDIT = document.createElement("img");
      img_EDIT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/page_edit.png");
      img_EDIT.setAttribute('title', 'Edit the page');
      img_EDIT.setAttribute("class", "page_toolbar");
      img_EDIT.setAttribute("onclick", 'edit_page_break("' + form_view + '")');

      var td_EDIT = document.createElement("td");
      td_EDIT.appendChild(img_EDIT);

      td_img.appendChild(img);
      tr_img.appendChild(td_title);
      tr_img.appendChild(td_img);
      tr_img.appendChild(td_X);
      tr_img.appendChild(td_X_all);
      tr_img.appendChild(td_EDIT);
      tbody_img.appendChild(tr_img);

      tr = document.createElement('tr');
      tr.setAttribute('class', 'wdform_tr1');
      tr.setAttribute('style', 'background-color:rgba(0,0,0,0)');
      td = document.createElement('td');
      td.setAttribute('class', 'wdform_td1 form_maker_table');

      tr_page_nav = document.createElement('tr');
      tr_page_nav.setAttribute('valign', 'top');
      tr_page_nav.setAttribute('class', 'wdform_footer');
      tr_page_nav.setAttribute('style', 'background-color:rgba(0,0,0,0);');

      td_page_nav = document.createElement('td');
      td_page_nav.setAttribute('colspan', '100');

      table_min_page_nav = document.createElement('table');
      table_min_page_nav.setAttribute('width', '100%');
      table_min_page_nav.setAttribute('style', "border:none;");

      tbody_min_page_nav = document.createElement('tbody');
      tr_min_page_nav = document.createElement('tr');
      tr_min_page_nav.setAttribute('id', 'form_id_temppage_nav' + form_view);
      table_min = document.createElement('table');
      table_min.setAttribute('class', 'wdform_table2 form_maker_table');
      tbody_min = document.createElement('tbody');
      tbody_min.setAttribute('class', 'wdform_tbody2 form_maker_table');
      table_min.appendChild(tbody_min);
      td.appendChild(table_min);
      tr.appendChild(td);
      tbody_min_page_nav.appendChild(tr_min_page_nav);
      table_min_page_nav.appendChild(tbody_min_page_nav);
      td_page_nav.appendChild(table_min_page_nav);
      tr_page_nav.appendChild(td_page_nav);
      tbody.appendChild(tr);
      tbody.appendChild(tr_page_nav);
      table.appendChild(tbody);
      table.appendChild(tbody_img);
      document.getElementById('take').appendChild(table);
      form_view_element = document.getElementById('form_id_tempform_view' + form_view);
      page_title = document.getElementById('_div_between').getAttribute('page_title');
      next_title = document.getElementById('_div_between').getAttribute('next_title');
      next_type = document.getElementById('_div_between').getAttribute('next_type');
      next_class = document.getElementById('_div_between').getAttribute('next_class');
      next_checkable = document.getElementById('_div_between').getAttribute('next_checkable');
      previous_title = document.getElementById('_div_between').getAttribute('previous_title');
      previous_type = document.getElementById('_div_between').getAttribute('previous_type');
      previous_class = document.getElementById('_div_between').getAttribute('previous_class');
      previous_checkable = document.getElementById('_div_between').getAttribute('previous_checkable');
      form_view_element.setAttribute('next_title', next_title);
      form_view_element.setAttribute('next_type', next_type);
      form_view_element.setAttribute('next_class', next_class);
      form_view_element.setAttribute('next_checkable', next_checkable);
      form_view_element.setAttribute('previous_title', previous_title);
      form_view_element.setAttribute('previous_type', previous_type);
      form_view_element.setAttribute('previous_class', previous_class);
      form_view_element.setAttribute('previous_checkable', previous_checkable);
      form_view_element.setAttribute('page_title', page_title);
      var input = document.getElementById('_div_between');
      atr = input.attributes;
      for (v = 0; v < 30; v++) {
        if (atr[v]) {
          if (atr[v].name.indexOf("add_") == 0) {
            form_view_element.setAttribute(atr[v].name, atr[v].value);
          }
        }
      }
      if (form_view_count == 2) {
        generate_page_nav(form_view);
        generate_page_nav(old_to_gen);
      }
      else {
        generate_page_nav(form_view);
      }
      form_maker_close_window();
      return;
    }
  }
  form_view = 0;
  for (t = form_view_max; t > 0; t--) {
    if (document.getElementById('form_id_tempform_view' + t)) {
      if (jQuery("#form_id_tempform_view" + t).is(":visible")) {
        form_view = t;
        break;
      }
    }
  }
  if (form_view == 0) {
    alert(Drupal.t("The pages are closed."));
    return;
  }
  if (document.getElementById('main_editor').style.display == "block") {
    if (document.getElementById('editing_id').value) {
      i = document.getElementById('editing_id').value;
      document.getElementById('editing_id').value = "";
      tr = document.getElementById(i);
      destroyChildren(tr);
      var img_X = document.createElement("img");
      img_X.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/delete_el.png");
      img_X.setAttribute("height", "17");
      img_X.setAttribute("title", "Remove the field");
      img_X.style.cssText = "cursor:pointer; margin:2px";
      img_X.setAttribute("onclick", 'remove_row("' + i + '")');

      var td_X = document.createElement("td");
      td_X.setAttribute("id", "X_" + i);
      td_X.setAttribute("valign", "middle");
      td_X.setAttribute("align", "right");
      td_X.setAttribute("class", "element_toolbar");
      td_X.appendChild(img_X);
      var img_UP = document.createElement("img");
      img_UP.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/up.png");
      img_UP.setAttribute("title", "Move the field up");

      img_UP.style.cssText = "cursor:pointer";
      img_UP.setAttribute("onclick", 'up_row("' + i + '")');

      var td_UP = document.createElement("td");
      td_UP.setAttribute("id", "up_" + i);
      td_UP.setAttribute("valign", "middle");
      td_UP.setAttribute("class", "element_toolbar");
      td_UP.appendChild(img_UP);

      var img_DOWN = document.createElement("img");
      img_DOWN.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/down.png");
      img_DOWN.setAttribute("title", "Move the field down");
      img_DOWN.style.cssText = "margin:2px;cursor:pointer";
      img_DOWN.setAttribute("onclick", 'down_row("' + i + '")');

      var td_DOWN = document.createElement("td");
      td_DOWN.setAttribute("id", "down_" + i);
      td_DOWN.setAttribute("valign", "middle");
      td_DOWN.setAttribute("class", "element_toolbar");
      td_DOWN.appendChild(img_DOWN);

      var img_RIGHT = document.createElement("img");
      img_RIGHT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/right.png");
      img_RIGHT.setAttribute("title", "Move the field to the right");
      img_RIGHT.style.cssText = "cursor:pointer";
      img_RIGHT.setAttribute("onclick", 'right_row("' + i + '")');

      var td_RIGHT = document.createElement("td");
      td_RIGHT.setAttribute("id", "right_" + i);
      td_RIGHT.setAttribute("valign", "middle");
      td_RIGHT.setAttribute("class", "element_toolbar");
      td_RIGHT.appendChild(img_RIGHT);

      var img_LEFT = document.createElement("img");
      img_LEFT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/left.png");
      img_LEFT.setAttribute("title", "Move the field to the left");
      img_LEFT.style.cssText = "margin:2px;cursor:pointer";
      img_LEFT.setAttribute("onclick", 'left_row("' + i + '")');

      var td_LEFT = document.createElement("td");
      td_LEFT.setAttribute("id", "left_" + i);
      td_LEFT.setAttribute("valign", "middle");
      td_LEFT.setAttribute("class", "element_toolbar");
      td_LEFT.appendChild(img_LEFT);

      var img_EDIT = document.createElement("img");
      img_EDIT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/edit.png");
      img_EDIT.setAttribute("title", "Edit the field");
      img_EDIT.style.cssText = "margin:2px;cursor:pointer";
      img_EDIT.setAttribute("onclick", 'edit("' + i + '")');

      var td_EDIT = document.createElement("td");
      td_EDIT.setAttribute("id", "edit_" + i);
      td_EDIT.setAttribute("valign", "middle");
      td_EDIT.setAttribute("class", "element_toolbar");
      td_EDIT.appendChild(img_EDIT);

      var img_DUBLICATE = document.createElement("img");
      img_DUBLICATE.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/dublicate.png");
      img_DUBLICATE.setAttribute("title", "Dublicate the field");
      img_DUBLICATE.style.cssText = "margin:2px;cursor:pointer";
      img_DUBLICATE.setAttribute("onclick", 'dublicate("' + i + '")');

      var td_DUBLICATE = document.createElement("td");
      td_DUBLICATE.setAttribute("id", "dublicate_" + i);
      td_DUBLICATE.setAttribute("valign", "middle");
      td_DUBLICATE.setAttribute("class", "element_toolbar");
      td_DUBLICATE.appendChild(img_DUBLICATE);

      var img_PAGEUP = document.createElement("img");
      img_PAGEUP.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/page_up.png");
      img_PAGEUP.setAttribute("title", "Move the field to the upper page");
      img_PAGEUP.style.cssText = "margin:2px;cursor:pointer";
      img_PAGEUP.setAttribute("onclick", 'page_up("' + i + '")');

      var td_PAGEDOWN = document.createElement("td");
      td_PAGEDOWN.setAttribute("id", "page_up_" + i);
      td_PAGEDOWN.setAttribute("valign", "middle");
      td_PAGEDOWN.setAttribute("class", "element_toolbar");
      td_PAGEDOWN.appendChild(img_PAGEUP);

      var img_PAGEDOWN = document.createElement("img");
      img_PAGEDOWN.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/page_down.png");
      img_PAGEDOWN.setAttribute("title", "Move the field to the lower page");
      img_PAGEDOWN.style.cssText = "margin:2px;cursor:pointer";
      img_PAGEDOWN.setAttribute("onclick", 'page_down("' + i + '")');

      var td_PAGEUP = document.createElement("td");
      td_PAGEUP.setAttribute("id", "dublicate_" + i);
      td_PAGEUP.setAttribute("valign", "middle");
      td_PAGEUP.setAttribute("class", "element_toolbar");
      td_PAGEUP.appendChild(img_PAGEDOWN);
      var in_editor = document.createElement("td");
      in_editor.setAttribute("id", i + "_element_sectionform_id_temp");
      in_editor.setAttribute("align", 'left');
      in_editor.setAttribute("valign", "top");
      in_editor.setAttribute("colspan", "2");
      in_editor.setAttribute('class', 'toolbar_padding');
      if (document.getElementById('textAreaContent').style.display == "none") {
        if (document.getElementsByTagName("iframe")[0]) {
          ifr_id = document.getElementsByTagName("iframe")[0].id;
          ifr = getIFrameDocument(ifr_id);
          in_editor.innerHTML = ifr.body.innerHTML;
        }
      }
      else {
        in_editor.innerHTML = document.getElementById('textAreaContent').value;
      }
      var label = document.createElement('span');
      label.setAttribute("id", i + "_element_labelform_id_temp");
      label.innerHTML = "custom_" + i;
      label.style.cssText = 'display:none';
      td_EDIT.appendChild(label);
      tr.appendChild(in_editor);
      tr.appendChild(td_X);
      tr.appendChild(td_LEFT);
      tr.appendChild(td_UP);
      tr.appendChild(td_DOWN);
      tr.appendChild(td_RIGHT);
      tr.appendChild(td_EDIT);
      tr.appendChild(td_DUBLICATE);
      tr.appendChild(td_PAGEUP);
      tr.appendChild(td_PAGEDOWN);
      j = 2;
    }
    else {
      i = gen;
      gen++;
      var tr = document.createElement('tr');
      tr.setAttribute("id", i);
      tr.setAttribute("type", "type_editor");
      var select_ = document.getElementById('sel_el_pos');
      var option = document.createElement('option');
      option.setAttribute("id", i + "_sel_el_pos");
      option.setAttribute("value", i);
      option.innerHTML = "custom_" + i;
      l = document.getElementById('form_id_tempform_view' + form_view).childNodes.length;
      if (document.getElementById('form_id_tempform_view' + form_view).firstChild.nodeType == 3) {
        table = document.getElementById('form_id_tempform_view' + form_view).childNodes[l - 3].childNodes[1].childNodes[1].childNodes[1];
      }
      else {
        table = document.getElementById('form_id_tempform_view' + form_view).childNodes[l - 2].firstChild.firstChild.firstChild;
      }
      var img_X = document.createElement("img");
      img_X.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/delete_el.png");
      img_X.setAttribute("height", "17");
      img_X.setAttribute("title", "Remove the field");
      img_X.style.cssText = "cursor:pointer; margin:2px";
      img_X.setAttribute("onclick", 'remove_row("' + i + '")');

      var td_X = document.createElement("td");
      td_X.setAttribute("id", "X_" + i);
      td_X.setAttribute("valign", "middle");
      td_X.setAttribute("align", "right");
      td_X.setAttribute("class", "element_toolbar");
      td_X.appendChild(img_X);

      var img_UP = document.createElement("img");
      img_UP.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/up.png");
      img_UP.setAttribute("title", "Move the field up");
      img_UP.style.cssText = "cursor:pointer";
      img_UP.setAttribute("onclick", 'up_row("' + i + '")');

      var td_UP = document.createElement("td");
      td_UP.setAttribute("id", "up_" + i);
      td_UP.setAttribute("valign", "middle");
      td_UP.setAttribute("class", "element_toolbar");
      td_UP.appendChild(img_UP);

      var img_DOWN = document.createElement("img");
      img_DOWN.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/down.png");
      img_DOWN.setAttribute("title", "Move the field down");
      img_DOWN.style.cssText = "margin:2px;cursor:pointer";
      img_DOWN.setAttribute("onclick", 'down_row("' + i + '")');

      var td_DOWN = document.createElement("td");
      td_DOWN.setAttribute("id", "down_" + i);
      td_DOWN.setAttribute("valign", "middle");
      td_DOWN.setAttribute("class", "element_toolbar");
      td_DOWN.appendChild(img_DOWN);

      var img_RIGHT = document.createElement("img");
      img_RIGHT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/right.png");
      img_RIGHT.setAttribute("title", "Move the field to the right");
      img_RIGHT.style.cssText = "cursor:pointer";
      img_RIGHT.setAttribute("onclick", 'right_row("' + i + '")');

      var td_RIGHT = document.createElement("td");
      td_RIGHT.setAttribute("id", "right_" + i);
      td_RIGHT.setAttribute("valign", "middle");
      td_RIGHT.setAttribute("class", "element_toolbar");
      td_RIGHT.appendChild(img_RIGHT);

      var img_LEFT = document.createElement("img");
      img_LEFT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/left.png");
      img_LEFT.setAttribute("title", "Move the field to the left");
      img_LEFT.style.cssText = "margin:2px;cursor:pointer";
      img_LEFT.setAttribute("onclick", 'left_row("' + i + '")');

      var td_LEFT = document.createElement("td");
      td_LEFT.setAttribute("id", "left_" + i);
      td_LEFT.setAttribute("valign", "middle");
      td_LEFT.setAttribute("class", "element_toolbar");
      td_LEFT.appendChild(img_LEFT);

      var img_EDIT = document.createElement("img");
      img_EDIT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/edit.png");
      img_EDIT.setAttribute("title", "Edit the field");
      img_EDIT.style.cssText = "margin:2px;cursor:pointer";
      img_EDIT.setAttribute("onclick", 'edit("' + i + '")');

      var td_EDIT = document.createElement("td");
      td_EDIT.setAttribute("id", "edit_" + i);
      td_EDIT.setAttribute("valign", "middle");
      td_EDIT.setAttribute("class", "element_toolbar");
      td_EDIT.appendChild(img_EDIT);

      var img_DUBLICATE = document.createElement("img");
      img_DUBLICATE.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/dublicate.png");
      img_DUBLICATE.setAttribute("title", "Dublicate the field");
      img_DUBLICATE.style.cssText = "margin:2px;cursor:pointer";
      img_DUBLICATE.setAttribute("onclick", 'dublicate("' + i + '")');

      var td_DUBLICATE = document.createElement("td");
      td_DUBLICATE.setAttribute("id", "dublicate_" + i);
      td_DUBLICATE.setAttribute("valign", "middle");
      td_DUBLICATE.setAttribute("class", "element_toolbar");
      td_DUBLICATE.appendChild(img_DUBLICATE);

      var img_PAGEUP = document.createElement("img");
      img_PAGEUP.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/page_up.png");
      img_PAGEUP.setAttribute("title", "Move the field to the upper page");
      img_PAGEUP.style.cssText = "margin:2px;cursor:pointer";
      img_PAGEUP.setAttribute("onclick", 'page_up("' + i + '")');

      var td_PAGEDOWN = document.createElement("td");
      td_PAGEDOWN.setAttribute("id", "page_up_" + i);
      td_PAGEDOWN.setAttribute("valign", "middle");
      td_PAGEDOWN.setAttribute("class", "element_toolbar");
      td_PAGEDOWN.appendChild(img_PAGEUP);

      var img_PAGEDOWN = document.createElement("img");
      img_PAGEDOWN.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/page_down.png");
      img_PAGEDOWN.setAttribute("title", "Move the field to the lower page");
      img_PAGEDOWN.style.cssText = "margin:2px;cursor:pointer";
      img_PAGEDOWN.setAttribute("onclick", 'page_down("' + i + '")');

      var td_PAGEUP = document.createElement("td");
      td_PAGEUP.setAttribute("id", "dublicate_" + i);
      td_PAGEUP.setAttribute("valign", "middle");
      td_PAGEUP.setAttribute("class", "element_toolbar");
      td_PAGEUP.appendChild(img_PAGEDOWN);
      var in_editor = document.createElement("td");
      in_editor.setAttribute("id", i + "_element_sectionform_id_temp");
      in_editor.setAttribute("align", 'left');
      in_editor.setAttribute("valign", "top");
      in_editor.setAttribute("colspan", "2");
      in_editor.setAttribute('class', 'toolbar_padding');
      if (document.getElementById('textAreaContent').style.display == "none") {
        // in_editor.innerHTML = ifr.body.innerHTML;
        if (document.getElementsByTagName("iframe")[0]) {
          ifr_id = document.getElementsByTagName("iframe")[0].id;
          ifr = getIFrameDocument(ifr_id);
          in_editor.innerHTML = ifr.body.innerHTML;
        }
      }
      else {
        in_editor.innerHTML = document.getElementById('textAreaContent').value;
        // in_editor.innerHTML = document.getElementById('editor').value;
      }
      var label = document.createElement('span');
      label.setAttribute("id", i + "_element_labelform_id_temp");
      label.innerHTML = "custom_" + i;
      label.style.cssText = 'display:none';
      td_EDIT.appendChild(label);
      tr.appendChild(in_editor);
      tr.appendChild(td_X);
      tr.appendChild(td_LEFT);
      tr.appendChild(td_UP);
      tr.appendChild(td_DOWN);
      tr.appendChild(td_RIGHT);
      tr.appendChild(td_EDIT);
      tr.appendChild(td_DUBLICATE);
      tr.appendChild(td_PAGEUP);
      tr.appendChild(td_PAGEDOWN);
      if (document.getElementById('pos_end').checked) {
        table.appendChild(tr);
      }
      if (document.getElementById('pos_begin').checked) {
        table.insertBefore(tr, table.firstChild);
      }
      if (document.getElementById('pos_before').checked) {
        beforeTr = document.getElementById(document.getElementById('sel_el_pos').value);
        table = beforeTr.parentNode;
        beforeOption = document.getElementById(document.getElementById('sel_el_pos').value + '_sel_el_pos');
        table.insertBefore(tr, beforeTr);
        select_.insertBefore(option, beforeOption);
      }
      j = 2;
    }
    form_maker_close_window();
  }
  else if (document.getElementById('show_table').innerHTML) {
    if (document.getElementById('editing_id').value) {
      i = document.getElementById('editing_id').value;
    }
    else {
      i = gen;
    }
    type = document.getElementById("element_type").value;
    if (type == "type_hidden") {
      if (document.getElementById(i + '_elementform_id_temp').name == "") {
        alert(Drupal.t("The name of the field is required."));
        return;
      }
    }
    if (type == "type_map") {
      if_gmap_updateMap(i);
    }
    if (type == "type_mark_map") {
      if_gmap_updateMap(i);
    }
    if (document.getElementById(i + '_element_labelform_id_temp').innerHTML) {
      if (document.getElementById('editing_id').value) {
        Disable();
        i = document.getElementById('editing_id').value;
        in_lab = false;
        labels_array = new Array();
        for (w = 0; w < gen; w++) {
          if (w != i) {
            if (document.getElementById(w + '_element_labelform_id_temp')) {
              labels_array.push(document.getElementById(w + '_element_labelform_id_temp').innerHTML);
            }
          }
        }

        for (t = 0; t < labels_array.length; t++) {
          if (document.getElementById(i + '_element_labelform_id_temp').innerHTML == labels_array[t]) {
            in_lab = true;
            break;
          }
        }
        if (in_lab) {
          alert(Drupal.t('Sorry, the labels must be unique.'));
          return;
        }
        else {
          document.getElementById('editing_id').value = "";
          tr = document.getElementById(i);
          destroyChildren(tr);
          tr.setAttribute('type', type);
          var img_X = document.createElement("img");
          img_X.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/delete_el.png");
          img_X.setAttribute("height", "17");
          img_X.setAttribute("title", "Remove the field");
          img_X.style.cssText = "cursor:pointer; margin:2px";
          img_X.setAttribute("onclick", 'remove_row("' + i + '")');

          var td_X = document.createElement("td");
          td_X.setAttribute("id", "X_" + i);
          td_X.setAttribute("valign", "middle");
          td_X.setAttribute("align", "right");
          td_X.setAttribute("class", "element_toolbar");
          td_X.appendChild(img_X);
          var img_UP = document.createElement("img");
          img_UP.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/up.png");
          img_UP.setAttribute("title", "Move the field up");
          img_UP.style.cssText = "cursor:pointer";
          img_UP.setAttribute("onclick", 'up_row("' + i + '")');

          var td_UP = document.createElement("td");
          td_UP.setAttribute("id", "up_" + i);
          td_UP.setAttribute("valign", "middle");
          td_UP.setAttribute("class", "element_toolbar");
          td_UP.appendChild(img_UP);

          var img_DOWN = document.createElement("img");
          img_DOWN.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/down.png");
          img_DOWN.setAttribute("title", "Move the field down");
          img_DOWN.style.cssText = "margin:2px;cursor:pointer";
          img_DOWN.setAttribute("onclick", 'down_row("' + i + '")');

          var td_DOWN = document.createElement("td");
          td_DOWN.setAttribute("id", "down_" + i);
          td_DOWN.setAttribute("valign", "middle");
          td_DOWN.setAttribute("class", "element_toolbar");
          td_DOWN.appendChild(img_DOWN);

          var img_RIGHT = document.createElement("img");
          img_RIGHT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/right.png");
          img_RIGHT.setAttribute("title", "Move the field to the right");
          img_RIGHT.style.cssText = "cursor:pointer";
          img_RIGHT.setAttribute("onclick", 'right_row("' + i + '")');

          var td_RIGHT = document.createElement("td");
          td_RIGHT.setAttribute("id", "right_" + i);
          td_RIGHT.setAttribute("valign", "middle");
          td_RIGHT.setAttribute("class", "element_toolbar");
          td_RIGHT.appendChild(img_RIGHT);

          var img_LEFT = document.createElement("img");
          img_LEFT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/left.png");
          img_LEFT.setAttribute("title", "Move the field to the left");
          img_LEFT.style.cssText = "margin:2px;cursor:pointer";
          img_LEFT.setAttribute("onclick", 'left_row("' + i + '")');

          var td_LEFT = document.createElement("td");
          td_LEFT.setAttribute("id", "left_" + i);
          td_LEFT.setAttribute("valign", "middle");
          td_LEFT.setAttribute("class", "element_toolbar");
          td_LEFT.appendChild(img_LEFT);

          var img_EDIT = document.createElement("img");
          img_EDIT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/edit.png");
          img_EDIT.setAttribute("title", "Edit the field");
          img_EDIT.style.cssText = "margin:2px;cursor:pointer";
          img_EDIT.setAttribute("onclick", 'edit("' + i + '")');

          var td_EDIT = document.createElement("td");
          td_EDIT.setAttribute("id", "edit_" + i);
          td_EDIT.setAttribute("valign", "middle");
          td_EDIT.setAttribute("class", "element_toolbar");
          td_EDIT.appendChild(img_EDIT);

          var img_DUBLICATE = document.createElement("img");
          img_DUBLICATE.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/dublicate.png");
          img_DUBLICATE.setAttribute("title", "Dublicate the field");
          img_DUBLICATE.style.cssText = "margin:2px;cursor:pointer";
          img_DUBLICATE.setAttribute("onclick", 'dublicate("' + i + '")');

          var td_DUBLICATE = document.createElement("td");
          td_DUBLICATE.setAttribute("id", "dublicate_" + i);
          td_DUBLICATE.setAttribute("valign", "middle");
          td_DUBLICATE.setAttribute("class", "element_toolbar");
          td_DUBLICATE.appendChild(img_DUBLICATE);

          var img_PAGEUP = document.createElement("img");
          img_PAGEUP.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/page_up.png");
          img_PAGEUP.setAttribute("title", "Move the field to the upper page");
          img_PAGEUP.style.cssText = "margin:2px;cursor:pointer";
          img_PAGEUP.setAttribute("onclick", 'page_up("' + i + '")');

          var td_PAGEDOWN = document.createElement("td");
          td_PAGEDOWN.setAttribute("id", "page_up_" + i);
          td_PAGEDOWN.setAttribute("valign", "middle");
          td_PAGEDOWN.setAttribute("class", "element_toolbar");
          td_PAGEDOWN.appendChild(img_PAGEUP);

          var img_PAGEDOWN = document.createElement("img");
          img_PAGEDOWN.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/page_down.png");
          img_PAGEDOWN.setAttribute("title", "Move the field to the lower page");
          img_PAGEDOWN.style.cssText = "margin:2px;cursor:pointer";
          img_PAGEDOWN.setAttribute("onclick", 'page_down("' + i + '")');

          var td_PAGEUP = document.createElement("td");
          td_PAGEUP.setAttribute("id", "dublicate_" + i);
          td_PAGEUP.setAttribute("valign", "middle");
          td_PAGEUP.setAttribute("class", "element_toolbar");
          td_PAGEUP.appendChild(img_PAGEDOWN);
          if (document.getElementById('edit_for_label_position_top')) {
            if (document.getElementById('edit_for_label_position_top').checked) {
              var add1 = document.getElementById(i + '_label_sectionform_id_temp');
              var add2 = document.getElementById(i + '_element_sectionform_id_temp');
              add2.className += " toolbar_padding";
              tr.appendChild(add1);
              tr.appendChild(add2);
            }
            else {
              var td1 = document.createElement('td');
              td1.setAttribute("colspan", "2");
              td1.setAttribute("id", i + '_label_and_element_sectionform_id_temp');
              td1.setAttribute('class', 'toolbar_padding');
              var add = document.getElementById(i + '_elemet_tableform_id_temp');
              td1.appendChild(add);
              tr.appendChild(td1);
            }
          }
          else {
            var td1 = document.createElement('td');
            td1.setAttribute("colspan", "2");
            td1.setAttribute('class', 'toolbar_padding');
            td1.setAttribute("id", i + '_label_and_element_sectionform_id_temp');
            var add = document.getElementById(i + '_elemet_tableform_id_temp');
            td1.appendChild(add);
            tr.appendChild(td1);
          }
          tr.appendChild(td_X);
          tr.appendChild(td_LEFT);
          tr.appendChild(td_UP);
          tr.appendChild(td_DOWN);
          tr.appendChild(td_RIGHT);
          tr.appendChild(td_EDIT);
          if (type != "type_captcha" && type != "type_recaptcha") {
            tr.appendChild(td_DUBLICATE);
          }
          else {
            td_DUBLICATE.removeChild(img_DUBLICATE);
            tr.appendChild(td_DUBLICATE);
          }
          tr.appendChild(td_PAGEDOWN);
          tr.appendChild(td_PAGEUP);
          j = 2;
          form_maker_close_window();
          call(i, key);
        }
      }
      else {
        i = gen;
        in_lab = false;
        labels_array = new Array();
        for (w = 0; w < gen; w++) {
          if (document.getElementById(w + '_element_labelform_id_temp')) {
            labels_array.push(document.getElementById(w + '_element_labelform_id_temp').innerHTML);
          }
        }
        for (t = 0; t < labels_array.length; t++) {
          if (document.getElementById(i + '_element_labelform_id_temp').innerHTML == labels_array[t]) {
            in_lab = true;
            break;
          }
        }
        if (in_lab) {
          alert(Drupal.t('Sorry, the labels must be unique.'));
          return
        }
        else {
          if (type == "type_address") {
            gen = gen + 6;
          }
          else {
            gen++;
          }
          l = document.getElementById('form_id_tempform_view' + form_view).childNodes.length;
          if (document.getElementById('form_id_tempform_view' + form_view).firstChild.nodeType == 3) {
            table = document.getElementById('form_id_tempform_view' + form_view).childNodes[l - 3].childNodes[1].childNodes[1].childNodes[1];
          }
          else {
            table = document.getElementById('form_id_tempform_view' + form_view).childNodes[l - 2].firstChild.firstChild.firstChild;
          }
          var tr = document.createElement('tr');
          tr.setAttribute("id", i);
          tr.setAttribute("type", type);


          var select_ = document.getElementById('sel_el_pos');
          var option = document.createElement('option');
          option.setAttribute("id", i + "_sel_el_pos");
          option.setAttribute("value", i);
          option.innerHTML = document.getElementById(i + '_element_labelform_id_temp').innerHTML;

          var img_X = document.createElement("img");
          img_X.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/delete_el.png");
          img_X.setAttribute("height", "17");
          img_X.setAttribute("title", "Remove the field");
          img_X.style.cssText = "cursor:pointer; margin:2px";
          img_X.setAttribute("onclick", 'remove_row("' + i + '")');

          var td_X = document.createElement("td");
          td_X.setAttribute("id", "X_" + i);
          td_X.setAttribute("valign", "middle");
          td_X.setAttribute("align", "right");
          td_X.setAttribute("class", "element_toolbar");
          td_X.appendChild(img_X);

          var img_UP = document.createElement("img");
          img_UP.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/up.png");
          img_UP.setAttribute("title", "Move the field up");
          img_UP.style.cssText = "cursor:pointer";
          img_UP.setAttribute("onclick", 'up_row("' + i + '")');

          var td_UP = document.createElement("td");
          td_UP.setAttribute("id", "up_" + i);
          td_UP.setAttribute("valign", "middle");
          td_UP.setAttribute("class", "element_toolbar");
          td_UP.appendChild(img_UP);

          var img_DOWN = document.createElement("img");
          img_DOWN.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/down.png");
          img_DOWN.setAttribute("title", "Move the field down");
          img_DOWN.style.cssText = "margin:2px;cursor:pointer";
          img_DOWN.setAttribute("onclick", 'down_row("' + i + '")');

          var td_DOWN = document.createElement("td");
          td_DOWN.setAttribute("id", "down_" + i);
          td_DOWN.setAttribute("valign", "middle");
          td_DOWN.setAttribute("class", "element_toolbar");
          td_DOWN.appendChild(img_DOWN);

          var img_RIGHT = document.createElement("img");
          img_RIGHT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/right.png");
          img_RIGHT.setAttribute("title", "Move the field to the right");
          img_RIGHT.style.cssText = "cursor:pointer";
          img_RIGHT.setAttribute("onclick", 'right_row("' + i + '")');

          var td_RIGHT = document.createElement("td");
          td_RIGHT.setAttribute("id", "right_" + i);
          td_RIGHT.setAttribute("valign", "middle");
          td_RIGHT.setAttribute("class", "element_toolbar");
          td_RIGHT.appendChild(img_RIGHT);

          var img_LEFT = document.createElement("img");
          img_LEFT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/left.png");
          img_LEFT.setAttribute("title", "Move the field to the left");
          img_LEFT.style.cssText = "margin:2px;cursor:pointer";
          img_LEFT.setAttribute("onclick", 'left_row("' + i + '")');

          var td_LEFT = document.createElement("td");
          td_LEFT.setAttribute("id", "left_" + i);
          td_LEFT.setAttribute("valign", "middle");
          td_LEFT.setAttribute("class", "element_toolbar");
          td_LEFT.appendChild(img_LEFT);

          var img_EDIT = document.createElement("img");
          img_EDIT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/edit.png");
          img_EDIT.setAttribute("title", "Edit the field");
          img_EDIT.style.cssText = "margin:2px;cursor:pointer";
          img_EDIT.setAttribute("onclick", 'edit("' + i + '")');

          var td_EDIT = document.createElement("td");
          td_EDIT.setAttribute("id", "edit_" + i);
          td_EDIT.setAttribute("valign", "middle");
          td_EDIT.setAttribute("class", "element_toolbar");
          td_EDIT.appendChild(img_EDIT);

          var img_DUBLICATE = document.createElement("img");
          img_DUBLICATE.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/dublicate.png");
          img_DUBLICATE.setAttribute("title", "Dublicate the field");
          img_DUBLICATE.style.cssText = "margin:2px;cursor:pointer";
          img_DUBLICATE.setAttribute("onclick", 'dublicate("' + i + '")');

          var td_DUBLICATE = document.createElement("td");
          td_DUBLICATE.setAttribute("id", "dublicate_" + i);
          td_DUBLICATE.setAttribute("valign", "middle");
          td_DUBLICATE.setAttribute("class", "element_toolbar");
          td_DUBLICATE.appendChild(img_DUBLICATE);

          var img_PAGEUP = document.createElement("img");
          img_PAGEUP.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/page_up.png");
          img_PAGEUP.setAttribute("title", "Move the field to the upper page");
          img_PAGEUP.style.cssText = "margin:2px;cursor:pointer";
          img_PAGEUP.setAttribute("onclick", 'page_up("' + i + '")');

          var td_PAGEDOWN = document.createElement("td");
          td_PAGEDOWN.setAttribute("id", "page_up_" + i);
          td_PAGEDOWN.setAttribute("valign", "middle");
          td_PAGEDOWN.setAttribute("class", "element_toolbar");
          td_PAGEDOWN.appendChild(img_PAGEUP);

          var img_PAGEDOWN = document.createElement("img");
          img_PAGEDOWN.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/page_down.png");
          img_PAGEDOWN.setAttribute("title", "Move the field to the lower page");
          img_PAGEDOWN.style.cssText = "margin:2px;cursor:pointer";
          img_PAGEDOWN.setAttribute("onclick", 'page_down("' + i + '")');

          var td_PAGEUP = document.createElement("td");
          td_PAGEUP.setAttribute("id", "dublicate_" + i);
          td_PAGEUP.setAttribute("valign", "middle");
          td_PAGEUP.setAttribute("class", "element_toolbar");
          td_PAGEUP.appendChild(img_PAGEDOWN);
          if (document.getElementById('edit_for_label_position_top'))
            if (document.getElementById('edit_for_label_position_top').checked) {
              var add1 = document.getElementById(i + '_label_sectionform_id_temp');
              var add2 = document.getElementById(i + '_element_sectionform_id_temp');
              add2.className += " toolbar_padding";
              tr.appendChild(add1);
              tr.appendChild(add2);
            }
            else {
              var td1 = document.createElement('td');
              td1.setAttribute("colspan", "2");
              td1.setAttribute("id", i + '_label_and_element_sectionform_id_temp');
              td1.setAttribute('class', 'toolbar_padding');
              var add = document.getElementById(i + '_elemet_tableform_id_temp');
              td1.appendChild(add);
              tr.appendChild(td1);
            }
          else {
            var td1 = document.createElement('td');
            td1.setAttribute("colspan", "2");
            td1.setAttribute("id", i + '_label_and_element_sectionform_id_temp');
            td1.setAttribute('class', 'toolbar_padding');
            var add = document.getElementById(i + '_elemet_tableform_id_temp');
            td1.appendChild(add);
            tr.appendChild(td1);
          }
          tr.appendChild(td_X);
          tr.appendChild(td_LEFT);
          tr.appendChild(td_UP);
          tr.appendChild(td_DOWN);
          tr.appendChild(td_RIGHT);
          tr.appendChild(td_EDIT);
          if (type != "type_captcha" && type != "type_recaptcha") {
            tr.appendChild(td_DUBLICATE);
          }
          else {
            td_DUBLICATE.removeChild(img_DUBLICATE);
            tr.appendChild(td_DUBLICATE);
          }
          tr.appendChild(td_PAGEUP);
          tr.appendChild(td_PAGEDOWN);
          if (document.getElementById('pos_end').checked) {
            table.appendChild(tr);
          }
          if (document.getElementById('pos_begin').checked) {
            table.insertBefore(tr, table.firstChild);
          }
          if (document.getElementById('pos_before').checked) {
            beforeTr = document.getElementById(document.getElementById('sel_el_pos').value);
            table = beforeTr.parentNode;
            beforeOption = document.getElementById(document.getElementById('sel_el_pos').value + '_sel_el_pos');
            table.insertBefore(tr, beforeTr);
            select_.insertBefore(option, beforeOption);
          }
          j = 2;
          form_maker_close_window();
          call(i, key);
        }
      }
    }
    else {
      alert(Drupal.t("The field label is required."));
      return;
    }
  }
  else {
    alert(Drupal.t("Please select an element to add."));
  }
}

function edit(id) {
  enable2();
  document.getElementById('editing_id').value = id;
  type = document.getElementById(id).getAttribute('type');
  k = 0;
  w_choices = new Array();
  w_choices_checked = new Array();
  w_choices_disabled = new Array();
  w_allow_other_num = 0;
  t = 0;
  if (document.getElementById(id + '_element_labelform_id_temp').innerHTML) {
    w_field_label = document.getElementById(id + '_element_labelform_id_temp').innerHTML;
  }
  if (document.getElementById(id + '_label_and_element_sectionform_id_temp')) {
    w_field_label_pos = "top";
  }
  else {
    w_field_label_pos = "left";
  }

  if (document.getElementById(id + "_elementform_id_temp")) {
    s = document.getElementById(id + "_elementform_id_temp").style.width;
    w_size = s.substring(0, s.length - 2);
  }

  if (document.getElementById(id + "_requiredform_id_temp")) {
    w_required = document.getElementById(id + "_requiredform_id_temp").value;
  }
  if (document.getElementById(id + "_uniqueform_id_temp")) {
    w_unique = document.getElementById(id + "_uniqueform_id_temp").value;
  }
  if (document.getElementById(id + '_label_sectionform_id_temp')) {
    w_class = document.getElementById(id + '_label_sectionform_id_temp').getAttribute("class");
    if (!w_class) {
      w_class = "";
    }
  }
  switch (type) {
    case 'type_editor': {
      w_editor = document.getElementById(id + "_element_sectionform_id_temp").innerHTML;
      type_editor(id, w_editor);
      break;

    }
    case 'type_section_break': {
      w_editor = document.getElementById(id + "_element_sectionform_id_temp").innerHTML;
      type_section_break(id, w_editor);
      break;

    }
    case 'type_text': {
      w_first_val = document.getElementById(id + "_elementform_id_temp").value;
      w_title = document.getElementById(id + "_elementform_id_temp").title;
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_text(id, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_required, w_unique, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_number': {
      w_first_val = document.getElementById(id + "_elementform_id_temp").value;
      w_title = document.getElementById(id + "_elementform_id_temp").title;
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_number(id, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_required, w_unique, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_password': {
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_password(id, w_field_label, w_field_label_pos, w_size, w_required, w_unique, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_textarea': {
      w_first_val = document.getElementById(id + "_elementform_id_temp").value;
      w_title = document.getElementById(id + "_elementform_id_temp").title;
      s = document.getElementById(id + "_elementform_id_temp").style.height;
      w_size_h = s.substring(0, s.length - 2);
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_textarea(id, w_field_label, w_field_label_pos, w_size, w_size_h, w_first_val, w_title, w_required, w_unique, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_phone': {
      w_first_val = [document.getElementById(id + "_element_firstform_id_temp").value, document.getElementById(id + "_element_lastform_id_temp").value];
      w_title = [document.getElementById(id + "_element_firstform_id_temp").title, document.getElementById(id + "_element_lastform_id_temp").title];
      s = document.getElementById(id + "_element_lastform_id_temp").style.width;
      w_size = s.substring(0, s.length - 2);
      atrs = return_attributes(id + '_element_firstform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_phone(id, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_required, w_unique, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_name': {
      if (document.getElementById(id + '_element_middleform_id_temp')) {
        w_name_format = "extended";
      }
      else {
        w_name_format = "normal";
      }
      w_first_val = [document.getElementById(id + "_element_firstform_id_temp").value, document.getElementById(id + "_element_lastform_id_temp").value];
      w_title = [document.getElementById(id + "_element_firstform_id_temp").title, document.getElementById(id + "_element_lastform_id_temp").title];
      s = document.getElementById(id + "_element_firstform_id_temp").style.width;
      w_size = s.substring(0, s.length - 2);
      atrs = return_attributes(id + '_element_firstform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_name(id, w_field_label, w_field_label_pos, w_first_val, w_title, w_size, w_name_format, w_required, w_unique, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_address': {
      s = document.getElementById(id + "_div_address").style.width;
      w_size = s.substring(0, s.length - 2);
      atrs = return_attributes(id + '_street1form_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_address(id, w_field_label, w_field_label_pos, w_size, w_required, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_submitter_mail': {
      w_first_val = document.getElementById(id + "_elementform_id_temp").value;
      w_title = document.getElementById(id + "_elementform_id_temp").title;
      w_send = document.getElementById(id + "_sendform_id_temp").value;
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_submitter_mail(id, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_send, w_required, w_unique, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_checkbox': {
      if (document.getElementById(id + '_hor')) {
        w_flow = "hor";
      }
      else {
        w_flow = "ver";
      }
      w_randomize = document.getElementById(id + "_randomizeform_id_temp").value;
      w_allow_other = document.getElementById(id + "_allow_otherform_id_temp").value;
      v = 0;
      for (k = 0; k < 100; k++) {
        if (document.getElementById(id + "_elementform_id_temp" + k)) {
          if (document.getElementById(id + "_elementform_id_temp" + k).getAttribute('other')) {
            if (document.getElementById(id + "_elementform_id_temp" + k).getAttribute('other') == '1') {
              w_allow_other_num = t;
            }
          }
          w_choices[t] = document.getElementById(id + "_elementform_id_temp" + k).value;
          w_choices_checked[t] = document.getElementById(id + "_elementform_id_temp" + k).checked;
          t++;
          v = k;
        }
      }
      atrs = return_attributes(id + '_elementform_id_temp' + v);
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_checkbox(id, w_field_label, w_field_label_pos, w_flow, w_choices, w_choices_checked, w_required, w_randomize, w_allow_other, w_allow_other_num, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_radio': {
      if (document.getElementById(id + '_hor')) {
        w_flow = "hor";
      }
      else {
        w_flow = "ver";
      }
      w_randomize = document.getElementById(id + "_randomizeform_id_temp").value;
      w_allow_other = document.getElementById(id + "_allow_otherform_id_temp").value;
      v = 0;
      for (k = 0; k < 100; k++) {
        if (document.getElementById(id + "_elementform_id_temp" + k)) {
          if (document.getElementById(id + "_elementform_id_temp" + k).getAttribute('other')) {
            if (document.getElementById(id + "_elementform_id_temp" + k).getAttribute('other') == '1') {
              w_allow_other_num = t;
            }
          }
          w_choices[t] = document.getElementById(id + "_elementform_id_temp" + k).value;
          w_choices_checked[t] = document.getElementById(id + "_elementform_id_temp" + k).checked;
          t++;
          v = k;
        }
      }
      atrs = return_attributes(id + '_elementform_id_temp' + v);
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_radio(id, w_field_label, w_field_label_pos, w_flow, w_choices, w_choices_checked, w_required, w_randomize, w_allow_other, w_allow_other_num, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_time': {
      atrs = return_attributes(id + '_hhform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      w_hh = document.getElementById(id + '_hhform_id_temp').value;
      w_mm = document.getElementById(id + '_mmform_id_temp').value;
      if (document.getElementById(id + '_ssform_id_temp')) {
        w_ss = document.getElementById(id + '_ssform_id_temp').value;
        w_sec = "1";
      }
      else {
        w_ss = "";
        w_sec = "0";
      }
      if (document.getElementById(id + '_am_pm_select')) {
        w_am_pm = document.getElementById(id + '_am_pmform_id_temp').value;
        w_time_type = "12";
      }
      else {
        w_am_pm = 0;
        w_time_type = "24";
      }
      type_time(id, w_field_label, w_field_label_pos, w_time_type, w_am_pm, w_sec, w_hh, w_mm, w_ss, w_required, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_date': {
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      w_date = document.getElementById(id + '_elementform_id_temp').value;
      w_format = document.getElementById(id + '_buttonform_id_temp').getAttribute("format");
      w_but_val = document.getElementById(id + '_buttonform_id_temp').value;
      type_date(id, w_field_label, w_field_label_pos, w_date, w_required, w_class, w_format, w_but_val, w_attr_name, w_attr_value);
      break;

    }
    case 'type_date_fields': {
      atrs = return_attributes(id + '_dayform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      w_day = document.getElementById(id + '_dayform_id_temp').value;
      w_month = document.getElementById(id + '_monthform_id_temp').value;
      w_year = document.getElementById(id + '_yearform_id_temp').value;
      w_day_type = document.getElementById(id + '_dayform_id_temp').tagName;
      w_month_type = document.getElementById(id + '_monthform_id_temp').tagName;
      w_year_type = document.getElementById(id + '_yearform_id_temp').tagName;
      w_day_label = document.getElementById(id + '_day_label').innerHTML;
      w_month_label = document.getElementById(id + '_month_label').innerHTML;
      w_year_label = document.getElementById(id + '_year_label').innerHTML;

      s = document.getElementById(id + '_dayform_id_temp').style.width;
      w_day_size = s.substring(0, s.length - 2);

      s = document.getElementById(id + '_monthform_id_temp').style.width;
      w_month_size = s.substring(0, s.length - 2);

      s = document.getElementById(id + '_yearform_id_temp').style.width;
      w_year_size = s.substring(0, s.length - 2);

      if (w_year_type == 'SELECT') {
        w_from = document.getElementById(id + '_yearform_id_temp').getAttribute('from');
        w_to = document.getElementById(id + '_yearform_id_temp').getAttribute('to');
      }
      else {
        var current_date = new Date();
        w_from = '1901';
        w_to = current_date.getFullYear();
      }
      w_divider = document.getElementById(id + '_separator1').innerHTML;
      type_date_fields(id, w_field_label, w_field_label_pos, w_day, w_month, w_year, w_day_type, w_month_type, w_year_type, w_day_label, w_month_label, w_year_label, w_day_size, w_month_size, w_year_size, w_required, w_class, w_from, w_to, w_divider, w_attr_name, w_attr_value);
      break;

    }
    case 'type_own_select': {
      for (k = 0; k < 100; k++) {
        if (document.getElementById(id + "_option" + k)) {
          w_choices[t] = document.getElementById(id + "_option" + k).innerHTML;
          w_choices_checked[t] = document.getElementById(id + "_option" + k).selected;
          if (document.getElementById(id + "_option" + k).value == "") {
            w_choices_disabled[t] = true;
          }
          else {
            w_choices_disabled[t] = false;
          }
          t++;
        }
      }
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_own_select(id, w_field_label, w_field_label_pos, w_size, w_choices, w_choices_checked, w_required, w_class, w_attr_name, w_attr_value, w_choices_disabled);
      break;

    }
    case 'type_country': {
      w_countries = [];
      select_ = document.getElementById(id + '_elementform_id_temp');
      n = select_.childNodes.length;
      for (i = 0; i < n; i++) {
        w_countries.push(select_.childNodes[i].value);
      }
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_country(id, w_field_label, w_countries, w_field_label_pos, w_size, w_required, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_file_upload': {
      w_destination = document.getElementById(id + "_destination").value.replace("***destinationverj" + id + "***", "").replace("***destinationskizb" + id + "***", "");
      w_extension = document.getElementById(id + "_extension").value.replace("***extensionverj" + id + "***", "").replace("***extensionskizb" + id + "***", "");
      w_max_size = document.getElementById(id + "_max_size").value.replace("***max_sizeverj" + id + "***", "").replace("***max_sizeskizb" + id + "***", "");
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_file_upload(id, w_field_label, w_field_label_pos, w_destination, w_extension, w_max_size, w_required, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_captcha': {
      w_digit = document.getElementById("_wd_captchaform_id_temp").getAttribute("digit");
      atrs = return_attributes('_wd_captchaform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_captcha(id, w_field_label, w_field_label_pos, w_digit, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_recaptcha': {
      w_public = document.getElementById("wd_recaptchaform_id_temp").getAttribute("public_key");
      w_private = document.getElementById("wd_recaptchaform_id_temp").getAttribute("private_key");
      w_theme = document.getElementById("wd_recaptchaform_id_temp").getAttribute("theme");
      atrs = return_attributes('wd_recaptchaform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_recaptcha(id, w_field_label, w_field_label_pos, w_public, w_private, w_theme, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_map': {
      w_lat = [];
      w_long = [];
      w_info = [];
      w_center_x = document.getElementById(id + "_elementform_id_temp").getAttribute("center_x");
      w_center_y = document.getElementById(id + "_elementform_id_temp").getAttribute("center_y");
      w_zoom = document.getElementById(id + "_elementform_id_temp").getAttribute("zoom");
      w_width = parseInt(document.getElementById(id + "_elementform_id_temp").style.width);
      w_height = parseInt(document.getElementById(id + "_elementform_id_temp").style.height);
      for (j = 0; j <= 20; j++) {
        if (document.getElementById(id + "_elementform_id_temp").getAttribute("lat" + j)) {
          w_lat.push(document.getElementById(id + "_elementform_id_temp").getAttribute("lat" + j));
          w_long.push(document.getElementById(id + "_elementform_id_temp").getAttribute("long" + j));
          w_info.push(document.getElementById(id + "_elementform_id_temp").getAttribute("info" + j));
        }
      }
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_map(id, w_center_x, w_center_y, w_long, w_lat, w_zoom, w_width, w_height, w_class, w_info, w_attr_name, w_attr_value);
      break;

    }
    case 'type_mark_map': {
      w_info = document.getElementById(id + "_elementform_id_temp").getAttribute("info0");
      w_long = document.getElementById(id + "_elementform_id_temp").getAttribute("long0");
      w_lat = document.getElementById(id + "_elementform_id_temp").getAttribute("lat0");
      w_zoom = document.getElementById(id + "_elementform_id_temp").getAttribute("zoom");
      w_width = parseInt(document.getElementById(id + "_elementform_id_temp").style.width);
      w_height = parseInt(document.getElementById(id + "_elementform_id_temp").style.height);
      w_center_x = document.getElementById(id + "_elementform_id_temp").getAttribute("center_x");
      w_center_y = document.getElementById(id + "_elementform_id_temp").getAttribute("center_y");
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_mark_map(id, w_field_label, w_field_label_pos, w_center_x, w_center_y, w_long, w_lat, w_zoom, w_width, w_height, w_class, w_info, w_attr_name, w_attr_value);
      break;

    }
    case 'type_submit_reset': {
      atrs = return_attributes(id + '_element_submitform_id_temp');
      w_act = !(document.getElementById(id + "_element_resetform_id_temp").style.display == "none");
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      w_submit_title = document.getElementById(id + "_element_submitform_id_temp").value;
      w_reset_title = document.getElementById(id + "_element_resetform_id_temp").value;
      type_submit_reset(id, w_submit_title, w_reset_title, w_class, w_act, w_attr_name, w_attr_value);
      break;

    }

    case 'type_button': {
      w_title = new Array();
      w_func = new Array();
      t = 0;
      v = 0;
      for (k = 0; k < 100; k++) {
        if (document.getElementById(id + "_elementform_id_temp" + k)) {
          w_title[t] = document.getElementById(id + "_elementform_id_temp" + k).value;
          w_func[t] = document.getElementById(id + "_elementform_id_temp" + k).getAttribute("onclick");
          t++;
          v = k;
        }
      }
      atrs = return_attributes(id + '_elementform_id_temp' + v);
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_button(id, w_title, w_func, w_class, w_attr_name, w_attr_value);
      break;

    }
    case 'type_hidden':     {
      w_value = document.getElementById(id + "_elementform_id_temp").value;
      w_name = document.getElementById(id + "_elementform_id_temp").name;

      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_hidden(id, w_name, w_value, w_attr_name, w_attr_value);
      break;
    }

  }
  var pos = document.getElementsByName("el_pos");
  pos[0].setAttribute("disabled", "disabled");
  pos[1].setAttribute("disabled", "disabled");
  pos[2].setAttribute("disabled", "disabled");
  var sel_el_pos = document.getElementById("sel_el_pos");
  sel_el_pos.setAttribute("disabled", "disabled");
}

function edit_page_break(id) {
  enable2();
  document.getElementById('editing_id').value = id;
  form_view_element = document.getElementById('form_id_tempform_view' + id);
  page_title = form_view_element.getAttribute('page_title');
  if (form_view_element.getAttribute('next_title')) {
    next_title = form_view_element.getAttribute('next_title');
    next_type = form_view_element.getAttribute('next_type');
    next_class = form_view_element.getAttribute('next_class');
    next_checkable = form_view_element.getAttribute('next_checkable');
    previous_title = form_view_element.getAttribute('previous_title');
    previous_type = form_view_element.getAttribute('previous_type');
    previous_class = form_view_element.getAttribute('previous_class');
    previous_checkable = form_view_element.getAttribute('previous_checkable');
    w_title = [ next_title, previous_title];
    w_type = [next_type, previous_type];
    w_class = [next_class, previous_class];
    w_check = [next_checkable, previous_checkable];
  }
  else {
    w_title = [ "Next", "Previous"];
    w_type = ["button", "button"];
    w_class = ["", ""];
    w_check = ['true', 'true'];
  }
  w_attr_name = [];
  w_attr_value = [];
  type_page_break(id, page_title, w_title, w_type, w_class, w_check, w_attr_name, w_attr_value);
}

function generate_page_bar() {
  el_page_navigation();
  add(0);
}

function remove_add_(id) {
  attr_name = new Array();
  attr_value = new Array();
  var input = document.getElementById(id);
  atr = input.attributes;
  for (v = 0; v < 30; v++) {
    if (atr[v]) {
      if (atr[v].name.indexOf("add_") == 0) {
        attr_name.push(atr[v].name.replace('add_', ''));
        attr_value.push(atr[v].value);
        input.removeAttribute(atr[v].name);
        v--;
      }
    }
  }
  for (v = 0; v < attr_name.length; v++) {
    input.setAttribute(attr_name[v], attr_value[v])
  }
}

function call(i, key) {
  if (key == 0) {
    edit(i);
    add('1');
  }
}

function make_page_percentage_front() {
  destroyChildren(document.getElementById("pages"));
  show_title = document.getElementById('el_show_title_input').checked;

  var div_parent = document.createElement('div');
  div_parent.setAttribute("class", "page_percentage_deactive");

  var div = document.createElement('div');
  div.setAttribute("id", "div_percentage");
  div.setAttribute("class", "page_percentage_active");
  var b = document.createElement('b');
  b.style.margin = '3px 7px 3px 3px';
  div.appendChild(b);
  k = 0;
  cur_page_title = '';
  for (j = 1; j <= form_view_max; j++) {
    if (document.getElementById('form_id_tempform_view' + j)) {
      if (document.getElementById('form_id_tempform_view' + j).getAttribute('page_title')) {
        w_pages = document.getElementById('form_id_tempform_view' + j).getAttribute('page_title');
      }
      else {
        w_pages = "";
      }
      k++;

      if (j == form_view) {
        if (show_title) {
          var cur_page_title = document.createElement('span');
          if (k == 1) {
            cur_page_title.style.paddingLeft = "30px";
          }
          else {
            cur_page_title.style.paddingLeft = "5px";
          }
          cur_page_title.innerHTML = w_pages;
        }
        page_number = k;
      }
    }
  }
  b.innerHTML = Math.round(((page_number - 1) / k) * 100) + '%';
  div.style.width = ((page_number - 1) / k) * 100 + '%';
  div_parent.appendChild(div);
  if (cur_page_title) {
    div_parent.appendChild(cur_page_title);
  }
  document.getElementById("pages").appendChild(div_parent);


}

function make_page_none_front() {
  document.getElementById("pages").innerHTML = "--------------------------------------------<br> NO PAGE BAR <br>--------------------------------------------";
}


function refresh_pages_without_deleting(id) {
  var form_view_elemet = document.getElementById("form_id_tempform_view" + id);
  var form_view_count = 0;
  for (i = 1; i <= 30; i++) {
    if (document.getElementById('form_id_tempform_view' + i)) {
      form_view_count++;
    }
  }
  if (form_view_count == 1) {
    form_view_elemet.innerHTML = '';

    tbody = form_view_elemet;

    tr = document.createElement('tr');
    tr.setAttribute('class', 'wdform_tr1');
    tr.setAttribute('style', 'background-color:rgba(0,0,0,0)');
    td = document.createElement('td');
    td.setAttribute('class', 'wdform_td1 form_maker_table');


    tr_page_nav = document.createElement('tr');
    tr_page_nav.setAttribute('valign', 'top');
    tr_page_nav.setAttribute('class', 'wdform_footer');
    tr_page_nav.setAttribute('style', 'background-color:rgba(0,0,0,0);');

    td_page_nav = document.createElement('td');
    td_page_nav.setAttribute('colspan', '100');

    table_min_page_nav = document.createElement('table');
    table_min_page_nav.setAttribute('width', '100%');
    table_min_page_nav.setAttribute('style', "border:none;");

    tbody_min_page_nav = document.createElement('tbody');
    tr_min_page_nav = document.createElement('tr');
    tr_min_page_nav.setAttribute('id', 'form_id_temppage_nav' + form_view);

    table_min = document.createElement('table');
    table_min.setAttribute('class', 'wdform_table2 form_maker_table');
    tbody_min = document.createElement('tbody');
    tbody_min.setAttribute('class', 'wdform_tbody2 form_maker_table');

    table_min.appendChild(tbody_min);
    td.appendChild(table_min);
    tr.appendChild(td);
    tbody_min_page_nav.appendChild(tr_min_page_nav);
    table_min_page_nav.appendChild(tbody_min_page_nav);
    td_page_nav.appendChild(table_min_page_nav);
    tr_page_nav.appendChild(td_page_nav);
    tbody.appendChild(tr);
    tbody.appendChild(tr_page_nav);
    return;
  }
  table = form_view_elemet.parentNode.previousSibling;
  while (table) {
    if (table.tagName == "TABLE") {
      break;
    }
    else {
      table = table.previousSibling;
    }
  }

  if (!table) {
    table = form_view_elemet.parentNode.nextSibling;
    while (table) {
      if (table.tagName == "TABLE") {
        break;
      }
      else {
        table = table.nextSibling;
      }
    }
  }
  i = gen;
  gen++;

  var tr = document.createElement('tr');
  tr.setAttribute("id", i);
  tr.setAttribute("class", "wdform_tr_section_break");
  tr.setAttribute("type", "type_section_break");

  var select_ = document.getElementById('sel_el_pos');
  var option = document.createElement('option');
  option.setAttribute("id", i + "_sel_el_pos");
  option.setAttribute("value", i);
  option.innerHTML = "custom_" + i;

  table_form_view = table.firstChild;

  var img_X = document.createElement("img");
  img_X.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/delete_el.png");
  img_X.setAttribute("height", "17");
  img_X.setAttribute("title", "Remove the field");
  img_X.style.cssText = "cursor:pointer; margin:2px";
  img_X.setAttribute("onclick", 'remove_section_break("' + i + '")');

  var td_X = document.createElement("td");
  td_X.setAttribute("id", "X_" + i);
  td_X.setAttribute("valign", "middle");
  td_X.setAttribute("align", "right");
  td_X.setAttribute("class", "element_toolbar");
  td_X.appendChild(img_X);

  var img_EDIT = document.createElement("img");
  img_EDIT.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/edit.png");
  img_EDIT.setAttribute("title", "Edit the field");
  img_EDIT.style.cssText = "margin:2px;cursor:pointer";
  img_EDIT.setAttribute("onclick", 'edit("' + i + '")');

  var td_EDIT = document.createElement("td");
  td_EDIT.setAttribute("id", "edit_" + i);
  td_EDIT.setAttribute("valign", "middle");
  td_EDIT.setAttribute("class", "element_toolbar");
  td_EDIT.appendChild(img_EDIT);

  var img_DUBLICATE = document.createElement("img");
  img_DUBLICATE.setAttribute("src", "" + Drupal.settings.form_maker.get_module_path + "/images/dublicate.png");
  img_DUBLICATE.setAttribute("title", "Dublicate the field");
  img_DUBLICATE.style.cssText = "margin:2px;cursor:pointer";
  img_DUBLICATE.setAttribute("onclick", 'dublicate("' + i + '")');

  var td_DUBLICATE = document.createElement("td");
  td_DUBLICATE.setAttribute("id", "dublicate_" + i);
  td_DUBLICATE.setAttribute("valign", "middle");
  td_DUBLICATE.setAttribute("class", "element_toolbar");
  td_DUBLICATE.appendChild(img_DUBLICATE);
  var in_editor = document.createElement("td");
  in_editor.setAttribute("id", i + "_element_sectionform_id_temp");
  in_editor.setAttribute("align", 'left');
  in_editor.setAttribute("valign", "top");
  in_editor.setAttribute("colspan", "100");
  in_editor.setAttribute('class', 'toolbar_padding');
  in_editor.innerHTML = "<hr/>";
  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = "custom_" + i;
  label.style.cssText = 'display:none';
  td_EDIT.appendChild(label);
  tr.appendChild(in_editor);
  tr.appendChild(td_X);
  tr.appendChild(td_EDIT);
  tr.appendChild(td_DUBLICATE);
  beforeTr = table_form_view.lastChild;
  // If deleted page had no fields.
  if (tr.childNodes.length > 0) {
    table_form_view.insertBefore(tr, beforeTr);
  }
  while (form_view_elemet.childNodes[1]) {
    beforeTr = table_form_view.lastChild;
    table_form_view.insertBefore(form_view_elemet.firstChild, beforeTr);
  }
  form_view_table = form_view_elemet.parentNode;
  document.getElementById("take").removeChild(form_view_table);
  refresh_pages(id);
}


function make_page_steps_front() {
  destroyChildren(document.getElementById("pages"));
  show_title = document.getElementById('el_show_title_input').checked;
  k = 0;
  for (j = 1; j <= form_view_max; j++) {
    if (document.getElementById('form_id_tempform_view' + j)) {
      if (document.getElementById('form_id_tempform_view' + j).getAttribute('page_title')) {
        w_pages = document.getElementById('form_id_tempform_view' + j).getAttribute('page_title');
      }
      else {
        w_pages = "";
      }
      k++;
      page_number = document.createElement('span');
      page_number.setAttribute('id', 'page_' + j);
      page_number.setAttribute('onClick', 'generate_page_nav("' + j + '")');
      if (j == form_view) {
        page_number.setAttribute('class', "page_active");
      }
      else {
        page_number.setAttribute('class', "page_deactive");
      }
      if (show_title) {
        page_number.innerHTML = w_pages;
      }
      else {
        page_number.innerHTML = k;
      }
      document.getElementById("pages").appendChild(page_number);
    }
  }
}

function remove_page(id) {
  if (confirm(Drupal.t('Do you want to delete the page?'))) {
    refresh_pages_without_deleting(id);
  }
}

function remove_page_all(id) {
  if (confirm(Drupal.t('Do you want to delete the all fields in this page?'))) {
    var form_view_elemet = document.getElementById("form_id_tempform_view" + id);
    var form_view_count = 0;
    for (i = 1; i <= 30; i++) {
      if (document.getElementById('form_id_tempform_view' + i)) {
        form_view_count++;
      }
    }
    if (form_view_count == 1) {

      form_view_elemet.innerHTML = '';

      tbody = form_view_elemet;

      tr = document.createElement('tr');
      tr.setAttribute('class', 'wdform_tr1');
      tr.setAttribute('style', 'background-color:rgba(0,0,0,0)');
      td = document.createElement('td');
      td.setAttribute('class', 'wdform_td1 form_maker_table');

      tr_page_nav = document.createElement('tr');
      tr_page_nav.setAttribute('valign', 'top');
      tr_page_nav.setAttribute('class', 'wdform_footer');
      tr_page_nav.setAttribute('style', 'background-color:rgba(0,0,0,0);');

      td_page_nav = document.createElement('td');
      td_page_nav.setAttribute('colspan', '100');

      table_min_page_nav = document.createElement('table');
      table_min_page_nav.setAttribute('width', '100%');
      table_min_page_nav.setAttribute('style', "border:none;");

      tbody_min_page_nav = document.createElement('tbody');
      tr_min_page_nav = document.createElement('tr');
      tr_min_page_nav.setAttribute('id', 'form_id_temppage_nav' + form_view);

      table_min = document.createElement('table');
      table_min.setAttribute('class', 'wdform_table2 form_maker_table');
      tbody_min = document.createElement('tbody');
      tbody_min.setAttribute('class', 'wdform_tbody2 form_maker_table');

      table_min.appendChild(tbody_min);
      td.appendChild(table_min);
      tr.appendChild(td);
      tbody_min_page_nav.appendChild(tr_min_page_nav);
      table_min_page_nav.appendChild(tbody_min_page_nav);
      td_page_nav.appendChild(table_min_page_nav);
      tr_page_nav.appendChild(td_page_nav);
      tbody.appendChild(tr);
      tbody.appendChild(tr_page_nav);
      return;
    }
    form_view_table = form_view_elemet.parentNode;
    document.getElementById("take").removeChild(form_view_table);
    refresh_pages(id);
  }
}

function refresh_pages(id) {
  temp = 1;
  form_view_count = 0;
  destroyChildren(document.getElementById("pages"));
  for (i = 1; i <= 30; i++) {
    if (document.getElementById('form_id_tempform_view' + i)) {
      form_view_count++;
    }
  }
  if (form_view_count > 1) {
    for (i = 1; i <= 30; i++) {
      if (document.getElementById('form_id_tempform_view' + i)) {
        page_number = document.createElement('span');
        page_number.setAttribute('id', 'page_' + i);
        page_number.setAttribute('class', 'page_deactive');
        page_number.innerHTML = (temp);
        temp++;
        document.getElementById("pages").appendChild(page_number);
      }
    }
  }
  else {
    destroyChildren(document.getElementById("edit_page_navigation"));
    for (i = 1; i <= 30; i++) {
      if (document.getElementById('form_id_tempform_view' + i)) {
        document.getElementById('form_id_tempform_view' + i).parentNode.style.borderWidth = "0px";
        document.getElementById('form_id_tempform_view' + i).style.display = "block";
        document.getElementById("form_id_temppage_nav" + i).innerHTML = "";
        form_maker_remove_spaces(document.getElementById("form_id_tempform_view_img" + i));
        document.getElementById("form_id_tempform_view_img" + i).childNodes[0].childNodes[0].removeAttribute("width", "100%");
        document.getElementById("form_id_tempform_view_img" + i).style.backgroundColor = "";
        document.getElementById("form_id_tempform_view_img" + i).childNodes[0].childNodes[0].style.display = "none";
        document.getElementById("show_page_img_" + i).src = '' + Drupal.settings.form_maker.get_module_path + '/images/minus.png';
        form_view = i;
        return;
      }
    }
  }
  for (i = parseInt(id) + 1; i <= 30; i++) {
    if (document.getElementById('form_id_tempform_view' + i)) {
      generate_page_nav(i);
      return;
    }
  }
  for (i = parseInt(id) - 1; i > 0; i--) {
    if (document.getElementById('form_id_tempform_view' + i)) {
      generate_page_nav(i);
      return;
    }
  }
}

function show_or_hide(id) {
  if (!jQuery("#form_id_tempform_view" + id).is(":visible")) {
    show_form_view(id);
  }
  else {
    hide_form_view(id);
  }
}

function show_form_view(id) {
  document.getElementById("form_id_tempform_view_img" + id).childNodes[0].childNodes[0].removeAttribute("width", "100%");
  document.getElementById("form_id_tempform_view_img" + id).style.backgroundColor = "";
  document.getElementById("form_id_tempform_view_img" + id).childNodes[0].childNodes[0].style.display = "none";
  document.getElementById("show_page_img_" + id).src = '' + Drupal.settings.form_maker.get_module_path + '/images/minus.png';
  jQuery("#form_id_tempform_view" + id).show('medium');
}

function hide_form_view(id) {
  form_maker_remove_spaces(document.getElementById("form_id_tempform_view_img" + id));
  jQuery("#form_id_tempform_view" + id).hide('medium', function () {
    document.getElementById("form_id_tempform_view_img" + id).childNodes[0].childNodes[0].setAttribute("width", "100%");
    document.getElementById("form_id_tempform_view_img" + id).childNodes[0].childNodes[0].innerHTML = document.getElementById("form_id_tempform_view" + id).getAttribute('page_title');
    document.getElementById("form_id_tempform_view_img" + id).childNodes[0].childNodes[0].removeAttribute('style');
    document.getElementById("form_id_tempform_view_img" + id).style.backgroundColor = "rgba(0, 0, 0, 0)";
    document.getElementById("show_page_img_" + id).src = '' + Drupal.settings.form_maker.get_module_path + '/images/plus.png';
  });

}

function generate_buttons(id) {
  form_view_elemet = document.getElementById("form_id_tempform_view" + id);
  if (form_view_elemet.parentNode.previousSibling) {
    if (form_view_elemet.parentNode.previousSibling.tagName == "TABLE") {
      table = true;
    }
    else if (form_view_elemet.parentNode.previousSibling.previousSibling)
      if (form_view_elemet.parentNode.previousSibling.previousSibling.tagName == "TABLE") {
        table = true;
      }
      else {
        table = false;
      } 
    else {
      table = false;
    }
    if (table) {
      if (form_view_elemet.getAttribute('previous_title')) {
        previous_title = form_view_elemet.getAttribute('previous_title');
        previous_type = form_view_elemet.getAttribute('previous_type');
        previous_class = form_view_elemet.getAttribute('previous_class');
      }
      else {
        previous_title = "Previous";
        previous_type = "button";
        previous_class = "";
      }
      next_or_previous = "previous";
      previous = make_pagebreak_button(next_or_previous, previous_title, previous_type, previous_class, id);
      var td = document.createElement("td");
      td.setAttribute("valign", "middle");
      td.setAttribute("align", "left");
      td.appendChild(previous);
      page_nav.appendChild(td);
    }
  }
  var td = document.createElement("td");
  td.setAttribute("id", "page_numbersform_id_temp" + id);
  td.setAttribute("width", "100%");
  td.setAttribute("valign", "middle");
  td.setAttribute("align", "center");
  page_nav.appendChild(td);
  if (form_view_elemet.parentNode.nextSibling) {
    if (form_view_elemet.parentNode.nextSibling.tagName == "TABLE") {
      table = true;
    }
    else if (form_view_elemet.parentNode.nextSibling.nextSibling) {
      if (form_view_elemet.parentNode.nextSibling.nextSibling.tagName == "TABLE") {
        table = true;
      }
      else {
        table = false;
      }
    }
    else {
      table = false;
    }
    if (table) {
      if (form_view_elemet.getAttribute('previous_title')) {
        next_title = form_view_elemet.getAttribute('next_title');
        next_type = form_view_elemet.getAttribute('next_type');
        next_class = form_view_elemet.getAttribute('next_class');
      }
      else {
        next_title = "Next";
        next_type = "button";
        next_class = "";
      }
      next_or_previous = "next";
      next = make_pagebreak_button(next_or_previous, next_title, next_type, next_class, id);
      var td = document.createElement("td");
      td.setAttribute("valign", "middle");
      td.setAttribute("align", "right");
      td.appendChild(next);
      page_nav.appendChild(td);
    }
  }
}

function generate_page_nav(id) {
  form_view = id;
  document.getElementById('form_id_tempform_view' + id).parentNode.style.borderWidth = "1px";
  for (t = 1; t <= form_view_max; t++) {
    if (document.getElementById('form_id_tempform_view' + t)) {
      page_nav = document.getElementById("form_id_temppage_nav" + t);
      destroyChildren(page_nav);
      generate_buttons(t);
    }
  }
  generate_page_bar();
  refresh_page_numbers();
}

function remove_section_break(id) {
  var tr = document.getElementById(id);
  is3 = false;
  if (tr.nextSibling.nodeType == 3) {
    move = tr.nextSibling.nextSibling.firstChild;
    to = tr.previousSibling.previousSibling.firstChild;
    is3 = true;
  }
  else {
    move = tr.nextSibling.firstChild;
    to = tr.previousSibling.firstChild;
  }
  l = move.childNodes.length;
  for (k = 0; k < l; k++) {
    if (to.childNodes[k]) {
      while (move.childNodes[k].firstChild.firstChild) {
        to.childNodes[k].firstChild.appendChild(move.childNodes[k].firstChild.firstChild);
      }
    }
    else {
      to.appendChild(move.childNodes[k]);
    }
  }
  if (is3) {
    tr.parentNode.removeChild(tr.nextSibling);
    tr.parentNode.removeChild(tr.nextSibling);
  }
  else {
    tr.parentNode.removeChild(tr.nextSibling);
  }
  tr.parentNode.removeChild(tr);
}

function remove_row(id) {
  var tr = document.getElementById(id);
  tr.parentNode.removeChild(tr);
}

function type_map(i, w_center_x, w_center_y, w_long, w_lat, w_zoom, w_width, w_height, w_class, w_info, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_map";
  delete_last_child();
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";


  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  edit_main_td7.setAttribute("colspan", "4");
  edit_main_td7.setAttribute("id", "markers");

  var center1 = document.createElement('p');
  center1.setAttribute("id", "center1");
  center1.setAttribute("style", "margin:0;");
  center1.innerHTML = "Drag the marker to change marker position.";

  var el_label_location = document.createElement('label');
  el_label_location.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_location.innerHTML = "Location";

  var el_img_add_marker = document.createElement('img');
  el_img_add_marker.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_img_add_marker.style.cssText = "cursor:pointer";
  el_img_add_marker.setAttribute("onClick", "add_marker('" + i + "', -1)");

  var el_label_map_size = document.createElement('label');
  el_label_map_size.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_map_size.innerHTML = "Map size";

  var el_map_width = document.createElement('input');
  el_map_width.setAttribute("type", "text");
  el_map_width.setAttribute("value", w_width);
  el_map_width.style.cssText = "margin-left:18px";
  el_map_width.setAttribute("onKeyPress", "return check_isnum(event)");
  el_map_width.setAttribute("onKeyUp", "change_w_style('" + i + "_elementform_id_temp', this.value);");

  Width = document.createTextNode("Width");

  var el_map_height = document.createElement('input');
  el_map_height.setAttribute("type", "text");
  el_map_height.setAttribute("value", w_height);
  el_map_height.style.cssText = "margin-left:15px";
  el_map_height.setAttribute("onKeyPress", "return check_isnum(event)");
  el_map_height.setAttribute("onKeyUp", "change_h_style('" + i + "_elementform_id_temp', this.value);");

  Height = document.createTextNode("Height");

  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_text')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_text')");

    var el_attr_value = document.createElement('input');
    el_attr_value.setAttribute("type", "text");
    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_text')");
    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_text')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var t = document.getElementById('edit_table');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');
  var br7 = document.createElement('br');
  var br8 = document.createElement('br');
  var br9 = document.createElement('br');
  var br10 = document.createElement('br');
  var br11 = document.createElement('br');

  edit_main_td2.appendChild(el_label_location);
  edit_main_td2.appendChild(center1);
  edit_main_td2.appendChild(el_img_add_marker);
  edit_main_td2.setAttribute("colspan", "2");

  edit_main_td3.appendChild(el_label_map_size);
  edit_main_td3_1.appendChild(Width);
  edit_main_td3_1.appendChild(el_map_width);
  edit_main_td3_1.appendChild(br5);
  edit_main_td3_1.appendChild(Height);
  edit_main_td3_1.appendChild(el_map_height);

  edit_main_td5.appendChild(el_style_label);
  edit_main_td5_1.appendChild(el_style_textarea);

  edit_main_td6.appendChild(el_attr_label);
  edit_main_td6.appendChild(el_attr_add);
  edit_main_td6.appendChild(br6);
  edit_main_td6.appendChild(el_attr_table);
  edit_main_td6.setAttribute("colspan", "2");

  edit_main_tr2.appendChild(edit_main_td2);

  edit_main_tr7.appendChild(edit_main_td7);

  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);

  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr7);

  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr6);
  edit_div.appendChild(edit_main_table);

  t.appendChild(edit_div);
  element = 'div';
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_map");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");

  var adding = document.createElement('div');
  adding.setAttribute("id", i + "_elementform_id_temp");
  adding.style.cssText = "width:" + w_width + "px; height: " + w_height + "px";
  adding.setAttribute("zoom", w_zoom);
  adding.setAttribute("center_x", w_center_x);
  adding.setAttribute("center_y", w_center_y);

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = "map_" + i;
  label.style.cssText = 'display:none';

  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');

  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'middle');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");
  td1.style.cssText = 'display:none';

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'middle');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');

  var main_td = document.getElementById('show_table');

  td1.appendChild(label);
  td2.appendChild(adding_type);
  td2.appendChild(adding);

  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);

  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  change_class(w_class, i);
  refresh_attr(i, 'type_text');
  if_gmap_init(i);
  n = w_long.length;
  for (j = 0; j < n; j++) {
    add_marker(i, j, w_long[j], w_lat[j], w_info[j]);
  }
}

function add_marker(id, i, w_long, w_lat, w_info) {
  edit_main_td7 = document.getElementById('markers');
  if (i == -1) {
    if (edit_main_td7.lastChild) {
      i = parseInt(edit_main_td7.lastChild.getAttribute("idi")) + 1;
    }
    else {
      i = 0;
    }
    w_long = null;
    w_lat = null;
    w_info = '';

  }
  var table_marker = document.createElement('table');
  table_marker.setAttribute("width", "100%");
  table_marker.setAttribute("border", "0");
  table_marker.setAttribute("id", "marker_opt" + i);
  table_marker.setAttribute("idi", i);

  var tr_marker = document.createElement('tr');
  var tr_hr = document.createElement('tr');

  var td_marker = document.createElement('td');
  var td_X = document.createElement('td');
  var td_hr = document.createElement('td');
  td_hr.setAttribute("colspan", "3");
  tr_hr.appendChild(td_hr);
  tr_marker.appendChild(td_marker);
  tr_marker.appendChild(td_X);
  table_marker.appendChild(tr_marker);

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');

  var hr = document.createElement('hr');
  hr.setAttribute("id", "br" + i);

  var el_info_textarea = document.createElement('textarea');
  el_info_textarea.setAttribute("id", "info" + i);
  el_info_textarea.setAttribute("rows", "3");
  el_info_textarea.setAttribute("value", w_info);
  el_info_textarea.style.cssText = "width:200px;";
  el_info_textarea.setAttribute("onKeyUp", "change_info(this.value,'" + id + "','" + i + "')");
  el_info_textarea.innerHTML = w_info;

  var Marker_info = document.createElement('label');
  Marker_info.style.cssText = " font-size: 11px; vertical-align:top; margin-right:43px";
  Marker_info.innerHTML = "Marker Info";

  var el_map_address = document.createElement('input');
  el_map_address.setAttribute("id", "addrval" + i);
  el_map_address.setAttribute("type", "text");
  el_map_address.setAttribute("value", "");
  el_map_address.setAttribute("size", "40");
  el_map_address.setAttribute("onchange", "changeAddress(" + id + "," + i + ")");

  var Address = document.createElement('label');
  Address.style.cssText = " font-size: 11px; vertical-align:top; margin-right:55px";
  Address.innerHTML = "Address";

  var el_map_longitude = document.createElement('input');
  el_map_longitude.setAttribute("id", "longval" + i);
  el_map_longitude.setAttribute("type", "text");
  el_map_longitude.setAttribute("value", w_long);
  el_map_longitude.setAttribute("size", "10");
  el_map_longitude.setAttribute("onkeyup", "update_position(" + id + ", " + i + ");");
  var Longitude = document.createElement('label');
  Longitude.style.cssText = " font-size: 11px; vertical-align:top; margin-right:50px";
  Longitude.innerHTML = "Longitude";

  var el_map_latitude = document.createElement('input');
  el_map_latitude.setAttribute("id", "latval" + i);
  el_map_latitude.setAttribute("type", "text");
  el_map_latitude.setAttribute("value", w_lat);
  el_map_latitude.setAttribute("size", "10");
  el_map_latitude.setAttribute("onkeyup", "update_position(" + id + ", " + i + ");");

  var Latitude = document.createElement('label');
  Latitude.style.cssText = " font-size: 11px; vertical-align:top; margin-right:59px";
  Latitude.innerHTML = "Latitude";
  var el_choices_remove = document.createElement('img');
  el_choices_remove.setAttribute("id", "el_button" + i + "_remove");
  el_choices_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
  el_choices_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
  el_choices_remove.setAttribute("align", 'top');
  el_choices_remove.setAttribute("onClick", "remove_map(" + id + "," + i + ")");

  td_hr.appendChild(hr);
  td_marker.appendChild(Address);
  td_marker.appendChild(el_map_address);
  td_marker.appendChild(br1);
  td_marker.appendChild(Longitude);
  td_marker.appendChild(el_map_longitude);
  td_marker.appendChild(br2);
  td_marker.appendChild(Latitude);
  td_marker.appendChild(el_map_latitude);
  td_marker.appendChild(br3);
  td_marker.appendChild(Marker_info);
  td_marker.appendChild(el_info_textarea);
  td_X.appendChild(el_choices_remove);
  edit_main_td7.appendChild(table_marker);
  var adding = document.getElementById(id + "_elementform_id_temp")
  adding.setAttribute("long" + i, w_long);
  adding.setAttribute("lat" + i, w_lat);
  adding.setAttribute("info" + i, w_info);
  add_marker_on_map(id, i, w_long, w_lat, w_info, true);
}

function remove_map(id, i) {
  table = document.getElementById('marker_opt' + i);
  table.parentNode.removeChild(table);
  map = document.getElementById(id + "_elementform_id_temp");
  map.removeAttribute("long" + i);
  map.removeAttribute("lat" + i);
  map.removeAttribute("info" + i);
  reomve_marker(id, i);
}


function type_mark_map(i, w_field_label, w_field_label_pos, w_center_x, w_center_y, w_long, w_lat, w_zoom, w_width, w_height, w_class, w_info, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_mark_map";
  delete_last_child();

  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_tr8 = document.createElement('tr');
  edit_main_tr8.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var edit_main_td8 = document.createElement('td');
  edit_main_td8.style.cssText = "padding-top:10px";
  var edit_main_td8_1 = document.createElement('td');
  edit_main_td8_1.style.cssText = "padding-top:10px";

  var el_label_label = document.createElement('label');
  el_label_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px; width:130px;";
  el_label_label.innerHTML = "Field label";

  var el_label_textarea = document.createElement('textarea');
  el_label_textarea.setAttribute("id", "edit_for_label");
  el_label_textarea.setAttribute("rows", "4");
  el_label_textarea.style.cssText = "width:200px;";
  el_label_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_labelform_id_temp', this.value)");
  el_label_textarea.innerHTML = w_field_label;

  var el_label_position_label = document.createElement('label');
  el_label_position_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_position_label.innerHTML = "Field label position";

  var el_label_position1 = document.createElement('input');
  el_label_position1.setAttribute("id", "edit_for_label_position_top");
  el_label_position1.setAttribute("type", "radio");
  el_label_position1.setAttribute("value", "left");

  el_label_position1.setAttribute("name", "edit_for_label_position");
  el_label_position1.setAttribute("onchange", "label_left(" + i + ")");
  el_label_position1.setAttribute("checked", "checked");
  Left = document.createTextNode("Left");

  var el_label_position2 = document.createElement('input');
  el_label_position2.setAttribute("id", "edit_for_label_position_left");
  el_label_position2.setAttribute("type", "radio");
  el_label_position2.setAttribute("value", "top");

  el_label_position2.setAttribute("name", "edit_for_label_position");
  el_label_position2.setAttribute("onchange", "label_top(" + i + ")");
  Top = document.createTextNode("Top");

  if (w_field_label_pos == "top") {
    el_label_position2.setAttribute("checked", "checked");
  }
  else {
    el_label_position1.setAttribute("checked", "checked");
  }
  var center1 = document.createElement('p');
  center1.innerHTML = "Drag the marker to change default marker position.";


  var el_label_location = document.createElement('label');
  el_label_location.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_location.innerHTML = "Default Location";

  var el_map_address = document.createElement('input');
  el_map_address.setAttribute("id", "addrval0");
  el_map_address.setAttribute("type", "text");
  el_map_address.setAttribute("value", "");

  el_map_address.setAttribute("size", "40");
  el_map_address.setAttribute("onchange", "changeAddress(" + i + ",0)");
  Address = document.createTextNode("Address");
  var el_map_longitude = document.createElement('input');
  el_map_longitude.setAttribute("id", "longval0");
  el_map_longitude.setAttribute("type", "text");
  el_map_longitude.setAttribute("value", w_long);
  el_map_longitude.setAttribute("size", "10");
  el_map_longitude.setAttribute("onkeyup", "update_position(" + i + ", 0);");
  Longitude = document.createTextNode("Longitude");

  var el_map_latitude = document.createElement('input');
  el_map_latitude.setAttribute("id", "latval0");
  el_map_latitude.setAttribute("type", "text");
  el_map_latitude.setAttribute("value", w_lat);
  el_map_latitude.setAttribute("size", "10");
  el_map_latitude.setAttribute("onkeyup", "update_position(" + i + ", 0);");
  Latitude = document.createTextNode("Latitude");

  var el_label_map_size = document.createElement('label');
  el_label_map_size.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_label_map_size.innerHTML = "Map size";

  var el_map_width = document.createElement('input');
  el_map_width.setAttribute("type", "text");
  el_map_width.setAttribute("value", w_width);
  el_map_width.style.cssText = "margin-left:18px";
  el_map_width.setAttribute("onKeyPress", "return check_isnum(event)");
  el_map_width.setAttribute("onKeyUp", "change_w_style('" + i + "_elementform_id_temp', this.value);");

  Width = document.createTextNode("Width");

  var el_map_height = document.createElement('input');
  el_map_height.setAttribute("type", "text");
  el_map_height.setAttribute("value", w_height);
  el_map_height.style.cssText = "margin-left:15px";
  el_map_height.setAttribute("onKeyPress", "return check_isnum(event)");
  el_map_height.setAttribute("onKeyUp", "change_h_style('" + i + "_elementform_id_temp', this.value);");
  Height = document.createTextNode("Height");
  var el_info_label = document.createElement('label');
  el_info_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_info_label.innerHTML = "Marker Info";

  var el_info_textarea = document.createElement('textarea');
  el_info_textarea.setAttribute("id", "info0");
  el_info_textarea.setAttribute("rows", "3");
  el_info_textarea.setAttribute("value", w_class);
  el_info_textarea.style.cssText = "width:200px; margin-left:2px";
  el_info_textarea.setAttribute("onKeyUp", "change_info(this.value,'" + i + "','" + 0 + "')");
  el_info_textarea.innerHTML = w_info;

  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");

  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_text')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_text')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");

    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_text')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_text')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var t = document.getElementById('edit_table');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');
  var br7 = document.createElement('br');
  var br8 = document.createElement('br');
  var br9 = document.createElement('br');
  var br10 = document.createElement('br');

  edit_main_td1.appendChild(el_label_label);
  edit_main_td1_1.appendChild(el_label_textarea);

  edit_main_td8.appendChild(el_label_position_label);
  edit_main_td8_1.appendChild(el_label_position1);
  edit_main_td8_1.appendChild(Left);
  edit_main_td8_1.appendChild(br10);
  edit_main_td8_1.appendChild(el_label_position2);
  edit_main_td8_1.appendChild(Top);


  edit_main_td2.appendChild(el_label_location);
  edit_main_td2.appendChild(center1);
  edit_main_td2.setAttribute("colspan", "2");

  edit_main_td7.appendChild(Address);
  edit_main_td7.appendChild(br9);
  edit_main_td7.appendChild(Longitude);
  edit_main_td7.appendChild(br8);
  edit_main_td7.appendChild(Latitude);
  edit_main_td7_1.appendChild(el_map_address);
  edit_main_td7_1.appendChild(br7);

  edit_main_td7_1.appendChild(el_map_longitude);
  edit_main_td7_1.appendChild(br2);
  edit_main_td7_1.appendChild(el_map_latitude);

  edit_main_td2.appendChild(br3);

  edit_main_td3.appendChild(el_label_map_size);
  edit_main_td3_1.appendChild(Width);
  edit_main_td3_1.appendChild(el_map_width);
  edit_main_td3_1.appendChild(br5);
  edit_main_td3_1.appendChild(Height);
  edit_main_td3_1.appendChild(el_map_height);

  edit_main_td4.appendChild(el_info_label);
  edit_main_td4_1.appendChild(el_info_textarea);

  edit_main_td5.appendChild(el_style_label);
  edit_main_td5_1.appendChild(el_style_textarea);

  edit_main_td6.appendChild(el_attr_label);
  edit_main_td6.appendChild(el_attr_add);
  edit_main_td6.appendChild(br6);
  edit_main_td6.appendChild(el_attr_table);
  edit_main_td6.setAttribute("colspan", "2");

  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr8.appendChild(edit_main_td8);
  edit_main_tr8.appendChild(edit_main_td8_1);
  edit_main_tr2.appendChild(edit_main_td2);

  edit_main_tr7.appendChild(edit_main_td7);
  edit_main_tr7.appendChild(edit_main_td7_1);

  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);
  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr8);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr7);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr5);
  edit_main_table.appendChild(edit_main_tr6);
  edit_div.appendChild(edit_main_table);

  t.appendChild(edit_div);

  element = 'div';
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_mark_map");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");

  var adding = document.createElement('div');
  adding.setAttribute("id", i + "_elementform_id_temp");
  adding.setAttribute("long0", w_long);
  adding.setAttribute("lat0", w_lat);
  adding.setAttribute("zoom", w_zoom);
  adding.style.cssText = "width:" + w_width + "px; height: " + w_height + "px";
  adding.setAttribute("info0", w_info);
  adding.setAttribute("center_x", w_center_x);
  adding.setAttribute("center_y", w_center_y);

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.innerHTML = w_field_label;
  label.setAttribute("class", "label");


  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');

  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'top');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'top');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');

  var main_td = document.getElementById('show_table');

  td1.appendChild(label);
  td2.appendChild(adding_type);
  td2.appendChild(adding);

  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);

  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  if (w_field_label_pos == "top") {
    label_top(i);
  }
  change_class(w_class, i);
  refresh_attr(i, 'type_text');
  if_gmap_init(i);
  add_marker_on_map(i, 0, w_long, w_lat, w_info, true);

}

function type_page_navigation(w_type, w_show_title, w_show_numbers, w_attr_name, w_attr_value) {
  enable2();
  document.getElementById("element_type").value = "type_page_navigation";
  delete_last_child();

  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border:0px;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";
  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";
  edit_main_td6.setAttribute("colspan", "2");

  var el_pagination_opt_label = document.createElement('label');
  el_pagination_opt_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_pagination_opt_label.innerHTML = "Pagination Options";

  var el_pagination_steps = document.createElement('input');
  el_pagination_steps.setAttribute("id", "el_pagination_steps");
  el_pagination_steps.setAttribute("type", "radio");
  el_pagination_steps.setAttribute("name", "el_pagination");
  el_pagination_steps.setAttribute("value", "steps");
  el_pagination_steps.style.cssText = "margin-left:20px;  padding:0; border-width: 1px";
  el_pagination_steps.setAttribute("onclick", "pagination_type('steps')");
  Steps = document.createTextNode("Steps");

  var el_pagination_percentage = document.createElement('input');
  el_pagination_percentage.setAttribute("id", "el_pagination_percentage");
  el_pagination_percentage.setAttribute("type", "radio");
  el_pagination_percentage.setAttribute("name", "el_pagination");
  el_pagination_percentage.setAttribute("value", "percentage");
  el_pagination_percentage.style.cssText = "margin-left:20px;  padding:0; border-width: 1px";
  el_pagination_percentage.setAttribute("onclick", "pagination_type('percentage')");
  Percentage = document.createTextNode("Percentage");

  var el_pagination_none = document.createElement('input');
  el_pagination_none.setAttribute("id", "el_pagination_none");
  el_pagination_none.setAttribute("type", "radio");
  el_pagination_none.setAttribute("name", "el_pagination");
  el_pagination_none.setAttribute("value", "none");
  el_pagination_none.style.cssText = "margin-left:20px;  padding:0; border-width: 1px";
  el_pagination_none.setAttribute("onclick", "pagination_type('none')");
  No_Context = document.createTextNode(" No Context");

  if (w_type == 'steps') {
    el_pagination_steps.setAttribute("checked", "checked");
  }
  else if (w_type == 'percentage') {
    el_pagination_percentage.setAttribute("checked", "checked");
  }
  else {
    el_pagination_none.setAttribute("checked", "checked");
  }
  var el_show_title_label = document.createElement('label');
  el_show_title_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_show_title_label.innerHTML = "Show Page Titles in Progress Bar";

  var el_show_title_input = document.createElement('input');
  el_show_title_input.setAttribute("id", "el_show_title_input");
  el_show_title_input.setAttribute("type", "checkbox");
  el_show_title_input.setAttribute("onClick", "show_title_pagebreak();");

  if (w_show_title) {
    el_show_title_input.setAttribute("checked", "checked");
  }
  var el_show_numbers_label = document.createElement('label');
  el_show_numbers_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_show_numbers_label.innerHTML = "Show Page Numbers in Footer";

  var el_show_numbers_input = document.createElement('input');
  el_show_numbers_input.setAttribute("id", "el_show_numbers_input");
  el_show_numbers_input.setAttribute("type", "checkbox");
  el_show_numbers_input.setAttribute("onClick", "show_numbers_pagebreak();");

  if (w_show_numbers) {
    el_show_numbers_input.setAttribute("checked", "checked");
  }
  var el_pagination_class_label = document.createElement('label');
  el_pagination_class_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_pagination_class_label.innerHTML = "Pagination Class name";

  var el_pagination_class_input = document.createElement('input');
  el_pagination_class_input.setAttribute("id", "next_element_style");
  el_pagination_class_input.setAttribute("type", "text");
  el_pagination_class_input.setAttribute("value", "");
  el_pagination_class_input.style.cssText = "width:100px; margin-left:20px";
  el_pagination_class_input.setAttribute("onChange", "change_pagebreak_class(this.value, 'next')");

  var el_page_titles_label = document.createElement('label');
  el_page_titles_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_page_titles_label.innerHTML = "Pages Title";

  edit_main_td6.appendChild(el_page_titles_label);
  w_pages = [];
  k = 0;
  for (j = 1; j <= form_view_max; j++) {
    if (document.getElementById('form_id_tempform_view' + j)) {
      k++;
      var br_temp = document.createElement('br');
      var el_page_title_input = document.createElement('input');
      el_page_title_input.setAttribute("type", "text");
      el_page_title_input.style.cssText = "width:100px";
      if (document.getElementById('form_id_tempform_view' + j).getAttribute('page_title') == null) {
        el_page_title_input.setAttribute("value", "Untitled Page");
      }
      else {
        el_page_title_input.setAttribute("value", document.getElementById('form_id_tempform_view' + j).getAttribute('page_title'));
      }
      el_page_title_input.setAttribute("id", "page_title_" + j);
      el_page_title_input.setAttribute("onKeyUp", "set_page_title(this.value,'" + j + "')");
      if (document.getElementById('form_id_tempform_view' + j).getAttribute('page_title')) {
        w_pages[j] = document.getElementById('form_id_tempform_view' + j).getAttribute('page_title');
      }
      else {
        w_pages[j] = "Untitled Page"
      }
      page_num = document.createTextNode(k + '. ');
      edit_main_td6.appendChild(br_temp);
      edit_main_td6.appendChild(page_num);
      edit_main_td6.appendChild(el_page_title_input);
    }
  }
  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr( 'type_checkbox')");

  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_checkbox')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");

    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_checkbox')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_checkbox')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var t = document.getElementById('edit_table');

  var hr = document.createElement('hr');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');

  edit_main_td1.appendChild(el_pagination_opt_label);
  edit_main_td1.appendChild(br4);
  edit_main_td1.appendChild(el_pagination_steps);
  edit_main_td1.appendChild(Steps);
  edit_main_td1.appendChild(br6);
  edit_main_td1.appendChild(el_pagination_percentage);
  edit_main_td1.appendChild(Percentage);
  edit_main_td1.appendChild(br5);
  edit_main_td1.appendChild(el_pagination_none);
  edit_main_td1.appendChild(No_Context);
  edit_main_td1.setAttribute("colspan", "2");

  edit_main_td3.appendChild(el_show_title_label);
  edit_main_td3_1.appendChild(el_show_title_input);

  edit_main_td4.appendChild(el_show_numbers_label);
  edit_main_td4_1.appendChild(el_show_numbers_input);

  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);
  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr6.appendChild(edit_main_td6);

  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_main_table.appendChild(edit_main_tr6);
  edit_div.appendChild(edit_main_table);

  t.appendChild(edit_div);

  var div = document.createElement('div');
  div.setAttribute("id", "main_div");
  var table = document.createElement('table');
  table.setAttribute("id", "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  table.setAttribute("width", "90%");

  var tr = document.createElement('tr');

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'top');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", "_element_sectionform_id_temp");
  td2.setAttribute("width", "100%");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var pages_div = document.createElement('div');
  pages_div.setAttribute("align", "left");
  pages_div.setAttribute("id", "pages_div");
  pages_div.style.width = '100%';
  pages_div.innerHTML = "";
  var numbers_div = document.createElement('div');
  numbers_div.setAttribute("align", "center");
  numbers_div.setAttribute("id", "numbers_div");
  numbers_div.style.width = '100%';
  numbers_div.style.paddingTop = '100px';
  numbers_div.innerHTML = "";
  td2.appendChild(pages_div);
  td2.appendChild(numbers_div);
  var main_td = document.getElementById('show_table');
  tr.appendChild(td2);
  table.appendChild(tr);
  div.appendChild(table);
  div.appendChild(br1);
  main_td.appendChild(div);
  if (w_type == 'steps') {
    make_page_steps(w_pages);
  }
  else if (w_type == 'percentage') {
    make_page_percentage(w_pages);
  }
  else {
    make_page_none(w_pages);
  }
  if (w_show_numbers) {
    show_numbers_pagebreak();
  }
}

function set_page_title(title, id) {
  document.getElementById("form_id_tempform_view" + id).setAttribute('page_title', title);
  show_title_pagebreak();
}

function show_numbers_pagebreak() {
  document.getElementById("numbers_div").innerHTML = "";
  if (document.getElementById("el_show_numbers_input").checked) {
    k = 0;
    for (j = 1; j <= form_view_max; j++) {
      if (document.getElementById('form_id_tempform_view' + j)) {
        k++;
        if (j == form_view) {
          page_number = k;
        }
      }
    }
    var cur = document.createElement('span');
    cur.setAttribute("class", "page_numbersform_id_temp");
    cur.innerHTML = page_number + '/' + k;
    document.getElementById("numbers_div").appendChild(cur);
  }
}

function refresh_page_numbers() {
  k = 0;
  if (document.getElementById('pages').getAttribute('show_numbers') == 'true') {
    for (j = 1; j <= form_view_max; j++) {
      if (document.getElementById('page_numbersform_id_temp' + j)) {
        k++;
      }
    }
  }
  cur_num = 0;
  for (j = 1; j <= form_view_max; j++) {
    if (document.getElementById('page_numbersform_id_temp' + j)) {
      cur_num++;
      document.getElementById("page_numbersform_id_temp" + j).innerHTML = '';
      if (document.getElementById('pages').getAttribute('show_numbers') == 'true') {
        var cur = document.createElement('span');
        cur.setAttribute("class", "page_numbersform_id_temp");
        cur.innerHTML = cur_num + '/' + k;
        document.getElementById("page_numbersform_id_temp" + j).appendChild(cur);
      }
    }
  }
}

function pagination_type(type) {
  document.getElementById("pages_div").innerHTML = "";
  w_pages = [];
  k = 0;
  for (j = 1; j <= form_view_max; j++) {
    if (document.getElementById('form_id_tempform_view' + j)) {
      k++;
      if (document.getElementById('form_id_tempform_view' + j).getAttribute('page_title')) {
        w_pages[j] = document.getElementById('form_id_tempform_view' + j).getAttribute('page_title');
      }
      else {
        w_pages[j] = "none";
      }
    }
  }
  if (type == 'steps') {
    make_page_steps(w_pages);
  }
  else if (type == 'percentage') {
    make_page_percentage(w_pages);
  }
  else {
    make_page_none();
  }
}

function show_title_pagebreak() {
  document.getElementById("pages_div").innerHTML = "";
  if (document.getElementById("el_pagination_steps").checked) {
    pagination_type('steps');
  }
  else if (document.getElementById("el_pagination_percentage").checked) {
    pagination_type('percentage');
  }
}

function make_page_steps(w_pages) {
  document.getElementById("pages_div").innerHTML = "";
  show_title = document.getElementById('el_show_title_input').checked;
  k = 0;
  for (j = 1; j <= form_view_max; j++) {
    if (w_pages[j]) {
      k++;
      if (w_pages[j] == "none") {
        w_pages[j] = '';
      }
      page_number = document.createElement('span');
      if (j == form_view) {
        page_number.setAttribute('class', "page_active");
      }
      else {
        page_number.setAttribute('class', "page_deactive");
      }
      if (show_title) {
        page_number.innerHTML = w_pages[j];
      }
      else {
        page_number.innerHTML = k;
      }
      document.getElementById("pages_div").appendChild(page_number);
    }
  }
}

function make_page_percentage(w_pages) {
  document.getElementById("pages_div").innerHTML = "";
  show_title = document.getElementById('el_show_title_input').checked;
  var div_parent = document.createElement('div');
  div_parent.setAttribute("class", "page_percentage_deactive");
  var div = document.createElement('div');
  div.setAttribute("id", "div_percentage");
  div.setAttribute("class", "page_percentage_active");
  var b = document.createElement('b');
  b.style.margin = '3px 7px 3px 3px';
  div.appendChild(b);
  k = 0;
  cur_page_title = '';
  for (j = 1; j <= form_view_max; j++) {
    if (w_pages[j]) {
      k++;
      if (w_pages[j] == "none") {
        w_pages[j] = '';
      }
      if (j == form_view) {
        if (show_title) {
          var cur_page_title = document.createElement('span');
          if (k == 1) {
            cur_page_title.style.paddingLeft = "30px";
          }
          else {
            cur_page_title.style.paddingLeft = "5px";
          }
          cur_page_title.innerHTML = w_pages[j];
        }
        page_number = k;
      }
    }
  }
  b.innerHTML = Math.round(((page_number - 1) / k) * 100) + '%';
  div.style.width = ((page_number - 1) / k) * 100 + '%';
  div_parent.appendChild(div);
  if (cur_page_title) {
    div_parent.appendChild(cur_page_title);
  }
  document.getElementById("pages_div").appendChild(div_parent);
}

function make_page_none() {
  document.getElementById("pages_div").innerHTML = "";
}

function el_page_navigation() {
  w_type = document.getElementById('pages').getAttribute('type');
  w_show_numbers = false;
  w_show_title = false;
  if (document.getElementById('pages').getAttribute('show_numbers') == "true") {
    w_show_numbers = true;
  }
  if (document.getElementById('pages').getAttribute('show_title') == "true") {
    w_show_title = true;
  }
  w_attr_name = [];
  w_attr_value = [];
  type_page_navigation(w_type, w_show_title, w_show_numbers, w_attr_name, w_attr_value);
}

function captcha_refresh(id) {
  srcArr = document.getElementById(id + "form_id_temp").src.split("&r=");
  document.getElementById(id + "form_id_temp").src = srcArr[0] + '&r=' + Math.floor(Math.random() * 100);
  document.getElementById(id + "_inputform_id_temp").value = '';
}

function up_row(id) {
  form = document.getElementById(id).parentNode;
  k = 0;
  while (form.childNodes[k]) {
    if (form.childNodes[k].getAttribute("id")) {
      if (id == form.childNodes[k].getAttribute("id")) {
        break;
      }
    }
    k++;
  }
  if (k != 0) {
    up = form.childNodes[k - 1];
    down = form.childNodes[k];
    form.removeChild(down);
    form.insertBefore(down, up);
    return;
  }
  form_view_elemet = form.parentNode.parentNode.parentNode.parentNode;
  current_tr = form.parentNode.parentNode.parentNode;
  if (current_tr.previousSibling) {
    if (current_tr.previousSibling.nodeType == 3) {
      if (current_tr.previousSibling.previousSibling.getAttribute('type')) {
        current_tr.previousSibling.previousSibling.previousSibling.previousSibling.firstChild.firstChild.firstChild.appendChild(document.getElementById(id));
        return;
      }
    }
    else {
      if (current_tr.previousSibling.getAttribute('type')) {
        current_tr.previousSibling.previousSibling.firstChild.firstChild.firstChild.appendChild(document.getElementById(id));
        return;
      }
    }
  }
  page_up(id);
}

function down_row(id) {
  form = document.getElementById(id).parentNode;
  l = form.childNodes.length;
  k = 0;
  while (form.childNodes[k]) {
    if (id == form.childNodes[k].id) {
      break;
    }
    k++;
  }
  if (k != l - 1) {
    up = form.childNodes[k];
    down = form.childNodes[k + 2];
    form.removeChild(up);
    if (!down) {
      down = null;
    }
    form.insertBefore(up, down);
    return;
  }
  form_view_elemet = form.parentNode.parentNode.parentNode.parentNode;
  current_tr = form.parentNode.parentNode.parentNode;
  if (current_tr.nextSibling.nodeType == 3) {
    if (current_tr.nextSibling.nextSibling.getAttribute('type')) {
      current_tr.nextSibling.nextSibling.nextSibling.nextSibling.firstChild.firstChild.firstChild.appendChild(document.getElementById(id));
      return;
    }
  }
  else {
    if (current_tr.nextSibling.getAttribute('type')) {
      current_tr.nextSibling.nextSibling.firstChild.firstChild.firstChild.appendChild(document.getElementById(id));
      return;
    }
  }
  page_down(id);
}

function right_row(id) {
  tr = document.getElementById(id);
  table_big = tr.parentNode.parentNode;
  td_big = tr.parentNode.parentNode.parentNode;
  if (table_big.nextSibling != null) {
    td_next = table_big.nextSibling;
    td_next.firstChild.appendChild(tr);
  }
  else {
    var new_td = document.createElement('td');
    new_td.setAttribute("valign", "top");
    var new_table = document.createElement('table');
    new_table.setAttribute("class", "wdform_table2 form_maker_table");
    var new_tbody = document.createElement('tbody');
    td_big.appendChild(new_table);
    new_table.appendChild(new_tbody);
    new_tbody.appendChild(tr);
  }
  if (table_big.firstChild.firstChild == null) {
    td_big.removeChild(table_big);
  }
}

function left_row(id) {
  tr = document.getElementById(id);
  td_big = tr.parentNode.parentNode.parentNode;
  table_big = tr.parentNode.parentNode;
  if (table_big.previousSibling != null) {
    table_previous = table_big.previousSibling;
    table_previous.firstChild.appendChild(tr);
    if (table_big.firstChild.firstChild == null) {
      td_big.removeChild(table_big);
    }
  }
}

function page_up(id) {
  form = document.getElementById(id).parentNode;
  form_view_elemet = form.parentNode.parentNode.parentNode.parentNode;
  form_view_elemet_copy = form.parentNode.parentNode.parentNode.parentNode;
  table = form_view_elemet.parentNode;
  while (table) {
    table = table.previousSibling;
    while (table) {
      if (table.tagName == "TABLE") {
        break;
      }
      else {
        table = table.previousSibling;
      }
    }
    if (!table) {
      alert(Drupal.t('Unable to move'));
      return;
    }
    form_maker_remove_spaces(table);
    if (jQuery(table.firstChild).is(":visible")) {
      break;
    }
  }
  n = table.firstChild.childNodes.length;
  table.firstChild.childNodes[n - 2].firstChild.firstChild.firstChild.appendChild(document.getElementById(id));
  refresh_pages(id);
}

function page_down(id) {
  form = document.getElementById(id).parentNode;
  form_view_elemet = form.parentNode.parentNode.parentNode.parentNode;
  form_view_elemet_copy = form.parentNode.parentNode.parentNode.parentNode;
  table = form_view_elemet.parentNode;
  while (table) {
    table = table.nextSibling;
    while (table) {
      if (table.tagName == "TABLE") {
        break;
      }
      else {
        table = table.nextSibling;
      }
    }
    if (!table) {
      alert(Drupal.t('Unable to move'));
      return;
    }
    if (jQuery(table.firstChild).is(":visible")) {
      break;
    }
  }
  n = table.firstChild.childNodes.length;
  table.firstChild.firstChild.firstChild.firstChild.firstChild.insertBefore(document.getElementById(id), table.firstChild.firstChild.firstChild.firstChild.firstChild.firstChild);
  refresh_pages(id);
}

function Disable() {
  select_ = document.getElementById('sel_el_pos');
  select_.setAttribute("disabled", "disabled");
  select_.innerHTML = "";
}

function all_labels() {
  var labels = new Array();
  for (var k = 1; k <= form_view_max; k++) {
    if (document.getElementById('form_id_tempform_view' + k)) {
      form_view_element = document.getElementById('form_id_tempform_view' + k);
      form_maker_remove_spaces(form_view_element);
      var n = form_view_element.childNodes.length - 2;
      for (var z = 0; z <= n; z++) {
        if (!form_view_element.childNodes[z].id) {
          var GLOBAL_tr = form_view_element.childNodes[z];
          for (var x = 0; x < GLOBAL_tr.firstChild.childNodes.length; x++) {
            var table = GLOBAL_tr.firstChild.childNodes[x];
            var tbody = table.firstChild;
            for (var y = 0; y < tbody.childNodes.length; y++) {
              var tr = tbody.childNodes[y];
              labels.push(document.getElementById(tr.id + '_element_labelform_id_temp').innerHTML);
            }
          }
        }
      }
    }
  }
  return labels;
}

/**
 * Remove witespaces from childNodes.
 */
function form_maker_remove_spaces(parent) {
  if (!parent) {
    parent = document;
  }
  var children = parent.childNodes;
  for (var i = children.length - 1; i >= 0; i--) {
    var child = children[i];
    if (child.nodeType == 3) {
      if (child.data.match(/^\s+$/)) {
        parent.removeChild(child);
      }
    }
    else {
      form_maker_remove_spaces(child);
    }
  }
}

function set_select(select_) {
  for (p = select_.length - 1; p >= 0; p--) {
    if (select_.options[p].selected) {
      select_.options[p].setAttribute("selected", "selected");
    }
    else {
      select_.options[p].removeAttribute("selected");
    }
  }
}

function change_day(ev, id) {
  if (check_day(ev, id)) {
    input = document.getElementById(id);
    input.setAttribute("value", input.value);
  }
}

function change_month(ev, id) {
  if (check_month(ev, id)) {
    input = document.getElementById(id);
    input.setAttribute("value", input.value);
  }
}

function change_year(id) {
  year = document.getElementById(id).value;
  from = parseFloat(document.getElementById(id).getAttribute('from'));
  to = parseFloat(document.getElementById(id).getAttribute('to'));
  year = parseFloat(year);
  if ((year >= from) && (year <= to)) {
    document.getElementById(id).setAttribute("value", year);
  }
  else {
    document.getElementById(id).setAttribute("value", '');
  }
}

function check_day(e, id) {
  var chCode1 = e.which || e.keyCode;
  if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57)) {
    return false;
  }
  day = "" + document.getElementById(id).value + String.fromCharCode(chCode1);
  if (day.length > 2) {
    return false;
  }
  if (day == '00') {
    return false;
  }
  day = parseFloat(day);
  if ((day < 0) || (day > 31)) {
    return false;
  }
  return true;
}

function check_month(e, id) {
  var chCode1 = e.which || e.keyCode;
  if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57)) {
    return false;
  }
  month = "" + document.getElementById(id).value + String.fromCharCode(chCode1);

  if (month.length > 2) {
    return false;
  }
  if (month == '00') {
    return false;
  }
  month = parseFloat(month);
  if ((month < 0) || (month > 12)) {
    return false;
  }
  return true;
}

function check_year2(id) {
  year = document.getElementById(id).value;
  from = parseFloat(document.getElementById(id).getAttribute('from'));
  year = parseFloat(year);
  if (year < from) {
    document.getElementById(id).value = '';
    alert(Drupal.t('The value of "year" is not valid.'));
  }
}

function check_year1(e, id) {
  var chCode1 = e.which || e.keyCode;
  if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57)) {
    return false;
  }
  year = "" + document.getElementById(id).value + String.fromCharCode(chCode1);
  to = parseFloat(document.getElementById(id).getAttribute('to'));
  year = parseFloat(year);
  if (year > to) {
    return false;
  }
  return true;
}

function change_func(id, label) {
  document.getElementById(id).setAttribute("onclick", label);
}

function change_size(size, num) {
  document.getElementById(num + '_elementform_id_temp').style.width = size + 'px';
  if (document.getElementById(num + '_element_input')) {
    document.getElementById(num + '_element_input').style.width = size + 'px';
  }
  switch (size) {
    case '111': {
      document.getElementById(num + '_elementform_id_temp').setAttribute("rows", "2");
      break;

    }
    case '222': {
      document.getElementById(num + '_elementform_id_temp').setAttribute("rows", "4");
      break;

    }
    case '444': {
      document.getElementById(num + '_elementform_id_temp').setAttribute("rows", "8");
      break;
    }
  }
}

function type_section_break(i, w_editor) {
  document.getElementById("element_type").value = "type_section_break";
  delete_last_child();
  oElement = document.getElementById('table_editor');
  var iReturnTop = 0;
  var iReturnLeft = 0;
  while (oElement != null) {
    iReturnTop += oElement.offsetTop;
    iReturnLeft += oElement.offsetLeft;
    oElement = oElement.offsetParent;
  }
  document.getElementById('main_editor').style.display = "block";
  document.getElementById('main_editor').style.left = iReturnLeft + 195 + "px";
  document.getElementById('main_editor').style.top = iReturnTop + 70 + "px";
  // ifr_id = document.getElementsByTagName("iframe")[0].id;
  // ifr = getIFrameDocument(ifr_id);
  if (document.getElementById('textAreaContent').style.display == "none") {
    // ifr.body.innerHTML = w_editor;
    if (document.getElementsByTagName("iframe")[0]) {
      ifr_id = document.getElementsByTagName("iframe")[0].id;
      ifr = getIFrameDocument(ifr_id);
      ifr.body.innerHTML = w_editor;
    }
  }
  else {
	// document.getElementById('editor').value = w_editor;
    document.getElementById('textAreaContent').value = w_editor;
    var table_div = document.getElementById('edit_table');
    var helparea = document.createElement("div");
    helparea.setAttribute("id", "help");
    helparea.setAttribute("style", "position:relative; top:30px; background-color: #FEF5F1;color: #8C2E0B;border-color: #ED541D; padding: 2px;border: 1px solid #DD7777;");
    helparea.innerHTML = "<h2>To show HTML editor download \'tinymce\' library from <a href=\'http://github.com/downloads/tinymce/tinymce/tinymce_3.5.7.zip\'>http://github.com/downloads/tinymce/tinymce/tinymce_3.5.7.zip</a> and extract it to sites/all/libraries/tinymce directory.</h2>";
    table_div.appendChild(helparea);
    var textAreaContent = document.getElementById('textAreaContent');
    // document.getElementById('textAreaContent').value = w_editor;
    textAreaContent.setAttribute("class", "form-textarea");
    textAreaContent.setAttribute("cols", "85");
    textAreaContent.setAttribute("rows", "10");
    textAreaContent.setAttribute("style", "visibility:visible; width:600px;");
  }
  // if (document.getElementById('textAreaContent').style.display == "none") {
    ////in_editor.innerHTML = ifr.body.innerHTML;
    // ifr_id = document.getElementsByTagName("iframe")[0].id;
    // ifr = getIFrameDocument(ifr_id);
    // in_editor.innerHTML = ifr.body.innerHTML;
  // }
  // else {
    // in_editor.innerHTML = document.getElementById('content').value;
    ////in_editor.innerHTML = document.getElementById('editor').value;
  // }
  element = 'div';
  var div = document.createElement('div');
  div.setAttribute("id", "main_div");
  var main_td = document.getElementById('show_table');
  main_td.appendChild(div);

  var div = document.createElement('div');
  div.style.width = "650px";
  document.getElementById('edit_table').appendChild(div);
}

function type_submit_reset(i, w_submit_title, w_reset_title, w_class, w_act, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_submit_reset";
  delete_last_child();

  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";

  var el_submit_title_label = document.createElement('label');
  el_submit_title_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_submit_title_label.innerHTML = "Submit button label";

  var el_submit_title_textarea = document.createElement('input');
  el_submit_title_textarea.setAttribute("id", "edit_for_title");
  el_submit_title_textarea.setAttribute("type", "text");
  el_submit_title_textarea.style.cssText = "width:160px";
  el_submit_title_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_submitform_id_temp', this.value)");
  el_submit_title_textarea.value = w_submit_title;
  var el_submit_func_label = document.createElement('label');
  el_submit_func_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_submit_func_label.innerHTML = "Submit function";
  var el_submit_func_textarea = document.createElement('input');
  el_submit_func_textarea.setAttribute("type", "text");
  el_submit_func_textarea.setAttribute("disabled", "disabled");
  el_submit_func_textarea.style.cssText = "width:160px";
  el_submit_func_textarea.value = "check_required('submit', 'form_id_temp')";

  var el_reset_title_label = document.createElement('label');
  el_reset_title_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_reset_title_label.innerHTML = "Reset button label";

  var el_reset_title_textarea = document.createElement('input');
  el_reset_title_textarea.setAttribute("id", "edit_for_title");
  el_reset_title_textarea.setAttribute("type", "text");
  el_reset_title_textarea.style.cssText = "width:160px";
  el_reset_title_textarea.setAttribute("onKeyUp", "change_label('" + i + "_element_resetform_id_temp', this.value)");
  el_reset_title_textarea.value = w_reset_title;

  var el_reset_active = document.createElement('input');
  el_reset_active.setAttribute("type", "checkbox");
  el_reset_active.style.cssText = "vertical-align:middle;";
  el_reset_active.setAttribute("onClick", "active_reset(this.checked, " + i + ")");
  if (w_act) {
    el_reset_active.setAttribute("checked", "checked");
  }
  var el_reset_active_label = document.createElement('label');
  el_reset_active_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_reset_active_label.innerHTML = "Display Reset button";

  var el_reset_func_label = document.createElement('label');
  el_reset_func_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_reset_func_label.innerHTML = "Reset function";
  var el_reset_func_textarea = document.createElement('input');
  el_reset_func_textarea.setAttribute("type", "text");
  el_reset_func_textarea.setAttribute("disabled", "disabled");
  el_reset_func_textarea.style.cssText = "width:160px";
  el_reset_func_textarea.value = "check_required('reset')";

  var el_style_label = document.createElement('label');
  el_style_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_style_label.innerHTML = "Class name";

  var el_style_textarea = document.createElement('input');
  el_style_textarea.setAttribute("id", "element_style");
  el_style_textarea.setAttribute("type", "text");
  el_style_textarea.setAttribute("value", w_class);
  el_style_textarea.style.cssText = "width:200px;";
  el_style_textarea.setAttribute("onChange", "change_class(this.value,'" + i + "')");
  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_submit_reset')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);

  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_submit_reset')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");

    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_submit_reset')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_submit_reset')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }

  var t = document.getElementById('edit_table');
  var hr = document.createElement('hr');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');
  var br7 = document.createElement('br');
  var br8 = document.createElement('br');
  var br9 = document.createElement('br');
  edit_main_td1.appendChild(el_submit_title_label);
  // edit_main_td1.appendChild(br7);
  edit_main_td1.appendChild(el_submit_func_label);
  edit_main_td1_1.appendChild(el_submit_title_textarea);
  edit_main_td1_1.appendChild(br1);
  edit_main_td1_1.appendChild(el_submit_func_textarea);

  edit_main_td2.appendChild(el_reset_active_label);
  // edit_main_td2.appendChild(br5);
  edit_main_td2.appendChild(el_reset_title_label);
  // edit_main_td2.appendChild(br8);
  edit_main_td2.appendChild(el_reset_func_label);
  edit_main_td2_1.appendChild(el_reset_active);
  edit_main_td2_1.appendChild(br9);
  edit_main_td2_1.appendChild(el_reset_title_textarea);
  edit_main_td2_1.appendChild(br2);
  edit_main_td2_1.appendChild(el_reset_func_textarea);

  edit_main_td3.appendChild(el_style_label);
  edit_main_td3_1.appendChild(el_style_textarea);

  edit_main_td4.appendChild(el_attr_label);
  edit_main_td4.appendChild(el_attr_add);
  edit_main_td4.appendChild(br3);
  edit_main_td4.appendChild(el_attr_table);
  edit_main_td4.setAttribute("colspan", "2");

  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);

  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);

  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_div.appendChild(edit_main_table);

  t.appendChild(edit_div);

  element = 'button';
  type1 = 'button';
  type2 = 'button';
  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_submit_reset");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");

  var adding_submit = document.createElement(element);
  adding_submit.setAttribute("type", type1);

  adding_submit.setAttribute("class", "button_submit");

  adding_submit.setAttribute("id", i + "_element_submitform_id_temp");
  adding_submit.setAttribute("value", w_submit_title);
  adding_submit.innerHTML = w_submit_title;
  adding_submit.setAttribute("onClick", "check_required('submit', 'form_id_temp');");

  var adding_reset = document.createElement(element);
  adding_reset.setAttribute("type", type2);

  adding_reset.setAttribute("class", "button_reset");
  if (!w_act) {
    adding_reset.style.display = "none";
  }
  adding_reset.setAttribute("id", i + "_element_resetform_id_temp");
  adding_reset.setAttribute("value", w_reset_title);
  adding_reset.setAttribute("onClick", "check_required('reset');");
  adding_reset.innerHTML = w_reset_title;

  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  var tr = document.createElement('tr');

  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'middle');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");
  td1.style.cssText = 'display:none';

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'middle');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.style.cssText = 'display:none';
  label.innerHTML = "type_submit_reset_" + i;
  var main_td = document.getElementById('show_table');

  td1.appendChild(label);
  td2.appendChild(adding_type);
  td2.appendChild(adding_submit);
  td2.appendChild(adding_reset);

  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);

  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  change_class(w_class, i);
  refresh_attr(i, 'type_submit_reset');
}

function type_hidden(i, w_name, w_value, w_attr_name, w_attr_value) {
  document.getElementById("element_type").value = "type_hidden";
  delete_last_child();
  var edit_div = document.createElement('div');
  edit_div.setAttribute("id", "edit_div");
  edit_div.setAttribute("style", "border-top:1px dotted black;padding:10px;  padding-top:0px; padding-bottom:0px; margin-top:10px;");

  var edit_main_table = document.createElement('table');
  edit_main_table.setAttribute("id", "edit_main_table");
  edit_main_table.setAttribute("class", "form_maker_table");
  edit_main_table.setAttribute("cellpadding", "0");
  edit_main_table.setAttribute("cellspacing", "0");

  var edit_main_tr1 = document.createElement('tr');
  edit_main_tr1.setAttribute("valing", "top");

  var edit_main_tr2 = document.createElement('tr');
  edit_main_tr2.setAttribute("valing", "top");

  var edit_main_tr3 = document.createElement('tr');
  edit_main_tr3.setAttribute("valing", "top");

  var edit_main_tr4 = document.createElement('tr');
  edit_main_tr4.setAttribute("valing", "top");

  var edit_main_tr5 = document.createElement('tr');
  edit_main_tr5.setAttribute("valing", "top");

  var edit_main_tr6 = document.createElement('tr');
  edit_main_tr6.setAttribute("valing", "top");

  var edit_main_tr7 = document.createElement('tr');
  edit_main_tr7.setAttribute("valing", "top");

  var edit_main_td1 = document.createElement('td');
  edit_main_td1.style.cssText = "padding-top:10px";
  var edit_main_td1_1 = document.createElement('td');
  edit_main_td1_1.style.cssText = "padding-top:10px";
  var edit_main_td2 = document.createElement('td');
  edit_main_td2.style.cssText = "padding-top:10px";
  var edit_main_td2_1 = document.createElement('td');
  edit_main_td2_1.style.cssText = "padding-top:10px";

  var edit_main_td3 = document.createElement('td');
  edit_main_td3.style.cssText = "padding-top:10px";
  var edit_main_td3_1 = document.createElement('td');
  edit_main_td3_1.style.cssText = "padding-top:10px";
  var edit_main_td4 = document.createElement('td');
  edit_main_td4.style.cssText = "padding-top:10px";
  var edit_main_td4_1 = document.createElement('td');
  edit_main_td4_1.style.cssText = "padding-top:10px";

  var edit_main_td5 = document.createElement('td');
  edit_main_td5.style.cssText = "padding-top:10px";
  var edit_main_td5_1 = document.createElement('td');
  edit_main_td5_1.style.cssText = "padding-top:10px";

  var edit_main_td6 = document.createElement('td');
  edit_main_td6.style.cssText = "padding-top:10px";
  var edit_main_td6_1 = document.createElement('td');
  edit_main_td6_1.style.cssText = "padding-top:10px";

  var edit_main_td7 = document.createElement('td');
  edit_main_td7.style.cssText = "padding-top:10px";
  var edit_main_td7_1 = document.createElement('td');
  edit_main_td7_1.style.cssText = "padding-top:10px";
  var el_field_id_label = document.createElement('label');
  el_field_id_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px; display: inline;";
  el_field_id_label.innerHTML = "Field Id";

  var el_field_id_input = document.createElement('input');
  el_field_id_input.setAttribute("type", "text");
  el_field_id_input.setAttribute("disabled", "disabled");
  el_field_id_input.setAttribute("value", i + "_elementform_id_temp");
  el_field_id_input.style.cssText = "margin-left: 41px; width:160px";

  var el_field_name_label = document.createElement('label');
  el_field_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px; display: inline;";
  el_field_name_label.innerHTML = "Field Name";

  var el_field_name_input = document.createElement('input');
  el_field_name_input.setAttribute("type", "text");

  el_field_name_input.setAttribute("value", w_name);
  el_field_name_input.style.cssText = "margin-left: 16px; width:160px";
  el_field_name_input.setAttribute("onChange", "change_field_name('" + i + "', this)");

  var el_field_value_label = document.createElement('label');
  el_field_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px; display: inline;";
  el_field_value_label.innerHTML = "Field Value";

  var el_field_value_input = document.createElement('input');
  el_field_value_input.setAttribute("type", "text");
  el_field_value_input.setAttribute("value", w_value);
  el_field_value_input.style.cssText = "margin-left: 16px; width:160px";
  el_field_value_input.setAttribute("onKeyUp", "change_field_value('" + i + "', this.value)");

  var el_attr_label = document.createElement('label');
  el_attr_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_attr_label.innerHTML = "Additional Attributes";
  var el_attr_add = document.createElement('img');
  el_attr_add.setAttribute("id", "el_choices_add");
  el_attr_add.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/add.png');
  el_attr_add.style.cssText = 'cursor:pointer; margin-left:68px';
  el_attr_add.setAttribute("title", 'add');
  el_attr_add.setAttribute("onClick", "add_attr(" + i + ", 'type_text')");
  var el_attr_table = document.createElement('table');
  el_attr_table.setAttribute("id", 'attributes');
  el_attr_table.setAttribute("border", '0');
  el_attr_table.style.cssText = 'margin-left:0px';
  var el_attr_tr_label = document.createElement('tr');
  el_attr_tr_label.setAttribute("idi", '0');
  var el_attr_td_name_label = document.createElement('th');
  el_attr_td_name_label.style.cssText = 'width:100px';
  var el_attr_td_value_label = document.createElement('th');
  el_attr_td_value_label.style.cssText = 'width:100px';
  var el_attr_td_X_label = document.createElement('th');
  el_attr_td_X_label.style.cssText = 'width:10px';
  var el_attr_name_label = document.createElement('label');
  el_attr_name_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_name_label.innerHTML = "Name";

  var el_attr_value_label = document.createElement('label');
  el_attr_value_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 11px";
  el_attr_value_label.innerHTML = "Value";

  el_attr_table.appendChild(el_attr_tr_label);
  el_attr_tr_label.appendChild(el_attr_td_name_label);
  el_attr_tr_label.appendChild(el_attr_td_value_label);
  el_attr_tr_label.appendChild(el_attr_td_X_label);
  el_attr_td_name_label.appendChild(el_attr_name_label);
  el_attr_td_value_label.appendChild(el_attr_value_label);
  n = w_attr_name.length;
  for (j = 1; j <= n; j++) {
    var el_attr_tr = document.createElement('tr');
    el_attr_tr.setAttribute("id", "attr_row_" + j);
    el_attr_tr.setAttribute("idi", j);
    var el_attr_td_name = document.createElement('td');
    el_attr_td_name.style.cssText = 'width:100px';
    var el_attr_td_value = document.createElement('td');
    el_attr_td_value.style.cssText = 'width:100px';

    var el_attr_td_X = document.createElement('td');
    var el_attr_name = document.createElement('input');

    el_attr_name.setAttribute("type", "text");

    el_attr_name.style.cssText = "width:100px";
    el_attr_name.setAttribute("value", w_attr_name[j - 1]);
    el_attr_name.setAttribute("id", "attr_name" + j);
    el_attr_name.setAttribute("onChange", "change_attribute_name(" + i + ", this, 'type_text')");

    var el_attr_value = document.createElement('input');

    el_attr_value.setAttribute("type", "text");

    el_attr_value.style.cssText = "width:100px";
    el_attr_value.setAttribute("value", w_attr_value[j - 1]);
    el_attr_value.setAttribute("id", "attr_value" + j);
    el_attr_value.setAttribute("onChange", "change_attribute_value(" + i + ", " + j + ", 'type_text')");

    var el_attr_remove = document.createElement('img');
    el_attr_remove.setAttribute("id", "el_choices" + j + "_remove");
    el_attr_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
    el_attr_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
    el_attr_remove.setAttribute("align", 'top');
    el_attr_remove.setAttribute("onClick", "remove_attr(" + j + ", " + i + ", 'type_text')");
    el_attr_table.appendChild(el_attr_tr);
    el_attr_tr.appendChild(el_attr_td_name);
    el_attr_tr.appendChild(el_attr_td_value);
    el_attr_tr.appendChild(el_attr_td_X);
    el_attr_td_name.appendChild(el_attr_name);
    el_attr_td_value.appendChild(el_attr_value);
    el_attr_td_X.appendChild(el_attr_remove);
  }
  var t = document.getElementById('edit_table');
  var hr = document.createElement('hr');
  var br = document.createElement('br');
  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');
  var br5 = document.createElement('br');
  var br6 = document.createElement('br');
  edit_main_td1.appendChild(el_field_id_label);
  edit_main_td1.appendChild(el_field_id_input);
  edit_main_td2.appendChild(el_field_name_label);
  edit_main_td2.appendChild(el_field_name_input);
  edit_main_td3.appendChild(el_field_value_label);
  edit_main_td3.appendChild(el_field_value_input);
  edit_main_td4.appendChild(el_attr_label);
  edit_main_td4.appendChild(el_attr_add);
  edit_main_td4.appendChild(br3);
  edit_main_td4.appendChild(el_attr_table);
  edit_main_tr1.appendChild(edit_main_td1);
  edit_main_tr1.appendChild(edit_main_td1_1);
  edit_main_tr2.appendChild(edit_main_td2);
  edit_main_tr2.appendChild(edit_main_td2_1);
  edit_main_tr3.appendChild(edit_main_td3);
  edit_main_tr3.appendChild(edit_main_td3_1);

  edit_main_tr4.appendChild(edit_main_td4);
  edit_main_tr4.appendChild(edit_main_td4_1);
  edit_main_tr5.appendChild(edit_main_td5);
  edit_main_tr5.appendChild(edit_main_td5_1);
  edit_main_tr6.appendChild(edit_main_td6);
  edit_main_tr6.appendChild(edit_main_td6_1);

  edit_main_table.appendChild(edit_main_tr1);
  edit_main_table.appendChild(edit_main_tr2);
  edit_main_table.appendChild(edit_main_tr3);
  edit_main_table.appendChild(edit_main_tr4);
  edit_div.appendChild(edit_main_table);

  t.appendChild(edit_div);

  element = 'input';
  type = 'hidden';

  var adding_type = document.createElement("input");
  adding_type.setAttribute("type", "hidden");
  adding_type.setAttribute("value", "type_hidden");
  adding_type.setAttribute("name", i + "_typeform_id_temp");
  adding_type.setAttribute("id", i + "_typeform_id_temp");

  var adding = document.createElement(element);
  adding.setAttribute("type", type);
  adding.setAttribute("value", w_value);
  adding.setAttribute("id", i + "_elementform_id_temp");
  adding.setAttribute("name", w_name);

  var div = document.createElement('div');
  div.setAttribute("id", "main_div");

  var table = document.createElement('table');
  table.setAttribute("id", i + "_elemet_tableform_id_temp");
  table.setAttribute("class", "form_maker_table");
  table.setAttribute("cellpadding", '0');
  table.setAttribute("cellspacing", '0');

  var tr = document.createElement('tr');

  var td1 = document.createElement('td');
  td1.setAttribute("valign", 'middle');
  td1.setAttribute("align", 'left');
  td1.setAttribute("id", i + "_label_sectionform_id_temp");
  td1.style.cssText = 'display:none';

  var td2 = document.createElement('td');
  td2.setAttribute("valign", 'middle');
  td2.setAttribute("align", 'left');
  td2.setAttribute("id", i + "_element_sectionform_id_temp");

  var br1 = document.createElement('br');
  var br2 = document.createElement('br');
  var br3 = document.createElement('br');
  var br4 = document.createElement('br');

  var label = document.createElement('span');
  label.setAttribute("id", i + "_element_labelform_id_temp");
  label.style.cssText = 'display:none';
  label.innerHTML = w_name;
  var main_td = document.getElementById('show_table');

  td1.appendChild(label);
  td2.appendChild(adding);
  td2.appendChild(adding_type);
  tr.appendChild(td1);
  tr.appendChild(td2);
  table.appendChild(tr);

  div.appendChild(table);
  div.appendChild(br3);
  main_td.appendChild(div);
  refresh_attr(i, 'type_text');
}

function field_to_select(id, type) {
  switch (type) {
    case 'day': {
      w_width = document.getElementById('edit_for_day_size').value != '' ? document.getElementById('edit_for_day_size').value : 50;
      w_day = document.getElementById(id + "_dayform_id_temp").value;
      document.getElementById(id + "_td_date_input1").innerHTML = '';

      var select_day = document.createElement('select');
      select_day.setAttribute("id", id + '_dayform_id_temp');
      select_day.setAttribute("name", id + '_dayform_id_temp');
      select_day.setAttribute("onChange", 'set_select(this)');
      select_day.setAttribute("class", "form-select");
      select_day.style.width = w_width + 'px';

      var options = document.createElement('option');
      options.setAttribute("value", '');
      options.innerHTML = '';
      select_day.appendChild(options);
      for (k = 1; k <= 31; k++) {
        if (k < 10) {
          k = '0' + k;
        }
        var options = document.createElement('option');
        options.setAttribute("value", k);
        options.innerHTML = k;
        if (k == w_day) {
          options.setAttribute("selected", "selected");
        }
        select_day.appendChild(options);
      }
      document.getElementById(id + "_td_date_input1").appendChild(select_day);
      break;

    }
    case 'month': {
      w_width = document.getElementById('edit_for_month_size').value != '' ? document.getElementById('edit_for_month_size').value : 150;
      w_month = document.getElementById(id + "_monthform_id_temp").value;
      document.getElementById(id + "_td_date_input2").innerHTML = '';

      var select_month = document.createElement('select');
      select_month.setAttribute("id", id + '_monthform_id_temp');
      select_month.setAttribute("name", id + '_monthform_id_temp');
      select_month.setAttribute("onChange", 'set_select(this)');
      select_month.setAttribute("class", "form-select");
      select_month.style.width = w_width + 'px';

      var options = document.createElement('option');
      options.setAttribute("value", '');
      options.innerHTML = '';
      select_month.appendChild(options);
      var myMonths = new Array("<!--repstart-->January<!--repend-->", "<!--repstart-->February<!--repend-->", "<!--repstart-->March<!--repend-->", "<!--repstart-->April<!--repend-->", "<!--repstart-->May<!--repend-->", "<!--repstart-->June<!--repend-->", "<!--repstart-->July<!--repend-->", "<!--repstart-->August<!--repend-->", "<!--repstart-->September<!--repend-->", "<!--repstart-->October<!--repend-->", "<!--repstart-->November<!--repend-->", "<!--repstart-->December<!--repend-->");
      for (k = 1; k <= 12; k++) {
        if (k < 10) {
          k = '0' + k;
        }
        var options = document.createElement('option');
        options.setAttribute("value", k);
        options.innerHTML = myMonths[k - 1];
        if (k == w_month) {
          options.setAttribute("selected", "selected");
        }
        select_month.appendChild(options);
      }
      document.getElementById(id + "_td_date_input2").appendChild(select_month);
      break;

    }
    case 'year': {
      w_width = document.getElementById('edit_for_year_size').value != '' ? document.getElementById('edit_for_year_size').value : 60;
      w_year = document.getElementById(id + "_yearform_id_temp").value;

      document.getElementById(id + "_td_date_input3").innerHTML = '';
      var select_year = document.createElement('select');
      select_year.setAttribute("id", id + '_yearform_id_temp');
      select_year.setAttribute("name", id + '_yearform_id_temp');
      select_year.setAttribute("onChange", 'set_select(this)');
      select_year.setAttribute("class", "form-select");
      select_year.style.width = w_width + 'px';

      var options = document.createElement('option');
      options.setAttribute("value", '');
      options.innerHTML = '';
      select_year.appendChild(options);
      from = parseInt(document.getElementById("edit_for_year_interval_from").value);
      to = parseInt(document.getElementById("edit_for_year_interval_to").value);
      for (k = to; k >= from; k--) {
        var options = document.createElement('option');
        options.setAttribute("value", k);
        options.innerHTML = k;
        if (k == w_year) {
          options.setAttribute("selected", "selected");
        }
        select_year.appendChild(options);
      }
      select_year.value = w_year;
      select_year.setAttribute('from', from);
      select_year.setAttribute('to', to);
      document.getElementById(id + "_td_date_input3").appendChild(select_year);
      break;
    }
  }
  refresh_attr(id, 'type_date_fields');
}

function field_to_text(id, type) {
  switch (type) {
    case 'day': {
      w_width = document.getElementById('edit_for_day_size').value != '' ? document.getElementById('edit_for_day_size').value : 50;
      w_day = document.getElementById(id + "_dayform_id_temp").value;
      document.getElementById(id + "_td_date_input1").innerHTML = '';
      var day = document.createElement('input');
      day.setAttribute("type", 'text');
      day.setAttribute("value", w_day);
      day.setAttribute("id", id + "_dayform_id_temp");
      day.setAttribute("name", id + "_dayform_id_temp");
      day.setAttribute("onChange", "change_value('" + id + "_dayform_id_temp')");
      day.setAttribute("onKeyPress", "return check_day(event, '" + id + "_dayform_id_temp')");
      day.setAttribute("onBlur", "if (this.value=='0') this.value=''; else add_0('" + id + "_dayform_id_temp')");
      day.style.width = w_width + 'px';
      document.getElementById(id + "_td_date_input1").appendChild(day);
      break;

    }
    case 'month': {
      w_width = document.getElementById('edit_for_month_size').value != '' ? document.getElementById('edit_for_month_size').value : 150;
      w_month = document.getElementById(id + "_monthform_id_temp").value;
      document.getElementById(id + "_td_date_input2").innerHTML = '';
      var month = document.createElement('input');
      month.setAttribute("type", 'text');
      month.setAttribute("value", w_month);
      month.setAttribute("id", id + "_monthform_id_temp");
      month.setAttribute("name", id + "_monthform_id_temp");
      month.style.width = w_width + 'px';
      month.setAttribute("onKeyPress", "return check_month(event, '" + id + "_monthform_id_temp')");
      month.setAttribute("onChange", "change_value('" + id + "_monthform_id_temp')");
      month.setAttribute("onBlur", "if (this.value=='0') this.value=''; else add_0('" + id + "_monthform_id_temp')");
      document.getElementById(id + "_td_date_input2").appendChild(month);
      break;

    }
    case 'year': {
      w_width = document.getElementById('edit_for_year_size').value != '' ? document.getElementById('edit_for_year_size').value : 60;
      w_year = document.getElementById(id + "_yearform_id_temp").value;
      document.getElementById(id + "_td_date_input3").innerHTML = '';
      from = parseInt(document.getElementById("edit_for_year_interval_from").value);
      to = parseInt(document.getElementById("edit_for_year_interval_to").value);
      if ((parseInt(w_year) < from) || (parseInt(w_year) > to)) {
        w_year = '';
      }
      var year = document.createElement('input');
      year.setAttribute("type", 'text');
      year.setAttribute("value", w_year);
      year.setAttribute("id", id + "_yearform_id_temp");
      year.setAttribute("name", id + "_yearform_id_temp");
      year.setAttribute("onChange", "change_year('" + id + "_yearform_id_temp')");
      year.setAttribute("onKeyPress", "return check_year1(event, '" + id + "_yearform_id_temp')");
      year.setAttribute("onBlur", "check_year2('" + id + "_yearform_id_temp')");
      year.style.width = w_width + 'px';
      year.setAttribute('from', from);
      year.setAttribute('to', to);
      document.getElementById(id + "_td_date_input3").appendChild(year);
      break;
    }
  }
  refresh_attr(id, 'type_date_fields');
}

function set_divider(id, divider) {
  document.getElementById(id + "_separator1").innerHTML = divider;
  document.getElementById(id + "_separator2").innerHTML = divider;
}

function year_interval(id) {
  from = parseInt(document.getElementById("edit_for_year_interval_from").value);
  to = parseInt(document.getElementById("edit_for_year_interval_to").value);
  if (to - from < 0) {
    alert(Drupal.t('Invalid interval of years.'));
    document.getElementById("edit_for_year_interval_from").value = to;
  }
  else {
    if (document.getElementById(id + "_yearform_id_temp").tagName == 'SELECT') {
      field_to_select(id, 'year');
    }
    else {
      field_to_text(id, 'year');
    }
  }
}

function enable_modals() {
  SqueezeBox.assign($$('a.modal'), {
    parse:'rel'
  });
}


function active_reset(val, id) {
  if (val) {
    document.getElementById(id + '_element_resetform_id_temp').style.display = "inline";
  }
  else {
    document.getElementById(id + '_element_resetform_id_temp').style.display = "none";
  }
}

function check_required() {
  alert(Drupal.t('"Submit" and "Reset" buttons are disabled in back end.'));
}

function isNumber(n) {
  return !isNaN(parseFloat(n)) && isFinite(n);
}
function change_field_name(id, x) {
  value = x.value;
  if (value == parseInt(value)) {
    alert(Drupal.t('The name of the field cannot be a number.'));
    x.value = "";
    document.getElementById(id + '_elementform_id_temp').name = '';
    document.getElementById(id + '_element_labelform_id_temp').innerHTML = '';
    return;
  }
  if (value == id + "_elementform_id_temp") {
    alert(Drupal.t('"Field Name" should differ from "Field Id".'))
    x.value = "";
  }
  else {
    document.getElementById(id + '_elementform_id_temp').name = value;
    document.getElementById(id + '_element_labelform_id_temp').innerHTML = value;
  }
}

function change_field_value(id, value) {
  document.getElementById(id + '_elementform_id_temp').value = value;
}

function return_attributes(id) {
  attr_names = new Array();
  attr_value = new Array();
  var input = document.getElementById(id);
  if (input) {
    atr = input.attributes;
    for (i = 0; i < 30; i++) {
      if (atr[i]) {
        if (atr[i].name.indexOf("add_") == 0) {
          attr_names.push(atr[i].name.replace('add_', ''));
          attr_value.push(atr[i].value);
        }
      }
    }
  }
  return Array(attr_names, attr_value);
}


function change_attributes(id, attr) {
  var div = document.createElement('div');
  var element = document.getElementById(id);
  element.setAttribute(attr, '');
}

function add_button(i) {
  edit_main_td4 = document.getElementById('buttons');
  if (edit_main_td4.lastChild) {
    j = parseInt(edit_main_td4.lastChild.getAttribute("idi")) + 1;
  }
  else {
    j = 1;
  }
  var table_button = document.createElement('table');
  table_button.setAttribute("width", "100%");
  table_button.setAttribute("border", "0");
  table_button.setAttribute("id", "button_opt" + j);
  table_button.setAttribute("idi", j);
  var tr_button = document.createElement('tr');
  var tr_hr = document.createElement('tr');
  var td_button = document.createElement('td');
  var td_X = document.createElement('td');
  var td_hr = document.createElement('td');
  td_hr.setAttribute("colspan", "3");
  tr_hr.appendChild(td_hr);
  tr_button.appendChild(td_button);
  tr_button.appendChild(td_X);
  table_button.appendChild(tr_hr);
  table_button.appendChild(tr_button);
  var br1 = document.createElement('br');
  var hr = document.createElement('hr');
  hr.setAttribute("id", "br" + j);
  var el_title_label = document.createElement('label');
  el_title_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_title_label.innerHTML = "Button name";
  var el_title = document.createElement('input');
  el_title.setAttribute("id", "el_title" + j);
  el_title.setAttribute("type", "text");
  el_title.setAttribute("value", "Button");
  el_title.style.cssText = "width:100px; margin-left:43px; padding:0; border-width: 1px";
  el_title.setAttribute("onKeyUp", "change_label('" + i + "_elementform_id_temp" + j + "', this.value);");
  var el_func_label = document.createElement('label');
  el_func_label.style.cssText = "color:#00aeef; font-weight:bold; font-size: 13px";
  el_func_label.innerHTML = "OnClick function";
  var el_func = document.createElement('input');
  el_func.setAttribute("id", "el_func" + j);
  el_func.setAttribute("type", "text");
  el_func.setAttribute("value", "");
  el_func.style.cssText = "width:100px; margin-left:20px;; padding:0; border-width: 1px";
  el_func.setAttribute("onKeyUp", "change_func('" + i + "_elementform_id_temp" + j + "', this.value);");
  var el_choices_remove = document.createElement('img');
  el_choices_remove.setAttribute("id", "el_button" + j + "_remove");
  el_choices_remove.setAttribute("src", '' + Drupal.settings.form_maker.get_module_path + '/images/delete.png');
  el_choices_remove.style.cssText = 'cursor:pointer; vertical-align:middle; margin:3px';
  el_choices_remove.setAttribute("align", 'top');
  el_choices_remove.setAttribute("onClick", "remove_button(" + j + "," + i + ")");
  td_hr.appendChild(hr);
  td_button.appendChild(el_title_label);
  td_button.appendChild(el_title);
  td_button.appendChild(br1);
  td_button.appendChild(el_func_label);
  td_button.appendChild(el_func);
  td_X.appendChild(el_choices_remove);
  edit_main_td4.appendChild(table_button);
  element = 'button';
  type = 'button';
  td2 = document.getElementById(i + "_element_sectionform_id_temp");
  var adding = document.createElement(element);
  adding.setAttribute("type", type);
  adding.setAttribute("id", i + "_elementform_id_temp" + j);
  adding.setAttribute("name", i + "_elementform_id_temp" + j);
  adding.setAttribute("value", "Button");
  adding.innerHTML = "Button";
  adding.setAttribute("onclick", "");
  td2.appendChild(adding);
  refresh_attr(i, 'type_checkbox');
}

function remove_button(j, i) {
  table = document.getElementById('button_opt' + j);
  button = document.getElementById(i + '_elementform_id_temp' + j);
  table.parentNode.removeChild(table);
  button.parentNode.removeChild(button);
}

function check_isnum_3_10(e) {
  var chCode1 = e.which || e.keyCode;
  if (chCode1 > 31 && (chCode1 < 51 || chCode1 > 57)) {
    return false;
  }
  else if ((document.getElementById('captcha_digit').value + (chCode1 - 48)) > 9) {
    return false;
  }
  return true;
}

function set_sel_am_pm(select_) {
  if (select_.options[0].selected) {
    select_.options[0].setAttribute("selected", "selected");
    select_.options[1].removeAttribute("selected");
  }
  else {
    select_.options[1].setAttribute("selected", "selected");
    select_.options[0].removeAttribute("selected");
  }
}

function change_captcha_digit(digit) {
  captcha = document.getElementById('_wd_captchaform_id_temp');
  if (document.getElementById('captcha_digit').value) {
    captcha.setAttribute("digit", digit);
    captcha.setAttribute("src", "" + Drupal.settings.form_maker.captcha_url + digit + "&i=form_id_temp");
    document.getElementById('_wd_captcha_inputform_id_temp').style.width = (document.getElementById('captcha_digit').value * 10 + 15) + "px";
  }
  else {
    captcha.setAttribute("digit", "6");
    captcha.setAttribute("src", "" + Drupal.settings.form_maker.captcha_url + "6" + "&i=form_id_temp");
    document.getElementById('_wd_captcha_inputform_id_temp').style.width = (6 * 10 + 15) + "px";
  }
}

function change_w(id, w) {
  document.getElementById(id).setAttribute("width", w)
}

function change_h(id, h) {
  document.getElementById(id).setAttribute("height", h);
}

function change_key(value, attribute) {
  document.getElementById('wd_recaptchaform_id_temp').setAttribute(attribute, value);
}

function dublicate(id) {
  enable2();
  type = document.getElementById(id).getAttribute('type');
  if (document.getElementById(id + '_element_labelform_id_temp').innerHTML) {
    w_field_label = document.getElementById(id + '_element_labelform_id_temp').innerHTML;
  }
  labels = all_labels();
  m = 0;
  t = true;
  if (type != "type_section_break") {
    while (t) {
      m++;
      for (k = 0; k < labels.length; k++) {
        t = true;
        if (labels[k] == w_field_label + '(' + m + ')') {
          break;
        }
        t = false;
      }
    }
    w_field_label = w_field_label + '(' + m + ')';
  }
  k = 0;
  w_choices = new Array();
  w_choices_checked = new Array();
  w_choices_disabled = new Array();
  w_allow_other_num = 0;
  t = 0;
  if (document.getElementById(id + '_label_and_element_sectionform_id_temp')) {
    w_field_label_pos = "top";
  }
  else {
    w_field_label_pos = "left";
  }
  if (document.getElementById(id + "_elementform_id_temp")) {
    s = document.getElementById(id + "_elementform_id_temp").style.width;
    w_size = s.substring(0, s.length - 2);
  }
  if (document.getElementById(id + "_requiredform_id_temp")) {
    w_required = document.getElementById(id + "_requiredform_id_temp").value;
  }
  if (document.getElementById(id + "_uniqueform_id_temp")) {
    w_unique = document.getElementById(id + "_uniqueform_id_temp").value;
  }
  if (document.getElementById(id + '_label_sectionform_id_temp')) {
    w_class = document.getElementById(id + '_label_sectionform_id_temp').getAttribute("class");
    if (!w_class) {
      w_class = "";
    }
  }
  switch (type) {
    case 'type_editor': {
      w_editor = document.getElementById(id + "_element_sectionform_id_temp").innerHTML;
      type_editor(gen, w_editor);
      add(0);
      break;

    }
    case 'type_section_break': {
      w_editor = document.getElementById(id + "_element_sectionform_id_temp").innerHTML;
      type_section_break(gen, w_editor);
      add(0);
      break;

    }
    case 'type_text': {
      w_first_val = document.getElementById(id + "_elementform_id_temp").value;
      w_title = document.getElementById(id + "_elementform_id_temp").title;
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_text(gen, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_required, w_unique, w_class, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_number': {
      w_first_val = document.getElementById(id + "_elementform_id_temp").value;
      w_title = document.getElementById(id + "_elementform_id_temp").title;
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_number(gen, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_required, w_unique, w_class, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_password': {
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_password(gen, w_field_label, w_field_label_pos, w_size, w_required, w_unique, w_class, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_textarea': {
      w_first_val = document.getElementById(id + "_elementform_id_temp").value;
      w_title = document.getElementById(id + "_elementform_id_temp").title;
      s = document.getElementById(id + "_elementform_id_temp").style.height;
      w_size_h = s.substring(0, s.length - 2);
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_textarea(gen, w_field_label, w_field_label_pos, w_size, w_size_h, w_first_val, w_title, w_required, w_unique, w_class, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_phone': {
      w_first_val = [document.getElementById(id + "_element_firstform_id_temp").value, document.getElementById(id + "_element_lastform_id_temp").value];
      w_title = [document.getElementById(id + "_element_firstform_id_temp").title, document.getElementById(id + "_element_lastform_id_temp").title];
      s = document.getElementById(id + "_element_lastform_id_temp").style.width;
      w_size = s.substring(0, s.length - 2);
      atrs = return_attributes(id + '_element_firstform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_phone(gen, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_required, w_unique, w_class, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_name': {
      if (document.getElementById(id + '_element_middleform_id_temp')) {
        w_name_format = "extended";
      }
      else {
        w_name_format = "normal";
      }
      w_first_val = [document.getElementById(id + "_element_firstform_id_temp").value, document.getElementById(id + "_element_lastform_id_temp").value];
      w_title = [document.getElementById(id + "_element_firstform_id_temp").title, document.getElementById(id + "_element_lastform_id_temp").title];
      s = document.getElementById(id + "_element_firstform_id_temp").style.width;
      w_size = s.substring(0, s.length - 2);
      atrs = return_attributes(id + '_element_firstform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_name(gen, w_field_label, w_field_label_pos, w_first_val, w_title, w_size, w_name_format, w_required, w_unique, w_class, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_address': {
      s = document.getElementById(id + "_div_address").style.width;
      w_size = s.substring(0, s.length - 2);
      atrs = return_attributes(id + '_street1form_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_address(gen, w_field_label, w_field_label_pos, w_size, w_required, w_class, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_submitter_mail': {
      w_first_val = document.getElementById(id + "_elementform_id_temp").value;
      w_title = document.getElementById(id + "_elementform_id_temp").title;
      w_send = document.getElementById(id + "_sendform_id_temp").value;
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_submitter_mail(gen, w_field_label, w_field_label_pos, w_size, w_first_val, w_title, w_send, w_required, w_unique, w_class, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_checkbox': {
      if (document.getElementById(id + '_hor')) {
        w_flow = "hor";
      }
      else {
        w_flow = "ver";
      }
      w_randomize = document.getElementById(id + "_randomizeform_id_temp").value;
      w_allow_other = document.getElementById(id + "_allow_otherform_id_temp").value;
      v = 0;
      for (k = 0; k < 100; k++) {
        if (document.getElementById(id + "_elementform_id_temp" + k)) {
          if (document.getElementById(id + "_elementform_id_temp" + k).getAttribute('other'))
            if (document.getElementById(id + "_elementform_id_temp" + k).getAttribute('other') == '1')
              w_allow_other_num = t;
          w_choices[t] = document.getElementById(id + "_elementform_id_temp" + k).value;
          w_choices_checked[t] = document.getElementById(id + "_elementform_id_temp" + k).checked;
          t++;
          v = k;
        }
      }
      atrs = return_attributes(id + '_elementform_id_temp' + v);
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_checkbox(gen, w_field_label, w_field_label_pos, w_flow, w_choices, w_choices_checked, w_required, w_randomize, w_allow_other, w_allow_other_num, w_class, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_radio': {
      if (document.getElementById(id + '_hor')) {
        w_flow = "hor";
      }
      else {
        w_flow = "ver";
      }
      w_randomize = document.getElementById(id + "_randomizeform_id_temp").value;
      w_allow_other = document.getElementById(id + "_allow_otherform_id_temp").value;
      v = 0;
      for (k = 0; k < 100; k++) {
        if (document.getElementById(id + "_elementform_id_temp" + k)) {
          if (document.getElementById(id + "_elementform_id_temp" + k).getAttribute('other'))
            if (document.getElementById(id + "_elementform_id_temp" + k).getAttribute('other') == '1')
              w_allow_other_num = t;
          w_choices[t] = document.getElementById(id + "_elementform_id_temp" + k).value;
          w_choices_checked[t] = document.getElementById(id + "_elementform_id_temp" + k).checked;
          t++;
          v = k;
        }
      }
      atrs = return_attributes(id + '_elementform_id_temp' + v);
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_radio(gen, w_field_label, w_field_label_pos, w_flow, w_choices, w_choices_checked, w_required, w_randomize, w_allow_other, w_allow_other_num, w_class, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_time': {
      atrs = return_attributes(id + '_hhform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      w_hh = document.getElementById(id + '_hhform_id_temp').value;
      w_mm = document.getElementById(id + '_mmform_id_temp').value;
      if (document.getElementById(id + '_ssform_id_temp')) {
        w_ss = document.getElementById(id + '_ssform_id_temp').value;
        w_sec = "1";
      }
      else {
        w_ss = "";
        w_sec = "0";
      }
      if (document.getElementById(id + '_am_pm_select')) {
        w_am_pm = document.getElementById(id + '_am_pmform_id_temp').value;
        w_time_type = "12";
      }
      else {
        w_am_pm = 0;
        w_time_type = "24";
      }
      type_time(gen, w_field_label, w_field_label_pos, w_time_type, w_am_pm, w_sec, w_hh, w_mm, w_ss, w_required, w_class, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_date': {
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      w_date = document.getElementById(id + '_elementform_id_temp').value;
      w_format = document.getElementById(id + '_buttonform_id_temp').getAttribute("format");
      w_but_val = document.getElementById(id + '_buttonform_id_temp').value;
      type_date(gen, w_field_label, w_field_label_pos, w_date, w_required, w_class, w_format, w_but_val, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_date_fields': {
      atrs = return_attributes(id + '_dayform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      w_day = document.getElementById(id + '_dayform_id_temp').value;
      w_month = document.getElementById(id + '_monthform_id_temp').value;
      w_year = document.getElementById(id + '_yearform_id_temp').value;
      w_day_type = document.getElementById(id + '_dayform_id_temp').tagName;
      w_month_type = document.getElementById(id + '_monthform_id_temp').tagName;
      w_year_type = document.getElementById(id + '_yearform_id_temp').tagName;
      w_day_label = document.getElementById(id + '_day_label').innerHTML;
      w_month_label = document.getElementById(id + '_month_label').innerHTML;
      w_year_label = document.getElementById(id + '_year_label').innerHTML;
      s = document.getElementById(id + '_dayform_id_temp').style.width;
      w_day_size = s.substring(0, s.length - 2);
      s = document.getElementById(id + '_monthform_id_temp').style.width;
      w_month_size = s.substring(0, s.length - 2);
      s = document.getElementById(id + '_yearform_id_temp').style.width;
      w_year_size = s.substring(0, s.length - 2);
      if (w_year_type == 'SELECT') {
        w_from = document.getElementById(id + '_yearform_id_temp').getAttribute('from');
        w_to = document.getElementById(id + '_yearform_id_temp').getAttribute('to');
      }
      else {
        var current_date = new Date();
        w_from = '1901';
        w_to = current_date.getFullYear();
      }
      w_divider = document.getElementById(id + '_separator1').innerHTML;
      type_date_fields(gen, w_field_label, w_field_label_pos, w_day, w_month, w_year, w_day_type, w_month_type, w_year_type, w_day_label, w_month_label, w_year_label, w_day_size, w_month_size, w_year_size, w_required, w_class, w_from, w_to, w_divider, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_own_select': {
      for (k = 0; k < 100; k++) {
        if (document.getElementById(id + "_option" + k)) {
          w_choices[t] = document.getElementById(id + "_option" + k).innerHTML;
          w_choices_checked[t] = document.getElementById(id + "_option" + k).selected;
          if (document.getElementById(id + "_option" + k).value == "") {
            w_choices_disabled[t] = true;
          }
          else {
            w_choices_disabled[t] = false;
          }
          t++;
        }
      }
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_own_select(gen, w_field_label, w_field_label_pos, w_size, w_choices, w_choices_checked, w_required, w_class, w_attr_name, w_attr_value, w_choices_disabled);
      add(0);
      break;

    }
    case 'type_country': {
      w_countries = [];

      select_ = document.getElementById(id + '_elementform_id_temp');
      n = select_.childNodes.length;
      for (i = 0; i < n; i++) {
        w_countries.push(select_.childNodes[i].value);
      }
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_country(gen, w_field_label, w_countries, w_field_label_pos, w_size, w_required, w_class, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_file_upload': {
      w_destination = document.getElementById(id + "_destination").value.replace("***destinationverj" + id + "***", "").replace("***destinationskizb" + id + "***", "");
      w_extension = document.getElementById(id + "_extension").value.replace("***extensionverj" + id + "***", "").replace("***extensionskizb" + id + "***", "");
      w_max_size = document.getElementById(id + "_max_size").value.replace("***max_sizeverj" + id + "***", "").replace("***max_sizeskizb" + id + "***", "");
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_file_upload(gen, w_field_label, w_field_label_pos, w_destination, w_extension, w_max_size, w_required, w_class, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_map': {
      w_lat = [];
      w_long = [];
      w_info = [];
      w_center_x = document.getElementById(id + "_elementform_id_temp").getAttribute("center_x");
      w_center_y = document.getElementById(id + "_elementform_id_temp").getAttribute("center_y");
      w_zoom = document.getElementById(id + "_elementform_id_temp").getAttribute("zoom");
      w_width = parseInt(document.getElementById(id + "_elementform_id_temp").style.width);
      w_height = parseInt(document.getElementById(id + "_elementform_id_temp").style.height);
      for (j = 0; j <= 20; j++) {
        if (document.getElementById(id + "_elementform_id_temp").getAttribute("lat" + j)) {
          w_lat.push(document.getElementById(id + "_elementform_id_temp").getAttribute("lat" + j));
          w_long.push(document.getElementById(id + "_elementform_id_temp").getAttribute("long" + j));
          w_info.push(document.getElementById(id + "_elementform_id_temp").getAttribute("info" + j));
        }
      }
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_map(gen, w_center_x, w_center_y, w_long, w_lat, w_zoom, w_width, w_height, w_class, w_info, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_mark_map': {
      w_info = document.getElementById(id + "_elementform_id_temp").getAttribute("info0");
      w_long = document.getElementById(id + "_elementform_id_temp").getAttribute("long0");
      w_lat = document.getElementById(id + "_elementform_id_temp").getAttribute("lat0");
      w_zoom = document.getElementById(id + "_elementform_id_temp").getAttribute("zoom");
      w_width = parseInt(document.getElementById(id + "_elementform_id_temp").style.width);
      w_height = parseInt(document.getElementById(id + "_elementform_id_temp").style.height);
      w_center_x = document.getElementById(id + "_elementform_id_temp").getAttribute("center_x");
      w_center_y = document.getElementById(id + "_elementform_id_temp").getAttribute("center_y");
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_mark_map(gen, w_field_label, w_field_label_pos, w_center_x, w_center_y, w_long, w_lat, w_zoom, w_width, w_height, w_class, w_info, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_submit_reset': {
      atrs = return_attributes(id + '_element_submitform_id_temp');
      w_act = !(document.getElementById(id + "_element_resetform_id_temp").style.display == "none");
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      w_submit_title = document.getElementById(id + "_element_submitform_id_temp").value;
      w_reset_title = document.getElementById(id + "_element_resetform_id_temp").value;
      type_submit_reset(gen, w_submit_title, w_reset_title, w_class, w_act, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_button': {
      w_title = new Array();
      w_func = new Array();
      t = 0;
      v = 0;
      for (k = 0; k < 100; k++) {
        if (document.getElementById(id + "_elementform_id_temp" + k)) {
          w_title[t] = document.getElementById(id + "_elementform_id_temp" + k).value;
          w_func[t] = document.getElementById(id + "_elementform_id_temp" + k).getAttribute("onclick");
          t++;
          v = k;
        }
      }
      atrs = return_attributes(id + '_elementform_id_temp' + v);
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_button(gen, w_title, w_func, w_class, w_attr_name, w_attr_value);
      add(0);
      break;

    }
    case 'type_hidden': {
      w_value = document.getElementById(id + "_elementform_id_temp").value;
      w_name = document.getElementById(id + "_elementform_id_temp").name;
      atrs = return_attributes(id + '_elementform_id_temp');
      w_attr_name = atrs[0];
      w_attr_value = atrs[1];
      type_hidden(gen, w_name, w_value, w_attr_name, w_attr_value);
      add(0);
      break;
    }
  }
}

/**
 * Change select theme.
 */
function form_maker_theme_change() {
  document.getElementById('selected_theme_css').value = document.getElementById('select_theme').value;
}