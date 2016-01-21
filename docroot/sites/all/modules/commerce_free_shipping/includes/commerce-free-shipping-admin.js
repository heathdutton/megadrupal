/*
 * @file
 * JavaScript for mylucky_form_request form.
 */

(function($) {

  Drupal.behaviors.commerceFreeShippingActivateFieldsetHandling = {
    attach : function() {
      $('.commerce-free-shipping-admin-form .base-checkbox-enable').click(function(e) {
        var $base = $(this).closest('.base');
        var $title = $base.find('legend').first().find('a');
        var html = $title.html();
        if ($(this).is(':checked')) {
          $base.removeClass('active').addClass('active');
          $title.find('strong.active').removeClass('element-invisible');
          $title.find('strong.inactive').addClass('element-invisible');
        } else {
          $base.removeClass('active');
          $title.find('strong.active').addClass('element-invisible');
          $title.find('strong.inactive').removeClass('element-invisible');
        }
      });
    }
  };
})(jQuery);
