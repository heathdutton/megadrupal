(function ($) {

  Drupal.behaviors.activeClass = {
    attach: function (context, settings) {
      $('#edit-fb-autopost-page :input:checked').closest('.form-item').addClass('active');
      $('#edit-fb-autopost-page :input').once('activeClass').click(function () {
        $(this).closest('.form-item').toggleClass('active');
      })
    }
  };

})(jQuery);