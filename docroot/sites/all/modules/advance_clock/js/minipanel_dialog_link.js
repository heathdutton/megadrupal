(function($) {
  // Store our function as a property of Drupal.behaviors.
  Drupal.behaviors.minipanel_dialog_link = { attach: function(context, settings) {
      $('.minipanel-dialog-link-link a', context, settings).once('.minipanel-dialog-link-link a', function() {
        if ( $(this).attr('title') != '<blank>' ) {
          var title = $(this).attr('title');
        }
        else {
          var title = '';
        }
        var $mini = $(this).parent().parent().find('.minipanel-dialog-link-mini');
        $mini.dialog({ "modal": true, "draggable": false, "width": 560, "title": title, "resizable": false, "autoOpen": false });
        $(this).click(function(e) {
          e.preventDefault();
          $mini.dialog("open");
          return false;
        });
      });
    }
  };
}(jQuery));
