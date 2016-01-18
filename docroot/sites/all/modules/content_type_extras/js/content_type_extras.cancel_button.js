(function ($) {
  Drupal.behaviors.content_type_extras_cancel_button = {
    attach: function(context, settings) {
      
      if($('form.node-form #edit-cancel').hasClass('cte-processed') ||
         $('form.node-form #edit-cancel--2').hasClass('cte-processed')) {
           return;
      }
      else {
        $('form.node-form #edit-cancel, form.node-form #edit-cancel--2').addClass('cte-processed');      
        $('form.node-form #edit-cancel, form.node-form #edit-cancel--2').click(function() {
         var answer = confirm(Drupal.t('Are you sure you want to cancel and lose all changes?'));
          if (answer) {
            history.go(-1);
          }
        });      
      }
    }
  }
})(jQuery);
