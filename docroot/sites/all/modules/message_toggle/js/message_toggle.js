/**
 * @file
 *   JavaScript for the Message Toggle module.
 */

(function ($) {
  Drupal.behaviors.messageToggle = {
    attach: function(context) {

      $('.messages').once('message-toggle', function(){
        var $this = $(this).addClass('message-toggle-expanded');

        // Creates wrapper. Adds a class to designate status, warning, error.
        var $messagesWrapper = $('<div>').addClass('message-toggle-wrapper');
        $messagesWrapper.addClass($this.attr('class').split(" ")[1] + '-wrapper');

        // Provides SHOW link when collapsed.
        var $collapsed_message = $('<div>').addClass('messages').addClass('message-toggle-collapsed').hide();

        // HIDE and SHOW links.
        $this.prepend('<a class="message-toggle">' + Drupal.t('Hide') + '</a>');
        $collapsed_message.prepend('<a class="message-toggle">' + Drupal.t('Show') + '</a>');

        // Add the new elements.
        $this.wrap($messagesWrapper);
        $this.after($collapsed_message);

        // Toggle between expanded and collapsed when clicked.
        $this.parent('.message-toggle-wrapper').find('.message-toggle').click(function() {
          var $wrapper = $(this).parents('.message-toggle-wrapper');
          $wrapper.children('.message-toggle-expanded').toggle();
          $wrapper.children('.message-toggle-collapsed').toggle();
        });
      });

    }
  }
})(jQuery);
