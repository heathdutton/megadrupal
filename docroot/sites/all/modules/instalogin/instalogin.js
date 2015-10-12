(function ($) {

Drupal.behaviors.instaLogin = {
  attach: function (context) {

    $('#instalogin-admin-settings-form #edit-instalogin-regenerate').click(function () {
      if(confirm(Drupal.t('Are you sure you want to regenerate the secret key? InstaLogin apps using the current key will not be able to authenticate any more.'))) {
        return true;
      }

      return false;
    });

  }
};

})(jQuery);
