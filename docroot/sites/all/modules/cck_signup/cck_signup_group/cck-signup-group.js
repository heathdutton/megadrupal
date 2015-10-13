(function ($) {
Drupal.behaviors.cckSignupGroup = {
  attach: function(context) {
    var language = Drupal.settings.cckSignupGroup.language;
    var field = $('#edit-' + Drupal.settings.cckSignupGroup.field + '-' + language + '-0-value', context);
    var group = $('.' + Drupal.settings.cckSignupGroup.group, context);
    if (field.val() < 2) {
      group.hide();
    }
    field.change(function () {
      if ($(this).val() < 2) {
        group.hide();
      }
      else {
        group.fadeIn();
        $('input:first', group).focus();
      }
    });
  }
};

}) (jQuery);