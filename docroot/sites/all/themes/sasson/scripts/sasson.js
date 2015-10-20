/**
 * sasson javascript core
 *
 */
(function($) {

  Drupal.behaviors.sasson = {
    attach: function(context, settings) {

      $('html').removeClass('no-js');

    }
  };

  // Console.log wrapper to avoid errors when console is not present
  // usage: log('inside coolFunc',this,arguments);
  // paulirish.com/2009/log-a-lightweight-wrapper-for-consolelog/
  window.log = function() {
    log.history = log.history || [];   // store logs to an array for reference
    log.history.push(arguments);
    if (this.console) {
      console.log(Array.prototype.slice.call(arguments));
    }
  };

})(jQuery);
