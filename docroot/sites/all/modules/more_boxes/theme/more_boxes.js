(function($) {
  Drupal.behaviors.imceBoxes = {
    attach: function(context) {
      $('.imce-browser-ph', context).click(function(){
        window.open(Drupal.settings.basePath+'imce?app=myApp|url%40edit-imce-image-path', '', 'width=760,height=560, resizable=1', '', '', '', 'ff');
      });
    }
  }
  
  Drupal.behaviors.imageGallery = {
    attach: function(context, settings) {
      
      // Some constants:
      // @TODO: Get these from settings.
      var maxPaged = 10;
      var pagedWidth = 17;
      
      var numPics = new Array();
      var clicksNext = new Array();
      var maxClicks = new Array();
      var margin = new Array();
      
        
      $('.block-boxes-node_images_box').each(function(index) {

        // Create the Image Gallery

        // Show only the first image (the others are hidden by css)
        $('.field-item:first-child img', this).show();
        // Add html for the caption and pager
        var captionBox = '<div class="gallery-caption"></div>';
        var pagerBox = '<div class="gallery-pager"></div>';
        $('.boxes-box-content', this).append(captionBox);
        $('.boxes-box-content', this).append(pagerBox);
        $('.gallery-caption', this).html($('.field-item:first-child img', this).attr('alt'));
        var pager = '';
        $('.field-item', this).each(function(pagerIndex) {
            if (pagerIndex == 0) {
              pager = pager + '<div class="pager-item active" id="' + index + '-0"></div>';
            }
            else {
              pager = pager + '<div class="pager-item" id="' + index + '-' + pagerIndex + '"></div>';
            }
        });
        $('.gallery-pager', this).html(pager);

        // Deal with over 10 items in the pager 
        numPics[index] = $(".field-item", this).length;
        if (numPics[index] > maxPaged) {
          $('.gallery-pager', this).wrap('<div class="scrolled-gallery" />');
          $('.gallery-pager', this).wrap('<div class="gallery-wrapper" />');
          $('.scrolled-gallery', this).prepend('<div class="prev inactive">&gt;</div>');
          $('.scrolled-gallery', this).prepend('<div class="next">&lt;</div>');
        }
        
        
        clicksNext[index] = 0;
        maxClicks[index] = numPics[index] - maxPaged + 2;
        margin[index] = 0;
      });
      
      // function for finding out the index of a gallery on this page
      function galleryIndex(id) {
        var returnMe = '';
        $('.block-boxes-node_images_box').each(function(index) {
          if ($(this).attr('id') == id) {
           returnMe = index;
          }
        });
        return returnMe;
      }
      

        // Click-on-pager functionality
        $('.block-boxes-node_images_box .gallery-pager .pager-item').click(function(e){
          $(this).siblings().removeClass('active');
          $(this).addClass('active');
          var id = $(this).attr('id');
          var idSplit = id.split('-');
          var numId = Number(idSplit[1]) + 1;
          // give the images' container div the next image's height - to prevent jumping in chrome
          var imgHeight = $(this).parents('.block-boxes-node_images_box').find('.field-item:nth-child(' + numId + ') img').height();
          $(this).parents('.block-boxes-node_images_box').find('.field-items').height(imgHeight);
          $(this).parents('.block-boxes-node_images_box').find('.field-item img').hide();
          $(this).parents('.block-boxes-node_images_box').find('.field-item:nth-child(' + numId + ') img').show();
          var imgAlt = $(this).parents('.block-boxes-node_images_box').find('.field-item:nth-child(' + numId + ') img').attr('alt');
          $(this).parents('.block-boxes-node_images_box').find('.gallery-caption').html(imgAlt);
        });


        // Click-on-next functionality
        $('.block-boxes-node_images_box .scrolled-gallery .next').click(function(e){
          var index = galleryIndex($(this).parents('.block-boxes-node_images_box').attr('id'));
          if (clicksNext[index] < maxClicks[index]) {
            clicksNext[index]++;
            margin[index] = margin[index] - pagedWidth;
            $(this).siblings('.gallery-wrapper').children('.gallery-pager').css('margin-left', margin[index]);
            $(this).siblings('.prev').removeClass('inactive');
          }
          if (clicksNext[index] == maxClicks[index]) {
            $(this).addClass('inactive');
          }
        });

        // Click-on-prev functionality
        $('.block-boxes-node_images_box .scrolled-gallery .prev').click(function(e){
          var index = galleryIndex($(this).parents('.block-boxes-node_images_box').attr('id'));
          if (clicksNext[index] + maxPaged - 2 <= numPics[index] && clicksNext[index] > 0) {
            clicksNext[index]--;
            margin[index] = margin[index] + pagedWidth;
            $(this).siblings('.gallery-wrapper').children('.gallery-pager').css('margin-left', margin[index]);
            $(this).siblings('.next').removeClass('inactive');
          }
          if (clicksNext[index] == 0) {
            $(this).addClass('inactive');
          }
        });


    }
  }
  
  Drupal.behaviors.moreBoxMinimise = {
    attach: function(context) {    
      $(".form-item-body-format").append('<a class="fieldset-legend">Tips</a>'); 
      $(".form-item-body-format a").toggle(function(){
        $("#edit-body-format-guidelines").slideDown(200); 
      }, 
      function(){
        $("#edit-body-format-guidelines").slideUp(200); 
      });   
    }
  }

  // DUPLICATE AND UNCOMMENT
  // Drupal.behaviors.behaviorName = {
  //  attach: function(context) {
  //    // Do some magic...
  //  }
  // };


})(jQuery);
