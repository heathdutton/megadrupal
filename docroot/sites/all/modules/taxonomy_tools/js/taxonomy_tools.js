(function ($) {
  Drupal.behaviors.TaxonomyTools = {
    attach: function() {
      $('.term-delete').click(function(){
        if ($(this).hasClass('clicked')) {
          $(this).removeClass('clicked');
          $('.form-checkbox').each(function(){
            $(this).attr('checked', false);
          });
        }
        else {
          $(this).addClass('clicked');
          $('.form-checkbox').each(function(){
            $(this).attr('checked', true);
          });
        }
      });
    }
  }
})(jQuery);
