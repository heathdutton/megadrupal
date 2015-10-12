/**
 * @file
 * Client side features for the Table Size settings form.
 */

(function($){
  $(document).ready(function() {
    $('#table-size-min').timepicker({
      showLeadingZero: true,
      onHourShow: tpStartOnHourShowCallback,
      onMinuteShow: tpStartOnMinuteShowCallback
    });
    $('#table-size-max').timepicker({
      showLeadingZero: true,
      onHourShow: tpEndOnHourShowCallback,
      onMinuteShow: tpEndOnMinuteShowCallback
    });
  });

  function tpStartOnHourShowCallback(hour) {
    var tpEndHour = $('#table-size-max').timepicker('getHour');
    // Check if proposed hour is prior or equal to selected end time hour
    if (hour <= tpEndHour) { 
      return true;
    }
    // if hour did not match, it can not be selected
    return false;
  }
  function tpStartOnMinuteShowCallback(hour, minute) {
    var tpEndHour = $('#table-size-max').timepicker('getHour');
    var tpEndMinute = $('#table-size-max').timepicker('getMinute');
    // Check if proposed hour is prior to selected end time hour
    if (hour < tpEndHour) { 
      return true;
    }
    // Check if proposed hour is equal to selected end time hour and minutes is prior
    if ((hour == tpEndHour) && (minute < tpEndMinute)) { 
      return true;
    }
    // if minute did not match, it can not be selected
    return false;
  }

  function tpEndOnHourShowCallback(hour) {
    var tpStartHour = $('#table-size-min').timepicker('getHour');
    // Check if proposed hour is after or equal to selected start time hour
    if (hour >= tpStartHour) { 
      return true;
    }
    // if hour did not match, it can not be selected
    return false;
  }
  function tpEndOnMinuteShowCallback(hour, minute) {
    var tpStartHour = $('#table-size-min').timepicker('getHour');
    var tpStartMinute = $('#table-size-min').timepicker('getMinute');
    // Check if proposed hour is after selected start time hour
    if (hour > tpStartHour) { 
      return true;
    }
    // Check if proposed hour is equal to selected start time hour and minutes is after
    if ((hour == tpStartHour) && (minute > tpStartMinute)) {
      return true;
    }
    // if minute did not match, it can not be selected
    return false;
  }
})(jQuery);
