// Auto upload image & hide upload button.
(function ($) {
  Drupal.behaviors.commune_embed = {
    attach: function(context, settings) {
      if($.embedly) {
         $.embedly.defaults.key = Drupal.settings.commune_embed.embedly_key;
         $('.commune-embed-embedly a', context).embedly();
      }
    }
  }
})(jQuery);