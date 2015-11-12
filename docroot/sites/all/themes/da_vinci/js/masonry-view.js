/**
 * @file
 * Controls the output of Masonry.
 *
 * In first place, we captured in a variable the layer which apply Masonry.
 *
 * The process of disposal of the elements in the view is faster than the load of the images.
 * This happen when our view has a lot of content without pager.
 * With imagesloaded obtain an advance about this placement, avoiding that the elements can overlap.
 *
 * With our functions, the contents will taking the place of the previous deleted content.
 */

(function ($) {
  Drupal.behaviors.da_vinciThemeMasonry = {
    attach: function (context) {
      var container = document.querySelector('.view .view-content');
      var msnry = new Masonry(container, {
        itemSelector: '.views-row',
        columnWidth: '.views-row'
      });

      imagesLoaded(container, function(){msnry.layout();});
      eventie.bind(container, 'click', function (event) {
        if (!classie.has(event.target, 'close')) {
          return;
        }
        msnry.remove($(event.target).closest('.views-row'));
        msnry.layout();
      });
      // Added classes to control body and view styles
      $("body").addClass('page-masonry');
      $("body .view").addClass('view-masonry');
      // Add Close element to "Masonry" article.
      $('.view-masonry article').append('<span class="close">close</span>');
    }
  }
})(jQuery);