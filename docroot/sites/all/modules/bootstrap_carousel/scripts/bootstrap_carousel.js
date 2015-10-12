/**
 * @file
 * Apply Bootstrap Carousel plugin.
 */

 (function ($) {
  
  $(document).ready(function(){
    
    // Defining "player" as a global variable - is an array of elements videos
    player = {};
    
    applyCarousel();
    applyYouTubePlayer();
    

  });
  
  function pauseCarouselMouseOverIframe(){
    
    $('.carousel .item.active, .carousel iframe.ytplayer').mouseover(function() {
      $('.carousel').carousel('pause');
    }).mouseout(function() {
      var idVideo = $('.carousel .item.active iframe.ytplayer').attr('id');
      
      if (typeof(idVideo) != 'undefined') {
        var stateVideo = getStatePlayer(idVideo);
        if (stateVideo != 1) {
          $('.carousel').carousel('cycle');
        }
      }
      else {
        $('.carousel').carousel('cycle');
      }
      
    });
    
  }
  
  function applyYouTubePlayer() {
    if (typeof(YT) != 'undefined' && typeof(YT.Player) != 'undefined') {
      youTubeEmbeddedPlayer();
    } 
    else {
      window.onYouTubeIframeAPIReady = function() {
        youTubeEmbeddedPlayer();
      };
    }
    
  }
    
  function youTubeEmbeddedPlayer(){
    
    $('div.video-container div.ytplayer').each(function() {
    
      var idElememt = $(this).attr('id');
      var idVideo = $(this).data('videoid');
      
      // Replace the 'ytplayer+idElement' element with an <iframe> and
      // YouTube player after the API code downloads.
      player[idElememt] = new YT.Player(idElememt, {
        height: '315',
        width: '560',
        videoId: idVideo,
        events: {
          'onReady': onPlayerReady
        },
        playerVars: {
          wmode: "opaque"
        }
      });

    });
    
    pauseCarouselMouseOverIframe();
    
  }
  
  function onPlayerReady(event) {
    event.target.addEventListener('onStateChange', onPlayerStateChange);    
  }
    
  function onPlayerStateChange(event) {
    if (event.data == YT.PlayerState.PLAYING) {
        $('.carousel').carousel('pause');
    }
  }
  
  function applyCarousel(){
    
    $('.carousel').carousel({
      interval: Drupal.settings.bootstrap_carousel.interval,
      pause: Drupal.settings.bootstrap_carousel.pause
    }).on('slide', function(event) {
      // if exist video player in current item
      var idItemCurrent = $(this).parent().find("div.item.active iframe.ytplayer").attr('id');
      
      if (typeof(idItemCurrent) != 'undefined') {
        // pause video
        pauseVideoWhenChangeSlide(idItemCurrent);
        // hide video
        $("#"+idItemCurrent).css("display", "none");
      }
    }).on('slid', function(event) {
      // if exist video player in next item
      var idItemDisabled = $(this).parent().find("div.item.active iframe.ytplayer").attr('id');
      
      if (typeof(idItemDisabled) != 'undefined') {
        //display video
        $("#"+idItemDisabled).css("display", "block");
      }
    });
        
  }
  
  
  function pauseVideoWhenChangeSlide(idItemCurrent) {
        
    var stateVideo = getStatePlayer(idItemCurrent);
        
    // If stateVideo == Playing then Pause Video
    if (stateVideo == 1) {
       var videoCurrent = player[idItemCurrent];
       videoCurrent.pauseVideo();
    }
    
  }
  
  function getStatePlayer(idElement) {
    
    var video = player[idElement];
    /**
     * getPlayerState(): Numeric
     * Returns the state of the player. Possible values:
     *   unstarted (-1), 
     *   ended (0), 
     *   playing (1), 
     *   paused (2), 
     *   buffering (3), 
     *   video cued (5).
     **/
    if (undefined !== video.getPlayerState) {
      return video.getPlayerState();
    }
    
  }
  
})(jQuery);
