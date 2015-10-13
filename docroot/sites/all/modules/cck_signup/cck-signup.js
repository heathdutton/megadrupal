
(function ($) {

Drupal.behaviors.cckSignupFieldsetSummaries = {
  attach: function (context) {
    $('fieldset.node-form-cck-signup', context).drupalSetSummary(function (context) {
      var vals = [];

      $('input:checked', context).parent().each(function () {
        vals.push(Drupal.checkPlain($.trim($(this).text())));
      });

      if (!$('.form-item-cck-signup-type input', context).is(':checked')) {
        vals.unshift(Drupal.t('Not enabled'));
      }
      else {
        // Node reference field
	var field = $(".form-item-cck-signup-field select option:selected", context).val();
	if (field != 0) {
          vals.push(Drupal.t('Field: @field', {'@field': field}));
	}
	else {
          vals.push('<span class="error">' + Drupal.t('Missing field') + '</span>');
	}
      }

      return vals.join(', ');
    });
  }
};

})(jQuery);
