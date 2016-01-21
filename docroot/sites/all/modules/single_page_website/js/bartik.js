(function($){
  $(document).ready(function(){
    $(".content .node h2 a").empty();
    var header_element = Drupal.settings.single_page.header_element;
    var footer_element = Drupal.settings.single_page.footer_element;
    var menu_element = Drupal.settings.single_page.menu_element;
    var content_element = Drupal.settings.single_page.content_element;
    $(header_element).css({
      'position' : 'fixed',
      'top' : '0',
      'width' : '100%',
      'z-index' : '500'
    });
    $(footer_element).css({
      'position' : 'fixed',
      'bottom' : '0',
      'width' : '100%',
      'z-index' : '500'
    });
    var header_height = parseInt($(header_element).height()) + parseInt($(header_element).css("padding-top")) + parseInt($(header_element).css("padding-bottom"));
    var footer_height = parseInt($(footer_element).height()) + parseInt($(footer_element).css("padding-top")) + parseInt($(footer_element).css("padding-bottom"));
    var window_height = $(window).height();
	$(".single_page_wrapper, #messages").css("padding-top",header_height + 20); // strange behavior of header height, I have added 20px to fix it
    $(".single_page_wrapper .single_page").each(function(index) {
      var content_height = $(this).find("#content").height();
	  var view_height = $(this).find(".view-content").height();
      var count = $(".single_page_wrapper .single_page").length
      if(content_height + view_height < window_height - header_height - footer_height){
        if(count == index + 1)
          $(this).parent().height(window_height-header_height);
        else
          $(this).parent().height(window_height-header_height-footer_height);
      }
    });
   
    $("#main > #breadcrumb").remove();
    $("#main > #sidebar-first").remove();
    $(".tabs").remove();
    $(".contextual-links-wrapper").remove();
    $("a#main-content").remove();
    
    while( $("#single_page_wrapper").parent().attr('id') != "main" ) {
      $("#single_page_wrapper").unwrap();
    }
   
    $("#single_page_wrapper > *").unwrap();
   
  });
})(jQuery);