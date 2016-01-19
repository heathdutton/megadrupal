(function($){

$(function(){
  // Hide the form submit button
  $('#go1-ui-twig-form').find('.form-submit').hide();

  var save = function(cm) {
    $('#edit-code').val(cm.getValue()).trigger('change');
  };

  CodeMirror.defineMode('mustache', function(config, parserConfig) {
    var mustacheOverlay = {
      token: function(stream, state) {
        var ch;
        if (stream.match('{{')) {
          while ((ch = stream.next()) != null)
            if (ch == '}' && stream.next() == '}') break;
          stream.eat('}');
          return 'mustache';
        }
        while (stream.next() != null && !stream.match('{{', false)) {}
        return null;
      }
    };

    return CodeMirror.overlayMode(CodeMirror.getMode(config, parserConfig.backdrop || 'text/html'), mustacheOverlay);
  });

  var editor = CodeMirror.fromTextArea(document.getElementById('edit-code'), {
    lineNumbers: true
    , viewportMargin: Infinity
    , readOnly: 0 === document.getElementsByClassName('form-submit').length
    , theme: 'monokai'
    , extraKeys: {'Cmd-S': save , 'Ctrl-S': save}
    , mode: 'mustache'
  });

});

})(jQuery);
