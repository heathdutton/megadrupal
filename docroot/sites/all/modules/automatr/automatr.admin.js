(function ($) {
  "use strict";

  /**
   * Open external links in a new window.
   */
  Drupal.behaviors.automatrExternalLinks = {
    attach: function (context) {
      $('#automatr-admin-settings-form a', context).filter(function () {
        // Ignore non-HTTP (e.g. mailto:) link.
        return this.href.indexOf('http') === 0;
      }).filter(function () {
        // Filter out links to the same domain.
        return this.hostname && this.hostname !== location.hostname;
      }).each(function () {
        // Open others in their own windows.
        $(this).attr('target', '_blank');
      });
    }
  };

  /**
   * Provide the summary information for the "Implementation scope" vertical tabs.
   */
  Drupal.behaviors.automatrImplementationScopeSummary = {
    attach: function (context) {
      // Make sure the drupalSetSummary method required for this behavior is
      // available.
      if (typeof jQuery.fn.drupalSetSummary == 'undefined') {
        return;
      }

      // Pages.
      $('fieldset#edit-pages', context).drupalSetSummary(function (context) {
        if (!$('textarea#edit-automatr-implementation-pages-pages', context).val()) {
          return Drupal.t('Not restricted');
        }
        else {
          return Drupal.t('Restricted to certain pages');
        }
      });

      // Roles.
      $('fieldset#edit-roles', context).drupalSetSummary(function (context) {
        var vals = [];
        $('input[type="checkbox"]:checked', context).each(function () {
          vals.push($.trim($(this).next('label').text()));
        });
        if (!vals.length) {
          vals.push(Drupal.t('Not restricted'));
        }
        return vals.join(', ');
      });
    }
  };

})(jQuery);
