(function($) {

  Drupal.behaviors.imageHotspotEdit = {
    attach: function(context, settings) {

      function deleteEmptyItems($hotspotDataDiv) {
        $hotspotDataDiv.find('.image-hotspot-data-item').each(function() {
          if (typeof $(this).attr('x1') === 'undefined') {
            $(this).remove();
          }
        });
      }

      function collectDataItems($editContainer, $hotspotDataDiv, $hotspotImg) {
        // Collect data from all items.
        var $coordInput = $editContainer.find('.image-hotspot-coordinates input');
        var $hotspotItems = $hotspotDataDiv.find('.image-hotspot-data-item');
        var hotspotData = new Array();

        $hotspotItems.each(function() {
          var hotspotLink = ($(this).find('.img-hotspot-link').val()) ? $(this).find('.img-hotspot-link').val().replace(/"/g, '\'') : '';
          if ($(this).attr('x1')) {
            hotspotData.push({
              'title': $(this).find('.img-hotspot-title').val().replace(/"/g, '\''),
              'description': $(this).find('.img-hotspot-descr').val().replace(/"/g, '\''),
              'link': hotspotLink,
              'x1': Math.round(100 / ($hotspotImg.width() / $(this).attr('x1'))),
              'x2': Math.round(100 / ($hotspotImg.width() / $(this).attr('x2'))),
              'y1': Math.round(100 / ($hotspotImg.height() / $(this).attr('y1'))),
              'y2': Math.round(100 / ($hotspotImg.height() / $(this).attr('y2')))
            });
          }
        });
        if (hotspotData.length > 0) {
          $coordInput.val($.toJSON(hotspotData));
        }
        else {
          $coordInput.val('');
        }
      }

      $('.image-hotspot-edit:not(.with-jcrop)', context).each(function() {
        // Main data.
        var $hotspotImg = $(this).find('.image-hotspot-img img');
        var $hotspotInputs = $(this).find('.image-hotspot-inputs');
        var $hotspotAddBtn = $(this).find('.image-hotspot-add');
        var $hotspotDataDiv = $(this).find('.image-hotspot-data');
        var $existHotspots = $hotspotDataDiv.find('.image-hotspot-data-item');
        var $editContainer = $(this);

        // Minimal and Maximal crop resolution.
        var JcropSettings = {
          minSize: [parseInt(settings.image_hotspot_settings.min_width), parseInt(settings.image_hotspot_settings.min_height)],
          maxSize: [parseInt(settings.image_hotspot_settings.max_width), parseInt(settings.image_hotspot_settings.max_height)]
        };

        // Initial Jcrop.
        var jcrop = $.Jcrop($hotspotImg, JcropSettings);
        $(this).addClass('with-jcrop');

        // Hotspots form.
        if ($existHotspots.length > 0) {
          $hotspotDataDiv.find('.img-hotspot-data-wrapper').hide();
        }
        else {
          $hotspotDataDiv.html($hotspotInputs.html());
        }
        $hotspotDataDiv.append($hotspotAddBtn.html());

        // Add another hotspot.
        $(this).find('.img-hotspot-add').click(function() {
          deleteEmptyItems($hotspotDataDiv);
          $hotspotDataDiv.find('.img-hotspot-data-wrapper').hide();
          $(this).before($hotspotInputs.html());
          jcrop.setSelect([0, 0, 0, 0]);
          jcrop.release();
          return false;
        });

        // Save hotspots coordinates.
        $hotspotDataDiv.delegate('.img-hotspot-save', 'click', function() {
          // Current item changes.
          var selection = jcrop.tellSelect();
          var $hotspotTitle = $(this).parent().find('.img-hotspot-title');

          if ($hotspotTitle.val() === '') {
            alert(Drupal.t('Hotspot title field is required.'));
          }
          else if (selection.h > 0) {
            var $hotspotItem = $(this).parent().parent();
            $hotspotItem.attr({
              'x1': selection.x,
              'x2': selection.x2,
              'y1': selection.y,
              'y2': selection.y2
            });
            $(this).parent().hide().prev().text($hotspotTitle.val());
            jcrop.release();
            collectDataItems($editContainer, $hotspotDataDiv, $hotspotImg);
          }
          else {
            alert(Drupal.t('Please select an area on the image.'));
          }
          return false;
        });

        // Select hotspot.
        $hotspotDataDiv.delegate('.img-hotspot-data-title', 'click', function() {
          var $item = $(this).parent();
          jcrop.setSelect([$item.attr('x1'), $item.attr('y1'), $item.attr('x2'), $item.attr('y2')]);
          deleteEmptyItems($hotspotDataDiv);
          $hotspotDataDiv.find('.img-hotspot-data-wrapper').hide();
          $(this).next().show();
          return false;
        });

        // Remove hotspot.
        $hotspotDataDiv.delegate('.img-hotspot-remove', 'click', function() {
          $(this).parent().parent().remove();
          jcrop.release();
          collectDataItems($editContainer, $hotspotDataDiv, $hotspotImg);
          return false;
        });
      });
    }
  }

  Drupal.behaviors.imageHotspotView = {
    attach: function(context, settings) {
      if (settings.imageHotspots) {
        $('.image-hotspot:not(.init)', context).each(function() {
          $(this).addClass('init').parents('.field-item').wrapInner('<div class="image-hotspot-wrapper">');
          var imageId = $(this).attr('id').split('-');
          var fid = 'fid' + imageId[2];
          var hotspots = eval('settings.imageHotspots.' + fid);

          if (hotspots) {
            var imageWidth = $(this).width();
            var imageHeight = $(this).height();
            var imageSrc = $(this).attr('src');
            var hotspotTitles = '';
            var hotspotIndex = 0;
            var classIndex = 0;
            var comma = ', ';
            var $imageWrap = $(this).parent();
            var styles = Drupal.settings.imageHotspotsColor;

            $.each(hotspots, function(i, hotspot) {
              // Hotspot item settings.
              if (i % styles.length === 0) {
                classIndex = 0;
              }
              if (i === (hotspots.length - 1)) {
                comma = '';
              }
              classIndex++;
              hotspotIndex++;
              var hotpostSpan = (hotspot.link != '') ? '<a href="' + hotspot.link + '">' + hotspot.title + '</a>' : hotspot.title;
              hotspotTitles += '<span class="hotspot-title" hotspot="' + hotspotIndex + '">' + hotpostSpan + '</span>' + comma;
              var hotpostWidth = imageWidth / (100 / (hotspot.x2 - hotspot.x1));
              var hotpostHeight = imageHeight / (100 / (hotspot.y2 - hotspot.y1));
              var hotpostTop = Math.round(imageHeight / (100 / hotspot.y1));
              var hotpostLeft = Math.round(imageWidth / (100 / hotspot.x1));
              var style = styles[classIndex-1];
              var border = 'none';
              var backgroundColor = (style['background-color'] == undefined) ? 'transparent' : '#' + style['background-color'];
              var opacity = style['opacity'];
              if (typeof style['border'] === "object") {
                border = style['border']['width'] + 'px ' + style['border']['style'] + ' #' + style['border']['color'];
              }
              // Build hotspot item.
              var $hotspotDiv = $('<div />', {
                'img-hotspot-title': hotspot.title,
                'img-hotspot-descr': hotspot.description,
                'class': 'img-hotspot-box'
              });
              $hotspotDiv.css({
                'top': hotpostTop + 'px',
                'left': hotpostLeft + 'px',
                'width': hotpostWidth + 'px',
                'height': hotpostHeight + 'px'
              });
              $hotspotDiv.append($('<div />', {'class': 'img-hotspot-highlight hotspot-highlight-' + hotspotIndex}).css({
                'background-image': 'url(' + imageSrc + ')',
                'background-position': -hotpostLeft + 'px' + ' ' + -hotpostTop + 'px'
              }));
              $hotspotDiv.append($('<div />', {'class': 'img-hotspot'}).css({
                'background-color': backgroundColor,
                'opacity': opacity,
                'border': border
              }));
              // Add hotspot item.
              $imageWrap.append($hotspotDiv);

              if (hotspot.link != '') {
                $hotspotDiv.wrap($('<a />', {'href': hotspot.link}));
              }
            });
            // Titles after image.
            $(this).parents('.image-hotspot-wrapper').append($('<div />', {'class': 'hotspot-titles'}).html(hotspotTitles));
            // Overlay for highlighs.
            $(this).after($('<div />', {'class': 'hotspot-overlay'}).css({
              'width': imageWidth + 'px',
              'height': imageHeight + 'px'
            }));
          }
        });
        // Tooltips.
        $('.img-hotspot-box', context).each(function() {
          var title = $(this).attr('img-hotspot-title');
          var descr = $(this).attr('img-hotspot-descr');
          var data = '<div class="img-hotspot-title">' + title + '</div>';
          if (descr !== '') {
            data += '<div class="img-hotspot-description">' + descr + '</div>';
          }
          $(this).tipTip({
            content: data,
            activation: 'hover',
            keepAlive: false
          });
        });
        // Highlights.
        $('.hotspot-title', context).hover(
          function() {
            var hotspotIndex = $(this).attr('hotspot');
            var $wrapper = $(this).parents('.image-hotspot-wrapper');
            var $hotspotHighlight = $wrapper.find('.hotspot-highlight-' + hotspotIndex);
            var $overlay = $wrapper.find('.hotspot-overlay');
            $hotspotHighlight.show();
            $overlay.show();
            $('.img-hotspot').hide();
          },
          function() {
            var hotspotIndex = $(this).attr('hotspot');
            $wrapper = $(this).parents('.image-hotspot-wrapper');
            $hotspotHighlight = $wrapper.find('.hotspot-highlight-' + hotspotIndex);
            $overlay = $wrapper.find('.hotspot-overlay');
            $hotspotHighlight.hide();
            $overlay.hide();
            $('.img-hotspot').show();
          }
        );
      }
    }
  }

})(jQuery);
