(function($){
  $(document).ready(function(){
    var header_elemint = Drupal.settings.single_page.header_element;
    var footer_element = Drupal.settings.single_page.footer_element;
    var menu_element = Drupal.settings.single_page.menu_element;
    var content_element = Drupal.settings.single_page.content_element;
    $(header_elemint).css({
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
    //if ($.browser.webkit) {
    //	$(".single_page_wrapper h2").css("padding-top",parseInt($(".single_page_wrapper h2").css("margin-top"))); // fix webkit anchor issue
    //}
    var header_height = parseInt($(header_elemint).height()) + parseInt($(header_elemint).css("padding-top")) + parseInt($(header_elemint).css("padding-bottom"));
    var footer_height = parseInt($(footer_element).height()) + parseInt($(footer_element).css("padding-top")) + parseInt($(footer_element).css("padding-bottom"));
    var menu_height = parseInt($(menu_element).height()) + parseInt($(menu_element).css("padding-top")) + parseInt($(menu_element).css("padding-bottom"));
    var window_height = $(window).height();
    //	var h2_height = parseInt($(".single_page_wrapper h2").height()) + parseInt($(".single_page_wrapper h2").css("margin-top")) + parseInt($('.single_page_wrapper h2').css("margin-bottom"));
    //	$(header_elemint).parent().css("margin-top", header_height);
    $(".single_page_wrapper").css("padding-top",header_height+menu_height);
    //	$(".single_page_wrapper .single_page").height(window_height-header_height-footer_height-h2_height);
    if ($(footer_element).length > 0){
      $(".single_page_wrapper .single_page").height(window_height-header_height-footer_height-menu_height-footer_height);
    }
    else{
      $(".single_page_wrapper .single_page").height(window_height-header_height-footer_height-menu_height);
    }
    $(".single_page_wrapper").height(window_height-header_height);
    //$(content_element).children("div:not(#content)").hide();
    $(".sidebar").hide();
    $(".sidebar-first #content .section").css("padding-left", "0");
    $(menu_element).css({
      'position' : 'fixed',
      'top' : header_height,
      'width' : '100%',
      'z-index' : '500'
    });
  });
})(jQuery);
