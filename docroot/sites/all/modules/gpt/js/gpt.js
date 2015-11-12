(function () {
  /**
   * Define GPT Ad class constructor.
   */
  var GPTAd = function (ad, responsive) {
    // To not lose scope in responsive handler we need to use self.
    var self = this;

    // Set properties.
    self.breakpoint = null;
    self.domEl = document.getElementById(ad.domId);
    self.networkCode = ad.networkCode || ad.network_code;
    self.outOfPage = ad.outOfPage || ad.outofpage || false;
    self.refresh = ad.refresh || false;
    self.resizeListener = null;
    self.size = ad.size;
    self.slot = {};
    self.slotDomEl = {};
    self.targetedAdUnit = ad.targetedAdUnit || ad.targeted_ad_unit;
    self.targeting = ad.targeting || null;

    // Add responsive listener if the ad has breakpoints and is responsive.
    if (responsive) {
      var count = 0;
      for (var i in self.size) {
        if (++count > 1) {
          // Create named anonymous function to preserve scope in listener.
          var resizeListener = function () { self.responsiveHandler() };
          if (window.addEventListener) {
            window.addEventListener('resize', resizeListener, false);
          }
          else if (window.attachEvent) {
            window.attachEvent('onresize', resizeListener);
          }
          break;
        }
      }
    }

    // Determine the initial breakpoint to use and create the dom element.
    self.setBreakpoint().getSlotDomEl();
  };

  /**
   * Define slot at breakpoint.
   */
  GPTAd.prototype.defineSlot = function (breakpoint) {
    breakpoint = this.getBreakpoint(breakpoint);
    this.slot[breakpoint] = Drupal.GPT.defineSlot({
        domId: this.getSlotDomId(breakpoint)
      , outOfPage: this.outOfPage
      , size: this.getSize(breakpoint)
      , targeting: this.targeting
      , unitName: this.getUnitName()
    });
  };

  /**
   * Helper to retrieve current breakpoint or sanitize passed breakpoint.
   */
  GPTAd.prototype.getBreakpoint = function (breakpoint) {
    if (typeof breakpoint === 'undefined') {
      breakpoint = this.breakpoint;
    }
    else {
      this.validBreakpoint(breakpoint);
    }
    return breakpoint;
  }

  /**
   * Retrieve ad slot DOM element at breakpoint, creates if needed.
   */
  GPTAd.prototype.getSlotDomEl = function (breakpoint) {
    var self = this;
    breakpoint = this.getBreakpoint(breakpoint);
    // If the dom element for this breakpoint does not exist.
    if (!(breakpoint in this.slotDomEl)) {
      var domId = self.getSlotDomId(breakpoint)
        , el = null;

      // Create the div for the slot if it doesn't exist.
      // TODO: May need document.write here for sync, doesn't seem to be needed.
      el = document.createElement('div');
      el.id = domId;
      self.domEl.appendChild(el);

      // Store the element.
      self.slotDomEl[breakpoint] = el;

      // Create the ad if there is a size for it.
      if (self.size[breakpoint] !== null || self.outOfPage) {
        Drupal.GPT.fetchSlot(self.getSlotDomId(breakpoint), self.defineSlot, self);
      }
    }

    return self.slotDomEl[breakpoint];
  };

  /**
   * Concatenate domId with specified breakpoint to get element ID.
   */
  GPTAd.prototype.getSlotDomId = function (breakpoint) {
    breakpoint = this.getBreakpoint(breakpoint);
    return this.domEl.id + '-' + breakpoint;
  };

  /**
   * Fetch the ad size.
   */
  GPTAd.prototype.getSize = function (breakpoint) {
    breakpoint = this.getBreakpoint(breakpoint);
    return this.size[breakpoint];
  };

  /**
   * Retrieve the unit name.
   */
  GPTAd.prototype.getUnitName = function () {
    return '/' + this.networkCode + '/' + this.targetedAdUnit;
  };

  /**
   * Determine if the ad unit has a slot at the breakpoint.
   */
  GPTAd.prototype.hasSlot = function (breakpoint) {
    breakpoint = this.getBreakpoint(breakpoint);
    return typeof this.slot[breakpoint] !== 'undefined';
  }

  /**
   * Retrieve the current slot that should be refreshed.
   */
  GPTAd.prototype.refreshSlot = function () {
    // Validate the ad can refresh and if its current slot has a size.
    if (this.refresh && this.getSize(this.breakpoint)) {
      return this.slot[this.breakpoint];
    }
  };

  /**
   * Resize ad for responsive support.
   */
  GPTAd.prototype.responsiveHandler = function () {
    var breakpoint = null;

    // Determine breakpoint for current size.
    for (var i in this.size) {
      if (i <= document.documentElement.clientWidth) {
        breakpoint = i;
      }
      else {
        break;
      }
    }

    // If the ad breakpoint needs to change.
    if (breakpoint !== this.breakpoint) {
      // Update the breakpoint, so this won't trigger again.
      this.breakpoint = breakpoint;
      // Hide all ads that are not being displayed.
      var current = this.getSlotDomEl();
      for (var j in this.slotDomEl) {
        if (this.slotDomEl[j] !== current) {
          this.slotDomEl[j].style.display = 'none';
        }
      }
      // Display the current ad.
      current.style.display = '';
    }
  };

  /**
   * Determine the current breakpoint.
   */
  GPTAd.prototype.setBreakpoint = function (breakpoint) {
    // If a breakpoint was specified, attempt to use it.
    if (typeof breakpoint !== 'undefined') {
      this.validBreakpoint(breakpoint).breakpoint = breakpoint;
    }
    // Otherwise determine the current breakpoint.
    else {
      for (var i in this.size) {
        if (i <= document.documentElement.clientWidth) {
          this.breakpoint = i;
        }
        else {
          break;
        }
      }
    }
    return this;
  };

  /**
   * Validate the breakpoint specified exists.
   */
  GPTAd.prototype.validBreakpoint = function (breakpoint) {
    if (breakpoint in this.size) {
      return this;
    }
    throw 'Invalid breakpoint specified.';
  };

  // Expose GPTAd to global scope through the Drupal object.
  Drupal.GPTAdClass = GPTAd;

  /**
   * Define GPT class constructor.
   */
  var GPT = function (options) {
    var self = this;
    options = options || {};
    self.pageOptions = {
        // Asynchronous mode.
        async: typeof options.async !== 'undefined' ? options.async : 1
        // Collapse div elements when there is no creative.
      , collapse: options.collapse || 0
        // Default network code for ad slots on the page.
      , networkCode: options.networkCode || options.network_code
        // Refreshing ads allowed.
      , refresh: typeof options.refresh !== 'undefined' ? options.async : 1
        // Whether responsive ads can be supported, condition below.
      , responsive: false
        // Single request architecture.
      , sra: typeof options.sra !== 'undefined' ? options.sra : 1
        // Page level targeting settings.
      , targeting: options.targeting || {}
        // Default targeted ad unit for ad slots on the page.
      , targetedAdUnit: options.targetedAdUnit || options.targeted_ad_unit
    };
    self.ads = {};
    self.initialized = false;

    // This is a bit wonky, but it's private.
    // Different init if async without SRA, if we try to use the async init
    // with SRA on some page loads googletag has not loaded from the external
    // JS before jQuery's .ready() occurs and .load() is too slow.
    if (self.pageOptions.async && !self.pageOptions.sra) {
      (function () {
        window.googletag = window.googletag || {};
        googletag.cmd = googletag.cmd || [];
        var gads = document.createElement("script");
        gads.async = self.pageOptions.async;
        gads.type = "text/javascript";
        var useSSL = "https:" == document.location.protocol;
        gads.src = (useSSL ? "https:" : "http:") + "//www.googletagservices.com/tag/js/gpt.js";
        var node = document.getElementsByTagName("script")[0];
        node.parentNode.insertBefore(gads, node);
      })();
      // Responsive ads can only be supported if async is enabled and SRA is
      // disabled.
      self.pageOptions.responsive = true;
    }
    else {
      (function () {
        var useSSL = 'https:' == document.location.protocol;
        var src = (useSSL ? 'https:' : 'http:') +
        '//www.googletagservices.com/tag/js/gpt.js';
        document.write('<scr' + 'ipt src="' + src + '"></scr' + 'ipt>');
      })();
    }
  };

  /**
   * Define a GPT slot object.
   *
   * In most cases you should not use this method directly but instead use
   * registerSlot().
   */
  GPT.prototype.defineSlot = function (o) {
    var self = Drupal.GPT
      , slot = null;

    if (o.size === null && !o.outOfPage) {
      return;
    }

    if (o.outOfPage) {
      slot = googletag.defineOutOfPageSlot(o.unitName, o.domId);
    }
    else {
      slot = googletag.defineSlot(o.unitName, o.size, o.domId);
    }

    if (slot) {
      slot.addService(googletag.pubads());
      // Apply targeting, if available.
      if ('targeting' in o) {
        self.setTargeting(slot, o.targeting);
      }
    }

    return slot;
  }

  /**
   * Retrieve ad slot DOM element, creates ad slot if non-existant.
   */
  GPT.prototype.fetchSlot = function (domId, callback, cbScope, cbArgs) {
    var self = this;

    // Create and load the ad.
    if (self.pageOptions.async) {
      // If GPT has already loaded ads on the site, render inline.
      if (self.initialized) {
        googletag.cmd.push(function () {
          callback.apply(cbScope, cbArgs);
          googletag.display(domId);
        });
      }
      // If GPT has not initialized then the ad will be displayed during
      // init.
    }
    else {
      // Define and display slot immediately for render, done inline.
      callback.apply(cbScope, cbArgs);
      googletag.display(domId);
    }
  }

  /**
   * Refresh all refreshable ad units on the page.
   */
  GPT.prototype.refresh = function () {
    // If not yet initialized then don't try to refresh.
    if (!this.initialized) {
      return;
    }

    // Only attempt refreshing if async and page enabled.
    if (!this.pageOptions.async || !this.pageOptions.refresh) {
      return;
    }

    var slots = [];
    for (var i in this.ads) {
      var slot = this.ads[i].refreshSlot();
      if (slot) {
        slots.push(slot);
      }
    }
    if (slots.length) {
      googletag.pubads().refresh(slots);
    }
  }

  /**
   * Register an element as an ad slot.
   */
  GPT.prototype.registerSlot = function (identifier, ad) {
    var self = Drupal.GPT;

    if (typeof identifier !== 'string') {
      throw 'registerSlot(): Invalid identifier provided.';
    }

    if (typeof ad !== 'object') {
      throw 'registerSlot(): Invalid arg provided.';
    }

    // Provide defaults from GPT page state.
    ad.networkCode = ad.networkCode || self.pageOptions.networkCode;
    ad.targetedAdUnit = ad.targetedAdUnit || self.pageOptions.targetedAdUnit;
    ad.domId = identifier;

    self.ads[identifier] = new Drupal.GPTAdClass(ad, self.pageOptions.responsive);
  };

  /**
   * Initialize GPT by enabling services.
   */
  GPT.prototype.run = function () {
    // When ran as a callback of googletag.cmd we lose scope...
    // this should be handled more elloquently instead of hard coding.
    var self = Drupal.GPT;

    // Only run once.
    if (self.initialized) {
      return;
    }

    // Define each slot registered. Note, when synchronous this will be empty.
    for (var i in self.ads) {
      self.ads[i].defineSlot();
    }

    // Set page level targeting.
    self.setTargeting(googletag.pubads(), self.pageOptions.targeting);

    if (self.pageOptions.async) {
      // Set collapse div elements when no creative.
      if (self.pageOptions.collapse) {
        googletag.pubads().collapseEmptyDivs();
      }

      // Set single request.
      if (self.pageOptions.sra) {
        googletag.pubads().enableSingleRequest();
      }
    }
    else {
      googletag.pubads().enableSyncRendering();
    }

    // Enable services.
    googletag.enableServices();

    // Render ads that are already registered, if async.
    if (self.pageOptions.async) {
      for (var j in self.ads) {
        if (self.ads[j].hasSlot()) {
          googletag.display(self.ads[j].getSlotDomId());
        }
      }
    }

    // We are now initialized. Note that this anonymous function is called
    // after the ads in the body register at runtime, which is why we need an
    // initialized property.
    self.initialized = true;
  };

  /**
   * Apply targeting object to a slot object or the pubads object.
   */
  GPT.prototype.setTargeting = function (obj, targeting) {
    for (var i in targeting) {
      var t = [];
      // Eval values if they need to be.
      for (var j = 0; j < targeting[i].length; j++) {
        if (targeting[i][j].eval) {
          // Wrap eval in a try incase of error.
          try {
            t.push(eval(targeting[i][j].value));
          }
          catch (e) {}
        }
        else {
          t.push(targeting[i][j].value);
        }
      }

      // Set either the only item, or the whole array.
      if (t.length === 1) {
        obj.setTargeting(i, t[0]);
      }
      else {
        obj.setTargeting(i, t);
      }
    }
  };

  // Expose GPT to global scope through the Drupal object.
  Drupal.GPTClass = GPT;
})();

/**
 * Refresh GPT ads.
 */
Drupal.behaviors.gpt = function (context) {
  Drupal.settings.gpt = Drupal.settings.gpt || {};

  if (Drupal.settings.gpt.notFirstRun) {
    // It's possible Drupal.GPT is not defined if critical values were missing.
    if (typeof Drupal.GPT !== 'undefined') {
      Drupal.GPT.refresh();
    }
  }
  else {
    Drupal.settings.gpt.notFirstRun = true;
  }
}

