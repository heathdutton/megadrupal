(function($) {

Drupal.behaviors.devtools.components.js_suppress_alert = {
  attach: function (context, settings) {
    // override the default alert() function to hide the popups
    window.alert = function(text) {
      // send the alert to the console when console.log is defined
      if (typeof console.log == 'function') { 
        console.log(text);
      }
    }
  }
};

})(jQuery);