/**
 * @file
 * Javascript file of Disable Context Menu module.
 */

(function ($) {

  Drupal.behaviors.disableContextMenu = {
    attach: function (context, settings) {
      // Getting the defined variables and the message.
      var selectors = settings.disableContextMenu.selectors;
      var message = settings.disableContextMenu.message;

      // Binding to the defined selectors.
      $(context).find(selectors).bind('contextmenu', function() {
        // If a message was set we bother the user with an annoying message.
        if(message) {
          alert(message);
        }

        // Disabling the context menu.
        return false;
      });
    }
  };

})(jQuery);
