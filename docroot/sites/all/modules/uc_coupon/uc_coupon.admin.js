(function($) {
  Drupal.behaviors.ucCouponAdmin = {
    attach: function(context) {
      $('#edit-discount', context).keyup(function() {
        if (this.value.indexOf('%') == -1) {
          $(this).siblings('span').show();
        }
        else {
          $(this).siblings('span').hide();
        }
      }).keyup();
    
      $('input[name=apply_to]', context).click(function() {
        if (this.value == 'cheapest' || this.value == 'expensive') {
          $('.form-item-apply-count').show();
        }
        else {
          $('.form-item-apply-count').hide();
        }
      }).filter(':checked').click();
    
      if ($('input[name=use_validity]', context).change(function() {
        $('.form-item-valid-from, .form-item-valid-until').toggle();
      }).is(':not(:checked)')) {
        $('.form-item-valid-from, .form-item-valid-until').hide();
      }
    }
  };
})(jQuery);
