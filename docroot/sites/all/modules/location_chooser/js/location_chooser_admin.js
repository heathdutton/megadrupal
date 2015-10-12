// $Id$

(function ($) {

Drupal.behaviors.locationChooserAdmin = {
  attach: function (context, settings) {
    $(".form-item-location-chooser-targets input", context).change(function() {
      var myType = $(this).val();
      if ($(this).is(':checked')) {
        $('#location-chooser-warning').addClass(myType);
      }
      else {
        $('#location-chooser-warning').removeClass(myType);
      }
    })
  }
};

}(jQuery));
