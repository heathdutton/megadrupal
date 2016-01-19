(function ($) {

  Drupal.behaviors.uniqueCommentsFieldsetSummaries = {
    attach: function (context) {
      $('fieldset.unique-comments-node-settings-form', context).drupalSetSummary(function (context) {
        return Drupal.checkPlain($('.form-item-unique-comments-disable input:checked', context).length > 0 ? 'Unique Comments Disabled' : 'Unique Comments Enabled');
      });
    }
  };

})(jQuery);
