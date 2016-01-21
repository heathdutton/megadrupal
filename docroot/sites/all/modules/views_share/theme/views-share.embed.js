(function ($) {

  Drupal.behaviors.viewsShareEmbed = {
    attach: function(context) {
    	if (Drupal.settings.viewsShareEmbed.rewriteLinks) {
      	$('a', context).not('.views-share-embedded').attr('target', '_blank');
      }
    }
  }

})(jQuery);
