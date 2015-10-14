(function($) {
  $.fn.layouter = function(textarea_id, content) {
    if (content) {
      if (typeof CKEDITOR != 'undefined' && typeof CKEDITOR.instances[textarea_id] != 'undefined') {
        CKEDITOR.instances[textarea_id].insertHtml(content);
      }
      if (typeof tinyMCE != 'undefined' && typeof tinyMCE.activeEditor != 'undefined' && tinyMCE.activeEditor != null) {
        if (tinyMCE.activeEditor.editorId == textarea_id) {
          tinyMCE.activeEditor.dom.add(tinyMCE.activeEditor.getBody(), 'div', {}, content);
        }
      }
    }
  }

  $.fn.addContent = function(content, id) {
    content = $('#' + id).val() + content;
    $('#' + id).val(content);
  }

  $.fn.replaceContent = function(content, id) {
    $('#' + id).val(content);
  }

  Drupal.behaviors.layouter = {
    attach:
    function(context) {
      if (typeof Drupal.settings.layouter != 'undefined') {
        var active_text_formats = Drupal.settings.layouter.active_text_formats;
        for (var textarea_id in Drupal.settings.layouter.default_formats) {
          var format_select_id = textarea_id.replace(/value/, '') + 'format--2';
          var $layouter_link = $('.layouter-link.' + textarea_id);
          $("#" + textarea_id).parent().append($layouter_link.parent());
          $('#' + format_select_id).change(function() {
            layouter_change = $(this).attr('id');
            layouter_change = layouter_change.replace(/format--2/, '') + 'value';
            $layouter_link = $('#layouter-' + layouter_change);
            if (active_text_formats.indexOf($(this).val()) != -1) {
              $layouter_link.parent().show();
            } 
            else {
              $layouter_link.parent().hide();
            }
          });
        }
      }
    }
  }
  
  Drupal.behaviors.layouter_cancel_modal = {
    attach: function(context) {
      var close_button = $('#modal-content .layouter-close');
      if (close_button.length && typeof Drupal.CTools.Modal != 'undefined') {
        close_button.click(function(e) {
          Drupal.CTools.Modal.dismiss();
          e.preventDefault();
        });
      }
    }
  }
})(jQuery);
