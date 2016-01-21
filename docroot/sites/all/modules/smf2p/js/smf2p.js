(function ($) {

Drupal.behaviors.smf2p_nodesummary = {
  attach: function (context) {
    $('fieldset.node-form-smf2p', context).drupalSetSummary(function (context) {
      var vals = [];

      $('input:checked', context).parent().each(function () {
        vals.push(Drupal.checkPlain($.trim($(this).text())));
      });

      if (!$('.form-item-smf2p-published input', context).is(':checked')) {
        vals.unshift(Drupal.t('No community publish'));
      }

      return vals.join(', ');
    });
    
    $('input[name="smf2p[published]"]').click(function() {
      if ($(this).is(':checked')) {
        $(this).parents('.form-wrapper').find('input[type=submit]').mousedown();
      }
    });
  }
};

})(jQuery);
