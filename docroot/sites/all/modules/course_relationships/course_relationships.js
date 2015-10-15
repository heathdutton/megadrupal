// Fade checkboxes of children when parent is selected.
(function($) {
  $(document).ready(function() {
    $('#course-relationships-add-to-cart-linked').click(function(e) {
      if ($(e.target).attr('checked')) {
        $(e.target).parents('.form-item').next('.course-relationships-children').find('input').fadeTo(0, 0);
      }
      else {
        $(e.target).parents('.form-item').next('.course-relationships-children').find('input').fadeTo(0, 1.0);
      }
    });


// Rewrite add to cart button of parent when doing a la carte selection
    $($('input[value="Enroll"]')[1]).click(function() {
      $("form#course-relationships-add-to-cart-linked .form-submit").trigger("click");
      $("#edit-submit").val("Processing...").attr("disabled", "disabled").css("opacity", 0.5);
    });
  });

})(jQuery);
