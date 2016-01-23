/**
 * @file
 * Facets form helper.
 *
 * We use the Form API #states processor to recursively check checkboxes.
 * However, this introduces a small issue when a user wants to *only* check a
 * child checkbox. Because the parent is not checked, upon page load the child
 * will be unchecked. The following code circumvents this behavior by simply
 * checking the box again after page load.
 * This also introduces another issue (#2539108) when activating a filter
 * through clicking on a link. The child checkboxes will get checked, but in
 * this case we don't want them to be.
 * Basically, this script undos the Form API #states processing on page load.
 */

(function($, Drupal) {
  'use strict';

  Drupal.behaviors.dsbPortalFacets = {

    attach: function(context, settings) {
      var $checkboxes = $('input[name^="filter["][checked]', context);
      if ($checkboxes.length) {
        // The found checkboxes are checked in HTML; they may not be for the
        // browser. Check them again.
        $checkboxes.attr('checked', true);
      }

      $checkboxes = $('input[name^="filter["]:not([checked]):checked', context);
      if ($checkboxes.length) {
        // The found checkboxes are not checked in HTML, yet are so for the
        // browser. Uncheck them.
        $checkboxes.attr('checked', false);
      }
    }

  };

})(jQuery, Drupal);
