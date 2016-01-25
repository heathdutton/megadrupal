/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function($) {
  "use strict";

  Drupal.wysiwyg.editor.init.etherpad_lite = function(settings) {

  };

  Drupal.etherpad = Drupal.etherpad || {};
  Drupal.etherpad.getPadContents = function(padid, callback, async) {
    if (typeof(async) === 'undefined') {
      async = true;
    }
    var _call = callback;
    $.ajax({
      'url': Drupal.settings.basePath + 'etherpad/get/',
      'type': 'POST',
      'data': {'padid': padid},
      'async': async,
      'dataType': 'json',
      'success' : function(data) {
        _call(data.content);
      }
    });
  };

  Drupal.etherpad.setPadContents = function(padid, nid, content) {
    $.ajax({
      'url': Drupal.settings.basePath + 'etherpad/set/' + nid,
      'type': 'POST',
      'data': {'padid': padid, 'content': content},
      'dataType': 'json',
      'success' : function(data) {
      }
    });
  };

  /**
   * Attach this editor to a target element.
   */
  Drupal.wysiwyg.editor.attach.etherpad_lite = function(context, params, settings) {
    var editorid = '#' + params.field, newID = params.field + '-editor';
    $(editorid).after($('<div>').attr('id', params.field + '-editor').attr('class', 'pad'));
    var obj = $(editorid).next(),
    egid = $('#etherpad_gid').val(),
    nid = $('#etherpad_nid').val(),
    padid =  egid + '$' + params.field;
    if (typeof(egid) !== 'undefined') {
      $(obj).pad({
        'padId': padid,
        'host': settings.host,
        'showChat': 'true',
        'showControls': 'true',
        'showLineNumbers': true,
        'userName': settings.user
      });
      if (typeof(nid) !== 'undefined' && nid.length>0) {
        Drupal.etherpad.getPadContents(padid, function(txt) {
          if (txt.length === 1) {
            Drupal.etherpad.setPadContents(padid, nid, $(editorid).val());
          }
        });
      }
      $(editorid).hide();
    }
  };

  /**
   * Detach a single or all editors.
   * See Drupal.wysiwyg.editor.detach.none() for a full description of this hook.
   */
  Drupal.wysiwyg.editor.detach.etherpad_lite = function(context, params) {
    var egid = $('#etherpad_gid').val(),
    padid = egid + '$' + params.field,
    editorid = '#' + params.field,
    newID = editorid + '-editor';
    Drupal.etherpad.getPadContents(padid, function(content) {
      $(editorid).val(content);
      $(newID).hide();
      $(editorid).show();
    }, false);
  };

})(jQuery);
