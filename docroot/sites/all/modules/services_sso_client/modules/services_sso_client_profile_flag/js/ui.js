(function ($) {
  Drupal.behaviors.services_sso_client_profile_flag_ui = {
    attach: function(context) {
      var form = $('#services-sso-client-profile-flag-actions');
      $('.tab-controls .item-list a', form).each(function(i) {
        var link = $(this);
        
        link.click(function(e) {
          $('.tab-content:not(.tab-content-'+link.attr('rel')+')', form).hide();
          $('.tab-content-'+link.attr('rel'), form).fadeIn();
          $('.tab-controls .item-list a', form).removeClass('active');
          $(this).addClass('active');
          return false;
        });
      });
    }
  };
})(jQuery);