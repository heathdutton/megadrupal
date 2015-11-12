/**
 * @file
 * Javascript functionality for the image cropper widget.
 */

(function ($) {

  Drupal.behaviors.advanced_crop = {
    attach: function(context, settings) {
        var settings = settings.advanced_crop;
        if(settings === undefined) {
            Drupal.settings.advanced_crop = JSON.parse($('#image-cropper-image-info').val());
        }
        imageCropperInitCropFields();
        $('#image-cropper-preview-thumb').once('image-cropper-preview').click(function() {
            imageCropperInit($(this));
        });
    }
  };

    /**
     * Initializes hidden form fields.
     */
    function imageCropperInitCropFields() {
        var settings = Drupal.settings.advanced_crop;
        jQuery('input.image-cropper-crop-field').each(function() {
            var $field = $(this);

            if ($field.val() == '') {
                var aspectX = $field.attr('data-min-width') ? $field.attr('data-min-width') : 1;
                var aspectY = $field.attr('data-min-height') ? $field.attr('data-min-height') : 1;
                var aspectRatio = (aspectX && aspectY) ? aspectX / aspectY : '';

                if (aspectRatio >= 1) {
                    // Width is greater.
                    var width = settings.image.width;
                    var height = width / aspectRatio;
                }
                else if (aspectRatio < 1) {
                    // Height is greater.
                    var height = settings.image.height;
                    var width = height * aspectRatio;
                }

                if (height > settings.image.height) {
                    height = settings.image.height;
                    width = height * aspectRatio;
                }

                if (width > settings.image.width) {
                    width = settings.image.width;
                    height = width / aspectRatio;
                }

                var data = {
                    width: width,
                    height: height,
                    x: (settings.image.width - width) / 2,
                    y: (settings.image.height - height) / 2
                }

                $field.val(JSON.stringify(data));
                $field.attr('data-original-value', $field.val());
            }
        });
    }

    /**
     * Builds and displays the modal.
     */
    function imageCropperInit() {
        var settings = Drupal.settings.advanced_crop;
        var $modal = $('<div class="image-cropper-modal image-cropper-clearfix">');
        var $helpText = '<div class="advance-crop-help-text">Click and drag on the top thump to do the cropping.</div>';
        var $cropContainer = imageCropperCreateCropContainer();
        var $thumbs = imageCropperCreateThumbs();
        var $modalMask = $('<div class="image-cropper-modal-mask"></div>').css('height', $(window).height() + 'px');

        // Wait for the image to finish loading before doing anything
        // so that we can get the exact dimensions.
        $cropContainer.find('img').load(function() {
            $modal
                .append($helpText)
                .append($cropContainer)
                .append($thumbs)
                .append('\
              <div style="clear:left;" class="form-actions form-wrapper" id="edit-actions--2">\
                <input type="button" class="button-yes form-submit" id="image-cropper-ok-button" value="OK">\
                <input type="button" class="button button-no form-submit" id="image-cropper-cancel-button" value="Cancel">\
              </div>\
            ');

            $('body').append($modalMask).append($modal);

            $modal
                .css('max-width', $(window).width() - 100 + 'px')
                .css('max-height', $(window).height() - 100 + 'px')
                .css('left', ($(window).width() - $modal.width()) / 2 + 'px')
                .css('top', ($(window).height() - $modal.height()) / 2 + 'px')
                .css('overflow', 'scroll');

            var thumbCount = $thumbs.find('li').length;
            var maxThumbWidth = 200;
            if ((($modal.width() - 20) / thumbCount - 12) < maxThumbWidth) {
                maxThumbWidth = ($modal.width() - 20) / thumbCount - 12;
            }
            $thumbs.find('li').css('max-width', maxThumbWidth);

            // Set the max width and height for the modal window, leaving a 50px space around the modal.
            $(this)
                .css('max-height', $(window).height() - 100 - $thumbs.height() - 50 + 'px')
                .css('max-width', $(window).width() - 100 - 50 + 'px');

            $thumbs.css('margin-left', ($modal.width() - $thumbs.width()) / 2 + 'px');

            $cropContainer.find('.image-cropper-crop-container-inner')
                .css('margin-left', ($modal.width() - $cropContainer.find('img').width()) / 2 + 'px');

            imageCropperStyleThumbs();
            imageCropperShowThumb($thumbs.find('.image-cropper-thumb:first'));

            // OK button click handler.
            $('#image-cropper-ok-button').on('click', document, function() {
                // Updated saved values.
                $('input.image-cropper-crop-field').once('advanced_crop').each(function() {
                    $(this).attr('data-original-value', $(this).val());
                });
                // Close popup.
                $modal.remove();
                $modalMask.remove();
            });

            // Cancel button click handler.
            $('#image-cropper-cancel-button').on('click', document, function() {
                // Revert back saved values.
                $('input.image-cropper-crop-field').once('advanced_crop').each(function() {
                    $(this).val($(this).attr('data-original-value'));
                });
                // Close popup.
                $modal.remove();
                $modalMask.remove();
            });
        });
    }

    /**
     * Creates the cropping area for the modal.
     *
     * @see imageCropperInit()
     */
    function imageCropperCreateCropContainer() {
        var settings = Drupal.settings.advanced_crop;
        return $('<div class="image-cropper-crop-container image-cropper-clearfix">')
            .append(
            $('<div class="image-cropper-crop-container-inner image-cropper-clearfix">')
                .append($('<img>').attr('src', settings.image.url))
                .append($('<div class="image-cropper-cropped-image">'))
        );
    }

    /**
     * Creates the thumbs for the modal.
     *
     * @see imageCropperInit()
     */
    function imageCropperCreateThumbs() {
        var settings = Drupal.settings.advanced_crop;
        var $ul = $('<ul class="image-cropper-thumbs image-cropper-clearfix">');

        $('input.image-cropper-crop-field').each(function() {
            var $field = $(this);
            var $li = $('<li>')
                .append($('<div class="image-cropper-thumb-label-wrapper"><span class="image-cropper-thumb-label">' + $field.attr('data-label') + '</span></div>'))
                .append(
                $('<div class="image-cropper-thumb">')
                    .attr('data-id', $field.attr('id'))
                    .append($('<img src="' + settings.image.url + '">'))
                    .append($('<div class="image-cropper-cropped-thumb">'))
            );

            $ul.append($li);
        });

        return $ul;
    }

    /**
     * Styles the thumbs and binds click handler.
     *
     * @see imageCropperInit()
     * @see imageCropperCreateThumbs()
     */
    function imageCropperStyleThumbs() {
        var settings = Drupal.settings.advanced_crop;
        $('.image-cropper-thumb').each(function() {
            $thumbImg = $(this).find('img');
            var $croppedThumb = $(this).find('.image-cropper-cropped-thumb');
            var cropData = JSON.parse($('input#' + $(this).attr('data-id')).val());
            var scalePercent = $thumbImg.width() / settings.image.width;
            var ct = {
                width: cropData.width * scalePercent,
                height: cropData.height * scalePercent,
                left: cropData.x * scalePercent,
                top: cropData.y * scalePercent
            };

            $croppedThumb.css('width', ct.width + 'px')
                .css('height', ct.height + 'px')
                .css('left', ct.left + 'px')
                .css('top', ct.top + 'px')
                .css('background', 'url(' + $thumbImg.attr('src') + ') ' + (ct.left * -1) + 'px ' + (ct.top * -1) + 'px no-repeat')
                .css('background-size', $thumbImg.width() + 'px ' + $thumbImg.height() + 'px');

        }).on('click', document, function() {
            imageCropperShowThumb($(this));
        });
    }

    /**
     * Displays the selected thumb and binds crop handlers.
     *
     * @param JQuery Object $selectedThumb
     *
     * @see imageCropperStyleThumbs()
     */
    function imageCropperShowThumb($selectedThumb) {
        var settings = Drupal.settings.advanced_crop;
        // Show which thumbnail is selected, deselect the rest.
        $selectedThumb.parent().addClass('selected').siblings().each(function() {
            $(this).removeClass('selected');
        });

        var cropField = $('input#' + $selectedThumb.attr('data-id'));
        var cropData = JSON.parse(cropField.val());

        // Main cropping window, where the user will actually do the cropping.
        var $image = $('.image-cropper-crop-container-inner img');
        var $croppedImage = $('.image-cropper-crop-container-inner .image-cropper-cropped-image');

        // Cropped image thumbnail.
        var $croppedThumb = $selectedThumb.find('.image-cropper-cropped-thumb');

        // Get aspect ratio.
        var aspectX = cropField.attr('data-min-width');
        var aspectY = cropField.attr('data-min-height');
        var aspectRatio = (aspectX && aspectY) ? aspectX / aspectY : 1;

        var scaledPercent = $image.width() / settings.image.width;
        var scaledCropData = {
            width: cropData.width * scaledPercent,
            height: cropData.height * scaledPercent,
            x: cropData.x * scaledPercent,
            y: cropData.y * scaledPercent
        };

        $croppedImage
            .css('width', scaledCropData.width + 'px')
            .css('height', scaledCropData.height + 'px')
            .css('left', scaledCropData.x + 'px')
            .css('top', scaledCropData.y + 'px')
            .css('background', 'url(' + settings.image.url + ') ' + (scaledCropData.x * -1) + 'px ' + (scaledCropData.y * -1) + 'px no-repeat')
            .css('background-size', $image.width() + 'px ' + $image.height() + 'px')
            .draggable({
                containment: $image,
                drag: function(event, ui) {
                    // Move background of the crop window.
                    $(this)
                        .css('background', 'url(' + $image.attr('src') + ') ' + ((ui.position.left + 1) * -1) + 'px ' + ((ui.position.top + 1) * -1) + 'px no-repeat')
                        .css('background-size', $image.width() + 'px ' + $image.height() + 'px');

                    // Calculate position, convert to percentage.
                    var left = (ui.position.left) / $image.width();
                    var top = (ui.position.top) / $image.height();

                    // Figure out the actual values of the position, based on percentage we just got.
                    var ctLeft = $selectedThumb.width() * left;
                    var ctTop = $selectedThumb.height() * top;

                    // Apply those values to the CSS of cropped thumb.
                    var $croppedThumbImg = $selectedThumb.find('img');
                    $croppedThumb
                        .css('left', ctLeft + 'px')
                        .css('top', ctTop + 'px')
                        .css('background', 'url(' + $image.attr('src') + ') -' + (ctLeft + 1) + 'px -' + (ctTop + 1) + 'px no-repeat')
                        .css('background-size', $croppedThumbImg.width() + 'px ' + $croppedThumbImg.height() + 'px');

                    // Update the hidden input with the new position values.
                    cropData.x = settings.image.width * left;
                    cropData.y = settings.image.height * top;
                    cropField.val(JSON.stringify(cropData));
                }
            }).resizable({
                containment: $image,
                aspectRatio: aspectRatio,
                minWidth: cropField.attr('data-min-width') * scaledPercent,
                resize: function (event, ui) {
                    // Get percentage of width and height.
                    var width = (ui.size.width / $image.width());
                    var height = (ui.size.height / $image.height());

                    // Apply the CSS to cropped thumb.
                    $croppedThumb
                        .css('width', $selectedThumb.width() * width + 'px')
                        .css('height', $selectedThumb.height() * height + 'px');

                    // Update the hidden input with the new size values.
                    cropData.width = settings.image.width * width;
                    cropData.height = settings.image.height * height;
                    cropField.val(JSON.stringify(cropData));
                }
            });

        var ciPos = $croppedImage.position();

        // Get starting position of cropped thumb and its background.
        var ctPercentX = ciPos.left / $image.width();
        var ctPercentY = ciPos.top / $image.height();
        var ctPercentW = ($croppedImage.width() - 2) / $image.width();
        var ctPercentH = ($croppedImage.height() - 2) / $image.height();
        var ctStartX = $selectedThumb.width() * ctPercentX;
        var ctStartY = $selectedThumb.height() * ctPercentY;
        var ctStartW = $selectedThumb.width() * ctPercentW;
        var ctStartH = $selectedThumb.height() * ctPercentH;

        $croppedThumb
            .css('background', 'url(' + $image.attr('src') + ') ' + (ctStartX * -1) + 'px ' + (ctStartY * -1) + 'px no-repeat')
            .css('background-size', $selectedThumb.width() + 'px ' + $selectedThumb.height() + 'px')
            .css('width', ctStartW + 'px')
            .css('height', ctStartH + 'px')
            .css('left', ctStartX + 'px')
            .css('top', ctStartY + 'px');
    }
})(jQuery);
