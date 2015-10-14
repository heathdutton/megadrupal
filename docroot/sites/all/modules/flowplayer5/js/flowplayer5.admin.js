/**
 * @file
 * Provides the Drupal behavior for the Flowplayer administration page.
 */

/**
 * The Flowplayer5 Drupal administration behavior.
 */
(function($) {
  Drupal.behaviors.flowplayer5admin = {
    attach: function(context, settings) {
      //get object 
      var o = this;
      // Root preview wrapper.
      var root = $('#flowplayer5-preview');
      // Load flowplayer5 object;
      var fl = Drupal.behaviors.flowplayer5;

      // Trigger on checkboxes controlls.
      $('#edit-flowplayer5-conts input[type=checkbox]').change(function() {
        var n = $(this).attr('name').match(/\[(.*?)\]/);
        if ($(this).is(':checked')) {
          fl.updatePlayerPreviewControls(n[1], $(this).val(), root);
        } else {
          fl.updatePlayerPreviewControls(n[1], 0, root);
        }
      });

      // Update player preview onload.
      o.updatePlayerPreview();

      // Create the Farbtastic color picker
      settings.flowplayer5AdminFarbtastic = $.farbtastic('#flowplayer5-color-picker', function(color) {
        $(settings.flowplayer5AdminTextbox).val(color);
        o.updateTextBox(color, settings.flowplayer5AdminTextbox);
      });

      // Make the focus of the textbox change the input box we're acting on.
      $('#flowplayer5-color input:text', context).focus(function() {
        settings.flowplayer5AdminTextbox = this;
      });

      // Colour the text boxes their value color.
      $('#flowplayer5-color input:text').each(function(index, object) {
        var value = $(object).val();
        if (value) {
          o.updateTextBox($(object).val(), object);
        }
      });
      // On variations change
      $('#edit-flowplayer5-variation').change(function() {
        o.updateSettings($(this).val());

        // Remove other styles class if there any.
        root.removeClass('minimalist functional playful')

        // Add select variation class.
        root.addClass($(this).val());

      });

    },
    // Update both the Flowplayer preview and the textbox background whenever a textbox gets changed.  
    updateTextBox: function(color, object) {
      // Update player colors.
      var fl = Drupal.behaviors.flowplayer5;
      fl.updatePlayerPreviewColors($(object).attr('rel'), color);

      $(object).css({
        'backgroundColor': color,
        'color': Drupal.settings.flowplayer5AdminFarbtastic.RGBToHSL(Drupal.settings.flowplayer5AdminFarbtastic.unpack(color))[2] > 0.5 ? '#000' : '#fff'
      });
      var target = $(object).attr('rel');
      return;
      var player = $('.flowplayer5-preview');
      if (player) {
        player.getControls().css(target, color);
      }
    },
    // Update all admin configurations on sample varitions
    updateSettings: function(vri) {
      //get object 
      var o = this;
      // Get settings 
      var sts = Drupal.settings.flowplayer5.settings;
      if (sts[vri]) {

        var chs = sts[vri]['check'];
        for (var ch in chs) {
          //   alert($('input[type=checkbox][name="flowplayer5_conts['+ch+']"]').val());
          if (chs[ch]) {
            $('input[type=checkbox][name="flowplayer5_conts[' + ch + ']"]').prop('checked', true);
          } else {
            $('input[type=checkbox][name="flowplayer5_conts[' + ch + ']"]').removeAttr('checked');
          }
        }
        var ips = sts[vri]['input'];
        for (var ip in ips) {
          $('input[type=text][name="flowplayer5_color_' + ip + '"]').val(ips[ip]);
          o.updateTextBox(ips[ip], $('input[type=text][name="flowplayer5_color_' + ip + '"]'));
        }
      }
    },
    updatePlayerPreview: function() {
      // Update controls for player preview.
      var o = this;
      var fl = Drupal.behaviors.flowplayer5;
      var cns = $('#edit-flowplayer5-conts input[type=checkbox]');
      // Root preview wrapper.
      var root = $('#flowplayer5-preview');

      $.each(cns, function() {
        var n = $(this).attr('name').match(/\[(.*?)\]/);
        if ($(this).is(':checked')) {
          fl.updatePlayerPreviewControls(n[1], $(this).val(), root);
        } else {
          fl.updatePlayerPreviewControls(n[1], 0, root);
        }

      });

      // Get input values.
      var civ = $('#flowplayer5-color input[type=text]');
      $.each(civ, function() {
        fl.updatePlayerPreviewColors($(this).attr('rel'), $(this).val());
      });

    }
  };
})(jQuery);