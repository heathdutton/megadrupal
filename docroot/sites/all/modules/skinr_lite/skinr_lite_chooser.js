/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


(function ($) {
  
  $("head").append("<style id='skinr-lite-colors-styles'> </style>");
  
  $.fn.skinrLiteRemoveClasses = function(removeClasses) {
    // @todo: this may be bad code. get a js programmer to do it better.
    this.each(function() {
      $(this).removeClass(removeClasses.join(' '));
    });
    $.each(removeClasses, function(i, cls) {
      if ($.each(Drupal.settings.skinrLite.classes[cls], function(el, type) {
        switch (type) {
          case 'select':
            el = el.split(':');
            if ($(el[0]).val() == el[1]) {
              $('body').addClass(cls);
              return false;
            }
            break;
          default:
            if ($(el).is(':checked') ) {
              $('body').addClass(cls);
              return false;
            }
        }
      }));
    });
    return $(this);
  };
  
  $.fn.skinrLiteChooser = function() {
    this.each(function() {
      var allOptions = Drupal.settings.skinrLite.settings[$(this).attr('skinrlite')].options;
      var removeClasses = [];
      var addClasses = [];
      
      if ($(this).is('select')) {
        $(this).children('option').each(function() {
          removeClasses = removeClasses.concat(allOptions[$(this).val()]['class']);
        });
        addClasses = allOptions[$(this).val()]['class'];
      }
      else if ($(this).is(':checked')) {
        if ($(this).is('.skinr-lite.radios')) {
          $(this).parents('.form-radios.skinr-lite.radios').find('input.skinr-lite.radios').each(function() {
            removeClasses = removeClasses.concat(allOptions[$(this).val()]['class']);
          });
        }
        addClasses = allOptions[$(this).val()]['class'];
      }
      else {
        if ($(this).is('.skinr-lite.checkboxes')) {
          $(this).parents('.form-checkboxes.skinr-lite.checkboxes').find('input.skinr-lite.checkboxes:checked').each(function() {
            addClasses = addClasses.concat(allOptions[$(this).val()]['class']);
          });
        }
        removeClasses = removeClasses.concat(allOptions[$(this).val()]['class']);
      }
      $('body').skinrLiteRemoveClasses(removeClasses);
      $('body').addClass(addClasses.join(' '));
    });
  };
  
  Drupal.behaviors.skinrLite = {
    attach: function (context, settings) {
      $('#skinr-lite-chooser-toggle').click(function() {
        $('body').toggleClass('skinr-lite-show-chooser');
        return false;
      }).show();
      $('select.skinr-lite, input.skinr-lite').change(function() {
        $(this).skinrLiteChooser();
      });
      $('a#skinr-lite-reset-colors').click(function() {
        $.each(Drupal.settings.skinrLiteColors.current, function(k, color) {
          $('#palette input[name="palette[' + k + ']"]').val(color).trigger('focus');
          window.location.reload();
        });
        return false;
      });
      $('a#skinr-lite-export-colors').click(function() {
        var scheme = Drupal.settings.skinrLiteColors.current;
        var results = '';
        $.each(scheme, function(k, color) {
          results = results + "    '" + k + "' => '" + $('#palette input[name="palette[' + k + ']"]').val() + "',\n";
        });
        alert(results);
      });
    }
  };

  Drupal.color = {
    callback: function(context, settings, form, farb, height, width) {
      
      var palette = Drupal.settings.skinrLiteColors.palette;
      var nonpalette = Drupal.settings.skinrLiteColors.nonpalette;
      var styles = Drupal.settings.skinrLiteColors.styles;
      var newColor, selectedColor, searchColor;
      
      /**
       * Copied from Drupal.behaviors.color attach (modules/color/color.js) - can we just use that function?
       */
      function shift_color(given, ref1, ref2) {
        // Convert to HSL.
        given = farb.RGBToHSL(farb.unpack(given));

        // Hue: apply delta.
        given[0] += ref2[0] - ref1[0];

        // Saturation: interpolate.
        if (ref1[1] == 0 || ref2[1] == 0) {
          given[1] = ref2[1];
        }
        else {
          var d = ref1[1] / ref2[1];
          if (d > 1) {
            given[1] /= d;
          }
          else {
            given[1] = 1 - (1 - given[1]) * d;
          }
        }

        // Luminance: interpolate.
        if (ref1[2] == 0 || ref2[2] == 0) {
          given[2] = ref2[2];
        }
        else {
          var d = ref1[2] / ref2[2];
          if (d > 1) {
            given[2] /= d;
          }
          else {
            given[2] = 1 - (1 - given[2]) * d;
          }
        }

        return farb.pack(farb.HSLToRGB(given));
      }
      
      $.each(palette, function(k, color) {
        newColor = $('#palette input[name="palette[' + k + ']"]', form).val();
        searchColor = RegExp(color, 'gi');
        styles = styles.replace(searchColor, newColor);
      });
      
      $.each(nonpalette, function(color, k) {
        selectedColor = $('#palette input[name="palette[' + k + ']"]', form).val();
        newColor = shift_color(selectedColor, settings.color.reference[k], farb.RGBToHSL(farb.unpack(color)));
        searchColor = RegExp(color, 'gi');
        styles = styles.replace(searchColor, newColor);
      });
      
      $('#skinr-lite-colors-styles').html(styles);
      
    }
  };
  
})(jQuery);
