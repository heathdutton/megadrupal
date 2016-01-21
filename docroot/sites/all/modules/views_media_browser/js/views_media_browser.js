(function ($) {
  Drupal.behaviors.viewsMediaBrowserLinks = {
    attach: function (context, settings) {
      // Applying the filters.
      $('a.exposed-button').click(function (){
        // Find the parent form.
        var parent_forms = $('a.exposed-button').parents('form');
        var parent_form = parent_forms[0];
        // Reset our filters.
        Drupal.settings.media.browser.library['filters'] = {};
        // Pull out all current settings.
        Drupal.settings.media.browser.library['filters'] = $.deparam($(parent_form).serialize());

        var ui = {tab:{hash:'#media-tab-library'}, panel:'div#media-tab-library.media-browser-tab'};
        if (Drupal.behaviors.mediaLibrary.library) {
          var library = Drupal.behaviors.mediaLibrary.library;
          library.done = false;
          library.cursor = 0;
          library.mediaFiles = [];
          library.selectedMediaFiles = [];
          $('#scrollbox').unbind('scroll', library, library.scrollUpdater);
          library.loading = true;
          $('#media-browser-tabset').trigger('tabsshow', ui);
          $('#scrollbox').bind('scroll', library, library.scrollUpdater);
          // If completed, won't trigger anything, but don't want something bound twice.
        }
      });
      $('#edit-display-format').change(function() {
        var action = $(this).parents('form').attr('action');
        action += '#media-tab-library';
        $(this).parents('form').attr('action', action);
        $(this).parents('form').submit();
      });

      $('#edit-filename').keydown(function(e) {
        if (e.keyCode == 13){
          $('a.exposed-button').click();
          e.preventDefault();
        }
      });

    }
  };
})(jQuery);
