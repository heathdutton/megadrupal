(function($){
  $(document).ready(function(){
    $(".single_page_wrapper").css("clear", "both");
    $(".single_page_wrapper .single_page").css("overflow", "inherit");
    var header_elemint = Drupal.settings.single_page.header_element;
    var footer_element = Drupal.settings.single_page.footer_element;
    var menu_element = Drupal.settings.single_page.menu_element;
    var content_element = Drupal.settings.single_page.content_element;
    var easing = Drupal.settings.single_page.easing;
    $(menu_element + " li:first").addClass("active");
    $(menu_element + " li:first a").addClass("active");
    var target_offset = $(".single_page_wrapper:first").offset();
    var target_top = target_offset.top;
    $('html, body').animate({
      scrollTop:target_top
    }, 500);
    var anchor = location.hash;
    if(anchor != ""){
      var current_item = $(anchor);
      var index = $("div.single_page_wrapper").index(current_item);
      $(menu_element + " li").removeClass("active");
      $(menu_element + " li a").removeClass("active");
      $(menu_element + " li:eq(" + index + ")").addClass("active");
      $(menu_element + " li:eq(" + index + ") a").addClass("active");
      var target_offset = $(anchor).offset();
      var target_top = target_offset.top;
      $('html, body').animate({
        scrollTop:target_top
      }, 500);
    }
    $(menu_element+" a").click(function(event){
      $(menu_element + " li").removeClass("active");
      $(menu_element + " li a").removeClass("active");
      $(this).addClass("active");
      $(this).parent().addClass("active");
      $(this).addClass("active");
      //prevent the default action for the click event
      event.preventDefault();

      //get the full url - like mysitecom/index.htm#home
      var full_url = this.href;

      //split the url by # and get the anchor target name - home in mysitecom/index.htm#home
      var parts = full_url.split("#");
      var trgt = parts[1];

      //get the top offset of the target anchor
      var target_offset = $("#"+trgt).offset();
      var target_top = target_offset.top;
      //window.location.hash = "#"+trgt.replace(" ", "_");
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