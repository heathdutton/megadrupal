(function ($) {

Drupal.behaviors.initRecruiterColorboxWithin = {
  attach: function (context, settings) {

    // Make sure colorbox links don't continue to open in popup.
    $('a', context)
      .not('.pager a')
      .not('[href^=mailto:]')
      .once('init-recruitercolorboxwithin-processed')
      .attr('target', '_blank');
  }

};

})(jQuery);
