/**
  * Javascript overrides.
  */

(function($) {

Drupal.behaviors.fusion_slateFirstWord = {
  attach:function (context, settings) {
    $('#site-name a').each(function(){
      var bt = $(this);
      bt.html(bt.text().replace(/(^\w+)/,'<span class="first-word">$1</span>'));
    });
    $('.title-dual h2.block-title').each(function(){
      var bt = $(this);
      bt.html(bt.text().replace(/(^\w+)/,'<span class="first-word">$1</span>'));
    });
    $('.title-white-bold-first h2.block-title').each(function(){
      var bt = $(this);
      bt.html(bt.text().replace(/(^\w+)/,'<span class="first-word">$1</span>'));
    });
  }
};

})(jQuery);
