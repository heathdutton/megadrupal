(function ($) {
  /**
   * Defines the personalize admin menu items' behaviour.
   */
  Drupal.behaviors.PersonalizeGaAdmin = {
    attach: function (context) {
      $('input.personalize-ga-personalize-decisions-checkboxes-control').once(function(){
        $(this).on('click', function() {
          var select_div = $('.personalize-ga-personalize-decisions-checkboxes');
          if ($(this).val() === 'some') {
            select_div.show();
            return;
          }
          select_div.hide();
        });
      });
      $('input.personalize-ga-personalize-decisions-checkboxes-control:checked').click();

      $('input.personalize-ga-visitor-actions-checkboxes-control').once(function(){
        $(this).on('click', function() {
          var select_div = $('.personalize-ga-visitor-actions-checkboxes');
          if ($(this).val() === 'some') {
            select_div.show();
            return;
          }
          select_div.hide();
        });
      });
      $('input.personalize-ga-visitor-actions-checkboxes-control:checked').click();
    }
  };

})(jQuery);
