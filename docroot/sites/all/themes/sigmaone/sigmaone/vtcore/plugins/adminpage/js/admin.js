/**
 * Main file to initialize all jQuery related to admin pages
 */
(function($) {
  Drupal.behaviors.adminPage = { 
    attach: function() {
      // Intialize table
      var options = {
          rowClass: 'system-themes-row',
          children: '.theme-selector',
          useHeight: true
      };
      $('.system-themes-list-disabled').once('system-themes-list-disabled').equalheightrow(options);
   }
  };
})( jQuery );
