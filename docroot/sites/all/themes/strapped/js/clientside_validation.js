//jQuery wrapper
(function ($) {
  "use strict";
  //Define a Drupal behaviour with a custom name
  Drupal.behaviors.strappedClientsideValidation = {
    attach: function (context) {
      if(Drupal.hasOwnProperty('clientsideValidation')){
        Drupal.clientsideValidation.prototype.strappedErrorPlacement = function (error, element) {
          // We need to distinguish between errors in #input_groups and those that are not.
          var parent = $(element).parent();
          if (parent.hasClass('input-group')) {
            // If the form element is inside and input group then we need to place the error just outside of it.
            error.insertAfter(parent);
          } else {
            // If the element is NOT inside an input group then place the error normally.
            error.insertAfter(element);
          }
        };
      }


    }
  }
})(jQuery);





