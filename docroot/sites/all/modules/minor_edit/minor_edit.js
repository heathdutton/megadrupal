(function ($) {

Drupal.behaviors.minorEditToggle = {
  attach: function (context) {
    $('fieldset.node-form-minor-edit', context).drupalSetSummary(function (context) {
      var minoreditCheckbox = $('.form-item-minor-edit input', context);

      // Return 'New revision' if the 'Create new revision' checkbox is checked,
      // or if the checkbox doesn't exist, but the revision log does. For users
      // without the "Administer content" permission the checkbox won't appear,
      // but the revision log will if the content type is set to auto-revision.
      if (minoreditCheckbox.is(':checked')) {
        return Drupal.t('Marked minor');
      }
    });

  }
};

})(jQuery);
