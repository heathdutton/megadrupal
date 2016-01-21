/**
 * @file
 * Behaviors for masquerade float block.
 *
 * This code initialize masquerade block with jQuery UI.
 */

(function ($) {

  /**
   * Helper functions.
   *
   * Compare library versions.
   *
   * @param version1 string
   *  version1
   * @param version2 string
   *   version2
   *
   * @return
   *    0 if two params are equal
   *    1 if the second is lower
   *   -1 if the second is higher
   */
  var versionCompare = function (version1, version2) {
    if (version1 == version2) {
      return 0;
    }
    var v1 = normalize(version1);
    var v2 = normalize(version2);
    var len = Math.max(v1.length, v2.length);
    for (var i = 0; i < len; i++) {
      v1[i] = v1[i] || 0;
      v2[i] = v2[i] || 0;
      if (v1[i] == v2[i]) {
        continue;
      }
      return v1[i] > v2[i] ? 1 : -1;
    }
    return 0;
  };

  function normalize(version) {
    return $.map(version.split('.'), function (value) {
      return parseInt(value, 10);
    });
  }

  Drupal.behaviors.masquerade_float_block = {
    attach: function (context, settings) {
      $('body', context).once('masquerade-float-block', function () {
        var form = settings.masquerade_float_block.block.content;
        var dialog = $('<div />').attr({title: settings.masquerade_float_block.block.subject}).html(settings.masquerade_float_block.block.content);

        var switcher = $('<div />').attr({style: "position: absolute;top:25px;right:0;background-color:black;color:white;cursor:pointer;z-index:999;padding:2px 4px;"});

        $(this).append(dialog);
        Drupal.attachBehaviors(form, settings);

        // 0 - closed.
        // 1 - opened.
        var dialog_state = $.cookie('mfb-dialog-state') || 1;
        // Elements position memory.
        var switcher_pos_top = $.cookie('mfb-switcher_pos_top') || 25;
        var switcher_pos_left = $.cookie('mfb-switcher_pos_left') || 0;
        var dialog_pos_top = $.cookie('mfb-dialog_pos_top') || 25;
        var dialog_pos_left = $.cookie('mfb-dialog_pos_left') || 0;

        switcher.css({top: switcher_pos_top + 'px', left: switcher_pos_left + 'px'});
        var dialogPosition = 'center';
        if (typeof $.ui.version !== 'undefined') {
          if (versionCompare('1.10', $.ui.version) === 1 || versionCompare('1.10', $.ui.version) === 0) {
            dialogPosition = [parseInt(dialog_pos_left), parseInt(dialog_pos_top)];
          }
          else {
            dialogPosition = {
              my: "left+" + dialog_pos_left + " top+" + dialog_pos_top,
              at: "left top",
              of: window
            };
          }
        }

        dialog.dialog({
          autoOpen: dialog_state == 1 ? true : false,
          position: dialogPosition,
          resizable: false,
          dragStop: function (event, ui) {
            $.cookie('mfb-dialog_pos_top', ui.position.top, {path: '/'});
            $.cookie('mfb-dialog_pos_left', ui.position.left, {path: '/'});
          },
          modal: true,
          close: function (event, ui) {
            switcher.show();
            $.cookie('mfb-dialog-state', 0, {path: '/'});
          },
          open: function (event, ui) {
            $.cookie('mfb-dialog-state', 1, {path: '/'});
          },
          buttons: {
            "Hide": function () {
              $(this).dialog("close");
              switcher.show();
            }
          }
        });

        switcher.height(20);
        switcher.width(170);

        switcher.draggable({
          stop: function (event, ui) {
            $.cookie('mfb-switcher_pos_top', ui.position.top, {path: '/'});
            $.cookie('mfb-switcher_pos_left', ui.position.left, {path: '/'});
          }
        });

        var t = Drupal.t;
        switcher.html(t('Show masquerade block'));
        $(this).append(switcher);

        if (dialog_state == 1) {
          switcher.hide();
        }

        switcher.click(function () {
          dialog.dialog('open');
          switcher.hide();
        });

      });
    }
  };

})(jQuery);
