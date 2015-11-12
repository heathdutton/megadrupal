/**
 * @file Add a little whiz to the configuration form by making the prefix
 * update automagically.
 */
(function($) {
  $(document).ready(function() {
    $('#edit-g2migrate-prefix').change(function (event) {
      $('.g2migrate-prefix').text(event.target.value);
    });
  });
})(jQuery);
