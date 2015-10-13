/**
 * @file
 * Microtemplating example JavaScript behavior.
 */

(function($) {
  Drupal.behaviors.microtemplatingExample = {
    attach : function(context, settings) {
      $('#todoapp', context).once('todoapp', function() {
        new TodoApp(this);
      });
    }
  };
})(window.jQuery);
