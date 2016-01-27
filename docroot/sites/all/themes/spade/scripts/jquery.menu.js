/**
 * @file
 * Keyboard navigable UI menu
 */

;( function( $, document, undefined ) {
  "use strict";

  var defaults = {
    item: '.js-menu__item'
  };

  var Menu = function(element, options) {
    // Instance properties
    this.settings = $.extend({}, defaults, options);
    this.activeItem = null;

    // DOM references
    this.element = element;
    this.$element = $(element);
    this.items = this.element.querySelectorAll(this.settings.item);
    this.$items = $(this.items);

    this.init();
  };

  Menu.prototype = {

    init: function() {
      var self = this;

      // Make the menu focusable, but only programmatically (script manages
      // focusing child items).
      this.element.setAttribute('tabindex', '-1');

      // Add a unique id, prefixed with the 'drupal' namespace, to each
      // menuitem. We need the ids so the aria-activedescendant attribute can
      // be programattically set to the currently-focused menuitem.
      $.each(this.items, function() {
        $(this).uniqueId('drupal');
      });

      // Point screen readers to the active menu item.
      this.$element.on('focusout.menu', function() {
        self.activeItem = null;
        self.element.removeAttribute('aria-activedescendant');
      });

      this.$element.on('focusin.menu', this.settings.item, function(event) {
        self.activeItem = event.target;
        self.element.setAttribute( 'aria-activedescendant', self.activeItem.getAttribute('id') );
      });

      this.$element.on({
        // On up/down arrow keypress, move focus up or down the menuitem list.
        'keydown.menu': function(event) {
          var index = self.$items.index(document.activeElement);
          var lastindex = self.$items.length - 1;

          // Determine which item needs focus and prevent scrolling the page.
          if ( event.which === 38 ) {  // key UPARROW
            event.preventDefault();
            index--;
            index = (index < 0) ? 0 : index;
          } else if (event.which === 40 ) {  // key DOWNARROW
            event.preventDefault();
            index++;
            index = (index > lastindex) ? lastindex : index;
          }

          // Focus the target menuitem.
          self.$items.get(index).focus();
        },

        // Listen for popups announcing their open event.
        'popup:open': function(event) {
          // If a popup was opened which contains this menu, focus the menu so
          // in order to capture keyboard events.
          if ( self.$element.closest(event.source) ) {
            self.$element.focus();

            // Inspect the original event that caused the popup to open.
            // If it was opened from the keyboard, focus the first menuitem.
            if ( event.original && event.original.type === 'keydown' ) {
              self.$items.first().focus();
            }
          }
        }
      });

      // Tag the element as initialized and listening for published events.
      this.$element.addClass('is-menu is-listening');
    }
  };

  // Use the plugin factory (jquery.plugin.js) to register the plugin.
  $.plugin('menu', Menu);

})( jQuery, document );
