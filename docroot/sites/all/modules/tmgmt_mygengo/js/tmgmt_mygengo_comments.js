(function ($) {

  Drupal.behaviors.tmgmt_mygengo = {
    attach: function (context, settings) {

      var $gengoIdField = $('input:hidden[name=submitted_comment_gengo_id]');
      var $gengoAction = $('input:hidden[name=submitted_gengo_action]').val();
      var $ajaxId = $('input:hidden[name=ajaxid]').val();

      var gengoId = $gengoIdField.val();
      if (gengoId) {
        $('html, body').animate({
          scrollTop: $("#" + gengoId + "-comments-list-closing").offset().top - 200
        }, 200);

        $gengoIdField.val('');
        if ($gengoAction == 'revision') {
          var $icon = $('#' + $ajaxId + ' div.tmgmt-ui-icon');
          $icon.removeClass('tmgmt-ui-icon-yellow');
          $icon.removeClass('tmgmt-ui-state-translated');
          $icon.addClass('tmgmt-ui-icon-grey');
          $icon.addClass('tmgmt-ui-state-pending');
          $icon.html('<span></span>');
          $('input.' + gengoId + '-gengo-id').hide();
        }
      }
    }
  };

})(jQuery);
