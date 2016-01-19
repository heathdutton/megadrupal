(function ($) {
  // Trigger and Bind support
  // Check:
  //   -> http://jamiethompson.co.uk/web/2008/06/17/publish-subscribe-with-jquery/
  //   -> http://anton.shevchuk.name/javascript/jquery-for-beginners-events/

  Drupal.behaviors.helloBar = {
    attach: function(context, settings) {
      function getRandomInt(min, max) {
        return Math.floor(Math.random() * (max - min + 1)) + min;
      }

      var key = getRandomInt(0, settings.HelloBar.length - 1);

      var message = settings.HelloBar[key].message;
      var options = settings.HelloBar[key].options;

      new HelloBar(message, options);

      // Trigger and Bind support here!
      if (settings.HelloBar[key].nid) {
        $(document).trigger('hellobar.show', settings.HelloBar[key].nid);
      }
    }
  };
})(jQuery);
