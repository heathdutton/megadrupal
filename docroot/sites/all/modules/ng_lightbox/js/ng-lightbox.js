(function ($) {
  "use strict";
  Drupal.behaviors.ng_lightbox = {
    attach: function (context, settings) {

      // Setup the first time.
      if (!settings.ng_lightbox_init) {
        this.initialize();
        settings.ng_lightbox_init = true;
      }

      var self = this;
      $('.lightbox__overlay').click(function(e) {
        if ($(e.target).is('.lightbox__overlay, .lightbox__header')) {
          self.hide($(this).closest('.lightbox__overlay'));
        }
      });
    },

    /**
     * Hide the given lightbox overlay.
     *
     * @param $overlay
     */
    hide: function($overlay) {
      // Remove the lock class on body.
      $('body').removeClass('lock');
      $overlay.addClass('ng-hidden');
      $('#ng-lightbox').empty();
    },

    /**
     * Initialize the lightbox the first time.
     */
    initialize: function() {
      // Append an empty div with an id that we can use for appending our AJAX
      // results to.
      var $body = $('body');
      $body.addClass('ng-lightbox-ready');
      $body.append('<div id="ng-lightbox"></div>');

      // Bind an event handler to document that hides all lightboxes.
      var self = this;
      $(document).keyup(function(e) {
        // I couldn't bare to have a lightbox without a keyboard shortcut to
        // close it.
        if (e.which === 27) {
          self.hide($('.lightbox__overlay'));
        }
      });
    }
  };

  Drupal.behaviors.ng_ajax_link = {
    attach: function (context, settings) {

      $('.ng-lightbox:not(.ng-lightbox-processed)').addClass('ng-lightbox-processed').each(function () {
        var base = 'ng-lightbox';
        var element_settings = {
          url: settings.basePath + 'ng-lightbox',
          event: 'click',
          wrapper: base,
          method: 'append',
          submit: {
            ng_lightbox_path: $(this).attr('href')
          }
        };

        Drupal.ajax[base] = new Drupal.ajax(base, this, element_settings);
      });
    }
  };

})(jQuery);
