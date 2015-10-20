(function ($) {

  Drupal.behaviors.simpleantispam = {
    attach: function (context, settings) {
      if ($.cookie('simpleantispamState') == 1) {
        $('input[name="smplntspm1"]', context).attr('checked', 'checked').parent().hide();
      }
    }
  };

})(jQuery);