
(function ($) {

Drupal.behaviors.translationHierarchyFieldsetSummaries = {
  attach: function (context) {
    $('fieldset#edit-translation', context).drupalSetSummary(function (context) {
      var blocking = $('#edit-translation-blocking', context).is(':checked') ? ', ' + Drupal.t('Blocking') : '';
      var status = $('#edit-translation-status', context).is(':checked') ? Drupal.t('Published') : Drupal.t('Not published');
      var translate;
      if ($('#edit-translation-retranslate', context).size()) {
        translate = $('#edit-translation-retranslate', context).is(':checked') ? Drupal.t('Flag translations as outdated') : Drupal.t('Do not flag translations as outdated');
      }
      else {
        translate = $('#edit-translation-translate', context).is(':checked') ? Drupal.t('Needs to be updated') : Drupal.t('Does not need to be updated');
      }
      return status + blocking + ', ' + translate;
    });
  }
};

})(jQuery);
