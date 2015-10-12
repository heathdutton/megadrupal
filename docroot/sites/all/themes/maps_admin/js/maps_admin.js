(function ($) {

/**
 * Provide some User friendly features, to help administrators on managing Drupal.
 */

Drupal.mapsAdminScrollHorz = Drupal.mapsAdminScrollHorz || { 'elements': [], 'timeout': 300, 'timeoutId': null };

/**
 * Attaches the MaPS Admin related behaviors.
 */
Drupal.behaviors.mapsAdmin = {
  attach: function (context, settings) {
    // Perform some changes on the HTML structure to simplify the stylesheet rules.
    $('body > #content #main_content form:not(.no_fieldset)').each(function() {
      if (!$(this).find('.sticky-header', 'fieldset').length) {
        $(this).addClass('no_fieldset');
      }
    });

    // Apply the jQuery Horizontal Scroll plugin.
    if (settings.mapsAdmin && settings.mapsAdmin.scrollHorz) {
      $(settings.mapsAdmin.scrollHorz + ':not(maps-admin-scroll-horz-processed)')
        .addClass('maps-admin-scroll-horz-processed')
        .each(function() {
          Drupal.mapsAdminScrollHorz.add($(this));
        });
    }
  }
};

/**
 * Add an element for horizontal scroll.
 */
Drupal.mapsAdminScrollHorz.add = function (element) {
  this.elements.push({ 'element': element, 'active': false, 'scrollWidth': null, 'scrollLeft': 0 });

  if (Drupal.mapsAdminScrollHorz.timeoutId == null) {
    Drupal.mapsAdminScrollHorz.loop();
  }
};

/**
 * Activate an element for horizontal scrolling.
 */
Drupal.mapsAdminScrollHorz.activate = function (object) {
  var self = this;
  object.active = true;

  object.element
    .bind('mousewheel', function(event, delta) {
      if (event.shiftKey) {
        this.scrollLeft -= (delta * 30);
        event.preventDefault();

        // Take care of table sticky header if any. Handle this here make
        // the refresh to be immediate so the header movement is more fluid.
        self.recalculateStickyHeader();

        // Avoid the periodic loop to recalculate the sticky header since
        // it has just be done.
        object.scrollLeft = this.scrollLeft;
      }
   })
   .addClass('mousewheel-active')
   .prepend('<div class="ui_info"><span>' + Drupal.t('SHIFT+SCROLL for horizontal scroll') + '</span></div>');
};

/**
 * Deactivate an element for horizontal scrolling.
 */
Drupal.mapsAdminScrollHorz.deactivate = function (object) {
  object.active = false;

  object.element
    .unbind('mousewheel')
    .removeClass('mousewheel-active');

  object.element.children('.ui_info').remove();
};

/**
 * Provide horizontal scroll support for Drupal sticky table header.
 */
Drupal.mapsAdminScrollHorz.recalculateStickyHeader = function () {
  $(window).triggerHandler('resize.drupal-tableheader');
}

/**
 * Handle a width resize event.
 *
 * Since there is no event for scroll changes, we need to emulate one,
 * using a periodic check.
 */
Drupal.mapsAdminScrollHorz.loop = function() {
  var self = this;

  if (self.elements.length) {
    this.timeoutId = setTimeout(function() {

      for (var i = 0; i < self.elements.length; i++) {
        var object = self.elements[i];
        var scrollWidth = object.element[0].scrollWidth;
        var scrollLeft = object.element.scrollLeft();
        var needScroll = scrollWidth > object.element.outerWidth();

        if (needScroll && !object.active) {
          if (object.element.hasClass('mousewheel-active')) {
            object.active = true;
          }
          else {
            self.activate(object);
          }
        }
        // Or remove the horizontal scroll if it is no more necessary, to
        // restore the normal mousewheel behavior. This may happen with
        // AJAX callbacks or scripts that play with the DOM.
        else if (!needScroll && object.active) {
          self.deactivate(object);
        }

        if (scrollWidth != object.scrollWidth || scrollLeft != object.scrollLeft) {
          self.recalculateStickyHeader();
          object.scrollWidth = scrollWidth;
          object.scrollLeft = scrollLeft;
        }
      } //endfor

      self.loop();

    }, self.timeout);
  }
  else if (self.timeoutId != null) {
    clearTimeout(self.timeoutId);
    self.timeoutId = null;
  }
};

}(jQuery));
