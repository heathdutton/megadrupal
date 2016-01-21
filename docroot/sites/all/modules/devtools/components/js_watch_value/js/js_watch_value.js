(function($) {

Drupal.behaviors.js_watch_value = {
  attach: function (context, settings) {

  },

  /**
   * This implements a custom success/fail callback for our AJAX form, since there
   * isn't anything out of the box with Drupal's AJAX/FAPI implementation.
   *
   * @param  {[type]} default_value   Default value to identify a non-set flag (ex. -1).
   * @param  {[type]} value_callback  Callback function which will return the current
   *                                  flag value, and optionally change it to the passed value.
   * @param  {[type]} change_callback Callback function which will receive the new
   *                                  flag value (to act upon it) when it's detected.
   * @return {[type]}                 [description]
   */
  watch_value: function (default_value, value_callback, change_callback) {
    setTimeout(function() {
      var current_value = value_callback();
      // target value is still set to the default, so call self again
      if (current_value == default_value) {
        Drupal.behaviors.js_watch_value.watch_value(default_value, value_callback, change_callback);
      }
      // target value has changed, so pass it to the callback and reset
      else {
        change_callback(current_value);
        value_callback(default_value);
        return;
      }
    }, 200);
  }
};

})(jQuery);