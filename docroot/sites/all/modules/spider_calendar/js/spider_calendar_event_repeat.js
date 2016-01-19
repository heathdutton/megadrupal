function spider_calendar_change_type(type) {
  if (document.getElementById('daily1').value == '') {
    document.getElementById('daily1').value = 1;
  }
  else {
    document.getElementById('repeat_input').removeAttribute('style');
  }
  if (document.getElementById('weekly1').value == '') {
    document.getElementById('weekly1').value = 1;
  }
  if (document.getElementById('monthly1').value == '') {
    document.getElementById('monthly1').value = 1;
  }
  if (document.getElementById('yearly1').value == '') {
    document.getElementById('yearly1').value = 1;
  }
  switch (type) {
    case 'no_repeat':
      // On add new event date_end has no required attribute.
      if (document.getElementById('date_end').getAttribute("required") != null) {
        document.getElementById('date_end').removeAttribute("required");
      }
      document.getElementById('daily').setAttribute('style','display:none');
      document.getElementById('weekly').setAttribute('style','display:none');
      document.getElementById('monthly').setAttribute('style','display:none');
      document.getElementById('year_month').setAttribute('style','display:none;');
      //document.getElementById('repeat_input').value = 1;
      //document.getElementById('month').value = '';
      document.getElementById('date_end').value = '';
      document.getElementById('repeat_until').setAttribute('style','display:none');
      break;

    case 'daily':
      document.getElementById('date_end').required = "required";
      document.getElementById('daily').removeAttribute('style');
      document.getElementById('weekly').setAttribute('style','display:none');
      document.getElementById('monthly').setAttribute('style','display:none');
      document.getElementById('repeat').innerHTML = 'Day(s)';
      document.getElementById('repeat_input').value = document.getElementById('daily1').value;
      //document.getElementById('month').value = '';
      document.getElementById('year_month').setAttribute('style','display:none');
      document.getElementById('repeat_until').removeAttribute('style');
      document.getElementById('repeat_input').onchange = function onchange(event) {return spider_calendar_input_value('daily1')};
      break;

    case 'weekly':
      document.getElementById('date_end').required = "required";
      document.getElementById('daily').removeAttribute('style');
      document.getElementById('weekly').removeAttribute('style');
      document.getElementById('weekly').setAttribute('style','margin:5px;');
      document.getElementById('monthly').setAttribute('style','display:none');
      document.getElementById('repeat').innerHTML = 'Week(s) on :'
      document.getElementById('repeat_input').value = document.getElementById('weekly1').value;
      //document.getElementById('month').value = '';
      document.getElementById('year_month').setAttribute('style','display:none');
      document.getElementById('repeat_until').removeAttribute('style');
      document.getElementById('repeat_input').onchange = function onchange(event) {return spider_calendar_input_value('weekly1')};
      break;

    case 'monthly':
      document.getElementById('date_end').required = "required";
      document.getElementById('daily').removeAttribute('style');
      document.getElementById('weekly').setAttribute('style','display:none');
      document.getElementById('monthly').removeAttribute('style');
      document.getElementById('repeat').innerHTML = 'Month(s)';
      document.getElementById('repeat_input').value = document.getElementById('monthly1').value;
      //document.getElementById('month').value='';
      document.getElementById('year_month').setAttribute('style','display:none');
      document.getElementById('repeat_until').removeAttribute('style');
      document.getElementById('repeat_input').onchange = function onchange(event) {return spider_calendar_input_value('monthly1')};
      break;

    case 'yearly':
      document.getElementById('date_end').required = "required";
      document.getElementById('daily').removeAttribute('style');
      document.getElementById('year_month').removeAttribute('style');
      document.getElementById('year_month').setAttribute('style','display:inline;');
      document.getElementById('weekly').setAttribute('style','display:none');
      document.getElementById('monthly').removeAttribute('style');
      document.getElementById('repeat').innerHTML = 'Year(s) in ';
      document.getElementById('repeat_input').value = document.getElementById('yearly1').value;
      //document.getElementById('month').value = '';
      document.getElementById('repeat_until').removeAttribute('style');
      document.getElementById('repeat_input').onchange = function onchange(event) {return spider_calendar_input_value('yearly1')};
      break;
  }
}

function spider_calendar_radio_month() {
  if (document.getElementById('radio1').checked == true) {	
    document.getElementById('monthly_list').disabled = true;
    document.getElementById('month_week').disabled = true;
    document.getElementById('month').disabled = false;
  }
  else {
    document.getElementById('month').disabled = true;
    document.getElementById('monthly_list').disabled = false;
    document.getElementById('month_week').disabled = false;
  }
}

function spider_calendar_checknumber(id) {
  if (typeof(event) != 'undefined') {
    var e = event;
    var charCode = e.which || e.keyCode;
    if (charCode > 31 && (charCode < 48 || charCode > 57)) {
      return false;
    }
  }
  return true;
}

function spider_calendar_week_value() {
  var value = '';
  for (i = 1; i <= 7; i++) {
    if (document.getElementById('week_'+i).checked) {
      value = value + document.getElementById('week_' + i).value + ',';
    }
  }
  document.getElementById('week').value = value;
}

function spider_calendar_input_value(id) {
  document.getElementById(id).value = document.getElementById('repeat_input').value;
}
