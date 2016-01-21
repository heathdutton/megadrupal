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
              $('#'+div).html("An error has occurred - " + errorThrown);
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
  var error = data['error'];
  var target = data['target'];
  var source_obj = data['source'];

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
      case 'rid':
      case 'nid':
      case 'vid':
        break;
      case 'field_collection':
        jQuery.each(f_object, function(t_parent_fc, t_fc_obj) {
          jQuery.each(t_fc_obj, function(t_child_fc, t_child_fc_arr) {
            jQuery.each(t_child_fc_arr, function(t_entity_id, t_child_fc_obj) {
              jQuery.each(t_child_fc_obj, function(t_child_fc_name, t_child_fc_und_arr) {
                jQuery.each(t_child_fc_und_arr, function(t_fc_und, t_child_fc_delta_arr) {
                  jQuery.each(t_child_fc_delta_arr, function(t_child_delta, t_field_obj) {
                    var source_text = '';
                    var target_text = '';

                    if (source_obj[field] != null && source_obj[field] != undefined) {
                      if (source_obj[field][t_parent_fc] != null && source_obj[field][t_parent_fc] != undefined) {
                        if (source_obj[field][t_parent_fc][t_child_fc] != null && source_obj[field][t_parent_fc][t_child_fc] != undefined) {
                          if (source_obj[field][t_parent_fc][t_child_fc][t_entity_id] != null && source_obj[field][t_parent_fc][t_child_fc][t_entity_id] != undefined) {
                            if (source_obj[field][t_parent_fc][t_child_fc][t_entity_id][t_child_fc_name] != null && source_obj[field][t_parent_fc][t_child_fc][t_entity_id][t_child_fc_name] != undefined) {
                              if (source_obj[field][t_parent_fc][t_child_fc][t_entity_id][t_child_fc_name][t_fc_und] != null && source_obj[field][t_parent_fc][t_child_fc][t_entity_id][t_child_fc_name][t_fc_und] != undefined) {
                                if (source_obj[field][t_parent_fc][t_child_fc][t_entity_id][t_child_fc_name][t_fc_und][t_child_delta] != null && source_obj[field][t_parent_fc][t_child_fc][t_entity_id][t_child_fc_name][t_fc_und][t_child_delta] != undefined) {
                                  source_text = escape_html(source_obj[field][t_parent_fc][t_child_fc][t_entity_id][t_child_fc_name][t_fc_und][t_child_delta]['translatedContent']);
                                }
                              }
                            }
                          }
                        }
                      }
                    }

                    if (source_text == '') {
                      source_text = '<span style="color:red;">Field Deleted</span>';
                    }

                    if (t_field_obj['translatedContent'] != null && t_field_obj['translatedContent'] != undefined) {
                      target_text = escape_html(t_field_obj['translatedContent']);
                    }

                    content += '<tr><td><b>' + t_field_obj['fieldLabel'] + '</b></td><td>' + source_text + '</td><td>' + target_text + '</td></tr>';
                  });
                });
              });
            });
          });
        });

        break;
      case 'metatag':
        jQuery.each(f_object, function(t_field, t_obj) {
          jQuery.each(t_obj, function(x_field, x_obj) {
            var source_text = '';
            var target_text = '';

            if (source_obj['metatag'][t_field] != null && source_obj['metatag'][t_field] != undefined) {
              if (source_obj['metatag'][t_field][x_field]['translatedContent'] != null && source_obj['metatag'][t_field][x_field]['translatedContent'] != undefined) {
                source_text = escape_html(source_obj['metatag'][t_field][x_field]['translatedContent']);
              }
            }

            if (source_text == '') {
              source_text = '<span style="color:red;">Field Deleted</span>';
            }

            if (x_obj['translatedContent'] != null && x_obj['translatedContent'] != undefined) {
              target_text = escape_html(x_obj['translatedContent']);
            }

            content += '<tr><td><b>' + x_obj['fieldLabel'] + '</b></td><td>' + source_text + '</td><td>' + target_text + '</td></tr>';
          });
        });

        break;
      case 'title':
        var source_text = '';
        var target_text = '';

        if (source_obj[field] != null && source_obj[field] != undefined) {
          source_text = escape_html(source_obj[field]);
        }

        if (f_object != null && f_object != undefined) {
          target_text = escape_html(f_object);
        }

        content += '<tr><td><b>Title</b></td><td>' + source_text + '</td><td>' + target_text + '</td></tr>';

        break;
      case 'path':
        var source_text = '';
        var target_text = '';

        if (source_obj[field] != null && source_obj[field] != undefined) {
          source_text = escape_html(source_obj[field]);
        }

        if (f_object != null && f_object != undefined) {
          target_text = escape_html(f_object);
        }

        content += '<tr><td><b>Path Alias</b></td><td>' + source_text + '</td><td>' + target_text + '</td></tr>';

        break;
      default:
        jQuery.each(f_object, function(t_und, t_obj) {
          jQuery.each(t_obj, function(delta, obj) {
            var source_text = '';
            var target_text = '';

            if (obj['alt'] == undefined && obj['title'] == undefined) {
              if (source_obj[field] != null && source_obj[field] != undefined) {
                if (source_obj[field][t_und] != null && source_obj[field][t_und] != undefined) {
                  if (source_obj[field][t_und][delta] != null && source_obj[field][t_und][delta] != undefined) {
                    if (source_obj[field][t_und][delta]['translatedContent'] != null && source_obj[field][t_und][delta]['translatedContent'] != undefined) {
                      source_text = escape_html(source_obj[field][t_und][delta]['translatedContent']);
                    }
                  }
                }
              }

              if (source_text == '') {
                source_text = '<span style="color:red;">Field Deleted</span>';
              }

              if (obj['translatedContent'] != null && obj['translatedContent'] != undefined) {
                target_text = escape_html(obj['translatedContent']);
              }

              content += '<tr><td><b>' + obj['fieldLabel'] + '</b></td><td>' + source_text + '</td><td>' + target_text + '</td></tr>';
            }
            else if (obj['alt'] != undefined || obj['title'] != undefined) {
              if (obj['alt'] != null && obj['alt'] != undefined) {
                if (source_obj[field] != null && source_obj[field] != undefined) {
                  if (source_obj[field][t_und] != null && source_obj[field][t_und] != undefined) {
                    if (source_obj[field][t_und][delta] != null && source_obj[field][t_und][delta] != undefined) {
                      if (source_obj[field][t_und][delta]['alt'] != null && source_obj[field][t_und][delta]['alt'] != undefined) {
                        source_text = escape_html(source_obj[field][t_und][delta]['alt']);
                      }
                    }
                  }
                }

                if (source_text == '') {
                  source_text = '<span style="color:red;">Field Deleted</span>';
                }

                target_text = escape_html(obj['alt']);
                content += '<tr><td><b>Image Alt</b></td><td>' + source_text + '</td><td>' + target_text + '</td></tr>';
              }

              if (obj['title'] == null || obj['title'] == undefined) {
                return;
              }

              var source_text = '';

              if (source_obj[field] != null && source_obj[field] != undefined) {
                if (source_obj[field][t_und] != null && source_obj[field][t_und] != undefined) {
                  if (source_obj[field][t_und][delta] != null && source_obj[field][t_und][delta] != undefined) {
                    if (source_obj[field][t_und][delta]['title'] != null && source_obj[field][t_und][delta]['title'] != undefined) {
                      source_text = escape_html(source_obj[field][t_und][delta]['title']);
                    }
                  }
                }
              }

              if (source_text == '') {
                source_text = '<span style="color:red;">Field Deleted</span>';
              }

              target_text = escape_html(obj['title']);
              content += '<tr><td><b>Image Title</b></td><td>' + source_text + '</td><td>' + target_text + '</td></tr>';
            }
          });
        });
    }
  });

  content += '</table>';

  jQuery('#' + _div).empty();
  jQuery('#' + _div).append(content);

  return true;
}
