/**
 * @file
 * Provide summaries for vertical tabs in the content entity forms.
 */
(function ($) {
  Drupal.behaviors.lyris_contentFieldsetSummeries = {
    attach: function (context) {

      $('fieldset#edit-fs-mailing', context).drupalSetSummary(function (context) {
        var subject = $('#edit-lyris-nativetitle').val();
        var to      = $('#edit-lyris-headerto').val();
        var from    = $('#edit-lyris-headerfrom').val();

        var out = Drupal.t('To') + ': ' + to + '<br />' + Drupal.t('From') + ': ' + from + '<br />' + Drupal.t('Subject') + ': ' + subject;

        return $(context).hasRequired() + out;
      });
    }
  }
})(jQuery);
