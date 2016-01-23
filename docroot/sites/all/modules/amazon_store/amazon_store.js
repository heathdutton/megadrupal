
(function ($) {

  Drupal.behaviors.amazonStoreBehavior = {
    attach: function(context, settings) {
      // Handle the show/hide full description in search results.
      $('.togglebtn', context).click(function () {
        $(this).parent().find('.toggle').toggle();
      });

      // Hide any items with this class. To replace gone-missing no-js.
      $(".amazon-store-no-js").hide();
      
      $('.amazon-store-autosubmit').change(function() {
        $(this).closest("form").submit();
      });

    }
  };
})(jQuery);
