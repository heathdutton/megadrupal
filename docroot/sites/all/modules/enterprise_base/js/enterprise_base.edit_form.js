(function ($) {
  /**
   * Move fields to right column on node edit form.
   */
  Drupal.behaviors.enterprise_main = {
    attach: function (context, settings) {
      // check if side column exists
	  if ($(".column-side").length == 1) {
	    //$(".column-main .form-item-publish-on").each(function() {
	    //    $(this).remove();
	    //    $('.column-side .column-wrapper').append($(this));
	    //});
	    // Move taxonomy fields to right sidebar in rubik.
	    $(".display_sidebar").each(function() {
	      $(this).remove();
	      $('.column-side .column-wrapper').append($(this));
	    });
	    // move schedular fields

	    // Hide the buttons on the bottom as they are duplicated
	    //$('.column-main #edit-actions').remove();
	  }
    }
  };
}(jQuery));