(function($) {
  "use strict";

/**
 * @class Drupal.availabilityCalendar.Viewport
 *
 * @constructor Creates a new Drupal.availabilityCalendar.Viewport object.
 * @param {Object} settings
 *   object with the following properties (name type (default) description):
 *   {
 *     cvid {number} Required: the selector of the calendar view to operate on.
 *     dimensionsCalculation string ('fixed') IF and how to calculate the
 *       dimensions of the viewport. One of:
 *       - none
 *       - fixed
 *       - responsive_block
 *       - responsive_inline
 *     month
 *       width     int (0) The width of 1 calendar month. Used to calculate the
 *                         width of the viewport. If 0, the actual width of a
 *                         calendar month is taken.
 *       height    int (0) The height of 1 calendar month. See width for more
 *                         info.
 *     cols        int (0) The number of months that is shown horizontally in
 *                         the viewport. If 0, this is calculated dynamically,
 *                         based on the available width.
 *     rows        int (1) The number of months that is shown vertically in the
 *                         viewport.
 *     scroll      int (0) Indicating how many rows or cols to scroll.
 *     animate     Animation parameters, @see http://api.jquery.com/animate/
 *       backward  Internal/advanced usage.
 *       forward   Internal/advanced usage.
 *       speed     int|string ('slow') The animation speed, see jquery
 *                         documentation of animate().
 *     totalMonths int (calculated) Internal usage, do not pass in.
 *     firstMonth  int (calculated) Internal usage, do not pass in.
 *     width  int (calculated) Internal usage, do not pass in. Contains the
 *            width of 1 calendar as will be used in calculations.
 *     height  int (calculated) Internal usage, do not pass in. Contains the
 *            height of 1 calendar as will be used in calculations.
 *   }
 */
Drupal.availabilityCalendar.Viewport = function(settings) {
  var that = this;
  /** @type {jQuery} */
  var view;
  /** @type {jQuery} */
  var viewport;
  /** @type {jQuery} */
  var viewportField;
  /** @type {Object} */
  var viewportSettings;

  /**
   * Initializes the viewport.
   *
   * But beware of reinitializing after ajax events triggered a new round of
   * attachBehaviors.
   */
  this.init = function(settings) {
    view = Drupal.availabilityCalendar.getCalendarView(settings.cvid).getView();
    viewport = $(".cal-viewport-inner", view);
    viewportField = viewport.parent().parent();
    viewport.once('Drupal-availabilityCalendar-Viewport-init', function() {
      initSettings(settings);
      calculateDimensions();
      initAnimation();
      initHandlers();
      $(window).resize(calculateDimensions);
    });
  };

  /**
   * Extends the settings with their defaults and adds the animations.
   */
  function initSettings(settings) {
    viewportSettings = $.extend(true, {
      dimensionsCalculation: 'fixed',
      responsive: false,
      month: {width: 0, height: 0},
      cols: 0,
      rows: 1,
      scroll: 1,
      animate: {speed: "slow"},
      totalMonths: Drupal.availabilityCalendar.getCalendarView(settings.cvid).getNumberOfMonths(),
      firstMonth: 1
    }, settings);

    // Get outer width of a month wrapper. These are needed to define the
    // animations.
    // If the calendar is initially hidden, e.g. in a collapsed fieldset or a
    // tab, the width and height cannot be easily retrieved. Therefore, a nice
    // small jQuery plugin - actual - is used to get the actual outerWidth and
    // outerHeight regardless current visibility. To save an additional HTTP
    // request, this plugin is inlined at the end of this file.
    if (viewportSettings.month.width === 0) {
      viewportSettings.month.width = $(".cal-month", view).first().actual('outerWidth', {includeMargin: true});
    }
    if (viewportSettings.month.height === 0) {
      viewportSettings.month.height = $(".cal-month", view).first().actual('outerHeight', {includeMargin: true});
    }
    viewportSettings.animate.custom = viewportSettings.animate.forward !== undefined;
  }

  /**
   * Sets the dimensions of the viewport.
   */
  function calculateDimensions() {
    if (viewportSettings.dimensionsCalculation !== 'none') {
      // Remove any width set in previous calls to this function, so
      // availableWidth can be calculated correctly;
      viewport.parent().removeAttr('style');
      viewport.removeAttr('style');
      // Calculate the number of cols, given the available width.
      var availableWidth = viewport.parent().actual('width');
      if (viewportSettings.dimensionsCalculation === 'responsive_inline') {
        viewportField.children('.cal-buttons').each(function() {
          availableWidth -= $(this).actual('outerWidth', {includeMargin: true});
        });
      }
      viewportSettings.cols = Math.max(1, Math.floor(availableWidth / viewportSettings.month.width));
      if (viewportSettings.rows === 1) {
        // Do not scroll more than the number of visible months.
        viewportSettings.scroll = Math.min(viewportSettings.cols, viewportSettings.scroll);
      }

      viewport.parent().width(viewportSettings.cols * viewportSettings.month.width);
      viewport.parent().height(viewportSettings.rows * viewportSettings.month.height);

      // Check the scroll value w.r.t. the actual viewport size.
      if (viewportSettings.rows === 1) {
        // If scrolling horizontally, the inner viewport should be infinitely wide.
        viewport.width(10000);
        // Do not scroll more than the number of cols.
        if (viewportSettings.scroll > viewportSettings.cols) {
          viewportSettings.scroll  = viewportSettings.cols;
        }
      }
      else {
        // Do not scroll more than the number of rows.
        if (viewportSettings.scroll > viewportSettings.rows) {
          viewportSettings.scroll  = viewportSettings.rows;
        }
      }

      if (viewportSettings.dimensionsCalculation === 'fixed') {
        // Only set the dimensions once for the fixed mode.
        viewportSettings.dimensionsCalculation = 'none';
      }
    }
  }

  function initAnimation() {
    if (!viewportSettings.animate.custom) {
      var isLtr = $('html').attr('dir') !== 'rtl';
      viewportSettings.animate.backward = viewportSettings.rows > 1 ? { top: "+=" + (viewportSettings.scroll * viewportSettings.month.height) }
        : isLtr ? { left:  "+=" + (viewportSettings.scroll * viewportSettings.month.width) }
        : { right: "+=" + (viewportSettings.scroll * viewportSettings.month.width) };
      viewportSettings.animate.forward = viewportSettings.rows > 1 ? { top: "-=" + (viewportSettings.scroll * viewportSettings.month.height) }
        : isLtr ? { left: "-=" + (viewportSettings.scroll * viewportSettings.month.width) }
        : { right: "-=" + (viewportSettings.scroll * viewportSettings.month.width) };
    }
  }

  /**
   * Initialize event handlers for our own buttons.
   */
  function initHandlers() {
    viewport.once('Drupal-availabilityCalendar-Viewport-initHandlers', function () {
      $(".cal-backward", view).click(function() { that.scrollBackward(); });
      $(".cal-forward", view).click(function() { that.scrollForward(); });
    });
    setEnabledState();
  }

  /**
   * Set the enabled/disabled state of the clickable elements.
   */
  function setEnabledState() {
    if (viewportSettings.firstMonth <= 1) {
      $(".cal-backward", view).attr("disabled", "disabled");
    }
    else {
      $(".cal-backward", view).removeAttr("disabled");
    }
    if (viewportSettings.firstMonth + viewportSettings.rows * viewportSettings.cols > viewportSettings.totalMonths) {
      $(".cal-forward", view).attr("disabled", "disabled");
    }
    else {
      $(".cal-forward", view).removeAttr("disabled");
    }
  }

  /**
   * Scroll the viewport backward (if possible).
   */
  this.scrollBackward = function() {
    if (viewportSettings.firstMonth > 1) {
      viewport.animate(viewportSettings.animate.backward, viewportSettings.animate.speed);
      viewportSettings.firstMonth -= viewportSettings.rows > 1 ? viewportSettings.scroll * viewportSettings.cols : viewportSettings.scroll;
      setEnabledState();
    }
  };

  /**
   * Scroll the viewport forward (if possible).
   */
  this.scrollForward = function() {
    if (viewportSettings.firstMonth + viewportSettings.rows * viewportSettings.cols <= viewportSettings.totalMonths) {
      viewport.animate(viewportSettings.animate.forward, viewportSettings.animate.speed);
      viewportSettings.firstMonth += viewportSettings.rows > 1 ? viewportSettings.scroll * viewportSettings.cols : viewportSettings.scroll;
      setEnabledState();
    }
  };

  this.init(settings);
};

/**
 * @type {Object} Collection of viewport instances.
 */
Drupal.availabilityCalendar.viewports = {};

/**
 * Multiton implementation for accessing viewports on the page.
 *
 * @param {number} cvid
 * @returns {?Drupal.availabilityCalendar.Viewport}
 */
Drupal.availabilityCalendar.getViewport = function(cvid) {
  return Drupal.availabilityCalendar.viewports[cvid] !== undefined ? Drupal.availabilityCalendar.viewports[cvid] : null;
};

/**
 * Initialization code that works by implementing the Drupal behaviors paradigm.
 *
 * Based on the information in the settings, the calendar viewports are created.
 */
Drupal.behaviors.availabilityCalendarViewport = {
  attach: function(context, settings) {
    if (settings.availabilityCalendar && settings.availabilityCalendar.viewports) {
      var key;
      for (key in settings.availabilityCalendar.viewports) {
        if (settings.availabilityCalendar.viewports.hasOwnProperty(key)) {
          var viewportSettings = settings.availabilityCalendar.viewports[key];
          if (Drupal.availabilityCalendar.viewports[viewportSettings.cvid] === undefined) {
            Drupal.availabilityCalendar.viewports[viewportSettings.cvid] = new Drupal.availabilityCalendar.Viewport(viewportSettings);
          }
          else {
            Drupal.availabilityCalendar.viewports[viewportSettings.cvid].init(viewportSettings);
          }
        }
      }
    }
  }
};

}(jQuery));

/*
 * Inlined version of jQuery.actual plugin.
 * See http://dreamerslab.com/blog/en/get-hidden-elements-width-and-height-with-jquery/
 *
 * The following copyright and license message only holds for the code below.
 */
/*! Copyright 2012, Ben Lin (http://dreamerslab.com/)
 * Licensed under the MIT License (LICENSE.txt).
 *
 * Version: 1.0.14
 *
 * Requires: jQuery 1.2.3 ~ 1.9.0
 */
;(function(e){e.fn.extend({actual:function(t,n){if(!this[t]){throw'$.actual => The jQuery method "'+t+'" you called does not exist'}var r={absolute:false,clone:false,includeMargin:false};var i=e.extend(r,n);var s=this.eq(0);var o,u;if(i.clone===true){o=function(){var e="position: absolute !important; top: -1000 !important; ";s=s.clone().attr("style",e).appendTo("body")};u=function(){s.remove()}}else{var a=[];var f="";var l;o=function(){if(e.fn.jquery>="1.8.0")l=s.parents().addBack().filter(":hidden");else l=s.parents().andSelf().filter(":hidden");f+="visibility: hidden !important; display: block !important; ";if(i.absolute===true)f+="position: absolute !important; ";l.each(function(){var t=e(this);a.push(t.attr("style"));t.attr("style",f)})};u=function(){l.each(function(t){var n=e(this);var r=a[t];if(r===undefined){n.removeAttr("style")}else{n.attr("style",r)}})}}o();var c=/(outer)/g.test(t)?s[t](i.includeMargin):s[t]();u();return c}})})(jQuery)
