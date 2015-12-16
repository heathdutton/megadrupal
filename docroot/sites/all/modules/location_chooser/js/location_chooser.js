// $Id$

(function ($) {

Drupal.behaviors.locationChooser = {
  attach: function (context, settings) {
    $("#edit-location-chooser", context).change(function() {
      var newLocation = $(this).val();
      var newLocationData = Drupal.settings.location_chooser[newLocation];
      $.each(newLocationData, function(key, value) {
        $("#edit-locations-0-" + key).val(value);
      })
    })
    $("fieldset.location .form-item input", context).change(function() {
      $("#edit-location-chooser").val(0);
    })
  }
};

}(jQuery));
