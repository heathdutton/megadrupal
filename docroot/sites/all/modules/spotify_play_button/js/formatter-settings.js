(function ($) {
  
    Drupal.spotifyPlayButton = Drupal.spotifyPlayButton || {};
    
    var $width;
    var $height;
    
    // Width has been changed, so update height accordingly
    Drupal.spotifyPlayButton.calculateHeight = function () {
        var value = Drupal.spotifyPlayButton.ensureDimension($width.val(), 'width');
        $width.val(value);
        // Only change the width value if it's enabled
        if ($height.is(":enabled")) {
            $height.val(value + Drupal.settings.spotifyPlayButton.heightDiff);
        }
        
    }
    
    // Height has been changed, so update width accordingly
    Drupal.spotifyPlayButton.calculateWidth = function () {
        var value = Drupal.spotifyPlayButton.ensureDimension($height.val(), 'height');
        $height.val(value);
        $width.val(value - Drupal.settings.spotifyPlayButton.heightDiff);
    }
    
    // Make sure width / height isn't bigger than the maximum permitted dimensions
    Drupal.spotifyPlayButton.ensureDimension = function (value, type) {
        switch (type) {
        case 'width':
            var min = Drupal.settings.spotifyPlayButton.minWidth;
            var max = Drupal.settings.spotifyPlayButton.maxWidth;
            break;
        case 'height':
            var min = Drupal.settings.spotifyPlayButton.minWidth + Drupal.settings.spotifyPlayButton.heightDiff;
            var max = Drupal.settings.spotifyPlayButton.maxWidth + Drupal.settings.spotifyPlayButton.heightDiff;
            break;
        }
        if (value < min) {
            value = min;
        } else if (value > max) {
            value = max;
        }
        return parseInt(value);
    }
    
    Drupal.behaviors.suggest = {
        attach: function (context) {
            $width = $('input.richseam-play-width');
            $height = $('input.richseam-play-height');
            
            $width.change(function () {
                Drupal.spotifyPlayButton.calculateHeight();
            });
            
            $height.change(function () {
                Drupal.spotifyPlayButton.calculateWidth();
            });
        }
    };
})(jQuery);