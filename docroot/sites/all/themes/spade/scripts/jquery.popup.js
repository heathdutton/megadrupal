/**
 * @file
 * Keyboard-navigable button/action that discloses a popup, optionally
 * containing a Menu.
 */

;( function( $, document, undefined ) {
  "use strict";

  var defaults = {
    trigger: '.js-popup__trigger',
    triggerActiveClass: 'is-active',
    target: '.js-popup__target',
    targetActiveClass: 'is-open',
    menu: '.js-popup__menu'
  };

  function Popup(element, options) {
    // Instance properties
    this.settings = $.extend({}, defaults, options);
    this.isActive = false;

    // DOM references
    this.element = element;
    this.$element = $(element);
    this.trigger = this.element.querySelector(this.settings.trigger);
    this.$trigger = $(this.trigger);
    this.popup = this.element.querySelector(this.settings.target);
    this.$popup = $(this.popup);

    this.init();
  }

  Popup.prototype = {

    init: function() {
      var self = this;

      this.$element.on({
        // Listen for popups announcing their open event and close if we are not
        // the source of the event (only one popup open at a time).
        // This event is custom and can’t be delegated by normal means.
        'popup:open': function(event) {
          if (self.isActive && event.source !== self.element) self.close();
        }
      });

      // All handlers are namespaced and delegated to the body.
      $(document).on({
        'keydown.popup': function(event) {
          // An 'escape' keypress anywhere closes all open popups.
          if (event.which === 27) {
            self.close();
          }
          // A 'down arrow' keypress on a trigger opens its popup.
          if (event.target === self.trigger && event.which === 40) {
            if (! self.isActive) self.open(event);
          }
        },

        'click.popup': function(event) {
          // A click on or inside a trigger toggles its popup.
          if ( $(event.target).closest(self.trigger).length ) {
            event.preventDefault();
            self.toggle();
          }
          // A click outside of a particular trigger and its popup closes the
          // popup.
          else if ( ! $(event.target).closest(self.popup).length ) {
            self.close();
          }
        },

        'focusin.popup': function() {
          // Initialization of a child menu (if any) is deferred until the
          // popup element gains focus.
          if ( $(event.target).closest(self.element).length ) {
            var menuElement = self.element.querySelector(self.settings.menu);
            if (menuElement !== null) $(menuElement).menu();
          }
        }
      });

      // Tag the element as initialized and listening for published events.
      this.$element.addClass('is-popup is-listening');
    },

    open: function(originalEvent) {
      this.isActive = true;
      this.$trigger.addClass(this.settings.triggerActiveClass);
      this.$popup.addClass(this.settings.targetActiveClass);
      // Expose the popup to screen readers
      this.popup.setAttribute('aria-hidden', false);

      if (!originalEvent) {
        originalEvent = null;
      }

      // Notify each listening component that a popup has opened, tell them
      // which one, and (if provided) include the original event object that
      // caused the popup to open.
      $('.is-listening').trigger({
        type: 'popup:open',
        source: this.element,
        original: originalEvent
      });
    },

    close: function() {
      // Hide the popup from screen readers.
      this.popup.setAttribute('aria-hidden', true);
      this.$popup.removeClass(this.settings.targetActiveClass);
      this.$trigger.removeClass(this.settings.triggerActiveClass);
      this.isActive = false;

      // If focus was within the popup, re-focus the popup trigger. Prevents
      // focus being lost inside the hidden popup.
      if ( $(document.activeElement).closest(this.popup).length ) {
        this.$trigger.focus();
      }

      // Annouce the close event to any subscribers.
      $('.is-listening').trigger('popup:close', this.element);
    },

    toggle: function(event) {
      if ( this.isActive ) {
        this.close();
      }
      else {
        this.open();
      }
    }

  };

  // Use the plugin factory (jquery.plugin.js) to register the plugin.
  $.plugin('popup', Popup);

})( jQuery, document );
