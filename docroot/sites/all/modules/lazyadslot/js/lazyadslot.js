/**
 * @file
 * Add the Ad Slots in a lazy load manner.
 */
var lazyLoadAdSlot = lazyLoadAdSlot || {};

(function ($) {

  'use strict';

  var windowHeight = window.innerHeight;
  var initialized = false;

  function _throttle(fn, threshold, scope) {
    threshold || (threshold = 250);
    var last,
      deferTimer;
    return function () {
      var context = scope || this;

      var now = +new Date,
        args = arguments;
      if (last && now < last + threshold) {
        // Hold on to it.
        clearTimeout(deferTimer);
        deferTimer = setTimeout(function () {
          last = now;
          fn.apply(context, args);
        }, threshold);
      }
      else {
        last = now;
        fn.apply(context, args);
      }
    };
  }

  var lazyLoadAd = {

    adSlot: {},
    adSlotCounter: {},
    adSlotsStore: [],
    top: 1,
    slotId: '',
    attr: {},

    getTop: function (tag) {
      var initialTop = parseInt(tag.top);
      return isNaN(initialTop) ? this.top : initialTop;
    },
    pushAd: function ($el, html) {
      var vector = (this.adSlot.attach_how === 'before') ? 'Before' : 'After';
      $(html)['insert' + vector]($el);
    },
    /**
     * Increase the counter per specified slot name.
     */
    increaseSlotCounter: function (slotName) {
      if (isNaN(this.adSlotCounter[slotName])) {
        this.adSlotCounter[slotName] = 0;
      }
      else {
        this.adSlotCounter[slotName]++;
      }
      return this.adSlotCounter[slotName];
    },
    /**
     * Add default attributes and merge with passed ones.
     */
    getAttr: function (newId) {
      var defaultClasses = 'lazyadslot lazyadslot-' + newId;

      // Add default classes.
      if (this.attr.class) {
        this.attr.class = this.attr.class + ' ' + defaultClasses;
      }
      else {
        this.attr.class = defaultClasses;
      }
      return this.attr;
    },
    detectSlot: function (force) {
      var uniqueKey, offset, slotElement;
      var windowTop = document.body.scrollTop || document.documentElement.scrollTop;

      for (var i = 0; i < this.adSlotsStore.length; i++) {
        var ad = this.adSlotsStore[i].tag,
          adPlacement = this.adSlotsStore[i].$el.selector;

        uniqueKey = ad.ad_tag + '_' + adPlacement;
        slotElement = this.adSlotsStore[i];

        if (!this.added[uniqueKey] && slotElement && slotElement.$el) {
          if (force === true || ad.onscroll === 1) {
            offset = (windowTop + windowHeight);
            if (force === true || offset > slotElement.offset) {
              this.added[uniqueKey] = true;
              this.addSlot(ad, slotElement.$el);
            }
          }
        }
      }
    },
    isSlotStored: function (selector) {
      var added = false;
      if (this.adSlotsStore.length > 0) {
        $.each(this.adSlotsStore, function (index, value) {
          if (value.$el.selector === selector) {
            added = true;
          }
        });
      }
      return added;
    },
    addSlotToStore: function () {
      for (var slotKey = 0; slotKey < this.adSlot.length; slotKey++) {
        var ad = this.adSlot[slotKey];
        for (var i = 0; i < ad.ad_placement.length; i++) {
          var selector = ad.ad_placement[i];
          if (selector && selector.length > 0) {
            var $el = $(selector);
            if ($el.length === 1 && !this.isSlotStored($el.selector)) {
              this.adSlotsStore.push({
                $el: $el,
                offset: parseInt($el.offset().top, 10) + $el.height() - this.top,
                tag: ad,
              });
            }
          }
        }
      }
    },
    /**
     * Generate new slot ID, and push the Ad into the page.
     */
    addSlot: function (adSlot, $el) {
      // Generate new slot definition/display with incremental id as unique.
      var currentIDregex = new RegExp(adSlot.ad_tag, 'g'),
        newID = adSlot.ad_tag + '_' + this.increaseSlotCounter(adSlot.ad_tag),
        adSlotRendered = adSlot.renderedDfp.replace(currentIDregex, newID);

      // Wrap the rendered slot.
      adSlotRendered = $('<div/>', this.getAttr(newID)).append(adSlotRendered);

      // Append the Slot declaration/display.
      this.pushAd($el, adSlotRendered);

      // Refresh the tag.
      if (parseInt(adSlot.refreshOnLoad)) {
        googletag.pubads().refresh([googletag.slots[newID]]);
      }

      this.slotId = newID;
      // Always destroy generated attributes.
      this.attr = {};
    },
    createTag: function (tag, attr) {

      this.top = this.getTop(tag);
      this.adSlot.push(tag);

      if (!$.isEmptyObject(attr)) {
        this.attr = attr;
      }

      // Instantly Ad load.
      this.addSlotToStore();
      // If it's not setup to load on scroll,
      // force it though conditions in order to be added instantly.
      this.detectSlot(!tag.onscroll);

      return this;
    },
    // Append the Ad to the page.
    execute: function () {
      var self = this;
      this.added = {};
      this.adSlot = [];

      window.addEventListener('scroll', _throttle(function () {
        self.detectSlot();
      }, 100));

      window.onresize = _throttle(function (event) {
        windowHeight = window.innerHeight;
        // Reset adSlotsStore as not to keep adding the same slots.
        self.addSlotToStore();
      }, 100);
    },
  };

  // Initialization function.
  lazyLoadAdSlot.init = function () {
    if (initialized === false) {
      lazyLoadAd.execute();
      initialized = true;
    }
    return lazyLoadAd;
  };

})(jQuery);
