
(function ($) {
  Drupal.color = {
    logoChanged: false,
    callback: function(context, settings, form, farb, height, width) {

      function hextorgb(hex) {
          // Expand shorthand form (e.g. "03F") to full form (e.g. "0033FF")
          var shorthandRegex = /^#?([a-f\d])([a-f\d])([a-f\d])$/i;
          hex = hex.replace(shorthandRegex, function(m, r, g, b) {
              return r + r + g + g + b + b;
          });

          var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
          return result ? 
              parseInt(result[1], 16)+","+
              parseInt(result[2], 16)+","+
              parseInt(result[3], 16)
          : null;
      }
      
      // Change the logo to be the real one.
      if (!this.logoChanged) {
        $('#preview nav #home-link-img').attr('src', Drupal.settings.color.logo);
        this.logoChanged = true;
      }
      // Remove the logo if the setting is toggled off. 
      if (Drupal.settings.color.logo == null) {
        $('#navbar-nav').remove('#home-link');
      }

      $('#edit-regions input').change(function() {
        for (var i = 0; i < Drupal.settings.theme_regions.length; i++) {
          var region = Drupal.settings.theme_regions[i];
          var opacity = parseFloat($('input[name="region_background_opacity-'+region+'"]').val());
          if(opacity < 0 || opacity > 1 || opacity == "" || isNaN(opacity)) {
               opacity = 1;  
          }
          $('#preview #preview-'+region, form).css('background', 'rgba('+hextorgb($('#palette input[name="palette['+region+'bg]"]', form).val())+','+opacity+')');
        }
      });

      for (var i = 0; i < Drupal.settings.theme_regions.length; i++) {
        var region = Drupal.settings.theme_regions[i];
        var opacity = parseFloat($('input[name="region_background_opacity-'+region+'"]').val());
       
        if(opacity < 0 || opacity > 1 || opacity == "" || isNaN(opacity)) {
             opacity = 1;  
        }

        $('#preview #preview-'+region, form).css('background', 'rgba('+hextorgb($('#palette input[name="palette['+region+'bg]"]', form).val())+','+opacity+')');
        $('#preview #preview-'+region, form).css('color', $('#palette input[name="palette['+region+'txt]"]', form).val());
        $('#preview #preview-'+region+' a', form).css('color', $('#palette input[name="palette['+region+'links]"]', form).val());

        $('#preview #preview-'+region+' a', form).hover(function() {
          $(this).css('color', $('#palette input[name="palette['+region+'hover]"]', form).val());
        },function(){
          $(this).css("color", $('#palette input[name="palette['+region+'links]"]', form).val());
        });
      }

    }
  };
})(jQuery);
