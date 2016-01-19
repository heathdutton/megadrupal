/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.behaviors.photobox = {};
  Drupal.behaviors.photobox.attach = function(context, settings) {
    // Set Photobox settings.
    $.extend(window._photobox.defaults, Drupal.settings.photobox);

    // Find all galleries on the page.
    var galleries = [];
    $('a.photobox', context).each(function(index, element) {
      var gallery = $(this).data('photoboxGallery');
      if ($.inArray(gallery, galleries) == -1) {
        galleries.push(gallery);
      }
    });

    // Initiate each gallery.
    galleries.forEach(function(gallery, i, arr) {
      // Find all links in this gallery.
      var $all_links = $('a.photobox[data-photobox-gallery="' + gallery + '"]', context),
      $parents = $all_links.parents();
      // Find all common parents.
      $all_links.each(function(index, element) {
        $parents = $parents.has($(this));
      });
      // Use first common parent as a container.
      $parents.first().photobox('a.photobox[data-photobox-gallery="' + gallery + '"]');
    });
  };

})(jQuery);
