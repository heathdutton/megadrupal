(function ($) {
  Drupal.behaviors.voipnumberfield = {};
  Drupal.behaviors.voipnumberfield.attach = function(context, settings) {
    //Hide conditional form fields

    voipnumberfield_hide_form_elements();

    //Check which radio button was selected to display appropriate fields
    if ($("#edit-voipnumberfield-choice--2").attr("checked")) {
      $(".form-item-field-settings-voipnumberfield-choice-country-list-voipnumberfield-allowed-countries").show();
    }
    else if ($("#edit-voipnumberfield-choice--3").attr("checked")) {
      $(".form-item-field-settings-voipnumberfield-choice-default-country-voipnumberfield-default").show();
    }

    /*Default country code radio button*/
    $("#edit-voipnumberfield-choice").change(function () {
      voipnumberfield_hide_form_elements();
    });

    /*Let user select from list radio button*/
    $("#edit-voipnumberfield-choice--2").change(function () {
      voipnumberfield_hide_form_elements();
      $(".form-item-field-settings-voipnumberfield-choice-country-list-voipnumberfield-allowed-countries").fadeIn();
    });

    /*Use default radio button*/
    $("#edit-voipnumberfield-choice--3").change(function () {
      voipnumberfield_hide_form_elements();
      $(".form-item-field-settings-voipnumberfield-choice-default-country-voipnumberfield-default").fadeIn();
    });

    //Make sure that default checkbox is selected only once per node
    $(".voipnumber-default").change(function () {
      if($(this).is(':checked')) {
        $(".voipnumber-default").attr("checked", false);
        $(this).attr("checked", true);
      }
    });

    function voipnumberfield_hide_form_elements() {
      $(".form-item-field-settings-voipnumberfield-choice-country-list-voipnumberfield-allowed-countries").hide();
      $(".form-item-field-settings-voipnumberfield-choice-default-country-voipnumberfield-default").hide();
    }
  };
})(jQuery);



