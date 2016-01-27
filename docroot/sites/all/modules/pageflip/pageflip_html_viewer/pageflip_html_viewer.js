
(function ($) {
  Drupal.pageFlip = Drupal.pageFlip || {};

  Drupal.pageFlip.whichChapter = function (page) {
    var chapter = 0;
    while (page > 0) {
      var numPages = Drupal.settings.pageFlip.chapters[chapter].numPages;
      // if we're on page n and the next chapter starts on n+1, we should consider
      // ourselves on the next chapter (n is always even, and we can see two pages
      // at once). thus the subtraction.
      if (page < (numPages - 1)) {
        break;
      }
      page -= numPages;
      chapter++;
    }
    return chapter;
  }

  /**
   * @param chapter
   *   chapter (0-based index)
   * @return
   *   page (0-based index) of the page that starts the given chapter's
   *   first page-pair.
   */
  Drupal.pageFlip.chapterStartPage = function (chapter) {
    var page = 0;
    for (var i = 0; i < chapter; i++) {
      page += Drupal.settings.pageFlip.chapters[i].numPages;
    }

    return page - (page % 2);
  };

  /* Document-complete tasks */
  $(function() {
    /**
     * Hash change handler. Called whenever the browser's location bar URL
     * fragment changes.
     */
    $(window).bind('hashchange', function() {
      var hash = location.hash;
      var pattern = /^#chapter([\d]+)$/; /* chapter # (start at its first page) */
      var pattern2 = /^#\/([\d]+)$/; /* page # from start of entire book */
      var page, chapter;

      if (pattern.test(hash)) {
        chapter = parseInt(hash.match(pattern)[1]);
        page = Drupal.pageFlip.chapterStartPage(chapter);
      }
      else if (pattern2.test(hash)) {
        page = parseInt(hash.match(pattern2)[1]);
        chapter = Drupal.pageFlip.whichChapter(page);
      }
      else if (hash == '') {
        // Treat blank hash as OK so that we still call pageChange callbacks.
        page = 0;
        chapter = 0;
      }
      else {
        // invalid fragment
        return;
      }

      if (page < 0 || page >= Drupal.settings.pageFlip.totalPages) {
        // todo: reset fragment to something valid?
        return;
      }

      Drupal.settings.pageFlip.curPage = page;
      $('select#edit-chapter').val(chapter);

      $('.pageflip-html-viewer-content-container').each(function() {
        $(this).cycle(parseInt(page / 2));
      });

      // Call any registered pageChange callbacks.
      $.each(Drupal.settings.pageFlip.callbacks['pageChange'], function (index, fn) {
        fn(Drupal.settings.pageFlip.curPage, Drupal.settings.pageFlip.totalPages);
      });
    });

    $(window).trigger('hashchange');

    // display a help message for mobile devices
    if (Drupal.settings.pageFlip.isMobile) {
      if (Drupal.settings.pageFlip.mobileHelpMessage.length > 0) {
        alert(Drupal.settings.pageFlip.mobileHelpMessage);
      }
    }
  });

  /**
   * Registers a PageFlip callback.
   * Valid callback:
   * - pageChange
   */
  Drupal.pageFlip.bind = function(callback, fn) {
    if ($.isFunction(fn)) {
      Drupal.settings.pageFlip.callbacks[callback].push(fn);
    }
  }

  /**
   * Our Drupal 'behavior' -- a function to bind event handlers to elements that
   * we haven't already processed. This allows Drupal to reload page elements
   * using AHAH/Ajax and "retain" bound functionality (by rerunning behaviors).
   */
  Drupal.behaviors.pageFlip = {
    attach: function(context) {
      // Set up the book container so we can use jquery.cycle to control page-pair visibility
      $('.pageflip-html-viewer-content-container:not(.pageflip-html-viewer-processed)', context).each(function() {
        $(this).cycle({
          fx: 'fade',
          speed: 750,
          nowrap: 1, // don't wrap-around
          timeout: 0
        });
      }).addClass('pageflip-html-viewer-processed');

      // Set up "previous page" links to cause pages to flip.
      $('a.pageflip-navigation-prev:not(.pageflip-html-viewer-processed)', context).each(function() {
        $(this).click(function(event) {
          event.preventDefault(); // ignore the a tag's actual link, which is a dummy
          if (Drupal.settings.pageFlip.curPage >= 2) {
            // Cause the page to flip by indirectly invoking our hashchange handler
            location.hash = '#/' + (Drupal.settings.pageFlip.curPage - 2);
          }
        });
      }).addClass('pageflip-html-viewer-processed');

      // Set up "next page" links, too.
      $('a.pageflip-navigation-next:not(.pageflip-html-viewer-processed)', context).each(function() {
        $(this).click(function(event) {
          event.preventDefault(); // ignore the a tag's actual link, which is a dummy
          if (Drupal.settings.pageFlip.curPage + 2 < Drupal.settings.pageFlip.totalPages) {
            location.hash = '#/' + (Drupal.settings.pageFlip.curPage + 2);
          }
        });
      }).addClass('pageflip-html-viewer-processed');

      // Change the chapter when the user selects a different one using the chapter select dropdown
      $('select#edit-chapter').change(function() {
        var newHash = '#/' + Drupal.pageFlip.chapterStartPage(parseInt($(this).val()));
        location.hash = newHash;
      });

    }
  };

})(jQuery);
