/**
 * Behaviors for adding SWF object files into a document.
 * @file
 */

(function($) { 
/**
 * Check the settings array and embed SWF objects.
 */
  Drupal.behaviors.SWFEmbed = {
    attach: function(context,settings) {
       jQuery.each(settings.swfembed.swf, function (name, value) {
           $('#' + name + ':not(object)',context).swfEmbed(value.url, value);
      });
    }
  }
})(jQuery);
