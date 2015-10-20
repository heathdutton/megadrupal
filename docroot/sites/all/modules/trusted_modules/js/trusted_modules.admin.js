(function($) {
Drupal.behaviors.trusted_modules_admin = {
  attach: function (context, settings) {
    // Make the table sortable
    $('.tablesorter').tablesorter({
        headers: { 
            0: { 
                sorter: false 
            }
        } 
    }); 
  }
};
})(jQuery);
