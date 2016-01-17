(function($){

Drupal.behaviors.unmask_password = {
  attach: function (context, settings) {
    $(document).ready(function(){     
      password = $('#unmask_password_field input[type=password]')
      password.showPassword();
      password.next().attr('autocomplete', 'off');
    });
  }
};

})(jQuery);
