// $Id: inline_messages.js,v 1.3.4.1 2010/11/10 20:32:18 jsfwd Exp $

/**
 * Inline Mesages
 * check for form errors & move .messages inline with the form
 */

(function ($) {
  $(document).ready(function(){
    
    if (Drupal.settings.form_submitted) {
      $form_id = $('form#' + Drupal.settings.form_submitted);
      
      var msg = $('.messages');
      if (msg.length) {
        $form_id.before(msg.attr('id', 'inline-messages'));
        $settings = Drupal.settings.inline_messages_scrollto;
        // set where the window will scrollto, account for the toolbar
        var pos = msg.offset().top;
        if ($('#toolbar').length) {
          var pos = pos - $('#toolbar').height();
        }
        $.scrollTo(pos, $settings['duration'], {offset:$settings['offset']});
      }
    }
    
  });
})(jQuery);