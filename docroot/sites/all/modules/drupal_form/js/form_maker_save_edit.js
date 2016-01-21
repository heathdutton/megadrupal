function form_maker_submitbutton(pressbutton) {  already_submited = true;
  var form = document.all_Form_Maker;
  if (form.mail.value != '') {
    subMailArr = form.mail.value.split(',');
    emailListValid = true;
    for (subMailIt = 0; subMailIt < subMailArr.length; subMailIt++) {
      trimmedMail = subMailArr[subMailIt].replace(/^\s+|\s+$/g, '');
      if (trimmedMail.search(/^\w+((-\w+)|(\.\w+))*\@[A-Za-z0-9]+((\.|-)[A-Za-z0-9]+)*\.[A-Za-z0-9]+$/) == -1) {
        alert(Drupal.t("This is not a list of valid email addresses."));
        emailListValid = false;
        break;
      }
    }
    if (!emailListValid) {
      return false;
    }
  }
  var tox = '';
  l_id_array = Drupal.settings.form_maker.labels_id.split(",");
  l_id_removed = [];
  for (x = 0; x < l_id_array.length; x++) {
    l_id_removed[x] = true;
  }
  l_label_array = Drupal.settings.form_maker.labels_label.split(",");
  l_type_array = Drupal.settings.form_maker.labels_type.split(",");
  for (x = 0; x < l_id_array.length; x++) {
    l_id_removed[x] = true;
  }
  for (t = 1; t <= form_view_max; t++) {
    if (document.getElementById('form_id_tempform_view' + t)) {
      form_view_element = document.getElementById('form_id_tempform_view' + t);
      n = form_view_element.childNodes.length - 2;
      for (q = 0; q <= n; q++) {
        if (form_view_element.childNodes[q].nodeType != 3) {
          if (!form_view_element.childNodes[q].id) {
            GLOBAL_tr = form_view_element.childNodes[q];
            for (x = 0; x < GLOBAL_tr.firstChild.childNodes.length; x++) {
              table = GLOBAL_tr.firstChild.childNodes[x];
              tbody = table.firstChild;
              for (y = 0; y < tbody.childNodes.length; y++) {
                is_in_old = false;
                tr = tbody.childNodes[y];
                l_id = tr.id;
                l_label = document.getElementById(tr.id + '_element_labelform_id_temp').innerHTML;
                l_label = l_label.replace(/(\r\n|\n|\r)/gm," ");
                l_type = tr.getAttribute('type');
                for (z = 0; z < l_id_array.length; z++) {
                  if (l_id_array[z] == l_id) {
                    l_id_removed[z] = false;
                    if (l_type_array[z] == "type_address") {
                      z++;
                      l_id_removed[z] = false;
                      z++;
                      l_id_removed[z] = false;
                      z++;
                      l_id_removed[z] = false;
                      z++;
                      l_id_removed[z] = false;
                      z++;
                      l_id_removed[z] = false;
                    }
                  }
                }
                if (tr.getAttribute('type') == "type_address") {
                  addr_id = parseInt(tr.id);
                  tox = tox + addr_id + '#**id**#' + 'Street Line' + '#**label**#' + tr.getAttribute('type') + '#****#';
                  addr_id++;
                  tox = tox + addr_id + '#**id**#' + 'Street Line2' + '#**label**#' + tr.getAttribute('type') + '#****#';
                  addr_id++;
                  tox = tox + addr_id + '#**id**#' + 'City' + '#**label**#' + tr.getAttribute('type') + '#****#';
                  addr_id++;
                  tox = tox + addr_id + '#**id**#' + 'State' + '#**label**#' + tr.getAttribute('type') + '#****#';
                  addr_id++;
                  tox = tox+addr_id + '#**id**#' + 'Postal' + '#**label**#' + tr.getAttribute('type') + '#****#';
                  addr_id++;
                  tox = tox + addr_id + '#**id**#' + 'Country' + '#**label**#' + tr.getAttribute('type') + '#****#';
                }
                else {
                  tox = tox + l_id + '#**id**#' + l_label + '#**label**#' + l_type + '#****#';
                }
              }
            }
          }
        }
      }
    }
  }
  for (x = 0; x < l_id_array.length; x++) {
    if (l_id_removed[x]) {
      tox = tox + l_id_array[x] + '#**id**#' + l_label_array[x] + '#**label**#' + l_type_array[x] + '#****#';
    }
  }
  document.getElementById('label_order').value = tox;
  refresh_();
  document.getElementById('pagination').value = document.getElementById('pages').getAttribute("type");
  document.getElementById('show_title').value = document.getElementById('pages').getAttribute("show_title");
  document.getElementById('show_numbers').value = document.getElementById('pages').getAttribute("show_numbers");  return true;
}

function refresh_() {
  document.getElementById('form').value = document.getElementById('take').innerHTML;
  document.getElementById('counter').value = gen;
  n = gen;
  for (i = 0; i < n; i++) {
    if (document.getElementById(i)) {
      for (z = document.getElementById(i).childNodes.length - 1; z >= 0 ; z--) {
        if (document.getElementById(i).childNodes[z].nodeType == 3) {
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[z]);
        }
      }
      if (document.getElementById(i).getAttribute('type') == "type_captcha" || document.getElementById(i).getAttribute('type') == "type_recaptcha") {
        if (document.getElementById(i).childNodes[10]) {
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
        }
        else {
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
        }
        continue;
      }
      if (document.getElementById(i).getAttribute('type') == "type_section_break") {
        document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
        document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
        document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
        continue;
      }
      if (document.getElementById(i).childNodes[10]) {
        document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
        document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
        document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
        document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
        document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
        document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
        document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
        document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
        document.getElementById(i).removeChild(document.getElementById(i).childNodes[2]);
      }
      else {
        while(document.getElementById(i).childNodes[1]) {
          document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
        }
        // document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
        // document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
        // document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
        // document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
        // document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
        // document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
        // document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
        // document.getElementById(i).removeChild(document.getElementById(i).childNodes[1]);
      }
    }
  }
  for (i = 0; i <= n; i++) {
    if (document.getElementById(i)) {
      type = document.getElementById(i).getAttribute("type");
      switch (type) {
        case "type_text":
        case "type_number":
        case "type_password":
        case "type_submitter_mail":
        case "type_own_select":
        case "type_country":
        case "type_hidden":
        case "type_map": {
          remove_add_(i + "_elementform_id_temp");
          break;

        }
        case "type_submit_reset": {
          remove_add_(i + "_element_submitform_id_temp");
          if (document.getElementById(i + "_element_resetform_id_temp")) {
            remove_add_(i + "_element_resetform_id_temp");
          }
          break;

        }
        case "type_captcha": {
          remove_add_("_wd_captchaform_id_temp");
          remove_add_("_element_refreshform_id_temp");
          remove_add_("_wd_captcha_inputform_id_temp");
          break;

        }
        case "type_recaptcha": {
          document.getElementById("public_key").value = document.getElementById("wd_recaptchaform_id_temp").getAttribute("public_key");
          document.getElementById("private_key").value = document.getElementById("wd_recaptchaform_id_temp").getAttribute("private_key");
          document.getElementById("recaptcha_theme").value = document.getElementById("wd_recaptchaform_id_temp").getAttribute("theme");
          document.getElementById('wd_recaptchaform_id_temp').innerHTML = '';
          remove_add_("wd_recaptchaform_id_temp");
          break;

        }
        case "type_file_upload": {
          remove_add_(i + "_elementform_id_temp");
          break;

        }
        case "type_textarea": {
          remove_add_(i + "_elementform_id_temp");
          break;

        }
        case "type_name": {
          if (document.getElementById(i + "_element_titleform_id_temp")) {
            remove_add_(i + "_element_titleform_id_temp");
            remove_add_(i + "_element_firstform_id_temp");
            remove_add_(i + "_element_lastform_id_temp");
            remove_add_(i + "_element_middleform_id_temp");
          }
          else {
            remove_add_(i + "_element_firstform_id_temp");
            remove_add_(i + "_element_lastform_id_temp");

          }
          break;

        }
        case "type_phone": {
          remove_add_(i + "_element_firstform_id_temp");
          remove_add_(i + "_element_lastform_id_temp");

          break;
        }
        case "type_address": {
          remove_add_(i + "_street1form_id_temp");
          remove_add_(i + "_street2form_id_temp");
          remove_add_(i + "_cityform_id_temp");
          remove_add_(i + "_stateform_id_temp");
          remove_add_(i + "_postalform_id_temp");
          remove_add_(i + "_countryform_id_temp");

          break;
        }
        case "type_checkbox":
        case "type_radio": {
          is = true;
          for (j = 0; j < 100; j++) {
            if (document.getElementById(i + "_elementform_id_temp" + j)) {
              remove_add_(i + "_elementform_id_temp" + j);
            }
          }
          break;

        }
        case "type_button": {
          for (j = 0; j < 100; j++) {
            if (document.getElementById(i + "_elementform_id_temp" + j)) {
              remove_add_(i + "_elementform_id_temp" + j);
            }
          }
          break;

        }
        case "type_time": {
          if (document.getElementById(i + "_ssform_id_temp")) {
            remove_add_(i + "_ssform_id_temp");
            remove_add_(i + "_mmform_id_temp");
            remove_add_(i + "_hhform_id_temp");
          }
          else {
            remove_add_(i + "_mmform_id_temp");
            remove_add_(i + "_hhform_id_temp");
          }
          break;

        }
        case "type_date": {
          remove_add_(i + "_elementform_id_temp");
          remove_add_(i + "_buttonform_id_temp");
          break;

        }
        case "type_date_fields": {
          remove_add_(i + "_dayform_id_temp");
          remove_add_(i + "_monthform_id_temp");
          remove_add_(i + "_yearform_id_temp");
          break;
        }
      }
    }
  }
  for (i = 1; i <= form_view_max; i++) {
    if (document.getElementById('form_id_tempform_view' + i)) {
      if (document.getElementById('page_next_' + i)) {
        document.getElementById('page_next_' + i).removeAttribute('src');
      }
      if (document.getElementById('page_previous_' + i)) {
        document.getElementById('page_previous_' + i).removeAttribute('src');
      }
      document.getElementById('form_id_tempform_view' + i).parentNode.removeChild(document.getElementById('form_id_tempform_view_img' + i));
      document.getElementById('form_id_tempform_view' + i).removeAttribute('style');
    }
  }
  for (t = 1; t <= form_view_max; t++) {
    if (document.getElementById('form_id_tempform_view' + t)) {
      form_view_element = document.getElementById('form_id_tempform_view' + t);
      n = form_view_element.childNodes.length - 2;
      for (z = 0; z <= n; z++) {
        if (form_view_element.childNodes[z]) {
          if (form_view_element.childNodes[z].nodeType != 3) {
            if (!form_view_element.childNodes[z].id) {
              del = true;
              GLOBAL_tr = form_view_element.childNodes[z];
              for (x = 0; x < GLOBAL_tr.firstChild.childNodes.length; x++) {
                table = GLOBAL_tr.firstChild.childNodes[x];
                tbody = table.firstChild;
                if (tbody.childNodes.length) {
                  del = false;
                }
              }
              if (del) {
                form_view_element.removeChild(form_view_element.childNodes[z]);
              }
            }
          }
        }
      }
    }
  }
  document.getElementById('form_front').value = document.getElementById('take').innerHTML;
}