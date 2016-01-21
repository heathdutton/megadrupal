
(function ($) {

Drupal.behaviors.refreshFieldsetSummaries = {
  attach: function (context) {
    $('fieldset.refresh-form', context).drupalSetSummary(function (context) {
      var refresh = $('.form-item-refresh input').val();

      return refresh ?
        Drupal.t('Refresh: @refresh seconds', { '@refresh': refresh }) :
        Drupal.t('No refresh');
    });
  }
};

})(jQuery);
