(function($) {

Drupal.behaviors.fieldWordBoundary = {
  attach: function (context, settings) {

    // Find the elements.
    $wordBoundaryCheckbox = $('input[name*="[settings][word_boundary]"]');
    $ellipsisWrapper = $('input[name*="[settings][ellipsis]"]').parent();

    // Set the initial state of the ellipsis checkbox.
    if ($wordBoundaryCheckbox.is(':checked')) {
      $ellipsisWrapper.show();
    }
    else {
      $ellipsisWrapper.hide();
    }
    
    // Toggle display of the ellipsis checkbox everytime the word boundary
    // value changes.
    $wordBoundaryCheckbox.change(function() {
      if ($(this).is(':checked')) {
        $ellipsisWrapper.show();
      }
      else {
        $ellipsisWrapper.hide();
      }
    });
  
  }
}

})(jQuery);