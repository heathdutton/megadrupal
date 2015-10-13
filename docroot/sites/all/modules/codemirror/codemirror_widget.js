(function ($) {

  var codeMirrorEditors = [];

  Drupal.behaviors.codemirrorWidget = {
    attach: function (context, settings) {
      $('textarea.codemirror', context).each(function (i, v) {
        //console.log($(this));
        console.log(i);
        $(this).once('codemirror', function () {
          codeMirrorEditors.push(CodeMirror.fromTextArea($(this).get(0), {
            lineNumbers: true,
            theme: Drupal.settings.codemirror[i].theme,
          }));
        });
      });
      /*
      console.log(context);
      $.each(settings.codemirror, function (id, params) {
        element = document.getElementById(id);
        codeMirrorEditors[id] = CodeMirror.fromTextArea(element, {
          lineNumbers: true,
          theme: params.theme,
        });
      });*/
      /*
      //console.log(settings.codemirror);
      $.each(settings.codemirror, function (i, v) {
        //var element = document.getElementById(i);
        var element = $('.' + i + ' textarea').get(0);
        console.log(element);
        codeMirrorEditors[i] = CodeMirror.fromTextArea(element, {
          lineNumbers: true,
          theme: v.theme,
        });
      });*/
      /*
      $('.field-widget-text-codemirror textarea', context).once('codemirrorWidget', function () {
        var id = $(this).attr('id');
        var editor = CodeMirror.fromTextArea($(this).get(0), {
          lineNumbers: true,
          theme: Drupal.settings.codemirror[id].theme,
        });
      });
      */
    },
    detach: function (context, settings, trigger) {

      //console.log(codeMirrorEditors);
      if (trigger == 'serialize') {
        $.each(codeMirrorEditors, function () {
          this.save();
        });
      }

      /*
      //console.log(trigger);
      //console.log(context);
      if (trigger == 'serialize') {
        $.each(settings.codemirror, function(i, v) {
          codeMirrorEditors[i].toTextArea();
          delete codeMirrorEditors[i];
        });
      } else {

      }
      */
    },
  };

})(jQuery);
