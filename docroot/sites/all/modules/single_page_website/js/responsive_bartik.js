(function($){
$(document).ready(function(){
    $(".content .node h2 a").empty();
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
	var header_height = parseInt($(header_elemint).height()) + parseInt($(header_elemint).css("padding-top")) + parseInt($(header_elemint).css("padding-bottom"));
	var footer_height = parseInt($(footer_element).height()) + parseInt($(footer_element).css("padding-top")) + parseInt($(footer_element).css("padding-bottom"));
	var window_height = $(window).height();
	$(".single_page_wrapper").css("padding-top",header_height);
	$(".single_page_wrapper .single_page").each(function(index) {
         var content_height = $(this).find("#content").height();
         var count = $(".single_page_wrapper .single_page").length
         if(content_height < window_height-header_height-footer_height){
	if(count == index + 1)
           $(this).parent().height(window_height-header_height);
	else
           $(this).parent().height(window_height-header_height-footer_height);
           }
       });
       //var single_page = $("#single_page_wrapper").html();
       //$(content_element).empty();
       //$(content_element).html(single_page);
   
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