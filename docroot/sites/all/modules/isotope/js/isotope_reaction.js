/**
 * @file Contains the js behavior for setting isotope configurations from
 * Context reactions.
 */
(function ($) {
  Drupal.behaviors.isotope_reaction = {
    attach: function (context, settings) {
      for (var machine_name in settings.isotope_reaction) {
        /**
         * All settings will be stored in:
         * Drupal.settings.isotope_reaction.CONTEXT_MACHINE_NAME
         *  - settings: The isotope-ready settings object
         *  - jqObj: the bound isotope jQuery element
         */
        reaction = settings.isotope_reaction[machine_name];

        // If specified, attempt to add appropriate css settings to items.
        if( typeof reaction.configuration.apply_css == 'object') {
          isoItems = $(reaction.configuration.isotope_config.itemSelector);
          isoItems.css({margin: '10px'});
          instance = isoItems.eq(0);
          for(attribute in reaction.configuration.apply_css) {
            switch (attribute) {
              case 'width':
                widthDiff = instance.outerWidth(true) - instance.width();
                isoItems.width(reaction.configuration.isotope_config[reaction.configuration.isotope_config.layoutMode].columnWidth - widthDiff);
                break;
              case 'height':
                heightDiff = instance.outerHeight(true) - instance.width();
                isoItems.height(reaction.configuration.isotope_config[reaction.configuration.isotope_config.layoutMode].rowHeight - heightDiff);
                break;
            }
          }
        }
        reaction.jqObj = $(reaction.configuration.container_selector, context);
        reaction.jqObj.isotope(reaction.settings, context);
      }
    }
  };
})(jQuery);
