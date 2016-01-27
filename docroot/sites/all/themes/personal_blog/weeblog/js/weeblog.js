(function ($) {
Drupal.behaviors.weeBlogAction = {
  attach: function (context) {
    $('#social-share-wrapper').each(function(){
      var social_share = $(this);
      var top = $this.height() + social_share.position().top;
      social_share.css({top: top});
    });
    $('.btn-backtotop').smoothScroll({
      speed: 350
    });
      
  }
}
});