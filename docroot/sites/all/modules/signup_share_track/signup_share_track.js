(function ($) {
  Drupal.behaviors.sst_user_view_alter = {
    attach: function (context, settings) {
      $('body div[class="view-content"]', context).hide();
    }
  };
  
})(jQuery);