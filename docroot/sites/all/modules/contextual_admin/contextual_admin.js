/**
 * @file
 * Javascript related to contextual links.
 */
(function ($) {

Drupal.behaviors.contextualAdmin = {
  attach: function (context) {
    $('.contextual-admin', context).closest(':has(.region-content)').addClass('contextual-links-region');
  }
};

})(jQuery);
