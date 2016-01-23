function spider_calendar_add_0(id) {
  input = document.getElementById(id);
  if (input.value.length == 1) {
    input.value = '0' + input.value;
    input.setAttribute("value", input.value);
  }
}

function spider_calendar_checkhour(tagid) {
  if (typeof(event) != 'undefined') {
    // For trans-browser compatibility.
    var e = event;
    var charCode = e.which || e.keyCode;
    if (charCode > 31 && (charCode < 48 || charCode > 57)) {
      return false;
    }
    hour = "" + document.getElementById(tagid).value + String.fromCharCode(e.charCode);
    hour = parseFloat(hour);
    if ((hour < 0) || (hour > 23)) {
      return false;
    }
  }
  return true;
}

function spider_calendar_checkminute(id) {
  if (typeof(event) != 'undefined') {
    // For trans-browser compatibility.
    var e = event;
    var charCode = e.which || e.keyCode;
    if (charCode > 31 && (charCode < 48 || charCode > 57)) {
      return false;
    }
    minute = "" + document.getElementById(id).value + String.fromCharCode(e.charCode);
    minute = parseFloat(minute);
    if ((minute < 0) || (minute > 59)) {
      return false;
    }
  }
  return true;
}
