<?php
/**
 * @file
 * Form maker front_end.
 */

/**
 * View form.
 */
function form_maker_content_view($nodeid) {
  // drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_calendar.js');
  // drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_calendar-setup.js');
  // drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_cal_function.js');
  // drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/mootools.js');
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_if_gmap_front_end.js');
  $external_js = 'http://maps.google.com/maps/api/js?sensor=false';
  drupal_add_js('document.write(unescape("%3Cscript src=\'' . $external_js . '\' type=\'text/javascript\'%3E%3C/script%3E"));', array('type' => 'inline'));
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_cal.js');
  drupal_add_css(drupal_get_path('module', 'form_maker') . '/css/form_maker_cal.css');
  // View reports.
  variable_set('form_maker_view_reports' . $nodeid, variable_get('form_maker_view_reports' . $nodeid, '0') + 1);
  if (isset($_SESSION['submited' . $nodeid])) {
    if ($_SESSION['submited' . $nodeid] != NULL) {
      drupal_set_message($_SESSION['submited' . $nodeid], 'status', FALSE);
    }
  }
  $_SESSION['submited' . $nodeid] = '';
  $is_recaptcha = FALSE;
  $form_maker_front_end = "";
  if (isset($_POST["counter" . $nodeid])) {
    $ok = form_maker_savedata($nodeid, $form_maker_front_end);    if (is_numeric($ok)) {
      form_maker_remove($ok);
    }
  }
  $row = db_query("SELECT * FROM {form_maker_table} WHERE vid=:vid", array(':vid' => $nodeid))->fetchObject();
  drupal_add_js($row->javascript, array('type' => 'inline', 'scope' => 'footer'));
  $label_id = array();
  $label_type = array();
  $label_all = explode('#****#', $row->label_order);
  $label_all = array_slice($label_all, 0, count($label_all) - 1);
  foreach ($label_all as $key => $label_each) {
    $label_id_each = explode('#**id**#', $label_each);
    array_push($label_id, $label_id_each[0]);
    $label_order_each=explode('#**label**#', $label_id_each[1]);
    array_push($label_type, $label_order_each[1]);
  }
  $captcha_url = url('form_maker/captcha', array('query' => array('digit' => ''), 'absolute' => FALSE));
  $captcha_rep_url = url('form_maker/captcha', array('query' => array('r2' => mt_rand(0, 1000), 'digit' => ''), 'absolute' => FALSE));
  $rep1 = array(
    "<!--repstart-->Title<!--repend-->",
    "<!--repstart-->First<!--repend-->",
    "<!--repstart-->Last<!--repend-->",
    "<!--repstart-->Middle<!--repend-->",
    "<!--repstart-->January<!--repend-->",
    "<!--repstart-->February<!--repend-->",
    "<!--repstart-->March<!--repend-->",
    "<!--repstart-->April<!--repend-->",
    "<!--repstart-->May<!--repend-->",
    "<!--repstart-->June<!--repend-->",
    "<!--repstart-->July<!--repend-->",
    "<!--repstart-->August<!--repend-->",
    "<!--repstart-->September<!--repend-->",
    "<!--repstart-->October<!--repend-->",
    "<!--repstart-->November<!--repend-->",
    "<!--repstart-->December<!--repend-->",
    "<!--repstart-->Street Address<!--repend-->",
    "<!--repstart-->Street Address Line 2<!--repend-->",
    "<!--repstart-->City<!--repend-->",
    "<!--repstart-->State / Province / Region<!--repend-->",
    "<!--repstart-->Postal / Zip Code<!--repend-->",
    "<!--repstart-->Country<!--repend-->",
    "<!--repstart-->Area Code<!--repend-->",
    "<!--repstart-->Phone Number<!--repend-->",
    $captcha_url,
    'class="captcha_img"',
    '' . base_path() . drupal_get_path('module', 'form_maker') . '/images/refresh.png',
    'form_id_temp',
    'style="padding-right:170px"',
  );
  $rep2 = array(
    t("Title"),
    t("First"),
    t("Last"),
    t("Middle"),
    t("January"),
    t("February"),
    t("March"),
    t("April"),
    t("May"),
    t("June"),
    t("July"),
    t("August"),
    t("September"),
    t("October"),
    t("November"),
    t("December"),
    t("Street Address"),
    t("Street Address Line 2"),
    t("City"),
    t("State"),
    t("Postal / Zip Code"),
    t("Country"),
    t("Area Code"),
    t("Phone Number"),
    $captcha_rep_url,
    'class="captcha_img" style="display:none"',
    '' . base_path() . drupal_get_path('module', 'form_maker') . '/images/refresh.png',
    $nodeid,
    '',
  );
  $untilupload = str_replace($rep1, $rep2, $row->form_front);
  while (strpos($untilupload, "***destinationskizb") > 0) {
    $pos1 = strpos($untilupload, "***destinationskizb");
    $pos2 = strpos($untilupload, "***destinationverj");
    $untilupload = str_replace(substr($untilupload, $pos1, $pos2 - $pos1 + 22), "", $untilupload);
  }
  $default_css = db_query("SELECT css FROM {form_maker_themes} WHERE id=:id", array(':id' => $row->theme))->fetchField();
  $new_form_theme = explode('{', $default_css);
  $count_after_explod_theme = count($new_form_theme);
  for ($i = 0; $i < $count_after_explod_theme; $i++) {
    $body_or_classes[$i] = explode('}', $new_form_theme[$i]);
  }
  for($i = 0; $i < $count_after_explod_theme; $i++) {
    if ($i == 0) {
      $body_or_classes[$i][0] = "#form" . $nodeid . ' ' . str_replace(',', ", #form" . $nodeid, $body_or_classes[$i][0]);
    }
    else {
      $body_or_classes[$i][1] = "#form" . $nodeid . ' ' . str_replace(',', ", #form" . $nodeid, $body_or_classes[$i][1]);
    }
  }
  for($i = 0; $i < $count_after_explod_theme; $i++) {
    $body_or_classes_implode[$i] = implode('}', $body_or_classes[$i]);
  }
  $form_theme = implode('{', $body_or_classes_implode);
  $form_maker_front_end .= '<style>' . $form_theme . '</style>';
  $a = 'form';
  $form_maker_front_end .= '
    <' . $a . ' name="form' . $nodeid . '" action="' . request_uri() . '" method="post" id="form' . $nodeid . '" enctype="multipart/form-data"  onsubmit="check_required(\'submit\', \'' . $nodeid . '\'); return false;" onKeyPress="swallowEnter(event);">
      <div id="' . $nodeid . 'pages" class="wdform_page_navigation" show_title="' . $row->show_title . '" show_numbers="' . $row->show_numbers . '" type="' . $row->pagination . '"></div>
      <input type="hidden" id="counter' . $nodeid . '" value="' . $row->counter . '" name="counter' . $nodeid . '" />';
  $form_maker_front_end .= $untilupload;
  $form_maker_front_end .= '
      <script>
        function form_maker_formOnload' . $nodeid . '() {';
  foreach ($label_type as $key => $type) {
    switch ($type) {
      case 'type_map':
        $form_maker_front_end .= '
          if (document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '")) {
            if_gmap_init(' . $label_id[$key] . ', ' . $nodeid . ');
            for (q = 0; q < 20; q++) {
              if (document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '").getAttribute("long" + q))	{
                w_long = parseFloat(document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '").getAttribute("long" + q));
                w_lat = parseFloat(document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '").getAttribute("lat" + q));
                w_info = parseFloat(document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '").getAttribute("info" + q));
                add_marker_on_map(' . $label_id[$key] . ', q, w_long, w_lat, w_info,' . $nodeid . ', false);
              }
            }
          }';
        break;

      case 'type_mark_map':
        $form_maker_front_end .= '
          if (document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '")) {
            if (!document.getElementById("' . $label_id[$key] . '_long' . $nodeid . '")) {
              var longit = document.createElement(\'input\');
              longit.setAttribute("type", \'hidden\');
              longit.setAttribute("id", \'' . $label_id[$key] . '_long' . $nodeid . '\');
              longit.setAttribute("name", \'' . $label_id[$key] . '_long' . $nodeid . '\');
              var latit = document.createElement(\'input\');
              latit.setAttribute("type", \'hidden\');
              latit.setAttribute("id", \'' . $label_id[$key] . '_lat' . $nodeid . '\');
              latit.setAttribute("name", \'' . $label_id[$key] . '_lat' . $nodeid . '\');
              document.getElementById("' . $label_id[$key] . '_element_section' . $nodeid . '").appendChild(longit);
              document.getElementById("' . $label_id[$key] . '_element_section' . $nodeid . '").appendChild(latit);
              if_gmap_init(' . $label_id[$key] . ', ' . $nodeid . ');
              w_long = parseFloat(document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '").getAttribute("long0"));
              w_lat = parseFloat(document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '").getAttribute("lat0"));
              w_info = parseFloat(document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '").getAttribute("info0"));
              longit.value = w_long;
              latit.value = w_lat;
              add_marker_on_map(' . $label_id[$key] . ', 0, w_long, w_lat, w_info, ' . $nodeid . ', true);
            }
          }';
        break;

      case 'type_captcha':
        $form_maker_front_end .= '
          if (document.getElementById("_wd_captcha' . $nodeid . '")) {
            captcha_refresh("_wd_captcha", "' . $nodeid . '");
          }';
        break;

      case 'type_recaptcha':
        $is_recaptcha = TRUE;
        break;

      case 'type_radio':
      case 'type_checkbox':
        $form_maker_front_end .= '
          if (document.getElementById("' . $label_id[$key] . '_randomize' . $nodeid . '")) {
            if (document.getElementById("' . $label_id[$key] . '_randomize' . $nodeid . '").value == "yes") {
              choises_randomize(\'' . $label_id[$key] . '\', \'' . $nodeid . '\');
            }
          }';
        break;

      default:
        break;
    }
  }
  $form_maker_front_end .= '
        }
      </script>';
  drupal_add_js(array(
    'form_maker' => array(
      'form_maker_nodeid' => $nodeid,
      'form_maker_request_uri' => request_uri(),
      )), 'setting');
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_frontend.js');
  drupal_add_js('if (window.form_maker_before_load) {form_maker_before_load();}', array('type' => 'inline', 'scope' => 'footer'));
  $form_maker_front_end .= '      <script>var form_maker_formOldFunctionOnLoad' . $nodeid . ' = null; form_maker_formLoadBody' . $nodeid . '();
    form_view_count' . $nodeid . ' = 0;
    for (i = 1; i <= 30; i++) {
      if (document.getElementById("' . $nodeid . 'form_view" + i)) {
        (form_view_count' . $nodeid . ') ++;
        form_view_max' . $nodeid . ' = i;
        document.getElementById("' . $nodeid . 'form_view" + i).parentNode.removeAttribute("style");
      }
    }
    if (form_view_count' . $nodeid . ' > 1) {
      for (i = 1; i <= form_view_max' . $nodeid . '; i++) {
        if (document.getElementById("' . $nodeid . 'form_view" + i)) {
          first_form_view' . $nodeid . ' = i;
          break;
        }
      }
      generate_page_nav_frontend(first_form_view' . $nodeid . ', "' . $nodeid . '", form_view_count' . $nodeid . ', form_view_max' . $nodeid . ');
    }
    function form_maker_formAddToOnload' . $nodeid . '() {
      if (form_maker_formOldFunctionOnLoad' . $nodeid . ') {
        form_maker_formOldFunctionOnLoad' . $nodeid . '();
      }
      form_maker_formOnload' . $nodeid . '();
    }

    function form_maker_formLoadBody' . $nodeid . '() {
      form_maker_formOldFunctionOnLoad' . $nodeid . ' = window.onload;
      window.onload = form_maker_formAddToOnload' . $nodeid . ';
    }</script>';
  if (isset($_POST["captcha_input"])) {
    $captcha_input = $_POST["captcha_input"];
  }
  if (isset($_POST["recaptcha_response_field"])) {
    $recaptcha_response_field = $_POST["recaptcha_response_field"];
  }
  if (isset($_POST["counter" . $nodeid])) {
    $counter = $_POST["counter" . $nodeid];
  }
  $old_key = -1;
  if (isset($counter)) {
    foreach ($label_type as $key => $type) {
      switch ($type) {
        case "type_text":
        case "type_number":
        case "type_submitter_mail": {          if (isset($_POST[$label_id[$key] . '_element' . $nodeid])) {
            drupal_add_js('if (document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '"))
              if (document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '").title != "' . addslashes($_POST[$label_id[$key] . '_element' . $nodeid]) . '") {
                document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key] . '_element' . $nodeid]) . '";
                document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '").className = "input_active";
              }', array('type' => 'inline',  'scope' => 'footer'));          }
          break;

        }
        case "type_textarea": {
          $order = array("\r\n", "\n", "\r");          if (isset($_POST[$label_id[$key] . '_element' . $nodeid])) {
            drupal_add_js('if (document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '"))
              if (document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '").title != "' . str_replace($order, '\n', addslashes($_POST[$label_id[$key] . '_element' . $nodeid])) . '") {
                document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '").innerHTML = "' . str_replace($order, '\n', addslashes($_POST[$label_id[$key] . '_element' . $nodeid])) . '";
                document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '").className = "input_active";
              }', array('type' => 'inline',  'scope' => 'footer'));          }
          break;

        }
        case "type_name": {
          if (isset($_POST[$label_id[$key] . '_element_title' . $nodeid])) {
            drupal_add_js('if (document.getElementById("' . $label_id[$key] . '_element_first' . $nodeid . '")) {
              if (document.getElementById("' . $label_id[$key] . '_element_title' . $nodeid . '").title != "' . addslashes($_POST[$label_id[$key] . '_element_title' . $nodeid]) . '") {
                document.getElementById("' . $label_id[$key] . '_element_title' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key] . '_element_title' . $nodeid]) . '";
                document.getElementById("' . $label_id[$key] . '_element_title' . $nodeid . '").className = "input_active";
              }
              if (document.getElementById("' . $label_id[$key] . '_element_first' . $nodeid . '").title != "' . addslashes($_POST[$label_id[$key] . '_element_first' . $nodeid]) . '") {
                document.getElementById("' . $label_id[$key] . '_element_first' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key] . '_element_first' . $nodeid]) . '";
                document.getElementById("' . $label_id[$key] . '_element_first' . $nodeid . '").className = "input_active";
              }
              if (document.getElementById("' . $label_id[$key] . '_element_last' . $nodeid . '").title != "' . addslashes($_POST[$label_id[$key] . '_element_last' . $nodeid]) . '") {
                document.getElementById("' . $label_id[$key] . '_element_last' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key] . '_element_last' . $nodeid]) . '";
                document.getElementById("' . $label_id[$key] . '_element_last' . $nodeid . '").className = "input_active";
              }
              if (document.getElementById("' . $label_id[$key] . '_element_middle' . $nodeid . '").title != "' . addslashes($_POST[$label_id[$key] . '_element_middle' . $nodeid]) . '") {
                document.getElementById("' . $label_id[$key] . '_element_middle' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key] . '_element_middle' . $nodeid]) . '";
                document.getElementById("' . $label_id[$key] . '_element_middle' . $nodeid . '").className = "input_active";
              }

            }', array('type' => 'inline',  'scope' => 'footer'));
          }
          else {            if (isset($_POST[$label_id[$key] . '_element_first' . $nodeid]) && isset($_POST[$label_id[$key] . '_element_last' . $nodeid])) {
              drupal_add_js('if (document.getElementById("' . $label_id[$key] . '_element_first' . $nodeid . '")) {
                if (document.getElementById("' . $label_id[$key] . '_element_first' . $nodeid . '").title != "' . addslashes($_POST[$label_id[$key] . '_element_first' . $nodeid]) . '") {
                  document.getElementById("' . $label_id[$key] . '_element_first' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key] . '_element_first' . $nodeid]) . '";
                  document.getElementById("' . $label_id[$key] . '_element_first' . $nodeid . '").className = "input_active";
                }
                if (document.getElementById("' . $label_id[$key] . '_element_last' . $nodeid . '").title != "' . addslashes($_POST[$label_id[$key] . '_element_last' . $nodeid]) . '") {
                  document.getElementById("' . $label_id[$key] . '_element_last' . $nodeid . '").value="' . addslashes($_POST[$label_id[$key] . '_element_last' . $nodeid]) . '";
                  document.getElementById("' . $label_id[$key] . '_element_last' . $nodeid . '").className = "input_active";
                }
              }', array('type' => 'inline',  'scope' => 'footer'));            }
          }
          break;

        }
        case "type_phone": {          if (isset($_POST[$label_id[$key] . '_element_first' . $nodeid]) && isset($_POST[$label_id[$key] . '_element_last' . $nodeid])) {
            drupal_add_js('if (document.getElementById("' . $label_id[$key] . '_element_first' . $nodeid . '")) {
              if (document.getElementById("' . $label_id[$key] . '_element_first' . $nodeid . '").title != "' . addslashes($_POST[$label_id[$key] . '_element_first' . $nodeid]) . '") {
                document.getElementById("' . $label_id[$key] . '_element_first' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key] . '_element_first' . $nodeid]) . '";
                document.getElementById("' . $label_id[$key] . '_element_first' . $nodeid . '").className = "input_active";
              }
              if (document.getElementById("' . $label_id[$key] . '_element_last' . $nodeid . '").title != "' . addslashes($_POST[$label_id[$key] . '_element_last' . $nodeid]) . '") {
                document.getElementById("' . $label_id[$key] . '_element_last' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key] . '_element_last' . $nodeid]) . '";
                document.getElementById("' . $label_id[$key] . '_element_last' . $nodeid . '").className = "input_active";
              }
            }', array('type' => 'inline',  'scope' => 'footer'));          }
          break;

        }
        case "type_address": {
          if ($key > $old_key) {
            if (isset($_POST[$label_id[$key] . '_street1' . $nodeid])) {
              drupal_add_js('if (document.getElementById("' . $label_id[$key] . '_street1' . $nodeid . '")) {
                document.getElementById("' . $label_id[$key] . '_street1' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key] . '_street1' . $nodeid]) . '";
                document.getElementById("' . $label_id[$key] . '_street2' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key + 1] . '_street2' . $nodeid]) . '";
                document.getElementById("' . $label_id[$key] . '_city' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key + 2] . '_city' . $nodeid]) . '";
                document.getElementById("' . $label_id[$key] . '_state' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key + 3] . '_state' . $nodeid]) . '";
                document.getElementById("' . $label_id[$key] . '_postal' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key + 4] . '_postal' . $nodeid]) . '";
                document.getElementById("' . $label_id[$key] . '_country' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key + 5] . '_country' . $nodeid]) . '";
              }', array('type' => 'inline',  'scope' => 'footer'));
            }
            $old_key = $key + 5;
          }
          break;

        }
        case "type_checkbox": {
          $is_other = FALSE;          if (isset($_POST[$label_id[$key] . '_allow_other' . $nodeid])) {
            if ($_POST[$label_id[$key] . '_allow_other' . $nodeid] == "yes") {
              if (isset($_POST[$label_id[$key] . '_other_input' . $nodeid])) {
                $other_element = addslashes($_POST[$label_id[$key] . '_other_input' . $nodeid]);
              }
              if (isset($_POST[$label_id[$key] . '_allow_other_num' . $nodeid])) {
                $other_element_id = addslashes($_POST[$label_id[$key] . '_allow_other_num' . $nodeid]);
                $is_other = TRUE;
              }
            }
          }
          drupal_add_js('if (document.getElementById("' . $label_id[$key] . '_other_input' . $nodeid . '")) {
            document.getElementById("' . $label_id[$key] . '_other_input' . $nodeid . '").parentNode.removeChild(document.getElementById("' . $label_id[$key] . '_other_br' . $nodeid . '"));
            document.getElementById("' . $label_id[$key] . '_other_input' . $nodeid . '").parentNode.removeChild(document.getElementById("' . $label_id[$key] . '_other_input' . $nodeid . '"));
          }
          for (k = 0; k < 30; k++) {
            if (document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '" + k)) {
              document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '" + k).removeAttribute("checked");
            }
            else {
              break;
            }
          }', array('type' => 'inline',  'scope' => 'footer'));
          for ($j = 0; $j < 100; $j++) {
            if (isset($_POST[$label_id[$key] . '_element' . $nodeid . $j])) {
              $element = $_POST[$label_id[$key] . '_element' . $nodeid . $j];
              drupal_add_js('document.getElementById("' . $label_id[$key] . '_element' . $nodeid . $j . '").setAttribute("checked", "checked")', array('type' => 'inline',  'scope' => 'footer'));
            }
          }
          if (isset($_POST[$label_id[$key] . '_other_input' . $nodeid])) {
            if ($is_other) {
              drupal_add_js('show_other_input("' . $label_id[$key] . '","' . $nodeid . '");
                document.getElementById("' . $label_id[$key] . '_other_input' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key] . '_other_input' . $nodeid]) . '";
                ', array('type' => 'inline',  'scope' => 'footer'));
            }
          }
          break;

        }
        case "type_radio": {
          $is_other = FALSE;
          if (isset($_POST[$label_id[$key] . '_allow_other' . $nodeid])) {
            if ($_POST[$label_id[$key] . '_allow_other' . $nodeid] == "yes") {
              if (isset($_POST[$label_id[$key] . '_other_input' . $nodeid])) {
                $other_element = addslashes($_POST[$label_id[$key] . '_other_input' . $nodeid]);
                $is_other = TRUE;
              }
            }
          }
          if (isset($_POST[$label_id[$key] . '_element' . $nodeid])) {
            drupal_add_js('if (document.getElementById("' . $label_id[$key] . '_other_input' . $nodeid . '")) {
              document.getElementById("' . $label_id[$key] . '_other_input' . $nodeid . '").parentNode.removeChild(document.getElementById("' . $label_id[$key] . '_other_br' . $nodeid . '"));
              document.getElementById("' . $label_id[$key] . '_other_input' . $nodeid . '").parentNode.removeChild(document.getElementById("' . $label_id[$key] . '_other_input' . $nodeid . '"));
            }
            for (k = 0; k < 50; k++) {
              if (document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '" + k)) {
                document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '" + k).removeAttribute("checked");
                if (document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '" + k).value == "' . addslashes($_POST[$label_id[$key] . '_element' . $nodeid]) . '") {
                  document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '" + k).setAttribute("checked", "checked");
                }
              }
              else {
                break;
              }
            }', array('type' => 'inline',  'scope' => 'footer'));
          }
          if (isset($_POST[$label_id[$key] . '_element' . $nodeid])) {
            if ($is_other) {
              drupal_add_js('show_other_input("' . $label_id[$key] . '","' . $nodeid . '");
                document.getElementById("' . $label_id[$key] . '_other_input' . $nodeid . '").value="' . addslashes($_POST[$label_id[$key] . '_other_input' . $nodeid]) . '";
              ', array('type' => 'inline',  'scope' => 'footer'));
            }
          }
          break;

        }
        case "type_time": {
          if (isset($_POST[$label_id[$key] . "_ss" . $nodeid])) {
            if (isset($_POST[$label_id[$key] . "_hh" . $nodeid]) && isset($_POST[$label_id[$key] . "_mm" . $nodeid])) {
              drupal_add_js('if (document.getElementById("' . $label_id[$key] . '_hh' . $nodeid . '")) {
                document.getElementById("' . $label_id[$key] . '_hh' . $nodeid . '").value = "' . $_POST[$label_id[$key] . '_hh' . $nodeid] . '";
                document.getElementById("' . $label_id[$key] . '_mm' . $nodeid . '").value = "' . $_POST[$label_id[$key] . '_mm' . $nodeid] . '";
                document.getElementById("' . $label_id[$key] . '_ss' . $nodeid . '").value = "' . $_POST[$label_id[$key] . '_ss' . $nodeid] . '";
              }', array('type' => 'inline',  'scope' => 'footer'));
            }
          }
          else {
            if (isset($_POST[$label_id[$key] . "_hh" . $nodeid]) && isset($_POST[$label_id[$key] . "_mm" . $nodeid])) {
              drupal_add_js('if (document.getElementById("' . $label_id[$key] . '_hh' . $nodeid . '")) {
                document.getElementById("' . $label_id[$key] . '_hh' . $nodeid . '").value = "' . $_POST[$label_id[$key] . '_hh' . $nodeid] . '";
                document.getElementById("' . $label_id[$key] . '_mm' . $nodeid . '").value = "' . $_POST[$label_id[$key] . '_mm' . $nodeid] . '";
              }', array('type' => 'inline',  'scope' => 'footer'));
            }
          }
          if (isset($_POST[$label_id[$key] . '_am_pm' . $nodeid])) {
            drupal_add_js('if (document.getElementById("' . $label_id[$key] . '_am_pm' . $nodeid . '")) {
              document.getElementById("' . $label_id[$key] . '_am_pm' . $nodeid . '").value = "' . $_POST[$label_id[$key] . '_am_pm' . $nodeid] . '";
            }', array('type' => 'inline',  'scope' => 'footer'));
          }
          break;

        }
        case "type_date_fields": {
          if (isset($_POST[$label_id[$key] . '_day' . $nodeid])) {
            // $date_fields = explode('-', $_POST[$label_id[$key] . '_element' . $nodeid]);
            /*drupal_add_js('if (document.getElementById("' . $label_id[$key] . '_day' . $nodeid . '")) {
              document.getElementById("' . $label_id[$key] . '_day' . $nodeid . '").value = "' . $_POST[$label_id[$key] . '_day' . $nodeid] . '";
              // document.getElementById("' . $label_id[$key] . '_day' . $nodeid . '").value = "' . $date_fields[0] . '";
              // document.getElementById("' . $label_id[$key] . '_month' . $nodeid . '").value = "' . $date_fields[1] . '";
              // document.getElementById("' . $label_id[$key] . '_year' . $nodeid . '").value = "' . $date_fields[2] . '";
            }', array('type' => 'inline',  'scope' => 'footer'));*/
            drupal_add_js('if (document.getElementById("' . $label_id[$key] . '_day' . $nodeid . '")) {
              document.getElementById("' . $label_id[$key] . '_day' . $nodeid . '").value = "' . $_POST[$label_id[$key] . '_day' . $nodeid] . '";
              document.getElementById("' . $label_id[$key] . '_month' . $nodeid . '").value = "' . $_POST[$label_id[$key] . '_month' . $nodeid] . '";
              document.getElementById("' . $label_id[$key] . '_year' . $nodeid . '").value = "' . $_POST[$label_id[$key] . '_year' . $nodeid] . '";
            }', array('type' => 'inline',  'scope' => 'footer'));
          }
          break;

        }
        case "type_date":
        case "type_own_select":
        case "type_country": {
          if (isset($_POST[$label_id[$key] . '_element' . $nodeid])) {
            drupal_add_js('if (document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '")) {
              document.getElementById("' . $label_id[$key] . '_element' . $nodeid . '").value = "' . addslashes($_POST[$label_id[$key] . '_element' . $nodeid]) . '";
            }', array('type' => 'inline',  'scope' => 'footer'));
            break;
          }
        }
        default: {
          break;
        }
      }
    }
  }
  $form_maker_front_end .= '
    </form>';
  if ($is_recaptcha) {
    require_once "form_maker_recaptchalib.inc";
    drupal_add_js('var RecaptchaOptions = {theme: "' . $row->recaptcha_theme . '"};', array('type' => 'inline'));
    $form_maker_front_end .= '
      <div id="main_recaptcha" style="display:none;">';
    if ($row->public_key) {
      $publickey = $row->public_key;
    }
    else {
      $publickey = '0';
    }
    $error = NULL;
    $form_maker_front_end .= recaptcha_get_html($publickey, $error);
    $form_maker_front_end .= '
      </div>';
    drupal_add_js('recaptcha_html = document.getElementById("main_recaptcha").innerHTML.replace(\'Recaptcha.widget = Recaptcha.jQuery("recaptcha_widget_div"); Recaptcha.challenge_callback();\', "");
      document.getElementById("main_recaptcha").innerHTML = "";
      if (document.getElementById("wd_recaptcha' . $nodeid . '")) {
        document.getElementById("wd_recaptcha' . $nodeid . '").innerHTML = recaptcha_html;
      }', array('type' => 'inline', 'scope' => 'footer'));
  }
  return $form_maker_front_end;
}

/**
 * Save fields.
 */
function form_maker_savedata($id, $front_end) {  
  require_once "form_maker_recaptchalib.inc";
  $all_files = array();
  if (isset($_POST["captcha_input"])) {
    $captcha_input = check_plain($_POST["captcha_input"]);
  }
  if (!isset($_SESSION['form_maker_captcha'])) {
    $_SESSION['form_maker_captcha'] = '';
  }
  $value = db_query("SELECT * FROM {form_maker_table} WHERE vid = :vid", array(':vid' => $id));
  $form = $value->fetchObject();
  if ((strpos($form->form_front, 'type_captcha') != FALSE) && (!isset($_POST["captcha_input"]))) {
    drupal_set_message(t('Nothing was submitted'), 'error', FALSE);
    return;
  }
  if (isset($form->counter)) {
    $counter = $form->counter;
  }
  if (isset($form->counter)) {
    if (isset($_POST["captcha_input"])) {
      if ($captcha_input == $_SESSION['form_maker_captcha']) {
        $all_files = form_maker_save_db($counter, $id, $front_end);
        if (is_numeric($all_files)) {
          form_maker_remove($all_files);
        }
        else {
          if (isset($counter)) {
            form_maker_sendmail($counter, $all_files, $id, $front_end);
          }
        }
      }
      else {        drupal_set_message(t('Error, incorrect Security code.'), 'error', FALSE);
      }
    }
    elseif (isset($_POST["recaptcha_response_field"])) {
      $privatekey = $form->private_key;
      $resp = recaptcha_check_answer($privatekey, $_SERVER["REMOTE_ADDR"], $_POST["recaptcha_challenge_field"], check_plain($_POST["recaptcha_response_field"]));
      if ($resp->is_valid) {
        $all_files = form_maker_save_db($counter, $id, $front_end);
        if (is_numeric($all_files)) {
          form_maker_remove($all_files);
        }
        else {
          if (isset($counter)) {
            form_maker_sendmail($counter, $all_files, $id, $front_end);
          }
        }
      }
      else {        drupal_set_message(t('Error, incorrect Security code.'), 'error', FALSE);
      }
    }
    else {
      $all_files = form_maker_save_db($counter, $id, $front_end);
      if (is_numeric($all_files)) {
        form_maker_remove($all_files);
      }
      else {
        if (isset($counter)) {
          form_maker_sendmail($counter, $all_files, $id, $front_end);
        }
      }
    }
    return $all_files;
  }
  return $all_files;
}

/**
 * Save fields in data base.
 */
function form_maker_save_db($counter, $id, $front_end) {
  $chgnac = TRUE;
  $all_files = array();
  $id_old = $id;
  $form = db_query("SELECT * FROM {form_maker_table} WHERE vid=:vid", array(':vid' => $id))->fetchObject();
  if (!$form->form_front) {
    $id = '';
  }
  $label_id = array();
  $label_label = array();
  $label_type = array();
  $label_all = explode('#****#', $form->label_order);
  
  $label_all = array_slice($label_all, 0, count($label_all) - 1);
  foreach ($label_all as $key => $label_each) {
    $label_id_each = explode('#**id**#', $label_each);
    array_push($label_id, $label_id_each[0]);
    $label_order_each = explode('#**label**#', $label_id_each[1]);
    array_push($label_label, $label_order_each[0]);
    array_push($label_type, $label_order_each[1]);
  }
  $max = db_query("SELECT MAX(group_id) FROM {form_maker_submits_table}")->fetchField();
  foreach ($label_type as $key => $type) {
    $value = '';
    if ($type == "type_submit_reset" || $type == "type_map" || $type == "type_editor" || $type == "type_captcha" || $type == "type_recaptcha" || $type == "type_button") {
      continue;
    }
    $i = $label_id[$key];
    if ($type != "type_address") {
      if (isset($_POST[$i . "_type" . $id])) {
        $deleted = check_plain($_POST[$i . "_type" . $id]);
      }
      else {
        break;
      }
    }
    switch ($type) {
      case "type_text":
      case "type_password":
      case "type_textarea":
      case "type_submitter_mail":
      case "type_date":
      case "type_own_select":
      case "type_country":
      case "type_number": {
        $value = check_plain($_POST[$i . '_element' . $id]);
        break;

      }
      case "type_mark_map": {
        $value = check_plain($_POST[$i . "_long" . $id]) . '***map***' . check_plain($_POST[$i . "_lat" . $id]);
        break;

      }
      case "type_date_fields": {
        $value = check_plain($_POST[$i . "_day" . $id]) . '-' . check_plain($_POST[$i . "_month" . $id]) . '-' . check_plain($_POST[$i . "_year" . $id]);
        break;

      }
      case "type_time": {
        $ss = check_plain($_POST[$i . "_ss" . $id]);
        if (isset($_POST[$i . "_ss" . $id])) {
          $value = check_plain($_POST[$i . "_hh" . $id]) . ':' . check_plain($_POST[$i . "_mm" . $id]) . ':' . check_plain($_POST[$i . "_ss" . $id]);
        }
        else {
          $value = check_plain($_POST[$i . "_hh" . $id]) . ':' . check_plain($_POST[$i . "_mm" . $id]);
        }
        $am_pm = check_plain($_POST[$i . "_am_pm" . $id]);
        if (isset($_POST[$i . "_am_pm" . $id])) {
          $value = $value . ' ' . check_plain($_POST[$i . "_am_pm" . $id]);
        }
        break;

      }
      case "type_phone": {
        $value = check_plain($_POST[$i . "_element_first" . $id]) . ' ' . check_plain($_POST[$i . "_element_last" . $id]);
        break;

      }
      case "type_name": {
        $element_title = check_plain($_POST[$i . "_element_title" . $id]);
        if (isset($_POST[$i . "_element_title" . $id])) {
          $value = check_plain($_POST[$i . "_element_title" . $id]) . ' ' . check_plain($_POST[$i . "_element_first" . $id]) . ' ' . check_plain($_POST[$i . "_element_last" . $id]) . ' ' . check_plain($_POST[$i . "_element_middle" . $id]);
        }
        else {
          $value = check_plain($_POST[$i . "_element_first" . $id]) . ' ' . check_plain($_POST[$i . "_element_last" . $id]);
        }
        break;
      }
      case "type_file_upload": {
        $file = $_FILES[$i . '_file' . $id];
        if ($file['name']) {
          $untilupload = $form->form;
          $pos1 = strpos($untilupload, "***destinationskizb" . $i . "***");
          $pos2 = strpos($untilupload, "***destinationverj" . $i . "***");
          $destination = drupal_substr($untilupload, $pos1 + (23 + (drupal_strlen($i) - 1)), $pos2 - $pos1 - (23 + (drupal_strlen($i) - 1)));
          $pos1 = strpos($untilupload, "***extensionskizb" . $i . "***");
          $pos2 = strpos($untilupload, "***extensionverj" . $i . "***");
          $extension = drupal_substr($untilupload, $pos1 + (21 + (drupal_strlen($i) - 1)), $pos2 - $pos1 - (21 + (drupal_strlen($i) - 1)));
          $pos1 = strpos($untilupload, "***max_sizeskizb" . $i . "***");
          $pos2 = strpos($untilupload, "***max_sizeverj" . $i . "***");
          $max_size = drupal_substr($untilupload, $pos1 + (20 + (drupal_strlen($i) - 1)), $pos2 - $pos1 - (20 + (drupal_strlen($i) - 1)));
          $file_name = $file['name'];
          if ($destination == '') {
            $destination = drupal_get_path('module', 'form_maker') . "/uploads";
          }
          /*$destination = JPATH_SITE.DS.$_POST[$i.'_destination');
           $extension = $_POST[$i.'_extension');
           $max_size = $_POST[$i.'_max_size');*/
          $file_size = $file['size'];
          if ($file_size > $max_size * 1024) {
            echo "<script> alert('The file exceeds the allowed size of" . $max_size . " KB');</script>";
            return ($max + 1);
          }  
          $uploaded_file_name_parts = explode('.', $file_name);
          $uploaded_file_extension = array_pop($uploaded_file_name_parts);
          $to = drupal_strlen($file_name) - drupal_strlen($uploaded_file_extension) - 1;
          $file_name_free = drupal_substr($file_name, 0, $to);
          $invalid_file_exts = explode(',', $extension);
          $ext_ok = FALSE;
          foreach ($invalid_file_exts as $key => $value) {
            if (is_numeric(strpos(strtolower($value), strtolower($uploaded_file_extension)))) {
              $ext_ok = TRUE;
            }
          }
          if ($ext_ok == FALSE) {
            echo "<script> alert('Sorry, you are not allowed to upload this type of file.');</script>";
            return ($max + 1);
          }
          $file_temp = $file['tmp_name'];
          $p = 1;
          while (file_exists($destination . "/" . $file_name)) {
            $to = drupal_strlen($file['name']) - drupal_strlen($uploaded_file_extension) - 1;
            $file_name = drupal_substr($file_name, 0, $to) . '(' . $p . ').' . $uploaded_file_extension;
            $p++;
          }
          // if (is_dir(ABSPATH . $destination)) {
            if (!move_uploaded_file($file_temp, $destination . '/' . $file_name)) {
              echo "<script> alert('Error, file cannot be moved.');</script>";
              return ($max + 1);
            }
          // }
          // else {
            // echo "<script> alert('Error, file cannot be moved.');</script>";
            // return ($max + 1);
          // }
          $value = base_path() . $destination . '/' . $file_name . '*@@url@@*';
          $file['tmp_name'] = $destination . "/" . $file_name;
          array_push($all_files, $file);
        }
        break;

      }
      case 'type_address': {
        $value = '*#*#*#';
        if (isset($_POST[$i . "_street1" . $id])) {
          $value = check_plain($_POST[$i . "_street1" . $id]);
          break;
        }
        if (isset($_POST[$i . "_street2" . $id])) {
          $value = check_plain($_POST[$i . "_street2" . $id]);
          break;
        }
        if (isset($_POST[$i . "_city" . $id])) {
          $value = check_plain($_POST[$i . "_city" . $id]);
          break;
        }
        if (isset($_POST[$i . "_state" . $id])) {
          $value = check_plain($_POST[$i . "_state" . $id]);
          break;
        }
        if (isset($_POST[$i . "_postal" . $id])) {
          $value = check_plain($_POST[$i . "_postal" . $id]);
          break;
        }
        if (isset($_POST[$i . "_country" . $id])) {
          $value = check_plain($_POST[$i . "_country" . $id]);
          break;
        }
        break;

      }
      case "type_hidden": {
        $value = check_plain($_POST[$label_label[$key]]);
        break;

      }
      case "type_radio": {
        if (isset($_POST[$i . "_other_input" . $id])) {
          $element = check_plain($_POST[$i . "_other_input" . $id]);
          $value = $element;
          break;
        }
        $value = check_plain($_POST[$i . "_element" . $id]);
        break;

      }
      case "type_checkbox": {
        $start = -1;
        $value = '';
        for ($j = 0; $j < 100; $j++) {
          //$element=$_POST[$i."_element".$id.$j];
          if (isset($_POST[$i . "_element" . $id . $j])) {
            $start = $j;
            break;
          }
        }
        $other_element_id = -1;
        $is_other = $_POST[$i . "_allow_other" . $id];
        if ($is_other == "yes") {
          $other_element_id = check_plain($_POST[$i . "_allow_other_num" . $id]);
        }
        if ($start != -1) {
          for ($j = $start; $j < 100; $j++) {
            //$element=$_POST[$i."_element".$id.$j];
            if (isset($_POST[$i . "_element" . $id . $j])) {
              if ($j == $other_element_id) {
                $value = $value . check_plain($_POST[$i . "_other_input" . $id]) . '***br***';
              }
              else {
                $value = $value . check_plain($_POST[$i . "_element" . $id . $j]) . '***br***';
              }
            }
          }
        }
        break;
      }
    }
    if ($type == "type_address") {
      if ($value == '*#*#*#') {
        break;
      }
    }
    if (isset($_POST[$i . "_unique" . $id])) {
      $unique_element = check_plain($_POST[$i . "_unique" . $id]);
      if ($unique_element == 'yes') {
        $unique = db_query("SELECT id FROM {form_maker_submits_table} WHERE form_id=:form_id AND element_label=:element_label AND element_value=:element_value", array(':form_id' => $id_old, ':element_label' => $i, ':element_value' => addslashes($value)))->fetchField();
        if ($unique) {
          echo "<script> alert('This field " . $label_label[$key] . " requires a unique entry and this value was already submitted.');</script>";
          return ($max + 1);
        }
      }
    }
    
    // $_SERVER['REMOTE_ADDR'];
    $ip = ip_address();
    $save_or_no = db_insert('form_maker_submits_table')
    ->fields(array(
      'form_id' => $id_old,
      'element_label' => $i,
      'element_value' => addslashes($value),
      'group_id' => $max + 1,
      'date' => date('Y-m-d H:i:s'),
      'ip' => $ip,
      ))
    ->execute();
    // var_dump($save_or_no);
    // continue;
    if (!$save_or_no) {
      return FALSE;
    }
    $chgnac = FALSE;
  }
  if ($chgnac) {
    if (count($all_files) == 0) {
      drupal_set_message(t('Nothing was submitted'), 'error', FALSE);
    }
  }
  return $all_files;
}

/**
 * Send mail.
 */
function form_maker_sendmail($counter, $all_files, $id, $front_end) {
  $value = db_query("SELECT * FROM {form_maker_table} WHERE vid=:vid", array(':vid' => $id));
  $row = $value->fetchObject();
  if (!$row->form_front) {
    $id = '';
  }
  $label_order_original = array();
  $label_order_ids = array();
  $label_label = array();
  $label_type = array();
  $cc = array();
  $row_mail_one_time = 1;
  $label_all = explode('#****#', $row->label_order);
  $label_all = array_slice($label_all, 0, count($label_all) - 1);
  foreach ($label_all as $key => $label_each) {
    $label_id_each = explode('#**id**#', $label_each);
    $label_id = $label_id_each[0];
    array_push($label_order_ids, $label_id);
    $label_order_each = explode('#**label**#', $label_id_each[1]);
    $label_order_original[$label_id] = $label_order_each[0];
    array_push($label_label, $label_order_each[0]);
    array_push($label_type, $label_order_each[1]);
  }
  $list = '<table border="1" cellpadding="3" cellspacing="0" style="width:600px;">';
  foreach ($label_order_ids as $key => $label_order_id) {
    $i = $label_order_id;
    if (isset($_POST[$i . "_type" . $id])) {
      $type = $_POST[$i . "_type" . $id];
    }
    else {
      $type = '';
    }
    if (isset($_POST[$i . "_type" . $id])) {
      if ($type != "type_map" and  $type != "type_submit_reset" and  $type != "type_editor" and  $type != "type_captcha" and  $type != "type_recaptcha" and  $type != "type_button") {
        $element_label = $label_order_original[$i];
        switch ($type) {
          case "type_text":
          case "type_password":
          case "type_textarea":
          case "type_date":
          case "type_own_select":
          case "type_country":
          case "type_number": {
            $element = $_POST[$i . "_element" . $id];
            if (isset($_POST[$i . "_element" . $id])) {
              $list = $list . '<tr valign="top"><td >' . $element_label . '</td><td ><pre style="font-family:inherit; margin:0px; padding:0px">' . $element . '</pre></td></tr>';
            }
            break;
          }
          case "type_submitter_mail": {
            $element = $_POST[$i . "_element" . $id];
            if (isset($_POST[$i . "_element" . $id])) {
              $list = $list . '<tr valign="top"><td >' . $element_label . '</td><td ><pre style="font-family:inherit; margin:0px; padding:0px">' . $element . '</pre></td></tr>';
              if ($_POST[$i . "_send" . $id] == "yes")
                array_push($cc, $element);
            }
            break;
          }
          case "type_time": {
            $hh = $_POST[$i . "_hh" . $id];
            if (isset($_POST[$i . "_hh" . $id])) {
              $ss = $_POST[$i . "_ss" . $id];
              if (isset($_POST[$i . "_ss" . $id]))
                $list = $list . '<tr valign="top"><td >' . $element_label . '</td><td >' . $_POST[$i . "_hh" . $id] . ':' . $_POST[$i . "_mm" . $id] . ':' . $_POST[$i . "_ss" . $id];
              else
                $list = $list . '<tr valign="top"><td >' . $element_label . '</td><td >' . $_POST[$i . "_hh" . $id] . ':' . $_POST[$i . "_mm" . $id];
              $am_pm = $_POST[$i . "_am_pm" . $id];
              if (isset($_POST[$i . "_am_pm" . $id]))
                $list = $list . ' ' . $_POST[$i . "_am_pm" . $id] . '</td></tr>';
              else
                $list = $list . '</td></tr>';
            }
            break;
          }
          case "type_phone": {
            $element_first = $_POST[$i . "_element_first" . $id];
            if (isset($_POST[$i . "_element_first" . $id])) {
              $list = $list . '<tr valign="top"><td >' . $element_label . '</td><td >' . $_POST[$i . "_element_first" . $id] . ' ' . $_POST[$i . "_element_last" . $id] . '</td></tr>';
            }
            break;
          }
          case "type_name": {
            $element_first = $_POST[$i . "_element_first" . $id];
            if (isset($_POST[$i . "_element_first" . $id])) {
              $element_title = $_POST[$i . "_element_title" . $id];
              if (isset($_POST[$i . "_element_title" . $id]))
                $list = $list . '<tr valign="top"><td >' . $element_label . '</td><td >' . $_POST[$i . "_element_title" . $id] . ' ' . $_POST[$i . "_element_first" . $id] . ' ' . $_POST[$i . "_element_last" . $id] . ' ' . $_POST[$i . "_element_middle" . $id] . '</td></tr>';
              else
                $list = $list . '<tr valign="top"><td >' . $element_label . '</td><td >' . $_POST[$i . "_element_first" . $id] . ' ' . $_POST[$i . "_element_last" . $id] . '</td></tr>';
            }
            break;
          }
          case "type_mark_map": {
            if (isset($_POST[$i . "_long" . $id])) {
              $list = $list . '<tr valign="top"><td >' . $element_label . '</td><td >Longitude:' . $_POST[$i . "_long" . $id] . '<br/>Latitude:' . $_POST[$i . "_lat" . $id] . '</td></tr>';
            }
            break;
          }
          case "type_address": {
            $street1 = $_POST[$i . "_street1" . $id];
            if (isset($_POST[$i . "_street1" . $id])) {
              $list = $list . '<tr valign="top"><td >' . $label_order_original[$i] . '</td><td >' . $_POST[$i . "_street1" . $id] . '</td></tr>';
              $i++;
              $list = $list . '<tr valign="top"><td >' . $label_order_original[$i] . '</td><td >' . $_POST[$i . "_street2" . $id] . '</td></tr>';
              $i++;
              $list = $list . '<tr valign="top"><td >' . $label_order_original[$i] . '</td><td >' . $_POST[$i . "_city" . $id] . '</td></tr>';
              $i++;
              $list = $list . '<tr valign="top"><td >' . $label_order_original[$i] . '</td><td >' . $_POST[$i . "_state" . $id] . '</td></tr>';
              $i++;
              $list = $list . '<tr valign="top"><td >' . $label_order_original[$i] . '</td><td >' . $_POST[$i . "_postal" . $id] . '</td></tr>';
              $i++;
              $list = $list . '<tr valign="top"><td >' . $label_order_original[$i] . '</td><td >' . $_POST[$i . "_country" . $id] . '</td></tr>';
              $i++;
            }
            break;
          }
          case "type_date_fields": {
            $day = $_POST[$i . "_day" . $id];
            if (isset($_POST[$i . "_day" . $id])) {
              $list = $list . '<tr valign="top"><td >' . $element_label . '</td><td >' . $_POST[$i . "_day" . $id] . '-' . $_POST[$i . "_month" . $id] . '-' . $_POST[$i . "_year" . $id] . '</td></tr>';
            }
            break;
          }
          case "type_radio": {
            if (isset($_POST[$i . "_other_input" . $id])) {
              $element = check_plain($_POST[$i . "_other_input" . $id]);
              $list = $list . '<tr valign="top"><td >' . $element_label . '</td><td >' . $_POST[$i . "_other_input" . $id] . '</td></tr>';
              break;
            }
            if (isset($_POST[$i . "_element" . $id])) {
              $element = check_plain($_POST[$i . "_element" . $id]);
              $list = $list . '<tr valign="top"><td >' . $element_label . '</td><td ><pre style="font-family:inherit; margin:0px; padding:0px">' . $element . '</pre></td></tr>';
            }
            break;
          }
          case "type_checkbox": {
            $list = $list . '<tr valign="top"><td >' . $element_label . '</td><td >';
            $start = -1;
            for ($j = 0; $j < 100; $j++) {
              if (isset($_POST[$i . "_element" . $id . $j])) {
                $start = $j;
                break;
              }
            }
            $other_element_id = -1;
            $is_other = $_POST[$i . "_allow_other" . $id];
            if ($is_other == "yes") {
              $other_element_id = $_POST[$i . "_allow_other_num" . $id];
            }
            if ($start != -1) {
              for ($j = $start; $j < 100; $j++) {
                $element = $_POST[$i . "_element" . $id . $j];
                if (isset($_POST[$i . "_element" . $id . $j]))
                  if ($j == $other_element_id) {
                    $list = $list . $_POST[$i . "_other_input" . $id] . '<br>';
                  }
                  else

                    $list = $list . $_POST[$i . "_element" . $id . $j] . '<br>';
              }
              $list = $list . '</td></tr>';
            }
            break;
          }
          default:
            break;
        }
      }
    }
  }
  $list = $list . '</table>';
  $list = wordwrap($list, 70, "\n", TRUE);
  $attachments = array();
  for ($k = 0; $k < count($all_files); $k++) {
    $attachments[$k] = $all_files[$k]['tmp_name'];
    // $attachment[$k] = dirname(__FILE__) . '/uploads/' . $all_files[$k]['name'];v
  }
  $body = $row->admin_script1 . '<br />' . $list . '<br />' . $row->admin_script2;
  $body_user = $row->script1 . '<br />' . $list . '<br />' . $row->script2;
  $params['body'] = $body;
  $params['subject'] = $row->title;
  $params['attachment'] = $attachments;
  $site_email = variable_get('site_mail', '');
  //array_push($cc, $row->mail);
  if ($row->mail != '') {
    $send = new Attachmentemail($row->mail, $site_email, $row->title, $body, $attachments);
    $send->send();
  }
  if (implode(',', $cc) != '') {
    $send_user = new Attachmentemail(implode(',', $cc), $site_email, $row->title, $body_user, $attachments);
    $send_user->send();
  }
  if ($row->mail) {
    if ($send != TRUE) {
      $_SESSION['submited' . $id] = t("Error, email was not sent");
      
    }
    else {
      $_SESSION['submited' . $id] = t("Your form was successfully submitted");
    }
  }
  else {
    $_SESSION['submited' . $id] = t("Your form was successfully submitted");
  }
  if ($row->redirect_url == '') {
    header('Location: ' . url('node/') . $id);
  }
  else {
    $redirect = explode('#****#', $row->redirect_url);
    switch ($redirect[0]) {
      case 'stay_on_form':
        header('Location: ' . url('node/') . $id);
        break;

      case 'article':
        header('Location: ' . url('node/') . $redirect[1]);
        break;

      case 'custom_text':
        variable_set('form_maker_custom_text_after_submit', $redirect[1]);
        header('Location: ' . url('node/') . $id);
        break;

      case 'url':
        header('Location: ' . $redirect[1]);
        break;
    }
  }
}

/**
 * Delete submission from submit table.
 */
function form_maker_remove($group_id) {
  db_query("DELETE FROM {form_maker_submits_table} WHERE group_id=:group_id", array(':group_id' => $group_id));
}

/**
 * Send the email.
 */
class Attachmentemail {
  protected $to = '';
  protected $from = '';
  protected $subject = '';
  protected $message = '';
  protected $attachedfiles = '';

  /**
   * Constructor.
   */
  public function __construct($to, $from, $subject, $message, $attachedfiles = array()) {
    $this->to = $to;
    $this->from = $from;
    $this->subject = $subject;
    $this->message = $message;
    $this->attachedfiles = $attachedfiles;
    $this->boundary = md5(date('r', time()));
  }

  /**
   * Send the email.
   */
  public function send() {
    $header = "From: " . ($this->from) . " <" . ($this->from) . ">" . PHP_EOL;
    $header .= "Reply-To: " . ($this->from) . PHP_EOL;

    // $header .= "Content-Transfer-Encoding: 7bit" . PHP_EOL;
    $header .= "Content-Type: multipart/mixed; boundary=\"" . $this->boundary . '"' . PHP_EOL;
    $header .= "MIME-Version: 1.0" . PHP_EOL;
    $message = "This is a multi-part message in MIME format." . PHP_EOL . PHP_EOL;
    $message .= "--" . $this->boundary . PHP_EOL;
    // $message .= "Content-Transfer-Encoding: binary" . PHP_EOL;
    $message .= "Content-type:text/html; charset=UTF-8; format=flowed;" . PHP_EOL . PHP_EOL;
    $message .= $this->message . PHP_EOL . PHP_EOL;
    $message .= $this->getbinaryattachments() . PHP_EOL;
    $message .= '--' . $this->boundary . '--' . PHP_EOL;
    $this->message = $message;

    if (mail($this->to, $this->subject, $this->message, $header)) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Get the attachments in a base64 format.
   */
  public function getbinaryattachments() {
    $output = '';

    foreach ($this->attachedfiles as $attachment) {
      $info = pathinfo($attachment);
      switch ($info['extension']) {
        case 'jpg':
        case 'jpe':
        case 'jpeg':
        case 'bmp':
        case 'gif':
          $mime = 'image/jpeg jpg';
          break;

        case 'xls':
        case 'xlsx':
          $mime = 'application/msexcel';
          break;

        case 'png':
          $mime = 'image/png';
          break;

        case 'doc':
        case 'docx':
          $mime = 'application/msword';
          break;

        case 'txt':
          $mime = 'text/plain';
          break;

        case 'zip':
          $mime = 'application/zip';
          break;

        case 'avi':
          $mime = 'video/x-msvideo';
          break;

        case 'htm':
        case 'html':
          $mime = 'text/html';
          break;

        case 'mp3':
          $mime = 'audio/mpeg';
          break;

        case 'mpa':
        case 'mpe':
        case 'mpeg':
        case 'mpg':
          $mime = 'video/mpeg';
          break;

        default:
          $mime = 'image/jpeg jpg';
          break;
      }
      $file = array(
        'path' => $attachment,
        'filename' => $info['basename'],
        'type' => $mime,
      );
      $attachment_bin = file_get_contents($file['path']);
      $attachment_bin = chunk_split(base64_encode($attachment_bin));
      $output .= '--' . $this->boundary . PHP_EOL;
      $output .= 'Content-Type: ' . $file['type'] . '; name="' . basename($file['path']) . '"' . PHP_EOL;
      $output .= 'Content-Transfer-Encoding: base64' . PHP_EOL;
      $output .= 'Content-Disposition: attachment' . PHP_EOL . PHP_EOL;
      $output .= $attachment_bin . PHP_EOL . PHP_EOL;
    }
    return $output;
  }
}
