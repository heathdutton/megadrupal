(function($) {

/**
 * Attach this editor to a target element.
 */
Drupal.wysiwyg.editor.attach.ueditor = function (context, params, settings) {
  var editorOption = {
    lang: settings['language'],
    zIndex: settings['zindex'],
    initialFrameHeight: settings['initialFrameHeight'],
    initialFrameWidth: '100%',
    initialContent: settings['initialContent'],
    serverUrl: settings['serverUrl'],
    toolbars: settings['toolbars'],
    autoHeightEnabled: settings['auto_height'],
    autoFloatEnabled: settings['auto_float'],
    allowDivTransToP: settings['allowdivtop'],
    elementPathEnabled: settings['show_elementpath'],
    wordCount: settings['show_wordcount'],
    pageBreakTag: settings['pageBreakTag'],
    UEDITOR_HOME_URL: settings['editorPath'],
  };
  var editor = new baidu.editor.ui.Editor(editorOption);
  // Attach editor.
  editor.render(params.field);

  Drupal.wysiwyg.instances[params.field]['rendered'] = editor;
};

/**
 * Detach a single or all editors.
 */
Drupal.wysiwyg.editor.detach.ueditor = function (context, params, trigger) {
  if (trigger === 'serialize') {
    // Get editor.
    var editor = Drupal.wysiwyg.instances[params.field]['rendered'];
    // Move content to original textarea.
    var txtarea = $('div#' + params.field).parent().find('textarea');
    txtarea.val(editor.getContent());
  }else if(trigger === 'unload'){
    var editor = Drupal.wysiwyg.instances[params.field]['rendered'];
    editor.destroy();
  }
};

})(jQuery);
