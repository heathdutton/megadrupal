/**
 * @file
 * Fullscreen gallery module jQuery functions.
 *
 * File has been checked with use strict, jsLint, jsHint.
 */

(function ($) {
  "use strict";
  Drupal.behaviors.fullscreen_gallery = {
    attach: function (context, settings) {
      var windowWidth, windowHeight;
      var galleryTopHeight, imageTitleHeight, galleryContentPadding, loadingSizeHalf;
      var imgHeight, imgWidth, imgMaxHeight, imgMaxWidth;
      var thumbnailsWidth, thumbnailScrolledTo, thumbnailsNavAnimDur, thumbnailsMarginLeft, thumbnailsArrowsWidth, thumbsMaxWidth;
      var clickEnabled;
      var thumbnail_offset;
      var timer;
      var $fullscreen_gallery = $('#fullscreen_gallery');
      var $thumbnails = $('div.thumbnails');
      var $thumbnailsImages = $('div.thumbnails-images');
      var $prevButton = $('div.prev-button');
      var $nextButton = $('div.next-button');
      var $backButton = $('div.back-button');

      setVariables();
      resize_thumbnails_block();
      init_fullscreen_gallery();
      /**
       * Set initial values for predefined main variables.
       */
      function setVariables() {
        // Handle right sidebar displaying if needed.
        if (settings.fullscreen_gallery.rs_width) {
          set_right_sidebar(settings.fullscreen_gallery.rs_width, settings.fullscreen_gallery.rs_width_type);
        }

        // Get browser window parameters.
        windowWidth = $(window).width();
        windowHeight = $(window).height();

        // Get gallery header height.
        galleryTopHeight = parseInt($fullscreen_gallery.find('div.gallery-top').outerHeight(), 10);

        // Handle image title area displaying.
        if ($fullscreen_gallery.find('div.image-title').html().trim() == '') {
          // No image title available, hide image title area.
          imageTitleHeight = 0;
          $fullscreen_gallery.find('div.image-title').css('padding', '0px');
          $fullscreen_gallery.find('div.image-title').parent().addClass('no-title');
        }
        else {
          // Get the image title area real height.
          imageTitleHeight = parseInt($fullscreen_gallery.find('div.image-title').outerHeight(), 10);
        }

        // Get available area for displaying big image.
        galleryContentPadding = parseInt($fullscreen_gallery.find('div.gallery-content').css('padding-top'), 10) + parseInt($fullscreen_gallery.find('div.gallery-content').css('padding-bottom'), 10);
        imgMaxHeight = windowHeight - galleryTopHeight - imageTitleHeight - galleryContentPadding;
        imgMaxWidth = parseInt($fullscreen_gallery.find('div.cinner').width(), 10);

        // Set thumbnails parameters.
        thumbnailsWidth = 0;
        thumbnailScrolledTo = 0;
        thumbnailsNavAnimDur = 150;
        thumbnailsMarginLeft = parseInt($thumbnailsImages.css('margin-left').replace('px', ''), 10);

        loadingSizeHalf = parseInt(parseInt($fullscreen_gallery.find('#fs_loading').css('height').replace('px', ''), 10) / 2, 10);
        clickEnabled = true;
        thumbnail_offset = getURLParameter('thumbnail_offset');
      }

      /**
       * Init gallery. Runs on document load, and every time when browser window area has been resized.
       */
      function init_fullscreen_gallery() {
        // Calculate and set the actual image style.
        set_calculated_image_style();

        // Set gallery dimensions.
        $fullscreen_gallery.css('height', windowHeight + 'px');
        $fullscreen_gallery.css('overflow', 'hidden');
        $fullscreen_gallery.find('div.gallery-right').css('height', (windowHeight - galleryContentPadding - galleryTopHeight) + 'px');
        $fullscreen_gallery.find('div.gallery-left div.current-image div.image-title').hide();

        // Calculate and set the loading animation position.
        $fullscreen_gallery.find('#fs_loading').css('margin-left', (parseInt(imgMaxWidth / 2, 10) - loadingSizeHalf) + 'px');
        $fullscreen_gallery.find('#fs_loading').css('margin-top', (parseInt(imgMaxHeight / 2, 10) - loadingSizeHalf) + 'px');
        $fullscreen_gallery.find('#fs_loading').show();

        $fullscreen_gallery.find('div.gallery-left div.current-image div.img-container img').load(function () {
          // Actual image has been loaded, set image, navigation and image title dimensions.
          imageLoaded();
        }).each(function () {
          if (this.complete) {
            $(this).load();
          }
        });
      }

      /**
       * Set the most appropriate image style by the maximum height of image display area.
       */
      function set_calculated_image_style() {
        var diff = 999999;
        var new_diff, selected_style;
        if (typeof settings.fullscreen_gallery === 'object' && Object.keys(settings.fullscreen_gallery.styles).length) {
          // Set initial diff to very high value.
          $.each(settings.fullscreen_gallery.styles, function (index, value) {
            // Calculate difference in pixels between actual image style height and the maximum height of image display area.
            new_diff = value - imgMaxHeight;
            if (new_diff < diff && new_diff >= 0) {
              // The calculated difference is smaller, than the previous (and still positive).
              // Set the actual style as the most appropriate so far.
              diff = new_diff;
              selected_style = index;
            }
          });

          // Set the image source according to most appropriate image style.
          $fullscreen_gallery.find('div.current-image img').attr('src', settings.fullscreen_gallery.style_urls[selected_style]);
        }
      }

      /**
       * Handle right sidebar displaying by setting the given width parameter.
       *
       * @param {int} width
       *   The right sidebars width.
       * @param {string} type
       *   The right sidebars width unit. Possible values: px (pixel), pe (percent)
       */
      function set_right_sidebar(width, type) {
        switch (type) {
          case 'px':
            $fullscreen_gallery.find('div.gallery-left div.main-content').css('margin-right', width + 'px');
            $fullscreen_gallery.find('div.gallery-right').css('margin-left', '-' + width + 'px');
            $fullscreen_gallery.find('div.gallery-right').css('width', width + 'px');
            break;

          case 'pe':
            $fullscreen_gallery.find('div.gallery-left').css('width', (100 - width) + '%');
            $fullscreen_gallery.find('div.gallery-left div.main-content').css('margin-right', '0px');
            $fullscreen_gallery.find('div.gallery-right').css('margin-left', '0px');
            $fullscreen_gallery.find('div.gallery-right').css('width', width + '%');
            break;
        }
      }

      /**
       * The actual image has been loaded, set image, navigation and image title dimensions according to the image dimensions.
       */
      function imageLoaded() {
        // Display actual image height to maximum available height, and set width by aspect ratio (css: auto).
        $fullscreen_gallery.find('div.current-image img').css('height', imgMaxHeight + 'px');
        $fullscreen_gallery.find('div.current-image img').css('width', 'auto');

        imgWidth = $fullscreen_gallery.find('div.current-image img').width();
        if (imgWidth > imgMaxWidth) {
          // The image width is bigger than the maximum available width, so the image
          // should be displayed based on the maximum available width, and set height by aspect ratio (css: auto).
          $fullscreen_gallery.find('div.gallery-left div.current-image img').css('width', imgMaxWidth + 'px');
          $fullscreen_gallery.find('div.gallery-left div.current-image img').css('height', 'auto');
        }

        // Get the new image dimensions after resizing definitions before.
        imgWidth = $fullscreen_gallery.find('div.current-image img').width();
        imgHeight = $fullscreen_gallery.find('div.current-image img').height();

        // Set the image vertically centered.
        var imgPaddingTop = parseInt((imgMaxHeight - imgHeight) / 2, 10);
        $fullscreen_gallery.find('div.current-image').css('padding-top', imgPaddingTop + 'px');

        // Display the image with fadeIn effect, and set image container width.
        $fullscreen_gallery.find('div.gallery-left div.current-image img').fadeIn().css("display", "block");
        $fullscreen_gallery.find('div.current-image div.cinner').css('width', imgWidth + 'px');

        // Set navigation and navigation button dimensions.
        $fullscreen_gallery.find('div.nav').css('width', $fullscreen_gallery.find('div.current-image').width() + 'px');

        var buttonWidth = (parseInt(imgMaxWidth / 2, 10) - 14) + 'px';
        var buttonHeight = (imgMaxHeight + imageTitleHeight) + 'px';
        $prevButton.css('width', buttonWidth).css('height', buttonHeight);
        $nextButton.css('width', buttonWidth).css('height', buttonHeight);
        $prevButton.find('a').css('width', buttonWidth).css('height', buttonHeight).css('display', 'block');
        $nextButton.find('a').css('width', buttonWidth).css('height', buttonHeight).css('display', 'block');

        // Hide or show image title area depending on whether it exists.
        if ($fullscreen_gallery.find('div.image-title').html() == '') {
          $fullscreen_gallery.find('div.image-title').hide();
        }
        else {
          $fullscreen_gallery.find('div.gallery-left div.current-image div.image-title').fadeIn();
        }

        // Show the navigation buttons with fadeIn effect.
        $prevButton.find('a').fadeIn();
        $nextButton.find('a').fadeIn();
        // Hide the loading animation with fadeOut effect.
        $fullscreen_gallery.find('#fs_loading').fadeOut();
      }

      /**
       * Set thumbnails block width and position.
       */
      function resize_thumbnails_block() {
        var img_count = $thumbnails.find('div.thumbnails-inner img').length;
        var loaded_img_count = 0;
        $thumbnails.find('div.thumbnails-inner img').load(function () {
          loaded_img_count++;
          // All thumbnail images has been loaded.
          if (loaded_img_count >= img_count) {
            // Calculate thumbnail images width.
            thumbnailsWidth = 0;
            if ($thumbnailsImages.find('div.thumbnail-item').length) {
              $thumbnailsImages.find('div.thumbnail-item').each(function () {
                thumbnailsWidth += $(this).outerWidth();
              });
            }

            // Calculate thumbnail images maximum width.
            thumbnailsArrowsWidth = $thumbnails.find('div.left').outerWidth() + $thumbnails.find('div.right').outerWidth();
            thumbsMaxWidth = $fullscreen_gallery.find('div.gallery-top div.gallery-title div.ginner').width() - thumbnailsArrowsWidth;
            if (thumbnailsWidth <= (thumbsMaxWidth + thumbnailsArrowsWidth)) {
              // Thumbnails navigation not needed.
              $thumbnails.find('div.left').hide();
              $thumbnails.find('div.right').hide();
              $thumbnails.css('width', thumbnailsWidth + 'px');
            }
            else {
              // Set thumbnails navigation is necessary, because thumbnails width is bigger than available area.
              $thumbnails.find('div.left').show();
              $thumbnails.find('div.right').show();

              // Calculate the width of images, which could be displayed in available area.
              var calculatedThumbnailsWidth = 0;
              $($thumbnailsImages.find('div.thumbnail-item').get().reverse()).each(function () {
                if (calculatedThumbnailsWidth + $(this).outerWidth() < thumbsMaxWidth) {
                  calculatedThumbnailsWidth += $(this).outerWidth();
                }
              });
              // Set the thumbnails block width.
              $thumbnails.find('div.thumbnails-inner').css('width', calculatedThumbnailsWidth + 'px');
              $thumbnails.css('width', (calculatedThumbnailsWidth + thumbnailsArrowsWidth) + 'px');
            }

            // If thumbnails has been stepped before, applicate it.
            if (thumbnail_offset) {
              for (var i = 0; i < (thumbnail_offset - thumbnailScrolledTo); i++) {
                stepThumbnailsRight(0);
              }
            }
          }
        }).each(function () {
          if (this.complete) {
            $(this).load();
          }
        });
      }

      /**
       * Moves thumbnail images in thumbnails block to the left direction by one image.
       *
       * @param {int} dur
       *   The animation duration in milliseconds
       */
      function stepThumbnailsLeft(dur) {
        // Get the leftmost image width and the thumbnails block width.
        var scrollWidth = $thumbnailsImages.find('div.thumbnail-item:eq(' + (thumbnailScrolledTo - 1) + ')').outerWidth();

        // Moving of images is enabled only when previous steppings are finished, and hidden images exists on the left side.
        if (clickEnabled && thumbnailsMarginLeft < 0) {
          // Disable thumbnail stepping function.
          clickEnabled = false;

          // Step thumbnails block by animating margin-left parameter.
          $thumbnailsImages.animate({
            'margin-left': '+=' + scrollWidth + 'px'
          }, dur, function () {
            // After moving items enable thumbnail stepping function, and update actual thumbnail parameters.
            clickEnabled = true;
            thumbnailScrolledTo = thumbnailScrolledTo - 1;
            thumbnailsMarginLeft = thumbnailsMarginLeft + scrollWidth;
            if (parseInt($thumbnailsImages.css('margin-left').replace('px', ''), 10) == 0) {
              $thumbnails.find('div.left').addClass('inactive');
            }
            $thumbnails.find('div.right').removeClass('inactive');
          });
        }
      }

      /**
       * Moves thumbnail images in thumbnails block to the right direction by one image.
       *
       * @param {int} dur
       *   The animation duration in milliseconds
       */
      function stepThumbnailsRight(dur) {
        // Get the rightmost image width and the thumbnails block width.
        var scrollWidth = $thumbnailsImages.find('div.thumbnail-item:eq(' + thumbnailScrolledTo + ')').outerWidth();
        var tiWidth = parseInt($thumbnails.find('div.thumbnails-inner').css('width').replace('px', ''), 10);

        // Moving of images is enabled only when previous steppings are finished, and hidden images exists on the right side.
        if (clickEnabled && thumbnailsMarginLeft * (-1) < thumbnailsWidth - tiWidth) {
          // Disable thumbnail stepping function.
          clickEnabled = false;

          // Step thumbnails block by animating margin-left parameter.
          $thumbnailsImages.animate({
            'margin-left': '-=' + scrollWidth + 'px'
          }, dur, function () {
            // After moving items enable thumbnail stepping function, and update actual thumbnail parameters.
            clickEnabled = true;
            thumbnailScrolledTo = thumbnailScrolledTo + 1;
            thumbnailsMarginLeft = thumbnailsMarginLeft - scrollWidth;
            if ((-1) * parseInt($thumbnailsImages.css('margin-left').replace('px', ''), 10) >= thumbnailsWidth - tiWidth) {
              $thumbnails.find('div.right').addClass('inactive');
            }
            $thumbnails.find('div.left').removeClass('inactive');
          });
        }
      }

      /**
       * Moves thumbnail images the the leftmost thumbnail image.
       */
      function stepToFirst() {
        $thumbnailsImages.css('margin-left', '0px');
        $thumbnails.find('div.left').addClass('inactive');
        $thumbnails.find('div.right').removeClass('inactive');
        thumbnailScrolledTo = 0;
        thumbnailsMarginLeft = 0;
      }

      // Thumbnail item or main image navigation item clicked.
      $('div.thumbnail-item a, div.nav a').click(function () {
        $fullscreen_gallery.find('#fs_loading').show();
        if ($(this).attr('href').indexOf("?") > -1) {
          $(this).attr('href', $(this).attr('href') + '&thumbnail_offset=' + thumbnailScrolledTo);
        }
        else {
          $(this).attr('href', $(this).attr('href') + '?thumbnail_offset=' + thumbnailScrolledTo);
        }
      });

      // Left thumbnail navigation item clicked.
      $thumbnails.find('div.left a').click(function () {
        stepThumbnailsLeft(thumbnailsNavAnimDur);
        return false;
      });

      // Right thumbnail navigation item clicked.
      $thumbnails.find('div.right a').click(function () {
        stepThumbnailsRight(thumbnailsNavAnimDur);
        return false;
      });

      // Gallery close button clicked.
      $backButton.click(function () {
        // Remove first two elements.
        var url_parts = location.pathname.split("/").splice(2, 4);

        // Add anchor to current image in href.
        var old_href = decodeURIComponent($(this).attr('href'));
        $(this).attr('href', old_href + '#fs_gallery_item_' + url_parts.join('_'));
      });

      // Navigation by keyboard.
      $(document.documentElement).keyup(function (e) {
        // Keyboards left key.
        if (e.keyCode == 37) {
          $prevButton.find('a')[0].click();
        }

        // Keyboards right key.
        if (e.keyCode == 39) {
          $nextButton.find('a')[0].click();
        }

        // Keyboards escape key.
        if (e.keyCode == 27) {
          $backButton.find('a')[0].click();
        }
      });

      // Browser window has been resized.
      $(window).bind('resize', function () {
        // Set resizing timer. Without timer manual "drag" resizing causes a lot of functions call needlessly.
        timer = window.setTimeout(function () {
          // Set gallery width to auto, and reinitialize the whole gallery display and calculate process.
          $fullscreen_gallery.find('div.cinner').css('width', 'auto');
          setVariables();
          init_fullscreen_gallery();
          resize_thumbnails_block();
          stepToFirst();
          window.clearTimeout(timer);
        }, 1000);
      });

      // The page and media items are fully loaded.
      $(window).load(function () {
        // Image dimensions are available only after loading of all objects.
        resize_thumbnails_block();
      });

      /**
       * Returns the url parameter according to given variable name.
       *
       * @param {string} name
       *   The url parameter name.
       *
       * @return {string}
       *   The related value if exists, empty string otherwise.
       */
      function getURLParameter(name) {
        return decodeURIComponent((new RegExp('[?|&]' + name + '=' + '([^&;]+?)(&|#|;|$)').exec(location.search)||[,""])[1].replace(/\+/g, '%20')) || null;
      }
    }
  };
})(jQuery);
