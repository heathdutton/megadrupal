/**
 * @file
 * Some UI improvements.
 */
(function ($) {
  Drupal.behaviors.seventeen = {
    attach: function(context) {

      $('.messages:not(:has(span.hide-message))').prepend('<span class="hide-message">X</span>').not('.view-changed').fadeIn(1000);
      $('.hide-message').click(function(){$(this).parent().fadeOut()});

      $('body.page-admin-modules td.checkbox').each(function(index, value) {
        if ($(value).find('input').attr('disabled') && !$(value).find('input').attr('checked')) {
          $(value).next().find('strong').css('color', '#999')
        }
      })
  
    }
  }
})(jQuery);
