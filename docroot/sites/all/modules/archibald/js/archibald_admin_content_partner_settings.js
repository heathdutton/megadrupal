(function ($) {

Drupal.behaviors.archibald_admin_content_overview = {
  attach: function (context, settings) {
    var logo_button_remove = $("#edit-icon > #edit-icon-remove-button");
    if (logo_button_remove) {
      var logo_button_remove_wrapper = $('<input type="button" class="form-submit" id="edit-icon-remove-button_wrapper" value="' + Drupal.t('Remove') + '" />');

      $(logo_button_remove_wrapper).unbind('click').bind('click', function() {

        var cancel_button = $('<input type="button" class="form-submit" value="' + Drupal.t('Cancel') + '"/>');
        var confirm_button = $('<input type="button" class="form-submit" value="' + Drupal.t('Confirm') + '"/>');

        var html  = '<div id="remove_partner_logo_dialog" title="' + Drupal.t('You are changing the content partner logo!') + '">';
            html += '   <div>' + Drupal.t('WARNING: If you change the content partner logo you have to republish all descriptions of this content partner.');
            html += '</div>';

             $(html).dialog({
               modal: true,
               width: 500,
               open: function() {
                 $(".ui-dialog-titlebar-close > span").html(Drupal.t('Close Window') + '<img border="0" src="' + Drupal.settings.archibald.urls.module_base_path + '/images/close.png">');
                 $("#remove_partner_logo_dialog").append(confirm_button);
                 $("#remove_partner_logo_dialog").append(cancel_button);

                 cancel_button.bind('click', function() {
                   $('#remove_partner_logo_dialog').dialog('destroy').remove();
                 });

                 confirm_button.bind('click', function() {
                   $('#remove_partner_logo_dialog').dialog('destroy').remove();
                   $(logo_button_remove_wrapper).hide();
                   $(logo_button_remove).show().click().mousedown().mouseup();
                });
               }
             });


      });
      $('#edit-icon-remove-button_wrapper').remove();
      $(logo_button_remove).hide().after(logo_button_remove_wrapper);
    }
  }
};

})(jQuery);
