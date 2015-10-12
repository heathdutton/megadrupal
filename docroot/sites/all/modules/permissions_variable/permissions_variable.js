(function($) {
  Drupal.behaviors.permissionsVar = {
    /**
     * Implements attach.
     */
    attach: function attach(context, settings) {
      var $permissionsForm = $(context).find('#user-admin-permissions');
      if (!$permissionsForm.length) {
        return;
      }

      var $exportCode = $permissionsForm.find('.permissions-variable-export.code');
      $exportCode.hide().each(function(i, el) {
        var permission = $(this).data('permission');
        $(this).before('<p><a class="permissions-variable-export toggle" data-permission="' + permission + '" href="#">' + Drupal.settings.permissionsVar.toggleExportTextCodeHidden + '</a></p>');
      });

      $permissionsForm.delegate('.permissions-variable-export.toggle', 'click', Drupal.behaviors.permissionsVar.onToggleClick);
    },

    /**
     * Event handler for clicking an export toggle.
     */
    onToggleClick: function onToggleClick(e) {
      var $toggle = $(e.currentTarget);
      var permission = $toggle.data('permission');

      $('.permissions-variable-export.code[data-permission="' + permission + '"]').toggle();
      $toggle.toggleClass('code-visible');

      if ($toggle.is('.code-visible')) {
        $toggle.text(Drupal.settings.permissionsVar.toggleExportTextCodeVisible);
      }
      else {
        $toggle.text(Drupal.settings.permissionsVar.toggleExportTextCodeHidden);
      }
      e.preventDefault();
    }
  };

})(jQuery);
