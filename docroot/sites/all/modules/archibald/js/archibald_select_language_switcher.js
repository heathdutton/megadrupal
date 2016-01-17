(function ($) {

Drupal.behaviors.archibald_block_language_switcher = {
  attach: function (context, settings) {
    $(".archibald_select_language_switcher").unbind('change').bind('change', function() {
      document.location.href = $(this).val();
    });
  }
};


})(jQuery);