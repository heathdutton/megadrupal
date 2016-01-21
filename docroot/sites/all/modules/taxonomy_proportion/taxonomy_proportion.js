(function ($) {

  Drupal.behaviors.taxonomy_proportion = {
    attach:function(context) {
      // Show / hide the proportion field when the checkbox is clicked.
      $('.form-proportion-checkbox input').click(function() {
        $(this).parent().siblings('.form-proportion-textfield').toggleClass('element-hidden');
      });  
    }
  };

}(jQuery)); 