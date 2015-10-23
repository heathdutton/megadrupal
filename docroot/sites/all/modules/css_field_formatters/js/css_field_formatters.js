(function ($) {

  Drupal.behaviors.css_field_formatter = {
    attach: function (context, settings) {
      // Load and init ace editor asynchronous to support ajax forms.
      $.getScript('//cdn.jsdelivr.net/ace/1.1.6/noconflict/ace.js', function(data, textStatus, jqxhr ) {
        $('textarea[data-ace-editor]').each(function () {
          var textarea = $(this);
          textarea.removeAttr('data-ace-editor');

          // Hide Drupal textarea.
          textarea.siblings('.grippie').hide();

          var editDiv = $('<div>', {
            width: '100%',
            'class': textarea.attr('class')
          }).css('min-height', textarea.height())
            .insertBefore(textarea);

          textarea.addClass('element-invisible');

          // Init ace editor.
          var editor = ace.edit(editDiv[0]);
          editor.getSession().setValue(textarea.val());
          editor.getSession().setMode('ace/mode/css');
          editor.getSession().setTabSize(2);
          editor.setTheme('ace/theme/monokai');
          editor.setOptions({
            minLines: 3,
            maxLines: 20
          });

          // Update Drupal textarea value.
          editor.getSession().on('change', function(){
            textarea.val(editor.getSession().getValue());
          });
        });
      });
    }
  };

})(jQuery);