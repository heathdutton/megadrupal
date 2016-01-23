(function($) {
  Drupal.behaviors.icheck = {
    attach: function(context, settings) {
      for (var id in settings.icheck) {
        var skin = settings.icheck[id]['skin'];
        var color = settings.icheck[id]['color'];
        var classSuffix = (skin === color) ? '_' + skin : '_' + skin + '-' + color;

        // Because of AJAX, Drupal.settings are merged, so we need to get one
        // of the values.
        if (skin instanceof Array) {
          skin = skin.pop();
        }
        if (color instanceof Array) {
          color = color.pop();
        }

        if (skin === 'line') {
          // Remove old label.
          var label = $("label[for='" + id + "']");
          var label_text = label.text();
          label.remove();

          // line skin need specific label.
          $('#' + id, context).iCheck({
            checkboxClass: 'icheckbox' + classSuffix,
            radioClass: 'iradio' + classSuffix,
            insert: '<div class="icheck_line-icon"></div>' + label_text
          });
        }
        else {
          $('#' + id, context).iCheck({
            checkboxClass: 'icheckbox' + classSuffix,
            radioClass: 'iradio' + classSuffix,
          });
        }
      }
    }
  };

  Drupal.behaviors.icheckTableSelectAll = {
    attach: function() {
      $('table th.select-all').bind('DOMNodeInserted', function() {
        $(this).find('input[type="checkbox"]').change(function() {
          var _index = $(this).parents('th').index() + 1;
          $(this).parents('thead').next('tbody').find('tr td:nth-child(' + _index + ') input')
            .iCheck(!$(this).is(':checked') ? 'check' : 'uncheck')
            .iCheck($(this).is(':checked') ? 'check' : 'uncheck');
        });
      });
    }
  };
})(jQuery);
