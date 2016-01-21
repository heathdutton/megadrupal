/* hide the advanced properties */
toggleAdvanced();
disableValues();
checkRedirectText();

function disableValues() {
    var values = document.getElementsByClassName('value');
    for(var i = 0; i < values.length; i++) {
      var checkBoxId = values[i].id.replace('value', 'enabled');
      var checkBox = document.getElementById(checkBoxId);
      if(!checkBox.checked) {
        values[i].disabled = 'disabled';
      }
    }
}

function toggleAdvanced() {
    var adv_prop = document.getElementById('advanced_properties_list');
    var adv_toggle = document.getElementById('advanced_toggle');
    if(adv_prop.style.display != 'none') {
        adv_prop.style.display = 'none';
        adv_toggle.innerHTML = '(show)';
    }
    else {
        adv_prop.style.display = 'table';
        adv_toggle.innerHTML = '(hide)';
    }
}

function toggleRow(ev) {
  var valuesSelect = document.getElementById(
    ev.srcElement.id.replace('enabled', 'value')
  );
  if(valuesSelect != null) {
    if(ev.srcElement.checked) {
      valuesSelect.removeAttribute('disabled');
    }
    else {
      valuesSelect.disabled = 'disabled';
    }
  }
  else {
      console.log('no values object with that name.');
  }
}

function confirmDelete() {
    return confirm('Are you sure you want to delete this rule?');
}

function checkRedirectText() {
    if(document.getElementById('theme_list').value == 'url_redirect') {
        document.getElementById('url_textfield').style.display = 'inline';
    }
    else {
        document.getElementById('url_textfield').style.display = 'none';
    }
}
