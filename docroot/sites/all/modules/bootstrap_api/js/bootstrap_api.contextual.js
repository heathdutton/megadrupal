/**
 * @file
 * replace for the core contextual.js
 */

(function ($) {
  /**
   * Attaches outline behavior for regions associated with contextual links.
   */
  Drupal.behaviors.bootstrap_api_contextualLinks = {
    attach: function (context) {

      $('div.contextual-links-wrapper', context).once('contextual-links', function () {
        var $wrapper = $(this);
        var $region = $wrapper.closest('.contextual-links-region');
        var $links = $wrapper.find('ul.contextual-links');

        var $trigger = $('<a class="contextual-links-trigger btn dropdown-toggle" href="#" />')
          .html('<i class="icon icon-cog"></i> <span class="element-invisible">' + Drupal.t('Configure') + '</span> <span class="caret"></span>');

        // add Class for bootstrap
        $links.addClass('dropdown-menu');

        // Attach hover behavior to trigger and ul.contextual-links.
        $trigger.add($links).hover(
          function () {
            $region.addClass('contextual-links-region-active');
          },
          function () {
            $region.removeClass('contextual-links-region-active');
          }
        );

        $trigger.dropdown();

        $region.on('mouseleave', function(){
          $wrapper.removeClass('open');
        });

        // Prepend the trigger.
        $wrapper.prepend($trigger);
      });
    }
  };
})(jQuery);
