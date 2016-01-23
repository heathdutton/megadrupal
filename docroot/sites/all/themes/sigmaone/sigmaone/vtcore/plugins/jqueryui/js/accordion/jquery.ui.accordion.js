/**
 * Main file to initialize all jQuery UI Misc elements
 */
(function($) {
  Drupal.behaviors.jqueryUIAccordionInit = { 
    attach: function() {
      var target = $('.ui-accordion'),
          icons = {
          header: 'ui-icon-circle-arrow-e',
          headerSelected: 'ui-icon-circle-arrow-s'
      };
      target.each(function() {
        var self = $(this);
        
        // Remove icon     
        self.find('.ui-icon')
            .remove();
        
        // Remove active state
        self.children('h3:gt(0)')
            .removeClass('ui-state-active');
        
        // Build accordion
        self.once().accordion({
          icons: icons
        });
        
        // Add Extra Class that somehow is missing
        self.find('.ui-accordion-header.ui-state-active').addClass('ui-corner-top');

        self.find('.ui-header-accordion-button')
            .show()
            .UIhover()
            .click(function() {
              window.location = $(this).attr('href');
              return false;
            });        
      });

   }
  };
})( jQuery );
