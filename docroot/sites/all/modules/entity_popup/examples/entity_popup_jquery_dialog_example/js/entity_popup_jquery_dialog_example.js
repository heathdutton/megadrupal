(function($, Drupal) {
  Drupal.behaviors.entityPopupJqueryDialogExample = {
    attach: function(context) {
      var $body = $('body');
      $body.once('entity-popup-jquery-dialog-example', function () {
        $body.append('<div id="entity-popup-jquery-dialog-example"></div>');
      });
      var $dialog = $('#entity-popup-jquery-dialog-example');
      $('a.entity-popup-jquery-dialog-example', context).once('entity-popup-jquery-dialog-example', function () {
          var $self = $(this);
          $self.click(function(event) {
            event.preventDefault();
            var href = $self.attr('href');
            // Create an element so we can parse our a URL no matter if its internal or external.
            var parse = document.createElement('a');
            parse.href = href;
            $dialog.load('/entity-popup' + parse.pathname, function() {
              $dialog.dialog('destroy');
              $dialog.dialog({autoOpen: true, modal: true, width: 'auto', resizable: false});
              $dialog.dialog('open');
            });
          });

      });
    }
  };
})(jQuery, Drupal);
