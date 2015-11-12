(function ($) {

  Drupal.behaviors.international_phone = {
    attach: function (context, settings) {
      $(".international_phone-number").intlTelInput({
        //utilsScript: "lib/libphonenumber/build/utils.js"
      });
    }
  };

}(jQuery));
