/**
 * @file
 * Some basic behaviors and utility functions for Views.
 */
(function ($) {

  Drupal.ViewsElasticGrid = (function (wrapper_selector) {
    // list of items
    var $grid = $(wrapper_selector),
      // the items
      $items = $grid.children('li'),
      // current expanded item's index
      current = -1,
      // position (top) of the expanded item
      // used to know if the preview will expand in a different row
      previewPos = -1,
      // extra amount of pixels to scroll the window
      scrollExtra = 0,
      // extra margin when expanded (between preview overlay and the next items)
      marginExpanded = 10,
      $window = $(window), winsize,
      $body = $('html, body'),
      // transitionend events
      transEndEventNames = {
        'WebkitTransition': 'webkitTransitionEnd',
        'MozTransition': 'transitionend',
        'OTransition': 'oTransitionEnd',
        'msTransition': 'MSTransitionEnd',
        'transition': 'transitionend'
      },
      transEndEventName = transEndEventNames[ VEGModernizr.prefixed('transition') ],
      // support for csstransitions
      support = VEGModernizr.csstransitions,
      // default settings
      settings = {
        minHeight: 500,
        speed: 350,
        easing: 'ease'
      };

    function init(config) {

      // the settings..
      settings = $.extend(true, {}, settings, config);

      initIndexes();

      // get window´s size
      getWinSize();
      // initialize some events
      initEvents();

      initHeights();

      // If an initial fragment is in the URL, open that item
      // so that bookmarks, links work.
      openInitial();

    }

    function initIndexes() {
      var count = 0;
      $items.each(function () {
        var $item = $(this);
        $item.data({
          vegIndex: count
        });
        count++;
      });

    }

    function initHeights() {
      $items.each(function () {
        var $item = $(this);
        $item.data({
          offsetTop: $item.offset().top,
          height: $item.height()
        });
      });
    }

    function openInitial() {
      var hash = window.location.hash;
      if (hash.length) {
        $item = $(hash);
        showPreview($item);
      }
    }

    // saves the item´s offset top and height (if saveheight is true)
    function saveItemInfo(saveheight) {
      $items.each(function () {
        var $item = $(this);
        $item.data('offsetTop', $item.offset().top);
        if (saveheight) {
          $item.data('height', $item.find('.elastic-grid-thumbnail').height());
        }
      });
    }

    function initItemsEvents($items) {
      $items.find('span.veg-close').bind('click', function () {
        hidePreview();
        return false;
      });

      $items.find('a.veg-thumbnail-link').bind('click', function (e) {
        var $item = $(this).parent().parent();
        // check if item already opened
        if (current === $item.data('vegIndex')) {
          hidePreview()
        }
        else {
          showPreview($item);
        }
        // Set the URL fragment
        document.location.hash = $item.attr('id');
        return false;
      });
    }

    function initEvents() {

      // when clicking an item, show the preview with the item´s info and large image.
      // close the item if already expanded.
      // also close if clicking on the item´s cross
      initItemsEvents($items);

      // on window resize get the window´s size again
      // reset some values..
      $window.bind('debouncedresize', function () {

        scrollExtra = 0;
        previewPos = -1;
        // save item´s offset
        saveItemInfo();
        getWinSize();
        var preview = $.data(this, 'preview');
        if (typeof preview != 'undefined') {
          hidePreview();
        }

      });

    }

    function getWinSize() {
      winsize = { width: $window.width(), height: $window.height() };
    }

    function showPreview($item, skip_transition) {

      var preview = $.data(this, 'preview'),
      // item´s offset top
        position = $item.data('offsetTop');

      scrollExtra = 0;

      // if a preview exists and previewPos is different (different row) from item´s top then close it
      if (typeof preview != 'undefined') {

        // not in the same row
        if (previewPos !== position) {
          hidePreview(false);
          previewPos = position;

          // initialize new preview for the clicked item
          preview = $.data(this, 'preview', new VEGPreview($item));
          preview.open();
          return false;
        }
        // same row
        else {
          preview.close(true);
          // update previewPos
          previewPos = position;

          // initialize new preview for the clicked item
          preview = $.data(this, 'preview', new VEGPreview($item));

          // expand preview overlay
          preview.open(true);
          return false;
        }

      }

      // update previewPos
      previewPos = position;

      // initialize new preview for the clicked item
      preview = $.data(this, 'preview', new VEGPreview($item));

      // expand preview overlay
      preview.open(skip_transition);

    }

    function hidePreview(skip_transition) {
      current = -1;
      var preview = $.data(this, 'preview');
      if (preview) {
        preview.close(skip_transition);
        if (!skip_transition) {
          $.removeData(this, 'preview');
        }
      }
    }

    // the preview obj / overlay
    function VEGPreview($item) {
      this.$item = $item;
      this.expandedIdx = this.$item.data('vegIndex');
      var self = this;
      $.each($items, function ($i) {
        if ($($items[$i]).attr('id') == $item.attr('id')) {
          self.expandedIdx = $i;
        }
      });
      this.create();
      this.update();
    }

    VEGPreview.prototype = {
      create: function (skip_transition) {
        this.$previewEl = this.$item.find('div.elastic-grid-expanded');
        // set the transitions for the preview and the item
        if (support) {
          this.setTransition();
        }
      },
      update: function ($item) {

        if ($item) {
          this.$item = $item;
        }

        // if already expanded remove class "og-expanded" from current item and add it to new item
        if (current !== -1) {
          var $currentItem = $items.eq(current);
          $currentItem.removeClass('veg-expanded');
          this.$item.addClass('veg-expanded');
        }

        // update current value
        current = this.expandedIdx;

      },
      open: function (skip_transition) {
        if (skip_transition) {
          this.clearTransition();
          this.setHeights();
          setTimeout($.proxy(function () {
            // set the height for the preview and the item

            // scroll to position the preview in the right place
            this.setTransition();
          }, this), 25);
        }
        else {
          setTimeout($.proxy(function () {
            // set the height for the preview and the item
            this.setHeights();
            // scroll to position the preview in the right place
            this.positionPreview();
          }, this), 25);
        }

      },
      close: function (skip_transition) {

        var self = this,
          onEndFn = function () {
            if (support) {
              $(this).unbind(transEndEventName);
            }
            self.$item.removeClass('veg-expanded');
          };

        if (skip_transition) {
          self.clearTransition();
        }

        setTimeout($.proxy(function () {
          this.$previewEl.css('height', 0);

          // the current expanded item (might be different from this.$item)
          var $expandedItem = $items.eq(this.expandedIdx);
          $expandedItem.css('height', $expandedItem.data('height')).bind(transEndEventName, onEndFn);

          if (!support) {
            onEndFn.call();
          }

        }, this), 25);

        return false;

      },
      calcHeight: function () {

        var heightPreview = winsize.height - this.$item.data('height') - marginExpanded,
          itemHeight = winsize.height;

        if (heightPreview < settings.minHeight) {
          heightPreview = settings.minHeight;
          itemHeight = settings.minHeight + this.$item.data('height') + marginExpanded;
        }

        this.height = heightPreview;
        this.itemHeight = itemHeight;

      },
      setHeights: function () {

        var self = this,
          onEndFn = function () {
            if (support) {
              self.$item.unbind(transEndEventName);
            }
            self.$item.addClass('veg-expanded');
          };

        this.calcHeight();

        this.$item.css('height', this.itemHeight).bind(transEndEventName, onEndFn);
        this.$previewEl.css('height', this.height);

        if (!support) {
          onEndFn.call();
        }

      },
      positionPreview: function () {

        // scroll page
        // case 1 : preview height + item height fits in window´s height
        // case 2 : preview height + item height does not fit in window´s height and preview height is smaller than window´s height
        // case 3 : preview height + item height does not fit in window´s height and preview height is bigger than window´s height
        var position = this.$item.data('offsetTop'), //this.$item.find('.elastic-grid-thumbnail').offset().top,
          previewOffsetT = this.$previewEl.offset().top - scrollExtra,
          scrollVal = this.height + this.$item.data('height') + marginExpanded <= winsize.height ? position : this.height < winsize.height ? previewOffsetT - ( winsize.height - this.height ) : previewOffsetT;
        $body.scrollTop(scrollVal);

      },
      setTransition: function () {
        this.$previewEl.css('transition', 'height ' + settings.speed + 'ms ' + settings.easing);
        this.$item.css('transition', 'height ' + settings.speed + 'ms ' + settings.easing);
      },
      clearTransition: function () {
        this.$previewEl.css('transition', '');
        this.$item.css('transition', '');
      },
      getEl: function () {
        return this.$previewEl;
      }
    }


    return {
      init: init
    }


  });

  /**
   * Launch the elastic grid code.
   */
  Drupal.behaviors.viewsElasticGridExpander = {
    attach: function (context) {
      $.each(Drupal.settings.views_elastic_grid, function (dom_id, selector) {
        var veg = Drupal.ViewsElasticGrid(selector);
        veg.init();
      });
    }
  };

})(jQuery);