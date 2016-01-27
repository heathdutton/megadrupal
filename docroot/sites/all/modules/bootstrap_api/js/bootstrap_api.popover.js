/**
 * @file
 *
 * @author Dennis Brücke (blackice2999) | TWD - team:Werbedesign UG
 *   @see http://drupal.org/user/413429
 *   @see http://team-wd.de
 */

;
(function ($) {
  "use strict";

  Drupal.bootstrap_api = Drupal.bootstrap_api || {};
  Drupal.bootstrap_api.Popover = Drupal.bootstrap_api.Popover || {};

  // ************************************************************************************************
  // * Drupal.behavior
  // ************************************************************************************************
  Drupal.behaviors.bootstrap_api_popover = {
    attach: function (context, settings) {

      // bind popover behavior on settings provided elements
      for (var popover_el in settings.bootstrap_api_popover) {
        if (!$('#' + popover_el + '.bootstrap-api-popover-processed', context).length) {
          var element_settings = settings.bootstrap_api_popover[popover_el];
          if (typeof element_settings.elements == 'undefined') {
            element_settings.elements = '#' + popover_el;
          }

          $(element_settings.elements, context).each(function () {
            $(this).popover(element_settings);
          });

          $('#' + popover_el, context).addClass('bootstrap-api-popover-processed');
        }
      }

      // Bind default popover behavior on rel
      $('[rel=popover]', context).once('bootstrap-api-popover', function () {
        $(this).popover();
      });

      // Bind Popover behaviors to all form items showing the class.
      $('[class=use-popover]', context).has('.description').once('bootstrap-api-popover', function () {
        var $descEl = $('.description', this);
        var $inputEl = $('input', this);

        $($inputEl).popover({
          content: $descEl.text(),
          trigger: 'focus'
        });

        $descEl.hide();
      });
    }
  };
})(jQuery);
