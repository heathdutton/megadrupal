(function($) {
  Drupal.behaviors.php_ace = {
    attach: function(context, settings) {
      $('.ace-enabled').each(function(i) {
        var textarea = $(this);
        id = textarea.attr('id');
        textarea.before('<div id="' + id + '-ace"></div>');
        var editor = ace.edit(id + '-ace');
        editor.getSession().setMode({path:"ace/mode/php", inline:true})
        editor.getSession().setValue(textarea.val());
        editor.getSession().on('change', function(){
          textarea.val(editor.getSession().getValue());
        });
        textarea.hide();
      });
    }
  };
})(jQuery);