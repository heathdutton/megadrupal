(function ($) {
  Drupal.behaviors.layoutEditorPalette = {
    attach: function(context, settings) {
      $('body').once('layout-editor-palette' ,function() {
        $(this).prepend(settings.editbar);
        $(this).prepend(settings.palette);      
      });
    }
  } 
  
  Drupal.behaviors.browserEditor = {
    attach: function(context) {
      var $dialog = $('#out-of-context-editor', context).dialog({
        autoOpen: false,
        title: 'Layout Editor',
        zIndex: 500,
        position: [20,50],
        dialogClass: 'out-of-context-palette'
      });
      
      $('.edit-out-of-context').click(function() { 
        $dialog.dialog('open');
        return false;
      });
    }
  }
})(jQuery);