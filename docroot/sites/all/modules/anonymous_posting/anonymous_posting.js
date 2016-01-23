(function ($) {

  Drupal.behaviors.anonmyous_postingFieldsetSummaries = {
    attach: function (context) {

      // node edit form case
      $('fieldset.anonmyous_posting-node-edit-form-contact_informations', context).drupalSetSummary(function (context) {
        var out = '';
        $('input', context).each(function(index) {
          if (index == 0) {
            out += ' ' + ($(this).val() || Drupal.settings.anonymous);
          } else {
            if ($(this).val().length>0) {
              out += ' - ' + $(this).val();
            }
          }
        })
        return Drupal.t('@summary', {'@summary': out});
      });

      // node type edit form case
      $('fieldset.anonmyous_posting-node-type-edit-form-contact_informations', context).drupalSetSummary(function (context) {
        var value = $('.form-item-anonymous-posting-setting-type input:checked', context).val() || Drupal.settings.anonymous_posting_value,
        options = Drupal.settings.anonymous_posting_options;
        return Drupal.t('@setting_type', {'@setting_type': options[value]});
      });
    }
  };

})(jQuery);