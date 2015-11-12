(function($) {
Drupal.behaviors.tribune_admin = {
  attach: function() {
    $('#edit-custom-users input[type=submit]').click(function() {
      $(this).parents('fieldset:first').remove();
    });

    $('#edit-custom-users input[type=checkbox][name*=banned]').click(function() {
      if ($(this).is(':checked')) {
        $(this).parents('fieldset:first').find('input:not([name*=banned])').removeAttr('checked');
      }
    });
    $('#edit-custom-users input[type=checkbox]:not([name*=banned])').click(function() {
      if ($(this).is(':checked')) {
        $(this).parents('fieldset:first').find('input[name*=banned]').removeAttr('checked');
      }
    });
  }
};

})(jQuery);
