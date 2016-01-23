/**
 * @file
 * Integrates Handlebars library and functionality with D7 core js.
 */

(function($, Drupal) {
   Handlebars.registerHelper("debug", function(optionalValue) {
     console.log("Current Context");
     console.log("====================");
     console.log(this);

     if (optionalValue) {
       console.log("Value");
       console.log("====================");
       console.log(optionalValue);
     }
   });
  /**
   * Initialize global object for holding Handlebar template definitions.
   */
  Drupal.handlebars = {};

  /**
   * Drupal behaviors for D3 module.
   */
  Drupal.behaviors.handlebars = {
    attach: function(context, settings) {
      var id, $context = $(context);
      // Check to see if templates were added. 
      if (settings.handlebars && settings.handlebars.inventory) {
        // Iterate over each template in the inventory.
        for (id in settings.handlebars.inventory) {
          // Only process the visualization once, if it exists.
          $context.find('#' + id).once('hbr', function () {
            Drupal.handlebars[id.replace(new RegExp('-', 'g'), '_')] = Handlebars.compile($('#' + id).html());
          });
        }
      }
    }
  };

})(jQuery, Drupal);
