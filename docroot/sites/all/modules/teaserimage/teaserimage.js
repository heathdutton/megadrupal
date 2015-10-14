(function ($) {
Drupal.behaviors.teaserimageFieldsetSummaries = {
  attach: function (context) {
    $('#edit-teaserimage', context).drupalSetSummary(function (context) {
      if ($('#edit-teaserimage-1', context).attr('checked')) {
        return Drupal.t('Enabled for this type');
      }
        return Drupal.t('Disabled for this type');
    });
  }
};
})(jQuery);
