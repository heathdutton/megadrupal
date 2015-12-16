(function ($) {
  Drupal.behaviors.viewsSlideshowj360 = {
      attach: function (context, settings) {
        for (id in Drupal.settings.viewsSlideshowj360) {
          $('#' + id + ':not(.viewsSlideshowj360-processed)', context)
          .addClass('viewsSlideshowj360-processed')
          .each(function () {
            // get settings
            var settings = Drupal.settings.viewsSlideshowj360[$(this).attr('id')];
            var w = '0';
            var h;

            // get original width & height from the view-saved informations
            var origWidth = $('input#j360-img-width').val();
            var origHeight = $('input#j360-img-height').val();

            if (settings['source_type'] == 'image-array') {
              // automatically img identification as a child of the invoked element
              var imageArray = [];
              var i = 0;
              $('img', $(this)).each(function() {
                // add the image src to the array of images to be processed
                imageArray.push($(this).attr('src'));
                i++;
              });

              // Add base settings
              w = origWidth;
              h = origHeight;
              settings['frames'] = i;
              //on spritespin stable (but old)
              settings['image'] = imageArray;
              //on spritespin dev
              settings['source'] = imageArray;
              settings['frameStepX'] = undefined;
              settings['framesX'] = undefined;

              settings['module'] = "360";
            }
            else {
              var image = $('img', $(this)).attr('src');
              settings['image'] = image;
              // on spritespin dev
              settings['source'] = image;

              switch (settings['source_type']) {
                case 'inline':
                  settings['resolutionX'] = origWidth;
                  settings['resolutionY'] = origHeight;
                  h = settings['resolutionY'];
                  w = settings['resolutionX'] / settings['frames'];
                  settings['module'] = "360";
                  // Some values MUST BE set to undefined
                  settings['frameStepX'] = undefined;
                  settings['framesX'] = undefined;

                  break;

                case 'grid':
                  settings['resolutionX'] = origWidth;
                  settings['resolutionY'] = origHeight;
                  w = settings['resolutionX'] / settings['framesX'];
                  h = settings['resolutionY'] / settings['framesX'];
                  // MUST BE ZERO HERE!
                  settings['frameStepX'] = 0;
                  settings['module'] = "360";

                  break;

                case 'panorama':
                  h = parseInt(origHeight);
                  w = (settings['maxwidth'] > 0) ? w = settings['maxwidth'] : parseInt(origWidth) / 5;

                  settings['frames'] = parseInt(origWidth);
                  //start from the first frame
                  settings['frame'] = 0;
                  settings['panorama'] = 'true';
                  settings['module'] = "panorama";
                  //rotation inverted
                  settings['sense'] = -1;
                  settings['frameStepX'] = (settings['frameStepX'] > 0) ? settings['frameStepX'] : 1;

                  break;
              }
            }

            settings['width'] = w;
            settings['height'] = h;
            settings['frameWrap'] = true;
            settings['enableCanvas'] = true;
            settings['behavior'] = "drag";
            settings['frameStep'] = 1;
            settings['frameTime'] = 60;
            //settings['sense'] = -1; //rotation inverted

            if (parseInt(w) > 0) {
              // Fire up spritespin
              $('#' + id).spritespin(settings);
            }
          });
        }
      }
  };
}(jQuery));
