(function ($) {
  Drupal.brightcoveLibrary = Drupal.brightcoveLibrary || {};
  Drupal.brightcoveLibrary.library = Drupal.brightcoveLibrary.library || {};
  Drupal.brightcoveLibrary.loaded = Drupal.brightcoveLibrary.loaded || false;

  Drupal.behaviors.brightcoveLibrary = {
    attach: function (context, settings) {

      // Safely override the original function, using Proxy Pattern (http://docs.jquery.com/Types#Proxy_Pattern)
      var proxied = Drupal.media.browser.validateButtons;
      Drupal.media.browser.validateButtons = function() {
        if (this.id === 'media-tab-brightcove') {
          $('a.fake-cancel').bind('click', Drupal.media.browser.submit);
        }
        else {
          return proxied.apply(this);
        }
      };

      // Check if object already exists
      if (typeof Drupal.brightcoveLibrary.library.start != 'function') {
        Drupal.brightcoveLibrary.library = new Drupal.media.browser.library(Drupal.settings.media.browser.brightcove);
      }

      $('#media-browser-tabset').not('.bc-processed').addClass('bc-processed').bind('tabsselect', function (event, ui) {
        if (ui.tab.hash === '#media-tab-brightcove') {
          // Prevent reloading of media list on tabselect if already loaded in media list
          if (!Drupal.brightcoveLibrary.loaded) {
            var params = {};
            for (var p in Drupal.settings.media.browser.brightcove) {
              params[p] = Drupal.settings.media.browser.brightcove[p];
            }

            Drupal.brightcoveLibrary.library.start($(ui.panel), params);
            $('#scrollbox').bind('scroll', Drupal.brightcoveLibrary.library, Drupal.brightcoveLibrary.library.scrollUpdater);
            Drupal.brightcoveLibrary.loaded = true;
          }
        }
      });

      $('#edit-filter').not('.bc-processed').addClass('bc-processed').click(function(ev) {
        ev.preventDefault();

        // Acquire filter form values
        var searchVal = $(Drupal.brightcoveLibrary.library.renderElement).find('.search-radio:checked').val();
        var keywordsVal = $(Drupal.brightcoveLibrary.library.renderElement).find('#edit-keywords').val();
        // set library object parameters (used for ajax loading new media into list)
        Drupal.brightcoveLibrary.library.params.filter = {search: searchVal, keywords: keywordsVal};

        // Remove the media list
        Drupal.brightcoveLibrary.library.cursor = 0;
        Drupal.brightcoveLibrary.library.mediaFiles = [];
        $(Drupal.brightcoveLibrary.library.renderElement).find('#media-browser-library-list li').remove();
        $('#scrollbox').unbind('scroll').bind('scroll', Drupal.brightcoveLibrary.library, Drupal.brightcoveLibrary.library.scrollUpdater);

        // Set a flag so we don't make multiple concurrent AJAX calls
        Drupal.brightcoveLibrary.library.loading = true;
        // Reload the media list
        Drupal.brightcoveLibrary.library.loadMedia();
      });

      $('#edit-reset').not('.bc-processed').addClass('bc-processed').click(function(ev) {
        ev.preventDefault();

        // Reset filter form values
        delete Drupal.brightcoveLibrary.library.params.filter;
        $(Drupal.brightcoveLibrary.library.renderElement).find('.search-radio[value=name]').attr('checked', true);
        $(Drupal.brightcoveLibrary.library.renderElement).find('#edit-keywords').val('');
        $('#scrollbox').unbind('scroll').bind('scroll', Drupal.brightcoveLibrary.library, Drupal.brightcoveLibrary.library.scrollUpdater);

        // Remove the media list
        Drupal.brightcoveLibrary.library.cursor = 0;
        Drupal.brightcoveLibrary.library.mediaFiles = [];
        $(Drupal.brightcoveLibrary.library.renderElement).find('#media-browser-library-list li').remove();
        // Set a flag so we don't make multiple concurrent AJAX calls
        Drupal.brightcoveLibrary.library.loading = true;
        // Reload the media list
        Drupal.brightcoveLibrary.library.loadMedia();
      });

      $(document).delegate('#media-browser-library-list a', 'mousedown', function() {
        var uri = $(this).attr('data-uri');
        $("input[name='submitted-video']").val(uri);
        var file = {uri: uri};
        var files = new Array();
        files.push(file);
        Drupal.media.browser.selectMedia(files);
      });
    }
  };

  Drupal.ajax.prototype.commands.brightcove_media_upload = function (ajax, response, status) {
    Drupal.brightcoveLibrary.library.mediaSelected([response.data]);
    $(Drupal.brightcoveLibrary.library.renderElement).find('.fake-ok').trigger('click');
  };
})(jQuery);
