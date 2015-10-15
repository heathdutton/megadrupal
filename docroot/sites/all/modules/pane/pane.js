(function($) {
  Drupal.behaviors.pane = {
    attach: function(context, settings) {
      // Show the tab specified at the end of the URL
      var path = location.pathname.split('/');
      var pane = path[path.length - 1];
      if (pane.substr(0, 4) == 'pane') {
        var $tab = $('#edit-'+pane).data('verticalTab');
        if ($tab !== undefined) {
          $tab.tabShow();
        }
      }
    }
  }
})(jQuery);
