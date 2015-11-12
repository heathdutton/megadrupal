/**
 * @file facebook_album.js
 * The plugin that retrieves the information from the facebook album module.
 *
 * Version: 2.x
 * Drupal Core: 7.x
 *
 */

(function($) {

  $(document).ready(function(){

    var cbOptions        = {};
    var paging           = {};
    var firstBatch       = true;
    var albumInitialized = false;
    var loading          = false;
    var isWebkit         = false;

    var albumContainer   = $('#facebook-album-images-container');


    /*
     * Parse the color box settings into an array
     */
    if(Drupal.settings.facebookAlbum.colorboxOptions != "") {
      var cbProps = ("rel:'fbGal', maxWidth:'95%', maxHeight:'95%', " + Drupal.settings.facebookAlbum.colorboxOptions).split(',');
    }
    else {
      var cbProps = ("rel:'fbGal', maxWidth:'95%', maxHeight:'95%'").split(',');
    }

    for(var i = 0; i < cbProps.length; i++){
      var prop = cbProps[i].split(':');
      cbOptions[prop[0].replace(/ /g,'')] = prop[1].replace(/\"|\'/g, "");
    }


    if(navigator.userAgent.indexOf('AppleWebKit') != -1) {
      isWebkit = true;
    }


    /*
     * Handle click events on a given album cover photo.
     * Reset container content and loadPhotos for the respective album
     */
    $('#facebook-album').on('click', '.album-wrapper', function(e) {

      e.preventDefault();

      fadeContainerOut();

      var albumLink        = $(this).children('.fb-link').data('album-link');
      var albumDescription = $(this).data('description');
      var albumName        = $(this).data('name');
      var albumLocation    = $(this).data('location');
      paging['link']       = albumLink;

      var header = '<span id="fb-back-link" class="fb-link" href="#">&laquo ' + Drupal.t('Back to Albums') + '</span> - <span>';
      header    += '<span>' + albumName + '</span>';

      if (albumLocation.length) {
        header += '<p>' + Drupal.t('Taken in:') + albumLocation + '</p>';
      }

      header += '<p>' + albumDescription + '</p>';

      $('#fb-album-header').append(header);
      albumContainer.html('');
      albumContainer.removeClass('album-covers');
      albumContainer.addClass('album-photos');

      displayLoadingIcon(true);
      loadAlbumPhotos( albumLink, null );
    });


    /*
     * Handle the back button click action
     */
    $('#facebook-album').on('click', '#fb-back-link', function(e) {
      e.preventDefault();

      initializeAlbums();
    });


    /*
     * Handle the infinity scrolling functionality on the photos
     * When a scroll down past the currently loaded images
     * is detected, load the next set of photos if there are any
     */
    $(window).scroll(function(e){

      var scroll_y_pos = window.pageYOffset + window.innerHeight;
      var containerPosition = albumContainer.offset().top + albumContainer.outerHeight(true);

      if ((scroll_y_pos > containerPosition || scroll_y_pos == containerPosition)) {

        if (paging['after'] != null && !loading && !firstBatch) {

          displayLoadingIcon(true);
          loading = true;

          if (albumContainer.hasClass('album-photos')) {
            loadAlbumPhotos(paging['link'], paging['after']);
          }
          else if (albumContainer.hasClass('album-covers')) {
            loadAlbums(paging['after']);
          }
        }
      }
    });


    function displayLoadingIcon(show) {

        if (show) {
          var icon = '<div class="spinner">';
          icon += '<div class="rect1"></div>';
          icon += '<div class="rect2"></div>';
          icon += '<div class="rect3"></div>';
          icon += '<div class="rect4"></div>';
          icon += '<div class="rect5"></div>';
          icon += '</div>';

          $('.fb-loading-icon').html(icon);
        }
        else {
          $('.fb-loading-icon').html('');
        }
    }


    /*
     * Load all the albums and reset any variables used while loading photos
     * within a given album
     */
    function initializeAlbums() {
      loading = false;
      firstBatch = true;

      albumContainer.html('');
      albumContainer.removeClass('album-photos');
      albumContainer.addClass('album-covers');
      $('#fb-album-header').html('');

      displayLoadingIcon(true);
      loadAlbums(null);
    }


    function fadeContainerOut() {
      albumContainer.fadeOut(500);
    }


    /*
     * Load album content and loop through each photo container
     * fetching the album cover asynchronously to save load times
     */
    function loadAlbums(after) {

      var postfix = '';
      var url_prefix = Drupal.settings.basePath + Drupal.settings.pathPrefix;

      if (after != null) {
        postfix = '/next/' + after;
      }

      $.ajax({
        url: url_prefix + 'facebook_album/get/albums' + postfix,
        dataType: 'json',
        success: function(response) {

          if (response.data != null && response.data.content != null) {

            if (firstBatch && albumInitialized) {
              scrollToAlbum();
              albumInitialized = true;
            }

            firstBatch = false;

            if (albumContainer.hasClass('album-covers')) {
              albumContainer.append(response.data.content);
              albumContainer.fadeIn(500);
            }

            handlePaging(response);
            loading = false;
            displayLoadingIcon(false);

            $('#facebook-album .album-thumb-wrapper i.unloaded').each(function () {
              loadAlbumCover($(this));
            });
          }
        }
      });
    }


    function scrollToAlbum() {

      $( isWebkit ? 'body' : 'html').animate({
        scrollTop: $("#facebook-album").offset().top - 50
      }, 1000);
    }


    /*
     * Fetch the album cover and set the image container's background to the result
     */
    function loadAlbumCover(image) {

      var url_prefix = Drupal.settings.basePath + Drupal.settings.pathPrefix;

      $.ajax({
        url: url_prefix + 'facebook_album/get/album/' + image.data('cover-id') + '/cover',
        dataType: 'json',
        success: function(response) {
          setImage(response, image);
        }
      });
    }



    function setImage(response, image) {
      if (response.data != null && response.data.url != null) {
        image.css('background-image', 'url(' + response.data.url + ')');
        image.removeClass('unloaded');
        image.parent().parent('a').attr('href', response.data.url);
      }
    }


    function setName(response, image) {
      if (response.data != null && response.data.name != null) {
        image.attr('title', response.data.name);
        image.parent().parent('a').attr('title', response.data.name);
      }
    }



    function loadPhotoUrl(image) {

      var url_prefix = Drupal.settings.basePath + Drupal.settings.pathPrefix;
      var pid = image.attr('data-photo-id');

      $.ajax({
        url: url_prefix + 'facebook_album/get/photo/' + pid,
        dataType: 'json',
        success: function(response) {
          setImage(response, image);
          setName(response, image);
        }
      });
    }


    /*
     * Recursively load all photos within the album
     */
    function loadAlbumPhotos(albumLink, after) {

      var url = after != null ? albumLink + '/next/' + after : albumLink;
      var url_prefix = Drupal.settings.basePath + Drupal.settings.pathPrefix;

      $.ajax({
        url: url_prefix + url,
        dataType: 'json',
        success: function(response) {

          if (response.data != null && response.data.content != null) {

            if (albumContainer.hasClass('album-photos')) {
              albumContainer.append(response.data.content);
              albumContainer.fadeIn(500);

              $('.photo-thumb-wrapper i').each(function(i){
                loadPhotoUrl($(this));
              });
            }

            if (firstBatch) {
              scrollToAlbum();
              firstBatch = false;
            }

            if ($.fn.colorbox) {
              $( 'a.photo-thumb' ).colorbox( cbOptions );
            }

            handlePaging(response);
            displayLoadingIcon(false);
            loading = false;
          }
        }
      });
    }


    /*
     * Set paging variables
     */
    function handlePaging(response) {
      if (response.data.after != null ) {
        paging['after'] = response.data.after;
      }
      else {
        paging['after'] = null;
        firstBatch = true;
      }
    }

    initializeAlbums();

  });

})(jQuery);
