(function($){
  $(document).ready(function(){
    $(".single_page_wrapper").css("clear", "both");
    $(".single_page_wrapper .single_page").css("overflow", "inherit");
    var header_element = Drupal.settings.single_page.header_element;
    var footer_element = Drupal.settings.single_page.footer_element;
    var menu_element = Drupal.settings.single_page.menu_element;
    var content_element = Drupal.settings.single_page.content_element;
    var easing = Drupal.settings.single_page.easing;

    var target_offset = $(".single_page_wrapper:first").offset();
    var target_top = target_offset.top;
    var anchor = location.hash;

    $(menu_element + " > li > a").click(function(event){

      //prevent the default action for the click event
      event.preventDefault();

      //get the full url - like mysitecom/index.htm#home
      var full_url = this.href;

      //split the url by # and get the anchor target name - home in mysitecom/index.htm#home
      var parts = full_url.split("#");

      var trgt;
      if(parts[1].indexOf('/')!=-1){
      	var pretarg = parts[1].split("/");
      	trgt = pretarg[pretarg.length - 1];
      }else{
      	trgt = parts[1];
  	  }

      //get the top offset of the target anchor
      var target_offset = $("#"+trgt).offset();
      var target_top = target_offset.top;

      //goto that anchor by setting the body scroll top to anchor top
      if(easing == 'none') {
        $('html, body').animate({
          scrollTop:target_top
        }, 500);
      }
      else {
        $('html, body').animate({
          scrollTop:target_top
        }, {
          duration: 1000,
          easing: easing
        });
      }
    });
  });
})(jQuery);