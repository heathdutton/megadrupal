(function($) {

  Drupal.behaviors.closeblock = {

    attach: function (context, settings) {
      
      if (settings.closeblock == undefined) {
        return;
      }
      
      $.each(settings.closeblock, function(id, info) {
        $('#'+ id  + ':not(.closeblock-processed)', context).addClass('closeblock-processed').each(function() {
          var
            $block = $(this);
            
          if (info.closed) {
            $block.hide();
            return;
          }
          
          var
            $close_buttton = $('<span />').addClass('closeblock-button').html('close'),
            $close_contaier = $('<div />').addClass('closeblock').append($close_buttton);
          
          $block.prepend($close_contaier);
          
          $close_buttton.click(function() {
            if (info.type) {
              $block[info.type](info.speed);
            }
            else {
              $block.hide();
            }
            if (info.save) {
              var
               _path = Drupal.settings.basePath + ['closeblock', info.module, info.delta].join('/');
              $.post(_path);
            }
          });
        });
      });
      
    }

  };
  
})(jQuery);
