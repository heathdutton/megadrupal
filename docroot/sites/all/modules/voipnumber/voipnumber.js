Drupal.behaviors.voipnumber = function (context) {
  //Hide conditional form fields
  voipnumber_hide_form_elements();

  //Check which radio button was selected to display appropriate fields
  if ($("#edit-voipnumber-choice-1").attr("checked")) {
    $("#edit-voipnumber-allowed-countries-wrapper").show();
  }
  else if ($("#edit-voipnumber-choice-2").attr("checked")) {
    $("#edit-voipnumber-show-code-wrapper").show();
  }

  /*Default country code radio button*/
  $("#edit-voipnumber-choice").change(function () {
    voipnumber_hide_form_elements();
  });

  /*Let user select from list radio button*/
  $("#edit-voipnumber-choice-1").change(function () {
    voipnumber_hide_form_elements();
    $("#edit-voipnumber-allowed-countries-wrapper").fadeIn();
  });

  /*Use default radio button*/
  $("#edit-voipnumber-choice-2").change(function () {
    voipnumber_hide_form_elements();
    $("#edit-voipnumber-show-code-wrapper").fadeIn();
  });
}

function voipnumber_hide_form_elements() {
  $("#edit-voipnumber-allowed-countries-wrapper").hide();
  $("#edit-voipnumber-show-code-wrapper").hide();
}
