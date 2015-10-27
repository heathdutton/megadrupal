(function ($) {
Drupal.behaviors.spam_blackhole = {
  attach: function (context) {
    if (Drupal.settings.spam_blackhole && Drupal.settings.spam_blackhole.forms) {
      $('input[name="form_id"]:not(.spam-blackhole-processed)', context).each(function () {
        if (Drupal.settings.spam_blackhole && Drupal.settings.spam_blackhole.forms) {
          forms = Drupal.settings.spam_blackhole.forms;
          for (var i = 0; i < forms.length; i++) {
            form_id = $(this).attr('value');
            if (forms[i] == form_id) {
              cur_form = $(this).parents('form')[0];
              action = $(cur_form).attr('action');
              $(cur_form).attr('action', action.replace(Drupal.settings.spam_blackhole.dummy_url, Drupal.settings.spam_blackhole.base_url));
            }
          }
        }
      }).addClass('spam-blackhole-processed');
    }
  }
};
})(jQuery);
