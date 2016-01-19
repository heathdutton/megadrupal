/**
 * @file
 * Initialize CodeMirror.
 */

(function ($) {
  'use strict';

  Drupal.behaviors.phpx_toolsCodemirror = {
    attach: function (context, settings) {
      if (typeof CodeMirror != 'undefined') {
        var foldFunc = CodeMirror.newFoldFunction(CodeMirror.braceRangeFinder);
        var editor = CodeMirror.fromTextArea(document.getElementById('edit-code'), {
          lineNumbers: true,
          matchBrackets: true,
          mode: 'application/x-httpd-php-open',
          indentUnit: 2,
          autofocus: true,
          enterMode: 'keep',
          tabMode: 'shift',
          continuousScanning: 500,

          onCursorActivity: function () {
            editor.matchHighlight('CodeMirror-matchhighlight');
          },

          onGutterClick: foldFunc,

          extraKeys: {
            'Ctrl-Q': function (cm) {
              foldFunc(cm, cm.getCursor().line);
            },
            'F11': function () {
              $('#page').css('position', 'static');
              var scroller = editor.getScrollerElement();
              var wrapper = editor.getWrapperElement();

              if (scroller.className.search(/\bCodeMirror-fullscreen\b/) === -1) {
                scroller.className += ' CodeMirror-fullscreen';
                wrapper.className += ' fullscreen';
                scroller.style.height = '100%';
                scroller.style.width = '100%';
              }
              else {
                scroller.className = scroller.className.replace(' CodeMirror-fullscreen', '');
                scroller.style.height = '';
                scroller.style.width = '';
              }

              editor.refresh();
            },
            'Esc': function () {
              $('#page').css('position', 'relative');
              var scroller = editor.getScrollerElement();
              var wrapper = editor.getWrapperElement();

              if (scroller.className.search(/\bCodeMirror-fullscreen\b/) !== -1) {
                scroller.className = scroller.className.replace(' CodeMirror-fullscreen', '');
                wrapper.className = wrapper.className.replace(' fullscreen', '');
                scroller.style.height = '';
                scroller.style.width = '';
                editor.refresh();
              }
            }
          }
        });

        codemirror = {};
        codemirror.editor = editor;
      }
    }
  };

})(jQuery);
