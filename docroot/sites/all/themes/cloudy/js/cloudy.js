(function ($) {

var query = 'all and (max-width: 800px)';

Drupal.behaviors.tiptip = {
  attach: function (context, settings) {
    $('.flag[title]').tipTip({defaultPosition: 'right'});
  }
}

/**
 * Reveals the search agent on click.
 */
Drupal.behaviors.agentTrigger = {
  attach: function (context, settings) {
    $('.agent-box', context).once('agent-trigger', function () {
      $('.agent-target').hide();
      $('.agent-link a').click(function () {
        $('.agent-target').slideToggle();
        return false;
      }).appendTo(this);
    });
  }
}

/**
 * Turns all tables into responsive, scrollable tables on mobile devices.
 */
Drupal.behaviors.responsiveTables = {
  attach: function (context, settings) {
    // Do not attach the responsive tables to the CKEditor, as it will stop
    // working.
    $('table:not(.cke_editor)', context).once('responsive-table', function () {
      $(this).wrap('<div class="responsive-table-wrapper" />').wrap('<div class="responsive-table-scroller" />');
    });
  }
}

/**
 * Add superfish style to every menu in navigation region
 */
Drupal.behaviors.superfishNavigation = {
  attach: function(context, settings) {
    if (!$.matchmedia(query)) {
      $('#block--menu-menu-applicant-menu, #block--menu-menu-recruiter-menu', context).once('superfish-navigation', function () {
        $(this).find('ul.menu').first().addClass('sf-menu').superfish({
              delay:       600,
              speed:       'fast',
              dropShadows: false,
              autoArrows:  false,
          });
      });
    }
  }
}

/**
 * Open the overlay.
 *
 * @param url
 *   The URL of the page to open in the overlay.
 *
 * @return
 *   TRUE if the overlay was opened, FALSE otherwise.
 */
if (Drupal.hasOwnProperty('overlay')) {
  Drupal.overlay.open = function (url) {
    // Just one overlay is allowed.
    if (this.isOpen || this.isOpening) {
      return this.load(url);
    }

    if (!$.matchmedia(query).applies) {
      this.isOpening = true;
      // Store the original document title.
      this.originalTitle = document.title;

      // Create the dialog and related DOM elements.
      this.create();

      this.isOpening = false;
      this.isOpen = true;
      $(document.documentElement).addClass('overlay-open');
      this.makeDocumentUntabbable();

      // Allow other scripts to respond to this event.
      $(document).trigger('drupalOverlayOpen');

      return this.load(url);
    }
    else {
      window.location.href = url;
    }
  };
}

})(jQuery);
