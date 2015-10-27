(function ($, Drupal, PhotoSwipe, PhotoSwipeUI_Default) {
  Drupal.behaviors.photoswipe = {
    /**
     * PhotoSwipe Options, coming from Drupal.settings.
     */
    photoSwipeOptions: {},
    /**
     * Instantiated galleries.
     */
    galleries: [],
    /**
     * Load PhotoSwipe once page is ready
     */
    attach: function (context, settings) {
      this.photoSwipeOptions = settings.photoswipe ? settings.photoswipe.options : {};

      var $galleries = $('.photoswipe-gallery', context);
      if ($galleries.length) {
        // loop through all gallery elements and bind events
        $galleries.each( function (index) {
          var $gallery = $(this);
          $gallery.attr('data-pswp-uid', index + 1);
          $gallery.on('click', Drupal.behaviors.photoswipe.onThumbnailsClick);
        });
      }
      var $imagesWithoutGalleries = $('a.photoswipe', context).filter(function(elem) {
        return !$(this).parents('.photoswipe-gallery').length;
      });
      if ($imagesWithoutGalleries.length) {
        // We have no galleries just individual images.
        $imagesWithoutGalleries.each(function(index) {
          $imageLink = $(this);
          $imageLink.wrap('<span class="photoswipe-gallery"></span>');
          var $gallery = $imageLink.parent();
          $gallery.attr('data-pswp-uid', index + 1);
          $gallery.on('click', Drupal.behaviors.photoswipe.onThumbnailsClick);
          $galleries.push($gallery);
        });
      }

      // Parse URL and open gallery if it contains #&pid=3&gid=1
      var hashData = this.parseHash();
      if(hashData.pid > 0 && hashData.gid > 0) {
        this.openPhotoSwipe(hashData.pid - 1 ,  $($galleries[hashData.gid - 1]));
      }
    },
    /**
     * Triggers when user clicks on thumbnail.
     *
     * Code taken from http://photoswipe.com/documentation/getting-started.html
     * and adjusted accordingly.
     */
    onThumbnailsClick: function(e) {
      e = e || window.event;
      e.preventDefault ? e.preventDefault() : e.returnValue = false;

      var $clickedGallery = $(this);

      var eTarget = e.target || e.srcElement;
      var $eTarget = $(eTarget);

      // find root element of slide
      var clickedListItem = $eTarget.closest('.photoswipe');

      if (!clickedListItem) {
        return;
      }

      // get the index of the clicked element
      index = clickedListItem.index('.photoswipe');
      if (index >= 0) {
        // open PhotoSwipe if valid index found
        Drupal.behaviors.photoswipe.openPhotoSwipe(index, $clickedGallery);
      }
      return false;
    },
    /**
     * Code taken from http://photoswipe.com/documentation/getting-started.html
     * and adjusted accordingly.
     */
    openPhotoSwipe: function(index, galleryElement, options) {
      var pswpElement = $('.pswp')[0];
      var items = [];
      options = options || Drupal.behaviors.photoswipe.photoSwipeOptions;

      var images = galleryElement.find('a.photoswipe');
      images.each(function (index) {
        var $image = $(this);
        size = $image.data('size') ? $image.data('size').split('x') : ['',''];
        items.push(
          {
            src : $image.attr('href'),
            w: size[0],
            h: size[1],
            title : $image.data('overlay-title')
          }
        );
      })

      // define options
      options.index = index;
      // define gallery index (for URL)
      options.galleryUID = galleryElement.data('pswp-uid');

      // Pass data to PhotoSwipe and initialize it
      var gallery = new PhotoSwipe(pswpElement, PhotoSwipeUI_Default, items, options);
      gallery.init();
      this.galleries.push(gallery);
    },
    /**
     * Parse picture index and gallery index from URL (#&pid=1&gid=2)
     *
     * Code taken from http://photoswipe.com/documentation/getting-started.html
     * and adjusted accordingly.
     */
    parseHash: function() {
      var hash = window.location.hash.substring(1),
      params = {};

      if (hash.length < 5) {
        return params;
      }

      var vars = hash.split('&');
      for (var i = 0; i < vars.length; i++) {
        if (!vars[i]) {
          continue;
        }
        var pair = vars[i].split('=');
        if (pair.length < 2) {
          continue;
        }
        params[pair[0]] = pair[1];
      }

      if(params.gid) {
        params.gid = parseInt(params.gid, 10);
      }

      if(!params.hasOwnProperty('pid')) {
        return params;
      }
      params.pid = parseInt(params.pid, 10);

      return params;
    }
  };
})(jQuery, Drupal, PhotoSwipe, PhotoSwipeUI_Default);
