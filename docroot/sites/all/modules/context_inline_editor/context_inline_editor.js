(function ($) {
  Drupal.behaviors.contextInlineEditor = {
    attach: function() {
      $('div.toolbar-drawer:not(.contextInlineEditor-processed)').each(function() {
        $(this).append('<a class="context-inline-editor-edit" href="#">Edit contexts</a>');
        $('.context-inline-editor-edit').click(function(e){
          $('#context-inline-editor').removeClass('element-invisible').dialog({ 
            title: Drupal.t('Context editor'), 
            position: ['left','top'],
            buttons: {
              'Save': function() { 
                $('#context-inline-editor a.done:visible').click();
                $('.context-inline-editor-save').click();
              },
              'Cancel': function() {
                $('.context-inline-editor-cancel').click();
              }
            }
          }).parent().css({position:'fixed'});
          e.preventDefault();
          return false;
        });
      }).addClass('contextInlineEditor-processed');
    },
    detach: function() {
      
    }
  };
})(jQuery);
