(function($) {
/*
 * Custom behavior
 */
Drupal.behaviors.jqueryuiFormattersDialog = {
  attach: function(context, settings) {
    resizable = settings.jqueryuiFormattersDialog.resizable;
    modal = settings.jqueryuiFormattersDialog.modal;
    closeOnEscape = settings.jqueryuiFormattersDialog.closeOnEscape;
    $('.jqueryui-formatters.opener').click(function(event) {
      id = $(this).attr('data-delta');
      instance = $(this).attr('data-instance');
      $('#jqueryui-formatters-text-' + id).dialog({ closeOnEscape: closeOnEscape[instance], resizable: resizable[instance], modal: modal[instance] });
        event.preventDefault();
    })
  }
};

})(jQuery);
