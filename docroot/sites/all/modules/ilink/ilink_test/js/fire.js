(function ($) {

  Drupal.behaviors.ilink_test = {

    attach: function (context, settings) {

      Drupal.Ilink.fire.events.mouseenter = function (data) {
        console.log(data);
      }
    }
  }
})(jQuery);
