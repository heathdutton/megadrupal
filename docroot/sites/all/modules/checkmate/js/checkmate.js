(function ($) {
  Drupal.behaviors.checkmate_toggle = {
    attach: function (context) {
      $.each($('a.checkmate_toggle_check', context), function(key, item) {
        if (!$(item, context).hasClass('checkmate_bound')) {
          $(item, context).addClass('checkmate_bound');
          $(item, context).bind('click', function(e){
            target = $(e.currentTarget, context);
            $('.full_form_view', target.parent()).toggle();
            if ($('.full_form_view', target.parent()).css('display') == 'block') {
              target.html(Drupal.settings.checkmate.hide);
            }
            else {
              target.html(Drupal.settings.checkmate.show);
            }
            e.preventDefault();
          }, false);
        }
      });
    }
  };
})(jQuery);
