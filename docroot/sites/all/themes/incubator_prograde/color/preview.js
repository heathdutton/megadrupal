// Handle the color changes and update the preview window.
(function ($) {
  Drupal.color = {
    logoChanged: false,
    callback: function(context, settings, form, farb, height, width) {
      var color = '#ffffff';
      
      // Titles
      // -----------------------------------------------------------------------
        $('#preview h2', form).css('color', $('#palette input[name="palette[title]"]', form).val());
        $('#preview h3', form).css('color', $('#palette input[name="palette[title]"]', form).val());
      
      // Background
      // -----------------------------------------------------------------------
        $('#preview', form).css('background-color', $('#palette input[name="palette[bg]"]', form).val());
        $('#preview-main', form).css('background-color', $('#palette input[name="palette[bg]"]', form).val());
      
      // Text
      // -----------------------------------------------------------------------
        $('#preview', form).css('color', $('#palette input[name="palette[text]"]', form).val());
      
      // Links
      // -----------------------------------------------------------------------
        $('#preview a:link', form).css('color', $('#palette input[name="palette[link]"]', form).val());
      
      // Base color
      // and Text on Color
      // -----------------------------------------------------------------------
        color = $('#palette input[name="palette[base]"]', form).val();
        text_highlight = $('#palette input[name="palette[text_highlight]"]', form).val();
      
        // User zone
        $('#preview .zone-user-wrapper', form).css('background-color', color).css('color', text_highlight);
        $('#preview .zone-user-wrapper a:link', form).css('color', text_highlight);
      
        // Titles with Backgrounds
        $('#preview .block h2.block-title', form).css('background-color', color).css('color', text_highlight);
        $('#preview .node-teaser h2.node-title', form).css('background-color', color).css('color', text_highlight);
      
        // Main menu
        $('#preview .navigation', form).css('border-bottom', '2px solid ' + color);
        $('#preview .navigation li.active', form).css('background-color', color);
        $('#preview .navigation li.active a:link', form).css('color', text_highlight);
        
      
      // Secondary color
      // -----------------------------------------------------------------------
      
      
      // Blocks and borders
      // -----------------------------------------------------------------------
        var block_bg = $('select[name="skins[incubator_prograde_block_bg]"]').val();
        var border = $('#palette input[name="palette[block]"]', form).val();
        color = border;
        if (block_bg === 'pagebg') {
          color = $('#palette input[name="palette[bg]"]', form).val();
        }
        else if (block_bg === 'white') {
          color = '#ffffff';
        }
        $('#preview .block', form).css('background-color', color).css('border-color', border);
        $('#preview .section-footer', form).css('background-color', color).css('border-color', border);
        $('#preview .node-teaser', form).css('background-color', color).css('border-color', border);
      
      
    }
  };
  Drupal.behaviors.exportColorscheme = {
    attach: function (context, settings) {
      $('#preview a.export-colors').click(function() {
        var colorscheme = '';
        var palette = $('#palette input[type=text]');
        palette.each(function() {
          colorscheme = colorscheme + "    '" + this.name.replace('palette[', '').replace(']', '') + "' => '" + $(this).val() + "',\n";
        });
        alert(colorscheme);
        return false;
      });
    }
  };
  Drupal.behaviors.sitenameSkinrColors = {
    attach: function (context, settings) {
      $('select[name="skins[incubator_prograde_block_bg]"]').change(function() {
        var value = $(this).val();
        var color = $('#palette input[name="palette[block]"]').val();
        if (value === 'pagebg') {
          color = $('#palette input[name="palette[bg]"]').val();
        }
        else if (value === 'white') {
          color = '#ffffff';
        }
        $('#preview .block').css('background-color', color);
        $('#preview .section-footer').css('background-color', color);
        $('#preview .node-teaser').css('background-color', color);
      });
    }
  };
  
})(jQuery);
