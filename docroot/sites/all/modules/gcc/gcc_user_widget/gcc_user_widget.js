/**
 * @file
 * TODO.
 */

(function ($) {

/**
 * Attaches the Ajax behavior to each Ajax form element.
 */
Drupal.behaviors.gccUserWidget = {

  attach: function(context, settings) {

    for (var id in settings.gccUserWidget) {

      $('#' + id, context).once('gcc-user-widget').each(function() {

        var container = $('<span></span>').css('position', 'relative').attr('id', id + '-container').insertAfter($(this).parent());

        var ajax = new Drupal.ajax(id + ' .button', this, {

          url: settings.gccUserWidget[id].path,
          event: 'click',
          effect: 'none',
          method: 'append',
          wrapper: id + '-container',
          prevent: 'click'
        });
      });

      $('.gcc-user-widget-cancel', context).once('gcc-user-widget').click(function() {

        $(this).parents('.gcc-user-widget-form').remove();
        return false;
      });
    }
  }
};

})(jQuery);
