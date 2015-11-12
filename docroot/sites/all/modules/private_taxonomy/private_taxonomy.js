(function ($) {
  Drupal.behaviors.private_taxonomy = {
    attach: function (context, settings) {
      $('#edit-existing-users').click(function() {
        if (confirm(Drupal.t('Create default taxonomies for existing users?'))) {
          $.ajax({
            url: Drupal.settings.basePath +
                 'admin/structure/taxonomy/user/create',
            success: function() {
              alert(Drupal.t('Default taxonomies created.'));
            }
          });
        }
        return false;
      });
    }
  };
})(jQuery);
