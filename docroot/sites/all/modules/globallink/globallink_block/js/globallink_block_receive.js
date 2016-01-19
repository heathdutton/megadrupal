var _div;
var _img;

(function($) {
  Drupal.behaviors.globallink = {
    attach: function() {
      if (Drupal.settings.globallink == undefined) {
        return;
      }

      var popup = Drupal.settings.globallink.popup;
      var previewpath = Drupal.settings.globallink.previewpath;
      var rids = Drupal.settings.globallink.rids;

      _img = Drupal.settings.globallink.ajax_image;

      $.each(popup, function(link, div) {
        $('#' + link).click(function() {
          _div = div;

          $('#' + div).dialog({
            modal: true,
            show: {
              effect: "blind",
              duration: 100
            },
            width: 700,
            height : 400
          });

          $('#' + div).empty();

          $('#' + div).append('<div style="width: 100%; height:100%; text-align:center;"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div></div>');

          $.ajax({
            type: 'POST',
            url: previewpath,
            dataType: 'json',
            data: 'rid=' + rids[div],
            success: ajax_completed,
            error: function(xhr, textStatus, errorThrown) {
              $('#' + _div).empty();
              $('#'+div).html(textStatus);
            }
          });
        });
      });
    }
  }
})(jQuery);

function escape_html(string) {
  return jQuery('<pre>').text(string).html();
}

function ajax_completed(data) {
  var content = '<table class="tpt_popup_table"><tr><th>&nbsp;</th><th>Source Content</th><th>Translated Content</th></tr>';

  error = data['error'];
  target = data['target'];
  source_obj = data['source'];

  if (error != null && error != undefined) {
    content += '<tr><td colspan="3"><span style="color: red;text-align: center;">' + error + '</span></td></tr>';
    content += '</table>';

    jQuery('#' + _div).empty();
    jQuery('#' + _div).append(content);

    return true;
  }

  if (source_obj == null || source_obj == undefined || source_obj == '') {
    return true;
  }

  jQuery.each(target, function(field, f_object) {
    switch (field) {
      case 'title':
        var source_text = '';
        var target_text = '';

        if (source_obj[field] != null && source_obj[field] != undefined) {
          source_text = escape_html(source_obj[field]);
        }

        if (source_text == '') {
          source_text = '<span style="color:red;">Field Empty</span>';
        }

        if (f_object != null && f_object != undefined) {
          target_text = escape_html(f_object);

          if (target_text != '') {
            if (field == 'title') {
              label = 'Title';
            }

            content += '<tr><td><b>' + label + '</b></td><td>' + source_text + '</td><td>' + target_text + '</td></tr>';
          }
        }

        break;
      case 'body':
        var source_text = '';
        var target_text = '';

        if (source_obj[field] != null && source_obj[field] != undefined) {
          source_text = escape_html(source_obj[field]);
        }

        if (source_text == '') {
          source_text = '<span style="color:red;">Field Empty</span>';
        }

        if (f_object != null && f_object != undefined) {
          target_text = escape_html(f_object);

          if (target_text != '') {
            if (field == 'body') {
              label = 'Body';
            }

            content += '<tr><td><b>' + label + '</b></td><td>' + source_text + '</td><td>' + target_text + '</td></tr>';
          }
        }

        break;
    }
  });

  content += '</table>';

  jQuery('#' + _div).empty();
  jQuery('#' + _div).append(content);

  return true;
}
