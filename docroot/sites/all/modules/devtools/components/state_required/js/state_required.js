(function ($) {

  // remove core binding
  $(document).unbind('state:required');

  // add new binding override
  $(document).bind('state:required', function(e) {
    if (e.trigger) {
      $(e.target).closest('.form-item, .form-wrapper').find('label .form-required').remove();

      if (e.value) {
        $(e.target).closest('.form-item, .form-wrapper').find('label').append('<span class="form-required">*</span>');
      }
    }
  });
  
})(jQuery);