(function ($) {
  "use strict";

  /**
   * Setup NG Lightbox and apply to the appropriate elements.
   */
  Drupal.behaviors.ng_lightbox = {
    attach: function (context, settings) {

      // Setup the first time.
      if (!settings.ng_lightbox_init) {
        NGLightbox.initialize();
        settings.ng_lightbox_init = true;
      }

      // Bind an event handler to close NG Lightbox using either the header or overlay.
      $('.lightbox__overlay').click(function(e) {
        if ($(e.target).is('.lightbox__overlay, .lightbox__header')) {
          NGLightbox.hide($(this).closest('.lightbox__overlay').get(0));
        }
      });

      // Any element with the class ".ng-lightbox" will have NG Lightbox applied.
      $('.ng-lightbox:not(.' + NGLightbox.processed + ')').each(function () {
        NGLightbox.initElement(this);
      });
    }
  };

  /**
   * Defines the NG Lightbox object.
   */
  window.NGLightbox = {
    processed: 'ng-lightbox-processed',
    lightboxId: 'ng-lightbox',

    /**
     * Apply NG Lightbox to any dom element. Useful when you require a client side integration rather than theme_link().
     *
     * @param element
     *   The dom element to bind the lightbox to. Usually an anchor.
     * @param path
     *   The path that is loaded into the lightbox. If you use an anchor this can be blank and the href attribute will
     *   be used.
     */
    initElement: function(element, path) {
      var $element = $(element);

      // If we've already processed this element, do nothing.
      if ($element.hasClass(this.processed)) {
        return;
      }

      // If path is not defined, lets just presume that the element is an anchor
      // and grab the href attribute.
      if (typeof path === 'undefined') {
        path = $element.attr('href');
      }

      // Setup the AJAX settings for Drupal.ajax.
      var element_settings = {
        url: Drupal.settings.basePath + 'ng-lightbox',
        event: 'click',
        wrapper: this.lightboxId,
        method: 'append',
        submit: {
          ng_lightbox_path: path
        }
      };
      Drupal.ajax[this.lightboxId] = new Drupal.ajax(this.lightboxId, element, element_settings);
      $element.addClass(this.processed);

      // Override the Drupal.ajax success callback so we can do things when
      // NG Lightbox has loaded.
      var successCallback = Drupal.ajax[this.lightboxId].options.success;
      Drupal.ajax[this.lightboxId].options.success = function(response, status, xmlhttprequest) {
        successCallback(response, status, xmlhttprequest);

        // Attempt to set focus on the first input within the lightbox since
        // forms are common. However this is harmless if there are no forms and
        // we fire an additional event for others.
        var $lightbox = $('#' + NGLightbox.lightboxId);
        $lightbox.find('input').first().focus();
        $lightbox.trigger('ng-lightbox-loaded');
      };
    },

    /**
     * Hide the given lightbox overlay given a dom element.
     */
    hide: function(element) {
      var $element = $(element);

      // Remove the lock class on body.
      $('body').removeClass('lock');
      $element.addClass('ng-hidden');
      $('#' + this.lightboxId).empty();
    },

    /**
     * Initialize the NG Lightbox for the first time.
     */
    initialize: function() {
      // Append an empty div with an id that we can use for appending our AJAX
      // results to.
      $('body')
        .addClass('ng-lightbox-ready')
        .append('<div id="' + this.lightboxId + '"></div>');

      // Bind an event handler to document that hides all lightboxes.
      var self = this;
      $(document).keyup(function(e) {
        // I couldn't bare to have a lightbox without a keyboard shortcut to
        // close it.
        if (e.which === 27) {
          self.hide($('.lightbox__overlay').get(0));
        }
      });
    }
  };

})(jQuery);
