function update_preview(command, element) {
  switch (command) {
    case 'total' :
      table_total_ads = element.options[element.selectedIndex].value;
      redraw_table();
      break;
    case 'rows' :
      table_rows = element.options[element.selectedIndex].value;
      redraw_table();
      break;
    case 'font' :
      update_font_size(element.options[element.selectedIndex].value);
      break;
    case 'link_color' :
      update_link_color(element.value);
      break;
  }
}

function update_link_color(value) {
  if (typeof(value) == 'undefined') {
    value = document.getElementById('edit-textlinkads_link_color').value;
  }
  var spans = document.getElementsByTagName('span');
  for (var i = 0, arr_length = spans.length; i < arr_length; ++i) {
    if (spans[i].className == 'textlinkads-example-link') {
      spans[i].style.color = value;
    }
  }
  link_color = value;
}

function update_bg_color(value) {
  if (typeof(value) == 'undefined') {
    value = document.getElementById('edit-textlinkads-bg-color').value;
  }
  if (!document.getElementById('edit-textlinkads-no-bg').checked) {
    no_bg = 0;
    document.getElementById('textlinkads_preview_table').style.backgroundColor = value;
  }
  else {
  	no_bg = 1;
    document.getElementById('textlinkads_preview_table').style.backgroundColor = 'transparent';
  }
  bg_color = value;
}
function update_border_color(value) {
  if (typeof(value) == 'undefined') {
    value = document.getElementById('edit-textlinkads-border-color').value;
  }
  border_color = value;
  if (!document.getElementById('edit-textlinkads-no-border').checked) {
    no_border = 0;
    document.getElementById('textlinkads_preview_table').style.border = '1px solid ' + value;
    // FireFox bug fix ... weird
    redraw_table();
  }
  else {
    no_border = 1;
    document.getElementById('textlinkads_preview_table').style.border = 'none';
  }
}

function update_font_size(size) {
  if (typeof(size) == 'undefined') {
    size = document.getElementById('edit-textlinkads-font').options[document.getElementById('edit-textlinkads-font').selectedIndex].value;
  }
  var spans = document.getElementsByTagName('span');
  for (var i = 0, arr_length = spans.length; i < arr_length; ++i) {
    if (spans[i].className == 'textlinkads-example-link') {
      spans[i].style.fontSize = size + 'px';
    }
  }
  font_size = size;
}

function redraw_table() {
  //alert('total: ' + table_total_ads + ' rows ' + table_rows);
  var table = '<table id="textlinkads_preview_table" style="border: ' + (no_border == 1 ? 'none' : '1px solid' + border_color) + '; border-spacing: 0px; background-color: '+ (no_bg == 1  ? 'transparent' : bg_color) +';" cellpadding="5" cellspacing="0"><tbody>';
  var k;
  var columns = Math.ceil(table_total_ads / table_rows)
  for (var i = 0; i < table_total_ads;) {
    table += "<tr>";
    for (k = 0; k < columns; k++) {
      if (i < table_total_ads) {
        table += "<td><span class='textlinkads-example-link' style='cursor: pointer;text-decoration: underline;font-size:"+ font_size +"px; color:"+ link_color +";'>" + example_link + "</span></td>";
      } else {
        table += "<td> </td>";
      }
      i++;
    }
    table += "</tr>";
  }
  table += '</tbody></table>';

  document.getElementById('preview').innerHTML = table;
}

function addLoadEvent(func) {
  var oldOnload = window.onload;
  if (typeof window.onload != 'function') {
    window.onload = func;
  }
  else {
    window.onload = function() {
      oldOnload();
      func();
    }
  }
}

function init() {
  redraw_table();
  update_bg_color();
  update_border_color();
  document.getElementById('edit-textlinkads-total').onchange = function() {update_preview('total', this)};
  document.getElementById('edit-textlinkads-font').onchange =  function() {update_preview('font', this)};
  document.getElementById('edit-textlinkads-rows').onchange =  function() {update_preview('rows', this)};
}

addLoadEvent(init);
