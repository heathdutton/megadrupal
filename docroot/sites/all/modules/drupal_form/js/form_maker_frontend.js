// Choices id.
var F = 2;
var c;
var a = new Array();

function show_other_input(num, form_id) {
  for (k = 0; k < 50; k++) {
    if (document.getElementById(num + "_element" + form_id + k)) {
      if (document.getElementById(num + "_element" + form_id + k).getAttribute('other')) {
        if (document.getElementById(num + "_element" + form_id + k).getAttribute('other') == 1) {
          element_other = document.getElementById(num + "_element" + form_id + k);
          break;
        }
      }
    }
  }
  parent = element_other.parentNode;
  var br = document.createElement('br');
  br.setAttribute("id", num + "_other_br" + form_id);
  var el_other = document.createElement('input');
  el_other.setAttribute("id", num + "_other_input" + form_id);
  el_other.setAttribute("name", num + "_other_input" + form_id);
  el_other.setAttribute("type", "text");
  el_other.setAttribute("class", "other_input");
  parent.appendChild(br);
  parent.appendChild(el_other);
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

function check_isnum(e) {
  var chCode1 = e.which || e.keyCode;
  if (chCode1 > 31 && (chCode1 < 48 || chCode1 > 57)) {
    return false;
  }
  return true;
}

function captcha_refresh(id, genid) {
  srcArr = document.getElementById(id + genid).src.split("&r=");
  document.getElementById(id + genid).src = srcArr[0] + '&r=' + Math.floor(Math.random() * 100);
  document.getElementById(id + "_input" + genid).value = '';
  document.getElementById(id + genid).style.display = "block";
}

function set_checked(id, j, form_id) {
  checking = document.getElementById(id + "_element" + form_id + j);
  if (checking.getAttribute('other')) {
    if (checking.getAttribute('other') == 1) {
      if (!checking.checked) {
        if (document.getElementById(id + "_other_input" + form_id)) {
          document.getElementById(id + "_other_input" + form_id).parentNode.removeChild(document.getElementById(id + "_other_br" + form_id));
          document.getElementById(id + "_other_input" + form_id).parentNode.removeChild(document.getElementById(id + "_other_input" + form_id));
        }
        return false;
      }
    }
  }
  return true;
}

function set_select() {
}

function set_default(id, j, form_id) {
  if (document.getElementById(id + "_other_input" + form_id)) {
    document.getElementById(id + "_other_input" + form_id).parentNode.removeChild(document.getElementById(id + "_other_br" + form_id));
    document.getElementById(id + "_other_input" + form_id).parentNode.removeChild(document.getElementById(id + "_other_input" + form_id));
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

function check_year2(id, str) {
  year = document.getElementById(id).value;
  from = parseFloat(document.getElementById(id).getAttribute('from'));
  year = parseFloat(year);
  if (year < from) {
    document.getElementById(id).value = '';
  }
}

function delete_value(id) {
  if (window.getComputedStyle) {
    ofontStyle = window.getComputedStyle(document.getElementById(id), null).fontStyle;
  }
  else if (document.getElementById(id).currentStyle) {
    ofontStyle = document.getElementById(id).currentStyle.fontStyle;
  }
  if (ofontStyle == "italic") {
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
    input.setAttribute("value", input.title);
    input.className = 'input_deactive';
    input.setAttribute("class", 'input_deactive');
  }
}

function change_value(id) {
  input = document.getElementById(id);
  tag = input.tagName;
  if (tag == "TEXTAREA") {
// destroyChildren(input)
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

function change_file_value(destination, id) {
  input = document.getElementById(id);
  input.setAttribute("value", destination);
}

function change_label(id, label) {
  document.getElementById(id).innerHTML = label;
  document.getElementById(id).value = label;
}

function change_in_value(id, label) {
  document.getElementById(id).setAttribute("value", label);
}

function destroyChildren(node) {
  while (node.firstChild)
    node.removeChild(node.firstChild);
}

function generate_page_nav_frontend(id, form_id, form_view_count, form_view_max) {
  form_view = id;
  page_nav = document.getElementById(form_id + 'page_nav' + id);
  page_nav.setAttribute('style', 'background-color:rgba(0,0,0,0);');
  destroyChildren(page_nav);
  form_view_elemet = document.getElementById(form_id + 'form_view' + id);

  display_none_form_views_all_frontend(form_id);

  generate_page_bar_frontend(id, form_id, form_view_count, form_view_max);

  form_view_elemet.parentNode.style.display = "";

  if (form_view_elemet.parentNode.previousSibling && form_view_elemet.parentNode.previousSibling.previousSibling) {
    if (form_view_elemet.parentNode.previousSibling.tagName == "TABLE") {
      table = form_view_elemet.parentNode.previousSibling;
    }
    else if (form_view_elemet.parentNode.previousSibling.previousSibling.tagName == "TABLE") {
      table = form_view_elemet.parentNode.previousSibling.previousSibling;
    }
    else {
      table = "none";
    }
    if (table != "none") {
      if (!table.firstChild.tagName) {
        table.removeChild(table.firstChild);
      }
      previous_title = form_view_elemet.getAttribute('previous_title');
      previous_type = form_view_elemet.getAttribute('previous_type');
      previous_class = form_view_elemet.getAttribute('previous_class');
      previous_checkable = form_view_elemet.getAttribute('previous_checkable');
      next_or_previous = "previous";
      previous = make_pagebreak_button_frontend(next_or_previous, previous_title, previous_type, previous_class, previous_checkable, id, form_id, form_view_count, form_view_max);
      var td = document.createElement("td");
      td.setAttribute("valign", "middle");
      td.setAttribute("align", "left");
      td.appendChild(previous);
      page_nav.appendChild(td);
    }
  }
  var td = document.createElement("td");
  td.setAttribute("id", form_id + "page_numbers" + form_view);
  td.setAttribute("width", "100%");
  td.setAttribute("valign", "middle");
  td.setAttribute("align", "center");
  if (document.getElementById(form_id + 'pages').getAttribute('show_numbers') == "true") {
    k = 0;
    for (j = 1; j <= form_view_max; j++) {
      if (document.getElementById(form_id + 'form_view' + j)) {
        k++;
        if (j == form_view) {
          page_number = k;
        }
      }
    }
    var cur = document.createElement('span');
    cur.setAttribute("class", "page_numbers");
    cur.innerHTML = page_number + '/' + k;
    td.appendChild(cur);
  }
  page_nav.appendChild(td);
  not_next = false;
  if (form_view_elemet.parentNode.nextSibling) {
    if (form_view_elemet.parentNode.nextSibling.tagName == "TABLE") {
      table = form_view_elemet.parentNode.nextSibling;
    }
    else if (form_view_elemet.parentNode.nextSibling.nextSibling) {
      if (form_view_elemet.parentNode.nextSibling.nextSibling.tagName == "TABLE") {
        table = form_view_elemet.parentNode.nextSibling.nextSibling;
      }
      else {
        table = "none";
      }
    }
    else {
      table = "none";
    }
    if (table != "none") {
      next_title = form_view_elemet.getAttribute('next_title');
      next_type = form_view_elemet.getAttribute('next_type');
      next_class = form_view_elemet.getAttribute('next_class');
      next_checkable = form_view_elemet.getAttribute('next_checkable');
      next_or_previous = "next";
      next = make_pagebreak_button_frontend(next_or_previous, next_title, next_type, next_class, next_checkable, id, form_id, form_view_count, form_view_max);
      var td = document.createElement("td");
      td.setAttribute("valign", "middle");
      td.setAttribute("align", "right");
      td.appendChild(next);
      page_nav.appendChild(td);
    }
    else {
      not_next = true;
    }
  }
  else {
    not_next = true;
  }
  for (x = 0; x < parseInt(document.getElementById('counter' + form_id).value); x++) {
    if (document.getElementById(x + '_type' + form_id)) {
      if (document.getElementById(x + '_type' + form_id).value == "type_map") {
        if_gmap_init(x, form_id);
        for (q = 0; q < 20; q++) {
          if (document.getElementById(x + "_element" + form_id).getAttribute("long" + q)) {
            w_long = parseFloat(document.getElementById(x + "_element" + form_id).getAttribute("long" + q));
            w_lat = parseFloat(document.getElementById(x + "_element" + form_id).getAttribute("lat" + q));
            w_info = parseFloat(document.getElementById(x + "_element" + form_id).getAttribute("info" + q));
            add_marker_on_map(x, q, w_long, w_lat, w_info, form_id, false);
          }
        }
      }
      if (document.getElementById(x + '_type' + form_id).value == "type_mark_map") {
        if (!document.getElementById(x + '_long' + form_id)) {
          var longit = document.createElement('input');
          longit.setAttribute("type", 'hidden');
          longit.setAttribute("id", x + '_long' + form_id);
          longit.setAttribute("name", x + '_long' + form_id);

          var latit = document.createElement('input');
          latit.setAttribute("type", 'hidden');
          latit.setAttribute("id", x + '_lat' + form_id);
          latit.setAttribute("name", x + '_lat' + form_id);
          document.getElementById(x + "_element_section" + form_id).appendChild(longit);
          document.getElementById(x + "_element_section" + form_id).appendChild(latit);
          w_long = parseFloat(document.getElementById(x + "_element" + form_id).getAttribute("long0"));
          w_lat = parseFloat(document.getElementById(x + "_element" + form_id).getAttribute("lat0"));
          w_info = document.getElementById(x + "_element" + form_id).getAttribute("info0");
        }
        else {
          w_long = parseFloat(document.getElementById(x + "_long" + form_id).value);
          w_lat = parseFloat(document.getElementById(x + "_lat" + form_id).value);
          w_info = document.getElementById(x + "_element" + form_id).getAttribute("info0");
        }
        longit = document.getElementById(x + '_long' + form_id);
        latit = document.getElementById(x + '_lat' + form_id);

        if_gmap_init(x, form_id);

        curpoint = new google.maps.LatLng(w_lat, w_long);

        gmapdata[x].setCenter(curpoint);

        longit.value = w_long;
        latit.value = w_lat;
        add_marker_on_map(x, 0, w_long, w_lat, w_info, form_id, true);
      }
    }
  }
}

function display_none_form_views_all_frontend(form_id) {
  for (t = 1; t < 30; t++) {
    if (document.getElementById(form_id + 'form_view' + t)) {
      document.getElementById(form_id + 'form_view' + t).parentNode.style.display = "none";
    }
  }
}

function generate_page_bar_frontend(form_view, form_id, form_view_count, form_view_max) {
  if (document.getElementById(form_id + 'pages').getAttribute('type') == 'steps') {
    make_page_steps_front_frontend(form_view, form_id, form_view_count, form_view_max);
  }
  else if (document.getElementById(form_id + 'pages').getAttribute('type') == 'percentage') {
    make_page_percentage_front_frontend(form_view, form_id, form_view_count, form_view_max);
  }
  else {
    make_page_none_front_frontend(form_id);
  }
  if (document.getElementById(form_id + 'pages').getAttribute('type') == 'show_numbers') {
    td = document.getElementById(form_id + 'page_numbers' + form_view);
    if (td) {
      destroyChildren(td);
      k = 0;
      for (j = 1; j <= form_view_max; j++) {
        if (document.getElementById(form_id + 'form_view' + j)) {
          k++;
          if (j == form_view) {
            page_number = k;
          }
        }
      }
      var cur = document.createElement('span');
      cur.setAttribute("class", "page_numbers");
      cur.innerHTML = page_number + '/' + k;
      td.appendChild(cur);
    }
  }
  else {
    td = document.getElementById(form_id + 'page_numbers' + form_view);
    if (td) {
      destroyChildren(document.getElementById(form_id + 'page_numbers' + form_view));
    }
  }
}
function make_page_steps_front_frontend(form_view, form_id, form_view_count, form_view_max) {
  destroyChildren(document.getElementById(form_id + 'pages'));
  show_title = (document.getElementById(form_id + 'pages').getAttribute('show_title') == 'true');
  next_checkable = (document.getElementById(form_id + 'form_view' + form_view).getAttribute('next_checkable') == 'true');
  previous_checkable = (document.getElementById(form_id + 'form_view' + form_view).getAttribute('previous_checkable') == 'true');
  k = 0;
  for (j = 1; j <= form_view_max; j++) {
    if (document.getElementById(form_id + 'form_view' + j)) {
      if (document.getElementById(form_id + 'form_view' + j).getAttribute('page_title')) {
        w_pages = document.getElementById(form_id + 'form_view' + j).getAttribute('page_title');
      }
      else {
        w_pages = "";
      }
      k++;
      page_number = document.createElement('span');
      page_number.setAttribute('id', 'page_' + j);
      if (j < form_view) {
        if (previous_checkable) {
          page_number.setAttribute('onClick', 'if(check(' + form_view + ', ' + form_id + ')) generate_page_nav_frontend("' + j + '", "' + form_id + '", "' + form_view_count + '", "' + form_view_max + '")');
        }
        else {
          page_number.setAttribute('onClick', 'generate_page_nav_frontend("' + j + '", "' + form_id + '", "' + form_view_count + '", "' + form_view_max + '")');
        }
      }
      if (j > form_view) {
        if (next_checkable) {
          page_number.setAttribute('onClick', 'if(check(' + form_view + ', ' + form_id + ')) generate_page_nav_frontend("' + j + '", "' + form_id + '", "' + form_view_count + '", "' + form_view_max + '")');
        }
        else {
          page_number.setAttribute('onClick', 'generate_page_nav_frontend("' + j + '", "' + form_id + '", "' + form_view_count + '", "' + form_view_max + '")');
        }
      }
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
      document.getElementById(form_id + 'pages').appendChild(page_number);
    }
  }
}

function make_page_percentage_front_frontend(form_view, form_id, form_view_count, form_view_max) {
  destroyChildren(document.getElementById(form_id + 'pages'));
  show_title = (document.getElementById(form_id + 'pages').getAttribute('show_title') == 'true');
  var div_parent = document.createElement('div');
  div_parent.setAttribute("class", "page_percentage_deactive");
  var div = document.createElement('div');
  div.setAttribute("id", "div_percentage");
  div.setAttribute("class", "page_percentage_active");
  div.setAttribute("align", "right");
  var b = document.createElement('b');
  b.setAttribute("class", "wdform_percentage_text");
  div.appendChild(b);
  k = 0;
  cur_page_title = '';
  for (j = 1; j <= form_view_max; j++) {
    if (document.getElementById(form_id + 'form_view' + j)) {
      if (document.getElementById(form_id + 'form_view' + j).getAttribute('page_title')) {
        w_pages = document.getElementById(form_id + 'form_view' + j).getAttribute('page_title');
      }
      else {
        w_pages = "";
      }
      k++;
      if (j == form_view) {
        if (show_title) {
          var cur_page_title = document.createElement('span');
          cur_page_title.innerHTML = w_pages;
          cur_page_title.innerHTML = w_pages;
          cur_page_title.setAttribute("class", "wdform_percentage_title");
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
  document.getElementById(form_id + 'pages').appendChild(div_parent);
}

function make_page_none_front_frontend(form_id) {
  destroyChildren(document.getElementById(form_id + 'pages'));
}

function make_pagebreak_button_frontend(next_or_previous, title, type, class_, checkable, id, form_id, form_view_count, form_view_max) {
  switch (type) {
    case 'button': {
      var element = document.createElement('button');
      element.setAttribute('id', "page_" + next_or_previous + "_" + id);
      element.setAttribute('type', "button");
      element.setAttribute('class', class_);
      if (checkable == "true") {
        element.setAttribute('onClick', "if(check(" + id + ", " + form_id + ")) page_" + next_or_previous + "(" + id + "," + form_id + "," + form_view_count + "," + form_view_max + ")");
      }
      else {
        element.setAttribute('onClick', "page_" + next_or_previous + "(" + id + "," + form_id + "," + form_view_count + "," + form_view_max + ")");
      }
      element.innerHTML = title;
      return element;
      break;

    }
    case 'text': {
      var element = document.createElement('span');
      element.setAttribute('id', "page_" + next_or_previous + "_" + id);
      element.setAttribute('class', class_);
      if (checkable == "true") {
        element.setAttribute('onClick', "if(check(" + id + ", " + form_id + ")) page_" + next_or_previous + "(" + id + "," + form_id + "," + form_view_count + "," + form_view_max + ")");
      }
      else {
        element.setAttribute('onClick', "page_" + next_or_previous + "(" + id + "," + form_id + "," + form_view_count + "," + form_view_max + ")");
      }
      element.innerHTML = title;
      return element;
      break;

    }
    case 'img': {
      var element = document.createElement('img');
      element.setAttribute('id', "page_" + next_or_previous + "_" + id);
      element.setAttribute('class', class_);
      if (checkable == "true") {
        element.setAttribute('onClick', "if(check(" + id + ", " + form_id + ")) page_" + next_or_previous + "(" + id + "," + form_id + "," + form_view_count + "," + form_view_max + ")");
      }
      else {
        element.setAttribute('onClick', "page_" + next_or_previous + "(" + id + "," + form_id + "," + form_view_count + "," + form_view_max + ")");
      }
      if (title.indexOf("http") == 0) {
        element.src = title;
      }
      else {
        element.src = title;
      }
      return element;
      break;
    }
  }
}

function page_previous(id, form_id, form_view_count, form_view_max) {
  form_view_elemet = document.getElementById(form_id + 'form_view' + id);
  if (form_view_elemet.parentNode.previousSibling && form_view_elemet.parentNode.previousSibling.previousSibling) {
    if (form_view_elemet.parentNode.previousSibling.tagName == "TABLE") {
      table = form_view_elemet.parentNode.previousSibling;
    }
    else {
      table = form_view_elemet.parentNode.previousSibling.previousSibling;
    }
  }
  if (!table.firstChild.tagName) {
    table.removeChild(table.firstChild);
  }
  generate_page_nav_frontend(table.firstChild.id.replace(form_id + 'form_view', ""), form_id, form_view_count, form_view_max);
}

function page_next(id, form_id, form_view_count, form_view_max) {
  form_view_elemet = document.getElementById(form_id + 'form_view' + id);
  if (form_view_elemet.parentNode.nextSibling) {
    if (form_view_elemet.parentNode.nextSibling.tagName == "TABLE") {
      table = form_view_elemet.parentNode.nextSibling;
    }
    else {
      table = form_view_elemet.parentNode.nextSibling.nextSibling;
    }
  }
  if (!table.firstChild.tagName) {
    table.removeChild(table.firstChild);
  }
  generate_page_nav_frontend(table.firstChild.id.replace(form_id + 'form_view', ""), form_id, form_view_count, form_view_max);
}

function randomSort(a, b) {
  return( parseInt(Math.random() * 10) % 2 );
}

function choises_randomize(id, form_id) {
  ot = -1;
  j_array = new Array;
  for (j = 0; j < 100; j++) {
    if (document.getElementById(id + "_element" + form_id + j)) {
      if (document.getElementById(id + "_element" + form_id + j).getAttribute("other"))
        if (document.getElementById(id + "_element" + form_id + j).getAttribute("other") == 1) {
          ot = j;
          continue;
        }
      j_array.push(j);
    }
  }
  j_array.sort(randomSort);
  parent_ = document.getElementById(id + "_element" + form_id + j_array[0]).parentNode.parentNode.parentNode;
  for (j = 0; j < j_array.length; j++) {
    parent_.appendChild(document.getElementById(id + "_element" + form_id + j_array[j]).parentNode.parentNode);
  }
  if (ot != -1) {
    parent_.appendChild(document.getElementById(id + "_element" + form_id + ot).parentNode.parentNode);
  }
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

function getfileextension(id, form_id) {
  var fileinput = document.getElementById(id + "_element" + form_id);
  var filename = fileinput.value;
  if (filename.length == 0) {
    return true;
  }
  var dot = filename.lastIndexOf(".");
  var extension = filename.substr(dot + 1, filename.length);
  var exten = document.getElementById(id + "_extension").value.replace("***extensionverj" + id + "***", "").replace("***extensionskizb" + id + "***", "");
  exten = exten.split(',');
  for (x = 0; x < exten.length; x++) {
    exten[x] = exten[x].replace(/\./g, '');
    exten[x] = exten[x].replace(/ /g, '');
    if (extension.toLowerCase() == exten[x].toLowerCase()) {
      return true;
    }
  }
  return false;
}

function check_required(but_type, form_id) {
  if (but_type == 'reset') {
    if (window.form_maker_before_reset) {
      form_maker_before_reset();
    }
    window.location = Drupal.settings.form_maker.form_maker_request_uri;
    return;
  }
  if (window.form_maker_before_submit) {
    form_maker_before_submit();
  }
  n = parseInt(document.getElementById('counter' + form_id).value);
  ext_available = true;
  seted = true;
  for (i = 0; i <= n; i++) {
    if (seted) {
      if (document.getElementById(i + "_type" + form_id)) {
        if (document.getElementById(i + "_required" + form_id)) {
          if (document.getElementById(i + "_required" + form_id).value == "yes") {
            type = document.getElementById(i + "_type" + form_id).value;
            switch (type) {
              case "type_text":
              case "type_number":
              case "type_password":
              case "type_submitter_mail":
              case "type_own_select":
              case "type_country": {
                if (document.getElementById(i + "_element" + form_id).value == document.getElementById(i + "_element" + form_id).title || document.getElementById(i + "_element" + form_id).value == "") {
                  seted = false;
                }
                break;

              }
              case "type_file_upload": {
                if (document.getElementById(i + "_element" + form_id).value == "") {
                  seted = false;
                  break;
                }
                ext_available = getfileextension(i, form_id);
                if (!ext_available) {
                  seted = false;
                }
                break;

              }
              case "type_textarea": {
                if (document.getElementById(i + "_element" + form_id).innerHTML == document.getElementById(i + "_element" + form_id).title || document.getElementById(i + "_element" + form_id).innerHTML == "") {
                  seted = false;
                }
                break;

              }
              case "type_name": {
                if (document.getElementById(i + "_element_title" + form_id)) {
                  if (document.getElementById(i + "_element_title" + form_id).value == "" || document.getElementById(i + "_element_first" + form_id).value == "" || document.getElementById(i + "_element_last" + form_id).value == "" || document.getElementById(i + "_element_middle" + form_id).value == "" || document.getElementById(i + "_element_first" + form_id).value == document.getElementById(i + "_element_first" + form_id).title || document.getElementById(i + "_element_last" + form_id).value == document.getElementById(i + "_element_last" + form_id).title || document.getElementById(i + "_element_title" + form_id).value == document.getElementById(i + "_element_title" + form_id).title || document.getElementById(i + "_element_middle" + form_id).value == document.getElementById(i + "_element_middle" + form_id).title) {
                    seted = false;
                  }
                }
                else {
                  if (document.getElementById(i + "_element_first" + form_id).value == "" || document.getElementById(i + "_element_last" + form_id).value == "" || document.getElementById(i + "_element_first" + form_id).value == document.getElementById(i + "_element_first" + form_id).title || document.getElementById(i + "_element_last" + form_id).value == document.getElementById(i + "_element_last" + form_id).title) {
                    seted = false;
                  }
                }
                break;

              }
              case "type_phone": {
                if (document.getElementById(i + "_element_first" + form_id).value == "" || document.getElementById(i + "_element_last" + form_id).value == "" || document.getElementById(i + "_element_first" + form_id).value == document.getElementById(i + "_element_first" + form_id).title || document.getElementById(i + "_element_last" + form_id).value == document.getElementById(i + "_element_last" + form_id).title) {
                  seted = false;
                }
                break;

              }
              case "type_address": {
                if (document.getElementById(i + "_street1" + form_id).value == "" || document.getElementById(i + "_city" + form_id).value == "" || document.getElementById(i + "_state" + form_id).value == "" || document.getElementById(i + "_postal" + form_id).value == "" || document.getElementById(i + "_country" + form_id).value == "") {
                  seted = false;
                }
                break;

              }
              case "type_checkbox":
              case "type_radio": {
                is = true;
                for (j = 0; j < 100; j++) {
                  if (document.getElementById(i + "_element" + form_id + j)) {
                    if (document.getElementById(i + "_element" + form_id + j).checked) {
                      is = false;
                      break;
                    }
                  }
                }
                if (is) {
                  seted = false;
                }
                break;

              }
              case "type_time": {
                if (document.getElementById(i + "_ss" + form_id)) {
                  if (document.getElementById(i + "_ss" + form_id).value == "" || document.getElementById(i + "_mm" + form_id).value == "" || document.getElementById(i + "_hh" + form_id).value == "") {
                    seted = false;
                  }
                }
                else {
                  if (document.getElementById(i + "_mm" + form_id).value == "" || document.getElementById(i + "_hh" + form_id).value == "") {
                    seted = false;
                  }
                }
                break;

              }
              case "type_date": {
                if (document.getElementById(i + "_element" + form_id).value == "") {
                  seted = false;
                }
                break;
              }
              case "type_date_fields": {
                if (document.getElementById(i + "_day" + form_id).value == "" || document.getElementById(i + "_month" + form_id).value == "" || document.getElementById(i + "_year" + form_id).value == "") {
                  seted = false;
                }
                break;
              }
            }

          }
          else {
            type = document.getElementById(i + "_type" + form_id).value;
            if (type == "type_file_upload") {
              ext_available = getfileextension(i, form_id);
            }
            if (!ext_available) {
              seted = false;
            }
          }
        }
      }
    }
    else {
      if (!ext_available) {
        alert(Drupal.t("Sorry, you are not allowed to upload this type of file."));
        break;
      }
      x = document.getElementById(i - 1 + '_element_label' + form_id);
      while (x.firstChild) {
        x = x.firstChild;
      }      alert(Drupal.t("'" + x.nodeValue + "' field is required."));
      break;
    }
  }
  if (seted) {
    for (i = 0; i <= n; i++) {
      if (document.getElementById(i + "_type" + form_id)) {
        if (document.getElementById(i + "_type" + form_id).value == "type_submitter_mail") {
          if (document.getElementById(i + "_element" + form_id).value != '' && document.getElementById(i + "_element" + form_id).value.search(/^\w+((-\w+)|(\.\w+))*\@[A-Za-z0-9]+((\.|-)[A-Za-z0-9]+)*\.[A-Za-z0-9]+$/) == -1) {
            alert(Drupal.t("This is not a valid email address."));
            return;
          }
        }
      }
    }
  }
  if (seted) {
    create_headers(form_id);
  }
}

function form_maker_getElementsByAttribute(node,tag,attr,value){
  var elems = (tag=="*" && node.all) ? node.all : node.getElementsByTagName(tag),
    returnElems = new Array(),
    nValue = (typeof value!="undefined") ? new RegExp("(^|\\s)" + value + "(\\s|$)") : null,
    nAttr,
    cur;
  for (var i = 0; i < elems.length; i++) {
    cur = elems[i];
    nAttr = cur.getAttribute && cur.getAttribute(attr);
    if (typeof nAttr == "string" && nAttr.length > 0) {
      if (typeof value == "undefined" || (nValue && nValue.test(nAttr))) {
        returnElems.push(cur);
      }
    }
  }
  return returnElems;
}

function check(id, form_id) {
  n = parseInt(document.getElementById("counter" + form_id).value);
  var form_view_curren = document.getElementById(form_id + "form_view" + id);
  ext_available = true;
  seted = true;
  for (i = 0; i <= n; i++) {
    if (seted) {  
      if (form_maker_getElementsByAttribute(form_view_curren, "*", "id", "" + i + "_type" + form_id + "") != '') {
      // if (document.getElementById(i + "_type" + form_id)) {
      if (document.getElementById(i + "_required" + form_id)) {
          if (document.getElementById(i + "_required" + form_id).value == "yes") {
            type = document.getElementById(i).getAttribute("type");
            switch (type) {
              case "type_text":
              case "type_number":
              case "type_password":
              case "type_submitter_mail":
              case "type_own_select":
              case "type_country": {
                if (document.getElementById(i + "_element" + form_id).value == document.getElementById(i + "_element" + form_id).title || document.getElementById(i + "_element" + form_id).value == "") {
                  seted = false;
                }
                break;
              }

              case "type_file_upload": {
                if (document.getElementById(i + "_element" + form_id).value == "") {
                  seted = false;
                  break;
                }
                ext_available = getfileextension(i, form_id);
                if (!ext_available) {
                  seted = false;
                }
                break;
              }

              case "type_textarea": {
                if (document.getElementById(i + "_element" + form_id).innerHTML == document.getElementById(i + "_element" + form_id).title || document.getElementById(i + "_element" + form_id).innerHTML == "") {
                  seted = false;
                }
                break;
              }

              case "type_name": {
                if (document.getElementById(i + "_element_title" + form_id)) {
                  if (document.getElementById(i + "_element_title" + form_id).value == "" || document.getElementById(i + "_element_first" + form_id).value == "" || document.getElementById(i + "_element_last" + form_id).value == "" || document.getElementById(i + "_element_middle" + form_id).value == "" || document.getElementById(i + "_element_first" + form_id).value == document.getElementById(i + "_element_first" + form_id).title || document.getElementById(i + "_element_last" + form_id).value == document.getElementById(i + "_element_last" + form_id).title || document.getElementById(i + "_element_title" + form_id).value == document.getElementById(i + "_element_title" + form_id).title || document.getElementById(i + "_element_middle" + form_id).value == document.getElementById(i + "_element_middle" + form_id).title) {
                    seted = false;
                  }
                }
                else {
                  if (document.getElementById(i + "_element_first" + form_id).value == "" || document.getElementById(i + "_element_last" + form_id).value == "" || document.getElementById(i + "_element_first" + form_id).value == document.getElementById(i + "_element_first" + form_id).title || document.getElementById(i + "_element_last" + form_id).value == document.getElementById(i + "_element_last" + form_id).title) {
                    seted = false;
                  }
                }
                break;

              }

              case "type_phone": {
                if (document.getElementById(i + "_element_first" + form_id).value == "" || document.getElementById(i + "_element_last" + form_id).value == "" || document.getElementById(i + "_element_first" + form_id).value == document.getElementById(i + "_element_first" + form_id).title || document.getElementById(i + "_element_last" + form_id).value == document.getElementById(i + "_element_last" + form_id).title) {
                  seted = false;
                }
                break;

              }

              case "type_address": {
                if (document.getElementById(i + "_street1" + form_id).value == "" || document.getElementById(i + "_city" + form_id).value == "" || document.getElementById(i + "_state" + form_id).value == "" || document.getElementById(i + "_postal" + form_id).value == "" || document.getElementById(i + "_country" + form_id).value == "") {
                  seted = false;
                }
                break;

              }

              case "type_checkbox":
              case "type_radio": {
                is = true;
                for (j = 0; j < 100; j++) {
                  if (document.getElementById(i + "_element" + form_id + j)) {
                    if (document.getElementById(i + "_element" + form_id + j).checked) {
                      is = false;
                      break;
                    }
                  }
                }
                if (is) {
                  seted = false;
                }
                break;
              }

              case "type_time": {
                if (document.getElementById(i + "_ss" + form_id)) {
                  if (document.getElementById(i + "_ss" + form_id).value == "" || document.getElementById(i + "_mm" + form_id).value == "" || document.getElementById(i + "_hh" + form_id).value == "") {
                    seted = false;
                  }
                }
                else {
                  if (document.getElementById(i + "_mm" + form_id).value == "" || document.getElementById(i + "_hh" + form_id).value == "") {
                    seted = false;
                  }
                }
                break;

              }

              case "type_date": {
                if (document.getElementById(i + "_element" + form_id).value == "") {
                  seted = false;
                }
                break;
              }
              case "type_date_fields": {
                if (document.getElementById(i + "_day" + form_id).value == "" || document.getElementById(i + "_month" + form_id).value == "" || document.getElementById(i + "_year" + form_id).value == "")
                  seted = false;
                break;
              }
            }

          }
          else {
            type = document.getElementById(i).getAttribute("type");
            if (type == "type_file_upload")
              ext_available = getfileextension(i, form_id);
            if (!ext_available) {
              seted = false;
            }
          }
        }
      }
    }
    else {
      if (!ext_available) {
        alert(Drupal.t("Sorry, you are not allowed to upload this type of file"));
        break;
      }
      x = document.getElementById(i - 1 + '_element_label' + form_id);
      while (x.firstChild) {
        x = x.firstChild;
      }
      alert(Drupal.t("'" + x.nodeValue + "' field is required."));
      break;
    }
  }
  if (seted) {
    for (i = 0; i <= n; i++) {
      if (document.getElementById(i)) {
        if (document.getElementById(i).getAttribute("type") == "type_submitter_mail") {
          if (document.getElementById(i + "_element" + form_id).value != '' && document.getElementById(i + "_element" + form_id).value.search(/^\w+((-\w+)|(\.\w+))*\@[A-Za-z0-9]+((\.|-)[A-Za-z0-9]+)*\.[A-Za-z0-9]+$/) == -1) {
            alert(Drupal.t("This is not a valid email address."));
            return;
          }
        }
      }
    }
  }
  if (seted) {
    return true;
  }
}

function create_headers(form_id) {
  if (a[form_id] == 1)
    return;
  a[form_id] = 1;

  /*n=<?php echo $row->counter?>;
   for(i=0; i<n; i++)
   {
   if(document.getElementById(i))
   {
   if(document.getElementById(i).getAttribute("type")!="type_map" && document.getElementById(i).getAttribute("type")!="type_captcha" && document.getElementById(i).getAttribute("type")!="type_recaptcha" && document.getElementById(i).getAttribute("type")!="type_submit_reset" && document.getElementById(i).getAttribute("type")!="type_button")
   if(document.getElementById(i+'_element_label<?php echo $id ?>'))
   {
   var input = document.createElement('input');
   input.setAttribute("type", 'hidden');
   input.setAttribute("name", i+'_element_label<?php echo $id ?>');
   input.value=i;
   form_.appendChild(input);
   if(document.getElementById(i).getAttribute("type")=="type_date_fields")
   {	
   var input = document.createElement('input');
   input.setAttribute("type", 'hidden');
   input.setAttribute("name", i+'_element<?php echo $id ?>');
   input.value=document.getElementById(i+'_day<?php echo $id ?>').value+'-'+document.getElementById(i+'_month<?php echo $id ?>').value+'-'+document.getElementById(i+'_year<?php echo $id ?>').value;
   form_.appendChild(input);
   }
   }
   }
   }*/
  document.getElementById("form" + form_id).submit();
}/** * Swallow Enter on front end. */function swallowEnter(e) {  var chCode1 = e.which || e.keyCode;  if (chCode1 == 13) {    return false;  }  return true;}
